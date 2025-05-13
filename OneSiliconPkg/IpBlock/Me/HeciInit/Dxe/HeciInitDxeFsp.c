/** @file
  The implementation about Me ReadyToBootEvent for FSP support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HeciInitLib.h>
#include <Library/MeChipsetLib.h>
#include <Library/PerformanceLib.h>
#include "MeInit.h"

EFI_EVENT mExitBootServicesEvent     = NULL;

/**
  The notification registered as a callback to perform Me event prior to boot

  @param[in] Event            The event that triggered this notification function
  @param[in] Context          Pointer to the notification functions context
**/
VOID
EFIAPI
MeReadyToBootEventDxeFsp (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));
  MeReadyToBoot ();

  gBS->CloseEvent (Event);
}

/**
  The notification registered as a callback to perform Me ExitBootServices event prior to boot.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.
**/
VOID
EFIAPI
MeEndOfFirmwareEventDxeFsp (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINT8  MeDevFunction;

  ///
  /// Don't set D0I3 if disabled by policy
  ///
  if (MeHeciD0I3Enabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "%a () - SKIP setting D0I3 bits for HECI devices on boot time\n", __FUNCTION__));
    return;
  }

  DEBUG ((DEBUG_INFO, "%a () - setting D0I3 bits for HECI devices on boot time\n", __FUNCTION__));

  //
  // Set D0i3 bit
  //
  for (MeDevFunction = HECI1; MeDevFunction <= HECI4; MeDevFunction++) {
    if (!IsHeciDeviceFunction (MeDevFunction)) {
      continue;
    }
    SetD0I3Bit (MeDevFunction);
  }

  return;
}

/**
  FSP HeciInit Module Entry Point for signal ReadyToBoot FSP event

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
HeciInitDxeFspEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     Event;
  EFI_HANDLE    EopInFspHandle;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  PERF_INMODULE_BEGIN ("HeciInitDxeFsp");
  ///
  /// Create a Ready to Boot event.
  ///
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             MeReadyToBootEventDxeFsp,
             NULL,
             &Event
             );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    //
    // Install the protocol to prevent duplicated EOP sent outside FSP.
    //
    EopInFspHandle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &EopInFspHandle,
                    &gMeSendEopInFspProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  MeEndOfFirmwareEventDxeFsp,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &mExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  PERF_INMODULE_END ("HeciInitDxeFsp");

  DEBUG ((DEBUG_INFO, "%a () End\n", __FUNCTION__));
  return EFI_SUCCESS;
}