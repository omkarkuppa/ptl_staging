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
#include "TestCaseReadLsm6dsoxOrientation.h"
#include "TestCaseReadSensorData.h"
#include "TestCaseWriteSensor16.h"
#include "TestCaseWriteSensorByte.h"
#include <Library/DebugLib.h>
#include <I2cSensorLSM6DSOX.h>
#include <Library/MemoryAllocationLib.h>
#include <PrivateMock.h>
#include <Library/TimerLib.h>
#include <I2cSensorPeiLib.h>
#include <I2cSensorPei.h>
#include "Library/PcdLib.h"

/**
  Mock of ReadLsm6dsoxOrientation

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
UnitTestReadLsm6dsoxOrientation (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS   Status;
  UINT8        Data;
  UINT16       Index;
  UINT8        *SensorWriteBuffer;
  UINT32       BufferSize = 1;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  SensorWriteBuffer = AllocateZeroPool (2);

  // Mock Function for ReadSensorData - WHO_AM_I
  Data = V_LSM6DSL_CHIP_ID;
  *SensorWriteBuffer = R_LSM6DSL_WHO_AM_I;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, BufferSize);
  will_return (SerialIoI2cRead, &Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_WHO_AM_I, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  // Mock Function for WriteSensorByte - CTRL3_REG_SW_RESET
  SensorWriteBuffer[0] = R_LSM6DSL_CTRL3_C;
  SensorWriteBuffer[1] = B_LSM6DSL_CTRL3_REG_SW_RESET;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensorByte (R_LSM6DSL_CTRL3_C, B_LSM6DSL_CTRL3_REG_SW_RESET);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  // Mock Function for ReadSensorData - CTRL3_REG_SW_RESET
  Data = B_LSM6DSL_CTRL3_REG_SW_RESET;
  *SensorWriteBuffer = R_LSM6DSL_CTRL3_C;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, BufferSize);
  will_return (SerialIoI2cRead, &Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_CTRL3_C, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  // Mock Function for WriteSensorByte - CTRL3_REG_IF_INC
  SensorWriteBuffer[0] = R_LSM6DSL_CTRL3_C;
  SensorWriteBuffer[1] = B_LSM6DSL_CTRL3_REG_IF_INC;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensorByte (R_LSM6DSL_CTRL3_C, B_LSM6DSL_CTRL3_REG_IF_INC);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  // Mock Function for ReadSensorData - CTRL3_REG_IF_INC
  Data = B_LSM6DSL_CTRL3_REG_IF_INC;
  *SensorWriteBuffer = R_LSM6DSL_CTRL3_C;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, 1);
  will_return (SerialIoI2cRead, Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_CTRL3_C, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  // Mock Function for ReadSensorData - CTRL6_C
  Data = B_LSM6DSL_CTRL3_REG_IF_INC;
  *SensorWriteBuffer = R_LSM6DSL_CTRL3_C;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, BufferSize);
  will_return (SerialIoI2cRead, &Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_CTRL3_C, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //Mock Function for Write 60h to R_CTRL1_XL (10h)
  SensorWriteBuffer[0] = R_LSM6DSL_CTRL1_XL;
  SensorWriteBuffer[1] = V_LSM6DSL_CTRL1_XL_WRITE;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensorByte (R_LSM6DSL_CTRL1_XL, V_LSM6DSL_CTRL1_XL_WRITE);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  Data = V_LSM6DSL_CTRL1_XL_WRITE;
  *SensorWriteBuffer = R_LSM6DSL_CTRL1_XL;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, 1);
  will_return (SerialIoI2cRead, &Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_CTRL1_XL, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //Mock Function for Write 80h to R_TAP_CFG (58h)
  SensorWriteBuffer[0] = R_LSM6DSL_TAP_CFG;
  SensorWriteBuffer[1] = V_LSM6DSL_TAP_CFG_WRITE;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensorByte (R_LSM6DSL_TAP_CFG, V_LSM6DSL_TAP_CFG_WRITE);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  Data = V_LSM6DSL_TAP_CFG_WRITE;
  *SensorWriteBuffer = R_LSM6DSL_TAP_CFG;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, BufferSize);
  will_return (SerialIoI2cRead, &Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_TAP_CFG, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //Mock function for Write 40h to R_TAP_THS_6D (59h)
  SensorWriteBuffer[0] = R_LSM6DSL_TAP_THS_6D;
  SensorWriteBuffer[1] = V_LSM6DSL_TAP_THS_6D_WRITE;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensorByte (R_LSM6DSL_TAP_THS_6D, V_LSM6DSL_TAP_THS_6D_WRITE);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  Data = V_LSM6DSL_TAP_THS_6D_WRITE;
  *SensorWriteBuffer = R_LSM6DSL_TAP_THS_6D;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, 1);
  will_return (SerialIoI2cRead, &Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_TAP_THS_6D, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //Mock function for Write 01h to R_CTRL8_XL (17h)
  SensorWriteBuffer[0] = R_LSM6DSL_CTRL8_XL;
  SensorWriteBuffer[1] = V_LSM6DSL_CTRL8_XL_WRITE;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensorByte (R_LSM6DSL_CTRL8_XL, V_LSM6DSL_CTRL8_XL_WRITE);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  Data = V_LSM6DSL_CTRL8_XL_WRITE;
  *SensorWriteBuffer = R_LSM6DSL_CTRL8_XL;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, BufferSize);
  will_return (SerialIoI2cRead, &Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_CTRL8_XL, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //Mock function for Write 04h to MD1_CFG (5Eh)
  SensorWriteBuffer[0] = R_LSM6DSL_MD1_CFG;
  SensorWriteBuffer[1] = V_LSM6DSL_MD1_CFG_WRITE;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensorByte (R_LSM6DSL_MD1_CFG, V_LSM6DSL_MD1_CFG_WRITE);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  Data = V_LSM6DSL_MD1_CFG_WRITE;
  *SensorWriteBuffer = R_LSM6DSL_MD1_CFG;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, 1);
  will_return (SerialIoI2cRead, &Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_LSM6DSL_MD1_CFG, &Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //Mock function for Read device orientation
  Data = 0;
  *SensorWriteBuffer = R_LSM6DSL_D6D_SRC;
  for (Index = 0; Index < 256; Index++) {
    if ((Data & B_LSM6DSL_D6D_SRC_XL) || (Data & B_LSM6DSL_D6D_SRC_XH)) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, BufferSize);
        will_return (SerialIoI2cRead, &Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_LSM6DSL_D6D_SRC, &Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data ++;
  }

  Data = 0;
  *SensorWriteBuffer = R_LSM6DSL_D6D_SRC;
  for (Index = 0; Index < 256; Index++) {
    if ((Data & B_LSM6DSL_D6D_SRC_YL) || (Data & B_LSM6DSL_D6D_SRC_YH)) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, BufferSize);
        will_return (SerialIoI2cRead, &Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_LSM6DSL_D6D_SRC, &Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data ++;
  }

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}