/** @file
  Header file for Serial Io Common Lib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _SERIAL_IO_ACCESS_LIB_H_
#define _SERIAL_IO_ACCESS_LIB_H_

/**
  Returns BAR0

  @param[in] PciCfgBase         Pci Config Base

  @retval    64bit MMIO BAR Address
**/
UINT64
GetSerialIoBar (
  IN UINT64        PciCfgBase
  );

/**
  Returns UART Pci Config Space

  @param[in] UartNumber         UART Number

  @retval    UART Pci Config Space Address
**/
UINT64
GetSerialIoUartPciCfg (
  IN UINT8        UartNumber
  );

/**
  Checks if Device with given PciDeviceId is one of SerialIo I2C controllers
  If yes, its number is returned through I2cIndex parameter, otherwise I2cIndex is not updated

  @param[in]  PciDevId                  Device ID
  @param[out] I2cNumber                 Number of SerialIo I2C controller

  @retval TRUE                          yes it is a SerialIo I2C controller
  @retval FALSE                         no it isn't a SerialIo I2C controller
**/
BOOLEAN
IsSerialIoI2cDeviceId (
  IN  UINT16    PciDevId,
  OUT UINT8     *I2cNumber
  );

/**
  Checks if I2c is Function 0 Enabled

  @param[in] I2cIndex                   Number of the SerialIo I2C controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoI2cFunction0Enabled (
  IN UINT8    I2cIndex
  );

/**
  Checks if Uart is Function 0 Enabled

  @param[in] UartIndex                   Number of the SerialIo Uart controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoUartFunction0Enabled (
  IN UINT8    UartIndex
  );

/**
  Checks if Spi is Function 0 Enabled

  @param[in] SpiIndex                   Number of the SerialIo Spi controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoSpiFunction0Enabled (
  IN UINT8    SpiIndex
  );

#endif // _SERIAL_IO_ACCESS_LIB_H_
