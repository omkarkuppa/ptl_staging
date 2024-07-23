/** @file
  HECI driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2007 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HeciInitLib.h>
#include <IntelRcStatusCode.h>
#include "MeInit.h"
#include "HeciInit.h"

GLOBAL_REMOVE_IF_UNREFERENCED HECI_INSTANCE *mHeciContext;

/**
  Callback function to check asynchronous event when the timer event is signaled.

  @param[in]  Event     The Event this notify function registered to.
  @param[in]  Context   Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
ProcessAsyncTaskList (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  EFI_STATUS        Status;
  HECI_PROTOCOL     *Heci;
  UINT32            MockRecvBuffer[HECI_MOCK_RECV_BUFFER_SIZE];
  UINT32            MockRecvLength;

  MockRecvLength = sizeof (MockRecvBuffer);
  Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &Heci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Heci protocol error\n"));
    return;
  }

  Status = Heci->ReadMsg (HECI1, NON_BLOCKING, (UINT32 *) &MockRecvBuffer, &MockRecvLength);
}

/**
  HECI driver entry point used to initialize support for the HECI device.

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
InitializeHeci (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       Event;
  UINT32                          MeStatus;
  UINT32                          MeMode;

  DEBUG ((DEBUG_INFO, "ME-BIOS: HECI Protocol Entry.\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_INIT_START); //PostCode (0xE04)

  mHeciContext = AllocateZeroPool (sizeof (HECI_INSTANCE));

  ///
  /// Initialize HECI protocol pointers
  ///
  if (mHeciContext == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI Protocol Exit - Error by lack of memory for HECI_INSTANCE.\n"));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_EXIT_ERR_LACKOFMEM); //PostCode (0xE84)
    return Status;
  }
  mHeciContext->HeciCtlr.ResetHeci    = ResetHeciInterface;
  mHeciContext->HeciCtlr.SendwAck     = HeciSendwAck;
  mHeciContext->HeciCtlr.ReadMsg      = HeciReceive;
  mHeciContext->HeciCtlr.SendMsg      = HeciSend;
  mHeciContext->HeciCtlr.InitHeci     = HeciInitialize;
  mHeciContext->HeciCtlr.ReInitHeci   = HeciReInitialize;
  mHeciContext->HeciCtlr.MeResetWait  = MeResetWait;
  mHeciContext->HeciCtlr.GetMeStatus  = HeciGetMeStatus;
  mHeciContext->HeciCtlr.GetMeMode    = HeciGetMeMode;
  mHeciContext->HeciCtlr.HeciAsyncSend = HeciAsyncSend;
  InitializeListHead (&mHeciContext->Queue);

  ///
  /// Install the HECI interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mHeciContext->Handle,
                  &gHeciProtocolGuid,
                  &mHeciContext->HeciCtlr,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI Protocol Exit - Error by install HECI protocol fail, Status: %r\n", Status));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_EXIT_ERR_INSTALPROTOFAIL); //PostCode (0xEC4)
    return Status;
  }

  //
  // Create the asynchronous interrupt monitor
  //
  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  ProcessAsyncTaskList,
                  NULL,
                  &mHeciContext->AsyncMsgTimer
                  );

  HeciGetMeStatus (&MeStatus);
  HeciGetMeMode (&MeMode);

  InitializeHeciFlow ();

  //
  // Register End of DXE event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MeEndOfDxeEvent,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             MeReadyToBootEvent,
             NULL,
             &Event
             );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "ME-BIOS: HECI Protocol Exit - Success.\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_INIT_EXIT); //PostCode (0xE24)

  return Status;
}
