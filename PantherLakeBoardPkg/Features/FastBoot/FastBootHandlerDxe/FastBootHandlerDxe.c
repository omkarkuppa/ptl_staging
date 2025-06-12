/** @file
  This driver updates boot order for fast boot.

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

@par Specification
**/

#include <Base.h>
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/FastBootException.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/PrintLib.h>
#include <Guid/GlobalVariable.h>
#include <SetupVariable.h>
#include <Library/HobLib.h>
#include <Library/DxeAsfLib.h>
#include <AttemptUsbFirst.h>
#include <PlatformNvRamHookLib.h>
#include <Protocol/ResetNotification.h>
#include <Guid/EventGroup.h>
#include <Library/EcMiscLib.h>
#include <Protocol/AsfProtocol.h>

#define BOOT_MANAGER_USB_ENTRY   L"USB Entry for Windows To Go"
#define INTERNAL_UEFI_SHELL_NAME L"Internal UEFI Shell"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_EVENT               mEndOfDxeEvent;
GLOBAL_REMOVE_IF_UNREFERENCED SETUP_DATA              *mSystemConfiguration = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                 mAsfBootOptionsPresent = FALSE;

/**
  Detect if Exception occured.
  
  @param         ExceptionCategory
  @retval True   If exception occured.
  @retval Flase  If exception did not occur.
**/
BOOLEAN
DxeFastBootExceptionCheck(
  FAST_BOOT_EXCEPTION_CATEGORY  *ExceptionCategory
  )
{
  EFI_STATUS                    Status;
  FAST_BOOT_EXCEPTION_PROTOCOL  *FastBootExceptionProtocol;
  BOOLEAN                       ExceptionOccurred;
  EFI_HANDLE                    *Handle;
  UINTN                         Number;
  UINTN                         Index;

  //
  // Locate all handles of Fast Boot Exception protocol to find out if any exception has occurred.
  //
  ExceptionOccurred = FALSE;
  Handle            = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gFastBootExceptionProtocolGuid,
                  NULL,
                  &Number,
                  &Handle
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < Number; Index++) {
      Status = gBS->HandleProtocol (
                      Handle[Index],
                      &gFastBootExceptionProtocolGuid,
                      (VOID **) &FastBootExceptionProtocol
                      );
      if (EFI_ERROR (Status)) {
        continue;
      }
      if (FastBootExceptionProtocol->FbExceptionType > NoException) {
        ExceptionOccurred  = TRUE;
        *ExceptionCategory = FastBootExceptionProtocol->FbExceptionCategory;
        break;
      }
    }
    if (Handle != NULL) {
      gBS->FreePool (Handle);
    }
  }
  return ExceptionOccurred;
}

/**
  Check Fast Boot is enabled or not

  @retval TRUE                   Fast Boot is enabled
  @retval FALSE                  Fast Boot is disabled
**/
BOOLEAN
FastBootEnabled (
  VOID
  )
{
  if (mSystemConfiguration == NULL) {
    GetVariable2 (L"Setup", &gSetupVariableGuid, (VOID **) &mSystemConfiguration, NULL);
    if (mSystemConfiguration == NULL) {
      return FALSE;
    }
  }

  return mSystemConfiguration->FastBoot != 0 ;
}


