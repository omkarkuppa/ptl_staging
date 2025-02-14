/** @file
  PTL PCD Lpss Gpio Initialization library.
  All function in this library is available for SEC phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <IncludePrivate/Library/Ptl/PtlPcdP2SbSocLib.h>
#include <Include/Library/Ptl/PcdSecGpioTopologyLib/PtlPcdSecGpioTopologyLib.h>
#include <GpioV2Signals.h>
#include <GpioV2Pad.h>
#include <LpssUartConfig.h>
#include <Register/GpioV2PcdPins.h>
#include <Library/IoLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/Ptl/PcdInfoLib/PtlPcdInfoLib.h>
#include <LpssI2cConfig.h>
#include <LpssSpiConfig.h>
#define MAX_UART_INSTANCES 3
#define MAX_I2C_INSTANCES  6
#define MAX_SPI_INSTANCES  3

typedef enum {
  GpioUartPinRx,
  GpioUartPinTx,
  GpioUartPinRts,
  GpioUartPinCts
} GPIOV2_SEC_UART_PIN_MUX;

typedef enum {
  GpioI2cPinScl,
  GpioI2cPinSda,
} GPIOV2_SEC_I2C_PIN_MUX;

typedef enum {
  GpioSpiPinCs,
  GpioSpiPinClk,
  GpioSpiPinMiso,
  GpioSpiPinMosi
}GPIOV2_SEC_SPI_PIN_MUX;

typedef struct {
  GPIOV2_PAD UartTx;
  GPIOV2_PAD UartRx;
  GPIOV2_PAD UartRts;
  GPIOV2_PAD UartCts;
} UART_PIN;

typedef struct {
  GPIOV2_PAD I2cScl;
  GPIOV2_PAD I2cSda;
} I2C_PIN;

typedef struct {
  GPIOV2_PAD SpiCs;
  GPIOV2_PAD SpiClk;
  GPIOV2_PAD SpiMiso;
  GPIOV2_PAD SpiMosi;
} SPI_PIN;

typedef struct {
  UART_PIN UartPin[MAX_UART_INSTANCES];
} UART_PIN_SET;

typedef struct {
  I2C_PIN I2cPin[MAX_I2C_INSTANCES];
} I2C_PIN_SET;

typedef struct {
  SPI_PIN SpiPin[MAX_SPI_INSTANCES];
} SPI_PIN_SET;
/**
  This procedure does minimum Gpio Configuration required for Lpss Devices
  in SEC phase.

  @param[in] GpioPad             GPIO pad
  @param[in] PadModeValue        GPIO pad mode value, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
STATIC
EFIAPI
SecLpssGpioConfigure (
  IN  GPIOV2_PAD                  GpioPad,
  IN  GPIOV2_PAD_MODE             PadMode
  )
{
  UINT32                          RegisterOffset;
  UINT32                          AndValue;
  UINT32                          OrValue;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_PORT_16_ID                 Pid;
  UINT32                          Value;
  UINT32                          CommunityIndex;

  if (GpioPad == 0) {
    ASSERT (GpioPad == GPIOV2_PAD_NONE);
    return EFI_INVALID_PARAMETER;
  }
  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  Pid.Pid16bit   = ((UINT16) (PtlPcdSecGpioGetCommunityPid (CommunityIndex)));
  PtlPcdGetP2SbController (&P2SbController, Pid);

  //
  //
  //  Get DW0 Register Offset to program Pad Mode
  //
  PtlPcdSecGpioGetDw0RegOffset (GpioPad, &RegisterOffset);

  //
  //  Program Pad Mode
  //
  AndValue = (UINT32)~(GPIOV2_PAD_MODE_MASK << GPIOV2_PAD_MODE_DW0_POS);
  OrValue  = ((PadMode >> 1) & GPIOV2_PAD_MODE_MASK) << GPIOV2_PAD_MODE_DW0_POS;
  Value    = MmioRead32(P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, RegisterOffset));
  Value    &= AndValue;
  Value    |= OrValue;
  MmioWrite32( P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, RegisterOffset), Value);

  return EFI_SUCCESS;
}

UART_PIN_SET PtlUartPinSet = {
    {
      { GPIOV2_PTL_PCD_MUXING__XXGPP_H_9__UART0_TXD, GPIOV2_PTL_PCD_MUXING__XXGPP_H_8__UART0_RXD, GPIOV2_PTL_PCD_MUXING__XXGPP_H_10__UART0_RTS_B, GPIOV2_PTL_PCD_MUXING__XXGPP_H_11__UART0_CTS_B },
      { GPIOV2_PTL_PCD_MUXING__XXGPP_H_7__UART1_TXD, GPIOV2_PTL_PCD_MUXING__XXGPP_H_6__UART1_RXD, GPIOV2_PAD_NONE, GPIOV2_PAD_NONE },
      { GPIOV2_PTL_PCD_MUXING__XXGPP_F_2__UART2_TXD, GPIOV2_PTL_PCD_MUXING__XXGPP_F_1__UART2_RXD, GPIOV2_PTL_PCD_MUXING__XXGPP_F_0__UART2_RTS_B, GPIOV2_PTL_PCD_MUXING__XXGPP_F_3__UART2_CTS_B }
    }
};



I2C_PIN_SET PtlI2cPinSet = {
    {
      { GPIOV2_PTL_PCD_MUXING__XXGPP_H_20__I2C0_SCL, GPIOV2_PTL_PCD_MUXING__XXGPP_H_19__I2C0_SDA},
      { GPIOV2_PTL_PCD_MUXING__XXGPP_H_22__I2C1_SCL, GPIOV2_PTL_PCD_MUXING__XXGPP_H_21__I2C1_SDA},
      { GPIOV2_PTL_PCD_MUXING__XXGPP_B_3__A_I2C2_SCL, GPIOV2_PTL_PCD_MUXING__XXGPP_B_2__A_I2C2_SDA },
      { GPIOV2_PTL_PCD_MUXING__XXGPP_H_7__I2C3_SCL, GPIOV2_PTL_PCD_MUXING__XXGPP_H_6__I2C3_SDA },
      { GPIOV2_PTL_PCD_MUXING__XXGPP_B_19__A_I2C4_SCL, GPIOV2_PTL_PCD_MUXING__XXGPP_B_18__A_I2C4_SDA },
      { GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__I2C5_SCL, GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__I2C5_SCL }
    }
};



SPI_PIN_SET PtlSpiPinSet = {
    {
      { GPIOV2_PTL_PCD_MUXING__XXGPP_E_17__GSPI0_CS0_B, GPIOV2_PTL_PCD_MUXING__XXGPP_E_11__GSPI0_CLK, GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__GSPI0_MISO, GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__GSPI0_MOSI },
      { GPIOV2_PTL_PCD_MUXING__XXGPP_F_17__GSPI1_CS0_B, GPIOV2_PTL_PCD_MUXING__XXGPP_F_11__GSPI1_CLK, GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__GSPI1_MISO, GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__GSPI1_MOSI },
      { GPIOV2_PAD_NONE, GPIOV2_PAD_NONE, GPIOV2_PAD_NONE, GPIOV2_PAD_NONE }
    }
};



GPIOV2_PAD
GetUartGpioPad (
  IN UART_PIN_SET            *UartPinSet,
  IN UINT8                   UartInstance,
  IN GPIOV2_SEC_UART_PIN_MUX PinType
  )
{
  switch (PinType) {
    case GpioUartPinRx:
      return UartPinSet->UartPin[UartInstance].UartRx;
    case GpioUartPinTx:
      return UartPinSet->UartPin[UartInstance].UartTx;
    case GpioUartPinRts:
      return UartPinSet->UartPin[UartInstance].UartRts;
    case GpioUartPinCts:
      return UartPinSet->UartPin[UartInstance].UartCts;
    default:
      return GPIOV2_PAD_NONE;
  }
}

GPIOV2_PAD
GetI2cGpioPad (
  IN I2C_PIN_SET               *I2cPinSet,
  IN UINT8                     I2cInstance,
  IN GPIOV2_SEC_I2C_PIN_MUX    PinType
  )
{
  switch (PinType) {
    case GpioI2cPinScl:
      return I2cPinSet->I2cPin[I2cInstance].I2cScl;
    case GpioI2cPinSda:
      return I2cPinSet->I2cPin[I2cInstance].I2cSda;
    default:
      return GPIOV2_PAD_NONE;
  }
}

GPIOV2_PAD
GetSpiGpioPad (
 IN SPI_PIN_SET               *SpiPinSet,
 IN UINT8                     SpiInstance,
 IN GPIOV2_SEC_SPI_PIN_MUX    PinType
  )
{
  switch (PinType) {
    case GpioSpiPinCs:
      return SpiPinSet->SpiPin[SpiInstance].SpiCs;
    case GpioSpiPinClk:
      return SpiPinSet->SpiPin[SpiInstance].SpiClk;
    case GpioSpiPinMiso:
      return SpiPinSet->SpiPin[SpiInstance].SpiMiso;
    case GpioSpiPinMosi:
      return SpiPinSet->SpiPin[SpiInstance].SpiMosi;
    default:
      return GPIOV2_PAD_NONE;
  }
}

/**
  Gets GPIO PinMux value
  @param[in]  PinMux         Pin Mux value from PCD
  @param[in]  UartInstance   UART instance (0 for UART0, 1 for UART1, 2 for UART2)
  @param[in]  PinType        LPSS Uart Pin Type
**/
GPIOV2_PAD
PtlPcdSecGetUartGpioPad (
  IN UINT32                  UartPin,
  IN UINT8                   UartInstance,
  IN GPIOV2_SEC_UART_PIN_MUX PinType
  )
{
  if (UartPin == 0) {
    if (PtlIsPcdP () || PtlIsPcdH ()) {
      return GetUartGpioPad(&PtlUartPinSet, UartInstance, PinType);
    }



  }
  return UartPin;
}

/**
  Gets GPIO PinMux value
  @param[in]  PinMux         Pin Mux value from PCD
  @param[in]  SpiInstance    SPI instance (0 for SPI0, 1 for SPI1, 2 for SPI2)
  @param[in]  PinType        LPSS SPI Pin Type
**/
GPIOV2_PAD
PtlPcdSecGetSpiGpioPad (
  IN UINT32                  SpiPin,
  IN UINT8                   SpiInstance,
  IN GPIOV2_SEC_SPI_PIN_MUX  PinType
  )
{
  if (SpiPin == 0) {
    if (PtlIsPcdP () || PtlIsPcdH ()) {
      return GetSpiGpioPad(&PtlSpiPinSet, SpiInstance, PinType);
    }


  }
  return SpiPin;
}

/**
  Gets GPIO PinMux value
  @param[in]  PinMux         Pin Mux value from PCD
  @param[in]  I2cInstance    I2C instance (0 for I2C0, 1 for I2C1, 2 for I2C2)
  @param[in]  PinType        LPSS I2c Pin Type
**/
GPIOV2_PAD
PtlPcdSecGetI2cGpioPad (
  IN UINT32                  I2cPin,
  IN UINT8                   I2cInstance,
  IN GPIOV2_SEC_I2C_PIN_MUX  PinType
  )
{
  if (I2cPin == 0) {
    if (PtlIsPcdP () || PtlIsPcdH ()) {
      return GetI2cGpioPad(&PtlI2cPinSet, I2cInstance, PinType);
    }


  }

  return I2cPin;
}

