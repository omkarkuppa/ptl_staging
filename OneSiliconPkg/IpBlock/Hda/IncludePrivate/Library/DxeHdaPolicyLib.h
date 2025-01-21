/** @file
  DXE Hda policy library.

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
#ifndef _DXE_HDA_POLICY_LIB_H_
#define _DXE_HDA_POLICY_LIB_H_

#include <Protocol/PchPolicy.h>

/**
  Print HDAUDIO_DXE_CONFIG and serial out.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
HdaDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HdaDxeLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Hda config block table size.

  @retval      Size of config block
**/
UINT16
HdaDxeGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Hda ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
HdaDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _DXE_HDA_POLICY_LIB_H_
