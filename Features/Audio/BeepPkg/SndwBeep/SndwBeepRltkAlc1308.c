/** @file
  This file contains beeping operations specific for Realtek Alc1308 codec

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <SndwMipiCmd.h>
#include <SndwAccess.h>
#include <Beep.h>

#include "SndwBeepPrivateData.h"

#define MAP_RANGE(X, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX) (X - IN_MIN) * (OUT_MAX - OUT_MIN) / (IN_MAX - IN_MIN) +  OUT_MIN

#define RLTK_ALC1308_DEFAULT_FREQUENCY     0x22
#define RLTK_ALC1308_MAX_FREQUENCY_REG     0x3F
#define RLTK_ALC1308_MIN_FREQUENCY_REG     0x0
#define RLTK_ALC1308_MAX_FREQUENCY_HZ      345
#define RLTK_ALC1308_MIN_FREQUENCY_HZ      5
#define RLTK_ALC1308_FREQUENCY_MAP(X)      MAP_RANGE (X, RLTK_ALC1308_MIN_FREQUENCY_HZ, RLTK_ALC1308_MAX_FREQUENCY_HZ, RLTK_ALC1308_MIN_FREQUENCY_REG, RLTK_ALC1308_MAX_FREQUENCY_REG)

#define RLTK_ALC1308_DEFAULT_AMPLITUDE     0x2F
#define RLTK_ALC1308_MAX_AMPLITUDE_REG     0x3F
#define RLTK_ALC1308_MIN_AMPLITUDE_REG     0x0
#define RLTK_ALC1308_MAX_AMPLITUDE_dB      20
#define RLTK_ALC1308_MIN_AMPLITUDE_dB      -65
#define RLTK_ALC1308_AMPLITUDE_MAP(X)      MAP_RANGE (X, RLTK_ALC1308_MIN_AMPLITUDE_dB, RLTK_ALC1308_MAX_AMPLITUDE_dB, RLTK_ALC1308_MIN_AMPLITUDE_REG, RLTK_ALC1308_MAX_AMPLITUDE_REG)

GLOBAL_REMOVE_IF_UNREFERENCED SNDW_COMMAND RltkAlc1308SndwBeepInitCmds[] = {
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC320,
      .RegData = 0xFF,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC321,
      .RegData = 0x37,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC322,
      .RegData = 0x16,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC080,
      .RegData = 0x04,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC310,
      .RegData = 0x22,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC311,
      .RegData = 0x7F,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC090,
      .RegData = 0x61,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC360,
      .RegData = 0x0B,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC361,
      .RegData = 0x80,
    }
  },
  {
    .TxWrite = {
      .OpCode = SndwCmdWrite,
      .RegAddr = 0xC501,
      .RegData = 0x22,
    }
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED SNDW_COMMAND RltkAlc1308SndwBeepFrequencyControlCmds =
{
  .TxWrite = {
    .OpCode = SndwCmdWrite,
    .RegAddr = 0xC501,
    .RegData = RLTK_ALC1308_DEFAULT_FREQUENCY,
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED SNDW_COMMAND RltkAlc1308SndwBeepAmplitudeControlCmds =
{
  .TxWrite = {
    .OpCode = SndwCmdWrite,
    .RegAddr = 0xC502,
    .RegData = RLTK_ALC1308_DEFAULT_AMPLITUDE,
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED SNDW_COMMAND RltkAlc1308SndwBeepOnCmds =
{
  .TxWrite = {
    .OpCode = SndwCmdWrite,
    .RegAddr = 0xC500,
    .RegData = 0x50,
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED SNDW_COMMAND RltkAlc1308SndwBeepOffCmds = {
  .TxWrite = {
    .OpCode = SndwCmdWrite,
    .RegAddr = 0xC500,
    .RegData = 0x10,
  }
};

/**
  This function returns register value corresponding to given frequency

  @param[in] Frequency          Frequency for which register value should be returned

  @retval UINT8                 Register value of Alc1308 for specified frequency
**/
UINT8
Alc1308GetRegValueFromFrequency (
  IN UINTN                      Frequency
  )
{
  ///
  /// Override frequency value if given value is out of range
  ///
  Frequency = MAX (Frequency, RLTK_ALC1308_MIN_FREQUENCY_HZ);
  Frequency = MIN (Frequency, RLTK_ALC1308_MAX_FREQUENCY_HZ);

  DEBUG ((DEBUG_INFO, "Selected frequency: %d Hz.\n", Frequency));

  return (UINT8) (RLTK_ALC1308_FREQUENCY_MAP (Frequency));
}


/**
  This function returns register value corresponding to given amplitude

  @param[in] Amplitude          Amplitude for which register value should be returned

  @retval UINT8                 Register value of Alc1308 for specified amplitude
**/
UINT8
Alc1308GetRegValueFromAmplitude (
  IN INTN                       Amplitude
  )
{
  ///
  /// Override amplitude value if given value is out of range
  ///
  Amplitude = MAX (Amplitude, RLTK_ALC1308_MIN_AMPLITUDE_dB);
  Amplitude = MIN (Amplitude, RLTK_ALC1308_MAX_AMPLITUDE_dB);

  DEBUG ((DEBUG_INFO, "Selected amplitude: %d dB.\n", Amplitude));

  return (UINT8) (RLTK_ALC1308_AMPLITUDE_MAP (Amplitude));
}

