/**@file
    Unit test case of the I2c Pss Monza Dxe driver.

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
#include <I2cPssMonza.h>
#include "TestCasePssMonzaDriverBindingStart.h"
#include <Protocol/DriverBinding.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <BootServicesMock.h>

/**
  Starts PSS Monza chip initialize by I2cIoProtocol.
  If initialization completes successfully, this driver will install gI2cPssProtocolGuid protocol
  on this device. This protocol can be further used to get serial number from PSS chip.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle
                                   must support I2C protocol interface.
  @param[in]  RemainingDevicePath  ignored

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failed to start the device.

**/
extern
EFI_STATUS
EFIAPI
PssMonzaDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Mock PssMonzaDriverBindingStart

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
UnitTestPssMonzaDriverBindingStart (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_I2C_IO_PROTOCOL             *I2cIoProtocol;
  EFI_DRIVER_BINDING_PROTOCOL     DriverBinding;
  EFI_STATUS                      Status;
  UINT8                           Counter;
  PROTOCOLS_LIST                  *ProtocolList;
  CHAR16                          *UnicodeString1;
  CHAR16                          *UnicodeString2;
  CHAR16                          UnicodeCheck[] = L"I2C PSS Monza Driver";

  I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  ProtocolList = NULL;
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  //
  // Mock boot services.
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocol;
  gBS->InstallMultipleProtocolInterfaces = MockBootServiceInstallMultipleProtocolInterfaces;
  //
  // Test1: Mock handle doesn't support gEfiI2cIoProtocolGuid.
  // Expected Result: EFI_UNSUPPORTED.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cIoProtocolGuid);
  will_return (MockBootServiceOpenProtocol, EFI_UNSUPPORTED);
  will_return (MockBootServiceOpenProtocol, I2cIoProtocol);

  Status = PssMonzaDriverBindingStart (&DriverBinding, NULL, NULL);
  //
  // Test2: Mock handle support gEfiI2cIoProtocolGuid then check InstallMultipleProtocolInterfaces
  //        if Protocol is already installed on the handle specified by Handle.
  // Expected Result: EFI_INVALID_PARAMETER.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cIoProtocolGuid);
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, I2cIoProtocol);

  //
  // Mock AddUnicodeString2 and check if UnicodeString is "I2C PSS Monza Driver".
  //
  will_return (AddUnicodeString2, &UnicodeString1);
  will_return (AddUnicodeString2, EFI_SUCCESS);
  will_return (AddUnicodeString2, &UnicodeString2);
  will_return (AddUnicodeString2, EFI_SUCCESS);
  //
  // Mock InstallMultipleProtocolInterfaces. All protocols will store in ProtocolList.
  // Then check protocols that we expected by CompareGuid().
  //
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, &ProtocolList);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, &Counter);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, EFI_INVALID_PARAMETER);
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);

  Status = PssMonzaDriverBindingStart (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  // Check UnicodeString whether "I2C PSS Monza Driver" or not.
  if ((!StrCmp (UnicodeString1,UnicodeCheck)) && (!StrCmp (UnicodeString2,UnicodeCheck))) {
    UT_ASSERT_EQUAL (EFI_SUCCESS, EFI_SUCCESS);
  } else {
    UT_ASSERT_EQUAL (EFI_SUCCESS, EFI_INVALID_PARAMETER);
  }
  //
  // Test3: Mock handle support gEfiI2cIoProtocolGuid then check InstallMultipleProtocolInterfaces
  //        all the protocol interface was installed.
  // Expected Result: EFI_SUCCESS.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cIoProtocolGuid);
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, I2cIoProtocol);

  //
  // Mock AddUnicodeString2 and check if UnicodeString is "I2C PSS Monza Driver".
  //
  will_return (AddUnicodeString2, &UnicodeString1);
  will_return (AddUnicodeString2, EFI_SUCCESS);
  will_return (AddUnicodeString2, &UnicodeString2);
  will_return (AddUnicodeString2, EFI_SUCCESS);

  //
  // Mock InstallMultipleProtocolInterfaces. All protocols will store in ProtocolList.
  // Then check protocols that we expected by CompareGuid().
  //
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, &ProtocolList);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, &Counter);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, EFI_SUCCESS);

  Status = PssMonzaDriverBindingStart (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  // Check ProtocolList include gI2cPssProtocolGuid and UnicodeString is "I2C PSS Monza Driver".
  if ((!StrCmp (UnicodeString1,UnicodeCheck)) && (!StrCmp (UnicodeString2,UnicodeCheck)) && (CompareGuid (ProtocolList[0].Protocol, &gI2cPssProtocolGuid))) {
    Status = EFI_SUCCESS;
  } else {
  // If protocols or UnicodeString not we expected.
    Status =EFI_INVALID_PARAMETER;
  }
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  FreePool (I2cIoProtocol);
  FreePool (ProtocolList);
  FreePool (gBS);
  return UNIT_TEST_PASSED;
}