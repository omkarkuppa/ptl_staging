/** @file
  This PEIM driver initialize BMI323 Orientation sensor

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

#include "I2cSensorBMI323.h"
#include "I2cSensorPei.h"

/**
  Function Call to read device orientation from BMI323 orientation sensor using I2C interface.

  @retval EFI_STATUS        Returns EFI Status

**/
EFI_STATUS
EFIAPI
ReadBmi323Orientation (
  VOID
  )
{
  UINT8       Data[2];
  UINT16      Value;
  UINT32      Ctr = 0;
  UINT8       Orientation;
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "Reading orientation from Bmi323 Sensor\n"));

  ZeroMem (Data, sizeof (Data));
  // Read sensor chip id
  Status = ReadSensorData (R_BMI323_CHIP_ID, &Data[0], 2);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Value = (Data[1] << 8) | Data[0];
  DEBUG ((DEBUG_INFO, "Value of R_BMI323_CHIP_ID:%x\n", Value));

  if (Value != V_BMI323_ID) {
    DEBUG ((DEBUG_ERROR, "Wrong BMI323 sensor chip id.\n"));
    return EFI_DEVICE_ERROR;
  }

  // Read device status
  ReadSensorData (R_BMI323_DEVICE_STATUS, &Data[0], 2);
  Value = (Data[1] << 8) | Data[0];
  DEBUG ((DEBUG_INFO, "Value of R_BMI323_DEVICE_STATUS:%x\n", Value));

  if ((Value & 0x1) != V_BMI323_DEVICE_POWER_OK) {
    DEBUG ((DEBUG_ERROR, "BMI323 sensor power error.\n"));
    return EFI_DEVICE_ERROR;
  }

  // Read Sensor status
  ReadSensorData (R_BMI323_SENSOR_STATUS, &Data[0], 2);
  Value = (Data[1] << 8) | Data[0];
  DEBUG ((DEBUG_INFO, "Value of R_BMI323_SENSOR_STATUS:%x\n", Value));

  if ((Value & 0x1) != V_BMI323_SENSOR_INIT_OK) {
    DEBUG ((DEBUG_ERROR, "BMI323 initialzation error.\n"));
    return EFI_DEVICE_ERROR;
  }

  //Enable feature engin
  WriteSensor16 (R_BMI323_FEATURE_IO2, V_BMI323_FEATURE_ENGINE_START);
  WriteSensor16 (R_BMI323_FEATURE_IO_STATUS, V_BMI323_FEATURE_IO_STATUS_WRITE);
  WriteSensor16 (R_BMI323_FEATURE_CTRL, V_BMI323_FEATURE_CTRL_ENGINE_EN);

  //wait for feature engine to init
  while (TRUE) {
    ReadSensorData (R_BMI323_FEATURE_IO1, &Data[0], 2);
    if ((Data[0] & 0xF) == V_BMI323_FEATURE_ENGINE_INITIALZED) {
      break;
    }
    if (Ctr > V_TIMEOUT) {
      DEBUG ((DEBUG_ERROR, "BMI323 Feature engine initialzation error.\n"));
      return EFI_DEVICE_ERROR;
    }
    MicroSecondDelay (5000);
    Ctr ++;
  }

  WriteSensor16 (R_BMI323_ORIENT_1, V_BMI323_ORIENT1_SYMMETRICAL); // Symmetrical spread of area for portrait and landscape orientation

  Status = ReadSensorData (R_BMI323_FEATURE_EVENT_EXT, &Data[0], 2); // Read orientaion
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Orientation = Data[0] & 0x3;

  if (Orientation == V_BMI323_PORTRAIT_UPRIGHT || Orientation == V_BMI323_PORTRAIT_UPSIDE_DOWN) {
    PcdSetBoolS (PcdI2cSensorDisplayRotationOn, FALSE);  // Portrait
  }
  else {
    PcdSetBoolS (PcdI2cSensorDisplayRotationOn, TRUE);   // Landscape
  }

  return EFI_SUCCESS;
}
