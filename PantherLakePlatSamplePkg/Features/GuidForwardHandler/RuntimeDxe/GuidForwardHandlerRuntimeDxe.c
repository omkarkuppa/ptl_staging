/** @file
  Platform PPV GUID forward code implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include "GuidForwardHandlerRuntimeDxe.h"

EFI_RSC_HANDLER_PROTOCOL  *mRscHandlerProtocol       = NULL;
EFI_EVENT                 mExitBootServicesEvent     = NULL;

/**
  Unregister status code callback functions only available at boot time from
  report status code router when exiting boot services.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
UnregisterBootTimeHandlers (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  if (mRscHandlerProtocol) {
    mRscHandlerProtocol->Unregister (GuidForwardWorker);
  }

  gBS->CloseEvent (Event);
}

/**
  Entry point of DXE Guid Forward Driver.

  This function is the entry point of this DXE Status Code Driver.
  It initializes registers status code handlers, and registers event for EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
GuidForwardHandlerDxeEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;

  if (!(PcdGetBool (PcdGuidForward))) {
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mRscHandlerProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  mRscHandlerProtocol->Register (GuidForwardWorker, TPL_HIGH_LEVEL);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UnregisterBootTimeHandlers,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &mExitBootServicesEvent
                  );


  return EFI_SUCCESS;
}

