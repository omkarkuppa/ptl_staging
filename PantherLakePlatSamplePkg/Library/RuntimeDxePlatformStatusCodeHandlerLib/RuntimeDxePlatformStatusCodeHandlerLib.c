/** @file
  Platform status code implementation.

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

@par Specification
**/

#include "RuntimeDxePlatformStatusCodeHandlerLib.h"
#include <Library/FspErrorInfoLib.h>

EFI_RSC_HANDLER_PROTOCOL  *mEfiRscHandlerProtocol       = NULL;

/**
  ReportStatusCode worker for FSP Error Information.

  @param  CodeType         Always (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED)
  @param  Value            Always 0
  @param  Instance         Always 0
  @param  CallerId         This optional parameter may be used to identify the caller.
                           It may be used to identify which internal component of the FSP
                           was executing at the time of the error.
  @param  Data             This data contains FSP error type and status code.

  @retval EFI_SUCCESS      Show error status sent by FSP successfully.
  @retval RETURN_ABORTED   Function skipped as unrelated.

**/
EFI_STATUS
EFIAPI
FspErrorStatusCodeReportWorkerDxe (
  IN EFI_STATUS_CODE_TYPE       CodeType,
  IN EFI_STATUS_CODE_VALUE      Value,
  IN UINT32                     Instance,
  IN EFI_GUID                   *CallerId,
  IN EFI_STATUS_CODE_DATA       *Data OPTIONAL
  )
{
  EFI_STATUS Ret;

  Ret = FspErrorStatusCodeReportWorker (CodeType, Value, Instance, (CONST EFI_GUID *) CallerId, (CONST EFI_STATUS_CODE_DATA *) Data);

  //
  // If register an unregister function of gEfiEventExitBootServicesGuid,
  // then some log called in ExitBootServices() will be lost,
  // so unregister the handler after receive the value of exit boot service.
  //
  if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE &&
      Value == (EFI_SOFTWARE_EFI_BOOT_SERVICE | EFI_SW_BS_PC_EXIT_BOOT_SERVICES)) {
    if (PcdGet8 (PcdFspModeSelection) == 0) {
      mEfiRscHandlerProtocol->Unregister (FspErrorStatusCodeReportWorkerDxe);
    }
  }

  return Ret;
}

/**
  Unregister status code callback functions only available at boot time from
  report status code router when exiting boot services.

**/
VOID
EFIAPI
UnregisterUsb3BootTimeHandler (
  VOID
  )
{
  if (PcdGetBool (PcdStatusCodeUseUsb3)) {
    mEfiRscHandlerProtocol->Unregister (Usb3StatusCodeReportWorker);
  }
}

/**
  Unregister SerialIoUart status code handler when exiting boot services.
  
  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context.
**/
VOID
EFIAPI
UnregisterSerialIoUartBootTimeHandler (
  VOID
  )
{
  if (PcdGetBool (PcdStatusCodeUseSerialIoUart) ) {
    if (mEfiRscHandlerProtocol != NULL) {
      mEfiRscHandlerProtocol->Unregister (SerialIoUartStatusCodeReportWorker);
    }
  }
}

/**
  Unregister SerialIoUart status code handler when exiting boot services.
  
  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context.
**/
VOID
EFIAPI
UnregisterSerialIoUartOnExitBootServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  if (PcdGetBool (PcdStatusCodeUseSerialIoUart) ) {
    if (mEfiRscHandlerProtocol != NULL) {
      mEfiRscHandlerProtocol->Unregister (SerialIoUartStatusCodeReportWorker);
    }
  }

  // Close the event
  gBS->CloseEvent (Event);
}

/**
  Register status code callback function only when Report Status Code protocol
  is installed.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
RegisterBootTimeHandlers (
  IN EFI_EVENT        Event,
  IN VOID             *Context
)
{
  EFI_STATUS                 Status;
  UINT8                      StatusCodeFlags;

  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mEfiRscHandlerProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  StatusCodeFlags = GetDebugInterface ();

  if (PcdGetBool (PcdStatusCodeUseUsb3)) {
    if (StatusCodeFlags & STATUS_CODE_USE_USB3) {
      Status = Usb3DebugPortInitialize ();
      ASSERT_EFI_ERROR (Status);
    }
    mEfiRscHandlerProtocol->Register (Usb3StatusCodeReportWorker, TPL_HIGH_LEVEL);
  }

  if (PcdGet8 (PcdFspModeSelection) == 0) {
    mEfiRscHandlerProtocol->Register (FspErrorStatusCodeReportWorkerDxe, TPL_HIGH_LEVEL);
  }

  if (PcdGetBool (PcdStatusCodeUseSerialIoUart) ) {
    if (StatusCodeFlags & STATUS_CODE_USE_SERIALIO) {
      SerialIoUartDebugInit ();
    }
    mEfiRscHandlerProtocol->Register (SerialIoUartStatusCodeReportWorker, TPL_HIGH_LEVEL);
  }

}

/**
  Entry point of DXE Status Code Driver.

  This function is the entry point of this DXE Status Code Driver.
  It initializes registers status code handlers, and registers event for EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

  @param  ImageHandle       The firmware allocated handle for the EFI image.
  @param  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
StatusCodeHandlerRuntimeDxeConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_EVENT                 RegisterStatusCodeHandlerEvent;
  VOID                      *Registration;

  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mEfiRscHandlerProtocol
                  );

  if (!EFI_ERROR (Status)) {
    RegisterBootTimeHandlers (NULL, NULL);
  } else {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    RegisterBootTimeHandlers,
                    NULL,
                    &RegisterStatusCodeHandlerEvent
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Register for protocol notifications on this event
    //
    Status = gBS->RegisterProtocolNotify (
                    &gEfiRscHandlerProtocolGuid,
                    RegisterStatusCodeHandlerEvent,
                    &Registration
                    );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
