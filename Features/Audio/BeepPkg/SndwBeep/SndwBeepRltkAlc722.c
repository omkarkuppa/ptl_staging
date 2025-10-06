/** @file
  This file contains beeping operations specific for Realtek Alc722 codec

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiDxeSndwInitLib.h>
#include <SndwMipiCmd.h>
#include <SndwAccess.h>
#include <Beep.h>

#include "SndwBeepPrivateData.h"

#define ALC722_FREQUENCY_1KHZ   0x0C
#define ALC722_FREQUENCY_2_4KHZ 0x05
#define ALC722_FREQUENCY_6KHZ   0x02
#define ALC722_FREQUENCY_800HZ  0x0F
#define ALC722_FREQUENCY_571HZ  0x15
#define ALC722_FREQUENCY_462HZ  0x1A
#define ALC722_FREQUENCY_333HZ  0x24
#define ALC722_FREQUENCY_293HZ  0x29
#define ALC722_FREQUENCY_235HZ  0x33
#define ALC722_FREQUENCY_214HZ  0x38
#define ALC722_FREQUENCY_182HZ  0x42
#define ALC722_FREQUENCY_169HZ  0x47
#define ALC722_FREQUENCY_148HZ  0x51
#define ALC722_FREQUENCY_140HZ  0x56
#define ALC722_FREQUENCY_125HZ  0x60
#define ALC722_FREQUENCY_119HZ  0x65

GLOBAL_REMOVE_IF_UNREFERENCED SDCA_COMMAND RltkAlc722SndwBeepInitCmds[] = {
  {0x41181081, 0x17},
  {0x41181082, 0x17},
};

GLOBAL_REMOVE_IF_UNREFERENCED SDCA_COMMAND RltkAlc722SndwBeepOnCmds[] = {
  {0x41081080, ALC722_FREQUENCY_293HZ} // This register sets the freq and start the beep
};

GLOBAL_REMOVE_IF_UNREFERENCED SDCA_COMMAND RltkAlc722SndwBeepOffCmds[] = {
  {0x41081080, 0x0}
};


/**
  This function turns on tone generator located in Alc722 codec

  @param[in] This               Pointer to Sndw beep PPI/Protocol.
  @param[in] Frequency          Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude          Amplitude value for tone generator. Refer to codec specification.
  @param[in] NumOfBeeps         Number of beeps.
  @param[in] BeepDuration       Length of beep sound in miliseconds.
  @param[in] PauseDuration      Duration of pause between beeps.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
  @retval EFI_INVALID_PARAMETER The private data of the driver could not be found.
**/
EFI_STATUS
EFIAPI
SndwBeepAlc722 (
  IN CONST BEEP                 *This,
  IN UINTN                      Frequency,
  IN INTN                       Amplitude,
  IN UINTN                      NumOfBeeps,
  IN UINTN                      BeepDuration,
  IN UINTN                      PauseDuration
  )
{
  SNDW_ACCESS                   *SndwAccessApi;
  UINTN                         Index;
  EFI_STATUS                    Status;
  SNDW_BEEP_CONTEXT             *SndwBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwBeepContext = SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL (This);

  SndwAccessApi = SndwBeepContext->SndwAccessApi;

  Status = SendSdcaCommand (
             SndwAccessApi,
             RltkAlc722SndwBeepInitCmds,
             ARRAY_SIZE (RltkAlc722SndwBeepInitCmds),
             SndwBeepContext->SndwCodecInfo
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tone generator initialization failed.\n"));
    return EFI_DEVICE_ERROR;
  }

  for  (Index = 0; Index < NumOfBeeps; Index++) {
    Status = SendSdcaCommand (
               SndwAccessApi,
               RltkAlc722SndwBeepOnCmds,
               ARRAY_SIZE (RltkAlc722SndwBeepOnCmds),
               SndwBeepContext->SndwCodecInfo
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Turn on tone generator failed.\n"));
      return EFI_DEVICE_ERROR;
    }

    MicroSecondDelay (BeepDuration * 1000);

    Status = SendSdcaCommand (
               SndwAccessApi,
               RltkAlc722SndwBeepOffCmds,
               ARRAY_SIZE (RltkAlc722SndwBeepOffCmds),
               SndwBeepContext->SndwCodecInfo
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Turn off tone generator failed.\n"));
      return EFI_DEVICE_ERROR;
    }

    if (Index == (NumOfBeeps - 1)) {
      break;
    }

    MicroSecondDelay (PauseDuration * 1000);
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  This function turns on tone generator located in Alc722 codec

  @param[in] This               Pointer to Sndw beep PPI/Protocol
  @param[in] Frequency          Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude          Amplitude value for tone generator. Refer to codec specification.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
  @retval EFI_INVALID_PARAMETER The private data of the driver could not be found.
**/
EFI_STATUS
EFIAPI
SndwBeepOnAlc722 (
  IN CONST BEEP                 *This,
  IN UINTN                      Frequency,
  IN INTN                       Amplitude
  )
{
  SNDW_ACCESS                   *SndwAccessApi;
  EFI_STATUS                    Status;
  SNDW_BEEP_CONTEXT             *SndwBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwBeepContext = SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL (This);

  SndwAccessApi = SndwBeepContext->SndwAccessApi;

  Status = SendSdcaCommand (
             SndwAccessApi,
             RltkAlc722SndwBeepInitCmds,
             ARRAY_SIZE (RltkAlc722SndwBeepInitCmds),
             SndwBeepContext->SndwCodecInfo
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tone generator initialization failed.\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = SendSdcaCommand (
             SndwAccessApi,
             RltkAlc722SndwBeepOnCmds,
             ARRAY_SIZE (RltkAlc722SndwBeepOnCmds),
             SndwBeepContext->SndwCodecInfo
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Turn on tone generator failed.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}

/**
  This function turns off tone generator located in Alc722 codec

  @param[in] This               Pointer to Sndw beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
  @retval EFI_INVALID_PARAMETER The private data of the driver could not be found.
**/
EFI_STATUS
EFIAPI
SndwBeepOffAlc722 (
  IN CONST BEEP                 *This
  )
{
  SNDW_ACCESS                   *SndwAccessApi;
  EFI_STATUS                    Status;
  SNDW_BEEP_CONTEXT             *SndwBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwBeepContext = SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL (This);

  SndwAccessApi = SndwBeepContext->SndwAccessApi;

  Status = SendSdcaCommand (
             SndwAccessApi,
             RltkAlc722SndwBeepOffCmds,
             ARRAY_SIZE (RltkAlc722SndwBeepOffCmds),
             SndwBeepContext->SndwCodecInfo
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Turn off tone generator failed.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}
