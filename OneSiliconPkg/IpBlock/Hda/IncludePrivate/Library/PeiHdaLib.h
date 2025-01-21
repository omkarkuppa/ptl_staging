/** @file
  Header file for PeiHdaLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _PEI_HDA_LIB_H_
#define _PEI_HDA_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <HdAudioConfig.h>
#include <HdaCtrl.h>
#include <Hda.h>

#define HDA_MAX_LOOP_TIME            2000
#define HDA_WAIT_PERIOD              5

/*
  Configure HDA Audio Link GPIOs

  @param[in] HdaPreMemConfig      Hda PreMem Configuration.
*/
typedef
VOID
(*HDA_CONFIGURE_HDA_LINK_GPIO) (
  IN HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig
  );

/*
  Configure DMIC Audio Link GPIOs

  @param[in] HdaPreMemConfig      Hda PreMem Configuration.
  @param[in] DmicIndex            Dmic Link Index
*/
typedef
VOID
(*HDA_CONFIGURE_DMIC_LINK_GPIO) (
  IN HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig,
  IN UINT32                  DmicIndex
  );

/*
  Configure SSP Audio Link GPIOs

  @param[in] HdaPreMemConfig      Hda PreMem Configuration.
  @param[in] SspIndex       SSP Link Index
*/
typedef
VOID
(*HDA_CONFIGURE_SSP_LINK_GPIO) (
  IN HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig,
  IN UINT32                  SspIndex
  );

/*
  Configure Master Clock GPIOs for SSP Link

  @param[in] HdaPreMemConfig   Hda PreMem Configuration.
  @param[in] MasterClockIndex  SSP Master Clock Index
*/
typedef
VOID
(*HDA_CONFIGURE_SSP_MASTER_CLOCK_GPIO) (
  IN HDAUDIO_PREMEM_CONFIG     *HdaPreMemConfig,
  IN UINT32                    MasterClockIndex
  );

/*
  Configure Sndw Audio Link GPIOs

  @param[in] SndwIndex       Sndw Link Index
*/
typedef
VOID
(*HDA_CONFIGURE_SNDW_LINK_GPIO) (
  IN  UINT32                 SndwIndex
  );

/*
  Configure Sndw Multiline
*/
typedef
EFI_STATUS
(*HDA_CONFIGURE_SNDW_MULTILANE) (
  IN HDAUDIO_PREMEM_CONFIG         *HdaPreMemConfig,
  IN UINT8                         MultilaneIndex
  );

/*
  Function disables HDA Device on PMC level.
*/
typedef
VOID
(*HDA_PMC_DISABLE_DEVICE) (
  VOID
  );

/*
  Function disables HDA Device on PSF level.
*/
typedef
VOID
(*HDA_PSF_DISABLE_DEVICE) (
  VOID
  );

/*
  Function disables DSP Bar on PSF level.
*/
typedef
VOID
(*HDA_PSF_DISABLE_DSP_BAR) (
  VOID
  );

/*
  Functions initialize IDisp audio codec in SA.
*/
typedef
EFI_STATUS
(*HDA_INITIALIZE_IDISP_AUDIO_CODEC) (
  VOID
  );

/*
  Functions initialize IDisp audio codec frequency in SA.

  @param[in] RequestedBclkFrequency     IDisplay Link clock frequency to be set
  @param[in] RequestedTmode             IDisplay Link T-Mode to be set
*/
typedef
EFI_STATUS
(*HDA_CONIGURE_IDISP_AUDIO_CODEC_FREQUENCY) (
  IN  HDAUDIO_LINK_FREQUENCY      RequestedBclkFrequency,
  IN  HDAUDIO_IDISP_TMODE         RequestedTmode
  );

