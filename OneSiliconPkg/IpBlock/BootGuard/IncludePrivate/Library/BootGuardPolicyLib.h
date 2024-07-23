/** @file
  Prototype of the CpuInitPolicy library.

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

@par Specification
**/
#ifndef _BOOT_GUARD_LIB_H_
#define _BOOT_GUARD_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print BootGuard config blocks of SI_PREMEM_POLICY_PPI and serial out.

  @param[in] SiPreMemPolicyPpi             The Si PreMem Policy PPI instance
**/
VOID
EFIAPI
BootGuardPrintPolicy (
  IN  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );

/**
  Get BootGuard config block table total size.

  @retval Size of BootGuard config block table
**/
UINT16
EFIAPI
BootGuardGetConfigBlockTotalSize (
  VOID
  );

/**
  BootGuardAddConfigBlocks add BootGuard config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add BootGuard config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
BootGuardAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

#endif // _BOOT_GUARD_LIB_H_
