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
#include <EcRamOffset.h>
#include "TestCaseGetBatteryInformation.h"
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeBatteryLib.h>
#include <Protocol/Battery.h>

extern
EFI_STATUS
EFIAPI
GetBatteryStatus (
  IN  EFI_BATTERY_PROTOCOL  *This,
  OUT BATTERY_STATUS        *BatteryStatus
  );

/**
  Unit test for GetBatteryStatus

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
UnitTestGetBatteryStatus (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BATTERY_STATUS       BatteryStatus;
  EFI_BATTERY_PROTOCOL *BatteryProtocol;
  EFI_STATUS           Status;
  UINT32               DummyBatteryPresentRate;
  UINT32               DummyBatteryRemainingCapacity;
  UINT32               DummyBatteryPresentVoltage;

  DEBUG ((DEBUG_INFO, "%a Start Test\n", __FUNCTION__));

  //
  // Test function GetBatteryStatus with input parameter using NULL case
  //
  // Expect Result:
  //   - EFI_INVALID_PARAMETER: have error handle the NULL input parameter
  //
  Status = GetBatteryStatus (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Test function GetBatteryStatus with input parameter using Mock case
  //
  // Expect Result:
  //   - EFI_SUCCESS: Get battery for real battery information
  //
  BatteryProtocol = AllocateZeroPool (sizeof (EFI_BATTERY_PROTOCOL));
  ZeroMem (&BatteryStatus, sizeof (BATTERY_STATUS));

  //
  // GetBatteryState needed mock
  //
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_STATUS);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, BIT3);

  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_STATUS);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (BIT0 | BIT1 | BIT2));

  //
  // GetBatteryPresentRate needed mock
  //
  DummyBatteryPresentRate       = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_STATUS);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, BIT3);

  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_STATUS);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (BIT0 | BIT3));

  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_DISCHARGE_CURRENT + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyBatteryPresentRate >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_DISCHARGE_CURRENT);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyBatteryPresentRate & 0xFF);

  //
  // GetBatteryRemainingCapacity needed mock
  //
  DummyBatteryRemainingCapacity = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_STATUS);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, BIT3);

  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_REMAINING_CAPACITY + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyBatteryRemainingCapacity >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_REMAINING_CAPACITY);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyBatteryRemainingCapacity & 0xFF);

  //
  // GetBatteryPresentVoltage needed mock
  //
  DummyBatteryPresentVoltage    = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_STATUS);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, BIT3);

  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_FULL_RESOLUTION_VOLTAGE + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyBatteryPresentVoltage >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_FULL_RESOLUTION_VOLTAGE);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyBatteryPresentVoltage & 0xFF);

  Status = GetBatteryStatus (BatteryProtocol, &BatteryStatus);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (BatteryStatus.BatteryState & BIT0, BIT0);
  UT_ASSERT_EQUAL (BatteryStatus.BatteryState & BIT1, BIT1);
  UT_ASSERT_EQUAL (BatteryStatus.BatteryState & BIT2, BIT2);
  UT_ASSERT_EQUAL (BatteryStatus.BatteryPresentRate, DummyBatteryPresentRate);
  UT_ASSERT_EQUAL (BatteryStatus.BatteryRemainingCapacity, DummyBatteryRemainingCapacity);
  UT_ASSERT_EQUAL (BatteryStatus.BatteryPresentVoltage, DummyBatteryPresentVoltage);

  DEBUG ((DEBUG_INFO, "%a End Test\n", __FUNCTION__));
  FreePool (BatteryProtocol);

  return UNIT_TEST_PASSED;
}
