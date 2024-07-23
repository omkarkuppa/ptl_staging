/** @file
  Prototype of the PeiITbtPolicyLib library.

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
#ifndef _PEI_TBT_POLICY_LIB_H_
#define _PEI_TBT_POLICY_LIB_H_
#include <Ppi/SiPolicy.h>
#include <Library/ConfigBlockLib.h>
#include <TcssDataHob.h>

/**
  This function prints the PEI phase PreMem policy.

  @param[in] SiPolicyPpi                Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
ITbtPrintPolicy (
  IN  SI_POLICY_PPI                     *SiPolicyPpi
  );

/**
  Get TBT config block table total size.

  @retval                               Size of TBT config block table
**/
UINT16
EFIAPI
ITbtGetConfigBlockTotalSize (
  VOID
  );

/**
  ITbtAddConfigBlocks add all TBT config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add TBT config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
ITbtAddConfigBlocks (
  IN VOID                               *ConfigBlockTableAddress
  );
#endif // _DXE_TBT_POLICY_LIB_H_
