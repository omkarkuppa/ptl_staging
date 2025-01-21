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
#define MAX_UART_INSTANCES 3

typedef enum {
  GpioUartPinRx,
  GpioUartPinTx,
  GpioUartPinRts,
  GpioUartPinCts
} GPIOV2_SEC_UART_PIN_MUX;

typedef struct {
  GPIOV2_PAD UartTx;
  GPIOV2_PAD UartRx;
  GPIOV2_PAD UartRts;
  GPIOV2_PAD UartCts;
} UART_PIN;
typedef struct {
  UART_PIN UartPin[MAX_UART_INSTANCES];
} UART_PIN_SET;

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

// PTL Platform
UART_PIN_SET PtlUartPinSet = {
    {
      { GPIOV2_PTL_PCD_MUXING__XXGPP_H_9__UART0_TXD, GPIOV2_PTL_PCD_MUXING__XXGPP_H_8__UART0_RXD, GPIOV2_PTL_PCD_MUXING__XXGPP_H_10__UART0_RTS_B, GPIOV2_PTL_PCD_MUXING__XXGPP_H_11__UART0_CTS_B },
      { GPIOV2_PTL_PCD_MUXING__XXGPP_H_7__UART1_TXD, GPIOV2_PTL_PCD_MUXING__XXGPP_H_6__UART1_RXD, GPIOV2_PAD_NONE, GPIOV2_PAD_NONE },
      { GPIOV2_PTL_PCD_MUXING__XXGPP_F_2__UART2_TXD, GPIOV2_PTL_PCD_MUXING__XXGPP_F_1__UART2_RXD, GPIOV2_PTL_PCD_MUXING__XXGPP_F_0__UART2_RTS_B, GPIOV2_PTL_PCD_MUXING__XXGPP_F_3__UART2_CTS_B }
    }
};



GPIOV2_PAD
GetUartGpioPad (
  UART_PIN_SET            *UartPinSet,
  UINT8                   UartInstance,
  GPIOV2_SEC_UART_PIN_MUX PinType
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
/**
  Gets GPIO PinMux value
  @param[in]  PinMux         Pin Mux value from PCD
  @param[in]  UartInstance   UART instance (0 for UART0, 1 for UART1, 2 for UART2)
  @param[in]  PinType        LPSS Uart Pin Type
**/
GPIOV2_PAD
PtlPcdSecGetUartGpioPad (
  IN UINT32                  PinMux,
  IN UINT8                   UartInstance,
  IN GPIOV2_SEC_UART_PIN_MUX PinType
  )
{
  if (PinMux == 0) {
    if (PtlIsPcdP()) {
      return GetUartGpioPad(&PtlUartPinSet, UartInstance, PinType);
    }


  }
  
  return PinMux;
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
