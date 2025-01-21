/** @file
  This file includes console platform DXE driver install console device GUIDs and update console
  environment variables.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@Par Specification Reference:
**/

#include "BleConPlatform.h"

EFI_DRIVER_BINDING_PROTOCOL  gBleConPlatformTextInDriverBinding = {
  BleConPlatformTextInDriverBindingSupported,
  BleConPlatformTextInDriverBindingStart,
  BleConPlatformTextInDriverBindingStop,
  0xa,
  NULL,
  NULL
};

/**
  Entrypoint of this module.

  This function is the entrypoint of this module. It installs Driver Binding
  Protocols together with Component Name Protocols.

  @param  ImageHandle       The firmware allocated handle for the EFI image.
  @param  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             This driver does not support this device.

**/
EFI_STATUS
EFIAPI
InitializeBleConPlatform (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  DEBUG ((EFI_D_INFO, "InitializeBleConPlatform\n"));

  Status = EfiLibInstallDriverBinding (
             ImageHandle,
             SystemTable,
             &gBleConPlatformTextInDriverBinding,
             ImageHandle
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Status:%r\n", Status));
  }
  return Status;
}

/**
  Check to see if EFI_SIMPLE_TEXT_INPUT_PROTOCOL is supported on ControllerHandle.

  @param  This                Protocol instance pointer.
  @param  ControllerHandle    Handle of device to check.
  @param  RemainingDevicePath Optional parameter use to pick a specific child
                              device to start.

  @retval EFI_SUCCESS         This driver supports this device.
  @retval other               This driver does not support this device.

**/
EFI_STATUS
EFIAPI
BleConPlatformTextInDriverBindingSupported (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   ControllerHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath  OPTIONAL
  )
{
  EFI_STATUS Status;
  Status = BleConPlatformDriverBindingSupported (
              This,
              ControllerHandle,
              &gEfiSimpleTextInProtocolGuid
              );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Status:%r\n", Status));
  }
  return Status;
}

/**
  Check to see if the specified protocol is supported on ControllerHandle.

  @param  This                Protocol instance pointer.
  @param  ControllerHandle    Handle of device to check.
  @param  ProtocolGuid        The specfic protocol.

  @retval EFI_SUCCESS         This driver supports this device.
  @retval other               This driver does not support this device.

**/
EFI_STATUS
BleConPlatformDriverBindingSupported (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   ControllerHandle,
  IN  EFI_GUID                     *ProtocolGuid
  )
{
  EFI_STATUS  Status;
  VOID        *Interface;

  Interface = NULL;
  //
  // Check to see if this is a physical device by checking if it has a Device Path Protocol.
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiDevicePathProtocolGuid,
                  NULL,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Status:%r\n", Status));
    return Status;
  }

  //
  // Check to see if this device supports the specified Protocol.
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  ProtocolGuid,
                  (VOID **) &Interface,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Status:%r\n", Status));
    return Status;
  }

  Status = gBS->CloseProtocol (
                  ControllerHandle,
                  ProtocolGuid,
                  This->DriverBindingHandle,
                  ControllerHandle
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Status:%r\n", Status));
  }
  return Status;
}

