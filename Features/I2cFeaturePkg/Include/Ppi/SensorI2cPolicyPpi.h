/** @file
  The PEI_SENSOR_I2C_POLICY_PPI provides platform information to PEI SensorI2c PEIM.

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

@par Specification Reference:
**/

#ifndef _PEI_SENSOR_I2C_POLICY_PPI_H_
#define _PEI_SENSOR_I2C_POLICY_PPI_H_

/**
  Globally unique identifier for PEI platform policy PPI.
**/

#define PEI_SENSOR_I2C_POLICY_PPI_GUID \
{ \
  0x8bebe3c4, 0xf852, 0x4427, { 0x88, 0x28, 0xa3, 0x2b, 0x1f, 0xd6, 0x7e, 0xf0 } \
}

/**
Pre-declaration of PEI graphics platform policy PPI.
**/
typedef struct _PEI_SENSOR_I2C_POLICY_PPI PEI_SENSOR_I2C_POLICY_PPI;

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
  Function Call to read device orientation from BMI323 orientation sensor using I2C interface.

  @retval EFI_STATUS        Returns EFI Status

**/

typedef
EFI_STATUS
(EFIAPI *READ_BMI323_ORIENTATION) (
  VOID
  );

/**
  This defines the PEI Sensor I2c Policy PPI structure.
**/
struct _PEI_SENSOR_I2C_POLICY_PPI {
  READ_BMI323_ORIENTATION       ReadBmi323Orientation;          ///< Function Pointer for reading Bmi323O Orientation Data.
  READ_LSM6DSOX_ORIENTATION     ReadLsm6dsoxOrientation;       ///< Function Pointer for reading Lsm6dsox Orientation Data.
};

#endif
