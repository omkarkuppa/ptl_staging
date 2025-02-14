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

#ifndef _SENSOR_BMI323_H_
#define _SENSOR_BMI323_H_

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>


#define R_BMI323_CHIP_ID                    0x0
#define V_BMI323_ID                         0x43
#define R_BMI323_DEVICE_STATUS              0x1
#define V_BMI323_DEVICE_POWER_OK            0x0
#define R_BMI323_SENSOR_STATUS              0x2
#define V_BMI323_SENSOR_INIT_OK             0x1
#define R_BMI323_INT_STATUS_INT1            0x0D
#define R_BMI323_FEATURE_IO0                0x10
#define R_BMI323_FEATURE_IO1                0x11
#define R_BMI323_FEATURE_IO2                0x12
#define R_BMI323_FEATURE_IO_STATUS          0x14
#define R_BMI323_ORIENT_1                   0x1C
#define R_BMI323_ACC_CONF                   0x20
#define R_BMI323_GYR_CONF                   0x21
#define R_BMI323_INT_MAP1                   0x3A
#define R_BMI323_FEATURE_CTRL               0x40
#define R_BMI323_FEATURE_DATA_ADDR          0x41
#define R_BMI323_FEATURE_DATA_TX            0x42
#define R_BMI323_FEATURE_EVENT_EXT          0x47
#define R_BMI323_CFG_RES                    0x7E

#define V_BMI323_PORTRAIT_UPRIGHT           0x0
#define V_BMI323_LANDSCAPE_LEFT             0x1
#define V_BMI323_PORTRAIT_UPSIDE_DOWN       0x2
#define V_BMI323_LANDSCAPE_RIGHT            0x3

#define V_BMI323_ORIENT1_SYMMETRICAL        0x2CE0
#define V_BMI323_FEATURE_ENGINE_START       0x012C
#define V_BMI323_FEATURE_ENGINE_INITIALZED  0x1
#define V_BMI323_FEATURE_IO_STATUS_WRITE    0x1
#define V_BMI323_FEATURE_CTRL_ENGINE_EN     0x1
#define V_BMI323_FEATURE_ORIENT_EN          0x80
#define V_BMI323_CMD_SOFT_REST              0xDEAF
#define V_BMI323_ACC_NOR_MODE               0x4027
#define V_BMI323_GYR_NOR_MODE               0x404B
#define V_BMI323_INT_MAP1_ORIENT_OUT        0x40

#define V_TIMEOUT                           10

/**
  Function Call to read device orientation from BMI323 orientation sensor using I2C interface.

  @retval EFI_STATUS        Returns EFI Status

**/

typedef
EFI_STATUS
(EFIAPI *READ_BMI323_ORIENTATION) (
  VOID
  );

/**
  Function Call to read device orientation from BMI323 orientation sensor using I2C interface.

  @retval EFI_STATUS        Returns EFI Status

**/
EFI_STATUS
EFIAPI
ReadBmi323Orientation (
  VOID
  );

#endif  // _SENSOR_BMI323_H_
