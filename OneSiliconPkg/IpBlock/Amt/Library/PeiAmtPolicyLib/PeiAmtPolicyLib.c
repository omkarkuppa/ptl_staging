/** @file
  This file is PeiAmtPolicy library.

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <AmtConfig.h>
#include <Ppi/SiPolicy.h>

/**
  Load default settings for AMT config block in PEI phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadAmtPeiDefault (
  IN VOID                           *ConfigBlockPointer
  );

STATIC COMPONENT_BLOCK_ENTRY  mAmtIpBlocks [] = {
  {&gAmtPeiConfigGuid,      sizeof (AMT_PEI_CONFIG),          AMT_PEI_CONFIG_REVISION,      LoadAmtPeiDefault},
};

/**
  Get AMT config block table total size.

  @retval        Size of AMT config block table
**/
UINT16
EFIAPI
AmtGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mAmtIpBlocks[0], sizeof (mAmtIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  AmtAddConfigBlocks add all AMT config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add AMT config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AmtAddConfigBlocks (
  IN VOID      *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "AMT AddConfigBlocks\n"));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mAmtIpBlocks[0], sizeof (mAmtIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Load default settings for AMT config block in PEI phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadAmtPeiDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  AMT_PEI_CONFIG        *AmtPeiConfig;

  AmtPeiConfig = ConfigBlockPointer;

  AmtPeiConfig->AmtEnabled = TRUE;
}

/**
  Dump values of AMT config block in PEI phase.

  @param[in] AmtPeiConfig                   The pointer to the config block
**/
VOID
EFIAPI
PrintAmtPeiConfig (
  IN  AMT_PEI_CONFIG            *AmtPeiConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------------ AMT_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision          : 0x%x\n", AmtPeiConfig->Header.Revision));

  DEBUG ((DEBUG_INFO, " AmtEnabled        : 0x%x\n", AmtPeiConfig->AmtEnabled));
  DEBUG ((DEBUG_INFO, " WatchDogEnabled   : 0x%x\n", AmtPeiConfig->WatchDogEnabled));
  DEBUG ((DEBUG_INFO, " WatchDogTimerOs   : 0x%x\n", AmtPeiConfig->WatchDogTimerOs));
  DEBUG ((DEBUG_INFO, " WatchDogTimerBios : 0x%x\n", AmtPeiConfig->WatchDogTimerBios));
  DEBUG ((DEBUG_INFO, " AmtSolEnabled     : 0x%x\n", AmtPeiConfig->AmtSolEnabled));
}

/**
  Print PEI AMT Config Block

  @param[in] SiPolicyPpi The RC Policy PPI instance
**/
VOID
EFIAPI
AmtPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  AMT_PEI_CONFIG                    *AmtPeiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gAmtPeiConfigGuid, (VOID *) &AmtPeiConfig);
  ASSERT_EFI_ERROR (Status);

  PrintAmtPeiConfig (AmtPeiConfig);

  DEBUG_CODE_END ();
}