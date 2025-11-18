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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include "SanitizeUi.h"

#define PLATFORM_ERASE_HORIZONTAL_RESOLUTION 800
#define PLATFORM_ERASE_VERTICAL_RESOLUTION   600
#define PLATFORM_ERASE_COLUMNS_NUMBER        100
#define PLATFORM_ERASE_ROWS_NUMBER           31

GLOBAL_REMOVE_IF_UNREFERENCED UINTN                                 mConOutCols;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                                 mConOutRows;
GLOBAL_REMOVE_IF_UNREFERENCED CHAR16                                *mScreenTitle = NULL;

/**
  This function will change video resolution and text mode
  according to defined setup mode or defined boot mode

  @retval  EFI_SUCCESS                 Mode is changed successfully.
  @retval  Others                      Mode failed to be changed.

**/
EFI_STATUS
SetPlatformEraseConsoleMode (
  VOID
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *SimpleTextOut;
  UINTN                                 SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info;
  UINT32                                MaxGopMode;
  UINT32                                MaxTextMode;
  UINT32                                ModeNumber;
  UINT32                                NewHorizontalResolution;
  UINT32                                NewVerticalResolution;
  UINT32                                NewColumns;
  UINT32                                NewRows;
  UINTN                                 HandleCount;
  EFI_HANDLE                            *HandleBuffer;
  EFI_STATUS                            Status;
  UINTN                                 Index;
  UINTN                                 CurrentColumn;
  UINTN                                 CurrentRow;

  MaxGopMode  = 0;
  MaxTextMode = 0;

  //
  // Get current video resolution and text mode
  //
  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID**)&GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    GraphicsOutput = NULL;
  }

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiSimpleTextOutProtocolGuid,
                  (VOID**)&SimpleTextOut
                  );
  if (EFI_ERROR (Status)) {
    SimpleTextOut = NULL;
  }

  if ((GraphicsOutput == NULL) || (SimpleTextOut == NULL)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Set the requried resolution for Platform Erase Screen
  //
  NewHorizontalResolution = PLATFORM_ERASE_HORIZONTAL_RESOLUTION;
  NewVerticalResolution   = PLATFORM_ERASE_VERTICAL_RESOLUTION;
  NewColumns              = PLATFORM_ERASE_COLUMNS_NUMBER;
  NewRows                 = PLATFORM_ERASE_ROWS_NUMBER;

  MaxGopMode  = GraphicsOutput->Mode->MaxMode;
  MaxTextMode = SimpleTextOut->Mode->MaxMode;

  //
  // 1. If current video resolution is same with required video resolution,
  //    video resolution need not be changed.
  //    1.1. If current text mode is same with required text mode, text mode need not be changed.
  //    1.2. If current text mode is different from required text mode, text mode need be changed.
  // 2. If current video resolution is different from required video resolution, we need restart whole console drivers.
  //
  for (ModeNumber = 0; ModeNumber < MaxGopMode; ModeNumber++) {
    Status = GraphicsOutput->QueryMode (
                               GraphicsOutput,
                               ModeNumber,
                               &SizeOfInfo,
                               &Info
                               );
    if (!EFI_ERROR (Status)) {
      if ((Info->HorizontalResolution == NewHorizontalResolution) &&
          (Info->VerticalResolution == NewVerticalResolution)) {
        if ((GraphicsOutput->Mode->Info->HorizontalResolution == NewHorizontalResolution) &&
            (GraphicsOutput->Mode->Info->VerticalResolution == NewVerticalResolution)) {
          //
          // Current resolution is same with required resolution, check if text mode need be set
          //
          Status = SimpleTextOut->QueryMode (SimpleTextOut, SimpleTextOut->Mode->Mode, &CurrentColumn, &CurrentRow);
          ASSERT_EFI_ERROR (Status);
          if (CurrentColumn == NewColumns && CurrentRow == NewRows) {
            //
            // If current text mode is same with required text mode. Do nothing
            //
            FreePool (Info);
            return EFI_SUCCESS;
          } else {
            //
            // If current text mode is different from requried text mode.  Set new video mode
            //
            for (Index = 0; Index < MaxTextMode; Index++) {
              Status = SimpleTextOut->QueryMode (SimpleTextOut, Index, &CurrentColumn, &CurrentRow);
              if (!EFI_ERROR(Status)) {
                if ((CurrentColumn == NewColumns) && (CurrentRow == NewRows)) {
                  //
                  // Required text mode is supported, set it.
                  //
                  Status = SimpleTextOut->SetMode (SimpleTextOut, Index);
                  ASSERT_EFI_ERROR (Status);
                  //
                  // Update text mode PCD.
                  //
                  Status = PcdSet32S (PcdConOutColumn, PLATFORM_ERASE_COLUMNS_NUMBER);
                  Status = PcdSet32S (PcdConOutRow, PLATFORM_ERASE_ROWS_NUMBER);
                  FreePool (Info);
                  return EFI_SUCCESS;
                }
              }
            }
            if (Index == MaxTextMode) {
              //
              // If requried text mode is not supported, return error.
              //
              FreePool (Info);
              return EFI_UNSUPPORTED;
            }
          }
        } else {
          //
          // If current video resolution is not same with the new one, set new video resolution.
          // In this case, the driver which produces simple text out need be restarted.
          //
          Status = GraphicsOutput->SetMode (GraphicsOutput, ModeNumber);
          if (!EFI_ERROR (Status)) {
            FreePool (Info);
            break;
          }
        }
      }
      FreePool (Info);
    }
  }

  if (ModeNumber == MaxGopMode) {
    //
    // If the resolution is not supported, return error.
    //
    return EFI_UNSUPPORTED;
  }

  //
  // Set PCD to Inform GraphicsConsole to change video resolution.
  // Set PCD to Inform Consplitter to change text mode.
  //
  Status = PcdSet32S (PcdVideoHorizontalResolution, NewHorizontalResolution);
  Status = PcdSet32S (PcdVideoVerticalResolution, NewVerticalResolution);
  Status = PcdSet32S (PcdConOutColumn, NewColumns);
  Status = PcdSet32S (PcdConOutRow, NewRows);

  //
  // Video mode is changed, so restart graphics console driver and higher level driver.
  // Reconnect graphics console driver and higher level driver.
  // Locate all the handles with GOP protocol and reconnect it.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleTextOutProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < HandleCount; Index++) {
      gBS->DisconnectController (HandleBuffer[Index], NULL, NULL);
    }
    for (Index = 0; Index < HandleCount; Index++) {
      gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
    }
    if (HandleBuffer != NULL) {
      FreePool (HandleBuffer);
    }
  }

  return EFI_SUCCESS;
}


