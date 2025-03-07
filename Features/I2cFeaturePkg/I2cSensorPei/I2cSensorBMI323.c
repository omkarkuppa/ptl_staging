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
#include "I2cGopConfig.h"
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
  UINT8       Data[4];
  UINT16      Value;
  UINT32      Ctr = 0;
  UINT8       Orientation;
  EFI_STATUS  Status;
  EFI_HOB_GUID_TYPE              *GuidHob;
  GOP_CONFIG_DRIVER_HOB  *GopConfigDriverHob;

  DEBUG ((DEBUG_INFO, "Reading orientation from Bmi323 Sensor\n"));
  
  GuidHob = GetFirstGuidHob (&gGopConfigDriverHobGuid);
  if (GuidHob != NULL) {
    GopConfigDriverHob = (GOP_CONFIG_DRIVER_HOB *) GET_GUID_HOB_DATA (GuidHob);
  }else{
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "Reading orientation from Bmi323 Sensor\n"));

  ZeroMem (Data, sizeof (Data));

  // Reset sensor
  WriteSensor16 (R_BMI323_CFG_RES, V_BMI323_CMD_SOFT_REST);
  MicroSecondDelay (5000);

  // Read sensor chip id
  Status = ReadSensorData (R_BMI323_CHIP_ID, &Data[0], 4);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Value =  Data[2];
  DEBUG ((DEBUG_INFO, "Value of R_BMI323_CHIP_ID:%x\n", Value));

  if (Value != V_BMI323_ID) {
    DEBUG ((DEBUG_ERROR, "Wrong BMI323 sensor chip id.\n"));
    return EFI_DEVICE_ERROR;
  }

  // Read device status
  ZeroMem (Data, sizeof (Data));
  ReadSensorData (R_BMI323_DEVICE_STATUS, &Data[0], 4);
  Value = (Data[3] << 8) | Data[2];
  DEBUG ((DEBUG_INFO, "Value of R_BMI323_DEVICE_STATUS:%x\n", Value));

  if ((Value & 0x1) != V_BMI323_DEVICE_POWER_OK) {
    DEBUG ((DEBUG_ERROR, "BMI323 sensor power error.\n"));
    return EFI_DEVICE_ERROR;
  }

  // Read Sensor status
  ZeroMem (Data, sizeof (Data));
  ReadSensorData (R_BMI323_SENSOR_STATUS, &Data[0], 4);
  Value = (Data[3] << 8) | Data[2];
  DEBUG ((DEBUG_INFO, "Value of R_BMI323_SENSOR_STATUS:%x\n", Value));

  if ((Value & 0x1) != V_BMI323_SENSOR_INIT_OK) {
    DEBUG ((DEBUG_ERROR, "BMI323 initialization error.\n"));
    return EFI_DEVICE_ERROR;
  }

  //Enable feature engin
  WriteSensor16 (R_BMI323_FEATURE_IO2, V_BMI323_FEATURE_ENGINE_START);
  WriteSensor16 (R_BMI323_FEATURE_IO_STATUS, V_BMI323_FEATURE_IO_STATUS_WRITE);
  WriteSensor16 (R_BMI323_FEATURE_CTRL, V_BMI323_FEATURE_CTRL_ENGINE_EN);

  //wait for feature engine to init
  while (TRUE) {
    ReadSensorData (R_BMI323_FEATURE_IO1, &Data[0], 4);
    if ((Data[2] & 0xF) == V_BMI323_FEATURE_ENGINE_INITIALZED) {
      break;
    }
    if (Ctr > V_TIMEOUT) {
      DEBUG ((DEBUG_ERROR, "BMI323 Feature engine initialization error.\n"));
      return EFI_DEVICE_ERROR;
    }
    MicroSecondDelay (5000);
    Ctr ++;
  }

  //Acc config
  WriteSensor16 (R_BMI323_ACC_CONF, V_BMI323_ACC_NOR_MODE); // Config accelerometer with normal mode parameters
  ReadSensorData (R_BMI323_ACC_CONF, &Data[0], 4);
  Value = (Data[3] << 8) | Data[2];
  DEBUG ((DEBUG_INFO, "Acc config:%x\n", Value));
  //gyro config
  WriteSensor16 (R_BMI323_GYR_CONF, V_BMI323_GYR_NOR_MODE); // Config gyroscope with normal mode parameters
  ReadSensorData (R_BMI323_GYR_CONF, &Data[0], 4);
  Value = (Data[3] << 8) | Data[2];
  DEBUG ((DEBUG_INFO, "Gyro config:%x\n", Value));
  //Orientation config
  WriteSensor16 (R_BMI323_FEATURE_DATA_ADDR, R_BMI323_ORIENT_1);
  WriteSensor16 (R_BMI323_FEATURE_DATA_TX, V_BMI323_ORIENT1_SYMMETRICAL); // Symmetrical spread of area for portrait and landscape orientation
  WriteSensor16 (R_BMI323_FEATURE_DATA_ADDR, R_BMI323_ORIENT_1);
  ReadSensorData (R_BMI323_FEATURE_DATA_TX, &Data[0], 4);
  Value = (Data[3] << 8) | Data[2];
  DEBUG ((DEBUG_INFO, "ORIENT_1 config:%x\n", Value));
  
  //Orientation Feature enable
  ReadSensorData (R_BMI323_FEATURE_IO0, &Data[0], 4);
  Value = (Data[3] << 8) | Data[2];
  WriteSensor16 (R_BMI323_FEATURE_IO0, 0); // Clear register before setting
  WriteSensor16 (R_BMI323_FEATURE_IO0, Value | V_BMI323_FEATURE_ORIENT_EN); // Enable orientation detection
  WriteSensor16 (R_BMI323_FEATURE_IO_STATUS, V_BMI323_FEATURE_IO_STATUS_WRITE);
  ReadSensorData (R_BMI323_FEATURE_IO0, &Data[0], 4);
  Value = (Data[3] << 8) | Data[2];
  DEBUG ((DEBUG_INFO, "Feature_io0:%x\n", Value));

  //Set Int1 for Orientation
  WriteSensor16 (R_BMI323_INT_MAP1, V_BMI323_INT_MAP1_ORIENT_OUT);
  Ctr = 0;
  do {
    ReadSensorData (R_BMI323_INT_STATUS_INT1, &Data[0], 4);
    Value = (Data[3] << 8) | Data[2];
    DEBUG ((DEBUG_INFO, "INT1:%x\n", Value));
    if (Value != 0) {
      Status = ReadSensorData (R_BMI323_FEATURE_EVENT_EXT, &Data[0], 4); // Read orientation
      if (EFI_ERROR (Status)) {
        return Status;
      }
      Orientation = Data[2] & V_BMI323_LANDSCAPE_RIGHT;
      DEBUG ((DEBUG_INFO, "Orientation:%x\n", Orientation));
      if (Orientation == V_BMI323_PORTRAIT_UPRIGHT || Orientation == V_BMI323_PORTRAIT_UPSIDE_DOWN) {
        //Pass Software Id to GOP
        GopConfigDriverHob->SoftwareId = V_PORTRAIT;
      }
      else {
        //Pass Software Id to GOP
        GopConfigDriverHob->SoftwareId = V_LANDSCAPE;
      }
      break;
    }
    if (Ctr > V_TIMEOUT) {
      DEBUG ((DEBUG_ERROR, "BMI323 does not got orientation data from sensor.\n"));
      return EFI_DEVICE_ERROR;
    }
    MicroSecondDelay (200000);
    Ctr ++;
  } while (TRUE);

  return EFI_SUCCESS;
}
