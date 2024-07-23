/** @file
  Beep interface definitions.

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

#ifndef __BEEP_H__
#define __BEEP_H__

typedef struct _BEEP BEEP;

/**
  Function sends command which disables sine tone generator.

  @param[in] This                 Pointer to Codec Beep PPI/Protocol

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Not able to send command to codec
  @retval EFI_DEVICE_ERROR        Responses from codec are incorrect
**/
typedef
EFI_STATUS
(EFIAPI *BEEP_OFF) (
  IN CONST BEEP                   *This
  );

/**
  Function sends command which enables sine tone generator.

  @param[in] This                 Pointer to Codec Beep PPI/Protocol
  @param[in] Frequency            Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude            Amplitude value for tone generator. Refer to codec specification.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Not able to send command to codec
  @retval EFI_DEVICE_ERROR        Responses from codec are incorrect
**/
typedef
EFI_STATUS
(EFIAPI *BEEP_ON) (
  IN CONST BEEP                   *This,
  IN UINTN                        Frequency,
  IN INTN                         Amplitude
  );

/**
  Function send command which enable sine tone generator

  @param[in] This                 Pointer to Codec Beep PPI/Protocol.
  @param[in] Frequency            Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude            Amplitude value for tone generator. Refer to codec specification.
  @param[in] NumOfBeeps           Number of beeps.
  @param[in] BeepDuration         Length of beep sound in miliseconds.
  @param[in] PauseDuration        Duration of pause between beeps.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Not able to send command to codec
  @retval EFI_DEVICE_ERROR        Responses from codec are incorrect
**/
typedef
EFI_STATUS
(EFIAPI *BEEP_GEN) (
  IN CONST BEEP                   *This,
  IN UINTN                        Frequency,
  IN INTN                         Amplitude,
  IN UINTN                        NumOfBeeps,
  IN UINTN                        BeepDuration,
  IN UINTN                        PauseDuration
  );

/**
  This function enables beep interface.

  @param[in] This               Pointer to Codec Beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
typedef
EFI_STATUS
(EFIAPI *BEEP_INTERFACE_ENABLE) (
  IN CONST BEEP                 *This
  );

/**
  This function disables beep interface.

  @param[in] This               Pointer to Codec Beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
typedef
EFI_STATUS
(EFIAPI *BEEP_DISABLE) (
  IN CONST BEEP                 *This
  );

///
/// Codec Beep
///
/// This ppi/protocol:
/// In case of sndw codec manipulates the Sndw controller to perform transactions as a
/// manager on the Sndw bus and connected codec. Support allows to initialize
/// tone generator built in Sndw codec and perform beep sound.
/// In case of hda codec initializes the codec with verbs and then allows to
/// perfom beep on codec using sine tone generator capability avaiable on the codec.
struct _BEEP {
  ///
  /// Stop beep generation
  ///
  BEEP_OFF BeepOff;
  ///
  /// Start beep generation
  ///
  BEEP_ON  BeepOn;
  ///
  /// Generate multiple beep sounds of a certain length
  ///
  BEEP_GEN  Beep;
  ///
  /// Enable beep interface
  ///
  BEEP_INTERFACE_ENABLE  Enable;
  ///
  /// Disable beep interface
  ///
  BEEP_DISABLE Disable;
};

#endif  //  __BEEP_H__
