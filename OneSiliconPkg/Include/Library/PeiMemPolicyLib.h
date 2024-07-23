/** @file
  Header file for the PeiMemPolicyLib library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _PEI_MEM_POLICY_LIB_H_
#define _PEI_MEM_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadMemConfigNoCrcDefault (
  IN VOID    *ConfigBlockPointer
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadMemConfigDefault (
  IN VOID *ConfigBlockPointer
  );

/**
  Load some policy default for reference board.

  @param[in] ConfigBlockTableAddress    The pointer for Mrc config blocks

**/
VOID
MrcLoadSamplePolicyPreMem (
  IN VOID *ConfigBlockTableAddress
  );

/**
  Get Mrc config block table total size.

  @retval      Size of Mrc config block table
**/
UINT16
EFIAPI
MrcGetConfigBlockTotalSizePreMem (
  VOID
 );

/**
  MrcAddConfigBlocksPreMem add all Mrc PEI PreMem config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Mrc config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MrcAddConfigBlocksPreMem(
  IN VOID           *ConfigBlockTableAddress
  );

/**
  This function prints the PEI phase PreMem Mrc policy.

  @param[in] SiPolicyPreMemPpi          The MRC PreMem Policy PPI instance
**/
VOID
EFIAPI
MrcPrintPolicyPpiPreMem(
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  );
#endif // _PEI_MEM_POLICY_LIB_H_

