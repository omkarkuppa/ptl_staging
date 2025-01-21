/** @file
  USB4 Log Event and Log String Definitions

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

#ifndef _USB4_LOG_MAPPING_ENTRY_H_
#define _USB4_LOG_MAPPING_ENTRY_H_

#include <Uefi.h>

//
// Define the structure of string mapping
//   EvtCode - (EvtId << 2) | (argument number:2 bits)
//   LogStr  - return the corresponding message by EvtCode
//
typedef struct {
  UINT32  EvtCode;
  CHAR8*  LogStr;
} USB4_LOG_MAPPING_ENTRY;

#endif
