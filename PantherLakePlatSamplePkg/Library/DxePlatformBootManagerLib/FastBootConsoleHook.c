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

#include "BdsPlatform.h"
#include "FastBootSupport.h"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL     *mOriginalStConOut;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL     *mFastBootConOut;


EFI_STATUS
EFIAPI
DummyOutputString (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *This,
  IN  CHAR16                             *WString
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DummyTestString (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *This,
  IN  CHAR16                             *WString
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DummySetAttribute (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL        *This,
  IN UINTN                                  Attribute
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DummySetMode (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *This,
  IN  UINTN                              ModeNumber
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DummySetCursorPosition (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *This,
  IN  UINTN                              Column,
  IN  UINTN                              Row
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DummyEnableCursor (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *This,
  IN  BOOLEAN                            Visible
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DummyClearScreen (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *This
  )
{
  return EFI_SUCCESS;
}

VOID
InstallFastBootConOut (
  VOID
  )
/*++

Routine Description:

    This function hooks gST->ConOut to supress text display if FastBoot is enabled

Arguments:

    None

--*/
{
   mFastBootConOut = AllocatePool (sizeof (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL));
   ASSERT (mFastBootConOut != NULL);
   CopyMem(mFastBootConOut, mOriginalStConOut, sizeof (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL));

   mOriginalStConOut->OutputString = DummyOutputString;
   mOriginalStConOut->TestString = DummyTestString;
   mOriginalStConOut->SetMode = DummySetMode;
   mOriginalStConOut->SetAttribute = DummySetAttribute;
   mOriginalStConOut->ClearScreen = DummyClearScreen;
   mOriginalStConOut->SetCursorPosition = DummySetCursorPosition;
   mOriginalStConOut->EnableCursor = DummyEnableCursor;
}

VOID
FastBootHookConOut (
  VOID
  )
/*++

Routine Description:

    This function hooks gST->ConOut to supress text display after Console Out is installed
    if FastBoot is enabled

Arguments:

    None

--*/
{
  if ((FastBootEnabled()) && (!IsExceptionType2Detected())) {
    mOriginalStConOut = gST->ConOut;
    InstallFastBootConOut ();
  }
}

VOID
FastBootRestoreConOut (
  VOID
  )
/*++

Routine Description:

    This function will be called to restore gST->ConOut after all drivers have been connected

Arguments:

    None

--*/
{
  if ((FastBootEnabled()) && (!IsExceptionType2Detected())) {
    ASSERT (mFastBootConOut != NULL);
    CopyMem(mOriginalStConOut, mFastBootConOut, sizeof (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL));
    FreePool(mFastBootConOut);
  }
}