/**
  Initializes the library. Required to be used before calling any other function
  from this library.
**/
VOID
InitUiLib (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "Platform Erase :: %a\n", __FUNCTION__));
  SetPlatformEraseConsoleMode ();
  gST->ConOut->EnableCursor (gST->ConOut, FALSE);
  gST->ConOut->QueryMode (gST->ConOut, gST->ConOut->Mode->Mode, &mConOutCols, &mConOutRows);

  if (mScreenTitle != NULL) {
    FreePool (mScreenTitle);
    mScreenTitle = NULL;
  }
  DEBUG ((DEBUG_INFO, "std out terminal : %d x %d\n", mConOutCols, mConOutRows));
}

/**
  Allocates a buffer to hold one line of char16 type and fills it with a given character.

  @param[in] Filler                    Character to fill the buffer with

  @retval CHAR16*                      Pointer to a newly allocated buffer
**/
CHAR16*
AlocateFilledLine (
  IN CHAR16                            Filler
  )
{
  CHAR16                               *Line;
  UINTN                                Index;

  Line = AllocateZeroPool ((mConOutCols + 1) * sizeof (CHAR16));
  ASSERT (Line != NULL);

  if (Line != NULL) {
    for (Index = 0; Index < mConOutCols ; Index++){
      Line[Index] = Filler;
    }
    Line[mConOutCols] = L'\0';
  }

  return Line;
}

/**
  Creates a copy of the current ConOut mode.

  @param[in] Mode                      Pointer to a preallocated buffer
 */
VOID
PreserveConsoleMode (
  IN EFI_SIMPLE_TEXT_OUTPUT_MODE      *Mode
  )
{
  CopyMem (Mode, gST->ConOut->Mode, sizeof (EFI_SIMPLE_TEXT_OUTPUT_MODE));
}

/**
  Restores ConOut console mode using the given variable.

  @param[in] Mode                      Previously preserved console mode to restore
**/
VOID
RestoreConsoleMode (
  IN EFI_SIMPLE_TEXT_OUTPUT_MODE      *Mode
  )
{
  if (Mode != NULL) {
    gST->ConOut->EnableCursor (gST->ConOut, Mode->CursorVisible);
    gST->ConOut->SetCursorPosition (
                   gST->ConOut,
                   Mode->CursorColumn,
                   Mode->CursorRow
                   );
    gST->ConOut->SetAttribute (gST->ConOut, Mode->Attribute);
  }
}

