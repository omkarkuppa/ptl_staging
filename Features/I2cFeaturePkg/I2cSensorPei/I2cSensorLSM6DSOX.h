/** @file
  This PEIM driver initialize I2C sensor

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


#ifndef _SENSOR_LSM6DSOX_H_
#define _SENSOR_LSM6DSOX_H_

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

//Sensor Registers
#define I2C_NUMBER                     0x03
#define R_LSM6DSL_CTRL3_C              0x12
#define B_LSM6DSL_CTRL3_REG_SW_RESET   BIT0
#define B_LSM6DSL_CTRL3_REG_IF_INC     BIT2
#define R_LSM6DSL_CTRL6_C              0x15
#define B_LSM6DSL_CTRL6_HP_DISABLE     BIT4
#define R_LSM6DSL_WHO_AM_I             0x0F
#define V_LSM6DSL_CHIP_ID              0x6A
#define R_LSM6DSL_CTRL1_XL             0x10
#define V_LSM6DSL_CTRL1_XL_WRITE       0x60
#define R_LSM6DSL_TAP_CFG              0x58
#define V_LSM6DSL_TAP_CFG_WRITE        0x80
#define R_LSM6DSL_MD1_CFG              0x5E
#define V_LSM6DSL_MD1_CFG_WRITE        0x04
#define R_LSM6DSL_TAP_THS_6D           0x59
#define V_LSM6DSL_TAP_THS_6D_WRITE     0xC0
#define R_LSM6DSL_CTRL8_XL             0x17
#define V_LSM6DSL_CTRL8_XL_WRITE       0x01
#define R_LSM6DSL_D6D_SRC              0x1D
#define B_LSM6DSL_D6D_SRC_XL           BIT0
#define B_LSM6DSL_D6D_SRC_XH           BIT1
#define B_LSM6DSL_D6D_SRC_YL           BIT2
#define B_LSM6DSL_D6D_SRC_YH           BIT3

#define V_N_TIMES                      11

/**
  Function Call to read device orientation from LSM6DSL orientation sensor using I2C interface.

  @retval EFI_STATUS        return EFI Status

**/

typedef
EFI_STATUS
(EFIAPI *READ_LSM6DSOX_ORIENTATION) (
  VOID
  );
  
/**
  Function Call to read device orientation from LSM6DSL orientation sensor using I2C interface.

  @retval EFI_STATUS        return EFI Status

**/

EFI_STATUS
EFIAPI
ReadLsm6dsoxOrientation (
  VOID
  );

#endif  // _SENSOR_LSM6DSOX_H_




