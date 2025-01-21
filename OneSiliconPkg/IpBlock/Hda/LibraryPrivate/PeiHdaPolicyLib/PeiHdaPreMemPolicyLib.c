/** @file
  This file provides services for Hda PreMem policy function

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
#include <Library/PchInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <HdAudioConfig.h>

/**
  Print PCH_HDAUDIO_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
HdaPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                Status;
  HDAUDIO_PREMEM_CONFIG     *HdaPreMemConfig;
  UINTN                     Index;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ HD Audio PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable                                         = %x\n", HdaPreMemConfig->Enable));
  DEBUG ((DEBUG_INFO, " DSP Enable                                     = %x\n", HdaPreMemConfig->DspEnable));
  DEBUG ((DEBUG_INFO, " DSP UAA Compliance                             = %x\n", HdaPreMemConfig->DspUaaCompliance));
  DEBUG ((DEBUG_INFO, " Audio Link: HDA Link                           = %x\n", HdaPreMemConfig->AudioLinkHda.Enable));
  for (Index = 0; Index < PCH_MAX_HDA_SDI; Index++) {
    DEBUG ((DEBUG_INFO, " Hda Sdi%d Enable                              = %x\n", Index, HdaPreMemConfig->AudioLinkHda.SdiEnable[Index]));
  }
  for (Index = 0; Index < GetPchHdaMaxDmicLinkNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Audio Link: DMIC#%d                            = %x\n", Index, HdaPreMemConfig->AudioLinkDmic[Index].Enable));
    DEBUG ((DEBUG_INFO, " DMIC#%d Clock Select                           = %x\n", Index, HdaPreMemConfig->AudioLinkDmic[Index].DmicClockSelect));
  }
  for (Index = 0; Index < GetPchHdaMaxSspLinkNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Audio Link: SSP#%d                             = %x\n", Index, HdaPreMemConfig->AudioLinkSsp[Index].Enable));
  }
  for (Index = 0; Index < GetPchHdaMaxSndwLinkNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Audio Link: SoundWire#%d                       = %x\n", Index, HdaPreMemConfig->AudioLinkSndw[Index].Enable));
  }
  for (Index = 0; Index < GetPchHdaMaxSndwMultilaneNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Audio Link: SoundWire Multilane#%d             = %x\n", Index, HdaPreMemConfig->SndwMultilane[Index].Enable));
  }
  DEBUG ((DEBUG_INFO, " iDisp Codec Disconnect                         = %x\n", HdaPreMemConfig->IDispCodecDisconnect));
  DEBUG ((DEBUG_INFO, " iDisp Link Frequency                           = %x\n", HdaPreMemConfig->IDispLinkFrequency));
  DEBUG ((DEBUG_INFO, " iDisp Link T-Mode                              = %x\n", HdaPreMemConfig->IDispLinkTmode));
  DEBUG ((DEBUG_INFO, " ResetWaitTimer                                 = %x\n", HdaPreMemConfig->ResetWaitTimer));
  DEBUG ((DEBUG_INFO, " SubSystemIds                                   = %x\n", HdaPreMemConfig->SubSystemIds));
  DEBUG ((DEBUG_INFO, " Power/Clock Gating                             = %x\n", HdaPreMemConfig->PowerClockGating));
  DEBUG ((DEBUG_INFO, " Low Frequency Clock                            = %x\n", HdaPreMemConfig->LowFreqLinkClkSrc));
  DEBUG ((DEBUG_INFO, " SoundWire Clock Source Select                  = %x\n", HdaPreMemConfig->SoundWireClockSelect));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HdaPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT32                 Index;
  HDAUDIO_PREMEM_CONFIG  *HdaPreMemConfig;

  HdaPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HdaPreMemConfig->Header.GuidHob.Name = %g\n", &HdaPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HdaPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdaPreMemConfig->Header.GuidHob.Header.HobLength));

  HdaPreMemConfig->Enable                    = TRUE;
  HdaPreMemConfig->DspEnable                 = TRUE;
  HdaPreMemConfig->AudioLinkHda.Enable       = FALSE;
  HdaPreMemConfig->AudioLinkHda.SdiEnable[0] = TRUE;
  HdaPreMemConfig->AudioLinkHda.SdiEnable[1] = FALSE;

  for (Index = 0; Index < PCH_MAX_HDA_DMIC_LINK_NUM; Index++) {
    HdaPreMemConfig->AudioLinkDmic[Index].Enable = TRUE;
    HdaPreMemConfig->AudioLinkDmic[Index].DmicClockSelect = HdaDmicClockSelectClkA;
  }

  HdaPreMemConfig->AudioLinkSndw[0].Enable = FALSE;
  HdaPreMemConfig->AudioLinkSndw[1].Enable = TRUE;
  HdaPreMemConfig->AudioLinkSndw[2].Enable = FALSE;
  HdaPreMemConfig->AudioLinkSndw[3].Enable = FALSE;
  HdaPreMemConfig->SndwMultilane[0].Enable = FALSE;
  HdaPreMemConfig->SndwMultilane[1].Enable = HdaSndwMultilaneThreeDataLanes;
  HdaPreMemConfig->DspUaaCompliance        = FALSE;
  HdaPreMemConfig->IDispLinkFrequency      = HdaLinkFreq96MHz;
  HdaPreMemConfig->IDispLinkTmode          = HdaIDispMode8T;
  HdaPreMemConfig->PowerGatingSupported    = TRUE;
  HdaPreMemConfig->ResetWaitTimer          = 600; // Must be at least 521us (25 frames)
}

STATIC COMPONENT_BLOCK_ENTRY  mHdaPreMemBlocks = {
  &gHdAudioPreMemConfigGuid,
  sizeof (HDAUDIO_PREMEM_CONFIG),
  HDAUDIO_PREMEM_CONFIG_REVISION,
  HdaPreMemLoadConfigDefault
};

/**
  Get Hda config block table size.

  @retval      Size of config block
**/
UINT16
HdaPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mHdaPreMemBlocks.Size;
}

/**
  Add Hda ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
HdaPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mHdaPreMemBlocks, 1);
}
