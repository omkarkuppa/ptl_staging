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
#include "TestCaseTouchPanelDriverAbsolutePointerGetState.h"
#include <Library/DebugLib.h>
#include <I2cTouchPanel.h>

/**
  The GetState() function retrieves the current state of a pointer
  device. This includes information on the active state associated
  with the pointer device and the current position of the axes
  associated with the pointer device. If the state of the pointer
  device has not changed since the last call to GetState(), then
  EFI_NOT_READY is returned. If the state of the pointer device
  has changed since the last call to GetState(), then the state
  information is placed in State, and EFI_SUCCESS is returned. If
  a device error occurs while attempting to retrieve the state
  information, then EFI_DEVICE_ERROR is returned.


  @param This   A pointer to the EFI_ABSOLUTE_POINTER_PROTOCOL
                instance.

  @param State  A pointer to the state information on the
                pointer device.

  @retval EFI_SUCCESS       The state of the pointer device was
                            returned in State.

  @retval EFI_NOT_READY     The state of the pointer device has not
                            changed since the last call to GetState().

  @retval EFI_DEVICE_ERROR  A device error occurred while
                            attempting to retrieve the pointer
                            device's current state.

**/
extern
EFI_STATUS
EFIAPI
TouchPanelDriverAbsolutePointerGetState (
  IN      EFI_ABSOLUTE_POINTER_PROTOCOL  *This,
  IN OUT  EFI_ABSOLUTE_POINTER_STATE     *State
  );
/**
  Mock of TouchPanelDriverAbsolutePointerGetState

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
UnitTestTouchPanelDriverAbsolutePointerGetState (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  ABSOLUTE_POINTER_DEV  *This;
  EFI_ABSOLUTE_POINTER_STATE     *State;
  EFI_STATUS                     Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test1: If This or State equal to NULL, Unit Test will return EFI_INVALID_PARAMETER.
  // Expected Result: EFI_INVALID_PARAMETER.
  //

  // State = NULL
  This = AllocateZeroPool (sizeof (ABSOLUTE_POINTER_DEV));
  Status = TouchPanelDriverAbsolutePointerGetState (&(This->AbsolutePointerProtocol), NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  // This = NULL
  State = AllocateZeroPool (sizeof (EFI_ABSOLUTE_POINTER_STATE));
  Status = TouchPanelDriverAbsolutePointerGetState (NULL, State);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Test2-1: TouchDev->NewStateAvailable equal to zero. Unit Test will return EFI_NOT_READY.
  // Expected Result: EFI_NOT_READY.
  //
  This->Signature = TOUCH_SIGNATURE;
  This->NewStateAvailable = 0;
  Status = TouchPanelDriverAbsolutePointerGetState (&(This->AbsolutePointerProtocol), State);
  UT_ASSERT_EQUAL (Status, EFI_NOT_READY);
  //
  // Test2-2: TouchDev->NewStateAvailable equal to 1. Unit Test will return EFI_SUCCESS.
  // Expected Result: EFI_SUCCESS.
  //
  This->NewStateAvailable = 1;
  Status = TouchPanelDriverAbsolutePointerGetState (&(This->AbsolutePointerProtocol), State);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  FreePool (This);
  FreePool (State);
  return UNIT_TEST_PASSED;
}