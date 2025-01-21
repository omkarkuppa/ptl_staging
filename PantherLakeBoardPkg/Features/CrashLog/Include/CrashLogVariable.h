/** @file
  The Dxe drvier is used to update crashlog variable with the data from HOB

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

#ifndef __CRASHLOG_VARIABLE_H__
#define __CRASHLOG_VARIABLE_H__


//
// Global ID for Crashlog variable
//
#define CRASHLOG_VARIABLE_GUID \
  { \
    0x5f3fedfe, 0x5844, 0x4be7, { 0xbb, 0x5f, 0x6, 0x87, 0x6a, 0x11, 0xe8, 0xf6 } \
  }
#define CRASHLOG_CONTROL_VARIABLE_GUID \
  { \
    0x8414f285, 0x64c8, 0x40f7, { 0x92, 0xbd, 0x91, 0x37, 0x3b, 0x6b, 0x89, 0x8d } \
  }

#pragma pack(1)
//
// CrashLog variable
//
typedef struct {
  //
  //  Control the entire Crash Log collection flow
  //
  UINT8     EnableCrashLog;
  //
  //  Silicon Crash Log flow
  //
  UINT8     SiliconOnAllReset;
  UINT8     SiliconClear;
  UINT8     SiliconRearm;
  UINT8     SiliconGblRst;
  UINT32    SiliconGblRstMask0;
  UINT32    SiliconGblRstMask1;

  //
  //  Pch Crash Log flow
  //
  UINT8     PchGblRst;
  UINT32    PchGblRstMask0;
  UINT32    PchGblRstMask1;

  //
  //  CPU Crash Log flow
  //
  UINT8     CpuClear;
  UINT8     CpuRearm;
  UINT8     CpuStorageOff;

} CRASHLOG_VARIABLE;

//
// CrashLog control variable used to control show/hidden of option
//
typedef struct {
  //
  //  Silicon Crash Log flow
  //
  UINT8    SiliconOnAllReset;
  UINT8    SiliconClear;
  UINT8    SiliconRearm;
  UINT8    SiliconGblRst;

  //
  //  Pch Crash Log flow
  //
  UINT8    PchGblRst;

  //
  //  CPU Crash Log flow
  //
  UINT8    CpuClear;
  UINT8    CpuRearm;
  UINT8    CpuStorageOff;
} CRASHLOG_CONTROL_VARIABLE;
#pragma pack()

#define CRASHLOG_VARIABLE_NAME             L"CrashLogVariable"
#define CRASHLOG_CONTROL_VARIABLE_NAME     L"CrashLogCtlVariable"

extern EFI_GUID gCrashLogVariableGuid;
extern EFI_GUID gCrashLogCtlVariableGuid;

//
// Declaration for SETUP
//
#define CRASHLOG_SETUP_VARSTORE\
    efivarstore CRASHLOG_VARIABLE, \
        attribute = 0x7, name = CrashLogVariable, guid = CRASHLOG_VARIABLE_GUID;

#define CRASHLOG_CTL_SETUP_VARSTORE\
    efivarstore CRASHLOG_CONTROL_VARIABLE, \
        attribute = 0x7, name = CrashLogCtlVariable, guid = CRASHLOG_CONTROL_VARIABLE_GUID;

#endif
