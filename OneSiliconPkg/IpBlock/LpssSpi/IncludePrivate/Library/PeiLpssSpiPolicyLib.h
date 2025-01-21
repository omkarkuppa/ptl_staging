/** @file
  This file provides services for Lpss Spi policy function

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

#ifndef _PEI_LPSS_SPI_POLICY_LIB_H_
#define _PEI_LPSS_SPI_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print LPSS_SPI_CONFIG.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
LpssSpiPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LpssSpiLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Lpss Spi config block table size.

  @retval      Size of config block
**/
UINT16
LpssSpiGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Lpss Spi ConfigBlock.

  @param[in] ConfigBlockTableAddress       The pointer to config block table
  @retval    EFI_SUCCESS                   The policy default is initialized.
  @retval    EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
LpssSpiAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _PEI_SPI_POLICY_LIB_H_
