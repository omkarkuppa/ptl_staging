/** @file
  USB4 host router initialization routines in PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/IoMmu.h>
#include <SetupVariable.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4CmMode.h>
#include <Library/IUsb4HrLib.h>
#include "Usb4PlatformDriver.h"
#include <Library/TbtCommonLib.h>
#include <Library/DUsb4HrPeiLib.h>

/**
  Get the build PCD data for USB4 Maximum Topology Depth.
  USB4 Maximum topology depth, constraint USB4 CM to support two level of dock.
  @retval Value of Maximum depth platform supported
**/
UINT8
QueryUsb4MaxTopologyDepth (VOID)
{
  return FixedPcdGet8(PcdUsb4MaxTopologyDepth);
}

/**
  Get the BIOS setup data.
  The caller should call FreePool to release the memory of setup data after use it.

  @retval Pointer to BIOS setup data structure.
**/
SETUP_DATA *
QuerySetupData (VOID)
{
  EFI_STATUS                         Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI    *VariableServices;
  SETUP_DATA                         *SetupData;
  UINTN                              VarSize;

  SetupData = NULL;

  //
  // Locate Setup variables
  //
  VarSize = sizeof (SETUP_DATA);
  SetupData = (SETUP_DATA *) AllocateZeroPool (VarSize);
  if (SetupData == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate SetupData size\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesLocatePpi gEfiPeiReadOnlyVariable2PpiGuid failed, %r\n", Status));
    goto Exit;
  }
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VarSize,
                               SetupData
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuerySetupData: GetVariable (SetupData) failed, Status:%r\n", Status));
    goto Exit;
  }

Exit:
  if (EFI_ERROR (Status) && (SetupData != NULL)) {
    FreePool (SetupData);
    SetupData = NULL;
  }
  return SetupData;
}

