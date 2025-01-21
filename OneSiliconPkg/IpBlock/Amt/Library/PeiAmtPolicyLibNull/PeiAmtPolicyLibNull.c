/** @file
  This file is PeiAmtPolicy library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
  Get AMT config block table total size.

  @retval        Size of AMT config block table
**/
UINT16
EFIAPI
AmtGetConfigBlockTotalSize (
  VOID
  )
{
  return 0;
}

/**
  AmtAddConfigBlocks add all AMT config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add AMT config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AmtAddConfigBlocks (
  IN VOID      *ConfigBlockTableAddress
  )
{
  return EFI_SUCCESS;
}

/**
  Print PEI AMT Config Block

  @param[in] SiPolicyPpi The RC Policy PPI instance
**/
VOID
EFIAPI
AmtPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  return;
}