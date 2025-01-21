/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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

#ifndef _ATTEMPT_USB_FIRST_H_
#define _ATTEMPT_USB_FIRST_H_

#pragma pack(1)
typedef struct _ATTEMPT_USB_FIRST_HOTKEY_INFO {
  UINT8 RevisonId;         // Structure Revision ID
  UINT8 HotkeyTriggered;   // Hot key status
} ATTEMPT_USB_FIRST_HOTKEY_INFO;
#pragma pack()

#pragma pack(1)
typedef struct _ATTEMPT_USB_FIRST_VARIABLE {
  UINT8 UsbBootPrior;
} ATTEMPT_USB_FIRST_VARIABLE;
#pragma pack()

//
// Volatile variable definition for Attempt USB first features
//
#pragma pack(1)
typedef struct _ATTEMPT_USB_FIRST_RUNTIME_VARIABLE {
  UINT8 RevisonId;        // Structure Revision ID
  UINT8 UsbFirstEnable;   // Attempt USB First is enabled or not
} ATTEMPT_USB_FIRST_RUNTIME_VARIABLE;
#pragma pack()

//
// Volatile variable definition for third party Default Enabling via UEFI Variable.
//
#pragma pack(1)
typedef struct _ENABLE_CUSTOM_DEFAULTS{
  UINT32 EnableCustomDefaults;
} ENABLE_CUSTOM_DEFAULTS;
#pragma pack()

#define COENG_DEFAULTS_UNKNOWN   0
#define COENG_DEFAULTS_SUPPORTED 1
#define COENG_DEFAULTS_VAR_EXITS 2
#define COENG_DEFAULTS_VAR_SET   4
#define COENG_DEFAULTS_AVAILABLE (COENG_DEFAULTS_SUPPORTED | COENG_DEFAULTS_VAR_EXITS |COENG_DEFAULTS_VAR_SET)

extern EFI_GUID gAttemptUsbFirstHotkeyInfoHobGuid;
extern EFI_GUID gAttemptUsbFirstRuntimeVarInfoGuid;
#endif
