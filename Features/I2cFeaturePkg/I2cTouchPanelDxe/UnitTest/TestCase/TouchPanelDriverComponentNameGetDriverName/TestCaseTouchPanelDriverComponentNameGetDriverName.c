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
#include "TestCaseTouchPanelDriverComponentNameGetDriverName.h"
#include <Protocol/DriverBinding.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>
#include <Library/DebugLib.h>
#include <I2cTouchPanel.h>

extern
EFI_STATUS
EFIAPI
TouchPanelDriverComponentNameGetDriverName (
  IN EFI_COMPONENT_NAME_PROTOCOL   *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );
/**
  Mock TouchPanelDriverComponentNameGetDriverName

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
UnitTestTouchPanelDriverComponentNameGetDriverName (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                     Status;
  CHAR16                         *DriverName;
  CHAR16                         UnicodeCheck[] = L"HID touchpanel driver";

  DriverName = NULL;
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test1 :Mock LookupUnicodeString2 to get Driver name. Check DriverName whether "HID touchpanel driver" or not.
  // Expected Result: EFI_SUCCESS.
  //

  // Mock LookupUnicodeString2 to get Driver name.
  will_return (LookupUnicodeString2, EFI_SUCCESS);

  Status = TouchPanelDriverComponentNameGetDriverName (&gTouchPanelDriverComponentName, NULL, &DriverName);
  // Check DriverName whether "HID touchpanel driver" or not.
  UT_ASSERT_EQUAL (StrCmp (DriverName,UnicodeCheck), 0);
  // Check Status is EFI_SUCCESS.
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}