/**
  This function turns on tone generator located in Alc1308 codec

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
SndwBeepAlc1308 (
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
  SNDW_COMMAND                  CommandResponse;
  SNDW_BEEP_CONTEXT             *SndwBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwBeepContext = SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL (This);

  SndwAccessApi = SndwBeepContext->SndwAccessApi;

  for (Index = 0; Index < (sizeof (RltkAlc1308SndwBeepInitCmds) / sizeof (SNDW_COMMAND)); Index++) {
    Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepInitCmds[Index], &CommandResponse);
    if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
      DEBUG ((DEBUG_ERROR, "Tone generator initialization failed.\n"));
      DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }
  }

  RltkAlc1308SndwBeepFrequencyControlCmds.TxWrite.RegData = Alc1308GetRegValueFromFrequency (Frequency);
  Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepFrequencyControlCmds, &CommandResponse);
  if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
    DEBUG ((DEBUG_ERROR, "Frequency initialization in tone generator has failed.\n"));
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }
  RltkAlc1308SndwBeepAmplitudeControlCmds.TxWrite.RegData = Alc1308GetRegValueFromAmplitude (Amplitude);
  Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepAmplitudeControlCmds, &CommandResponse);
  if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
    DEBUG ((DEBUG_ERROR, "Amplitude initialization in tone generator has failed.\n"));
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }

  for (Index = 0; Index < NumOfBeeps; Index++) {
    Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepOnCmds, &CommandResponse);
    if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
      DEBUG ((DEBUG_ERROR, "Turn on tone generator failed.\n"));
      DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }
    MicroSecondDelay (BeepDuration * 1000);
    Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepOffCmds, &CommandResponse);
    if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
      DEBUG ((DEBUG_ERROR, "Turn off tone generator failed.\n"));
      DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }
    MicroSecondDelay (PauseDuration * 1000);
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  This function turns on tone generator located in Alc1308 codec

  @param[in] This               Pointer to Sndw beep PPI/Protocol
  @param[in] Frequency          Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude          Amplitude value for tone generator. Refer to codec specification.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
  @retval EFI_INVALID_PARAMETER The private data of the driver could not be found.
**/
EFI_STATUS
EFIAPI
SndwBeepOnAlc1308 (
  IN CONST BEEP                 *This,
  IN UINTN                      Frequency,
  IN INTN                       Amplitude
  )
{
  SNDW_ACCESS                   *SndwAccessApi;
  UINTN                         Index;
  EFI_STATUS                    Status;
  SNDW_COMMAND                  CommandResponse;
  SNDW_BEEP_CONTEXT             *SndwBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwBeepContext = SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL (This);

  SndwAccessApi = SndwBeepContext->SndwAccessApi;

  for (Index = 0; Index < (sizeof (RltkAlc1308SndwBeepInitCmds) / sizeof (SNDW_COMMAND)); Index++) {
    Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepInitCmds[Index], &CommandResponse);
    if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
      DEBUG ((DEBUG_ERROR, "Tone generator initialization failed.\n"));
      DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));
      return Status;
    }
  }

  RltkAlc1308SndwBeepFrequencyControlCmds.TxWrite.RegData = Alc1308GetRegValueFromFrequency (Frequency);
  Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepFrequencyControlCmds, &CommandResponse);
    if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
    DEBUG ((DEBUG_ERROR, "Frequency initialization in tone generator has failed.\n"));
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));
    return Status;
  }
  RltkAlc1308SndwBeepAmplitudeControlCmds.TxWrite.RegData = Alc1308GetRegValueFromAmplitude (Amplitude);
  Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepAmplitudeControlCmds, &CommandResponse);
    if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
    DEBUG ((DEBUG_ERROR, "Amplitude initialization in tone generator has failed.\n"));
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));
    return Status;
  }

  Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepOnCmds, &CommandResponse);
  if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
    DEBUG ((DEBUG_ERROR, "Turn on tone generator failed.\n"));
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}

/**
  This function turns off tone generator located in Alc1308 codec

  @param[in] This               Pointer to Sndw beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
  @retval EFI_INVALID_PARAMETER The private data of the driver could not be found.
**/
EFI_STATUS
EFIAPI
SndwBeepOffAlc1308 (
  IN CONST BEEP                 *This
  )
{
  SNDW_ACCESS                   *SndwAccessApi;
  EFI_STATUS                    Status;
  SNDW_COMMAND                  CommandResponse;
  SNDW_BEEP_CONTEXT             *SndwBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwBeepContext = SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL (This);

  SndwAccessApi = SndwBeepContext->SndwAccessApi;

  Status = SndwAccessApi->SendWithAck (SndwAccessApi, SndwBeepContext->SndwCodecInfo, RltkAlc1308SndwBeepOffCmds, &CommandResponse);
  if (EFI_ERROR (Status) || (CommandResponse.Rx.Ack == 0)) {
    DEBUG ((DEBUG_ERROR, "Turn off tone generator failed.\n"));
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}
