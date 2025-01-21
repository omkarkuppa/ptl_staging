/** @file
  PTL PCD PEI Ish policy library.

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
#ifndef _PTL_PCD_PEI_ISH_POLICY_LIB_H_
#define _PTL_PCD_PEI_ISH_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PtlPcdIshPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Ish config block table size.

  @retval      Size of config block
**/
UINT16
PtlPcdIshPreMemGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Ish ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PtlPcdIshPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PtlPcdIshLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Ish config block table size.

  @retval      Size of config block
**/
UINT16
PtlPcdIshGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Ish ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PtlPcdIshAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );
#endif // _PTL_PCD_PEI_ISH_POLICY_LIB_H_
