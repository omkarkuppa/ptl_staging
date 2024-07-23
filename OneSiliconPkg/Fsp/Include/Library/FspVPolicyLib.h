/** @file
  This file provides services for FspV policy function

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
#ifndef _FSPV_POLICY_LIB_H_
#define _FSPV_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print FSPV_POSTMEM_CONFIG.

  @param[in] SiPolicyPpi  Pointer to a SI_POLICY_PPI

**/
VOID
FspVPostMemPrintConfig (
  IN  SI_POLICY_PPI       *SiPolicyPpi
  );

/**
  Print FSPV_PREMEM_CONFIG.

  @param[in] SiPreMemPolicyPpi  Pointer to a SI_PREMEM_POLICY_PPI

**/
VOID
FspVPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  Load Post-Mem Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
FspVPostMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Load Pre-Mem Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
FspVPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get FspV Post-Mem config block table size.

  @retval      Size of config block
**/
UINT16
FspVPostMemGetConfigBlockTotalSize (
  VOID
  );

/**
  Get FspV Pre-Mem config block table size.

  @retval      Size of config block
**/
UINT16
FspVPreMemGetConfigBlockTotalSize (
  VOID
  );

/**
  Add FspV Post-Mem ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
FspVPostMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Add FspV Pre-Mem ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
FspVPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _FSPV_POLICY_LIB_H_
