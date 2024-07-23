/**@file
    Unit test case of the I2c TouchPanel Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "TestCaseTouchPanelDriverDriverBindingSupported.h"
#include <I2cTouchPanel.h>
#include <Library/UefiBootServicesTableLib.h>
#include <BootServicesMock.h>

/**
  Looks for controllers which declare the EFI_I2C_IO_PROTOCOL and match the device path
  supplied by the silicon vendor or third party I2C driver writer to the platform integrator.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to test. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                   parameter is ignored by device drivers, and is optional for bus
                                   drivers. For bus drivers, if this parameter is not NULL, then
                                   the bus driver must determine if the bus controller specified
                                   by ControllerHandle and the child controller specified
                                   by RemainingDevicePath are both supported by this
                                   bus driver.

  @retval EFI_SUCCESS              The device specified by ControllerHandle and
                                   RemainingDevicePath is supported by the driver specified by This.
  @retval EFI_ALREADY_STARTED      The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by the driver
                                   specified by This.
  @retval EFI_ACCESS_DENIED        The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by a different
                                   driver or an application that requires exclusive access.
                                   Currently not implemented.
  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle and
                                   RemainingDevicePath is not supported by the driver specified by This.
**/
extern
EFI_STATUS
EFIAPI
TouchPanelDriverDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Mock of TouchPanelDriverDriverBindingSupported

  @param[in]  Context                   [Optional] An optional parameter that enables:
                                        1) test-case reuse with varied parameters and
                                        2) test-case re-entry for Target tests that need a
                                        reboot.  This parameter is a VOID* and it is the
                                        responsibility of the test author to ensure that the
                                        contents are well understood by all test cases that may
                                        consume it.
  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestTouchPanelDriverDriverBindingSupported (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_I2C_IO_PROTOCOL              *I2cIoProtocol;
  EFI_STATUS                       Status;
  EFI_DRIVER_BINDING_PROTOCOL      DriverBinding;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Mock Boot service and I2cIoProtocol service.
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocol;
  I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  //
  // Test function TouchPanelDriverDriverBindingSupported with incorrect ControllerHandle.
  //
  // Expect Result:
  //   - EFI_UNSUPPORTED: Handle does not support Protocol with OpenProtocol.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cIoProtocolGuid);         // Assume TouchPanelDriverDriverBindingSupported will locate gEfiI2cIoProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_UNSUPPORTED);                           // Mock return Status = EFI_UNSUPPORTED
  will_return (MockBootServiceOpenProtocol, I2cIoProtocol);

  Status = TouchPanelDriverDriverBindingSupported  (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  //
  // Test function TouchPanelDriverDriverBindingSupported with correct ControllerHandle but GUID pointed to by the DeviceGuid field not match.
  //
  // Expect Result:
  //   - EFI_UNSUPPORTED: GUID pointed to by the DeviceGuid field not match.
  //
  I2cIoProtocol->DeviceGuid = &gEfiI2cIoProtocolGuid;                                   // DeviceGuid with incorrect Guid.
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cIoProtocolGuid);         // Assume TouchPanelDriverDriverBindingSupported will locate gEfiI2cIoProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                               // Mock return Status = EFI_SUCCESS
  will_return (MockBootServiceOpenProtocol, I2cIoProtocol);
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                              // Mock return Status = EFI_SUCCESS

  Status = TouchPanelDriverDriverBindingSupported  (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  //
  // Test function TouchPanelDriverDriverBindingSupported with correct ControllerHandle and GUID pointed to by the DeviceGuid field match.
  //
  // Expect Result:
  //   - EFI_SUCCESS.
  //
  I2cIoProtocol->DeviceGuid = &gI2cTouchPanelDeviceGuid;                                   // DeviceGuid with correct Guid.
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cIoProtocolGuid);            // Assume TouchPanelDriverDriverBindingSupported will locate gEfiI2cIoProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                                  // Mock return Status = EFI_SUCCESS
  will_return (MockBootServiceOpenProtocol, I2cIoProtocol);
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                                 // Mock return Status = EFI_SUCCESS

  Status = TouchPanelDriverDriverBindingSupported  (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  //
  // Destroy Mock Service
  //
  FreePool (gBS);
  FreePool (I2cIoProtocol);
  return UNIT_TEST_PASSED;
}