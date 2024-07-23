/** @file
  Prototype of the MemorySubSystemPolicy library.

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
#ifndef _MEMORY_SUB_SYSTEM_POLICY_LIB_H_
#define _MEMORY_SUB_SYSTEM_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/ConfigBlockLib.h>


/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadMemorySubSystemDefault (
  IN VOID    *ConfigBlockPointer
  );

/**
  Get Memory SubSystem config block table total size.

  @retval      Size of Memory SubSystem PEI Pre-Mem config block table
**/
UINT16
EFIAPI
MemorySubSystemGetConfigBlockTotalSize (
  VOID
  );

/**
  MemorySubSystemAddConfigBlock add Memory SubSystem config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Memory SubSystem config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MemorySubSystemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  This function prints the Memory SubSystem PreMem policy.

  @param[in] SiPolicyPreMemPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
MemorySubSystemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  );
#endif // _MEMORY_SUB_SYSTEM_POLICY_LIB_H_