/**
  Raise Fast Boot Exception with specified exception type & category

  @param[in] ExceptionType      Fast Boot Exception type
  @param[in] ExceptionCategory  Fast Boot Exception category

  @retval    EFI_SUCCESS        Fast Boot Exception successfully raised
  @retval    others             Fast Boot Exception protocol installation failed
**/
EFI_STATUS
RaiseFastBootException (
  FAST_BOOT_EXCEPTION_TYPE      ExceptionType,
  FAST_BOOT_EXCEPTION_CATEGORY  ExceptionCategory
  )
{
  FAST_BOOT_EXCEPTION_PROTOCOL  *FastBootException;
  EFI_HANDLE                    Handle;

  if (!FastBootEnabled ()) {
    return EFI_SUCCESS;
  }

  FastBootException = AllocatePool (sizeof (FAST_BOOT_EXCEPTION_PROTOCOL));
  ASSERT (FastBootException != NULL);
  if (FastBootException == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  FastBootException->FbExceptionType     = ExceptionType;
  FastBootException->FbExceptionCategory = ExceptionCategory;

  Handle = NULL;
  return gBS->InstallProtocolInterface (
                &Handle,
                &gFastBootExceptionProtocolGuid,
                EFI_NATIVE_INTERFACE,
                FastBootException
                );
}

/**
  Raise Fast Boot Exception if ASF Boot Options is available

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
AsfBootOptionsEventCallBack (
  EFI_EVENT Event,
  VOID      *Context
  )
{
  EFI_STATUS                     Status;
  FAST_BOOT_EXCEPTION_CATEGORY  ExceptionCategory   = 0;
  ALERT_STANDARD_FORMAT_PROTOCOL *AsfProtocol;

  Status = gBS->LocateProtocol (
                  &gAlertStandardFormatProtocolGuid,
                  NULL,
                  (VOID **) &AsfProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a() Locate ASF Protocol failed, Status = %r\n", __FUNCTION__, Status));
  }

  DxeFastBootExceptionCheck (&ExceptionCategory);

  if (!EFI_ERROR (Status) && AsfIsBootOptionsPresent ()) {
    if ((ExceptionCategory != BootFailure) && (AsfProtocol->AsfBootOptions.SpecialCommand != ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD)) {
      RaiseFastBootException (ExceptionType2, SpecialBoot);
    }
    mAsfBootOptionsPresent = TRUE;
  }
}

/**
  Put current boot option to the 1st option in the boot option list

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
FastBootUpdateBootOrder (
  EFI_EVENT Event,
  VOID      *Context
  )
{
  EFI_STATUS                          Status;
  UINT16                              *BootCurrent;
  UINT16                              *BootOrder;
  UINT16                              *NewBootOrder;
  UINTN                               Size;
  UINTN                               BootOptionCount;
  CHAR16                              OptionName[sizeof ("Boot####")];
  EFI_BOOT_MANAGER_LOAD_OPTION        BootOption;
  UINTN                               DataSize;
  ATTEMPT_USB_FIRST_RUNTIME_VARIABLE  AttemptUsbFirstRuntimeVarInfo;

  //
  // Skip updating boot order for special boot types
  //
  if (mAsfBootOptionsPresent) {
    return;
  }

  AttemptUsbFirstRuntimeVarInfo.RevisonId = 0;
  AttemptUsbFirstRuntimeVarInfo.UsbFirstEnable = 0;

  //
  // Read 'AttemptUSBFirstRuntime' variable
  //
  DataSize = sizeof (ATTEMPT_USB_FIRST_RUNTIME_VARIABLE);
  Status = gRT->GetVariable (
                  L"AttemptUSBFirstRuntime",
                  &gAttemptUsbFirstRuntimeVarInfoGuid,
                  NULL,
                  &DataSize,
                  &AttemptUsbFirstRuntimeVarInfo
                  );
  if (AttemptUsbFirstRuntimeVarInfo.UsbFirstEnable) {
    //
    // Don't update 'BootOrder' variable if 'Attempt USB First' is enabled
    //
    return;
  }

  GetEfiGlobalVariable2 (L"BootCurrent", (VOID **) &BootCurrent, NULL);
  if (BootCurrent == NULL) {
    return;
  }

  UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04X", *BootCurrent);
  Status = EfiBootManagerVariableToLoadOption (OptionName, &BootOption);
  ASSERT_EFI_ERROR (Status);

  //
  // Skip boot order update for following cases -
  // 1) Windows To Go feature is enabled
  // 2) In fast boot mode and last boot was to EFI shell.
  //    When BIOS successfully boots to any boot option, the BIOS shall store
  //    the successful boot target at the beginning of the boot list for subsequent boots.
  //    EFI Shell is considered as EFI application and still uses boot services.
  //    Booting to shell will not trigger exit boot services which will actually trigger the
  //    boot order change for subsequent boots. It is expected that "Shell" boot option will not
  //    move to top of boot order even after successfully booting to shell in fast boot mode.
  //
  if ((StrCmp (BootOption.Description, BOOT_MANAGER_USB_ENTRY) == 0) ||
      (StrCmp (BootOption.Description, INTERNAL_UEFI_SHELL_NAME) == 0)) {
    EfiBootManagerFreeLoadOption (&BootOption);
    FreePool (BootCurrent);
    return;
  }

  //
  //  Skip application type
  //
  if ((BootOption.Attributes & LOAD_OPTION_CATEGORY) != LOAD_OPTION_CATEGORY_BOOT) {
    EfiBootManagerFreeLoadOption (&BootOption);
    FreePool (BootCurrent);
    return;
  }

  GetEfiGlobalVariable2 (L"BootOrder", (VOID **) &BootOrder, &Size);
  if (BootOrder == NULL) {
    FreePool (BootCurrent);
    return;
  }

  if (*BootCurrent != BootOrder[0]) {
    UINTN Index;
    UINTN Index2;

    BootOptionCount = Size / sizeof (UINT16);
    //
    // Check overflow error for BootOptionCount
    //
    if (((Size != 0) && BootOptionCount) && (Size % sizeof (UINT16) != 0)) {
      FreePool (BootCurrent);
      FreePool (BootOrder);
      return;
    }

    NewBootOrder = AllocatePool ((BootOptionCount + 1) * sizeof (UINT16));
    ASSERT (NewBootOrder != NULL);
    if (NewBootOrder == NULL) {
      FreePool (BootCurrent);
      FreePool (BootOrder);
      return;
    }

    NewBootOrder[0] = *BootCurrent;
    for (Index = 0, Index2 = 1; Index2 < BootOptionCount; Index++) {
      if (BootOrder[Index] != *BootCurrent) {
        NewBootOrder[Index2++] = BootOrder[Index];
      }
    }

    Size = BootOptionCount * sizeof (UINT16);
    gRT->SetVariable (
           L"BootOrder",
           &gEfiGlobalVariableGuid,
           EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
           Size,
           NewBootOrder
           );
    FreePool (NewBootOrder);
  }

  FreePool (BootCurrent);
  FreePool (BootOrder);
}

/**
  Clear boot progress bit at lanch UEFI Shell.

  @param[in] Event     Event whose notification function is being invoked
  @param[in] Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnEnterShellCallBack (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS                        Status;
  VOID                              *Protocol;
  Status = gBS->LocateProtocol (
                  &gEfiShellProtocolGuid,
                  NULL,
                  &Protocol
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }
  // Clear Boot Progress bit [bit0] when lanch UEFI Shell
  UpdateFastBootFlagStatus (GetFastBootFlagStatus () & ~BIT0);
  if (Event) {
    gBS->CloseEvent (Event);
  }
}

/**
  Clear boot progress bit at lanuch setup menu.

  @param[in] Event     Event whose notification function is being invoked
  @param[in] Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnEnterSetupCallBack (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
{
  EFI_STATUS      Status;
  VOID            *Protocol;

  Status = gBS->LocateProtocol (
                  &gSetupEnterGuid,
                  NULL,
                  &Protocol
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }
  // Clear Boot Progress bit [bit0]
  UpdateFastBootFlagStatus (GetFastBootFlagStatus () & ~BIT0);
  if (Event) {
    gBS->CloseEvent (Event);
  }

}

/**
  Call back function for reset notification.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
FastBootResetNotificationCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  FAST_BOOT_EXCEPTION_CATEGORY  ExceptionCategory = 0;

  if ((FastBootEnabled ()) && (DxeFastBootExceptionCheck (&ExceptionCategory))) {
    if (ExceptionCategory == SpecialBoot) {
      DEBUG ((DEBUG_INFO, "Do not clear FastBoot Flag \n"));
    }
  } else {
    // Clear Boot Progress bit [bit0]
    UpdateFastBootFlagStatus (GetFastBootFlagStatus () & ~BIT0);
  }
}


/**
  Hook to reset notification protocol to properly reset function with FastBoot.
  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnResetNotifyInstall (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  EFI_STATUS  Status;
  EFI_RESET_NOTIFICATION_PROTOCOL   *ResetNotify;
  Status = gBS->LocateProtocol (&gEfiResetNotificationProtocolGuid, NULL, (VOID **) &ResetNotify);
  if (!EFI_ERROR (Status)) {
    ResetNotify->RegisterResetNotify (ResetNotify, FastBootResetNotificationCallback);
    if (Event) {
      gBS->CloseEvent (Event);
    }
  }
}

/**
  This function handles FastBootHandler task at the end of DXE.
  @param[in]  Event     The Event this notify function registered to.
  @param[in]  Context   Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
OnEndofDxeCallback (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  static BOOLEAN                S3DataSaved = FALSE;
  UINT64                        OsIndication;
  UINTN                         DataSize;
  UINT8                         Value;
  EFI_STATUS                    Status;

  gBS->CloseEvent (Event);

  if (S3DataSaved) {
    return;
  }

  S3DataSaved = TRUE;

  //
  // Read the home button value to determine if we need to override and boot straight to BIOS setup menu
  //
  Value = 0;
  if (EFI_ERROR (GetBoardButtonStatus (&Value))) {
    Value = 0;
  }

  if (((Value >> 3) & 0x1) == 0x1) {
    //
    // Read OsIndication register and set EFI_OS_INDICATIONS_BOOT_TO_FW_UI to boot to BIOS setup menu
    //
    OsIndication = 0;
    DataSize = sizeof (UINT64);
    Status = gRT->GetVariable (
                    L"OsIndications",
                    &gEfiGlobalVariableGuid,
                    NULL,
                    &DataSize,
                    &OsIndication
                    );
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }

    OsIndication |= ((UINT64) EFI_OS_INDICATIONS_BOOT_TO_FW_UI);
    Status = gRT->SetVariable (
                    L"OsIndications",
                    &gEfiGlobalVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    sizeof(UINT64),
                    &OsIndication
                    );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "Home Button pressed, setting indicator to boot to Setup Menu\n"));
  }
  return;
}

/**
  Registers callback on EndOfDxeEvent for FastBootHandler

**/
VOID
RegisterOnEndOfDxeCallbacks (
  VOID
  )
{
  EFI_STATUS                     Status;

  //
  // Performing OnEndofDxeCallback after the gEfiEndOfDxeEventGroup is signaled.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnEndofDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &mEndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

}


/**
  This function is the entry point of the Fast Boot Exception Handler driver.

  @param[in] ImageHandle A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
**/
EFI_STATUS
EFIAPI
FastBootHandlerInitialize (
  IN EFI_HANDLE                           ImageHandle,
  IN EFI_SYSTEM_TABLE                     *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;
  VOID        *Registration;
  EFI_EVENT                     OnEnterShellEvent;
  EFI_EVENT                     OnEnterSetupEvent;

  Status = EFI_SUCCESS;

  if (FastBootEnabled ()) {
    //Register callback on EndOfDxe
    RegisterOnEndOfDxeCallbacks ();
    //
    // Register protocol notification for ASF to check whether ASF Boot Options
    // is available or not
    //
    EfiCreateProtocolNotifyEvent (
      &gAlertStandardFormatProtocolGuid,
      TPL_CALLBACK,
      AsfBootOptionsEventCallBack,
      NULL,
      &Registration
      );

    //
    // Create ReadyToBoot event for Fast Boot to update the current boot device
    // to the 1st in the boot list
    //
    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               FastBootUpdateBootOrder,
               NULL,
               &Event
               );
    EfiCreateProtocolNotifyEvent (
      &gEfiResetNotificationProtocolGuid,
      TPL_CALLBACK,
      OnResetNotifyInstall,
      (VOID*)&ImageHandle,
      &Registration
      );
    EfiCreateProtocolNotifyEvent (
      &gSetupEnterGuid,
      TPL_CALLBACK,
      OnEnterSetupCallBack,
      NULL,
      &OnEnterSetupEvent
      );
    EfiCreateProtocolNotifyEvent (
      &gEfiShellProtocolGuid,
      TPL_CALLBACK,
      OnEnterShellCallBack,
      NULL,
      &OnEnterShellEvent
      );
  }

  return Status;
}
