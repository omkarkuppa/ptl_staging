/**@file
  Definition of platform sanitize variable

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

#ifndef __PLATFORM_SANITIZE_VARIABLE_H__
#define __PLATFORM_SANITIZE_VARIABLE_H__

#include <StorageSanitizeDefinitions.h>

//
// Global ID for Platform Sanitize variables
//
#define STORAGE_ERASE_VARIABLE_GUID \
  { 0x00a329fe, 0x40ae, 0x46ed, { 0xac, 0x57, 0xa6, 0xfb, 0x87, 0x58, 0x4f, 0x58 } }

#define PLATFORM_ERASE_VARIABLE_GUID \
  { 0x858c26d3, 0x3ee9, 0x4643, { 0xa1, 0x2f, 0x5d, 0xa4, 0xcc, 0x5f, 0x89, 0x65 } }

#define RPE_SETUP_VARIABLE_GUID \
  { 0xd4b3b5a7, 0x1201, 0x490b, { 0xb3, 0x91, 0x2b, 0xfd, 0x93, 0x64, 0x8f, 0x6d } }

#define LPE_SETUP_VARIABLE_GUID \
  { 0xf9f3e497, 0x3763, 0x4eec, { 0x93, 0xf0, 0x16, 0x2f, 0x88, 0x8e, 0x24, 0x8e } }

#define PS_NVM_BOOT_PARAMETERS_GUID \
  { 0x20bbbb2, 0x69fe, 0x4e84, { 0x93, 0x4e, 0x35, 0x77, 0x75, 0xa1, 0x28, 0x6c } }

//
// This variable is set when user de-select device from SETUP
// which used to keep a device list which skip storage erase action.
//-------------
// STORAGE_ERASE_DEVICE_VARIABLE
//
//  Device 1:
//  NameLength
//  SnLength
//  Name
//  Sn
// ------------
//  Device 2:
//  NameLength
//  SnLength
//  Name
//  Sn
// ------------
typedef struct {
  UINT32  NameLength;
  UINT32  SnLength;
  UINT16  Name[MN_MAX_LEN];
  UINT8   Sn[SN_MAX_LEN];
} STORAGE_ERASE_DEVICE_VARIABLE;

//
// Platform erase variable revision
//
#define PLATFORM_ERASE_VARIABLE_REVISION   1

typedef struct {
  //
  // Storage sanitize configuration
  //
  UINT8    SanitizeOrder[SANITIZE_COUNT];
  UINT8    RpeOptionExposure;
  UINT8    RpeOptionGrayOut;
  UINT8    CsmeUnconfigureExposure;
  UINT8    PlatformEraseSimulation;
} PLATFORM_ERASE_VARIABLE;

//
// Remote platform erase variable
//
#define RPE_SETUP_REVISION   2

typedef struct {
  UINT8    Revision;
  UINT8    RpeEnable;
  UINT8    PreviousRpeState;
  BOOLEAN  RpeStateChanged;
} RPE_SETUP;

//
// Local platform erase variable
//
#define LPE_SETUP_REVISION   1

typedef struct {
  UINT8  Revision;
  UINT8  LpeEnable;
  UINT8  LpeEraseAll;
  UINT8  LpeEraseSSD;
  UINT8  LpeStorageEraseVerify;
  UINT8  LpeClearTPM;
  UINT8  LpeOemCustomAction;
  UINT8  LpeClearNvm;
  UINT8  LpeBiosReloadGoldenConfig;
  UINT8  LpeCsmeUnconfigure;
} LPE_SETUP;

#define DEVICE_VARIABLE_NAME           L"StorageEraseDeviceSetup"
#define PLATFORM_ERASE_VARIABLE_NAME   L"PlatformEraseSetup"
#define LPE_SETUP_VARIABLE_NAME        L"LpeSetup"
#define RPE_SETUP_VARIABLE_NAME        L"RpeSetup"
#define PS_NVM_VARIABLE_BOOT_PARAM     L"PsNvmBootParameters"

extern EFI_GUID gPsStorageEraseVariableGuid;
extern EFI_GUID gPsPlatformEraseVariableGuid;
extern EFI_GUID gPsLpeSetupVariableGuid;
extern EFI_GUID gPsRpeSetupVariableGuid;
extern EFI_GUID gPsNvmBootParametersGuid;

#endif
