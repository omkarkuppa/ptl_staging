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
#include <Ppi/SiPolicy.h>

/**
  Print FUSA_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
EFI_STATUS
FusaPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  return EFI_SUCCESS;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
EFI_STATUS
FusaLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  return EFI_SUCCESS;
}

/**
  Get Fusa config block table size.

  @retval      Size of config block
**/
UINT16
FusaGetConfigBlockTotalSize (
  VOID
  )
{
  return 0;
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
  return EFI_SUCCESS;
}
