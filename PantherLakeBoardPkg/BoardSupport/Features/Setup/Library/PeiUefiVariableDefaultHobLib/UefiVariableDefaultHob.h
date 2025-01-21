/**@file

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

#ifndef __UEFI_VARIABLE_DEFAULT_H__
#define __UEFI_VARIABLE_DEFAULT_H__

#include <Uefi.h>

typedef struct {
  UINT16 DefaultId;
  UINT16 BoardId;
} DEFAULT_INFO;

typedef struct {
  UINT16 Offset;
  UINT8  Value;
} DATA_DELTA;

typedef struct {
  //
  // HeaderSize includes HeaderSize fields and DefaultInfo arrays
  //
  UINT16 HeaderSize;
  //
  // DefaultInfo arrays those have the same default setting.
  //
  DEFAULT_INFO DefaultInfo[1];
  //
  // Default data is stored as variable storage or the array of DATA_DELTA.
  //
} DEFAULT_DATA;

#endif
