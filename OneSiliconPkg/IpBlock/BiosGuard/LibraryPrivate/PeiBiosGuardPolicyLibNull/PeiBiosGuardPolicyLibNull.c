/** @file
  This file load BIOS Guard and config block.

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
#include <Ppi/SiPolicy.h>

/**
  Print BIOS_GUARD_CONFIG and serial out.

  @param[in] SiPolicyPpi  Instance of SI_POLICY_PPI
**/
VOID
BiosGuardConfigPrint (
  IN  SI_POLICY_PPI       *SiPolicyPpi
  )
{
  return;
}

/**
  Get BIOS Guard config block total size.

  @retval Size of config block
**/
UINT16
BiosGuardGetConfigBlockTotalSize (
  VOID
  )
{
  return 0;
}

/**
  Add BIOS Guard ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
BiosGuardInitAddConfigBlocks (
  IN VOID *ConfigBlockTableAddress
  )
{
  return EFI_SUCCESS;
}
