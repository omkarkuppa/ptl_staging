/** @file
  Prototype of the CpuPolicy library.

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
#ifndef _TXT_POLICY_LIB_H_
#define _TXT_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>
#include <TxtConfig.h>

/**
  Load Txt Config default Policy.
**/
VOID
LoadTxtPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Txt config block total size.

  @retval      Size of Txt config block
**/
UINT16
EFIAPI
TxtGetConfigBlockTotalSizePreMem (
  VOID
  );

/**
  TxtAddPreMemConfigBlocks function adds Txt pre-mem ConfigBlock to the pre-mem ConfigBlock table.

  @param[in] ConfigBlockTableAddress    The pointer to Txt config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
TxtAddConfigBlockPreMem (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  Print TXT_PREMEM_CONFIG and serial out.

  @param[in] TxtPreMemConfig     Pointer to a TXT_PREMEM_CONFIG
**/
VOID
TxtConfigPrintPreMem (
  IN  TXT_PREMEM_CONFIG        *TxtPreMemConfig
  );

/**
  Print Txt Config blocks of SI_PREMEM_POLICY_PPI and serial out in PreMem.

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance
**/
VOID
TxtPrintPolicyPreMem (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  );  
#endif // _TXT_POLICY_LIB_H_
