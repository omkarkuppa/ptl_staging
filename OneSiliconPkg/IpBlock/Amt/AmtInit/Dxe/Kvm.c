/** @file
  KVM Initialization function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Protocol/AsfProtocol.h>
#include <Protocol/AmtDisplayMsgProtocol.h>

/**
  Initialize KVM by sending HECI message to ME

  @param[in] Event                The event registered.
  @param[in] Context              Event context. Not used in this event handler.
**/
VOID
EFIAPI
QueryKvm (
  IN EFI_EVENT                    Event,
  IN VOID                         *Context
  )
{
  EFI_STATUS                      Status;
  UINT32                          ResponseCode;
  AMT_DISPLAY_MSG_PROTOCOL        *DisplayMsgProtocol;

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gAmtDisplayMsgProtocolGuid, NULL, (VOID**) &DisplayMsgProtocol);
  if (EFI_ERROR (Status)) {
    DisplayMsgProtocol = NULL;
  }


  if (DisplayMsgProtocol != NULL) {
    DisplayMsgProtocol->DisplayAmtMsg (MsgKvmWait);
  }

  Status = AmtQueryKvm (QueryRequest, &ResponseCode);
  if (EFI_ERROR (Status)) {
    if (DisplayMsgProtocol != NULL) {
      DisplayMsgProtocol->DisplayAmtMsg (MsgKvmTimesUp);
    }
    AmtQueryKvm (CancelRequest, &ResponseCode);
  } else if (ResponseCode == KvmSessionCancelled) {
    if (DisplayMsgProtocol != NULL) {
      DisplayMsgProtocol->DisplayAmtMsg (MsgKvmRejected);
    }
  }

  return;
}


/**
  Register KVM Notification on ConsoleOut Device installed.

  @param[in]  Event    The event of notify protocol.
  @param[in]  Context  Notify event context.
**/
VOID
EFIAPI
KvmInit (
  IN EFI_EVENT     Event,
  IN VOID          *Context
  )
{
  EFI_STATUS                      Status;
  VOID                            *AfterConsoleOutNotifyReg;
  EFI_EVENT                       AfterConsoleOutInstalledEvent;
  ALERT_STANDARD_FORMAT_PROTOCOL  *AsfProtocol;

  Status = gBS->LocateProtocol (
                  &gAlertStandardFormatProtocolGuid,
                  NULL,
                  (VOID **) &AsfProtocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "%a () start\n", __FUNCTION__));

  gBS->CloseEvent (Event);
  if (!AsfIsKvmEnabled ()) {
    return;
  }
  ///
  /// Set up Event for KVM for when Output Display Console is installed
  ///
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  QueryKvm,
                  NULL,
                  &AfterConsoleOutInstalledEvent
                  );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gEfiConsoleOutDeviceGuid,
                    AfterConsoleOutInstalledEvent,
                    &AfterConsoleOutNotifyReg
                    );
    ASSERT_EFI_ERROR (Status);
  }
  return;
}

