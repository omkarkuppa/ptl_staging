/** @file
  HDA config HOB definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _HDA_CONFIG_HOB_H_
#define _HDA_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>

/**
  The HDAUDIO_CONFIG_HOB block describes the configuration of the PCH cAVS controller
**/
typedef struct {
  UINT32  DspEnable             :  1; ///< DSP enablement: 0: Disable; <b>1: Enable</b>
  UINT32  DspUaaCompliance      :  1; ///< UAA-mode Select: <b>0: Non-Uaa</b>; 1: UAA
  UINT32  CodecSxWakeCapability :  1; ///< Capability to detect wake initiated by a codec in Sx, <b>0: Disable</b>; 1: Enable
  UINT32  AudioLinkSndw0        :  1; ///< SoundWire#0 link enablement: <b>0: Disable</b>; 1: Enable.
  UINT32  AudioLinkSndw1        :  1; ///< SoundWire#1 link enablement: <b>0: Disable</b>; 1: Enable.
  UINT32  AudioLinkSndw2        :  1; ///< SoundWire#2 link enablement: <b>0: Disable</b>; 1: Enable.
  UINT32  AudioLinkSndw3        :  1; ///< SoundWire#3 link enablement: <b>0: Disable</b>; 1: Enable.
  UINT32  AudioLinkSndw4        :  1; ///< SoundWire#4 link enablement: <b>0: Disable</b>; 1: Enable.
  UINT32  AudioLinkSndwLanes2   :  2; ///< SoundWire#2 Multilane enablement: <b>0: Disable</b>; 1: 2-lanes; 2: 3-lanes; 3: 4-lanes
  UINT32  AudioLinkSndwLanes3   :  2; ///< SoundWire#3 Multilane enablement: <b>0: Disable</b>; 1: 2-lanes; 2: 3-lanes; 3: 4-lanes
  UINT32  Pme                   :  1; ///< Azalia wake-on-ring, <b>0: Disable</b>; 1: Enable
  UINT32  SoundWireClockSelect  :  1; ///< SoundWire clock source select: <b> 0: XTAL</b>; 1: Audio PLL.
  UINT32  RsvdBits0             : 18; ///< Reserved bits
  UINT32  AudioXtal;                  ///< Detected XTAL
} HDAUDIO_CONFIG_HOB;

#endif
