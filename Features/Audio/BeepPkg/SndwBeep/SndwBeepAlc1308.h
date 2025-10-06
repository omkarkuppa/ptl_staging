/** @file
  Header file that contains function declarations of beeping operations for Alc1308

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

#ifndef __SNDW_BEEP_ALC1308__
#define __SNDW_BEEP_ALC1308__

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
  );

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
  );

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
  );

#endif /** __SNDW_BEEP_ALC1308__ **/
