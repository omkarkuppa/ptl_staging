/** @file
  This file provides services for Pmc policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/PeiPmcPolicyCommonLib.h>
#include <Library/PeiPmcPolicyLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <PmConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PmcLoadConfigDefaultPtl (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PM_CONFIG  *PmConfig;
  PmConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PmConfig->Header.GuidHob.Name = %g\n", &PmConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PmConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PmConfig->Header.GuidHob.Header.HobLength));

  PmConfig->MeWakeSts                            = TRUE;
  PmConfig->WolOvrWkSts                          = TRUE;

  PmConfig->WakeConfig.WolEnableOverride         = TRUE;
  PmConfig->WakeConfig.LanWakeFromDeepSx         = FALSE;

  PmConfig->PchSlpS3MinAssert                    = PchSlpS350ms;
  PmConfig->PchSlpS4MinAssert                    = PchSlpS41s;
  PmConfig->PchSlpSusMinAssert                   = PchSlpSus4s;
  PmConfig->PchSlpAMinAssert                     = PchSlpA2s;

  PmConfig->SlpLanLowDc                          = TRUE;
  PmConfig->PciePllSsc                           = 0xFF;
  PmConfig->CpuC10GatePinEnable                  = TRUE;
  PmConfig->OsIdleEnable                         = TRUE;
  PmConfig->V1p05PhyExtFetControlEn              = TRUE;

  PmConfig->S0ixAutoDemotion                     = TRUE;

  PmConfig->LpmS0ixSubStateEnable.Val            = 0xFF;
  PmConfig->Usb2PhySusPgEnable                   = TRUE;

  PmConfig->PsOnEnable                           = FALSE;
  PmConfig->PchPwrCycDur                         = 0xC7;
  PmConfig->DisableEnergyReport                  = FALSE;
  PmConfig->PmErDebugMode                        = TRUE;
  PmConfig->PmcWdtTimerEn                        = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PmcBclkLoadConfigDefaultPtl (
  IN VOID          *ConfigBlockPointer
  )
{
  BCLK_CONFIG  *BclkConfig;
  BclkConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "BclkConfig->Header.GuidHob.Name = %g\n", &BclkConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "BclkConfig->Header.GuidHob.Header.HobLength = 0x%x\n", BclkConfig->Header.GuidHob.Header.HobLength));
}


STATIC COMPONENT_BLOCK_ENTRY mPmcBlocks[] = {
  {&gPmConfigGuid, sizeof (PCH_PM_CONFIG), PCH_PM_CONFIG_REVISION, PmcLoadConfigDefaultPtl},
  {&gBclkConfigGuid, sizeof (BCLK_CONFIG), BCLK_CONFIG_REVISION, PmcBclkLoadConfigDefaultPtl}
};

/**
  Get Pmc config block table size.

  @retval      Size of config block
**/
UINT16
PmcGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mPmcBlocks[0], sizeof (mPmcBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Add Pmc ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PmcAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mPmcBlocks[0], sizeof (mPmcBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
