/** @file
  Header file for HD-A Beep Common Library for both PEI and DXE phase.

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

 @par Specification Reference:
**/

#ifndef _PEI_DXE_HDA_BEEP_LIB_H_
#define _PEI_DXE_HDA_BEEP_LIB_H_

#include <Beep.h>
#include <HdaBeepVerbTables.h>

#define HDA_BEEP_SIGNATURE SIGNATURE_32 ('h', 'd', 'a', 'b')

typedef struct {
  BOOLEAN                 IsCodecInitialized;
  UINT8                   SdiNum;
  UINT32                  VendorId;
  UINT32                  RevisionId;
} HDA_CODEC_INFO;

typedef struct {
  UINT32                  Signature;
  BEEP                    BeepApi;
  HDA_BEEP_VERB_TABLES    *HdaCodecVerbTables;
  HDA_CODEC_INFO          HdaCodecInfo;
  EFI_PCI_IO_PROTOCOL     *PciIo;
} HDA_BEEP_CONTEXT;

#define HDA_BEEP_CONTEXT_FROM_HDA_BEEP_PPI_PROTOCOL(a)  CR (a, HDA_BEEP_CONTEXT, BeepApi, HDA_BEEP_SIGNATURE)

typedef enum {
  HdaLinkFreq6MHz  = 0,
  HdaLinkFreq12MHz = 1,
  HdaLinkFreq24MHz = 2,
  HdaLinkFreq48MHz = 3,
  HdaLinkFreq96MHz = 4,
  HdaLinkFreqInvalid
} HDAUDIO_LINK_FREQUENCY;

/**
  This function turns on sine generator located in HDA codec

  @param[in] This            Pointer to Beep PPI/Protocol.
  @param[in] Frequency       Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude       Amplitude value for tone generator. Refer to codec specification.
  @param[in] NumOfBeeps      Number of beeps.
  @param[in] BeepDuration    Length of beep sound in miliseconds.
  @param[in] PauseDuration   Duration of pause between beeps in miliseconds.

  @retval EFI_SUCCESS        The function completed successfully.
  @retval EFI_DEVICE_ERROR   Not able to enable DSP memory space.

**/
EFI_STATUS
EFIAPI
HdaBeep (
  IN CONST BEEP              *This,
  IN UINTN                   Frequency,
  IN INTN                    Amplitude,
  IN UINTN                   NumOfBeeps,
  IN UINTN                   BeepDuration,
  IN UINTN                   PauseDuration
  );

/**
  This function turns on sine generator located in HDA codec

  @param[in] This            Pointer to Beep PPI/Protocol.
  @param[in] Frequency       Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude       Amplitude value for tone generator. Refer to codec specification.

  @retval EFI_SUCCESS        The function completed successfully.
  @retval EFI_NOT_READY      The codec is not ready to receive verbs for beep
**/
EFI_STATUS
EFIAPI
HdaBeepOn (
  IN CONST BEEP              *This,
  IN UINTN                   Frequency,
  IN INTN                    Amplitude
  );

/**
  This function turns off sine generator located in HDA codec

  @param[in] This               Pointer to Beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
EFI_STATUS
EFIAPI
HdaBeepOff (
  IN CONST BEEP                 *This
  );

/**
  This function enables HDA interface.

  @param[in] This               Pointer to Beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
EFI_STATUS
EFIAPI
HdaEnable (
  IN CONST BEEP                 *This
  );

/**
  This function disables HDA interface.

  @param[in] This               Pointer to Beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
EFI_STATUS
EFIAPI
HdaDisable (
  IN CONST BEEP                 *This
  );

#endif //* _PEI_DXE_HDA_BEEP_LIB_H_ *//
