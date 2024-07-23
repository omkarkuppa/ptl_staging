/** @file
  This file provides services for Gbe policy function

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
#include <GbeConfig.h>

/**
  Print GBE_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
GbePrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS    Status;
  GBE_CONFIG    *GbeConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGbeConfigGuid, (VOID *) &GbeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ GBE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable= %x\n", GbeConfig->Enable));
  DEBUG ((DEBUG_INFO, " LtrEnable= %x\n", GbeConfig->LtrEnable));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
GbeLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  GBE_CONFIG      *GbeConfig;

  GbeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "GbeConfig->Header.GuidHob.Name = %g\n", &GbeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GbeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GbeConfig->Header.GuidHob.Header.HobLength));

  GbeConfig->Enable    = TRUE;
  GbeConfig->LtrEnable = TRUE;
}

STATIC COMPONENT_BLOCK_ENTRY  mGbeBlocks = {
  &gGbeConfigGuid,
  sizeof (GBE_CONFIG),
  GBE_CONFIG_REVISION,
  GbeLoadConfigDefault
};

/**
  Get Gbe config block table size.

  @retval      Size of config block
**/
UINT16
GbeGetConfigBlockTotalSize (
  VOID
  )
{
  return mGbeBlocks.Size;
}

/**
  Add Gbe ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
GbeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mGbeBlocks, 1);
}
