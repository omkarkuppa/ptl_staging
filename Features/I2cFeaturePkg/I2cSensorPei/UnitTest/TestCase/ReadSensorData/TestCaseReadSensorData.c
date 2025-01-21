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
#include <Library/MemoryAllocationLib.h>
#include "TestCaseReadSensorData.h"
#include <Library/DebugLib.h>
#include <I2cSensorLSM6DSOX.h>
#include <I2cSensorPeiLib.h>
#include <PrivateMock.h>
#include <I2cSensorPei.h>

/**
  Read orientation sensor data

  @param[in]  Address
  @param[out]  Buffer
  @param[in]  BufferSize

  @retval  EFI_STATUS
**/
EFI_STATUS
ReadSensorData (
  IN UINT8     Address,
  OUT UINT8     *Buffer,
  IN UINT32    BufferSize
  );

/**
  Unit Test of ReadSensorData

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
UnitTestReadSensorData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS   Status;
  UINT8        *ReadBuffer;
  UINT8        *WriteBuffer;
  UINT32       BufferSize = 2;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  WriteBuffer = AllocateZeroPool (1);
  ReadBuffer  = AllocateZeroPool (BufferSize);

  // Mock function for PcdGet8.
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdOrientationI2cSensorBusNumber);
  will_return (LibPcdGet8, 0x03);
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdOrientationI2cSensorAddress);
  will_return (LibPcdGet8, 0x6b);

  // Mock function for SerialIoI2cWrite
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 1);
  will_return (SerialIoI2cWrite, WriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  // Mock function for PcdGet8.
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdOrientationI2cSensorAddress);
  will_return (LibPcdGet8, 0x6b);

  // Mock function for SerialIoI2cRead
  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, BufferSize);
  will_return (SerialIoI2cRead, ReadBuffer);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  //
  // Running Platform ReadSensorData function
  //
  Status = ReadSensorData (*WriteBuffer, ReadBuffer, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}