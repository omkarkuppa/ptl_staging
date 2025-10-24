/** @file
  This file provides services for CNVi PEI pre-mem policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <CnviConfig.h>

/**
  Print CNVI_PREMEM_CONFIG settings.

  @param[in] SiPreMemPolicyPpi  Instance of SI_PREMEM_POLICY_PPI
**/
VOID
CnviPreMemPrintConfig (
  IN CONST SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  )
{
  EFI_STATUS         Status;
  CNVI_PREMEM_CONFIG *CnviPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCnviPreMemConfigGuid, (VOID *) &CnviPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "DDR RFI Mitigation = %x\n", CnviPreMemConfig->DdrRfim));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer  Pointer to config block
**/
VOID
CnviPreMemLoadConfigDefault (
  IN VOID *ConfigBlockPointer
  )
{
  CNVI_PREMEM_CONFIG *CnviPreMemConfig;

  CnviPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CnviPreMemConfig->Header.GuidHob.Name = %g\n", &CnviPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CnviPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CnviPreMemConfig->Header.GuidHob.Header.HobLength));

  CnviPreMemConfig->DdrRfim = TRUE;  // DDR RFI Mitigation
}

STATIC COMPONENT_BLOCK_ENTRY mCnviPreMemBlock = {
  &gCnviPreMemConfigGuid,
  sizeof (CNVI_PREMEM_CONFIG),
  CNVI_PREMEM_CONFIG_REVISION,
  CnviPreMemLoadConfigDefault
};

/**
  Get CNVi PreMem config block table size.

  @retval Size of config block
**/
UINT16
CnviPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mCnviPreMemBlock.Size;
}

/**
  Add CNVi PreMem ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
CnviPreMemAddConfigBlock (
  IN VOID *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mCnviPreMemBlock, 1);
  return Status;
}
