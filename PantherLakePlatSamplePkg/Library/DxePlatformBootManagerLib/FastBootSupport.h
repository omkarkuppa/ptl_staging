/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#ifndef _FAST_BOOT_SUPPORT_H_
#define _FAST_BOOT_SUPPORT_H_

#include <Protocol/FastBootException.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/TcgPhysicalPresenceLib.h>
#include <Library/Tcg2PhysicalPresenceLib.h>

#define NORMAL_BOOT           0x00
#define QUIET_BOOT            0x01
#define SILENT_BOOT           0x02

#define KBC_INPBUF_VIA60_KBECHO         0xEE
#define KEYBOARD_8042_DATA_REGISTER     0x60
#define KEYBOARD_8042_STATUS_REGISTER   0x64

#define USB_CONSOLE                        0x00
#define RECONNECT_LAST_GOOD_INPUT_CONSOLE  0x01
#define WINDOWS_CONSOLE                    0x02

#define PS2_KEYBOARD_TIMEOUT                65536   // 0.07s
#define PS2_KEYBOARD_WAITFORVALUE_TIMEOUT   1000000 // 1s
#define PS2_KEYBOARD_KBEN                   0xF4
#define PS2_KEYBOARD_CMDECHO_ACK            0xFA

VOID
FastBootUpdateConsoleVariable (
  VOID
  );

VOID
FastBootUpdateTimeoutVariable (
  VOID
  );

BOOLEAN
FastBootEnabled (
  VOID
  );

UINT8
GetDisplayBootMode (
  VOID
  );

BOOLEAN
IsPs2KeyboardConnected (
  VOID
  );
VOID
FastBootGenericVariablesInit (
  VOID
  );

BOOLEAN
IsExceptionType2Detected (
  VOID
  );

EFI_STATUS
RaiseFastBootException (
  FAST_BOOT_EXCEPTION_TYPE      ExceptionType,
  FAST_BOOT_EXCEPTION_CATEGORY  ExceptionCategory
  );

EFI_STATUS
FastBootEnumConInDevice (
  VOID
  );

VOID
FastBootHookConOut (
  VOID
  );

VOID
FastBootRestoreConOut (
  VOID
  );

VOID
FastBootUpdateBootMode (
  EFI_BOOT_MODE *BootMode
  );

VOID
FastBootBootOptionPatch (
  EFI_BOOT_MODE                 BootMode
  );

#endif //_FAST_BOOT_SUPPORT_H_
