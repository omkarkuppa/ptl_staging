/** @file
  PEI Module adds config blocks for FspV.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <FspVConfig.h>
#include <Library/FspVPolicyLib.h>

/**
  Print FSPV_PREMEM_CONFIG.

  @param[in] SiPreMemPolicyPpi  Pointer to a SI_PREMEM_POLICY_PPI

**/
VOID
FspVPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS          Status;
  FSPV_PREMEM_CONFIG  *FspVPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gFspVPreMemConfigGuid, (VOID *) &FspVPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR ((Status))) {
    return;
  }

  DEBUG ((DEBUG_INFO, "------------------ FspV PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "Pre-Mem TestId = %x\n", FspVPreMemConfig->TestId));
}

/**
  Print FSPV_POSTMEM_CONFIG.

  @param[in] SiPolicyPpi  Pointer to a SI_POLICY_PPI

**/
VOID
FspVPostMemPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS           Status;
  FSPV_POSTMEM_CONFIG  *FspVPostMemConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gFspVConfigGuid, (VOID *) &FspVPostMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR ((Status))) {
    return;
  }

  DEBUG ((DEBUG_INFO, "------------------ FspV PostMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "Post-Mem TestId = %x\n", FspVPostMemConfig->TestId));
}

/**
  Load Pre-Mem Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
FspVPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  FSPV_PREMEM_CONFIG  *FspVPreMemConfig;
  FspVPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FspVPreMemConfig->Header.GuidHob.Name = %g\n", &FspVPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FspVPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FspVPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    FspV Pre-Mem Configuration
  ********************************/
  FspVPreMemConfig->TestId = RunAllFspmTest;

}

/**
  Load Post-Mem Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
FspVPostMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  FSPV_POSTMEM_CONFIG  *FspVPostMemConfig;
  FspVPostMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FspVPostMemConfig->Header.GuidHob.Name = %g\n", &FspVPostMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FspVPostMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FspVPostMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    FspV Post-Mem Configuration
  ********************************/
  FspVPostMemConfig->TestId = RunAllFspsTest;

}

STATIC COMPONENT_BLOCK_ENTRY  mFspVPreMemBlocks = {
  &gFspVPreMemConfigGuid,
  sizeof (FSPV_PREMEM_CONFIG),
  FSPV_PREMEM_CONFIG_REVISION,
  FspVPreMemLoadConfigDefault
};

STATIC COMPONENT_BLOCK_ENTRY  mFspVPostMemBlocks = {
  &gFspVConfigGuid,
  sizeof (FSPV_POSTMEM_CONFIG),
  FSPV_POSTMEM_CONFIG_REVISION,
  FspVPostMemLoadConfigDefault
};

/**
  Get FspV pre-mem config block table size.

  @retval      Size of config block
**/
UINT16
FspVPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mFspVPreMemBlocks.Size;
}

/**
  Get FspV post-mem config block table size.

  @retval      Size of config block
**/
UINT16
FspVPostMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mFspVPostMemBlocks.Size;
}

/**
  Add FspV Pre-Mem ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
FspVPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mFspVPreMemBlocks, 1);
}

/**
  Add FspV Post-Mem ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
FspVPostMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mFspVPostMemBlocks, 1);
}
