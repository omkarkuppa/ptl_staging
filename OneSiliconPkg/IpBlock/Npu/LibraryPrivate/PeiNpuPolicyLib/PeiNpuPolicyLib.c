/** @file
  This file provides services for NPU PEI policy default initialization

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
#include <Library/PeiNpuPolicyLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadNpuPeiDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  NPU_PEI_CONFIG              *NpuPeiConfig;

  NpuPeiConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "NpuPeiConfig->Header.GuidHob.Name = %g\n", &NpuPeiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "NpuPeiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", NpuPeiConfig->Header.GuidHob.Header.HobLength));

  //
  // Initialize NPU default configuration
  //
  NpuPeiConfig->NpuEnable = 1;
}

static COMPONENT_BLOCK_ENTRY  mNpuIpBlocks = {
  &gNpuPeiConfigGuid, sizeof (NPU_PEI_CONFIG), NPU_PEI_CONFIG_REVISION, LoadNpuPeiDefault
};

/**
  Get NPU PEI config block table total size.

  @retval     Size of NPU PEI config block table
**/
UINT16
EFIAPI
NpuGetConfigBlockTotalSize (
  VOID
  )
{
  return mNpuIpBlocks.Size;
}

/**
  NpuAddConfigBlocks add all NPU PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
NpuAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mNpuIpBlocks, 1);
  return Status;
}


/**
  This function prints the NPU PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
NpuPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                   Status;
  NPU_PEI_CONFIG               *NpuPeiConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gNpuPeiConfigGuid, (VOID *) &NpuPeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ NPU_PEI_CONFIG BEGIN-----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", NpuPeiConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " NpuEnable : 0x%x\n", NpuPeiConfig->NpuEnable));
  DEBUG ((DEBUG_INFO, "\n------------------------ NPU_PEI_CONFIG END -----------------\n"));

  DEBUG_CODE_END ();
  return;
}
