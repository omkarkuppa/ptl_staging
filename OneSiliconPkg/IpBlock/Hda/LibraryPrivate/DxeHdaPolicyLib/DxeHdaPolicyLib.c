/** @file
  This file provides services for Hda policy function

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Protocol/PchPolicy.h>
#include <HdAudioConfig.h>
#include <Hda.h>

/**
  Print HDAUDIO_DXE_CONFIG and serial out.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
HdaDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  )
{
  EFI_STATUS            Status;
  HDAUDIO_DXE_CONFIG    *HdaDxeConfig;
  UINT32                Index;

  Status = GetConfigBlock ((VOID *) PchPolicy, &gHdAudioDxeConfigGuid, (VOID *) &HdaDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ HD-Audio DXE Config ------------------\n"));

  for (Index = 0; Index < PCH_MAX_HDA_SNDW_LINK_NUM; Index++) {
    DEBUG ((DEBUG_INFO, " SNDW%d AutonomousClockStop  : %d\n", Index, HdaDxeConfig->SndwConfig[Index].AutonomousClockStop));
    DEBUG ((DEBUG_INFO, " SNDW%d DODS                 : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnDelaySelect));
    DEBUG ((DEBUG_INFO, " SNDW%d DODSE                : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnDelayExtSelect));
    DEBUG ((DEBUG_INFO, " SNDW%d DODSE2               : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnDelayExt2Select));
    DEBUG ((DEBUG_INFO, " SNDW%d DOAIS                : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnActiveIntervalSelect));
    DEBUG ((DEBUG_INFO, " SNDW%d DOAISE               : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnActiveIntervalExtSelect));
    DEBUG ((DEBUG_INFO, " SNDW%d DOAISE2              : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnActiveIntervalExt2Select));
    DEBUG ((DEBUG_INFO, " SNDW%d CLDE                 : %d\n", Index, HdaDxeConfig->SndwConfig[Index].ClockLoopbackEnableSndw));
    DEBUG ((DEBUG_INFO, " SNDW%d CLDS                 : %d\n", Index, HdaDxeConfig->SndwConfig[Index].ClockLoopbackDelaySelectSndw));
    DEBUG ((DEBUG_INFO, " SNDW%d CLSS                 : %d\n", Index, HdaDxeConfig->SndwConfig[Index].ClockLoopbackSourceSndw));
  }
  DEBUG ((DEBUG_INFO, " DSP Feature Mask              : 0x%x\n", HdaDxeConfig->DspFeatureMask));
  DEBUG ((DEBUG_INFO, " Discrete BT Offload Enabled   : %d\n", HdaDxeConfig->HdaDiscBtOffload.HdaDiscBtOffEnabled));
  DEBUG ((DEBUG_INFO, " Discrete BT Offload Ssp Link  : %d\n", HdaDxeConfig->HdaDiscBtOffload.HdaDiscBtOffSspLink));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HdaDxeLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT32              Index;
  HDAUDIO_DXE_CONFIG  *HdAudioDxeConfig;

  HdAudioDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HdaDxeConfig->Header.GuidHob.Name = %g\n", &HdAudioDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HdaDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdAudioDxeConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < PCH_MAX_HDA_SNDW_LINK_NUM; Index++) {
    HdAudioDxeConfig->SndwConfig[Index].DataOnDelayExtSelect          = 0x1;
    HdAudioDxeConfig->SndwConfig[Index].DataOnDelayExt2Select         = 0x1;
    HdAudioDxeConfig->SndwConfig[Index].DataOnActiveIntervalSelect    = 0x3;
    HdAudioDxeConfig->SndwConfig[Index].DataOnActiveIntervalExtSelect = 0x1;
    HdAudioDxeConfig->SndwConfig[Index].ClockLoopbackSourceSndw       = 0x1;
    HdAudioDxeConfig->SndwConfig[Index].ClockLoopbackDelaySelectSndw  = 0x2;
  }

  // WoV; BT Sideband; BT Intel HFP, A2DP, LE; ACX/SDCA and speaker aggregation
  HdAudioDxeConfig->DspFeatureMask = (BIT0 | BIT1 | BIT5 | BIT6 | BIT9);
}

STATIC COMPONENT_BLOCK_ENTRY  mHdaBlocks = {
  &gHdAudioDxeConfigGuid,
  sizeof (HDAUDIO_DXE_CONFIG),
  HDAUDIO_DXE_CONFIG_REVISION,
  HdaDxeLoadConfigDefault
};

/**
  Get Hda config block table size.

  @retval      Size of config block
**/
UINT16
HdaDxeGetConfigBlockTotalSize (
  VOID
  )
{
  return mHdaBlocks.Size;
}

/**
  Add Hda ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
HdaDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mHdaBlocks, 1);
}
