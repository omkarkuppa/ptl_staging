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
#include "TestCaseReadBmi323Orientation.h"
#include "TestCaseWriteSensor16.h"
#include "TestCaseWriteSensorByte.h"
#include <Library/DebugLib.h>
#include "I2cSensorBMI323.h"
#include <Library/MemoryAllocationLib.h>
#include <PrivateMock.h>
#include <Library/TimerLib.h>
#include <I2cSensorPeiLib.h>
#include <I2cSensorPei.h>
#include "Library/PcdLib.h"

/**
  Mock of ReadBmi323Orientation

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
UnitTestReadBmi323Orientation(
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS   Status;
  UINT8        Data[2];
  UINT16       Index;
  UINT8        *SensorWriteBuffer;
  UINT32       BufferSize = 2;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  SensorWriteBuffer = AllocateZeroPool (3);

  // Mock Function for ReadSensorData - Chip id
  Data[0] = V_BMI323_ID;
  Data[1] = 0;
  *SensorWriteBuffer = R_BMI323_CHIP_ID;
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 2);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cRead, ReadLength, 2);
  will_return (SerialIoI2cRead, Data);
  will_return (SerialIoI2cRead, EFI_SUCCESS);

  Status = ReadSensorData (R_BMI323_CHIP_ID, Data, BufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  // Mock Function for ReadSensorData - Device Status
  Data[0] = 0;
  *SensorWriteBuffer = R_BMI323_DEVICE_STATUS;
  for (Index = 0; Index < 256; Index++) {
    if ((Data[0] & 0x1) == V_BMI323_DEVICE_POWER_OK) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, BufferSize);
        will_return (SerialIoI2cRead, Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_BMI323_DEVICE_STATUS, Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data[0] ++;
  }

  // Mock Function for ReadSensorData - Sensor Status
  Data[0] = 0;
  *SensorWriteBuffer = R_BMI323_SENSOR_STATUS;
  for (Index = 0; Index < 256; Index++) {
    if ((Data[0] & 0x1) == V_BMI323_SENSOR_INIT_OK) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, BufferSize);
        will_return (SerialIoI2cRead, Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_BMI323_SENSOR_STATUS, Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data[0] ++;
  }

  // Mock Function for WriteSensor16 - Feature Engin
  SensorWriteBuffer[0] = R_BMI323_FEATURE_IO2;
  SensorWriteBuffer[1] = (UINT8)(V_BMI323_FEATURE_ENGINE_START >> 8);
  SensorWriteBuffer[2] = (UINT8)(V_BMI323_FEATURE_ENGINE_START & 0xFF);
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 3);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensor16 (R_BMI323_FEATURE_IO2, V_BMI323_FEATURE_ENGINE_START);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  SensorWriteBuffer[0] = R_BMI323_FEATURE_IO_STATUS;
  SensorWriteBuffer[1] = (UINT8)(V_BMI323_FEATURE_IO_STATUS_WRITE >> 8);
  SensorWriteBuffer[2] = (UINT8)(V_BMI323_FEATURE_IO_STATUS_WRITE & 0xFF);
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 3);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensor16 (R_BMI323_FEATURE_IO_STATUS, V_BMI323_FEATURE_IO_STATUS_WRITE);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  SensorWriteBuffer[0] = R_BMI323_FEATURE_CTRL;
  SensorWriteBuffer[1] = (UINT8)(V_BMI323_FEATURE_CTRL_ENGINE_EN >> 8);
  SensorWriteBuffer[2] = (UINT8)(V_BMI323_FEATURE_CTRL_ENGINE_EN & 0xFF);
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 3);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensor16 (R_BMI323_FEATURE_CTRL, V_BMI323_FEATURE_CTRL_ENGINE_EN);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  // Mock Function for ReadSensorData - Wait for Feature Engine to Init
  Data[0] = 0;
  *SensorWriteBuffer = R_BMI323_SENSOR_STATUS;
  for (Index = 0; Index < 256; Index++) {
    if ((Data[0] & 0xF) == V_BMI323_FEATURE_ENGINE_INITIALZED) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, BufferSize);
        will_return (SerialIoI2cRead, Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_BMI323_SENSOR_STATUS, Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data[0] ++;
  }

  // Mock Function for WriteSensor16 - Symmetrical Spread of Area for Portrait and Landscape Orientation
  SensorWriteBuffer[0] = R_BMI323_ORIENT_1;
  SensorWriteBuffer[1] = (UINT8)(V_BMI323_ORIENT1_SYMMETRICAL >> 8);
  SensorWriteBuffer[2] = (UINT8)(V_BMI323_ORIENT1_SYMMETRICAL & 0xFF);
  expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
  expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
  expect_value (SerialIoI2cWrite, WriteLength, 3);
  will_return (SerialIoI2cWrite, SensorWriteBuffer);
  will_return (SerialIoI2cWrite, EFI_SUCCESS);

  Status = WriteSensor16 (R_BMI323_ORIENT_1, V_BMI323_ORIENT1_SYMMETRICAL);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Mock Function for ReadSensorData - Read Orientaion
  //
  // PORTRAIT_UPRIGHT
  Data[0] = 0;
  *SensorWriteBuffer = R_BMI323_SENSOR_STATUS;
  for (Index = 0; Index < 256; Index++) {
    if ((Data[0] & 0x3) == V_BMI323_PORTRAIT_UPRIGHT) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, BufferSize);
        will_return (SerialIoI2cRead, Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_BMI323_SENSOR_STATUS, Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data[0] ++;
  }

  // PORTRAIT_UPSIDE_DOWN
  Data[0] = 0;
  *SensorWriteBuffer = R_BMI323_SENSOR_STATUS;
  for (Index = 0; Index < 256; Index++) {
    if ((Data[0] & 0x3) == V_BMI323_PORTRAIT_UPSIDE_DOWN) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, 2);
        will_return (SerialIoI2cRead, Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_BMI323_SENSOR_STATUS, Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data[0] ++;
  }

  // LANDSCAPE_LEFT
  Data[0] = 0;
  *SensorWriteBuffer = R_BMI323_SENSOR_STATUS;
  for (Index = 0; Index < 256; Index++) {
    if ((Data[0] & 0x3) == V_BMI323_LANDSCAPE_LEFT) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, BufferSize);
        will_return (SerialIoI2cRead, Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_BMI323_SENSOR_STATUS, Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data[0] ++;
  }

  // LANDSCAPE_RIGHT
  Data[0] = 0;
  *SensorWriteBuffer = R_BMI323_SENSOR_STATUS;
  for (Index = 0; Index < 256; Index++) {
    if ((Data[0] & 0x3) == V_BMI323_LANDSCAPE_LEFT) {
        expect_value (SerialIoI2cWrite, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cWrite, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cWrite, WriteLength, 2);
        will_return (SerialIoI2cWrite, SensorWriteBuffer);
        will_return (SerialIoI2cWrite, EFI_SUCCESS);

        expect_value (SerialIoI2cRead, I2cBusNumber, OrientationI2CSensorBusNumber);
        expect_value (SerialIoI2cRead, TargetAddress, OrientationI2CSensorAddress);
        expect_value (SerialIoI2cRead, ReadLength, BufferSize);
        will_return (SerialIoI2cRead, Data);
        will_return (SerialIoI2cRead, EFI_SUCCESS);

        Status = ReadSensorData (R_BMI323_SENSOR_STATUS, Data, BufferSize);
        UT_ASSERT_NOT_EFI_ERROR (Status);
    }

    Data[0] ++;
  }

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}