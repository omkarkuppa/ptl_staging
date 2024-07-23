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
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include "TestCaseGetBusFrequency.h"
#include <I2cPlatformDxe.h>
#include "PrivateMock/PrivateMock.h"

/**
  GetBusFrequency
  For now, this function simply returns 100kHz because we only use one bus,
  single configuration, with a single device and it accepts just that speed

  @param[in]      This                     A pointer to the EFI_I2C_ENUMERATE_PROTOCOL instance.
  @param[in]      I2cBusConfiguration      A pointer to the I2cBusConfiguration
  @param[out]     BusClockHertz            Pointer is updated with right Serial I2C controller frequency

  @retval EFI_SUCCESS              BusClockHertz is returned with freq value
  @retval EFI_NOT_FOUND            End of device list
**/
extern
EFI_STATUS
EFIAPI
GetBusFrequency (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN UINTN                            I2cBusConfiguration,
  OUT UINTN                           *BusClockHertz
  );
/**
  Mock GetBusFrequency.

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
UnitTestGetBusFrequency (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT64                       *Frequency;
  I2C_FREQUENCY_DATA_TABLE     *I2cFrequencyTable;
  UINT64                       BusClockHertz;
  UINT8                        Index;
  EFI_STATUS                   Status;
  I2C_PLATFORM_CONTEXT         *Private;
  UINTN                        I2cBusConfiguration;
  UINTN                        BusFrequency;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  I2cBusConfiguration = 0;
  BusFrequency = 0;
  Private = AllocateZeroPool (sizeof (I2C_PLATFORM_CONTEXT));
  Private->Signature = I2C_PLATFORM_SIGNATURE;
  I2cFrequencyTable = AllocateZeroPool (sizeof (I2C_FREQUENCY_DATA_TABLE));
  I2cFrequencyTable->Count = MAX_I2C_CONTROLLERS_NUM;
  BusClockHertz = 100 * 1000;
  Frequency = AllocateZeroPool (sizeof (UINTN));
  for (Index = 0; Index < I2cFrequencyTable->Count; Index ++) {
    I2cFrequencyTable->I2cFrequency[Index] = BusClockHertz;
  }
  //
  // Test Function "GetBusFrequency"
  // Mock Data
  //  1. Get Pcd PcdI2cFrequencyDataMap.
  //
  // Expected Result
  // EFI_SUCCESS: 1.PcdI2cFrequencyDataMap was found.
  //

  // Check PcdI2cFrequencyDataMap
  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdI2cFrequencyDataMap);
  will_return (LibPcdGetExPtr, I2cFrequencyTable);
  Status = GetBusFrequency ( &(Private->Enumerate), I2cBusConfiguration, &BusFrequency);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Destroy Mock Service
  //
  FreePool (Private);
  FreePool (Frequency);
  FreePool (I2cFrequencyTable);
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}
