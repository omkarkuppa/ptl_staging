/** @file
  GPIO Initialization Library. 
  This library contains functions to program GPIO in SEC  phase.

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

@par Specification Reference:
**/

#ifndef _PTL_PCD_GPIO_INIT_LIB_H_
#define _PTL_PCD_GPIO_INIT_LIB_H_

#include <LpssUartConfig.h>
#include <LpssI2cConfig.h>
#include <LpssSpiConfig.h>

/**
  Configures GPIO for each LPSS UART Controller

  @param[in] UartDeviceConfig   LPSS UART Config
**/
VOID
EFIAPI
PtlPcdPreMemLpssUartGpioConfigure (
  IN UINT8                       UartInstance,
  IN LPSS_UART_DEVICE_CONFIG     *UartDeviceConfig
  );

/**
  Configures GPIO for each LPSS I2C Controller

  @param[in] I2cDeviceConfig   LPSS I2C Config
**/
VOID
EFIAPI
PtlPcdPreMemLpssI2cGpioConfigure (
  IN UINT8                       I2cInstance,
  IN LPSS_I2C_CONTROLLER_CONFIG  *I2cDeviceConfig
  );

/**
  Configures GPIO for each LPSS SPI Controller

**/
VOID
EFIAPI
PtlPcdPreMemLpssSpiGpioConfigure (
  IN UINT8                     SpiInstance,
  IN LPSS_SPI_DEVICE_CONFIG    *SpiDeviceConfig
  );

#endif //_PTL_PCD_GPIO_INIT_LIB_H_