/**
  Creates a horizontal bar to put on the screen

  @param[out] Line                     Pointer to a newly allocated buffer containing the bar

  @retval CHAR16*                      Pointer to a newly allocated buffer containing the bar
**/
CHAR16*
CreateRedYellowStripe (
  OUT CHAR16      *Line
  )
{
  UINTN  Index;

  gST->ConOut->SetAttribute (gST->ConOut, EFI_YELLOW | EFI_BACKGROUND_RED);
  for (Index = 0; Index < mConOutCols; Index++) {
    if (Index % 2) {
      Line[Index] = L' ';
    } else {
      Line[Index] = BLOCKELEMENT_FULL_BLOCK;
    }
  }
  Line[mConOutCols] = L'\0';
  return Line;
}

/**
  Sets screen title which is displayed at top of the screen
**/
VOID
SetScreenTitle (
  VOID
  )
{
  UINTN   TitleSize;
  CHAR16  *Title = L"Platform Erase";

  TitleSize = StrSize (Title);
  mScreenTitle = AllocateZeroPool (TitleSize);
  ASSERT (mScreenTitle != NULL);
  if (mScreenTitle != NULL) {
    CopyMem (mScreenTitle, Title, TitleSize);
  }
}

/**
  Displays a footer on the screen
**/
VOID
PrintFooter (
  VOID
  )
{
  EFI_SIMPLE_TEXT_OUTPUT_MODE          SavedConsoleMode;
  CHAR16                               *Line;

  Line = AlocateFilledLine (L' ');
  if (Line == NULL) {
    return;
  }

  PreserveConsoleMode (&SavedConsoleMode);

  gST->ConOut->SetAttribute (gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLUE);

  gST->ConOut->SetCursorPosition (gST->ConOut, 0, mConOutRows - 3);
  gST->ConOut->OutputString (gST->ConOut, Line);

  gST->ConOut->SetCursorPosition (gST->ConOut, 0, mConOutRows - 2);
  gST->ConOut->OutputString (gST->ConOut, Line);

  gST->ConOut->SetCursorPosition (gST->ConOut, 0, mConOutRows - 1);
  gST->ConOut->OutputString (gST->ConOut, Line);

  CreateRedYellowStripe (Line);
  gST->ConOut->SetCursorPosition (gST->ConOut, 0, mConOutRows - 4);
  gST->ConOut->OutputString (gST->ConOut, Line);

  FreePool (Line);
  RestoreConsoleMode (&SavedConsoleMode);
}

/**
  Fills the screens background
**/
VOID
FillBackground (
  VOID
  )
{
  UINTN                                Index;
  EFI_SIMPLE_TEXT_OUTPUT_MODE          SavedConsoleMode;
  CHAR16                               *Line;

  Line = AlocateFilledLine (L' ');
  if (Line == NULL) {
    return;
  }

  PreserveConsoleMode (&SavedConsoleMode);

  gST->ConOut->SetAttribute (gST->ConOut,EFI_WHITE | EFI_BACKGROUND_BLACK);
  for (Index = 5; Index < mConOutRows - 3; Index++) {
    gST->ConOut->SetCursorPosition (gST->ConOut, 0, Index);
    gST->ConOut->OutputString (gST->ConOut, Line);
  }
  FreePool (Line);
  RestoreConsoleMode (&SavedConsoleMode);
}


/**
  Prints header at top of the screen and displays the screen title
**/
VOID
PrintHeader (
  VOID
  )
{
  EFI_SIMPLE_TEXT_OUTPUT_MODE          SavedConsoleMode;
  CHAR16                               *Line;

  Line = AlocateFilledLine (L' ');
  if (Line == NULL) {
    return;
  }

  PreserveConsoleMode (&SavedConsoleMode);
  gST->ConOut->SetAttribute (gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLUE);

  gST->ConOut->SetCursorPosition (gST->ConOut, 0, 1);
  gST->ConOut->OutputString (gST->ConOut, Line);
  gST->ConOut->SetCursorPosition (gST->ConOut, 0, 3);
  gST->ConOut->OutputString (gST->ConOut, Line);

  if (mScreenTitle != NULL) {
    CopyMem (
      &Line[(mConOutCols - StrLen (mScreenTitle) + 1) / 2],
      mScreenTitle,
      StrLen (mScreenTitle) * sizeof (CHAR16)
      );
  }

  gST->ConOut->SetCursorPosition (gST->ConOut, 0, 2);
  gST->ConOut->OutputString (gST->ConOut, Line);

  CreateRedYellowStripe (Line);
  gST->ConOut->SetCursorPosition (gST->ConOut, 0, 4);
  gST->ConOut->OutputString (gST->ConOut, Line);

  RestoreConsoleMode (&SavedConsoleMode);

  FreePool (Line);
}

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
  )
{
  EFI_SIMPLE_TEXT_OUTPUT_MODE          SavedConsoleMode;

  PreserveConsoleMode (&SavedConsoleMode);
  gST->ConOut->SetCursorPosition (gST->ConOut, Column, Row);
  gST->ConOut->SetAttribute (gST->ConOut, Attribute);
  gST->ConOut->OutputString (gST->ConOut, String);

  RestoreConsoleMode (&SavedConsoleMode);
}

