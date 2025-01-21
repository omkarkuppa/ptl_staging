/** @file
  PCH HD Audio Sx handler implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include "PchInitSmm.h"
#include <Register/PchRegs.h>
#include <Register/HdaRegs.h>
#include <Library/PchPciBdfLib.h>
#include <HdaConfigHob.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT64  mHdaPciBase;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN mHdaCodecSxWakeCapability = FALSE;

/**
  Updates Codec Sx Wake Capability setting: disabled/enabled
**/
VOID
UpdateHdaCodecSxWakeCapabilitySetting (
  VOID
  )
{
  VOID                *HobPtr;
  HDAUDIO_CONFIG_HOB  *HdaConfigHob;

  mHdaPciBase = HdaPciCfgBase ();

  //
  // Get HDAUDIO_CONFIG_HOB
  //
  HobPtr   = GetFirstGuidHob (&gPchHdaConfigHobGuid);
  ASSERT (HobPtr != NULL);
  HdaConfigHob = (HDAUDIO_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);

  if ((HdaConfigHob->CodecSxWakeCapability == FALSE) ||
      (PciSegmentRead16 (mHdaPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF)) {
    mHdaCodecSxWakeCapability = FALSE;
  } else {
    mHdaCodecSxWakeCapability = TRUE;
  }
}

/**
  HDA Controller Configurations Prior to Entering Sx
**/
VOID
PchHdaSxCallback (
  VOID
  )
{
  if (mHdaCodecSxWakeCapability == FALSE) {
    return;
  }

  PciSegmentOr32 (mHdaPciBase + R_HDA_CFG_PGCTL, (UINT32) B_HDA_CFG_PGCTL_CTLPGD);
}
