/**@file
  Storage platform related event notify function

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

#include <Base.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/EcMiscLib.h>

/**
  Creates and returns a notification event and registers that event by gConsoleOutConnectedProtocolGuid.

  @param  NotifyFunction  Supplies the function to notify when the event is signaled.
  @param  Registration    A pointer to a memory location to receive the registration value.
                          This value is passed to LocateHandle() to obtain new handles that
                          have been added that support the ProtocolGuid-specified protocol.

  @return The notification event that was created.

**/
EFI_EVENT
EFIAPI
RegisterNotifyForConsoleOutConnected (
  IN  EFI_EVENT_NOTIFY  NotifyFunction,
  OUT VOID              **Registration
  )
{
  if ((NotifyFunction == NULL) || (Registration == NULL)) {
    return NULL;
  }
  return EfiCreateProtocolNotifyEvent (
           &gConsoleOutConnectedProtocolGuid,
           TPL_CALLBACK,
           NotifyFunction,
           NULL,
           Registration
           );
}

/**
  Cancel Wdt.

  @retval EFI_SUCCESS       Successful.
  @retval Other             Failure.

**/
EFI_STATUS
EFIAPI
CancelWdt (
  VOID
  )
{
  //
  // Simply disarm wdt to avoid WDT causing reboot
  //
  return DisArmEcWdt ();
}
