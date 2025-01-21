/** @file
  This PEIM driver initialize LSM6DSOX Orientation sensor

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

#include "I2cSensorLSM6DSOX.h"
#include "I2cSensorPei.h"

/**
  Function Call to execute the Sensor confirguration using I2C interface.

  @retval EFI_SUCCESS        The operation succeeds.

**/
EFI_STATUS
EFIAPI
ReadLsm6dsoxOrientation (
  VOID
  )
{
  UINT8        Data;
  EFI_STATUS   Status;

  DEBUG ((DEBUG_INFO, "Reading orientation from LSM6DSOX Sensor\n"));

  Status = ReadSensorData (R_LSM6DSL_WHO_AM_I, &Data, 1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Data != V_LSM6DSL_CHIP_ID) {
    return EFI_DEVICE_ERROR;
  }

  Data = B_LSM6DSL_CTRL3_REG_SW_RESET;
  WriteSensorByte (R_LSM6DSL_CTRL3_C, Data);

  //Delay 2ms
  MicroSecondDelay (2000);

  ReadSensorData (R_LSM6DSL_CTRL3_C, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_CTRL3_C:%x\n", Data));

  Data = B_LSM6DSL_CTRL3_REG_IF_INC;
  WriteSensorByte (R_LSM6DSL_CTRL3_C, Data);
  ReadSensorData (R_LSM6DSL_CTRL3_C, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_CTRL3_C:%x\n", Data));

  ReadSensorData (R_LSM6DSL_CTRL6_C, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_CTRL6_C:%x\n", Data));
  Data = Data | B_LSM6DSL_CTRL6_HP_DISABLE;
  WriteSensorByte (R_LSM6DSL_CTRL6_C, Data);
  ReadSensorData (R_LSM6DSL_CTRL6_C, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_CTRL6_C:%x\n", Data));

  //Write 60h to R_CTRL1_XL (10h)            // ODR_XL = 416 Hz, FS_XL = +/- 2g
  Data = V_LSM6DSL_CTRL1_XL_WRITE;
  WriteSensorByte (R_LSM6DSL_CTRL1_XL, Data);
  ReadSensorData (R_LSM6DSL_CTRL1_XL, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_CTRL1_XL:%x\n", Data));

  //Write 80h to R_TAP_CFG (58h)             // Enable interrupts
  Data = V_LSM6DSL_TAP_CFG_WRITE;
  WriteSensorByte (R_LSM6DSL_TAP_CFG, Data);
  ReadSensorData (R_LSM6DSL_TAP_CFG, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_TAP_CFG:%x\n", Data));

  //Write 40h to R_TAP_THS_6D (59h)         // Enable 4D orientation detection with threshold of 60 degrees
  Data = V_LSM6DSL_TAP_THS_6D_WRITE;
  WriteSensorByte (R_LSM6DSL_TAP_THS_6D, Data);
  ReadSensorData (R_LSM6DSL_TAP_THS_6D, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_TAP_THS_6D:%x\n", Data));


  //Write 01h to R_CTRL8_XL (17h)            // Enable LPF2 filter to 6D functionality
  Data = V_LSM6DSL_CTRL8_XL_WRITE;
  WriteSensorByte (R_LSM6DSL_CTRL8_XL, Data);
  ReadSensorData (R_LSM6DSL_CTRL8_XL, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_CTRL8_XL:%x\n", Data));


  //Write 04h to MD1_CFG (5Eh)            // Disable routing of 6D event
  Data = V_LSM6DSL_MD1_CFG_WRITE;
  WriteSensorByte (R_LSM6DSL_MD1_CFG, Data);
  ReadSensorData (R_LSM6DSL_MD1_CFG, &Data, 1);
  DEBUG ((DEBUG_INFO, "value of R_LSM6DSL_MD1_CFG:%x\n", Data));

  //Read device orientation
  Status = ReadSensorData (R_LSM6DSL_D6D_SRC, &Data, 1);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "R_LSM6DSL_D6D_SRC = 0x%x\n", Data));
  if ((Data & B_LSM6DSL_D6D_SRC_XL) || (Data & B_LSM6DSL_D6D_SRC_XH)) {
    PcdSetBoolS (PcdI2cSensorDisplayRotationOn, TRUE);   // Landscape
  }
  else {
    PcdSetBoolS (PcdI2cSensorDisplayRotationOn, FALSE);  // Portrait
  }

  return EFI_SUCCESS;
}
