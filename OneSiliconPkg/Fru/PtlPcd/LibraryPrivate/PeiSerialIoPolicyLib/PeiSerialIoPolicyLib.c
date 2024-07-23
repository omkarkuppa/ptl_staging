/** @file
  This file provides services for SerialIo policy function

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/SerialIoUartDebugPropertyPcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SiPolicy.h>
#include <SerialIoConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PtlPcdSerialIoLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT8                  Index;
  UINT8                  CsIndex;
  SERIAL_IO_CONFIG       *SerialIoConfig;
  SERIAL_IO_UART_CONFIG  UartDeviceConfig;
  UINT8                  SerialIoUartNumber;

  ZeroMem (&UartDeviceConfig, sizeof (SERIAL_IO_UART_CONFIG));
  SerialIoConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SerialIoConfig->Header.GuidHob.Name = %g\n", &SerialIoConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SerialIoConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SerialIoConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
    SerialIoConfig->SpiDeviceConfig[Index].Mode        = SerialIoSpiDisabled;
    SerialIoConfig->SpiDeviceConfig[Index].CsEnable[0] = 1;
    for (CsIndex = 0; CsIndex < PCH_MAX_SERIALIO_SPI_CHIP_SELECTS; CsIndex++) {
      SerialIoConfig->SpiDeviceConfig[Index].CsPolarity[CsIndex] = 1;
    }
  }

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    SerialIoConfig->I2cDeviceConfig[Index].Mode = SerialIoI2cPci;
  }

  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    if (Index == 0) {
      SerialIoConfig->UartDeviceConfig[Index].Mode                = SerialIoUartHidden;
    } else {
      SerialIoConfig->UartDeviceConfig[Index].Mode                = SerialIoUartDisabled;
    }
    SerialIoConfig->UartDeviceConfig[Index].DBG2                = FALSE;
    SerialIoConfig->UartDeviceConfig[Index].DmaEnable           = TRUE;
    SerialIoConfig->UartDeviceConfig[Index].Attributes.BaudRate = 115200;
    SerialIoConfig->UartDeviceConfig[Index].Attributes.Parity   = 1;
    SerialIoConfig->UartDeviceConfig[Index].Attributes.DataBits = 8;
    SerialIoConfig->UartDeviceConfig[Index].Attributes.StopBits = 1;
    SerialIoConfig->UartDeviceConfig[Index].Attributes.AutoFlow = 0;
    SerialIoConfig->UartDeviceConfig[Index].PowerGating         = SerialIoUartPgAuto;
  }

  if (SerialIoUartDebugPcdGetDebugEnable () > 0) {
    SerialIoUartDebugPcdGetDeviceConfig (&UartDeviceConfig, &SerialIoUartNumber);
    SerialIoConfig->UartDeviceConfig[SerialIoUartNumber].Mode                = UartDeviceConfig.Mode;
    SerialIoConfig->UartDeviceConfig[SerialIoUartNumber].Attributes.BaudRate = UartDeviceConfig.Attributes.BaudRate;
    SerialIoConfig->UartDeviceConfig[SerialIoUartNumber].Attributes.Parity   = UartDeviceConfig.Attributes.Parity;
    SerialIoConfig->UartDeviceConfig[SerialIoUartNumber].Attributes.DataBits = UartDeviceConfig.Attributes.DataBits;
    SerialIoConfig->UartDeviceConfig[SerialIoUartNumber].Attributes.StopBits = UartDeviceConfig.Attributes.StopBits;
    SerialIoConfig->UartDeviceConfig[SerialIoUartNumber].Attributes.AutoFlow = UartDeviceConfig.Attributes.AutoFlow;
  }

  for (Index = 0; Index < GetMaxI3cInterfacesNum (); Index++) {
    SerialIoConfig->I3cDeviceConfig[Index].Mode                 = SerialIoI3cDisabled;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mSerialIoBlocks = {
  &gSerialIoConfigGuid,
  sizeof (SERIAL_IO_CONFIG),
  SERIAL_IO_CONFIG_REVISION,
  PtlPcdSerialIoLoadConfigDefault
};

/**
  Get SerialIo config block table size.

  @retval      Size of config block
**/
UINT16
PtlPcdSerialIoGetConfigBlockTotalSize (
  VOID
  )
{
  return mSerialIoBlocks.Size;
}

/**
  Add SerialIo ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PtlPcdSerialIoAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mSerialIoBlocks, 1);
}