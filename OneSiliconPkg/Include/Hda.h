/** @file
  Header file for HD Audio configuration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _HDA_H_
#define _HDA_H_

typedef enum {
  HdaVc0 = 0,
  HdaVc1 = 1
} HDAUDIO_VC_TYPE;

typedef enum {
  HdaLinkFreq6MHz  = 0,
  HdaLinkFreq12MHz = 1,
  HdaLinkFreq24MHz = 2,
  HdaLinkFreq48MHz = 3,
  HdaLinkFreq96MHz = 4,
  HdaLinkFreqInvalid
} HDAUDIO_LINK_FREQUENCY;

typedef enum  {
  HdaIDispMode2T  = 0,
  HdaIDispMode1T  = 1,
  HdaIDispMode4T  = 2,
  HdaIDispMode8T  = 3,
  HdaIDispMode16T = 4,
  HdaIDispTModeInvalid
} HDAUDIO_IDISP_TMODE;

typedef enum {
  HdaLink          = 0,
  HdaIDispLink     = 1,
  HdaDmic          = 2,
  HdaSsp           = 3,
  HdaSndw          = 4,
  HdaSndwMultilane = 5,
  HdaLinkUnsupported
} HDAUDIO_LINK_TYPE;

typedef enum {
  HdaDmicClockSelectBoth = 0,
  HdaDmicClockSelectClkA = 1,
  HdaDmicClockSelectClkB = 2,
  HdaDmicClockSelectInvalid
} HDAUDIO_DMIC_CLOCK_SELECT;

typedef enum {
  HdaSndwMultilaneDisabled       = 0,
  HdaSndwMultilaneTwoDataLanes   = 1,
  HdaSndwMultilaneThreeDataLanes = 2,
  HdaSndwMultilaneFourDataLanes  = 3
} HDAUDIO_SNDW_MULTILANE_SELECT;

typedef enum {
  HdaSndwDataOnDelay3ClockPeriods = 0,
  HdaSndwDataOnDelay4ClockPeriods = 1,
  HdaSndwDataOnDelayInvalid
} HDAUDIO_SNDW_DATA_ON_DELAY;

typedef enum {
  HdaSndwDataOnActiveIntervalSelect3ClockPeriods  = 0,
  HdaSndwDataOnActiveIntervalSelect4ClockPeriods  = 1,
  HdaSndwDataOnActiveIntervalSelect5ClockPeriods  = 2,
  HdaSndwDataOnActiveIntervalSelect6ClockPeriods =  3,
  HdaSndwDataOnActiveIntervalSelectInvalid
} HDAUDIO_SNDW_DATA_ON_ACTIVE_INTERVAL_SELECT;

typedef enum {
  HdaNoMicrophonePrivacySupport  = 0,
  HdaHwManagedMicrophonePrivacy  = 1,
  HdaFwManagedMicrophonePrivacy  = 2,
  HdaForceMicrophoneMute         = 3
} HDAUDIO_MIC_PRIVACY_MODE;

typedef enum {
  HdaSndwClockSourceSelectXtal     = 0,
  HdaSndwClockSourceSelectAudioPll = 1,
  HdaSndwClockSourceSelectInvalid
} HDAUDIO_SNDW_CLOCK_SRC_SELECT;

#endif
