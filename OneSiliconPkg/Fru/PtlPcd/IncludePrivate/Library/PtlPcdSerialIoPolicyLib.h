/** @file
  PtlPcdSerialIoPolicyLib header file

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

**/

#ifndef _PTL_PCD_SERIAL_IO_POLICY_LIB_H_
#define _PTL_PCD_SERIAL_IO_POLICY_LIB_H_

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PtlPcdSerialIoLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get SerialIo config block table size.

  @retval      Size of config block
**/
UINT16
PtlPcdSerialIoGetConfigBlockTotalSize (
  VOID
  );

/**
  Add SerialIo ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PtlPcdSerialIoAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif
