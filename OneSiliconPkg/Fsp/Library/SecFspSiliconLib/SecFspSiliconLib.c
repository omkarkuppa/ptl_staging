/** @file
  Fsp Silicon Common library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <PiPei.h>
#include <FspEas.h>
#include <FspmUpd.h>
#include <FspsUpd.h>
#include <FsptUpd.h>
#include <Library/PchLimits.h>
#include <SerialIoDevices.h>
#include <Library/FspCommonLib.h>
#include <Library/FspSwitchStackLib.h>
#include <Library/GetFsptApiParameter.h>
#include <Library/PcdLib.h>
#include <Library/LpssUartDebugPropertyPcdLib.h>
#include <LpssI2cConfig.h>
#include <LpssUartConfig.h>
#include <Library/LpssUartDebugPropertyPcdLib.h>

/**
  Serial Io Spi Configuration Wrapper

  @param[out] SpiDeviceConfig           A pointer to the SERIAL_IO_SPI_CONFIG.
  @param[out] SerialIoSpiNumber         The Number of Serial Io Spi.
  @param[out] SerialIoSpiMmioBase       MMIO Base Address by default in PCI Mode
**/
VOID
SerialIoSpiConfigurationWrapper (
  OUT SERIAL_IO_SPI_CONFIG  *SpiDeviceConfig,
  OUT UINT8                 *SerialIoSpiNumber,
  OUT UINTN                 *SerialIoSpiMmioBase
  )
{
  FSPT_UPD               *FsptUpd;
  UINT8                  CsIndex;

  FsptUpd = (FSPT_UPD *)((UINTN) (SecGetFsptApiParameter ()));
  if (FsptUpd != NULL) {
    SpiDeviceConfig->Mode                = FsptUpd->FsptConfig.PcdSerialIoSpiMode;
    if (SpiDeviceConfig->Mode == 0) {
      return;
    }
    *SerialIoSpiNumber                   = FsptUpd->FsptConfig.PcdSerialIoSpiNumber;
    *SerialIoSpiMmioBase                 = FsptUpd->FsptConfig.PcdSerialIoSpiMmioBase;
    SpiDeviceConfig->DefaultCsOutput     = FsptUpd->FsptConfig.PcdSerialIoSpiDefaultCsOutput;
    SpiDeviceConfig->CsMode              = FsptUpd->FsptConfig.PcdSerialIoSpiCsMode;
    SpiDeviceConfig->CsState             = FsptUpd->FsptConfig.PcdSerialIoSpiCsState;

    SpiDeviceConfig->PinMux.Clk          = FsptUpd->FsptConfig.PcdSerialIoSpiClkPinMux;
    SpiDeviceConfig->PinMux.Miso         = FsptUpd->FsptConfig.PcdSerialIoSpiMisoPinMux;
    SpiDeviceConfig->PinMux.Mosi         = FsptUpd->FsptConfig.PcdSerialIoSpiMosiPinMux;

    for (CsIndex = 0; CsIndex < PCH_MAX_SERIALIO_SPI_CHIP_SELECTS; CsIndex++) {
      SpiDeviceConfig->CsEnable[CsIndex]   = FsptUpd->FsptConfig.PcdSerialIoSpiCsEnable[CsIndex];
      SpiDeviceConfig->CsPolarity[CsIndex] = FsptUpd->FsptConfig.PcdSerialIoSpiCsPolarity[CsIndex];
      SpiDeviceConfig->PinMux.Cs[CsIndex]  = FsptUpd->FsptConfig.PcdSerialIoSpiCsPinMux[CsIndex];
    }
    return;
  }
  SpiDeviceConfig->Mode = 0;
}


/**
  Serial Io I2C Configuration Wrapper

  @param[out] I2cDeviceConfig           A pointer to the SERIAL_IO_I2C_CONFIG.
  @param[out] SerialIoI2cNumber         The Number of Serial Io I2c.
  @param[out] SerialIoI2cMmioBase       MMIO Base Address by default in PCI Mode
**/
VOID
SerialIoI2cConfigurationWrapper (
  OUT SERIAL_IO_I2C_CONFIG  *I2cDeviceConfig,
  OUT UINT8                 *SerialIoI2cNumber,
  OUT UINTN                 *SerialIoI2cMmioBase
  )
{
  FSPT_UPD               *FsptUpd;

  I2cDeviceConfig->Mode = 0;

  FsptUpd = (FSPT_UPD *)((UINTN) (SecGetFsptApiParameter ()));
  if (FsptUpd == NULL) {
    return;
  }

  *SerialIoI2cMmioBase = FsptUpd->FsptConfig.PcdSerialIoI2cMmioBase;
  *SerialIoI2cNumber = FsptUpd->FsptConfig.PcdSerialIoI2cNumber;
  if (*SerialIoI2cNumber == 0xFF) {
    return;
  }

  I2cDeviceConfig->Mode                = SerialIoI2cPci;
  I2cDeviceConfig->PadTermination      = FsptUpd->FsptConfig.PcdSerialIoI2cPadsTerm;
  I2cDeviceConfig->PinMux.Sda          = FsptUpd->FsptConfig.PcdSerialIoI2cSdaPin;
  I2cDeviceConfig->PinMux.Scl          = FsptUpd->FsptConfig.PcdSerialIoI2cSclPin;
  return;
}

/**
  Lpss I2C Configuration Wrapper

  @param[out] I2cDeviceConfig           A pointer to the SERIAL_IO_I2C_CONFIG.
  @param[out] LpssI2cNumber             The Number of Lpss I2c.
  @param[out] LpssI2cMmioBase           MMIO Base Address by default in PCI Mode
**/
VOID
LpssI2cConfigurationWrapper (
  OUT LPSS_I2C_CONTROLLER_CONFIG *I2cDeviceConfig,
  OUT UINT8                      *LpssI2cNumber,
  OUT UINT64                     *LpssI2cMmioBase
  )
{
  FSPT_UPD               *FsptUpd;

  I2cDeviceConfig->Mode = 0;

  FsptUpd = (FSPT_UPD *)((UINTN) (SecGetFsptApiParameter ()));
  if (FsptUpd == NULL) {
    return;
  }

  *LpssI2cMmioBase = FsptUpd->FsptConfig.PcdSerialIoI2cMmioBase;
  *LpssI2cNumber = FsptUpd->FsptConfig.PcdSerialIoI2cNumber;
  if (*LpssI2cNumber == 0xFF) {
    return;
  }

  I2cDeviceConfig->Mode                = LpssI2cPci;
  I2cDeviceConfig->PadTermination      = FsptUpd->FsptConfig.PcdSerialIoI2cPadsTerm;
  I2cDeviceConfig->PinMux.Sda          = FsptUpd->FsptConfig.PcdSerialIoI2cSdaPin;
  I2cDeviceConfig->PinMux.Scl          = FsptUpd->FsptConfig.PcdSerialIoI2cSclPin;
  return;
}

/**
  LPSS Uart Debug Configuration Wrapper

  @param[out] UartDeviceConfig           A pointer to the SERIAL_IO_UART_CONFIG.
  @param[out] SerialIoUartDebugEnable    LPSS Uart Debug Enable/Disable.
  @param[out] SerialIoUartNumber         The Number of LPSS Uart.
  @param[out] SerialIoUartPciMmioBase    MMIO Base Address by default in PCI Mode
**/
VOID
LpssUartDebugConfigurationWrapper (
  OUT LPSS_UART_DEVICE_CONFIG  *UartDeviceConfig,
  OUT UINT8                    *LpssUartDebugEnable,
  OUT UINT8                    *LpssUartNumber,
  OUT UINT32                   *LpssUartPciMmioBase
  )
{
  FSPT_UPD               *FsptUpd;

  FsptUpd = (FSPT_UPD *)((UINTN) (SecGetFsptApiParameter ()));
  if (FsptUpd != NULL) {
    *LpssUartDebugEnable              = FsptUpd->FsptConfig.PcdSerialIoUartDebugEnable;
    *LpssUartNumber                   = FsptUpd->FsptConfig.PcdSerialIoUartNumber;
    *LpssUartPciMmioBase              = FsptUpd->FsptConfig.PcdSerialIoUartDebugMmioBase;
    UartDeviceConfig->Mode                = FsptUpd->FsptConfig.PcdSerialIoUartMode;
    UartDeviceConfig->Attributes.BaudRate = FsptUpd->FsptConfig.PcdSerialIoUartBaudRate;
    UartDeviceConfig->Attributes.Parity   = FsptUpd->FsptConfig.PcdSerialIoUartParity;
    UartDeviceConfig->Attributes.DataBits = FsptUpd->FsptConfig.PcdSerialIoUartDataBits;
    UartDeviceConfig->Attributes.StopBits = FsptUpd->FsptConfig.PcdSerialIoUartStopBits;
    UartDeviceConfig->Attributes.AutoFlow = FsptUpd->FsptConfig.PcdSerialIoUartAutoFlow;
    UartDeviceConfig->PinMux.Rx           = FsptUpd->FsptConfig.PcdSerialIoUartRxPinMux;
    UartDeviceConfig->PinMux.Tx           = FsptUpd->FsptConfig.PcdSerialIoUartTxPinMux;
    UartDeviceConfig->PinMux.Rts          = FsptUpd->FsptConfig.PcdSerialIoUartRtsPinMux;
    UartDeviceConfig->PinMux.Cts          = FsptUpd->FsptConfig.PcdSerialIoUartCtsPinMux;
    return;
  }
  *LpssUartDebugEnable = LpssUartDebugPcdGetDebugEnable ();
  *LpssUartPciMmioBase = LpssUartDebugPcdGetPciDefaultMmioBase ();
  LpssUartDebugPcdGetDeviceConfig (UartDeviceConfig, LpssUartNumber);
}

