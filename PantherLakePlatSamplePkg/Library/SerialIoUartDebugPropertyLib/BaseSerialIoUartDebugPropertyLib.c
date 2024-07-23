/** @file

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

#include <ConfigBlock.h>
#include <SerialIoDevices.h>
#include <Library/SerialIoUartDebugPropertyPcdLib.h>

/**
  Returns UART's attributes

  @param[in,out]  UartAttributes   Uart Attributes
**/
VOID
SerialIoUartDebugGetAttributes (
  IN OUT SERIAL_IO_UART_ATTRIBUTES *UartAttributes
  )
{
  SerialIoUartDebugPcdGetAttributes (UartAttributes);
}

/**
  Returns Serial Io UART Controller Number used in Debug mode

  @retval  ControllerNumber   UART Controller Number
**/
UINT8
SerialIoUartDebugGetControllerNumber (
  VOID
  )
{
  return SerialIoUartDebugPcdGetControllerNumber ();
}

/**
  Returns Serial Io UART MMIO Base Address

  @retval  MMIO Base Address by default in PCI Mode
**/
UINT32
SerialIoUartDebugGetPciDefaultMmioBase (
  VOID
  )
{
  return SerialIoUartDebugPcdGetPciDefaultMmioBase ();
}

/**
  Returns Serial Io UART PCI Config Base

  @retval  PciCfgBase
**/
UINT32
SerialIoUartDebugGetDefaultPciCfgBase (
  VOID
  )
{
  return SerialIoUartDebugPcdGetDefaultPciCfgBase ();
}