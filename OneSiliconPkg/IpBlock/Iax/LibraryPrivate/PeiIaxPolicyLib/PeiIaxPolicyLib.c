/** @file
  This file provides services for IAX PEI policy default initialization

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

/**
  Load Config block default

  @param[in] ConfigBlockPointer    Pointer to config block
**/
VOID
LoadIaxPeiDefault (
  IN VOID         *ConfigBlockPointer
  )
{
  IAX_PEI_CONFIG  *IaxPeiConfig;

  IaxPeiConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "IaxPeiConfig->Header.GuidHob.Name = %g\n", &IaxPeiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IaxPeiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IaxPeiConfig->Header.GuidHob.Header.HobLength));

  //
  // Initialize IAX default configuration
  //
  IaxPeiConfig->IaxEnable = 0;
}

static COMPONENT_BLOCK_ENTRY  mIaxIpBlocks = {
  &gIaxPeiConfigGuid, sizeof (IAX_PEI_CONFIG), IAX_PEI_CONFIG_REVISION, LoadIaxPeiDefault
};

/**
  Get IAX PEI config block table total size.

  @retval     Size of IAX PEI config block table
**/
UINT16
EFIAPI
IaxGetConfigBlockTotalSize (
  VOID
  )
{
  return mIaxIpBlocks.Size;
}

/**
  IaxAddConfigBlocks add all IAX PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IaxAddConfigBlocks (
  IN VOID     *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mIaxIpBlocks, 1);

  return Status;
}

/**
  This function prints the IAX PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
IaxPrintPolicyPpi (
  IN  SI_POLICY_PPI    *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS           Status;
  IAX_PEI_CONFIG       *IaxPeiConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIaxPeiConfigGuid, (VOID *) &IaxPeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ IAX_PEI_CONFIG BEGIN-----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision  : %d\n", IaxPeiConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " IaxEnable : 0x%x\n", IaxPeiConfig->IaxEnable));
  DEBUG ((DEBUG_INFO, "------------------------ IAX_PEI_CONFIG END -----------------\n"));

  DEBUG_CODE_END ();
}
