/**@file
    Unit test case of the I2c Platform Dxe driver.

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
#include "TestCaseI2cPlatformDriverBindingSupported.h"
#include <Protocol/I2cBusConfigurationManagement.h>
#include <I2cPlatformDxe.h>
#include <Library/DebugLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/DriverBinding.h>
#include <Library/MemoryAllocationLib.h>
#include "PrivateMock/PrivateMock.h"
#include <BootServicesMock.h>
#include <Library/UefiBootServicesTableLib.h>
#include "TestCase/GetSocSeries/TestCaseGetSocSeries.h"


/**
  I2cPlatformDriverBindingSupported - Checks if this driver can install on device pointed to by Handle
  It will only install on I2c controllers

  Tests to see if this driver supports a given controller. If a child device is provided,
  it further tests to see if this driver supports creating a handle for the specified child device.

  This function checks to see if the driver specified by This supports the device specified by
  ControllerHandle. Drivers will typically use the device path attached to
  ControllerHandle and/or the services from the bus I/O abstraction attached to
  ControllerHandle to determine if the driver supports ControllerHandle. This function
  may be called many times during platform initialization. In order to reduce boot times, the tests
  performed by this function must be very small, and take as little time as possible to execute. This
  function must not change the state of any hardware devices, and this function must be aware that the
  device specified by ControllerHandle may already be managed by the same driver or a
  different driver. This function must match its calls to AllocatePages() with FreePages(),
  AllocatePool() with FreePool(), and OpenProtocol() with CloseProtocol().
  Since ControllerHandle may have been previously started by the same driver, if a protocol is
  already in the opened state, then it must not be closed with CloseProtocol(). This is required
  to guarantee the state of ControllerHandle is not modified by this function.

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
I2cPlatformDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Mock I2cPlatformDriverBindingSupported

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
UnitTestI2cPlatformDriverBindingSupported (
  IN UNIT_TEST_CONTEXT  Context
  )
{

  EFI_STATUS                                         Status;
  EFI_DRIVER_BINDING_PROTOCOL                        DriverBinding;
  EFI_PCI_IO_PROTOCOL                                *PciIo;
  SERIAL_IO_I2C_DEVID                                *SerialIoI2cDevid;
  UINT8                                              *I2cDevidTable;
  UINT8                                              Index;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Mock boot service OpenProtocol and CloseProtocol
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocol;

  //
  // Mock Pci.Read
  //
  PciIo = AllocateZeroPool (sizeof (EFI_PCI_IO_PROTOCOL));
  PciIo->Pci.Read = MockPciIoConfigRead;

  //
  // Test1: Mock OpenProtocol gEfiI2cBusConfigurationManagementProtocolGuid,
  //       If the device specified by ControllerHandle and RemainingDevicePath
  //       is already being managed by the driver specified by This.
  // Expected Result: EFI_ALREADY_STARTED.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cBusConfigurationManagementProtocolGuid);         // Assume UnitTestI2cPlatformDriverBindingSupported will locate gEfiI2cBusConfigurationManagementProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                                                       // Mock return Status = EFI_SUCCESS

  Status = I2cPlatformDriverBindingSupported (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_ALREADY_STARTED);

  //
  // Test2-1:
  //       1.Mock OpenProtocol gEfiI2cBusConfigurationManagementProtocolGuid,
  //         If the device specified by ControllerHandle and RemainingDevicePath
  //         is not supported by the driver specified by This.
  //       2.Mock correct I2C Device Ids for test.
  // Expected Result: EFI_SUCCESS.
  //

  //
  // Mock SERIAL_IO_I2C_DEVID structure.
  //
  SerialIoI2cDevid = AllocateZeroPool (sizeof (SERIAL_IO_I2C_DEVID));
  SerialIoI2cDevid->Count = MAX_I2C_CONTROLLERS_NUM;
  for (Index = 0; Index < SerialIoI2cDevid->Count; Index++) {
    SerialIoI2cDevid->SerialIoI2cDevId[Index] = GetLpssI2cDeviceId (Index);
  }
  I2cDevidTable = (UINT8 *) SerialIoI2cDevid;
  for (Index = 0; Index < MAX_I2C_CONTROLLERS_NUM; Index++) {
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cBusConfigurationManagementProtocolGuid);         // Assume UnitTestI2cPlatformDriverBindingSupported will locate gEfiI2cBusConfigurationManagementProtocolGuid
    will_return (MockBootServiceOpenProtocol, EFI_INVALID_PARAMETER);                                             // Mock return Status = EFI_SUCCESS

    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiPciIoProtocolGuid);                                 // Assume UnitTestI2cPlatformDriverBindingSupported will locate gEfiI2cBusConfigurationManagementProtocolGuid
    will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                                                       // Mock return Status = EFI_SUCCESS
    will_return (MockBootServiceOpenProtocol, PciIo);

    //
    // Mock MockPciIoConfigRead output parameter Value
    //
    expect_value (MockPciIoConfigRead, Offset, PCI_VENDOR_ID_OFFSET);
    will_return (MockPciIoConfigRead, SerialIoI2cDevid->SerialIoI2cDevId[Index]);
    will_return (MockPciIoConfigRead, EFI_SUCCESS);

    will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                                                      // Mock return Status = EFI_SUCCESS
    //Mock PcdSerialIoI2cDevId
    expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdSerialIoI2cDevId);
    will_return (LibPcdGetExPtr, I2cDevidTable);
    Status = I2cPlatformDriverBindingSupported (&DriverBinding, NULL, NULL);
    UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  }
  //
  // Test2-2:
  //       1.Mock OpenProtocol gEfiI2cBusConfigurationManagementProtocolGuid,
  //         If the device specified by ControllerHandle and RemainingDevicePath
  //         is not supported by the driver specified by This.
  //       2.Mock incorrect I2C Device Id :0xFFFF for test.
  // Expected Result: EFI_UNSUPPORTED.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cBusConfigurationManagementProtocolGuid);         // Assume UnitTestI2cPlatformDriverBindingSupported will locate gEfiI2cBusConfigurationManagementProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_INVALID_PARAMETER);                                                       // Mock return Status = EFI_SUCCESS

  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiPciIoProtocolGuid);                                 // Assume UnitTestI2cPlatformDriverBindingSupported will locate gEfiI2cBusConfigurationManagementProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                                                       // Mock return Status = EFI_SUCCESS
  will_return (MockBootServiceOpenProtocol, PciIo);

  //
  // Mock MockPciIoConfigRead output parameter Value
  //
  expect_value (MockPciIoConfigRead, Offset, PCI_VENDOR_ID_OFFSET);
  will_return (MockPciIoConfigRead, SERIAL_IO_ERROR_DEVID);
  will_return (MockPciIoConfigRead, EFI_SUCCESS);

  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                                                       // Mock return Status = EFI_SUCCESS
  //Mock PcdSerialIoI2cDevId
  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdSerialIoI2cDevId);
  will_return (LibPcdGetExPtr, I2cDevidTable);
  Status = I2cPlatformDriverBindingSupported (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  FreePool(gBS);
  FreePool(PciIo);
  FreePool(SerialIoI2cDevid);
  return UNIT_TEST_PASSED;
}