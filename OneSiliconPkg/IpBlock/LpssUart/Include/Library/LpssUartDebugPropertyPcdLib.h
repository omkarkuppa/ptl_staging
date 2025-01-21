/** @file

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

@par Specification Reference:
**/

#ifndef _LPSS_UART_DEBUG_PROPERTY_PCD_LIB_H_
#define _LPSS_UART_DEBUG_PROPERTY_PCD_LIB_H_

#include <LpssUartConfig.h>

/**
  Returns UART attributes

  @param[in,out] Attributes          UART Attributes
**/
VOID
LpssUartDebugPcdGetAttributes (
  IN OUT LPSS_UART_ATTRIBUTES  *Attributes
  );

/**
  Returns LPSS UART Controller Number used in Debug mode

  @retval  ControllerNumber   UART Controller Number
**/
UINT8
LpssUartDebugPcdGetControllerNumber (
  VOID
  );

/**
  Returns LPSS UART Debug Enable

  @retval  0 - Disabled
  @retval  1 - Enabled
  @retval  2 - Enabled without initializng
**/
UINT8
LpssUartDebugPcdGetDebugEnable (
  VOID
  );

/**
  Returns Lpss UART MMIO Base Address

  @retval  MMIO Base Address by default in PCI Mode
**/
UINT32
LpssUartDebugPcdGetPciDefaultMmioBase (
  VOID
  );

/**
  Returns Lpss UART PCI Config Base Address

  @retval  PCI Config Base Address for a Lpss Uart Controller
**/
UINT32
LpssUartDebugPcdGetDefaultPciCfgBase (
  VOID
  );

/**
  Returns LPSS UART PinMux Rx GPIO

  @param[in,out] Uart              UART Device Configuration
  @param[in,out] UartNumber        UART Device Number

**/
VOID
LpssUartDebugPcdGetDeviceConfig (
  IN OUT  LPSS_UART_DEVICE_CONFIG     *UartDeviceConfig,
  IN OUT  UINT8                       *UartNumber
  );

#endif //_LPSS_UART_DEBUG_PROPERTY_PCD_LIB_H_
