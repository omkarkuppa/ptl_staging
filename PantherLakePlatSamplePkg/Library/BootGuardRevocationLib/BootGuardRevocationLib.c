/** @file
  This file is SampleCode for Boot Guard revocation notification.

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

#include <Library/BootGuardRevocationLib.h>
#include <Library/TimerLib.h>

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
  )
{
  VA_LIST                          Args;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
  EFI_SIMPLE_TEXT_OUTPUT_MODE      SavedConsoleMode;
  UINTN                            Columns;
  UINTN                            Rows;
  UINTN                            Column;
  UINTN                            Row;
  UINTN                            NumberOfLines;
  UINTN                            MaxLength;
  CHAR16                           *String;
  UINTN                            Length;
  CHAR16                           *Line;

  //
  // Determine the length of the longest line in the popup and the the total
  // number of lines in the popup
  //
  VA_START (Args, Attribute);
  MaxLength = 0;
  NumberOfLines = 0;
  while ((String = VA_ARG (Args, CHAR16 *)) != NULL) {
    MaxLength = MAX (MaxLength, StrLen (String));
    NumberOfLines++;
  }
  VA_END (Args);

  //
  // If the total number of lines in the popup is zero, then ASSERT()
  //
  ASSERT (NumberOfLines != 0);

  //
  // If the maximum length of all the strings is zero, then ASSERT()
  //
  ASSERT (MaxLength != 0);

  //
  // Cache a pointer to the Simple Text Output Protocol in the EFI System Table
  //
  ConOut = gST->ConOut;

  //
  // Save the current console cursor position and attributes
  //
  CopyMem (&SavedConsoleMode, ConOut->Mode, sizeof (SavedConsoleMode));

  //
  // Retrieve the number of columns and rows in the current console mode
  //
  ConOut->QueryMode (ConOut, SavedConsoleMode.Mode, &Columns, &Rows);

  //
  // Disable cursor and set the foreground and background colors specified by Attribute
  //
  ConOut->EnableCursor (ConOut, FALSE);
  ConOut->SetAttribute (ConOut, Attribute);

  //
  // Limit NumberOfLines to height of the screen minus 3 rows for the box itself
  //
  NumberOfLines = MIN (NumberOfLines, Rows - 3);

  //
  // Limit MaxLength to width of the screen minus 2 columns for the box itself
  //
  MaxLength = MIN (MaxLength, Columns - 2);

  //
  // Compute the starting row and starting column for the popup
  //
  Row    = (Rows - (NumberOfLines + 3)) / 2;
  Column = (Columns - (MaxLength + 2)) / 2;

  //
  // Allocate a buffer for a single line of the popup with borders and a Null-terminator
  //
  Line = AllocateZeroPool ((MaxLength + 3) * sizeof (CHAR16));
  ASSERT (Line != NULL);
  if (Line == NULL) return;

  //
  // Draw top of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_DOWN_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_DOWN_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Draw middle of the popup with strings
  //
  VA_START (Args, Attribute);
  while ((String = VA_ARG (Args, CHAR16 *)) != NULL && NumberOfLines > 0) {
    Length = StrLen (String);
    SetMem16 (Line, (MaxLength + 2) * 2, L' ');
    if (Length <= MaxLength) {
      //
      // Length <= MaxLength
      //
      CopyMem (Line + 1 + (MaxLength - Length) / 2, String , Length * sizeof (CHAR16));
    } else {
      //
      // Length > MaxLength
      //
      CopyMem (Line + 1, String + (Length - MaxLength) / 2 , MaxLength * sizeof (CHAR16));
    }
    Line[0]             = BOXDRAW_VERTICAL;
    Line[MaxLength + 1] = BOXDRAW_VERTICAL;
    Line[MaxLength + 2] = L'\0';
    ConOut->SetCursorPosition (ConOut, Column, Row++);
    ConOut->OutputString (ConOut, Line);
    NumberOfLines--;
  }
  VA_END (Args);

  //
  // Draw bottom of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_UP_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_UP_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Free the allocated line buffer
  //
  FreePool (Line);

  //
  // Restore the cursor visibility, position, and attributes
  //
  ConOut->EnableCursor      (ConOut, SavedConsoleMode.CursorVisible);
  ConOut->SetCursorPosition (ConOut, SavedConsoleMode.CursorColumn, SavedConsoleMode.CursorRow);
  ConOut->SetAttribute      (ConOut, SavedConsoleMode.Attribute);
}

/**
  Provide a hook for OEM to deal with Boot Guard revocation flow.
**/
VOID
EFIAPI
BootGuardOemRevocationHook (
  VOID
  )
{
  EFI_STATUS        Status;
  EFI_INPUT_KEY     Key;
  CHAR16            *StringBuffer1;
  CHAR16            *StringBuffer2;
  UINT8             Timer;

  if (gST->ConOut != NULL && gST->ConIn != NULL) {
    //
    // Popup a menu to notice user
    //
    gST->ConOut->ClearScreen (gST->ConOut);

    StringBuffer1 = AllocateZeroPool (MAX_STRING_LEN * sizeof (CHAR16));
    ASSERT (StringBuffer1 != NULL);
    if (StringBuffer1 == NULL) return;
    StringBuffer2 = AllocateZeroPool (MAX_STRING_LEN * sizeof (CHAR16));
    ASSERT (StringBuffer2 != NULL);
    if (StringBuffer2 == NULL) {
      FreePool (StringBuffer1);
      return;
    }
    StrCpyS (StringBuffer1, (MAX_STRING_LEN * sizeof (CHAR16)) / sizeof (CHAR16), L"!!!WARNING: Boot Guard Revocation Failed!!!");
    StrCpyS (StringBuffer2, (MAX_STRING_LEN * sizeof (CHAR16)) / sizeof (CHAR16), L"Press <ENTER> to Shut down the computer.");

    DrawPopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE, StringBuffer1, StringBuffer2, NULL);

    //
    // Wait for a keystroke
    //
    Key.UnicodeChar = 0;
    Timer           = 1;

    do {
      Status = gBS->CheckEvent (gST->ConIn->WaitForKey);
      if (!EFI_ERROR (Status)) {
        Status = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Status = %r\n", Status));
        }
      }
      MicroSecondDelay (1000000);
      Timer --;
      if (Timer == 0) {
        gST->ConOut->ClearScreen (gST->ConOut);
        return;
      }
    } while (Key.UnicodeChar != CHAR_CARRIAGE_RETURN);

    FreePool (StringBuffer1);
    FreePool (StringBuffer2);

    gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
  }

  return;
}
