/** @file
  This driver configures and supports "PC Beep" on HDA codec in PEI phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par specification reference:
**/

#include <Base.h>
#include <Uefi.h>
#include <Ppi/PciDevice.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiDxeHdaBeepLib.h>
#include <Library/PeiDxeAceCommonLib.h>
#include <Ppi/BeepPpi.h>
#include <HdaBeepVerbTables.h>

EFI_STATUS
EFIAPI
HdaPciDevicePpiInstallationCallback (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR  mPciDevicePpiNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiPciDevicePpiGuid,
  HdaPciDevicePpiInstallationCallback
};

/**
  Checks if a newly installed PPI points to a valid HDA device and installs a HDA_PPI for it.

  @param[in]  PeiServices        Pointer to PeiServices
  @param[in]  NotifyDescriptor   Pointer to notify descriptor
  @param[in]  Ppi                Installed PPI

  @return Status of the check
**/
EFI_STATUS
EFIAPI
HdaPciDevicePpiInstallationCallback (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  )
{
  EFI_STATUS                     Status;
  HDA_BEEP_CONTEXT               *HdaBeepContext;
  EFI_PEI_PPI_DESCRIPTOR         *HdaBeepPpi;
  EDKII_PCI_DEVICE_PPI           *PciDevice;

  PciDevice = (EDKII_PCI_DEVICE_PPI*) Ppi;

  if (!IsHdaDeviceSupported (&PciDevice->PciIo)) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  HdaBeepContext = AllocateZeroPool (sizeof (HDA_BEEP_CONTEXT));
  if (NULL == HdaBeepContext) {
    DEBUG ((DEBUG_INFO, "%a () - End. Memory pool allocation error.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  HdaBeepContext->Signature                        = HDA_BEEP_SIGNATURE;
  HdaBeepContext->BeepApi.BeepOn                   = HdaBeepOn;
  HdaBeepContext->BeepApi.BeepOff                  = HdaBeepOff;
  HdaBeepContext->BeepApi.Beep                     = HdaBeep;
  HdaBeepContext->BeepApi.Enable                   = HdaEnable;
  HdaBeepContext->BeepApi.Disable                  = HdaDisable;
  HdaBeepContext->HdaCodecVerbTables               = &HdaBeepVerbTablesAlc256;
  HdaBeepContext->HdaCodecInfo.IsCodecInitialized  = FALSE;
  HdaBeepContext->HdaCodecInfo.SdiNum              = 0xFF;
  HdaBeepContext->HdaCodecInfo.VendorId            = 0xFFFFFFFF;
  HdaBeepContext->HdaCodecInfo.RevisionId          = 0xFFFFFFFF;
  HdaBeepContext->PciIo                            = &PciDevice->PciIo;

  DEBUG ((DEBUG_INFO, "HDA codec details for Beep :\n"));
  DEBUG ((DEBUG_INFO, "  VendorId:   0x%x\n", HdaBeepVerbTablesAlc256.BeepEnable->Header.VendorId));
  DEBUG ((DEBUG_INFO, "  DeviceId:   0x%x\n", HdaBeepVerbTablesAlc256.BeepEnable->Header.DeviceId));
  DEBUG ((DEBUG_INFO, "  RevisionId: 0x%x\n", HdaBeepVerbTablesAlc256.BeepEnable->Header.RevisionId));
  DEBUG ((DEBUG_INFO, "  SdiNum:     0x%x\n", HdaBeepVerbTablesAlc256.BeepEnable->Header.SdiNum));

  HdaBeepPpi = (EFI_PEI_PPI_DESCRIPTOR*) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (HdaBeepPpi == NULL) {
    DEBUG ((DEBUG_INFO, "%a () - End. Memory pool allocation error.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }
  HdaBeepPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  HdaBeepPpi->Guid  = &gBeepPpiGuid;
  HdaBeepPpi->Ppi   = &HdaBeepContext->BeepApi;

  Status = PeiServicesInstallPpi (HdaBeepPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to install PPI - %r\n", __FUNCTION__, Status));
    FreePool (HdaBeepContext);
    FreePool (HdaBeepPpi);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}

/**
  This function is the entry point of this PEI HDA Beep Driver.

  @param  FileHandle             Handle of the file being invoked.
  @param  PeiServices            Describes the list of possible PEI Services.

  @retval EFI_SUCCESS            The driver is successfully initialized.
  @retval EFI_UNSUPPORTED        Not able to find HDA Device.
  @retval Others                 Can't initialize the driver.
**/
EFI_STATUS
EFIAPI
PeiHdaBeepEntryPoint (
  IN EFI_PEI_FILE_HANDLE         FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  DEBUG ((DEBUG_INFO, "%a - Start ()\n", __FUNCTION__));

  PeiServicesNotifyPpi (&mPciDevicePpiNotify);

  DEBUG ((DEBUG_INFO, "%a - End ()\n", __FUNCTION__));

  return EFI_SUCCESS;
}
