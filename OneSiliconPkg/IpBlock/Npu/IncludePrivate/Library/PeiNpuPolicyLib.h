/** @file
  Header file for PeiNpuPolicy Private library.

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
#ifndef _PEI_NPU_POLICY_LIB_H_
#define _PEI_NPU_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>


/**
  This function prints the NPU PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
NpuPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Get NPU PEI config block table total size.

  @retval     Size of NPU PEI config block table
**/
UINT16
EFIAPI
NpuGetConfigBlockTotalSize (
  VOID
  );


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
  );

#endif // _PEI_NPU_POLICY_LIB_H_
