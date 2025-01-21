/** @file
  This DXE driver configures Sndw controllers, performs codecs enumeration process,
  allows to send/receive messages to/from them.

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
#include <Protocol/SndwAccessProtocol.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PeiDxeSndwInitLib.h>
#include <Library/PeiDxeAceCommonLib.h>

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
  This function is the entry point of this DXE Sndw Init Driver.

  @param  ImageHandle         The firmware allocated handle for the EFI image.
  @param  SystemTable         A pointer to the EFI System Table.

  @retval EFI_SUCCESS         The function completed successfully.
  @retval EFI_DEVICE_ERROR    Not able to enable DSP memory space.
  @retval EFI_UNSUPPORTED     Not able to find HDA Device.
**/
EFI_STATUS
EFIAPI
DxeSndwInitEntryPoint (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  EFI_STATUS                  Status;
  SNDW_ACCESS_CONTEXT         *SndwAccessContext;
  EFI_HANDLE                  SndwAccessHandle;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  SndwAccessHandle = NULL;
  SndwAccessContext = AllocateZeroPool (sizeof (SNDW_ACCESS_CONTEXT));
  if (NULL == SndwAccessContext) {
    DEBUG ((DEBUG_INFO, "%a () - End. Memory pool allocation error.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SndwAccessContext->Signature                = SNDW_ACCESS_SIGNATURE;
  SndwAccessContext->SndwAccess.Enable        = SndwAccessEnable;
  SndwAccessContext->SndwAccess.Disable       = SndwAccessDisable;
  SndwAccessContext->SndwAccess.Send          = SndwSend;
  SndwAccessContext->SndwAccess.Receive       = SndwReceive;
  SndwAccessContext->SndwAccess.SendWithAck   = SndwSendWithAck;
  SndwAccessContext->SndwAccess.GetFirstCodec = SndwGetFirstCodec;
  SndwAccessContext->SndwAccess.GetNextCodec  = SndwGetNextCodec;
  SndwAccessContext->CodecListHead            = NULL;
  SndwAccessContext->EnableCounter            = 0;
  Status = LocateHdaPciDevice (&SndwAccessContext->PciIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - End. Failed to find HDA PCI Device - %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  //
  // Install on a new handle
  //
  Status = gBS->InstallProtocolInterface (
                  &SndwAccessHandle,
                  &gSndwAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &SndwAccessContext->SndwAccess
                  );
  DEBUG ((DEBUG_INFO, "Sndw access Protocol Installattion Status = %r\n", Status));
  if (EFI_ERROR (Status)) {
    FreePool (SndwAccessContext);
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}
