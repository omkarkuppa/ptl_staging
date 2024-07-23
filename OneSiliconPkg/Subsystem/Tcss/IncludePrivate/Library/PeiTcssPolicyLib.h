/** @file
  PEI TCSS policy library header file.

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
#ifndef _PEI_TCSS_POLICY_LIB_H_
#define _PEI_TCSS_POLICY_LIB_H_

/**
  Print TCSS_PEI_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
TcssPrintConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  Print TCSS_PEI_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
TcssPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TcssLoadConfigPreMemDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TcssLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get TCSS PreMem config block table size.

  @retval      Size of config block
**/
UINT16
TcssGetConfigBlockTotalSizePreMem (
  VOID
  );

/**
  Get TCSS config block table size.

  @retval      Size of config block
**/
UINT16
TcssGetConfigBlockTotalSize (
  VOID
  );

/**
  Add TCSS PreMem ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
TcssAddConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Add TCSS ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
TcssAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif
