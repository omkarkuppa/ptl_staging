/** @file
  Sanitize UI related library

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

#ifndef _SANITIZE_UI_LIB_H_
#define _SANITIZE_UI_LIB_H_

/**
  Initializes the library. Required to be used before calling any other function
  from this library.
**/
VOID
InitUiLib (
  VOID
  );

/**
  Redraws an empty screen with title only
**/
VOID
ClearScreen (
  VOID
  );

/**
  Sets screen title which is displayed at top of the screen
**/
VOID
SetScreenTitle (
  VOID
  );

/**
  clear the popup dialog
**/
VOID
ClearPopupScreen (
  VOID
  );

/**
  Outputs provided c string at next line.

  @param[in] String                    C string to be printed
  @param[in] Attribute                 The attribute to set. Bits 0..3 are the
                                       foreground color, and bits 4..6 are the
                                       background color. All other bits are
                                       undefined and must be zero. The valid
                                       Attributes are defined in this file.
**/
VOID
OutputStringAtNext (
  IN CHAR16                            *String,
  IN UINTN                             Attribute
  );

/**
  Output provided c string at current line.

  @param[in] String                    C string to be printed
  @param[in] Attribute                 The attribute to set. Bits 0..3 are the
                                       foreground color, and bits 4..6 are the
                                       background color. All other bits are
                                       undefined and must be zero. The valid
                                       Attributes are defined in this file.
**/
VOID
UpdateString (
  IN CHAR16                            *String,
  IN UINTN                             Attribute
  );

/**
  Append provided c string at current line.

  @param[in] String    C string to be printed
  @param[in] Attribute The attribute to set. Bits 0..3 are the
                       foreground color, and bits 4..6 are the
                       background color. All other bits are
                       undefined and must be zero. The valid
                       Attributes are defined in this file.
**/
VOID
AppendString (
  IN CHAR16 *String,
  IN UINTN  Attribute
  );

/**
  Prints summary message after Platform Sanitize - SSD Erase operation has ended

  @param[in] Status                 SSD Erase operation result

**/
VOID
PsStoragePrintSummary (
  IN EFI_STATUS                        Status
  );
#endif /*_SANITIZE_UI_LIB_H_*/