/**
  Start this driver on the device for console input.

  Start this driver on ControllerHandle by opening Simple Text Input Protocol,
  reading Device Path, and installing Console In Devcice GUID on ControllerHandle.

  Append its device path into the console environment variables ConInDev.

  @param  This                 Protocol instance pointer.
  @param  ControllerHandle     Handle of device to bind driver
  @param  RemainingDevicePath  Optional parameter use to pick a specific child
                               device to start.

  @retval EFI_SUCCESS          This driver is added to ControllerHandle
  @retval EFI_ALREADY_STARTED  This driver is already running on ControllerHandle
  @retval other                This driver does not support this device.

**/
EFI_STATUS
EFIAPI
BleConPlatformTextInDriverBindingStart (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   ControllerHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  EFI_STATUS                      Status;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *TextIn;
  EFI_HANDLE                      BluetoothHcHandle;

  DEBUG ((EFI_D_INFO, "BleConPlatformTextInDriverBindingStart\n"));
  //
  // Get the Device Path Protocol to let the environment variables can be updated
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DevicePath,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Unable to open gEfiDevicePathProtocolGuid, Status:%r\n", Status));
    return Status;
  }

  //
  // Open the Simple Text Input Protocol BY_DRIVER
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiSimpleTextInProtocolGuid,
                  (VOID **) &TextIn,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
     DEBUG ((EFI_D_ERROR, "Unable to open gEfiSimpleTextInProtocolGuid, Status:%r\n", Status));
     return Status;
  }
  //
  // Locate the BleHc protocol from the device path protocol
  //
  Status = gBS->LocateDevicePath (&gEfiBluetoothHcProtocolGuid, &DevicePath, &BluetoothHcHandle);
  if (!EFI_ERROR (Status)) {
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &ControllerHandle,
                    &gEfiConsoleInDeviceGuid,
                    NULL,
                    NULL
                    );
  } else {
    Status = gBS->CloseProtocol (
                    ControllerHandle,
                    &gEfiSimpleTextInProtocolGuid,
                    This->DriverBindingHandle,
                    ControllerHandle
                    );
  }
  DEBUG ((EFI_D_INFO, "BleConPlatformTextInDriverBindingStart:Exit Status:%r\n", Status));
  return Status;
}

/**
  Stop this driver on ControllerHandle by removing Console In Devcice GUID
  and closing the Simple Text Input protocol on ControllerHandle.

  @param  This              Protocol instance pointer.
  @param  ControllerHandle  Handle of device to stop driver
  @param  NumberOfChildren  Number of Handles in ChildHandleBuffer. If number of
                            children is zero it will stop the entire bus driver.
  @param  ChildHandleBuffer List of Child Handles to Stop.

  @retval EFI_SUCCESS       This driver is removed ControllerHandle
  @retval other             This driver was not removed from this device

**/
EFI_STATUS
EFIAPI
BleConPlatformTextInDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   ControllerHandle,
  IN  UINTN                        NumberOfChildren,
  IN  EFI_HANDLE                   *ChildHandleBuffer
  )
{
  EFI_STATUS Status;
  DEBUG ((EFI_D_INFO, "BleConPlatformTextInDriverBindingStop\n"));
  //
  // Uninstall the Console Device GUIDs from Controller Handle
  //
  BleConPlatformUnInstallProtocol (
    This,
    ControllerHandle,
    &gEfiConsoleInDeviceGuid
    );

  //
  // Close the Simple Text Input Protocol
  //
  Status = gBS->CloseProtocol (
                  ControllerHandle,
                  &gEfiSimpleTextInProtocolGuid,
                  This->DriverBindingHandle,
                  ControllerHandle
                  );
  if (EFI_ERROR (Status)) {
     DEBUG ((EFI_D_ERROR, "Status:%r\n", Status));
  }
  return Status;
}

/**
  Uninstall the specified protocol.

  @param This            Protocol instance pointer.
  @param Handle          Handle of device to uninstall protocol.
  @param ProtocolGuid    The specified protocol need to be uninstalled.

**/
VOID
BleConPlatformUnInstallProtocol (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   Handle,
  IN  EFI_GUID                     *ProtocolGuid
  )
{
  EFI_STATUS  Status;
  DEBUG ((EFI_D_INFO, "BleConPlatformUnInstallProtocol\n"));
  Status = gBS->OpenProtocol (
                  Handle,
                  ProtocolGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Handle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Status = gBS->UninstallMultipleProtocolInterfaces (
                    Handle,
                    ProtocolGuid,
                    NULL,
                    NULL
                    );
  }
  DEBUG ((EFI_D_INFO, "Status:%r\n", Status));
  return;
}
