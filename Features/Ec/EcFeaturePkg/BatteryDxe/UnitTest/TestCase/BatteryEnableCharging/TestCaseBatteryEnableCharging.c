/**@file
    Unit test case of the Battery Dxe driver.

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

#include <EcCommands.h>
#include "TestCaseGetBatteryInformation.h"
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeBatteryLib.h>
#include <Protocol/Battery.h>

extern
EFI_STATUS
EFIAPI
BatteryEnableCharging (
  IN EFI_BATTERY_PROTOCOL  *This,
  IN BOOLEAN               EnableCharging
  );

/**
  Unit test for BatteryEnableCharging

  @param[in]  Context    [Optional] An optional parameter that enables:
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
UnitTestBatteryEnableCharging (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_BATTERY_PROTOCOL *BatteryProtocol;
  EFI_STATUS           Status;

  DEBUG ((DEBUG_INFO, "%a Start Test\n", __FUNCTION__));

  //
  // Test function GetBatteryStatus with input parameter using NULL case
  //
  // Expect Result:
  //   - EFI_INVALID_PARAMETER: have error handle the NULL input parameter
  //
  Status = BatteryEnableCharging (NULL, FALSE);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  Status = BatteryEnableCharging (NULL, TRUE);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Test function GetBatteryStatus with input parameter using Mock case
  //
  // Expect Result:
  //   - EFI_UNSUPPORTED: SetBatteryEnableCharging currently is not ready.
  //
  BatteryProtocol = AllocateZeroPool (sizeof (EFI_BATTERY_PROTOCOL));

  Status = BatteryEnableCharging (BatteryProtocol, FALSE);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  Status = BatteryEnableCharging (BatteryProtocol, TRUE);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);


  DEBUG ((DEBUG_INFO, "%a End Test\n", __FUNCTION__));
  FreePool (BatteryProtocol);

  return UNIT_TEST_PASSED;
}