/**
  Query the USB4 host router information in the platform.

  @param[in,out] Usb4PlatformInfo - Pointer to USB4 host routers information in the platform.

  @retval EFI_SUCCESS           - Query the USB4 host router information successfully.
  @retval EFI_NOT_FOUND         - No USB4 host router found in platform.
  @retval EFI_UNSUPPORTED       - Fail to query the USB4 host router information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4PlatformInfoQuery (
  IN OUT USB4_PLATFORM_INFO    *Usb4PlatformInfo
  )
{
  EFI_STATUS    Status;
  SETUP_DATA    *SetupData;

  DEBUG ((DEBUG_INFO, "Usb4PlatformInfoQuery entry\n"));

  SetupData = NULL;

  if (Usb4PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4PlatformInfoQuery: Null Usb4PlatformInfo\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  SetupData = QuerySetupData ();
  if (SetupData == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4PlatformInfoQuery: Fail to query setup data\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4PlatformInfo->CmModeOption         = SetupData->Usb4CmMode;
  Usb4PlatformInfo->PcieOverUsb4En       = SetupData->EnablePcieTunnelingOverUsb4;
  Usb4PlatformInfo->Usb4DebugLevel       = SetupData->Usb4DebugLevel;
  Usb4PlatformInfo->Usb4HrCount          = 0;
  Usb4PlatformInfo->Usb4HrMask           = 0;
  Usb4PlatformInfo->Usb4MaxTopologyDepth = QueryUsb4MaxTopologyDepth();
  Usb4PlatformInfo->CmVer2Support        = TRUE;

#if FixedPcdGetBool (PcdTcssSupport) == 1
  //
  // Query the integrated USB4 host router information in platform
  //
  Status = Iusb4PlatformInfoQuery (Usb4PlatformInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4PlatformInfoQuery: Fail to query integrated USB4 host router information, %r\n", Status));
  }
#endif

#if FixedPcdGetBool (PcdDTbtEnable) == 1
  //
  // Query the discrete USB4 host router information in platform
  //
  Status = Dusb4PlatformInfoQuery (Usb4PlatformInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4PlatformInfoQuery: Fail to query discrete USB4 host router information, %r\n", Status));
  }
#endif
  Usb4PlatformInfo->CmSwitchSupport      = FALSE;
  DEBUG ((DEBUG_INFO, "Platform Enabled USB4 HR count     = %d\n", Usb4PlatformInfo->Usb4HrCount));
  DEBUG ((DEBUG_INFO, "Platform Enabled USB4 HR mask      = 0x%0X\n", Usb4PlatformInfo->Usb4HrMask));
  DEBUG ((DEBUG_INFO, "Platform Setup USB4 CM Option      = 0x%0X\n", Usb4PlatformInfo->CmModeOption));
  DEBUG ((DEBUG_INFO, "Platform Applied USB4 CM Mode      = 0x%0x\n", Usb4PlatformInfo->CmMode));
  DEBUG ((DEBUG_INFO, "Platform CM mode switch capability = %d\n", Usb4PlatformInfo->CmSwitchSupport));
  DEBUG ((DEBUG_INFO, "Platform Setup USB4 Debug Level    = %d\n", Usb4PlatformInfo->Usb4DebugLevel));
  DEBUG ((DEBUG_INFO, "Platform Maximum depth of topology = %d\n", Usb4PlatformInfo->Usb4MaxTopologyDepth));

  Status = (Usb4PlatformInfo->Usb4HrCount != 0) ? EFI_SUCCESS : EFI_NOT_FOUND;
Exit:
  if (SetupData != NULL) {
    FreePool (SetupData);
  }
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfoQuery exit\n"));
  return Status;
}

/**
  Allocate DMA buffer for USB4 host router via IOMMU.

  @param[in]  IoMmu - Pointer to IOMMU ppi.
  @param[out] DmaBufBase - Pointer to the allocated DMA buffer base address.
  @param[out] DmaBufSize - Pointer to the allocated DMA buffer size.

  @retval EFI_SUCCESS           - Allocate DMA buffer successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
AllocUsb4HrDmaBuf (
  IN  EDKII_IOMMU_PPI       *IoMmu,
  OUT PHYSICAL_ADDRESS      *DmaBufBase,
  OUT UINT32                *DmaBufSize
  )
{
  EFI_STATUS            Status;
  VOID                  *HostAddress;
  UINTN                 Pages;
  UINTN                 NumberOfBytes;
  PHYSICAL_ADDRESS      DeviceAddress;
  VOID                  *Mapping;

  DEBUG ((DEBUG_INFO, "AllocUsb4HrDmaBuf entry\n"));

  HostAddress = NULL;
  Mapping     = NULL;

  if ((IoMmu == NULL) || (DmaBufBase == NULL) || (DmaBufSize == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Pages = USB4_PEI_DMA_BUFFER_PAGES;
  Status = IoMmu->AllocateBuffer (IoMmu, EfiBootServicesData, Pages, &HostAddress, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AllocUsb4HrDmaBuf: IoMmu AllocateBuffer failure, %r\n", Status));
    goto Exit;
  }

  NumberOfBytes = EFI_PAGES_TO_SIZE (Pages);
  DeviceAddress = 0;
  Status = IoMmu->Map (
                    IoMmu,
                    EdkiiIoMmuOperationBusMasterCommonBuffer,
                    HostAddress,
                    &NumberOfBytes,
                    &DeviceAddress,
                    &Mapping
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AllocUsb4HrDmaBuf: IoMmu Map failure, %r\n", Status));
    goto Exit;
  }

  Status = IoMmu->SetAttribute (
                    IoMmu,
                    Mapping,
                    EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AllocUsb4HrDmaBuf: IoMmu SetAttribute failure, %r\n", Status));
    goto Exit;
  }

  *DmaBufBase = DeviceAddress;
  *DmaBufSize = (UINT32) NumberOfBytes;
Exit:
  if (EFI_ERROR (Status)) {
    if (Mapping != NULL) {
      IoMmu->Unmap (IoMmu, Mapping);
    }
    if (HostAddress != NULL) {
      IoMmu->FreeBuffer (IoMmu, Pages, HostAddress);
    }
  }
  DEBUG ((DEBUG_INFO, "AllocUsb4HrDmaBuf exit\n"));
  return Status;
}

/**
  Reserved DMA buffer via IOMMU ppi for Tx/Rx ring access in Dxe before PCI bus enumeration if IOMMU is enabled.

  @param[in,out] Usb4PlatformInfo - Pointer to USB4 host routers information in the platform.

  @retval EFI_SUCCESS           - Initialize all USB4 host routers successfully.
  @retval EFI_UNSUPPORTED       - Unable to initialize all USB4 host routers to the preferred CM mode.
  @retval EFI_INVALID_PARAMETER - NULL Usb4PlatformInfo pointer.
**/
EFI_STATUS
PlatformUsb4DmaBufAlloc (
  IN OUT USB4_PLATFORM_INFO    *Usb4PlatformInfo
  )
{
  EFI_STATUS            Status;
  EDKII_IOMMU_PPI       *IoMmu;
  PHYSICAL_ADDRESS      DmaBufBase;
  UINT32                DmaBufSize;
  UINT32                HrIndex;

  DEBUG ((DEBUG_INFO, "PlatformUsb4DmaBufAlloc entry\n"));

  if (Usb4PlatformInfo == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4PlatformInfo->Usb4HrCount > USB4_HR_SUPPORT_MAX) {
    DEBUG ((DEBUG_ERROR, "Too many USB4 HRs to support, HR count = %d\n", Usb4PlatformInfo->Usb4HrCount));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Locate IoMmu PPI to reserve the PEI DMA buffer for Tx/Rx ring transfer if IOMMU is enabled
  //
  Status = PeiServicesLocatePpi (
             &gEdkiiIoMmuPpiGuid,
             0,
             NULL,
             (VOID **) &IoMmu
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PlatformUsb4DmaBufAlloc: gEdkiiIoMmuPpiGuid Not installed\n"));
    IoMmu = NULL;
  }

  if (IoMmu != NULL) {
    DEBUG ((DEBUG_INFO, "IOMMU is enabled!\n"));
    for (HrIndex = 0; HrIndex < Usb4PlatformInfo->Usb4HrCount; HrIndex++) {
      Status = AllocUsb4HrDmaBuf (IoMmu, &DmaBufBase, &DmaBufSize);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Fail to allocate IOMMU DMA buffer for USB4 HR, %r\n", Status));
        goto Exit;
      }
      Usb4PlatformInfo->Usb4Hr[HrIndex].PeiDmaBufBase = DmaBufBase;
      Usb4PlatformInfo->Usb4Hr[HrIndex].PeiDmaBufSize = DmaBufSize;
      DEBUG ((DEBUG_INFO, "USB4 PEI HR %d DMA buffer base = 0x%ll0x\n", HrIndex, DmaBufBase));
      DEBUG ((DEBUG_INFO, "USB4 PEI HR %d DMA buffer size = %d\n", HrIndex, DmaBufSize));
    }
  } else {
    //
    // It's not required to reserve DMA buffer if IOMMU is not enabled
    //
    DEBUG ((DEBUG_INFO, "IOMMU is not enabled!\n"));
    for (HrIndex = 0; HrIndex < Usb4PlatformInfo->Usb4HrCount; HrIndex++) {
      Usb4PlatformInfo->Usb4Hr[HrIndex].PeiDmaBufBase = 0;
      Usb4PlatformInfo->Usb4Hr[HrIndex].PeiDmaBufSize = 0;
    }
  }

  Status = EFI_SUCCESS;
Exit:
  DEBUG ((DEBUG_INFO, "PlatformUsb4DmaBufAlloc exit\n"));
  return Status;
}

