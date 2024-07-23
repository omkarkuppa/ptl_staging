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
#include "TestCaseI2cPlatformDriverBindingStart.h"
#include <Protocol/I2cBusConfigurationManagement.h>
#include <I2cPlatformDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include "PrivateMock/PrivateMock.h"
#include <BootServicesMock.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/DriverBinding.h>
#include <Library/BaseMemoryLib.h>
#include "TestCase/GetSocSeries/TestCaseGetSocSeries.h"

/**
  Starts a device controller or a bus controller.

  The Start() function is designed to be invoked from the EFI boot service ConnectController().
  As a result, much of the error checking on the parameters to Start() has been moved into this
  common boot service. It is legal to call Start() from other locations,
  but the following calling restrictions must be followed or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE.
  2. If RemainingDevicePath is not NULL, then it must be a pointer to a naturally aligned
     EFI_DEVICE_PATH_PROTOCOL.
  3. Prior to calling Start(), the Supported() function for the driver specified by This must
     have been called with the same calling parameters, and Supported() must have returned EFI_SUCCESS.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                   parameter is ignored by device drivers, and is optional for bus
                                   drivers. For a bus driver, if this parameter is NULL, then handles
                                   for all the children of Controller are created by this driver.
                                   If this parameter is not NULL and the first Device Path Node is
                                   not the End of Device Path Node, then only the handle for the
                                   child device specified by the first Device Path Node of
                                   RemainingDevicePath is created by this driver.
                                   If the first Device Path Node of RemainingDevicePath is
                                   the End of Device Path Node, no child handle is created by this
                                   driver.

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failed to start the device.

**/
extern
EFI_STATUS
EFIAPI
I2cPlatformDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Mock I2cPlatformDriverBindingStart

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
UnitTestI2cPlatformDriverBindingStart (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                                         Status;
  EFI_DRIVER_BINDING_PROTOCOL                        DriverBinding;
  SERIAL_IO_I2C_DEVID                                *SerialIoI2cDevid;
  UINT8                                              *I2cDevidTable;
  UINT8                                              *I2cDevidList;
  EFI_PCI_IO_PROTOCOL                                *PciIo;
  UINT8                                              Index;
  PROTOCOLS_LIST                                     *ProtocolList;
  UINT8                                              Counter;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  I2cDevidList =  AllocateZeroPool (sizeof (UINT8));
  ProtocolList = NULL;
  //
  // Mock boot service OpenProtocol and CloseProtocol
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocol;
  gBS->InstallMultipleProtocolInterfaces = MockBootServiceInstallMultipleProtocolInterfaces;
  //
  // Mock Pci.Read
  //
  PciIo = AllocateZeroPool (sizeof (EFI_PCI_IO_PROTOCOL));
  PciIo->Pci.Read = MockPciIoConfigRead;
  //
  // Mock SERIAL_IO_I2C_DEVID structure.
  //
  SerialIoI2cDevid = AllocateZeroPool (sizeof (SERIAL_IO_I2C_DEVID));
  SerialIoI2cDevid->Count = MAX_I2C_CONTROLLERS_NUM;

  for (Index = 0; Index < SerialIoI2cDevid->Count; Index++) {
    SerialIoI2cDevid->SerialIoI2cDevId[Index] = GetLpssI2cDeviceId (Index);
  }
  I2cDevidTable = (UINT8 *) SerialIoI2cDevid;

  //
  // Test1: Mock incorrect I2C Device Id :0xFFFF for test.
  // Expected Result: EFI_UNSUPPORTED.
  //
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiPciIoProtocolGuid);                                 // Assume UnitTestI2cPlatformDriverBindingStart will locate gEfiPciIoProtocolGuid
    will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                                                       // Mock return Status = EFI_SUCCESS
    will_return (MockBootServiceOpenProtocol, PciIo);
  //
  // Mock MockPciIoConfigRead output parameter Value
  //
  expect_value (MockPciIoConfigRead, Offset, PCI_VENDOR_ID_OFFSET);
  will_return (MockPciIoConfigRead, SERIAL_IO_ERROR_DEVID);                                                      // Mock incorrect I2C Device Id :0xFFFF
  will_return (MockPciIoConfigRead, EFI_SUCCESS);                                                                // Mock return Status = EFI_SUCCESS
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                                                       // Mock return Status = EFI_SUCCESS

  // Mock PcdSerialIoI2cDevId
  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdSerialIoI2cDevId);
  will_return (LibPcdGetExPtr, I2cDevidTable);

  Status = I2cPlatformDriverBindingStart (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);                                                                    // Expected Result: EFI_UNSUPPORTED.

  //
  // Test2: Mock correct I2C Device Id and check protocols.
  // Expected Result: EFI_SUCCESS.
  //
  for (Index = 0; Index < MAX_I2C_CONTROLLERS_NUM; Index++) {
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiPciIoProtocolGuid);                                 // Assume UnitTestI2cPlatformDriverBindingStart will locate gEfiPciIoProtocolGuid
    will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                                                       // Mock return Status = EFI_SUCCESS
    will_return (MockBootServiceOpenProtocol, PciIo);

    //
    // Mock MockPciIoConfigRead output parameter Value
    //
    expect_value (MockPciIoConfigRead, Offset, PCI_VENDOR_ID_OFFSET);
    will_return (MockPciIoConfigRead, SerialIoI2cDevid->SerialIoI2cDevId[Index]);
    will_return (MockPciIoConfigRead, EFI_SUCCESS);

    will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                                                       // Mock return Status = EFI_SUCCESS

    // Mock PcdSerialIoI2cDevId
    expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdSerialIoI2cDevId);
    will_return (LibPcdGetExPtr, I2cDevidTable);

    // Mock PcdI2cControllerDevList
    expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdI2cControllerDevList);
    will_return (LibPcdGetExPtr, I2cDevidList);
    //
    // Mock InstallMultipleProtocolInterfaces. All protocols will store in ProtocolList.
    // Then check protocols that we expected by CompareGuid().
    //
    will_return (MockBootServiceInstallMultipleProtocolInterfaces, &ProtocolList);
    will_return (MockBootServiceInstallMultipleProtocolInterfaces, &Counter);
    will_return (MockBootServiceInstallMultipleProtocolInterfaces, EFI_SUCCESS);

    Status = I2cPlatformDriverBindingStart (&DriverBinding, NULL, NULL);

    if (((CompareGuid (ProtocolList[0].Protocol, &gEfiI2cEnumerateProtocolGuid)) ||                               // Check ProtocolList include gEfiI2cEnumerateProtocolGuid and gEfiI2cBusConfigurationManagementProtocolGuid.
         (CompareGuid (ProtocolList[1].Protocol, &gEfiI2cEnumerateProtocolGuid))) &&
        ((CompareGuid (ProtocolList[0].Protocol, &gEfiI2cBusConfigurationManagementProtocolGuid)) ||
         (CompareGuid (ProtocolList[1].Protocol, &gEfiI2cBusConfigurationManagementProtocolGuid)))) {
      UT_ASSERT_EQUAL (EFI_SUCCESS, EFI_SUCCESS);
    } else {
      // If protocols not we expected.
      UT_ASSERT_EQUAL (EFI_DEVICE_ERROR, EFI_SUCCESS);
    }
  }
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  FreePool (I2cDevidList);
  FreePool (ProtocolList);
  FreePool (gBS);
  FreePool (PciIo);
  FreePool (SerialIoI2cDevid);
  return UNIT_TEST_PASSED;
}