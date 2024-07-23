/** @file
  This PEI driver configures and supports "PC Beep" by SoundWire interface.

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
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/BeepPpi.h>
#include <Ppi/SndwAccessPpi.h>

#include "SndwBeepPrivateData.h"
#include "SndwBeepCodecs.h"

/**
  This function enables Sndw interface.

  @param[in] This               Pointer to Sndw beep PPI.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
EFI_STATUS
EFIAPI
PeiSndwEnable (
  IN CONST BEEP_PPI             *This
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

  @param[in] *This              Pointer to Sndw beep PPI.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
EFI_STATUS
EFIAPI
PeiSndwDisable (
  IN CONST BEEP_PPI             *This
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
  This function performs basic structures initialization and installs Sndw Beep PPI.

  @param  FileHandle             Handle of the file being invoked.
  @param  PeiServices            Describes the list of possible PEI Services.

  @retval EFI_SUCCESS            The driver is successfully initialized.
  @retval Others                 Can't initialize the driver.
**/
EFI_STATUS
EFIAPI
PeiSndwBeepEntryPoint (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS              Status;
  SNDW_BEEP_CONTEXT       *SndwBeepContext;
  EFI_PEI_PPI_DESCRIPTOR  *SndwBeepPpi;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  SndwBeepContext = AllocateZeroPool (sizeof (SNDW_BEEP_CONTEXT));

  if (NULL == SndwBeepContext) {
    DEBUG ((DEBUG_INFO, "%a () - End. Memory pool allocation error.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (SndwBeepContext->SndwCodecInfo.CodecId.Data, PcdGetPtr (PcdSndwBeepCodecId), sizeof (SNDW_CODEC_ID));

  DEBUG ((DEBUG_INFO, "Codec ID:\n"));
  DEBUG ((DEBUG_INFO, "  Version           [0x50]: %02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.Version));
  DEBUG ((DEBUG_INFO, "  ManufacturerID[0] [0x51]: %02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.ManufacturerID[0]));
  DEBUG ((DEBUG_INFO, "  ManufacturerID[1] [0x52]: %02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.ManufacturerID[1]));
  DEBUG ((DEBUG_INFO, "  PartId[0]         [0x53]: %02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.PartId[0]));
  DEBUG ((DEBUG_INFO, "  PartId[1]         [0x54]: %02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.PartId[1]));
  DEBUG ((DEBUG_INFO, "  Class             [0x55]: %02x\n", SndwBeepContext->SndwCodecInfo.CodecId.Encoding.Class));

  SndwBeepContext->Signature           = SNDW_BEEP_SIGNATURE;
  SndwBeepContext->BeepApi.Enable      = PeiSndwEnable;
  SndwBeepContext->BeepApi.Disable     = PeiSndwDisable;

  switch (SwapBytes16 (*(UINT16 *) SndwBeepContext->SndwCodecInfo.CodecId.Encoding.PartId)) {
    case RltkAlc1308PartId:
      SndwBeepContext->BeepApi.BeepOn  = SndwBeepOnAlc1308;
      SndwBeepContext->BeepApi.BeepOff = SndwBeepOffAlc1308;
      SndwBeepContext->BeepApi.Beep    = SndwBeepAlc1308;
      break;
    case SndwCodecPartIdInvalid:
    default:
      DEBUG ((DEBUG_INFO, "There is no supported operation for the preconfigured codec, aborting installation of the PPI.\n"));
      return EFI_UNSUPPORTED;
  }

  Status = PeiServicesLocatePpi (&gSndwAccessPpiGuid, 0, NULL, (VOID **) &SndwBeepContext->SndwAccessApi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to locate PPI - %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  SndwBeepPpi = (EFI_PEI_PPI_DESCRIPTOR*) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SndwBeepPpi == NULL) {
    DEBUG ((DEBUG_INFO, "%a () - End. Memory pool allocation error.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }
  SndwBeepPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  SndwBeepPpi->Guid  = &gBeepPpiGuid;
  SndwBeepPpi->Ppi   = &SndwBeepContext->BeepApi;

  Status = PeiServicesInstallPpi (SndwBeepPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to install PPI - %r\n", __FUNCTION__, Status));
    FreePool (SndwBeepContext);
    FreePool (SndwBeepPpi);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}
