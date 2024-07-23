/** @file
  This file provides services for PchDmi policy function

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
#include <Library/PchInfoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <PchDmiConfig.h>
#include <DmiConfig.h>

extern EFI_GUID gPchDmiConfigGuid;
extern EFI_GUID gDmiConfigGuid;

/**
  Print PCH_DMI_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
PchDmiPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  PCH_DMI_CONFIG    *PchDmiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchDmiConfigGuid, (VOID *) &PchDmiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ DMI Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " PwrOptEnable= %x\n", PchDmiConfig->PwrOptEnable));
  DEBUG ((DEBUG_INFO, " DmiAspmCtrl = %x\n", PchDmiConfig->DmiAspmCtrl));
  DEBUG ((DEBUG_INFO, " CwbEnable= %x\n", PchDmiConfig->CwbEnable));
  DEBUG ((DEBUG_INFO, " DmiPowerReduction= %x\n", PchDmiConfig->DmiPowerReduction));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PchDmiLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_DMI_CONFIG   *PchDmiConfig;
  PchDmiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchDmiConfig->Header.GuidHob.Name = %g\n", &PchDmiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchDmiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchDmiConfig->Header.GuidHob.Header.HobLength));

  PchDmiConfig->CwbEnable = 1;
  if (IsAdlPch () && IsPchS ()) {
    PchDmiConfig->DmiAspmCtrl = PchPcieAspmL1;
    PchDmiConfig->L1RpCtl = FALSE;
  } else {
    PchDmiConfig->DmiAspmCtrl = PchPcieAspmAutoConfig;
    PchDmiConfig->L1RpCtl = TRUE;
  }
  PchDmiConfig->DmiPowerReduction = TRUE;
}

STATIC COMPONENT_BLOCK_ENTRY  mPchDmiBlocks [] = {
  {
    &gPchDmiConfigGuid,
    sizeof (PCH_DMI_CONFIG),
    PCH_DMI_CONFIG_REVISION,
    PchDmiLoadConfigDefault
  }
};

/**
  Get PchDmi config block table size.

  @retval      Size of config block
**/
UINT16
PchDmiGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mPchDmiBlocks[0], sizeof (mPchDmiBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Add PchDmi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PchDmiAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mPchDmiBlocks[0], sizeof (mPchDmiBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Load Config block default
  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
DmiUpLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  DMI_CONFIG   *DmiUpConfig;
  DmiUpConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "DmiConfig->Header.GuidHob.Name = %g\n", &DmiUpConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "DmiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", DmiUpConfig->Header.GuidHob.Header.HobLength));
}

STATIC COMPONENT_BLOCK_ENTRY  mDmiBlocks [1] = {
  {
    &gDmiConfigGuid,
    sizeof (DMI_CONFIG),
    DMI_CONFIG_REVISION,
    DmiUpLoadConfigDefault
  }
};

/**
  Get Dmi config block table size.
  @retval      Size of config block
**/
UINT16
DmiUpGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mDmiBlocks[0], sizeof (mDmiBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Add Dmi ConfigBlock.
  @param[in] ConfigBlockTableAddress    The pointer to config block table
  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
DmiUpAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mDmiBlocks[0], sizeof (mDmiBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}