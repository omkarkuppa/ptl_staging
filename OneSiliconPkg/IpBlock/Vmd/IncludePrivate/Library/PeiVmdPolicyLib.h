/** @file
  Prototype of the PeiVmdPolicy Init library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _PEI_VMD_POLICY_LIB_H_
#define _PEI_VMD_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  This function prints the PEI phase Vmd policy.

  @param[in] SiPolicyPpi              The RC Policy PPI instance
**/
VOID
EFIAPI
VmdPrintPeiPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Get Vmd PEI config block table total size.

  @retval     Size of Vmd PEI config block table
**/
UINT16
EFIAPI
VmdGetPeiConfigBlockTotalSize (
  VOID
  );

/**
  This function is used to add Vmd PEI Config Block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
VmdAddPeiConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _PEI_VMD_POLICY_LIBRARY_H_
