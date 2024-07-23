/** @file
  Serial Io GPIO Access Library. This library contains functions to program
  Serial Io GPIO in SEC and PEI phase.

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

#ifndef _PTL_PCD_SERIAL_IO_GPIO_ACCESS_LIB_H_
#define _PTL_PCD_SERIAL_IO_GPIO_ACCESS_LIB_H_

#include <SerialIoDevices.h>
#include <LpssI3cConfig.h>
#include <LpssI2cConfig.h>

/**
  Configures GPIO for each Serial IO UART Controller

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   SerialIo UART Config
**/
VOID
EFIAPI
SerialIoUartGpioEnable (
  IN UINT8                  UartNumber,
  IN SERIAL_IO_UART_CONFIG  *UartDeviceConfig
  );

/**
  Configures GPIO for each Lpss IO I2C Controller

  @param[in] I2cNumber         I2C Number
  @param[in] I2cDeviceConfig   Lpss I2C Config
**/
VOID
EFIAPI
SerialIoI2cGpioEnable (
  IN UINT8                        I2cNumber,
  IN LPSS_I2C_CONTROLLER_CONFIG  *I2cDeviceConfig
  );

/**
  Configures GPIO for each Serial IO SPI Controller

  @param[in] SpiNumber         SPI Number
  @param[in] SpiDeviceConfig   SerialIo SPI Config
**/
VOID
EFIAPI
SerialIoSpiGpioEnable (
  IN UINT8                  SpiNumber,
  IN SERIAL_IO_SPI_CONFIG   *SpiDeviceConfig
  );

/**
  Configures GPIO for each Lpss I3C Controller

  @param[in] I3cNumber         I3C Number
  @param[in] I3cDeviceConfig   Lpss I3C Config
**/
VOID
SerialIoI3cGpioEnable (
  IN UINT8                       I3cNumber,
  IN LPSS_I3C_DEVICE_CONFIG      *I3cDeviceConfig
  );
#endif //_PTL_PCD_SERIAL_IO_GPIO_ACCESS_LIB_H_
