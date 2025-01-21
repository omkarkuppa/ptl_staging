/** @file
  Platform FV address library

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

@par Specification
**/

#ifndef __PLATFORM_FV_ADDRESS_LIB_H__
#define __PLATFORM_FV_ADDRESS_LIB_H__

/**
  Get Firmware Binary FV base address

  @return FvBase associated with FvBinary FV.
**/
UINT32
GetFvBinaryBase (
  VOID
  );

/**
  Get Microcode FV base address

  @return FvBase associated with Microcode FV.
**/
EFI_STATUS
GetFvMicrocodeBase (
  VOID
  );

/**
  Get Recovery2 base address

  @return FvBase associated with Recovery2 FV.
**/
UINT32
GetFvRecovery2Base (
  VOID
  );

/**
  Get Recovery3 base address

  @return FvBase associated with Recovery3 FV.
**/
UINT32
GetFvRecovery3Base (
  VOID
  );


/**
  Get Fsp-S base address

  @return FvBase associated with Fsp-S FV.
**/
UINT32
GetFvFspsBase (
  VOID
  );

#endif
