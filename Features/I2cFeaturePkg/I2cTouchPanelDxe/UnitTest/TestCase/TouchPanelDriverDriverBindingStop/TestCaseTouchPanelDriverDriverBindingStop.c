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
#include "TestCaseTouchPanelDriverDriverBindingStop.h"
#include <I2cTouchPanel.h>
#include <BootServicesMock.h>
#include <PrivateMock.h>
/**
  Stops a device controller or a bus controller.

  The Stop() function is designed to be invoked from the EFI boot service DisconnectController().
  As a result, much of the error checking on the parameters to Stop() has been moved
  into this common boot service. It is legal to call Stop() from other locations,
  but the following calling restrictions must be followed, or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous call to this
     same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a valid
     EFI_HANDLE. In addition, all of these handles must have been created in this driver's
     Start() function, and the Start() function must have called OpenProtocol() on
     ControllerHandle with an Attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.

  @param[in]  This              A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle  A handle to the device being stopped. The handle must
                                support a bus specific I/O protocol for the driver
                                to use to stop the device.
  @param[in]  NumberOfChildren  The number of child device handles in ChildHandleBuffer.
  @param[in]  ChildHandleBuffer An array of child handles to be freed. May be NULL
                                if NumberOfChildren is 0.

  @retval EFI_SUCCESS           The device was stopped.
  @retval EFI_DEVICE_ERROR      The device could not be stopped due to a device error.

**/
extern
EFI_STATUS
EFIAPI
TouchPanelDriverDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  );
/**
  Mock of TouchPanelDriverDriverBindingStop

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
UnitTestTouchPanelDriverDriverBindingStop (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT64                        OpenProtocolStatus[TEST_CASE_3_ROUNDS]                        = {EFI_UNSUPPORTED,       EFI_SUCCESS          , EFI_SUCCESS};
  UINT64                        UninstallMultipleProtocolInterfacesStatus[TEST_CASE_3_ROUNDS] = {EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_SUCCESS};
  UINT8                         TestIndex;
  PROTOCOLS_LIST                *ProtocolList;
  EFI_STATUS                    Status;
  UINT8                         Counter;
  EFI_DRIVER_BINDING_PROTOCOL   DriverBinding;
  ABSOLUTE_POINTER_DEV          *TouchDev;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Mock Boot service.
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocolNonReturnStatus;
  gBS->UninstallMultipleProtocolInterfaces = MockBootServiceUninstallMultipleProtocolInterfaces;
  gBS->CloseEvent = MockCloseEventNonReturnStatus;
  ProtocolList = NULL;
  TouchDev = AllocateZeroPool (sizeof (ABSOLUTE_POINTER_DEV));
  for (TestIndex = 0; TestIndex < 3; TestIndex++ ) {
    //
    // Test function UnitTestTouchPanelDriverDriverBindingStop will be executed 3 times as follows:
    //
    //        Test1: Mock ControllerHandle doesn't support gEfiAbsolutePointerProtocolGuid.
    //        Expect Result: EFI_SUCCESS
    //
    //        Test2: Mock UninstallMultipleProtocolInterfaces() if Protocol is already installed on the handle specified by Handle.
    //        Expect Result: EFI_INVALID_PARAMETER
    //
    //        Test3: Mock all functions work success.
    //        Expect Result: EFI_SUCCESS
    //
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiAbsolutePointerProtocolGuid);                           // Assume TouchPanelDriverDriverBindingStop will locate gEfiAbsolutePointerProtocolGuid
    will_return (MockBootServiceOpenProtocol, OpenProtocolStatus[TestIndex]);
    will_return (MockBootServiceOpenProtocol, TouchDev);
    if (OpenProtocolStatus[TestIndex] == EFI_SUCCESS) {
      //
      // Mock UninstallMultipleProtocolInterfaces. All protocols will store in ProtocolList.
      // Then check protocols that we expected by CompareGuid().
      //
      will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &ProtocolList);
      will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &Counter);
      will_return (MockBootServiceUninstallMultipleProtocolInterfaces, UninstallMultipleProtocolInterfacesStatus[TestIndex]);
    }
      Status = TouchPanelDriverDriverBindingStop  (&DriverBinding, NULL, 0, NULL);
      if (TestIndex == 0) {
        // If Test case 1, TouchPanelDriverDriverBindingStop will return success.
        UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
      } else if (TestIndex == 1){
        // If Test case 2, TouchPanelDriverDriverBindingStop will return failure.
        UT_ASSERT_NOT_EQUAL (Status, EFI_SUCCESS);
      } else {
        // If Test case 3, TouchPanelDriverDriverBindingStop will return success.
        // and check ProtocolList include gEfiAbsolutePointerProtocolGuid.
        UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
        UT_ASSERT_EQUAL (CompareGuid (ProtocolList[0].Protocol, &gEfiAbsolutePointerProtocolGuid), TRUE);
      }
  }
  FreePool (gBS);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return UNIT_TEST_PASSED;
}