/**
  LPSS Additional Uart Configuration Wrapper
  Allows to configure another UART in FSPT

  @param[out] UartEnable                 LPSS Additional Uart Enable/Disable.
  @param[out] UartDeviceConfig           A pointer to the SERIAL_IO_UART_CONFIG.
  @param[out] LpssUartNumber         The Number of LPSS Uart.
  @param[out] LpssUartPciMmioBase    MMIO Base Address by default in PCI Mode
**/
VOID
Lpss2ndUartConfigurationWrapper (
  OUT UINT8                    *UartEnable,
  OUT LPSS_UART_DEVICE_CONFIG  *UartDeviceConfig,
  OUT UINT8                    *LpssUartNumber,
  OUT UINT32                   *LpssUartPciMmioBase
  )
{
  FSPT_UPD               *FsptUpd;

  FsptUpd = (FSPT_UPD *)((UINTN) (SecGetFsptApiParameter ()));
  if (FsptUpd != NULL) {
    *UartEnable                           = FsptUpd->FsptConfig.PcdSerialIo2ndUartEnable;
    if (FsptUpd->FsptConfig.PcdSerialIo2ndUartEnable == 0) {
      return;
    }
    *LpssUartNumber                       = FsptUpd->FsptConfig.PcdSerialIo2ndUartNumber;
    *LpssUartPciMmioBase                  = FsptUpd->FsptConfig.PcdSerialIo2ndUartMmioBase;
    UartDeviceConfig->Mode                = FsptUpd->FsptConfig.PcdSerialIo2ndUartMode;
    UartDeviceConfig->Attributes.BaudRate = FsptUpd->FsptConfig.PcdSerialIo2ndUartBaudRate;
    UartDeviceConfig->Attributes.Parity   = FsptUpd->FsptConfig.PcdSerialIo2ndUartParity;
    UartDeviceConfig->Attributes.DataBits = FsptUpd->FsptConfig.PcdSerialIo2ndUartDataBits;
    UartDeviceConfig->Attributes.StopBits = FsptUpd->FsptConfig.PcdSerialIo2ndUartStopBits;
    UartDeviceConfig->Attributes.AutoFlow = FsptUpd->FsptConfig.PcdSerialIo2ndUartAutoFlow;
    UartDeviceConfig->PinMux.Rx           = FsptUpd->FsptConfig.PcdSerialIo2ndUartRxPinMux;
    UartDeviceConfig->PinMux.Tx           = FsptUpd->FsptConfig.PcdSerialIo2ndUartTxPinMux;
    UartDeviceConfig->PinMux.Rts          = FsptUpd->FsptConfig.PcdSerialIo2ndUartRtsPinMux;
    UartDeviceConfig->PinMux.Cts          = FsptUpd->FsptConfig.PcdSerialIo2ndUartCtsPinMux;
    return;
  }
  *UartEnable = 0;
}

/**
  This function check the signture of UPD

  @param[in] ApiIdx           Internal index of the FSP API.
  @param[in] ApiParam         Parameter of the FSP API.
**/
EFI_STATUS
EFIAPI
FspUpdSignatureCheck (
  IN UINT32   ApiIdx,
  IN VOID     *ApiParam
  )
{
  EFI_STATUS    Status;
  FSPM_UPD      *FspmUpd;
  FSPS_UPD      *FspsUpd;

  Status = EFI_SUCCESS;
  FspmUpd = NULL;
  FspsUpd = NULL;

  if (ApiIdx == 3) {
    //
    // FspMemoryInit check
    //
    FspmUpd = (FSPM_UPD *)ApiParam;
    if (FspmUpd != NULL) {
      if ((FspmUpd->FspUpdHeader.Signature != FSPM_UPD_SIGNATURE)
        || ((UINTN)FspmUpd->FspmArchUpd.StackBase == 0 )
        || (FspmUpd->FspmArchUpd.StackSize < PcdGet32(PcdFspTemporaryRamSize))
        || (((UINTN)FspmUpd->FspmArchUpd.StackBase + FspmUpd->FspmArchUpd.StackSize) > BASE_4GB)
        || ((FspmUpd->FspmArchUpd.BootLoaderTolumSize % EFI_PAGE_SIZE) != 0)) {
        Status = EFI_INVALID_PARAMETER;
      }
    }
  } else if (ApiIdx == 5) {
    //
    // FspSiliconInit check
    //
    FspsUpd = GetFspSiliconInitUpdDataPointer();
    if (FspsUpd == NULL) {
      FspsUpd = (FSPS_UPD *) ApiParam;
    }
    if (FspsUpd != NULL) {
      if (FspsUpd->FspUpdHeader.Signature != FSPS_UPD_SIGNATURE) {
        Status = EFI_INVALID_PARAMETER;
      }
    }
  }

  return Status;
}
