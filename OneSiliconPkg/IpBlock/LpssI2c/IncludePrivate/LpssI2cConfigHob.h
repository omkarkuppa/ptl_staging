/** @file
  LPSS I2C config HOB definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _LPSS_I2C_CONFIG_HOB_H_
#define _LPSS_I2C_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>
#include <LpssI2cConfig.h>

/**
  The LPSS_I2C_CONFIG_HOB describes settings for PCH.
**/
typedef struct {
  LPSS_I2C_CONTROLLER_CONFIG     I2cDeviceConfig[PCH_MAX_SERIALIO_I2C_CONTROLLERS];   ///< I2C Device Configuration
} LPSS_I2C_CONFIG_HOB;

#endif
