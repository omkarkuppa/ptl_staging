/** @file
  DXE driver for platform ME hook protocol implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
//
// Silicon
//
#include <Protocol/PlatformMeHook.h>

/**
  Platform hook before BIOS sends Global Reset Heci Message to ME

  @retval EFI Status Code

**/
EFI_STATUS
EFIAPI
PlatformMeHookPreGlobalReset (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Platform hook before BIOS sends Data Clear Heci Message to ME

  @param[out]  ProceedDataClear  Pointer to consent sending Data Clear HECI message.

  @retval  EFI_SUCCESS  Succeed to execute the function.
           Others       Fail to execute the function.

**/
EFI_STATUS
EFIAPI
PlatformMeHookPreDataClear (
  OUT BOOLEAN  *ProceedDataClear
  )
{
  *ProceedDataClear = TRUE;

  gST->ConOut->ClearScreen (gST->ConOut);

  CreatePopUp (
    EFI_DARKGRAY | EFI_BACKGROUND_BROWN,
    NULL,
    L"",
    L"CSE Data Resiliency",
    L"MfsFailure is detected. Sending CSE Data Clear command...",
    L"",
    NULL
    );

  gBS->Stall (3000000); // Three seconds delay after poping up message

  return EFI_SUCCESS;
}

/**
  Platform hook after BIOS sends Data Clear Heci Message to ME

  @param[in]  DataClearStatus  Indicate the status of sending Data Clear HECI message.

  @retval  EFI_SUCCESS  Succeed to execute the function.
           Others       Fail to execute the function.

**/
EFI_STATUS
EFIAPI
PlatformMeHookPostDataClear (
  IN EFI_STATUS  DataClearStatus
  )
{
  CHAR16      StatusString[200];
  UINTN       MsgAttribute;

  gST->ConOut->ClearScreen (gST->ConOut);

  if (!EFI_ERROR (DataClearStatus)) {
    MsgAttribute = EFI_DARKGRAY  | EFI_BACKGROUND_BROWN;
  } else {
    MsgAttribute = EFI_LIGHTGRAY | EFI_BACKGROUND_RED;
  }

  UnicodeSPrint (StatusString, sizeof (StatusString), L"Sent Data Clear command: %r", DataClearStatus);
  CreatePopUp (
    MsgAttribute,
    NULL,
    L"",
    L"CSE Data Resiliency",
    StatusString,
    L"",
    NULL
    );

  gBS->Stall (3000000); // Three seconds delay after poping up message

  return EFI_SUCCESS;
}

//
// Protocol context to expose the services.
//
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_ME_HOOK_PROTOCOL  mPlatformMeHookProtocolContext = {
  PlatformMeHookPreGlobalReset, // PLATFORM_ME_HOOK_PRE_GLOBAL_RESET
  PlatformMeHookPreDataClear,   // PLATFORM_ME_HOOK_PRE_DATA_CLEAR
  PlatformMeHookPostDataClear   // PLATFORM_ME_HOOK_POST_DATA_CLEAR
};

/**
  Entry point of this module.

  @param[in]  ImageHandle  The firmware allocated handle for the EFI image.
  @param[in]  SystemTable  A pointer to the EFI System Table.

  @retval  EFI_SUCCESS          Entry point of this module executes successfully.
  @retval  EFI_ALREADY_STARTED  Protocol instance had been started.
  @retval  Others               Failed to initialize this module.

**/
EFI_STATUS
EFIAPI
PlatformMeHookDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS       Status;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPlatformMeHookProtocolGuid,
                  &mPlatformMeHookProtocolContext,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Protocol install error. Status = %r.\n", __FUNCTION__, Status));
  }

  return Status;
}
