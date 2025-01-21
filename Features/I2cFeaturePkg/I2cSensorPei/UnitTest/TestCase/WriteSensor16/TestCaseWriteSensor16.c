/**@file
    Unit test case of the I2c Sensor PEIM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include "TestCaseReadSensorData.h"
#include "TestCaseWriteSensor16.h"
#include <Library/DebugLib.h>
#include <I2cSensorLSM6DSOX.h>
#include <Library/MemoryAllocationLib.h>
#include <I2cSensorPeiLib.h>
#include <PrivateMock.h>
#include <I2cSensorPei.h>

/**
  Write 16 bits data to orientation sensor.

  @param[in]  Address    Sensor internal register address
  @param[in]  Value      Value to write

  @retval  EFI_STATUS
**/
EFI_STATUS
WriteSensor16 (
  IN UINT8      Address,
  IN UINT16     Value
  );

/**
  Unit Test of WriteSensor16.

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
UnitTestWriteSensor16 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS   Status;
  UINT8        WriBuf[3] = {0,0,0};

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  // Mock function for PcdGet8.
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdOrientationI2cSensorBusNumber);
  will_return (LibPcdGet8, 0x03);
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdOrientationI2cSensorAddress);
  will_return (LibPcdGet8, 0x6b);

  // Mock function for SerialIoI2cWrite
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 3);
  will_return (SerialIoI2cWrite, (UINT8*)WriBuf);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  //
  // Running Platform WriteSensor16 function
  //
  Status = WriteSensor16 (WriBuf[0], 1);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}