/** @file
  Header file for Console Platfrom DXE Driver.

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

#ifndef _CON_PLATFORM_H_
#define _CON_PLATFORM_H_

#include <Uefi.h>

#include <Protocol/DevicePath.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/BluetoothHc.h>
#include <Guid/ConsoleInDevice.h>

#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>

//
// Driver Binding Externs
//
extern EFI_DRIVER_BINDING_PROTOCOL   gBleConPlatformTextInDriverBinding;

/**
  Check to see if specific protocol could be supported on the ControllerHandle.

  @param  This                Protocol instance pointer.
  @param  ControllerHandle    Handle of device to check.
  @param  ProtocolGuid        The specfic protocol.

  @retval EFI_SUCCESS         This driver supports this device
  @retval other               This driver does not support this device

**/
EFI_STATUS
BleConPlatformDriverBindingSupported (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   ControllerHandle,
  IN  EFI_GUID                     *ProtocolGuid
  );

/**
  Check to see if EFI_SIMPLE_TEXT_INPUT_PROTOCOL is supported on ControllerHandle.

  @param  This                Protocol instance pointer.
  @param  ControllerHandle    Handle of device to check.
  @param  RemainingDevicePath Optional parameter to pick a specific child
                              device to start.

  @retval EFI_SUCCESS         This driver supports this device.
  @retval other               This driver does not support this device.

**/
EFI_STATUS
EFIAPI
BleConPlatformTextInDriverBindingSupported (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   Handle,
  IN  EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath  OPTIONAL
  );

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
  IN  EFI_HANDLE                   Handle,
  IN  EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

/**
  Stop this driver on ControllerHandle by removing Console In Devcice GUID
  and closing the Simple Text Input protocol on ControllerHandle.

  @param  This              Protocol instance pointer.
  @param  ControllerHandle  Handle of device to stop driver
  @param  NumberOfChildren  Number of Handles in ChildHandleBuffer. If number of
                            children is zero stop the entire bus driver.
  @param  ChildHandleBuffer List of Child Handles to Stop.

  @retval EFI_SUCCESS       This driver is removed ControllerHandle
  @retval other             This driver was not removed from this device

**/
EFI_STATUS
EFIAPI
BleConPlatformTextInDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   Handle,
  IN  UINTN                        NumberOfChildren,
  IN  EFI_HANDLE                   *ChildHandleBuffer
  );

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
  );

#endif
