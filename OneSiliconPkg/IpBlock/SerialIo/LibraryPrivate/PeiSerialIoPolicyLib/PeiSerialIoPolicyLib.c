/** @file
  This file provides services for SerialIo policy function

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
  Print SERIAL_IO_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
SerialIoPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  SERIAL_IO_CONFIG  *SerialIoConfig;
  UINT8             Index;
  UINT8             CsIndex;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ Serial IO Config ------------------\n"));

  for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
    DEBUG ((DEBUG_INFO, " SPI[%d].Mode                 = %d\n", Index, SerialIoConfig->SpiDeviceConfig[Index].Mode));
    DEBUG ((DEBUG_INFO, " SPI[%d].DefaultCsOutput      = %d\n", Index, SerialIoConfig->SpiDeviceConfig[Index].DefaultCsOutput));
    DEBUG ((DEBUG_INFO, " SPI[%d].CsMode               = %d\n", Index, SerialIoConfig->SpiDeviceConfig[Index].CsMode));
    DEBUG ((DEBUG_INFO, " SPI[%d].CsState              = %d\n", Index, SerialIoConfig->SpiDeviceConfig[Index].CsState));
    DEBUG ((DEBUG_INFO, " SPI[%d].PinMux.Clk           = 0x%x\n", Index, SerialIoConfig->SpiDeviceConfig[Index].PinMux.Clk));
    DEBUG ((DEBUG_INFO, " SPI[%d].PinMux.Miso          = 0x%x\n", Index, SerialIoConfig->SpiDeviceConfig[Index].PinMux.Miso));
    DEBUG ((DEBUG_INFO, " SPI[%d].PinMux.Mosi          = 0x%x\n", Index, SerialIoConfig->SpiDeviceConfig[Index].PinMux.Mosi));
    for (CsIndex = 0; CsIndex < GetPchMaxSerialIoSpiChipSelectsNum (); CsIndex++) {
      DEBUG ((DEBUG_INFO, " SPI[%d].CsEnable[%d]          = %d\n", Index, CsIndex, SerialIoConfig->SpiDeviceConfig[Index].CsEnable[CsIndex]));
      DEBUG ((DEBUG_INFO, " SPI[%d].CsPolarity[%d]        = %d\n", Index, CsIndex, SerialIoConfig->SpiDeviceConfig[Index].CsPolarity[CsIndex]));
      DEBUG ((DEBUG_INFO, " SPI[%d].PinMux.Cs[%d]         = 0x%x\n", Index, CsIndex, SerialIoConfig->SpiDeviceConfig[Index].PinMux.Cs[CsIndex]));
    }
  }

  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    DEBUG ((DEBUG_INFO, " UART[%d].Mode                = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].Mode));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.BaudRate = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].Attributes.BaudRate));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.Parity   = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].Attributes.Parity));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.DataBits = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].Attributes.DataBits));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.StopBits = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].Attributes.StopBits));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.AutoFlow = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].Attributes.AutoFlow));
    DEBUG ((DEBUG_INFO, " UART[%d].PinMux.Rx           = 0x%x\n", Index, SerialIoConfig->UartDeviceConfig[Index].PinMux.Rx));
    DEBUG ((DEBUG_INFO, " UART[%d].PinMux.Tx           = 0x%x\n", Index, SerialIoConfig->UartDeviceConfig[Index].PinMux.Tx));
    DEBUG ((DEBUG_INFO, " UART[%d].PinMux.Rts          = 0x%x\n", Index, SerialIoConfig->UartDeviceConfig[Index].PinMux.Rts));
    DEBUG ((DEBUG_INFO, " UART[%d].PinMux.Cts          = 0x%x\n", Index, SerialIoConfig->UartDeviceConfig[Index].PinMux.Cts));
    DEBUG ((DEBUG_INFO, " UART[%d].DBG2                = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].DBG2));
    DEBUG ((DEBUG_INFO, " UART[%d].PowerGating         = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].PowerGating));
    DEBUG ((DEBUG_INFO, " UART[%d].DmaEnable           = %d\n", Index, SerialIoConfig->UartDeviceConfig[Index].DmaEnable));
  }

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    DEBUG ((DEBUG_INFO, " I2C[%d].Mode                 = %d\n", Index, SerialIoConfig->I2cDeviceConfig[Index].Mode));
    DEBUG ((DEBUG_INFO, " I2C[%d].PinMux.Sda           = 0x%x\n", Index, SerialIoConfig->I2cDeviceConfig[Index].PinMux.Sda));
    DEBUG ((DEBUG_INFO, " I2C[%d].PinMux.Scl           = 0x%x\n", Index, SerialIoConfig->I2cDeviceConfig[Index].PinMux.Scl));
    DEBUG ((DEBUG_INFO, " I2C[%d].PadTermination       = %d\n", Index, SerialIoConfig->I2cDeviceConfig[Index].PadTermination));
  }

  for (Index = 0; Index < GetMaxI3cInterfacesNum (); Index++) {
    DEBUG ((DEBUG_INFO, "I3C[%d].Mode              = %d\n",   Index, SerialIoConfig->I3cDeviceConfig[Index].Mode));
    DEBUG ((DEBUG_INFO, "I3C[%d].Sda.PinMux        = 0x%x\n", Index, SerialIoConfig->I3cDeviceConfig[Index].Sda.PinMux));
    DEBUG ((DEBUG_INFO, "I3C[%d].Sda.Termination   = 0x%x\n", Index, SerialIoConfig->I3cDeviceConfig[Index].Sda.PadTermination));
    DEBUG ((DEBUG_INFO, "I3C[%d].Scl.PinMux        = 0x%x\n", Index, SerialIoConfig->I3cDeviceConfig[Index].Scl.PinMux));
    DEBUG ((DEBUG_INFO, "I3C[%d].Scl.Termination   = 0x%x\n", Index, SerialIoConfig->I3cDeviceConfig[Index].Scl.PadTermination));
    DEBUG ((DEBUG_INFO, "I3C[%d].Sclfb.PinMux      = 0x%x\n", Index, SerialIoConfig->I3cDeviceConfig[Index].SclFb.PinMux));
    DEBUG ((DEBUG_INFO, "I3C[%d].Sclfb.Termination = 0x%x\n", Index, SerialIoConfig->I3cDeviceConfig[Index].SclFb.PadTermination));
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
SerialIoLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT8                  Index;
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
  }

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    SerialIoConfig->I2cDeviceConfig[Index].Mode = SerialIoI2cDisabled;
  }

  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    SerialIoConfig->UartDeviceConfig[Index].Mode                = SerialIoUartDisabled;
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
  SerialIoLoadConfigDefault
};

/**
  Get SerialIo config block table size.

  @retval      Size of config block
**/
UINT16
SerialIoGetConfigBlockTotalSize (
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
SerialIoAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mSerialIoBlocks, 1);
}
