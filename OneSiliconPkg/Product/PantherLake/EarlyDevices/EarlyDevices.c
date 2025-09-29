/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#include <Base.h>
#include <PiPei.h>
#include <Library/PeimEntryPoint.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <UniversalPayload/SerialPortInfo.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/LpssUartDebugPropertyPcdLib.h>
#include <Library/PeiMeLib.h>
#include <Library/MeInitLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PciIoLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PerformanceLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Library/PeiSpiDmaLib.h>

EFI_STATUS
EFIAPI
PciDeviceMemoryDiscoveryPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Builds the HOBs with HECI devices configurations.
  These HOBs are required by the HECI driver for device initialization.
**/
VOID
PtlPcdBuildHeciDeviceConfigHobs (
  VOID
  );

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdLpssUartFixedBar0Offset [] = {
  0xC000,
  0xE000,
  0x10000
};

static EFI_PEI_NOTIFY_DESCRIPTOR mReinitializePciDevicePpiNotifyList = {
(EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
&gEfiPeiMemoryDiscoveredPpiGuid,
PciDeviceMemoryDiscoveryPpiNotifyCallback
};

/**
  Init routine for HDA in early PEI phase.
  @retval EFI_SUCCESS             The function completed successfully
  @retval Other                   HDA is not ready for communication
**/
EFI_STATUS
HdaInitEarlyPei (
  VOID
  )
{
  EFI_STATUS              Status;
  EDKII_PCI_DEVICE_PPI    *HdaPpi;
  EFI_PEI_PPI_DESCRIPTOR  *HdaPpiDescriptor;

  DEBUG ((DEBUG_INFO, "%a (): Start\n", __FUNCTION__));

  CreatePciDeviceIoFromSbdf (
    0,
    0,
    PCI_DEVICE_NUMBER_PCH_HDA,
    PCI_FUNCTION_NUMBER_PCH_HDA,
    NULL,
    &HdaPpi
    );

  HdaPpiDescriptor = AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (HdaPpiDescriptor) {
    HdaPpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    HdaPpiDescriptor->Ppi   = (VOID *) HdaPpi;
    HdaPpiDescriptor->Guid  = &gEdkiiPeiPciDevicePpiGuid;
    Status = PeiServicesInstallPpi (HdaPpiDescriptor);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Status = PeiServicesNotifyPpi (&mReinitializePciDevicePpiNotifyList);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  DEBUG ((DEBUG_INFO, "%a (): End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Performs early initialization of the Management Engine (ME) components.

  This function is responsible for the early phase initialization of ME-related components.
  It installs the ME Handle, builds HECI device configuration HOBs, and installs ME HOBs.

  @retval VOID
**/
VOID
EarlyMeInit (
  VOID
  )
{
  InstallMeHandle ();
  PtlPcdBuildHeciDeviceConfigHobs ();
  InstallMeHobs ();
}

/**
  Refreshes the function pointers after the PEIM is migrated to RAM.

  @param[in]  PeiServices       Pointer to PeiServices
  @param[in]  NotifyDescriptor  Pointer to notify descriptor
  @param[in]  Ppi               Installed PPI

  @return Status of the update
**/
EFI_STATUS
EFIAPI
PciDeviceMemoryDiscoveryPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                Status;
  EFI_PEI_PPI_DESCRIPTOR    *PpiDescriptor;
  EDKII_PCI_DEVICE_PPI      *PciDevicePpi;

  PciDevicePpi = NULL;
  Status = PeiServicesLocatePpi (
             &gEdkiiPeiPciDevicePpiGuid,
             0,
             &PpiDescriptor,
             (void **) &PciDevicePpi
             );
  if (EFI_ERROR (Status) || PciDevicePpi == NULL) {
    return EFI_NOT_FOUND;
  }

  return RefreshPciDevicePointersPostMem(&PciDevicePpi);
}

/**
  EarlyDevices entry point

  @param[in]  FileHandle        Handle of the file being invoked.
  @param[in]  PeiServices       Pointer to PEI Services table.

  @return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
EarlyDevicesEntryPoint (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO  *SerialPortInfoHob;
  EFI_HOB_GUID_TYPE                   *GuidHob;
  LPSS_UART_DEVICE_CONFIG             LpssUartDeviceConfig;
  UINT8                               UartNumber;
  UINTN                               FixedMmioBaseAddress;
  EDKII_PCI_DEVICE_PPI                *SmbusPpi;
  EFI_PEI_PPI_DESCRIPTOR              *SmbusPpiDescriptor;
#if FixedPcdGetBool (PcdSpiDmaEnable) == 1
  EFI_STATUS                          Status;
  VOID                                *MemoryDiscoveredPpi;
#endif
  PERF_INMODULE_BEGIN ("EarlyDevices");
#if FixedPcdGetBool (PcdSpiDmaEnable) == 1
  //
  // This is a mitigation to avoid accesses from SPI Flash while SPI DMA is in progress.
  // We use PeiServicesRegisterForShadow API to register EarlyDevices PEIM for copy when
  // memory is available. This code causes a re-entry for EarlyDevices which re-installs
  // the SPI DMA based FirmwareVolumeShadow from SPI Flash to Memory. So we execute from
  // Memory instead of SPI Flash when SPI DMA is executing.
  //
  // This code also assumes that the re-entry of EarlyDevices happens before FvReportPei
  // executes. EDKII implemention of PeiDispatcher executes re-entry PEIM modules before
  // any other PEIM modules. If this behaviour changes we would need to redo the change.
  //
  // HSD-WA RCR requested till PTL:
  // Link to the original DPMO issue: https://hsdes.intel.com/appstore/article/#/16024140925
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiMemoryDiscoveredPpiGuid,
             0,
             NULL,
             (VOID **)&MemoryDiscoveredPpi
             );
  if (Status == EFI_NOT_FOUND) {
    Status = PeiServicesRegisterForShadow (FileHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Registering for shadow failed\n", __FUNCTION__));
    }
    ASSERT_EFI_ERROR (Status);
  } else if (Status == EFI_SUCCESS){
    DEBUG ((DEBUG_INFO, "%a: Re-entry after Memory discovered. re-install SpiDma Ppi\n", __FUNCTION__));
    SpiDmaServiceInit ();
    PERF_INMODULE_END ("EarlyDevices");
    return EFI_SUCCESS;
  }
#endif
  CreatePciDeviceIoFromSbdf (
    0,
    0,
    PCI_DEVICE_NUMBER_PCH_SMBUS,
    PCI_FUNCTION_NUMBER_PCH_SMBUS,
    NULL,
    &SmbusPpi
    );

  SmbusPpiDescriptor = AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SmbusPpiDescriptor) {
    SmbusPpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    SmbusPpiDescriptor->Ppi   = (VOID *) SmbusPpi;
    SmbusPpiDescriptor->Guid  = &gEdkiiPeiPciDevicePpiGuid;
    PeiServicesInstallPpi (SmbusPpiDescriptor);

    PeiServicesNotifyPpi (&mReinitializePciDevicePpiNotifyList);
  }

  if (LpssUartDebugPcdGetDebugEnable ()) {
    GuidHob = GetFirstGuidHob (&gUniversalPayloadSerialPortInfoGuid);
    if (GuidHob == NULL) {

      UartNumber = LpssUartDebugPcdGetControllerNumber ();
      FixedMmioBaseAddress = PCH_SERIAL_IO_BASE_ADDRESS + mPtlPcdLpssUartFixedBar0Offset[UartNumber];
      LpssUartDebugPcdGetAttributes (&LpssUartDeviceConfig.Attributes);

      SerialPortInfoHob = BuildGuidHob (&gUniversalPayloadSerialPortInfoGuid, sizeof (UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO));
        if (SerialPortInfoHob != NULL) {
          SerialPortInfoHob->Header.Revision = UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_REVISION;
          SerialPortInfoHob->Header.Length   = sizeof (UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO);
          SerialPortInfoHob->RegisterBase    = FixedMmioBaseAddress;
          SerialPortInfoHob->RegisterStride  = 1;
          SerialPortInfoHob->BaudRate        = LpssUartDeviceConfig.Attributes.BaudRate;
          SerialPortInfoHob->UseMmio         = TRUE;
          DEBUG ((DEBUG_INFO, "SerialPortInfoHob->RegisterBase: %x\n", SerialPortInfoHob->RegisterBase));
          DEBUG ((DEBUG_INFO, "SerialPortInfoHob->BaudRate: %d\n", SerialPortInfoHob->BaudRate));
        }
    }
  }

  EarlyMeInit ();
  HdaInitEarlyPei ();
#if FixedPcdGetBool(PcdSpiDmaEnable) == 1
  SpiDmaServiceInit();
#endif

  PERF_INMODULE_END ("EarlyDevices");
  return EFI_SUCCESS;
}
