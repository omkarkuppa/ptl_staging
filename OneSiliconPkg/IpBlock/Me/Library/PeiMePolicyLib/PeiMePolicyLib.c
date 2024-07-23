/** @file
  This file is PeiMePolicy library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PeiMePolicyLib.h>
#include <MePeiConfig.h>
#include <MkhiMsgs.h>
#include <PchPolicyCommon.h>

/**
  Load default settings for ME config block in pre-mem phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMePeiPreMemDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  ME_PEI_PREMEM_CONFIG *MePeiPreMemConfig;

  MePeiPreMemConfig = ConfigBlockPointer;

  MePeiPreMemConfig->HeciTimeouts       = 1;

  //
  // Test policies
  //
  MePeiPreMemConfig->KtDeviceEnable     = PLATFORM_POR;
  MePeiPreMemConfig->HeciCommunication  = PLATFORM_POR;
  MePeiPreMemConfig->HeciCommunication2 = PLATFORM_POR;
  MePeiPreMemConfig->HeciCommunication3 = PLATFORM_POR;
}

/**
  Load default settings for ME config block in PEI phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMePeiDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  ME_PEI_CONFIG *MePeiConfig;
  MePeiConfig = ConfigBlockPointer;

  MePeiConfig->EndOfPostMessage     = EOP_SEND_IN_DXE;
  MePeiConfig->MeUnconfigOnRtcClear = 1;
  MePeiConfig->CseDataResilience    = 1;
}

STATIC COMPONENT_BLOCK_ENTRY  mMePreMemIpBlock [] = {
  {&gMePeiPreMemConfigGuid, sizeof (ME_PEI_PREMEM_CONFIG),  ME_PEI_PREMEM_CONFIG_REVISION,  LoadMePeiPreMemDefault}
};

STATIC COMPONENT_BLOCK_ENTRY  mMeIpBlock [] = {
  {&gMePeiConfigGuid,       sizeof (ME_PEI_CONFIG),         ME_PEI_CONFIG_REVISION,         LoadMePeiDefault}
};


/**
  Print PEI ME config block

  @param[in] SiPolicyPpiPreMem The RC Policy PPI instance
**/
VOID
EFIAPI
MePrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPpiPreMem
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  ME_PEI_PREMEM_CONFIG              *MePeiPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpiPreMem, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ ME_PEI_PREMEM_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                  : 0x%x\n", MePeiPreMemConfig->Header.Revision));

  DEBUG ((DEBUG_INFO, " HeciTimeouts              : 0x%x\n", MePeiPreMemConfig->HeciTimeouts));
  DEBUG ((DEBUG_INFO, " DidInitStat               : 0x%x\n", MePeiPreMemConfig->DidInitStat));
  DEBUG ((DEBUG_INFO, " DisableCpuReplacedPolling : 0x%x\n", MePeiPreMemConfig->DisableCpuReplacedPolling));
  DEBUG ((DEBUG_INFO, " DisableMessageCheck       : 0x%x\n", MePeiPreMemConfig->DisableMessageCheck));
  DEBUG ((DEBUG_INFO, " SkipMbpHob                : 0x%x\n", MePeiPreMemConfig->SkipMbpHob));
  DEBUG ((DEBUG_INFO, " HeciCommunication         : 0x%x\n", MePeiPreMemConfig->HeciCommunication));
  DEBUG ((DEBUG_INFO, " HeciCommunication2        : 0x%x\n", MePeiPreMemConfig->HeciCommunication2));
  DEBUG ((DEBUG_INFO, " HeciCommunication3        : 0x%x\n", MePeiPreMemConfig->HeciCommunication3));
  DEBUG ((DEBUG_INFO, " KtDeviceEnable            : 0x%x\n", MePeiPreMemConfig->KtDeviceEnable));
  DEBUG ((DEBUG_INFO, " SkipCpuReplacementCheck   : 0x%x\n", MePeiPreMemConfig->SkipCpuReplacementCheck));
  DEBUG_CODE_END ();
}

/**
  Print PEI ME config block

  @param[in] SiPolicyPpi The RC Policy PPI instance
**/
VOID
EFIAPI
MePrintPolicyPpi (
  IN  SI_POLICY_PPI *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  ME_PEI_CONFIG                     *MePeiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ ME_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                  : 0x%x\n", MePeiConfig->Header.Revision));

  DEBUG ((DEBUG_INFO, " MctpBroadcastCycle        : 0x%x\n", MePeiConfig->MctpBroadcastCycle));
  DEBUG ((DEBUG_INFO, " EndOfPostMessage          : 0x%x\n", MePeiConfig->EndOfPostMessage));
  DEBUG ((DEBUG_INFO, " DisableD0I3SettingForHeci : 0x%x\n", MePeiConfig->DisableD0I3SettingForHeci));
  DEBUG ((DEBUG_INFO, " MeUnconfigOnRtcClear      : 0x%x\n", MePeiConfig->MeUnconfigOnRtcClear));
  DEBUG ((DEBUG_INFO, " CseDataResilience         : 0x%x\n", MePeiConfig->CseDataResilience));
  DEBUG ((DEBUG_INFO, " SseCommunication          : 0x%x\n", MePeiConfig->SseCommunication));
  DEBUG ((DEBUG_INFO, " PseEomFlowEnable          : 0x%x\n", MePeiConfig->PseEomFlowEnable));
  DEBUG_CODE_END ();
}

/**
  Get ME config block table total size.

  @retval        Size of ME config block table
**/
UINT16
EFIAPI
MeGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mMePreMemIpBlock[0], sizeof (mMePreMemIpBlock) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Get ME config block table total size.

  @retval        Size of ME config block table
**/
UINT16
EFIAPI
MeGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mMeIpBlock[0], sizeof (mMeIpBlock) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  MeAddConfigBlocksPreMem add all config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeAddConfigBlocksPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "Me AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mMePreMemIpBlock) / sizeof (COMPONENT_BLOCK_ENTRY)));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mMePreMemIpBlock[0], sizeof (mMePreMemIpBlock) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  MeAddConfigBlocks add all config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "ME AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mMeIpBlock) / sizeof (COMPONENT_BLOCK_ENTRY)));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mMeIpBlock[0], sizeof (mMeIpBlock) / sizeof (COMPONENT_BLOCK_ENTRY));
}
