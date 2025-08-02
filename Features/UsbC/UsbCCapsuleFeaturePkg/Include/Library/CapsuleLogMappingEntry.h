/** @file
  CAPSULE Log Event and Log String Definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _CAPSULE_LOG_MAPPING_ENTRY_H_
#define _CAPSULE_LOG_MAPPING_ENTRY_H_

///
/// Define the structure of string mapping
///   EvtCode = (EventId << EVT_CODE_EVT_ID_OFFSET) | (ArgumentNumber << EVT_CODE_ARG_NUM_OFFSET) | EVT_CODE_ARG1_STATUS | EVT_CODE_ARG0_STATUS
///   LogStr  - return the corresponding message by EvtCode
///
typedef struct {
  UINT32  EvtCode;
  CHAR8*  LogStr;
} CAPSULE_LOG_MAPPING_ENTRY;

#endif
