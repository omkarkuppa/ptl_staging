/** @file
  Serial Io Uart Driver Component Name

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "SerialIoUartDriver.h"

extern EFI_DRIVER_BINDING_PROTOCOL  mSerialIoUartDriverBinding;

EFI_STATUS
EFIAPI
SerialIoUartComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );

EFI_STATUS
EFIAPI
SerialIoUartComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL   *This,
  IN  EFI_HANDLE                    ControllerHandle,
  IN  EFI_HANDLE                    ChildHandle OPTIONAL,
  IN  CHAR8                         *Language,
  OUT CHAR16                        **ControllerName
  );


GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME_PROTOCOL     mSerialIoUartDriverComponentName = {
  SerialIoUartComponentNameGetDriverName,
  SerialIoUartComponentNameGetControllerName,
  "eng"
};

static EFI_UNICODE_STRING_TABLE mSerialIoUartDriverNameTable[] = {
  {
    "eng",
    L"Serial Io Uart Driver"
  },
  {
    NULL,
    NULL
  }
};

/**
  Retrieves a Unicode string that is the user readable name of the EFI Driver.

  @param[in] This                 A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param[in] Language             A pointer to a three character ISO 639-2 language identifier.
                                  This is the language of the driver name that that the caller
                                  is requesting, and it must match one of the languages specified
                                  in SupportedLanguages. The number of languages supported by a
                                  driver is up to the driver writer.
  @param[in] DriverName           A pointer to the Unicode string to return.  This Unicode string
                                  is the name of the driver specified by This in the language
                                  specified by Language.

  @retval EFI_SUCCESS             The Unicode string for the Driver specified by This
                                  and the language specified by Language was returned in DriverName.
  @retval EFI_INVALID_PARAMETER   Language or DriverName is NULL.
  @exception EFI_UNSUPPORTED      The driver specified by This does not support the
                                  language specified by Language.
**/
EFI_STATUS
EFIAPI
SerialIoUartComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  )
{
  return LookupUnicodeString (
           Language,
           mSerialIoUartDriverComponentName.SupportedLanguages,
           mSerialIoUartDriverNameTable,
           DriverName
           );
}

/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by an EFI Driver.

  @param[in] This                 A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param[in] ControllerHandle     The handle of a controller that the driver specified by
                                  This is managing.  This handle specifies the controller
                                  whose name is to be returned.
  @param[in] ChildHandle          The handle of the child controller to retrieve the name
                                  of. This is an optional parameter that may be NULL.  It
                                  will be NULL for device drivers.  It will also be NULL
                                  for a bus drivers that wish to retrieve the name of the
                                  bus controller. It will not be NULL for a bus driver
                                  that wishes to retrieve the name of a child controller.
  @param[in] Language             A pointer to a three character ISO 639-2 language
                                  identifier. This is the language of the controller name
                                  that that the caller is requesting, and it must match one
                                  of the languages specified in SupportedLanguages.  The
                                  number of languages supported by a driver is up to the
                                  driver writer.
  @param[in] ControllerName       A pointer to the Unicode string to return.  This Unicode
                                  string is the name of the controller specified by
                                  ControllerHandle and ChildHandle in the language
                                  specified by Language from the point of view of the
                                  driver specified by This.

  @retval EFI_SUCCESS             The Unicode string for the user readable name in the
                                  language specified by Language for the driver
                                  specified by This was returned in DriverName.
  @retval EFI_INVALID_PARAMETER   ControllerHandle is not a valid EFI_HANDLE. Language or
                                  ControllerName is NULL.
  @exception EFI_UNSUPPORTED      The driver specified by This is not currently
                                  managing the controller specified by
                                  ControllerHandle and ChildHandle. The driver specified by This
                                  does not support the language specified by Language.
**/
EFI_STATUS
EFIAPI
SerialIoUartComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL   *This,
  IN  EFI_HANDLE                    ControllerHandle,
  IN  EFI_HANDLE                    ChildHandle OPTIONAL,
  IN  CHAR8                         *Language,
  OUT CHAR16                        **ControllerName
  )
{
  EFI_STATUS              Status;
  EFI_SERIAL_IO_PROTOCOL  *SerialIoProtcol;
  SERIAL_IO_UART_DEV      *UartDevice;

  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiSerialIoProtocolGuid,
                  (VOID **) &SerialIoProtcol,
                  mSerialIoUartDriverBinding.DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UartDevice = SERIAL_IO_UART_DEV_FROM_THIS (SerialIoProtcol);

  return LookupUnicodeString (
           Language,
           mSerialIoUartDriverComponentName.SupportedLanguages,
           UartDevice->ControllerNameTable,
           ControllerName
           );
}
