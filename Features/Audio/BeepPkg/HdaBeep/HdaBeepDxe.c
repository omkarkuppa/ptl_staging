/** @file
  This driver configures and supports "PC Beep" on HDA codec in DXE phase.

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
#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci22.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PeiDxeHdaBeepLib.h>
#include <Library/PeiDxeAceCommonLib.h>
#include <Protocol/BeepProtocol.h>
#include <HdaBeepVerbTables.h>

EFI_STATUS
EFIAPI
LocateHdaPciDevice (
  OUT EFI_PCI_IO_PROTOCOL       **PciIoProtocol
  )
{
  EFI_STATUS                    Status;
  UINTN                         PciIoHandleCount;
  UINTN                         Index;
  EFI_HANDLE                    *PciIoHandleBuffer;
  EFI_PCI_IO_PROTOCOL           *PciIo;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );
  if (EFI_ERROR (Status) || (PciIoHandleBuffer == NULL) || (PciIoHandleCount == 0)) {
    return Status;
  }

  for (Index = 0; Index < PciIoHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    PciIoHandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }

    // Check if this instance is HD-A Device
    if (IsHdaDeviceSupported (PciIo)) {
      break;
    }
  }
  gBS->FreePool (PciIoHandleBuffer);

  *PciIoProtocol = PciIo;

  DEBUG ((DEBUG_INFO, "%a () End.\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  This function is the entry point of this DXE HD-A Beep Driver.

  @param  ImageHandle         The firmware allocated handle for the EFI image.
  @param  SystemTable         A pointer to the EFI System Table.

  @retval EFI_SUCCESS         The function completed successfully.
  @retval EFI_DEVICE_ERROR    Not able to enable DSP memory space.
  @retval EFI_UNSUPPORTED     Requested HDA link not exists.
**/
EFI_STATUS
EFIAPI
DxeHdaBeepEntryPoint (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  EFI_HANDLE              HdaBeepHandle;
  EFI_STATUS              Status;
  HDA_BEEP_CONTEXT        *HdaBeepContext;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  HdaBeepHandle = NULL;
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
  Status = LocateHdaPciDevice (&HdaBeepContext->PciIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - End. Failed to find HDA PCI Device - %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "HDA codec details for Beep :\n"));
  DEBUG ((DEBUG_INFO, "  VendorId:   0x%x\n", HdaBeepVerbTablesAlc256.BeepEnable->Header.VendorId));
  DEBUG ((DEBUG_INFO, "  DeviceId:   0x%x\n", HdaBeepVerbTablesAlc256.BeepEnable->Header.DeviceId));
  DEBUG ((DEBUG_INFO, "  RevisionId: 0x%x\n", HdaBeepVerbTablesAlc256.BeepEnable->Header.RevisionId));
  DEBUG ((DEBUG_INFO, "  SdiNum:     0x%x\n", HdaBeepVerbTablesAlc256.BeepEnable->Header.SdiNum));

  ///
  /// Install on a new handle
  ///
  Status = gBS->InstallProtocolInterface (
                  &HdaBeepHandle,
                  &gBeepProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &HdaBeepContext->BeepApi
                  );
  DEBUG ((DEBUG_INFO, "Hda Beep Protocol Installattion Status = %r\n", Status));
  if (EFI_ERROR (Status)) {
    FreePool (HdaBeepContext);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}
