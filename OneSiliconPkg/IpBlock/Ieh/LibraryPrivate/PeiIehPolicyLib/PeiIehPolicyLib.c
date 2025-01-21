/** @file
  This file provides services for Ieh policy function

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
#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <IehConfig.h>

/**
  Print IEH_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
IehPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS    Status;
  IEH_CONFIG    *IehConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIehConfigGuid, (VOID *) &IehConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ Ieh Config ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Mode: 0x%X\n", IehConfig->Mode));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
IehLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  IEH_CONFIG        *IehConfig;
  IehConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "IehConfig->Header.GuidHob.Name = %g\n", &IehConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IehConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IehConfig->Header.GuidHob.Header.HobLength));
}

STATIC COMPONENT_BLOCK_ENTRY  mIehBlocks = {
  &gIehConfigGuid,
  sizeof (IEH_CONFIG),
  IEH_CONFIG_REVISION,
  IehLoadConfigDefault
};

/**
  Get Ieh config block table size.

  @retval      Size of config block
**/
UINT16
IehGetConfigBlockTotalSize (
  VOID
  )
{
  return mIehBlocks.Size;
}

/**
  Add Ieh ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
IehAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mIehBlocks, 1);
}
