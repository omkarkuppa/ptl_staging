/** @file
  Prototype of the MePolicyLibPei library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PEI_ME_POLICY_LIB_H_
#define _PEI_ME_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/ConfigBlockLib.h>

/**
  This function prints the PEI phase PreMem policy.

  @param[in] SiPolicyPreMemPpi              The RC PreMem Policy PPI instance
**/
VOID
EFIAPI
MePrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  );

/**
  This function prints the PEI phase policy.

  @param[in] SiPolicyPpi              The RC Policy PPI instance
**/
VOID
EFIAPI
MePrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Get Me config block table total size.

  @retval     Size of Me config block table
**/
UINT16
EFIAPI
MeGetConfigBlockTotalSize (
  VOID
  );

/**
  Get ME config block table total size.

  @retval      Size of ME config block table
**/
UINT16
EFIAPI
MeGetConfigBlockTotalSizePreMem (
  VOID
  );

/**
  MeAddConfigBlocksPreMem add all ME config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add ME config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeAddConfigBlocksPreMem (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  MeAddConfigBlocks add all ME config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add ME config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _PEI_ME_POLICY_LIB_H_
