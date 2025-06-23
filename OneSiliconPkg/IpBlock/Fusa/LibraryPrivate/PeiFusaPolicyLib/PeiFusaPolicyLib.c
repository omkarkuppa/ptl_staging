/** @file
  This file provides services for Functional Safety policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Ppi/SiPolicy.h>
#include <FusaConfig.h>

/**
  Print FUSA_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI

  @retval EFI_SUCCESS               The policy default is printed
**/
EFI_STATUS
FusaPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS    Status;
  FUSA_CONFIG    *FusaConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gFusaConfigGuid, (VOID *) &FusaConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ Fusa Config ------------------------\n"));
  DEBUG ((DEBUG_INFO, " DisplayFusaConfigEnable: 0x%X\n", FusaConfig->DisplayFusaConfigEnable));
  DEBUG ((DEBUG_INFO, " GraphicFusaConfigEnable: 0x%X\n", FusaConfig->GraphicFusaConfigEnable));
  DEBUG ((DEBUG_INFO, " OpioFusaConfigEnable   : 0x%X\n", FusaConfig->OpioFusaConfigEnable));
  DEBUG ((DEBUG_INFO, " PsfFusaConfigEnable    : 0x%X\n", FusaConfig->PsfFusaConfigEnable));
  DEBUG ((DEBUG_INFO, " IopFusaConfigEnable    : 0x%X\n", FusaConfig->IopFusaConfigEnable));
  DEBUG ((DEBUG_INFO, " IopFusaMcaCheckEnable  : 0x%X\n", FusaConfig->IopFusaMcaCheckEnable));
  return EFI_SUCCESS;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
FusaLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  FUSA_CONFIG        *FusaConfig;
  FusaConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FusaConfig->Header.GuidHob.Name = %g\n", &FusaConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FusaConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FusaConfig->Header.GuidHob.Header.HobLength));

  FusaConfig->IopFusaMcaCheckEnable = 1;
  return;
}

STATIC COMPONENT_BLOCK_ENTRY  mFusaBlocks = {
  &gFusaConfigGuid,
  sizeof (FUSA_CONFIG),
  FUSA_CONFIG_REVISION,
  FusaLoadConfigDefault
};

/**
  Get Fusa config block table size.

  @retval      Size of config block
**/
UINT16
FusaGetConfigBlockTotalSize (
  VOID
  )
{
  return mFusaBlocks.Size;
}

/**
  Add Fusa ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
FusaAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mFusaBlocks, 1);
}
