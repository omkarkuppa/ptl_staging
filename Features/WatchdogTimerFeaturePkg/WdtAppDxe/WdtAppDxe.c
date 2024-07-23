/**@file
  Platform-specific Wdt code.  Offers more convenient and foolproof API for
  manipulating WDT than low-level WDT driver which is part of Reference Code.
  WdtApp is too platform-specific to be included in reference code.  However,
  it is expected that each client will need to write something similar on his own.
  This implementation uses EFI timers to kick WDT periodically.  There are other
  possibilities.  For example, a more robust way would be to kick WDT at certain
  milestones of BIOS code execution.

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

@par Specification
**/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/Wdt.h>
#include <WdtAppVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

VOID
EFIAPI
FeedWatchdog (
  IN EFI_EVENT Event,
  IN VOID      *Context
);

EFI_STATUS
EFIAPI
RequestWdtNow (
  VOID
);


EFI_EVENT  mFeedEvent;


/**

  @param[in] ImageHandle
  @param[in] SystemTable

  @retval    EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
WdtAppDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS           Status;
  WDT_PROTOCOL         *WdtProtocol;
  UINTN                VariableSize;
  UINT32               Attributes;
  WDT_PERSISTENT_DATA  WdtPersistentData;
  UINT8                WdtRequiredByOs;
  UINT8                WdtRunning;

  DEBUG ((DEBUG_INFO, "(WdtAppDxe) %a Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol(&gWdtProtocolGuid, NULL, (VOID **) &WdtProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "(WdtAppDxe) Failed to locate Wdt protocol, Status = %r\n",Status));
    return EFI_SUCCESS;
  }

  ZeroMem (&WdtPersistentData, sizeof (WDT_PERSISTENT_DATA));
  VariableSize  = sizeof (WDT_PERSISTENT_DATA);

  Status = gRT->GetVariable (
                  WDT_PERSISTENT_DATA_C_NAME,
                  &gWdtPersistentDataGuid,
                  &Attributes,
                  &VariableSize,
                  &WdtPersistentData
                  );
  if (EFI_ERROR (Status)) {
    //
    // If variable doesn't exist, it is treated as if it was equal to 0
    //
    WdtPersistentData.Enable = 0;
  }

  WdtRequiredByOs = WdtProtocol->IsWdtRequired();
  WdtRunning      = WdtProtocol->IsWdtEnabled();

  if (WdtRequiredByOs == TRUE || WdtRunning == TRUE || WdtPersistentData.Enable == 1) {
    DEBUG ((DEBUG_INFO, "(WdtAppDxe) RequestWdtNow\n"));
    RequestWdtNow ();
  }

  WdtPersistentData.Enable = 0;
  gRT->SetVariable(
         WDT_PERSISTENT_DATA_C_NAME,
         &gWdtPersistentDataGuid,
         EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
         sizeof (WDT_PERSISTENT_DATA),
         &WdtPersistentData
         );

  DEBUG ((DEBUG_INFO, "(WdtAppDxe) %a End\n", __FUNCTION__));

  return EFI_SUCCESS;
}


/**
  Allows protocol's clients to request that WDT be turned on and periodically kicked.
  If called for the 1st time, starts WDT and installs timer-based event that will kick WDT.
  If called again, does nothing - the event for kicking Wdt is already there.

  @retval     EFI_SUCCESS       The function completed successfully.

**/
EFI_STATUS
EFIAPI
RequestWdtNow (
  VOID
  )
{
  EFI_STATUS    Status;
  WDT_PROTOCOL  *WdtProtocol;
  static UINT8  AlreadyTurnedOn = 0;

  WdtProtocol = NULL;

  DEBUG ((DEBUG_INFO, "(WdtAppDxe) Wdt turned on\n"));
  if (AlreadyTurnedOn != 0) {
    return EFI_SUCCESS;
  }
  AlreadyTurnedOn = 1;

  Status = gBS->LocateProtocol (&gWdtProtocolGuid, NULL, (VOID **) &WdtProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "(WdtApp) Wdt protocol was uninstalled. Disabling RequestWdtNow, Status = %r.\n",Status));
    return EFI_SUCCESS;
  }

  Status = WdtProtocol->ReloadAndStart (PcdGet8 (PcdWdtTimeoutValue));
  ASSERT_EFI_ERROR(Status);

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  FeedWatchdog,
                  NULL,
                  &mFeedEvent
                  );
  ASSERT_EFI_ERROR (Status);
  Status = gBS->SetTimer (
                  mFeedEvent,
                  TimerPeriodic,
                  PcdGet32 (PcdWdtReloadTimer)
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
};

/**
  Prevents WDT timeout by restarting it.
  If WDT protocol can't be located - which means it was uninstalled - this function
  unregisters the event that triggers it periodically.
  This function executes with highest prority on timer interrupt.  Interrupt may
  fire just between some other lower-priority module disabling WDT and resetting
  platform.  To prevent such flow, this function checks if WDT is currently enabled,
  and skips reloading it when it is disabled.

  @param[in] Event
  @param[in] Context

**/
VOID
EFIAPI
FeedWatchdog (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  WDT_PROTOCOL  *WdtProtocol;
  UINT8         Enabled;

  WdtProtocol = NULL;

  Status = gBS->LocateProtocol (&gWdtProtocolGuid, NULL, (VOID **) &WdtProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "(WdtApp) Wdt protocol was uninstalled. Disabling FeedWatchdog, Status = %r.\n",Status));
    Status = gBS->SetTimer (mFeedEvent, TimerCancel, 0);
    ASSERT_EFI_ERROR(Status);
    Status = gBS->CloseEvent (mFeedEvent);
    ASSERT_EFI_ERROR(Status);
    return;
  }

  Enabled = WdtProtocol->IsWdtEnabled();

  if (Enabled == TRUE) {
    Status = WdtProtocol->ReloadAndStart(PcdGet8 (PcdWdtTimeoutValue));
  }
}
