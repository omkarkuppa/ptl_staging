/** @file
  Sanitize UI related library.

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

#ifndef _SANITIZE_UI_H_
#define _SANITIZE_UI_H_

/**
  Displays a progress bar dialog to indicate ongoing operation.

  @param[in] Message                   A unicode string to display above the bar
  @param[in] Delay                     The amount of time for which the text will be displayed in seconds.
**/
VOID
ProgressBarDialog (
  IN CHAR16                            *Message,
  IN UINT32                            Delay
  );

/**
  Outputs provided c string on the given position.

  @param[in] String                    C string to be printed
  @param[in] Column                    Column number to put first char in
  @param[in] Row                       Row number to put first char in
  @param[in] Attribute                 The attribute to set. Bits 0..3 are the
                                       foreground color, and bits 4..6 are the
                                       background color. All other bits are
                                       undefined and must be zero. The valid
                                       Attributes are defined in this file.
**/
VOID
OutputString (
  IN CHAR16                            *String,
  IN UINTN                             Column,
  IN UINTN                             Row,
  IN UINTN                             Attribute
  );
#endif /*_SANITIZE_UI_H_*/
