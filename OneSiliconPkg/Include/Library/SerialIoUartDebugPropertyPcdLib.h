/** @file

  SerialIo Uart Debug Property Pcd Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _SERAIL_IO_DEBUG_PROPERTY_PCD_LIB_H_
#define _SERAIL_IO_DEBUG_PROPERTY_PCD_LIB_H_

#include <SerialIoDevices.h>

/**
  Returns UART attributes

  @param[in,out] Attributes          UART Attributes
**/
VOID
SerialIoUartDebugPcdGetAttributes (
  IN OUT SERIAL_IO_UART_ATTRIBUTES  *Attributes
  );

/**
  Returns Serial Io UART Controller Number used in Debug mode

  @retval  ControllerNumber   UART Controller Number
**/
UINT8
SerialIoUartDebugPcdGetControllerNumber (
  VOID
  );

/**
  Returns Serial Io UART Debug Enable

  @retval  0 - Disabled
  @retval  1 - Enabled
  @retval  2 - Enabled without initializng
**/
UINT8
SerialIoUartDebugPcdGetDebugEnable (
  VOID
  );

/**
  Returns Serial Io UART MMIO Base Address

  @retval  MMIO Base Address by default in PCI Mode
**/
UINT32
SerialIoUartDebugPcdGetPciDefaultMmioBase (
  VOID
  );

/**
  Returns Serial Io UART PCI Config Base Address

  @retval  PCI Config Base Address for a Serial IO Uart Controller
**/
UINT32
SerialIoUartDebugPcdGetDefaultPciCfgBase (
  VOID
  );

/**
  Returns Serial Io UART PinMux Rx GPIO

  @param[in,out] Uart              UART Device Configuration
  @param[in,out] UartNumber        UART Device Number

**/
VOID
SerialIoUartDebugPcdGetDeviceConfig (
  IN OUT  SERIAL_IO_UART_CONFIG   *Uart,
  IN OUT  UINT8                   *UartNumber
  );

#endif //_SERAIL_IO_DEBUG_PROPERTY_PCD_LIB_H_
