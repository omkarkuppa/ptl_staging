/** @file
  Lpss I2C Handle definition

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

#ifndef _LPSS_I2C_HANDLE_H_
#define _LPSS_I2C_HANDLE_H_

#include <ConfigBlock.h>
#include <Ppi/SiPolicy.h>
#include <LpssI2cController.h>
#include <LpssI2cConfig.h>

#include <Library/PchLimits.h>

typedef struct _LPSS_I2C_HANDLE LPSS_I2C_HANDLE;
/**
  Internal I2C policy options
**/
typedef struct {
  UINT8      InterruptPin;
  UINT8      Irq;
} LPSS_I2C_PRIVATE_CONFIG;

/**
  I2C Handle structure
  Stores all data necessary to initialize LPSS I2C IP block
**/
struct _LPSS_I2C_HANDLE {
  LPSS_I2C_CONTROLLER            *Controller;
  LPSS_I2C_CONTROLLER_CONFIG     *I2cConfig;
  LPSS_I2C_PRIVATE_CONFIG        PrivateConfig;
  UINT8                          ControllerId;
};

#endif //_LPSS_I2C_HANDLE_H_
