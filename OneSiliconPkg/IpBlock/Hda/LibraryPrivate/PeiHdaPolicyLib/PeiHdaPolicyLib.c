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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Hda.h>
#include <Ppi/SiPolicy.h>
#include <HdAudioConfig.h>

/**
  Print HDAUDIO_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
HdaPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  HDAUDIO_CONFIG    *HdaConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHdAudioConfigGuid, (VOID *) &HdaConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ HDA Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Pme                                            = %x\n", HdaConfig->Pme));
  DEBUG ((DEBUG_INFO, " Codec Sx Wake Capability                       = %x\n", HdaConfig->CodecSxWakeCapability));
  DEBUG ((DEBUG_INFO, " HD-A Link Frequency                            = %x\n", HdaConfig->HdAudioLinkFrequency));
  DEBUG ((DEBUG_INFO, " VerbTableEntryNum                              = %x\n", HdaConfig->VerbTableEntryNum));
  DEBUG ((DEBUG_INFO, " VerbTablePtr                                   = %x\n", HdaConfig->VerbTablePtr));
  DEBUG ((DEBUG_INFO, "-------- HDA Microphone Privacy Config ---------\n"));
  DEBUG ((DEBUG_INFO, " Mode                                           = %x\n", HdaConfig->MicPrivacy.Mode));
  DEBUG ((DEBUG_INFO, " Deglitch                                       = %x\n", HdaConfig->MicPrivacy.Deglitch));
  DEBUG ((DEBUG_INFO, " Hw Mode SoundWire[0]                           = %x\n", HdaConfig->MicPrivacy.HwModeSoundWire0));
  DEBUG ((DEBUG_INFO, " Hw Mode SoundWire[1]                           = %x\n", HdaConfig->MicPrivacy.HwModeSoundWire1));
  DEBUG ((DEBUG_INFO, " Hw Mode SoundWire[2]                           = %x\n", HdaConfig->MicPrivacy.HwModeSoundWire2));
  DEBUG ((DEBUG_INFO, " Hw Mode SoundWire[3]                           = %x\n", HdaConfig->MicPrivacy.HwModeSoundWire3));
  DEBUG ((DEBUG_INFO, " Hw Mode SoundWire[4]                           = %x\n", HdaConfig->MicPrivacy.HwModeSoundWire4));
  DEBUG ((DEBUG_INFO, " Hw Mode DMIC                                   = %x\n", HdaConfig->MicPrivacy.HwModeDmic));
  DEBUG ((DEBUG_INFO, " Timeout                                        = %d\n", HdaConfig->MicPrivacy.Timeout));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HdaLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  HDAUDIO_CONFIG   *HdaConfig;
  HdaConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HdaConfig->Header.GuidHob.Name = %g\n", &HdaConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HdaConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdaConfig->Header.GuidHob.Header.HobLength));
  /********************************
    HD-Audio configuration
  ********************************/
  HdaConfig->HdAudioLinkFrequency                          = HdaLinkFreq24MHz;
  HdaConfig->MicPrivacy.Mode                               = HdaHwManagedMicrophonePrivacy;
  HdaConfig->MicPrivacy.Deglitch                           = TRUE;
  HdaConfig->MicPrivacy.HwModeSoundWire0                   = FALSE;
  HdaConfig->MicPrivacy.HwModeSoundWire1                   = FALSE;
  HdaConfig->MicPrivacy.HwModeSoundWire2                   = FALSE;
  HdaConfig->MicPrivacy.HwModeSoundWire3                   = FALSE;
  HdaConfig->MicPrivacy.HwModeSoundWire4                   = FALSE;
  HdaConfig->MicPrivacy.HwModeDmic                         = FALSE;
  HdaConfig->MicPrivacy.Timeout                            = 100;
}

STATIC COMPONENT_BLOCK_ENTRY  mHdaBlocks = {
  &gHdAudioConfigGuid,
  sizeof (HDAUDIO_CONFIG),
  HDAUDIO_CONFIG_REVISION,
  HdaLoadConfigDefault
};

/**
  Get Hda config block table size.

  @retval      Size of config block
**/
UINT16
HdaGetConfigBlockTotalSize (
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
HdaAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mHdaBlocks, 1);
}
