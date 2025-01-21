/** @file
  Implementation file for Watchdog Timer functionality

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#include <PiDxe.h>
#include <Guid/HobList.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ConfigBlockLib.h>
#include <Protocol/Wdt.h>
#include <Library/WdtCommonLib.h>
#include <Register/PmcRegs.h>
#include <Library/UefiLib.h>
#include <Protocol/ResetNotification.h>

VOID
EFIAPI
WdtRunBeforeOsBoot (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );


VOID
EFIAPI
WdtPchResetCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  );

#define TIMEOUT_AFTER_POST_MULTIPLIER 16
#define MINIMUM_TIMEOUT_AT_S4_EXIT    600 ///< 10 minutes
GLOBAL_REMOVE_IF_UNREFERENCED EFI_HANDLE                  mImageHandle;
GLOBAL_REMOVE_IF_UNREFERENCED WDT_PROTOCOL                mWdtProtocol = {
  WdtReloadAndStart,
  WdtCheckStatus,
  WdtDisable,
  WdtAllowKnownReset,
  IsWdtRequired,
  IsWdtEnabled
};

/**
  Hook to reset notification protocol to properly reset function with Wdt.

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
    Status = ResetNotify->RegisterResetNotify (ResetNotify, WdtPchResetCallback);
    DEBUG ((DEBUG_INFO, "WdtPchResetCallback() installed\n"));
    if (Event) gBS->CloseEvent (Event);
  }
}


/**
  <b>WDT DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The WDT DXE provides a standard way for other modules to use the Watch dog timer Interface.
    This also registers a callback function to enable WDT if an OS application requested that
    just before booting to OS.

  - @pre
    - PCH PMIO Base configured
    - WDT driver needs to be executed before PCH Reset driver, so please update
      the FDF file accordingly to use APRIORI list to make sure that.

  - @result
    The WDT DXE module produces @link Include/Protocol/Wdt.h WDT_PROTOCOL @endlink.

  @param[in] ImageHandle          Image handle for this driver image
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             WDT DXE driver initialization completed successfully
**/
EFI_STATUS
EFIAPI
WdtDxeEntryPoint (
  IN  EFI_HANDLE       ImageHandle,
  IN  EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_EVENT                         Event;
  VOID                              *Registration;

  DEBUG ((DEBUG_INFO, "(Wdt) Entry Point to WdtDxe\n"));

  mImageHandle = ImageHandle;

  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_CALLBACK,
                  WdtRunBeforeOsBoot,
                  NULL,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "(Wdt) WDT event registration; Status = %r\n", Status));

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gWdtProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mWdtProtocol
                  );
  ASSERT_EFI_ERROR (Status);


  EfiCreateProtocolNotifyEvent ( &gEfiResetNotificationProtocolGuid,
                                 TPL_CALLBACK,
                                 OnResetNotifyInstall,
                                 NULL,
                                 &Registration
                                 );
  return EFI_SUCCESS;
}

/**
  Turns on watchdog timer just before booting to OS, if an OS application requested that.
  Clears request status.
  Uninstalls Wdt protocol to prevent other modules from interfering with actions described above.

  @param[in] Event                useless here, but required in functions invoked by events
  @param[in] Context              useless here, but required in functions invoked by events

  @retval    None
**/
VOID
EFIAPI
WdtRunBeforeOsBoot (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
{
  UINT32                            ReloadValue;
  UINT32                            Readback;
  EFI_STATUS                        Status;
  EFI_RESET_NOTIFICATION_PROTOCOL   *ResetNotify;

  gBS->CloseEvent (Event);

  DEBUG ((DEBUG_INFO, "(Wdt) RunWdtBeforeOsBoot\n"));
  gBS->UninstallProtocolInterface (
         mImageHandle,
         &gWdtProtocolGuid,
         &mWdtProtocol
         );

  Status = gBS->LocateProtocol (&gEfiResetNotificationProtocolGuid, NULL, (VOID **) &ResetNotify);
  if (!EFI_ERROR (Status)) {
    Status = ResetNotify->UnregisterResetNotify (ResetNotify, WdtPchResetCallback);
    ASSERT_EFI_ERROR (Status);
  }



  Readback    = IoRead32 (WdtGetAddress ());
  ReloadValue = TIMEOUT_AFTER_POST_MULTIPLIER * ((Readback & B_ACPI_IO_OC_WDT_CTL_AFTER_POST) >> 16);

  if (ReloadValue != 0) {
    ///
    /// start WDT with TimeoutValueAfterPost and clear that value from register
    ///
    Readback &= ~(B_ACPI_IO_OC_WDT_CTL_AFTER_POST);
    IoWrite32 (WdtGetAddress (), Readback);
    WdtReloadAndStart (ReloadValue);
  } else {
    WdtDisable ();
  }

  return;

}

/**
  WDT call back function for Reset notification.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
WdtPchResetCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  WdtAllowKnownReset ();
}