/*
  Function allow to access to SB register.

  @param[in] Offset            Offset of SB register.
  @param[in] AndData           Mask to AND with the SB register.
  @param[in] OrData            Mask to OR with the SB register.
*/
typedef
VOID
(*HDA_PCR_PID_DSP_AND_THEN_OR32) (
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/*
  Function allow to access to SB register.

  @param[in] Offset            Offset of SB register.

  @retval Value of SB register
*/
typedef
UINT32
(*HDA_PCR_PID_DSP_READ32) (
  IN  UINT32                            Offset
  );

/*
  Function base on type of link and number of link return if link is supported.

  @param[in] AudioLinkType   Link type support to be checked
  @param[in] AudioLinkIndex  Link number

  @retval    TRUE           Link supported
  @retval    FALSE          Link not supported
*/
typedef
BOOLEAN
(*HDA_IS_AUDIO_INTERFACE_SUPPORTED) (
  IN HDAUDIO_LINK_TYPE     AudioLinkType,
  IN UINT32                AudioLinkIndex
  );

/*
  Function check if Tmode is supported for IDisp codec.

  @retval    TRUE           Tmode supported
  @retval    FALSE          Tmode not supported
*/
typedef
BOOLEAN
(*HDA_IS_AUDIO_IDISP_TMODE_SUPPORTED) (
  IN HDAUDIO_IDISP_TMODE Tmode
  );

/*
  Configure Microphone Privacy related options that are platform specific
*/
typedef
EFI_STATUS
(*HDA_CONFIGURE_MICROPHONE_PRIVACY) (
  VOID
  );

/**
  HDA callbacks
  List of function pointers can be passed to the IP Block driver
**/
typedef struct {
  HDA_CONFIGURE_HDA_LINK_GPIO              HdaConfigureHdaLink;
  HDA_CONFIGURE_DMIC_LINK_GPIO             HdaConfigureDmicLinks;
  HDA_CONFIGURE_SSP_LINK_GPIO              HdaConfigureSspLinks;
  HDA_CONFIGURE_SNDW_LINK_GPIO             HdaConfigureSndwLinks;
  HDA_CONFIGURE_SSP_MASTER_CLOCK_GPIO      HdaConfigureSspMasterClock;
  HDA_PMC_DISABLE_DEVICE                   HdaPmcDisableDevice;
  HDA_PSF_DISABLE_DEVICE                   HdaPsfDisableDevice;
  HDA_PSF_DISABLE_DSP_BAR                  HdaPsfDisableDspBar;
  HDA_INITIALIZE_IDISP_AUDIO_CODEC         HdaInitializeDisplayAudioCodec;
  HDA_CONIGURE_IDISP_AUDIO_CODEC_FREQUENCY HdaConfigureIDispAudioCodecFrequency;
  HDA_PCR_PID_DSP_AND_THEN_OR32            HdaPcrPidDspAndThenOr32;
  HDA_PCR_PID_DSP_READ32                   HdaPcrPidDspRead32;
  HDA_IS_AUDIO_INTERFACE_SUPPORTED         HdaIsAudioInterfaceSupported;
  HDA_IS_AUDIO_IDISP_TMODE_SUPPORTED       HdaIsAudioIDispTmodeSupported;
  HDA_CONFIGURE_SNDW_MULTILANE             HdaConfigureSndwMultilane;
  HDA_CONFIGURE_MICROPHONE_PRIVACY         HdaConfigureMicrophonePrivacy;
} HDA_CALLBACK;

/**
  Internal HDA policy options
**/
typedef struct {
  UINT32     NodeId;
  UINT8      HdaMaxSndwLinkNum;
  UINT8      HdaMaxSndwMultilaneNum;
  UINT8      HdaMaxDmicLinkNum;
  UINT8      HdaMaxSspLinkNum;
  UINT8      IGfxEnabled;
  UINT8      SkipIGfxProgramming;
  UINT8      TrafficClassAssignmentUsb;
  UINT8      MicPrivacySupported;
} HDA_PRIVATE_CONFIG;

/**
  HDA handle structure
  Stores all data necessary to initialize HDA IP block (cAVS and ACE)
**/
typedef struct {
  HDA_CONTROLLER            *HdaController;
  HDAUDIO_CONFIG            *HdaConfig;
  HDAUDIO_PREMEM_CONFIG     *HdaPreMemConfig;
  HDA_PRIVATE_CONFIG        *HdaPrivateConfig;
  HDA_CALLBACK              *HdaCallback;
} HDA_HANDLE;

/**
  Polling the Status bit.
  Maximum polling time (us) equals HDA_MAX_LOOP_TIME * HDA_WAIT_PERIOD.

  @param[in] StatusReg            The register address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
EFI_STATUS
StatusPolling (
  IN      UINTN           StatusReg,
  IN      UINT32          PollingBitMap,
  IN      UINT32          PollingData
  );

/**
  Polling the Status bit.
  Maximum polling time (us) equals HDA_MAX_LOOP_TIME * HDA_WAIT_PERIOD.

  @param[in] StatusReg            The register address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
EFI_STATUS
StatusPolling16 (
  IN      UINTN           StatusReg,
  IN      UINT16          PollingBitMap,
  IN      UINT16          PollingData
  );

/**
  Print HDA handle, controller and callbacks information.

  @param[in] HdaHandle          Pointer to Hda controller configuration structure
**/
VOID
PrintHdaHandle (
  IN  HDA_HANDLE                *HdaHandle
  );

#endif
