/** @file
  This DXE driver configures and supports "PC Beep" by SoundWire interface.

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
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/BeepProtocol.h>
#include <Protocol/SndwAccessProtocol.h>

#include "SndwBeepPrivateData.h"
#include "SndwBeepCodecs.h"

/**
  This function enables Sndw interface.

  @param[in] This               Pointer to Sndw beep protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_INVALID_PARAMETER The private data of the driver could not be found.
**/
EFI_STATUS
EFIAPI
DxeSndwEnable (
  IN CONST BEEP_PROTOCOL        *This
  )
{
  SNDW_BEEP_CONTEXT             *SndwBeepContext;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwBeepContext = SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL (This);

  Status = SndwBeepContext->SndwAccessApi->Enable (SndwBeepContext->SndwAccessApi);

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}

/**
  This function disables Sndw interface.

  @param[in] This               Pointer to Sndw beep protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_INVALID_PARAMETER The private data of the driver could not be found.
**/
EFI_STATUS
EFIAPI
DxeSndwDisable (
  IN CONST BEEP_PROTOCOL        *This
  )
{
  SNDW_BEEP_CONTEXT             *SndwBeepContext;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwBeepContext = SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL (This);

  Status = SndwBeepContext->SndwAccessApi->Disable (SndwBeepContext->SndwAccessApi);

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}


/**
  This function performs basic structures initialization and installs Sndw Beep protocol.

  @param[in] Event          Event whose notification function is being invoked.
  @param[in] Context        Pointer to the notification function's context.

  @retval EFI_SUCCESS       The driver is successfully initialized.
  @retval Others            Can't initialize the driver.
**/
VOID
EFIAPI
DxeSndwBeepInit (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  )
{
  EFI_HANDLE                SndwBeepHandle;
  EFI_STATUS                Status;
  SNDW_BEEP_CONTEXT         *SndwBeepContext;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  gBS->CloseEvent (Event);

  SndwBeepHandle = NULL;
  SndwBeepContext = AllocateZeroPool (sizeof (SNDW_BEEP_CONTEXT));
  if (NULL == SndwBeepContext) {
    DEBUG ((DEBUG_INFO, "%a () - End. Memory pool allocation error.\n", __FUNCTION__));
    return;
  }

  CopyMem (SndwBeepContext->SndwCodecInfo.CodecId.Data, PcdGetPtr (PcdSndwBeepCodecId), sizeof (SNDW_CODEC_ID));

  DEBUG ((DEBUG_INFO, "Codec ID:\n"));
  DEBUG ((DEBUG_INFO, "  UniqueId          [0x50]: 0x%02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.UniqueId));
  DEBUG ((DEBUG_INFO, "  Version           [0x50]: 0x%02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.Version));
  DEBUG ((DEBUG_INFO, "  ManufacturerID[0] [0x51]: 0x%02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.ManufacturerID[0]));
  DEBUG ((DEBUG_INFO, "  ManufacturerID[1] [0x52]: 0x%02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.ManufacturerID[1]));
  DEBUG ((DEBUG_INFO, "  PartId[0]         [0x53]: 0x%02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.PartId[0]));
  DEBUG ((DEBUG_INFO, "  PartId[1]         [0x54]: 0x%02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.PartId[1]));
  DEBUG ((DEBUG_INFO, "  Class             [0x55]: 0x%02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.Class));

  SndwBeepContext->Signature           = SNDW_BEEP_SIGNATURE;
  SndwBeepContext->BeepApi.Enable      = DxeSndwEnable;
  SndwBeepContext->BeepApi.Disable     = DxeSndwDisable;

  switch (SwapBytes16 (*(UINT16 *) SndwBeepContext->SndwCodecInfo.CodecId.Encoding.PartId)) {
    case RltkAlc1308PartId:
      SndwBeepContext->BeepApi.BeepOn  = SndwBeepOnAlc1308;
      SndwBeepContext->BeepApi.BeepOff = SndwBeepOffAlc1308;
      SndwBeepContext->BeepApi.Beep    = SndwBeepAlc1308;
      break;
    case SndwCodecPartIdInvalid:
    default:
      DEBUG ((DEBUG_INFO, "There is no supported operation for the preconfigured codec, aborting installation of the protocol.\n"));
      return;
  }

  ///
  ///  Locate the Sndw Access Protocol.
  ///
  Status = gBS->LocateProtocol (
                  &gSndwAccessProtocolGuid,
                  NULL,
                  (VOID **) &SndwBeepContext->SndwAccessApi
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to locate protocol - %r\n", __FUNCTION__, Status));
    return;
  }

  ///
  /// Install on a new handle
  ///
  Status = gBS->InstallProtocolInterface (
                  &SndwBeepHandle,
                  &gBeepProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &SndwBeepContext->BeepApi
                  );
  DEBUG ((DEBUG_INFO, "Sndw Beep Protocol Installattion Status = %r\n", Status));
  if (EFI_ERROR (Status)) {
    FreePool (SndwBeepContext);
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return;
}


/**
  @param  ImageHandle         The firmware allocated handle for the EFI image.
  @param  SystemTable         A pointer to the EFI System Table.

  @retval EFI_SUCCESS         The function completed successfully.
  @retval EFI_DEVICE_ERROR    Not able to enable DSP memory space.
  @retval EFI_UNSUPPORTED     Requested Sndw link not exists.
**/
EFI_STATUS
EFIAPI
DxeSndwBeepEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  VOID                *Registration;
  EFI_EVENT           Event;

  Event = NULL;

  Event = EfiCreateProtocolNotifyEvent (
    &gSndwAccessProtocolGuid,
    TPL_CALLBACK,
    DxeSndwBeepInit,
    NULL,
    &Registration
    );
  if (Event == NULL) {
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}
