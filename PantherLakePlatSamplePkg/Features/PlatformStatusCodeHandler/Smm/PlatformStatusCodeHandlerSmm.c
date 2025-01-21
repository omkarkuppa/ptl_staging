/** @file
  Platform status code implementation.

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

@par Specification
**/

#include "PlatformStatusCodeHandlerSmm.h"

EFI_SMM_RSC_HANDLER_PROTOCOL  *mRscHandlerProtocol       = NULL;

/**
  Register status code callback function only when Report Status Code protocol
  is installed.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
EFI_STATUS
EFIAPI
RegisterBootTimeHandlers (
  IN CONST EFI_GUID        *Protocol,
  IN VOID                  *Interface,
  IN EFI_HANDLE            Handle
)
{
  EFI_STATUS                 Status;
  UINT8                      StatusCodeFlags;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &mRscHandlerProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  StatusCodeFlags = GetDebugInterface ();

  if (PcdGetBool (PcdStatusCodeUseUsb3)) {
    if (StatusCodeFlags & STATUS_CODE_USE_USB3) {
      Status = Usb3DebugPortInitialize ();
      ASSERT_EFI_ERROR (Status);
    }
    mRscHandlerProtocol->Register (Usb3StatusCodeReportWorker);
  }

  if (PcdGetBool (PcdStatusCodeUseSerialIoUart)) {
    if (StatusCodeFlags & STATUS_CODE_USE_SERIALIO) {
      SerialIoUartDebugInit ();
    }
    mRscHandlerProtocol->Register (SerialIoUartStatusCodeReportWorker);
  }

  return EFI_SUCCESS;
}

/**
  Entry point of SMM Status Code Driver.

  This function is the entry point of SMM Status Code Driver.

  @param  ImageHandle       The firmware allocated handle for the EFI image.
  @param  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
StatusCodeHandlerSmmEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                 Status;
  VOID                      *Registration;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &mRscHandlerProtocol
                    );
  if (!EFI_ERROR (Status)) {
    RegisterBootTimeHandlers (NULL, NULL, NULL);
  } else {
    gSmst->SmmRegisterProtocolNotify (
             &gEfiSmmRscHandlerProtocolGuid,
             RegisterBootTimeHandlers,
             &Registration
             );
  }

  return EFI_SUCCESS;
}
