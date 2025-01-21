/** @file
  Prototype of the PeiGraphicsPolicy library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _IGPU_PEI_POLICY_LIB_H_
#define _IGPU_PEI_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  This function prints the PEI phase PreMem IGPU policy.

  @param[in] SiPolicyPreMemPpi              The RC PreMem Policy PPI instance
**/
VOID
EFIAPI
IGpuPrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  );

/**
  This function prints the PEI phase IGPU policy.

  @param[in] SiPolicyPpi              The RC Policy PPI instance
**/
VOID
EFIAPI
IGpuPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Get IGPU PEI config block table total size.

  @retval     Size of IGPU PEI config block table
**/
UINT16
EFIAPI
IGpuGetConfigBlockTotalSize (
  VOID
  );

/**
  Get IGPU PEI PreMem config block table total size.

  @retval      Size of IGPU PEI PreMem config block table
**/
UINT16
EFIAPI
IGpuGetConfigBlockTotalSizePreMem (
  VOID
  );

/**
  This function is used to add IGPU PEI PreMem Config Block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IGpuAddConfigBlocksPreMem (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  This function is used to add IGPU PEI Config Block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IGpuAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _IGPU_PEI_POLICY_LIB_H_
