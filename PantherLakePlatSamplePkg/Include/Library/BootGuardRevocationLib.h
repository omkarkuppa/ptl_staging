/** @file
  Header file for Boot Guard revocation notification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#ifndef _BOOT_GUARD_REVOCATION_LIB_H_
#define _BOOT_GUARD_REVOCATION_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SimpleTextIn.h>

#define MAX_STRING_LEN  200
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/**
  Draws a dialog box to the console output device.

  If there are no strings in the variable argument list, then ASSERT().
  If all the strings in the variable argument list are empty, then ASSERT().

  @param[in]   Attribute  - Specifies the foreground and background color of the popup.
  @param[in]  ...         - The variable argument list that contains pointers to Null-
                            terminated Unicode strings to display in the dialog box.
                            The variable argument list is terminated by a NULL.

**/
VOID
EFIAPI
DrawPopUp (
  IN  UINTN          Attribute,
  ...
  );

/**
  Provide a hook for OEM to deal with Boot Guard revocation flow.
**/
VOID
EFIAPI
BootGuardOemRevocationHook (
  VOID
  );

#endif
