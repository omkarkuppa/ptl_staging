/** @file
  PTL SOC SerialIo Gpio Access library.
  All function in this library is available for PEI.

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
#include <Library/DebugLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/PcdLib.h>
#include <GpioV2Functionalities.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <GpioV2Services.h>
#include <Register/GpioAcpiDefines.h>
#include <GpioV2Pad.h>
#include <GpioV2Signals.h>
#include <Register/GpioAcpiDefines.h>
#include <SerialIoDevices.h>
#include <Library/GpioHelpersLib.h>
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
  )
{
  GPIOV2_SERVICES*   GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - failed to get GpioV2Access\n", __FUNCTION__));
    return;
  }

  // RX
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_UART_RX (UartNumber), UartDeviceConfig->PinMux.Rx);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_UART_RX (UartNumber), UartDeviceConfig->PinMux.Rx), GpioV2InputInversionDisable);

  // TX
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_UART_TX (UartNumber), UartDeviceConfig->PinMux.Tx);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_UART_TX (UartNumber), UartDeviceConfig->PinMux.Tx), GpioV2InputInversionDisable);

  if (UartDeviceConfig->Attributes.AutoFlow) {
    // RTS
    PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_UART_RTS (UartNumber), UartDeviceConfig->PinMux.Rts);
    GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_UART_RTS (UartNumber), UartDeviceConfig->PinMux.Rts), GpioV2InputInversionDisable);
    // CTS
    PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_UART_CTS (UartNumber), UartDeviceConfig->PinMux.Cts);
    GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_UART_CTS (UartNumber), UartDeviceConfig->PinMux.Cts), GpioV2InputInversionDisable);
  }
}

/**
  Configures GPIO for each Lpss IO I2C Controller

  @param[in] I2cNumber         I2C Number
  @param[in] I2cDeviceConfig   Lpss I2C Config
**/
VOID
EFIAPI
SerialIoI2cGpioEnable (
  IN UINT8                       I2cNumber,
  IN LPSS_I2C_CONTROLLER_CONFIG  *I2cDeviceConfig
  )
{
  GPIOV2_SERVICES*   GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    return;
  }
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - failed to get GpioV2Access\n", __FUNCTION__));
    return;
  }

  switch (I2cDeviceConfig->PadTermination) {
    case GpioV2TermDefault:
    case GpioV2TermNone:
    case GpioV2TermWpu1K:
    case GpioV2TermWpu5K:
    case GpioV2TermWpu20K:
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Gpio Pad Termination must be set as Default, None, None with 1v8 tolerance, 1k, 2k, 5k or 20k WPU\n"));
      ASSERT (FALSE);
  }

  //
  // SCL
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_I2C_SCL (I2cNumber), I2cDeviceConfig->PinMux.Scl);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I2C_SCL (I2cNumber), I2cDeviceConfig->PinMux.Scl), GpioV2InputInversionDisable);
  GpioServices->SetTerminationConfig (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I2C_SCL (I2cNumber), I2cDeviceConfig->PinMux.Scl), I2cDeviceConfig->PadTermination);

  //
  // SDA
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_I2C_SDA (I2cNumber), I2cDeviceConfig->PinMux.Sda);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I2C_SDA (I2cNumber), I2cDeviceConfig->PinMux.Sda), GpioV2InputInversionDisable);
  GpioServices->SetTerminationConfig (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I2C_SDA (I2cNumber), I2cDeviceConfig->PinMux.Sda), I2cDeviceConfig->PadTermination);
}


/**
  Configures GPIO for each Serial IO SPI Controller

  @param[in] SpiNumber         SPI Number
  @param[in] SpiDeviceConfig   SerialIo SPI Config
**/
VOID
EFIAPI
SerialIoSpiGpioEnable (
  IN UINT8                  SpiNumber,
  IN SERIAL_IO_SPI_CONFIG*  SpiDeviceConfig
)
{
  GPIOV2_SERVICES*   GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - failed to get GpioV2Access\n", __FUNCTION__));
    return;
  }

  //
  // Mosi
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_MOSI (SpiNumber), SpiDeviceConfig->PinMux.Mosi);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_MOSI (SpiNumber), SpiDeviceConfig->PinMux.Mosi), GpioV2InputInversionDisable);

  //
  // Miso
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_MISO (SpiNumber), SpiDeviceConfig->PinMux.Miso);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_MISO (SpiNumber), SpiDeviceConfig->PinMux.Miso), GpioV2InputInversionDisable);

  //
  // Clk
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_CLK (SpiNumber), SpiDeviceConfig->PinMux.Clk);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_CLK (SpiNumber), SpiDeviceConfig->PinMux.Clk), GpioV2InputInversionDisable);

  //
  //  Serial Io SPI on PTL Supports only 1 Chip Selects
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_CS (SpiNumber), SpiDeviceConfig->PinMux.Cs[0]);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_CS (SpiNumber), SpiDeviceConfig->PinMux.Cs[0]), GpioV2InputInversionDisable);
}

/**
  Configures GPIO for each Lpss I3C Controller

  @param[in] I3cNumber         I3C Number
  @param[in] I3cDeviceConfig   I3C Config
**/
VOID
SerialIoI3cGpioEnable (
  IN UINT8                  I3cNumber,
  IN LPSS_I3C_DEVICE_CONFIG      *I3cDeviceConfig
  )
{
  GPIOV2_SERVICES*   GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - failed to get GpioV2Access\n", __FUNCTION__));
    return;
  }

  //
  // SDA
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SDA (I3cNumber), I3cDeviceConfig->Sda.PinMux);
  GpioServices->SetTerminationConfig (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SDA (I3cNumber), I3cDeviceConfig->Sda.PinMux), I3cDeviceConfig->Sda.PadTermination);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SDA (I3cNumber), I3cDeviceConfig->Sda.PinMux), GpioV2InputInversionDisable);

  //
  // SCL
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SCL (I3cNumber), I3cDeviceConfig->Scl.PinMux);
  GpioServices->SetTerminationConfig (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SCL (I3cNumber), I3cDeviceConfig->Scl.PinMux), I3cDeviceConfig->Scl.PadTermination);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SCL (I3cNumber), I3cDeviceConfig->Scl.PinMux), GpioV2InputInversionDisable);

  //
  //  SCL_FB
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SCL_FB (I3cNumber), I3cDeviceConfig->SclFb.PinMux);
  GpioServices->SetTerminationConfig (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SCL_FB (I3cNumber), I3cDeviceConfig->SclFb.PinMux), I3cDeviceConfig->SclFb.PadTermination);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_I3C_SCL_FB (I3cNumber), I3cDeviceConfig->SclFb.PinMux), GpioV2InputInversionDisable);
}