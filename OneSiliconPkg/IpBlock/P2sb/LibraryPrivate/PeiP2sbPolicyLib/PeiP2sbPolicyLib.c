/** @file
  This file provides services for P2sb policy function

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
#include <Ppi/SiPolicy.h>
#include <P2sbConfig.h>

/**
  Print PCH_P2SB_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
P2sbPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  PCH_P2SB_CONFIG   *P2sbConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ P2SB Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "SbAccessUnlock= %x\n", P2sbConfig->SbAccessUnlock));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
P2sbLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_P2SB_CONFIG  *P2sbConfig;
  P2sbConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "P2sbConfig->Header.GuidHob.Name = %g\n", &P2sbConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "P2sbConfig->Header.GuidHob.Header.HobLength = 0x%x\n", P2sbConfig->Header.GuidHob.Header.HobLength));
}

STATIC COMPONENT_BLOCK_ENTRY  mP2sbBlocks = {
  &gP2sbConfigGuid,
  sizeof (PCH_P2SB_CONFIG),
  PCH_P2SB_CONFIG_REVISION,
  P2sbLoadConfigDefault
};

/**
  Get P2sb config block table size.

  @retval      Size of config block
**/
UINT16
P2sbGetConfigBlockTotalSize (
  VOID
  )
{
  return mP2sbBlocks.Size;
}

/**
  Add P2sb ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
P2sbAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mP2sbBlocks, 1);
}
