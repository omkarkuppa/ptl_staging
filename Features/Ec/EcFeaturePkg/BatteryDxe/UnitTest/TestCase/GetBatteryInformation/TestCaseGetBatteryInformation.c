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
GetBatteryInformation (
  IN  EFI_BATTERY_PROTOCOL  *This,
  OUT BATTERY_INFORMATION   *BatteryInformation
  );

/**
  Unit test for GetBatteryInformation based on real battery

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
UnitTestGetBatteryInformationRealBat (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BATTERY_INFORMATION  BatteryInformation;
  EFI_BATTERY_PROTOCOL *BatteryProtocol;
  EFI_STATUS           Status;
  UINT32               DummyDesignedCapacity;
  UINT32               DummyLastFullChargeCapacity;
  UINT32               DummyDesignVoltage;
  UINT32               DummyDesignCapacityOfWarning;
  UINT32               DummyDesignCapacityOfLow;

  DEBUG ((DEBUG_INFO, "%a Start Test\n", __FUNCTION__));

  //
  // Test function GetBatteryInformation with input parameter using NULL case
  //
  // Expect Result:
  //   - EFI_INVALID_PARAMETER: have error handle the NULL input parameter
  //
  Status = GetBatteryInformation (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Test function GetBatteryInformation with input parameter using Mock case
  //
  // Expect Result:
  //   - EFI_SUCCESS: Get battery for real battery information
  //
  BatteryProtocol = AllocateZeroPool (sizeof (EFI_BATTERY_PROTOCOL));
  ZeroMem (&BatteryInformation, sizeof (BATTERY_INFORMATION));

  //
  // GetDesignCapacity needed mock
  //

  DummyDesignedCapacity = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_DESIGN_CAPACITY + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyDesignedCapacity >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_DESIGN_CAPACITY);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyDesignedCapacity & 0xFF);


  //
  // GetLastFullChargeCapacity needed mock
  //
  DummyLastFullChargeCapacity = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_FULL_CHARGE_CAPACITY + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyLastFullChargeCapacity >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_FULL_CHARGE_CAPACITY);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyLastFullChargeCapacity & 0xFF);

  //
  // GetBatteryTechnology needed mock
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

  //
  // GetDesignVoltage needed mock
  //
  DummyDesignVoltage = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_DESIGN_VOLTAGE + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyDesignVoltage >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_DESIGN_VOLTAGE);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyDesignVoltage & 0xFF);

  //
  // GetDesignCapacityOfWarning needed mock
  //
  DummyDesignCapacityOfWarning = 0x9088;
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
  expect_value (IoWrite8, Value, (BATTERY_1_FULL_CHARGE_CAPACITY + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyDesignCapacityOfWarning >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_FULL_CHARGE_CAPACITY);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyDesignCapacityOfWarning & 0xFF);

  //
  // GetDesignCapacityOfLow  needed mock
  //
  DummyDesignCapacityOfLow = 0x9088;
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
  expect_value (IoWrite8, Value, (BATTERY_1_FULL_CHARGE_CAPACITY + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyDesignCapacityOfLow >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_FULL_CHARGE_CAPACITY);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyDesignCapacityOfLow & 0xFF);

  //
  // GetModelNumber needed mock
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

  //
  // GetSerialNumber needed mock
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

  //
  // GetBatteryType needed mock
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

  //
  // GetOEMInformation needed mock
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

  Status = GetBatteryInformation (BatteryProtocol, &BatteryInformation);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (BatteryInformation.DesignedCapacity, DummyDesignedCapacity);
  UT_ASSERT_EQUAL (BatteryInformation.LastFullChargeCapacity, DummyLastFullChargeCapacity);
  UT_ASSERT_EQUAL (BatteryInformation.BatteryTechnology, 0x00000001);                       // PHYSICAL_BATTERY_TECHNOLOGY
  UT_ASSERT_EQUAL (BatteryInformation.DesignVoltage, DummyDesignVoltage);
  UT_ASSERT_EQUAL (BatteryInformation.DesignCapacityOfWarning, DummyDesignCapacityOfWarning / 10);
  UT_ASSERT_EQUAL (BatteryInformation.DesignCapacityOfLow, DummyDesignCapacityOfLow / 25);
  UT_ASSERT_MEM_EQUAL (BatteryInformation.ModelNumber, "BASE-BAT", sizeof ("BASE-BAT"));    // PHYSICAL_MODEL_NUMBER
  UT_ASSERT_MEM_EQUAL (BatteryInformation.SerialNumber, "123456789", sizeof ("123456789")); // PHYSICAL_SERIAL_NUMBER
  UT_ASSERT_MEM_EQUAL (BatteryInformation.BatteryType, "LiP", sizeof ("LiP"));              // PHYSICAL_BATTERY_TYPE
  UT_ASSERT_MEM_EQUAL (BatteryInformation.OEMInformation, "Simplo", sizeof ("Simplo"));     // PHYSICAL_OEM_INFORMATION


  DEBUG ((DEBUG_INFO, "%a End Test\n", __FUNCTION__));
  FreePool (BatteryProtocol);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for GetBatteryInformation based on virtual battery

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
UnitTestGetBatteryInformationVirtualBat (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BATTERY_INFORMATION  BatteryInformation;
  EFI_BATTERY_PROTOCOL *BatteryProtocol;
  EFI_STATUS           Status;
  UINT32               DummyDesignedCapacity;
  UINT32               DummyLastFullChargeCapacity;
  UINT32               DummyDesignVoltage;

  DEBUG ((DEBUG_INFO, "%a Start Test\n", __FUNCTION__));

  //
  // Test function GetBatteryInformation with input parameter using NULL case
  //
  // Expect Result:
  //   - EFI_INVALID_PARAMETER: have error handle the NULL input parameter
  //
  Status = GetBatteryInformation (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Test function GetBatteryInformation with input parameter using Mock case
  //
  // Expect Result:
  //   - EFI_SUCCESS: Get battery for virtual battery information
  //
  BatteryProtocol = AllocateZeroPool (sizeof (EFI_BATTERY_PROTOCOL));
  ZeroMem (&BatteryInformation, sizeof (BATTERY_INFORMATION));

  //
  // GetDesignCapacity needed mock
  //
  DummyDesignedCapacity       = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_DESIGN_CAPACITY + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyDesignedCapacity >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_DESIGN_CAPACITY);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyDesignedCapacity & 0xFF);


  //
  // GetLastFullChargeCapacity needed mock
  //
  DummyLastFullChargeCapacity = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_FULL_CHARGE_CAPACITY + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyLastFullChargeCapacity >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_FULL_CHARGE_CAPACITY);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyLastFullChargeCapacity & 0xFF);

  //
  // GetBatteryTechnology needed mock
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
  will_return (IoRead8, 0x00);

  //
  // GetDesignVoltage needed mock
  //
  DummyDesignVoltage          = 0x8786;
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, (BATTERY_1_DESIGN_VOLTAGE + 1));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, (DummyDesignVoltage >> 8) & 0xFF);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_C_PORT);
  expect_value (IoWrite8, Value, EC_C_READ_MEM);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, 0x00);
  expect_value (IoWrite8, Port, EC_D_PORT);
  expect_value (IoWrite8, Value, BATTERY_1_DESIGN_VOLTAGE);
  expect_value (IoRead8, Port, EC_C_PORT);
  will_return (IoRead8, (EC_S_IBF | EC_S_OBF));
  expect_value (IoRead8, Port, EC_D_PORT);
  will_return (IoRead8, DummyDesignVoltage & 0xFF);

  //
  // GetDesignCapacityOfWarning needed mock
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
  will_return (IoRead8, 0x00);


  //
  // GetDesignCapacityOfLow  needed mock
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
  will_return (IoRead8, 0x00);

  //
  // GetModelNumber needed mock
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
  will_return (IoRead8, 0x00);

  //
  // GetSerialNumber needed mock
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
  will_return (IoRead8, 0x00);

  //
  // GetBatteryType needed mock
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
  will_return (IoRead8, 0x00);

  //
  // GetOEMInformation needed mock
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
  will_return (IoRead8, 0x00);

  Status = GetBatteryInformation (BatteryProtocol, &BatteryInformation);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (BatteryInformation.DesignedCapacity, DummyDesignedCapacity);
  UT_ASSERT_EQUAL (BatteryInformation.LastFullChargeCapacity, DummyLastFullChargeCapacity);
  UT_ASSERT_EQUAL (BatteryInformation.BatteryTechnology, 0x00000001);                                               // VIRTUAL_BATTERY_TECHNOLOGY
  UT_ASSERT_EQUAL (BatteryInformation.DesignVoltage, DummyDesignVoltage);
  UT_ASSERT_EQUAL (BatteryInformation.DesignCapacityOfWarning, 0x000003E8);                                         // VIRTUAL_DESIGN_CAPACITY_OF_WARNING
  UT_ASSERT_EQUAL (BatteryInformation.DesignCapacityOfLow, 0x00000190);                                             // VIRTUAL_DESIGN_CAPACITY_OF_LOW
  UT_ASSERT_MEM_EQUAL (BatteryInformation.ModelNumber, "CRB Battery 0", sizeof ("CRB Battery 0"));                  // VIRTUAL_MODEL_NUMBER
  UT_ASSERT_MEM_EQUAL (BatteryInformation.SerialNumber, "Battery 0", sizeof ("Battery 0"));                         // VIRTUAL_SERIAL_NUMBER
  UT_ASSERT_MEM_EQUAL (BatteryInformation.BatteryType, "Fake", sizeof ("Fake"));                                    // VIRTUAL_BATTERY_TYPE
  UT_ASSERT_MEM_EQUAL (BatteryInformation.OEMInformation, "-Virtual Battery 0-", sizeof ("-Virtual Battery 0-"));   // VIRTUAL_OEM_INFORMATION


  DEBUG ((DEBUG_INFO, "%a End Test\n", __FUNCTION__));
  FreePool (BatteryProtocol);

  return UNIT_TEST_PASSED;
}