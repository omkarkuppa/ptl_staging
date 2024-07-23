/** @file
  Header file for the PEI CNVi Policy Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _PEI_CNVI_POLICY_LIB_H_
#define _PEI_CNVI_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print CNVI_CONFIG settings.

  @param[in] SiPolicyPpi    Instance of SI_POLICY_PPI
**/
VOID
CnviPrintConfig (
  IN  SI_POLICY_PPI         *SiPolicyPpi
  );

/**
  Get CNVi config block table size.

  @retval      Size of config block
**/
UINT16
CnviGetConfigBlockTotalSize (
  VOID
  );

/**
  Add CNVi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
CnviAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Print CNVI_PREMEM_CONFIG settings.

  @param[in] SiPreMemPolicyPpi  Instance of SI_PREMEM_POLICY_PPI
**/
VOID
CnviPreMemPrintConfig (
  IN CONST SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );

/**
  Get CNVi PreMem config block table size.

  @retval Size of config block
**/
UINT16
CnviPreMemGetConfigBlockTotalSize (
  VOID
  );

/**
  Add CNVi PreMem ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
CnviPreMemAddConfigBlock (
  IN VOID *ConfigBlockTableAddress
  );

#endif //_PEI_CNVI_POLICY_LIB_H_