/**
  Configures GPIO for each Lpss UART Controller in SEC phase

  @param[in] UartDeviceConfig   Lpss UART Config
**/
VOID
EFIAPI
PtlPcdSecLpssUartGpioConfigure (
  IN LPSS_UART_DEVICE_CONFIG  *UartDeviceConfig,
  IN UINT8                    UartInstance
  )
{
  GPIOV2_PAD        GpioPad;

  // RX
  GpioPad = PtlPcdSecGetUartGpioPad (UartDeviceConfig->PinMux.Rx, UartInstance, GpioUartPinRx);
  SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

  // TX
  GpioPad = PtlPcdSecGetUartGpioPad (UartDeviceConfig->PinMux.Tx, UartInstance, GpioUartPinTx);
  SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

  if (UartDeviceConfig->Attributes.AutoFlow) {
    // RTS
    GpioPad = PtlPcdSecGetUartGpioPad (UartDeviceConfig->PinMux.Rts, UartInstance, GpioUartPinRts);
    SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

    // CTS
    GpioPad = PtlPcdSecGetUartGpioPad (UartDeviceConfig->PinMux.Cts, UartInstance, GpioUartPinCts);
    SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));
  }

  return;
}

/**
  Configures GPIO for each I2c Controller in SEC phase

  @param[in] I2cDeviceConfig   Lpss I2c Config
**/
VOID
EFIAPI
PtlPcdSecLpssI2cGpioConfigure (
  IN LPSS_I2C_CONTROLLER_CONFIG  *I2cDeviceConfig,
  IN UINT8                       I2cInstance
  )
{
  GPIOV2_PAD                     GpioPad;
  
  GpioPad = PtlPcdSecGetI2cGpioPad (I2cDeviceConfig->PinMux.Scl, I2cInstance, GpioI2cPinScl);
  SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

  GpioPad = PtlPcdSecGetI2cGpioPad (I2cDeviceConfig->PinMux.Sda, I2cInstance, GpioI2cPinSda);
  SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

  return;
}

/**
  Configures GPIO for each Lpss SPI Controller in SEC phase

  @param[in] SPIDeviceConfig   Lpss SPI Config
**/
VOID
EFIAPI
PtlPcdSecLpssSpiGpioConfigure (
  IN LPSS_SPI_DEVICE_CONFIG  *SpiDeviceConfig,
  IN UINT8                   SpiInstance
  )
{
  GPIOV2_PAD                 GpioPad;

  GpioPad = PtlPcdSecGetSpiGpioPad (SpiDeviceConfig->PinMux.Cs[0],SpiInstance,GpioSpiPinCs);
  SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

  GpioPad = PtlPcdSecGetSpiGpioPad (SpiDeviceConfig->PinMux.Clk, SpiInstance, GpioSpiPinClk);
  SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

  GpioPad = PtlPcdSecGetSpiGpioPad (SpiDeviceConfig->PinMux.Miso, SpiInstance, GpioSpiPinMiso);
  SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

  GpioPad = PtlPcdSecGetSpiGpioPad (SpiDeviceConfig->PinMux.Mosi, SpiInstance, GpioSpiPinMosi);
  SecLpssGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));

  return;
}