/**
  USB4 platform initialization for CM execution.
  - Reserved PEI DMA buffer for SW CM Tx/Rx ring if IOMMU is enabled.

  @param[in] Usb4PlatformInfo - Pointer to USB4 host routers information in the platform.

  @retval EFI_SUCCESS           - Initialize USB4 platform for CM support successfully.
  @retval EFI_UNSUPPORTED       - Fail to initialize USB4 platform for CM support
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4PlatformInit (
  IN PUSB4_PLATFORM_INFO    Usb4PlatformInfo
  )
{
  EFI_STATUS  Status;
  UINT8       Index;

  DEBUG ((DEBUG_INFO, "Usb4PlatformInit entry\n"));

  Status = EFI_SUCCESS;

  if (Usb4PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4PlatformInit: Null Usb4PlatformInfo\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4PlatformInfo->Usb4HrCount > USB4_HR_SUPPORT_MAX) {
    DEBUG ((DEBUG_ERROR, "Too many USB4 HRs to support, HR count = %d\n", Usb4PlatformInfo->Usb4HrCount));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Reserved PEI DMA buffer for SW CM Tx/Rx ring usage before PCI enumeration
  //
  if (Usb4PlatformInfo->CmMode == USB4_CM_MODE_SW_CM) {
    Status = PlatformUsb4DmaBufAlloc (Usb4PlatformInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4PlatformInit: Fail to allocate DMA buffer for USB4, %r\n", Status));
      goto Exit;
    }

    //
    // For improve boot time set force power in PEI phase with one shot,
    // set the timeout to 1, assume TBT FW will be ready in DXE phase
    //
    if (Usb4PlatformInfo->CmModeOption != USB4_SETUP_CM_MODE_DEBUG) {
      //
      // Update the CM mode. If the CM Mode is SW CM then turn on the ITBT DXE FP setting.
      // If the CmModeOption is Debug, ITBT switch to SW CM but the USB4 CM driver didn't active.
      // If the CmModeOption is OS Mode, The USB4 CM driver is active with CmMode option is SW CM.
      //
      for (Index = 0; Index < USB4_HR_SUPPORT_MAX; Index++) {
        if (Usb4PlatformInfo->Usb4Hr[Index].IntegratedHr) {
          Status = ITbtSetHrForcePower (0,
                     Usb4PlatformInfo->Usb4Hr[Index].NhiBdf.Bus,
                     Usb4PlatformInfo->Usb4Hr[Index].NhiBdf.Dev,
                     Usb4PlatformInfo->Usb4Hr[Index].NhiBdf.Func,
                     1
                     );
          if (Status == EFI_TIMEOUT) {
            DEBUG ((DEBUG_INFO, "Expect Usb4 PEIM set ITBT HR force power get time out\n"));
            Status = EFI_SUCCESS;
          }
        }
      }
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4PlatformInit exit\n"));
  return Status;
}