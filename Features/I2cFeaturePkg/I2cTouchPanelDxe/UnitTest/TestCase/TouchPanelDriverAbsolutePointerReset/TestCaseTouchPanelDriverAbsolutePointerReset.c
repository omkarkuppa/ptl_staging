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
#include "TestCaseTouchPanelDriverAbsolutePointerReset.h"
#include <Library/DebugLib.h>
#include <I2cTouchPanel.h>

/**
  This function resets the pointer device hardware. As part of
  initialization process, the firmware/device will make a quick
  but reasonable attempt to verify that the device is
  functioning. If the ExtendedVerification flag is TRUE the
  firmware may take an extended amount of time to verify the
  device is operating on reset. Otherwise the reset operation is
  to occur as quickly as possible. The hardware verification
  process is not defined by this specification and is left up to
  the platform firmware or driver to implement.

  @param This                 A pointer to the EFI_ABSOLUTE_POINTER_PROTOCOL
                              instance.

  @param ExtendedVerification Indicates that the driver may
                              perform a more exhaustive
                              verification operation of the
                              device during reset.

  @retval EFI_SUCCESS       The device was reset.

  @retval EFI_DEVICE_ERROR  The device is not functioning
                            correctly and could not be reset.

**/
extern
EFI_STATUS
EFIAPI
TouchPanelDriverAbsolutePointerReset (
  IN EFI_ABSOLUTE_POINTER_PROTOCOL *This,
  IN BOOLEAN                       ExtendedVerification
  );
/**
  Mock of TouchPanelDriverAbsolutePointerReset

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
UnitTestTouchPanelDriverAbsolutePointerReset (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                     Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test1 :Mock TouchPanelDriverAbsolutePointerReset.
  // Expected Result: EFI_UNSUPPORTED.
  //
  Status = TouchPanelDriverAbsolutePointerReset (NULL, 0);

  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}