/**
  Redraws an empty screen with title only
**/
VOID
ClearScreen (
  VOID
  )
{
  gST->ConOut->EnableCursor (gST->ConOut ,FALSE);
  gST->ConOut->ClearScreen (gST->ConOut);
  PrintHeader ();
  FillBackground ();
  PrintFooter ();
  gST->ConOut->SetCursorPosition (gST->ConOut, 5, 5);
}

/**
  Outputs provided c string at next line.

  @param[in] String     C string to be printed
  @param[in] Attribute  The attribute to set. Bits 0..3 are the
                        foreground color, and bits 4..6 are the
                        background color. All other bits are
                        undefined and must be zero. The valid
                        Attributes are defined in this file.
**/
VOID
OutputStringAtNext (
  IN CHAR16 *String,
  IN UINTN  Attribute
  )
{
  EFI_SIMPLE_TEXT_OUTPUT_MODE SavedConsoleMode;

  CopyMem (&SavedConsoleMode, gST->ConOut->Mode, sizeof (EFI_SIMPLE_TEXT_OUTPUT_MODE));
  gST->ConOut->SetCursorPosition (gST->ConOut, 0, SavedConsoleMode.CursorRow + 1);
  gST->ConOut->SetAttribute (gST->ConOut, Attribute);
  gST->ConOut->OutputString (gST->ConOut, String);
}

/**
  Output provided c string at current line.

  @param[in] String    C string to be printed
  @param[in] Attribute The attribute to set. Bits 0..3 are the
                       foreground color, and bits 4..6 are the
                       background color. All other bits are
                       undefined and must be zero. The valid
                       Attributes are defined in this file.
**/
VOID
UpdateString (
  IN CHAR16 *String,
  IN UINTN  Attribute
  )
{
  EFI_SIMPLE_TEXT_OUTPUT_MODE SavedConsoleMode;
  CHAR16                      *Line;

  Line = AllocateZeroPool ((mConOutCols + 1) * sizeof (CHAR16));;
  if (Line == NULL) {
    return;
  }
  SetMem16 (Line, (mConOutCols) * 2, L' ');

  CopyMem (&SavedConsoleMode, gST->ConOut->Mode, sizeof (EFI_SIMPLE_TEXT_OUTPUT_MODE));
  gST->ConOut->SetCursorPosition (gST->ConOut, 0, SavedConsoleMode.CursorRow);
  gST->ConOut->SetAttribute (gST->ConOut, Attribute);
  gST->ConOut->OutputString (gST->ConOut, Line);
  gST->ConOut->SetCursorPosition (gST->ConOut, 0, SavedConsoleMode.CursorRow);
  gST->ConOut->OutputString (gST->ConOut, String);
  FreePool (Line);
}

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
  )
{
  EFI_SIMPLE_TEXT_OUTPUT_MODE SavedConsoleMode;

  CopyMem (&SavedConsoleMode, gST->ConOut->Mode, sizeof (EFI_SIMPLE_TEXT_OUTPUT_MODE));
  gST->ConOut->SetCursorPosition (gST->ConOut, SavedConsoleMode.CursorColumn, SavedConsoleMode.CursorRow);
  gST->ConOut->SetAttribute (gST->ConOut, Attribute);
  gST->ConOut->OutputString (gST->ConOut, String);
}

