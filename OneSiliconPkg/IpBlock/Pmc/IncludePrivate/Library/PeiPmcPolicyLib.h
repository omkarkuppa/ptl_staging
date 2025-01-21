/** @file
  PEI Pmc policy library.

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
#ifndef _PEI_PMC_POLICY_LIB_H_
#define _PEI_PMC_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/PeiPmcPolicyCommonLib.h>


/**
  Get Pmc config block table size.

  @retval      Size of config block
**/
UINT16
PmcGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Pmc ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PmcAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Get Bclk config block table size.
  @retval      Size of config block
**/
UINT16
BclkConfigGetConfigBlockTotalSize (
  VOID
  );

/**
  Add BclkConfig PreMem ConfigBlock.
  @param[in] ConfigBlockTableAddress The pointer to config block table
  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
PmcBclkPreMemAddConfigBlock (
  IN VOID *ConfigBlockTableAddress
  );

/**
  Print BCLK_CONFIG on debug port.
  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
PmcBclkPrintConfig (
    IN  CONST SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );

#endif // _PEI_PMC_POLICY_LIB_H_
