/** @file
  Guid used for CrashLog HOB

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __CRASHLOG_HOB_H__
#define __CRASHLOG_HOB_H__


//
// Global ID for Crashlog Hob
//
#define CRASHLOG_RECORD_HOB_GUID \
  { \
    0x09f119d5, 0xa5ad, 0x4b30, {0x90, 0x9e, 0xfa, 0x94, 0xdc, 0xa3, 0xc9, 0xb5 } \
  }

//
// Crashlog agent type
//
typedef enum {
  CrashlogAgentCpu      = 0,
  CrashlogAgentSoc,
  CrashlogAgentPch,
  CrashlogAgentTypeMax
} CRASH_LOG_AGENT_TYPE;

//
// Structure to be used by crashlog HOB
//
typedef struct {
  UINT64    Address;
  UINT32    Size;
  UINT32    CrashNode;
} CRASHLOG_HOB_RECORD_ENTRY;

typedef struct {
  UINT32                  RecordCount;
  CRASH_LOG_AGENT_TYPE    CrashlogAgent;
  // CRASHLOG_HOB_RECORD_ENTRY    Records[];
} CRASHLOG_HOB_DATA;


extern EFI_GUID  gCrashLogRecordHobGuid;

#endif
