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
#ifndef _CPU_INIT_POLICY_LIB_H_
#define _CPU_INIT_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print whole CPU related config blocks of SI_PREMEM_POLICY_PPI and serial out.

  @param[in] SiPreMemPolicyPpi             The Si PreMem Policy PPI instance
**/
VOID
EFIAPI
CpuInitPreMemPrintPolicy (
  IN  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );

/**
  Get CPU PREMEM config block table total size.

  @retval Size of CPU PREMEM config block table
**/
UINT16
EFIAPI
CpuInitPreMemGetConfigBlockTotalSize (
  VOID
  );

/**
  CpuAddPreMemConfigBlocks add all CPU PREMEM config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU PREMEM config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CpuInitPreMemAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  Print whole CPU config blocks of SiPolicyPpi and serial out.

  @param[in] SiPolicyPpi             The SI Policy PPI instance
**/
VOID
EFIAPI
CpuInitPrintPolicy (
  IN  SI_POLICY_PPI                 *SiPolicyPpi
  );

/**
  Get CPU config block table total size.

  @retval Size of CPU config block table
**/
UINT16
EFIAPI
CpuInitGetConfigBlockTotalSize (
  VOID
  );

/**
  CpuAddConfigBlocks add all Cpu config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CpuInitAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

#endif // _CPU_INIT_POLICY_LIB_H_