/**
  clear the popup dialog
**/
VOID
ClearPopupScreen (
  VOID
  )
{
  UINTN                       Columns;
  UINTN                       Rows;
  UINTN                       Row;
  CHAR16                      *Line;
  UINTN                       Index;
  EFI_SIMPLE_TEXT_OUTPUT_MODE SavedConsoleMode;

  Line = NULL;

  CopyMem (&SavedConsoleMode, gST->ConOut->Mode, sizeof (EFI_SIMPLE_TEXT_OUTPUT_MODE));
  gST->ConOut->QueryMode (gST->ConOut, SavedConsoleMode.Mode, &Columns, &Rows);
  Row = (Rows) / 2 - 4;

  Line = (CHAR16*)AllocateZeroPool ((Columns + 1) * sizeof (CHAR16));

  if (Line == NULL) {
    DEBUG ((DEBUG_ERROR, "Platform Erase :: %a Failed to allocate memory", __FUNCTION__));
    return;
  }

  SetMem16 (Line, (Columns) * 2, L' ');
  Line[Columns] = L'\0';

  gST->ConOut->SetAttribute (gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
  for (Index = 0; Index <= 6; Index++) {
    gST->ConOut->SetCursorPosition (gST->ConOut, 0, Row + Index);
    gST->ConOut->OutputString (gST->ConOut, Line);
  }
  FreePool (Line);

  gST->ConOut->SetCursorPosition (gST->ConOut, SavedConsoleMode.CursorColumn, SavedConsoleMode.CursorRow);
  gST->ConOut->SetAttribute (gST->ConOut, SavedConsoleMode.Attribute);
}

/**
  Displays a progress bar dialog to indicate ongoing operation.

  @param[in] Message                   A unicode string to display above the bar
  @param[in] Delay                     The amount of time for which the text will be displayed in seconds.
**/
VOID
ProgressBarDialog (
  IN CHAR16 *Message,
  IN UINT32 Delay
  )
{
  EFI_SIMPLE_TEXT_OUTPUT_MODE          SavedConsoleMode;
  UINTN                                BarXPos;
  UINTN                                BarYPos;
  UINTN                                BarLen;
  UINTN                                IterationTime;
  CHAR16                               *Bar;
  UINTN                                DotPos;
  UINTN                                Index;
  UINTN                                Columns;
  UINTN                                Rows;

  PreserveConsoleMode (&SavedConsoleMode);
  gST->ConOut->QueryMode (gST->ConOut, gST->ConOut->Mode->Mode, &Columns, &Rows);

  BarLen = Columns / 3;
  BarXPos = (Columns - BarLen) / 2;
  BarYPos = (Rows / 2) + 1;
  Bar = AllocateZeroPool ((BarLen + 1) * 2);
  if (Bar == NULL) {
    return;
  }

  DotPos = 0;
  IterationTime = Delay * 1000000 / BarLen;

  if (NULL != Message) {
    gST->ConOut->SetAttribute (gST->ConOut, EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY);
    gST->ConOut->SetCursorPosition (gST->ConOut, (Columns - StrLen (Message) + 1) / 2, (Rows / 2) - 1);
    gST->ConOut->OutputString (gST->ConOut, Message);
  }
  gST->ConOut->SetAttribute (gST->ConOut, EFI_YELLOW | EFI_BACKGROUND_RED);
  for (Index = BarLen - 1; Index; Index--) {
    Bar[Index] = L' ';
  }
  while (TRUE) {
    Bar[DotPos] = L' ';
    DotPos++;
    if (DotPos == BarLen) {
      break;
    }
    Bar[DotPos] = BLOCKELEMENT_FULL_BLOCK;
    if (DotPos + 1 < BarLen) {
      Bar[DotPos + 1] = BLOCKELEMENT_FULL_BLOCK;
    }
    gST->ConOut->SetCursorPosition (gST->ConOut, BarXPos, BarYPos);
    gST->ConOut->OutputString (gST->ConOut, Bar);
    gST->ConOut->SetCursorPosition (gST->ConOut, BarXPos, BarYPos + 1);
    gST->ConOut->OutputString (gST->ConOut, Bar);
    gBS->Stall (IterationTime);
  }

  RestoreConsoleMode (&SavedConsoleMode);
  FreePool (Bar);
}

/**
  Prints summary message after Platform Sanitize - SSD Erase operation has ended

  @param[in] Status                 SSD Erase operation result

**/
VOID
PsStoragePrintSummary (
  IN EFI_STATUS Status
  )
{
  DEBUG ((DEBUG_INFO, "Platform Erase :: %a\n", __FUNCTION__));
  ClearScreen ();

  if (EFI_ERROR (Status)) {
    ProgressBarDialog (L"System has failed to erase the device. Exiting current SSD Erase attempt in 5 seconds.", 5);
  } else {
    ProgressBarDialog (L"Disk erase completed. Exiting SSD Erase in 5 seconds", 5);
  }
}
