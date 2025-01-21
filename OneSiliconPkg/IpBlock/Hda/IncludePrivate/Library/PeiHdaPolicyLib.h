/** @file
  PEI Hda policy library.

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
#ifndef _PEI_HDA_POLICY_LIB_H_
#define _PEI_HDA_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print PCH_HDAUDIO_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
HdaPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HdaPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Hda config block table size.

  @retval      Size of config block
**/
UINT16
HdaPreMemGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Hda ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
HdaPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Print HDAUDIO_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
HdaPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HdaLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Hda config block table size.

  @retval      Size of config block
**/
UINT16
HdaGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Hda ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
HdaAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _PEI_HDA_POLICY_LIB_H_
