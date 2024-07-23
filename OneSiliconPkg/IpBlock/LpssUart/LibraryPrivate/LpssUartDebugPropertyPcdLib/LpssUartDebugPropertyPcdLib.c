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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Protocol/SerialIo.h>
#include <Library/DebugLib.h>
#include <LpssUartConfig.h>

/**
  Returns UART attributes

  @param[in,out] Attributes          UART Attributes
**/
VOID
LpssUartDebugPcdGetAttributes (
  IN OUT LPSS_UART_ATTRIBUTES  *Attributes
  )
{
  Attributes->BaudRate = (UINT64) PcdGet32 (PcdSerialIoUartBaudRate);
  Attributes->DataBits = PcdGet8 (PcdSerialIoUartDataBits);
  Attributes->StopBits = (EFI_STOP_BITS_TYPE) PcdGet8 (PcdSerialIoUartStopBits);
  Attributes->Parity   = (EFI_PARITY_TYPE) PcdGet8 (PcdSerialIoUartParity);
  Attributes->AutoFlow = PcdGet8 (PcdSerialIoUartAutoFlow);
}

/**
  Returns LPSS UART Controller Number used in Debug mode

  @retval  ControllerNumber   UART Controller Number
**/
UINT8
LpssUartDebugPcdGetControllerNumber (
  VOID
  )
{
  return PcdGet8 (PcdSerialIoUartNumber);
}

/**
  Returns LPSS UART Debug Enable

  @retval  0 - Disabled
  @retval  1 - Enabled
  @retval  2 - Enabled without initializng
**/
UINT8
LpssUartDebugPcdGetDebugEnable (
  VOID
  )
{
  return PcdGet8 (PcdSerialIoUartDebugEnable);
}

/**
  Returns Lpss UART MMIO Base Address

  @retval  MMIO Base Address by default in PCI Mode
**/
UINT32
LpssUartDebugPcdGetPciDefaultMmioBase (
  VOID
  )
{
  return PcdGet32 (PcdSerialIoUartDebugMmioBase);
}

/**
  Returns Lpss UART PCI Config Base Address

  @retval  PCI Config Base Address for a Lpss Uart Controller
**/
UINT32
LpssUartDebugPcdGetDefaultPciCfgBase (
  VOID
  )
{
  return PcdGet32 (PcdSerialIoUartDebugPciCfgBase);
}

/**
  Returns LPSS UART PinMux Rx GPIO

  @param[in,out] Uart              UART Device Configuration
  @param[in,out] UartNumber        UART Device Number

**/
VOID
LpssUartDebugPcdGetDeviceConfig (
  IN OUT  LPSS_UART_DEVICE_CONFIG     *UartDeviceConfig,
  IN OUT  UINT8                       *UartNumber
  )
{
  *UartNumber = LpssUartDebugPcdGetControllerNumber ();
  LpssUartDebugPcdGetAttributes (&UartDeviceConfig->Attributes);
  UartDeviceConfig->PinMux.Rx   = PcdGet32 (PcdSerialIoUartRxPinMux);
  UartDeviceConfig->PinMux.Tx   = PcdGet32 (PcdSerialIoUartTxPinMux);
  UartDeviceConfig->PinMux.Rts  = PcdGet32 (PcdSerialIoUartRtsPinMux);
  UartDeviceConfig->PinMux.Cts  = PcdGet32 (PcdSerialIoUartCtsPinMux);
  UartDeviceConfig->PowerGating = PcdGet8  (PcdSerialIoUartPowerGating);
  UartDeviceConfig->Mode        = PcdGet8  (PcdSerialIoUartMode);
}
