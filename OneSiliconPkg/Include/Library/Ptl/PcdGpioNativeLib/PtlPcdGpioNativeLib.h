/** @file
  Header file for PtlPcdGpioNativeLib

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

#ifndef _PTL_PCD_GPIO_NATIVE_LIB_H_
#define _PTL_PCD_GPIO_NATIVE_LIB_H_

#include <Register/GpioV2ChipsetId.h>
#include <GpioV2Config.h>
#include <Pins/GpioV2PadMuxing.h>
#include <Library/GpioV2AccessLib.h>
#include <GpioV2Services.h>


/**
  This function provides GPIO Native Pad for a given native function
  @param[in]  GpioServices           GPIO Services
  @param[in]  PadFunction            PadFunction for a specific native signal. Please refer to GpioV2Signals.h

  @retval     GpioPad                GPIO pad with encoded native function
**/
GPIOV2_PAD
PtlPcdGpioGetNativePadByFunction (
  IN  GPIOV2_SERVICES  *GpioServices,
  IN  UINT32           PadFunction
  );

/**
  This procedure will get GPIO native pad based on provided native function and platform muxing selection (if needed).

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioV2Signals.h
  @param[in]  PinMux              GPIO Native pin mux platform config.
                                  This argument is optional and needs to be
                                  provided only if feature can be enabled
                                  on multiple pads

  @retval  NativePad              GPIO pad with encoded native function
**/
GPIOV2_NATIVE_PAD
PtlPcdGpioGetNativePadByFunctionAndPinMux (
  IN GPIOV2_SERVICES  *GpioServices,
  IN UINT32           PadFunction,
  IN UINT32           PinMux
  );

/**
  This procedure will set GPIO pad to native function based on provided native function
  and platform muxing selection (if needed).

  @param[in]  GpioServices        GPIO Services
  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioV2Signals.h
  @param[in]  PinMux              GPIO Native pin mux platform config.
                                  This argument is optional and needs to be
                                  provided only if feature can be enabled
                                  on multiple pads

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdGpioSetNativePadByFunction (
  IN  GPIOV2_SERVICES  *GpioServices,
  IN  UINT32           PadFunction,
  IN  UINT32           PinMux
  );

/**
  This function enables VISA@ pins by setting them into native mode

  @param[in]  GpioServices        GPIO Services

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioEnableVisaPins (
  IN GPIOV2_SERVICES  *GpioServices
  );

/**
  This function configures GPIO connection between CNVi and CRF

  @param[in]  GpioServices        GPIO Services

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioConfigureCnviCrfConnection (
  IN  GPIOV2_SERVICES    *GpioServices
  );

/**
  This function sets TBT_LSx pin into native mode

  @param[in]  GpioServices      Gpio Services
  @param[in]  TbtLsxDdiPort     TBT_LSx for DDI Port Number

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableTbtLsxInterface (
  IN  GPIOV2_SERVICES    *GpioServices,
  IN  GPIOV2_TBT_LSX     TbtLsxDdiPort
  );

/**
  This function sets TBT LSx_OE pins into native mode

  @param[in]  GpioServices     Gpio Services
  @param[in]  TbtLsxOePort     TBT LSx for OE Port Number

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableTbtLsxOeInterface (
  IN  GPIOV2_SERVICES    *GpioServices,
  IN  GPIOV2_TBT_LSX_OE  TbtLsxOePort
  );

/**
  This function configures CNVi MFUART2 external pins

  @param[in]  GpioServices        GPIO Services

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioSetCnviMfUart2ExternalPins (
  IN  GPIOV2_SERVICES    *GpioServices
  );

/**
  This function provides CNVi CRF presence strap pin

  @param[in]  GpioServices        Gpio Services

  @retval     BOOLEAN             True: Device is present
                                  False : Device is absent
**/
BOOLEAN
PtlPcdGpioGetCnviCrfStrapPinStatus (
  IN  GPIOV2_SERVICES    *GpioServices
  );

/**
  This function provides CNVi BT I2S pins

  @param[in]  GpioServices        GPIO Services
  @param[in]  ConnectionType      CNVi BT I2S connection type
  @param[in]  ConnectionIndex     CNVi BT I2S connection index

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
  @retval EFI_UNSUPPORTED         Unsupported operation
**/
EFI_STATUS
PtlPcdGpioConfigureCnviBtI2sConnection (
  IN  GPIOV2_SERVICES                      *GpioServices,
  IN  VGPIOV2_CNVI_BT_I2S_CONNECTION_TYPE  ConnectionType,
  IN  UINT32                               ConnectionIndex
  );

/**
  This function configures CNVi Bluetooth Enable pad

  @param[in]  GpioServices        GPIO Services
  @param[in]  Value               CNVi BT enable value
                                   0: Disable, 1: Enable

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioSetCnviBtEnState (
  IN  GPIOV2_SERVICES          *GpioServices,
  IN  UINT8                     Value
  );

/**
  This function enables CNVi BT interface

  @param[in]  GpioServices        GPIO Services
  @param[in]  BtInterface         Bluetooth Interface Value

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableCnviBtInterface (
  IN  GPIOV2_SERVICES              *GpioServices,
  IN  VGPIOV2_CNVI_BT_INTERFACE    BtInterface
  );

/**
  Power button debounce configuration
  Debounce time can be specified in microseconds. Only certain values according
  to below formula are supported:
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(glitch filter clock period).
  RTC clock with f = 32 KHz is used for glitch filter.
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(31.25 us).
  Supported DebounceTime values are following:
   DebounceTime = 0 -> Debounce feature disabled
   DebounceTime > 0 && < 250us -> Not supported
   DebounceTime = 250us - 1024000us -> Supported range (DebounceTime = 250us * 2^n)
  For values not supported by HW, they will be rounded down to closest supported one

  @param[in]  GpioServices   GPIO Services
  @param[in] DebounceTime    Debounce Time in microseconds
                             If Debounce Time = 0, Debouncer feature will be disabled
                             Function will set DebounceTime argument to rounded suppo    rted value

   @retval Status
**/
EFI_STATUS
PtlPcdGpioSetDebounceTimerforPwrBtn (
  IN  GPIOV2_SERVICES      *GpioServices,
  IN  UINT32               DebounceTime
  );

/**
  This function sets Time Sync Gpio into native mode

  @param[in]  GpioServices      GPIO Services
  @param[in]  Index       index

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableTimeSync (
  IN  GPIOV2_SERVICES    *GpioServices,
  IN  UINT32             Index
  );

/**
  This function sets CPU C10 Gate pins into native mode

  @param[in]  GpioServices        GPIO Services

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableCpuC10GatePin (
  IN  GPIOV2_SERVICES     *GpioServices
  );

/**
  This function sets VRALERTB pin into native mode

  @param[in]  GpioServices        GPIO Services

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableVrAlert (
  IN  GPIOV2_SERVICES     *GpioServices
  );

/**
  This procedure sets Smbus gpio pads into native mode

  @param[in] GpioServices            GPIO Services

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   GPIO V2 Services were not found
**/
EFI_STATUS
PtlPcdGpioEnableSmbus (
  IN GPIOV2_SERVICES    *GpioServices
  );

/**
  This procedure sets Smbus Alert gpio pads into native mode

  @param[in] GpioServices            GPIO Services

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   GPIO V2 Services were not found
**/
EFI_STATUS
PtlPcdGpioEnableSmbusAlert (
  IN GPIOV2_SERVICES    *GpioServices
  );

/**
  This function provides SPI IO pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1
  @param[in]  IoIndex                   IoIndex Valid from 0 (SPI_IO_0) to 3 (SPI_IO_3)

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiIo (
  IN  GPIOV2_SERVICES      *GpioServices,
  IN  UINT32               SpiIndex,
  IN  UINT32               IoIndex
  );

/**
  This function provides SPI ChipSelect pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiCs (
  IN GPIOV2_SERVICES      *GpioServices,
  IN UINT32               SpiIndex
  );

/**
  This function provides SPI Clock pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiClk (
  IN GPIOV2_SERVICES       *GpioServices,
  IN UINT32                SpiIndex
  );

/**
  This function programes SPI THC DSYNC pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiDSync (
  IN GPIOV2_SERVICES       *GpioServices,
  IN UINT32                SpiIndex
  );

/**
  This function provides SPI Reset pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1
  @param[out] Pad                       Reset pad for a given SPI instance

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioGetThcSpiResetPin (
  IN  GPIOV2_SERVICES       *GpioServices,
  IN  UINT32                SpiIndex,
  OUT GPIOV2_PAD            *Pad
  );

/**
  This function configures SPI Reset pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1
  @param[in]  NativePin                 TRUE  - pin needs to be configured as native pad
                                        FALSE - pin needs to be configured as GPIO pad
  @param[in]  ResetPad                  Reset pad

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiReset (
  IN GPIOV2_SERVICES        *GpioServices,
  IN UINT32                 SpiIndex,
  IN BOOLEAN                NativePin,
  IN GPIOV2_PAD             ResetPad
  );

/**
  This function provides GPIO Native Pad with encoded Wake on Touch native function for a given THC index

  @param[in]  ThcIndex            THC0 or THC1 - 0 or 1

  @retval     GpioPad             GPIO pad with encoded native function
**/
GPIOV2_PAD
PtlPcdGpioGetThcWotGpioPad (
  IN UINT8      ThcIndex
  );

/**
  This function configures THC Wake On Touch int pin

  @param[in]  GpioServices        GPIO Services
  @param[in]  SpiIndex            SPI1 or SPI2 - 0 or 1

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioConfigureThcWakeOnTouchPin (
  IN  GPIOV2_SERVICES      *GpioServices,
  IN  UINT32               SpiIndex
  );

/**
  This function provides I2C SCL/SDA pin for Touch Host Controller
  @param[in]  GpioServices              GPIO Services
  @param[in]  I2cIndex                  I2C1 or I2C2 - 0 or 1
  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableQuickI2c (
  IN GPIOV2_SERVICES       *GpioServices,
  IN UINT32                I2cIndex
  );

/**
  This function configures I2C Reset pin for Touch Host Controller
  @param[in]  GpioServices              GPIO Services
  @param[in]  I2cIndex                  I2C1 or I2C2 - 0 or 1
  @param[in]  NativePin                 TRUE  - pin needs to be configured as native pad
                                        FALSE - pin needs to be configured as GPIO pad
  @param[in]  ResetPad                  Reset pad
  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcI2cReset (
  IN GPIOV2_SERVICES        *GpioServices,
  IN UINT32                 I2cIndex,
  IN BOOLEAN                NativePin,
  IN GPIOV2_PAD             ResetPad
  );

/**
  This function configures I2c Int pin for Touch Host Controller
  @param[in]  GpioServices              GPIO Services
  @param[in]  I2cIndex                  I2C1 or I2C2 - 0 or 1
  @param[in]  UINT32 InterruptPinMuxing Thc Port Interrupt Pin Muxing
  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcI2cInt (
  IN GPIOV2_SERVICES        *GpioServices,
  IN UINT32                 I2cIndex,
  IN UINT32                 InterruptPinMuxing
  );

/**
  The function performs GPIO Power Management programming.

  @param[in] GpioServices            GPIO Services

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   GPIO V2 Services were not found
**/
EFI_STATUS
PtlPcdGpioConfigurePm (
  IN GPIOV2_SERVICES    *GpioServices
  );

/**
  This procedure will set GPIO Driver IRQ number

  @param[in] GpioServices            GPIO Services
  @param[in] Irq                     Irq number

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioSetIrq (
  IN GPIOV2_SERVICES    *GpioServices,
  IN UINT8              Irq
  );

/**
  This function enables USB OverCurrent pins by setting
  USB1 OCB pins into native mode

  @param[in]  GpioServices        Gpio Services
  @param[in]  OcPinNumber         USB OC pin number
  @param[in]  NativeActiveFlag    Native Function Virtual Wire Enable

  @retval     EFI_SUCCESS         The function completed successfully
  @retval     EFI_UNSUPPORTED     Unsupported operation
**/
EFI_STATUS
PtlPcdGpioEnableUsbOverCurrent (
  IN GPIOV2_SERVICES     *GpioServices,
  IN UINTN               OcPinNumber,
  IN UINT8               NativeActiveFlag
  );

/**
  This function sets PCHHOT pin into native mode

  @param[in]  GpioServices            Gpio Services

  @retval     EFI_SUCCESS             The function completed successfully
  @retval     EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioEnablePchHot (
  IN GPIOV2_SERVICES     *GpioServices
  );

/**
  This function performs initial IO Standby State related configurations
**/
VOID
PtlPcdGpioConfigureIoStandbyState (
  VOID
  );

/**
  This function enables Link Down functionality
  on platform specific GPIO

  @param[in]  GpioServices    Gpio Services

  @retval     EFI_SUCCESS     If enabling Link Down functionality is a success
  @retval     EFI_NOT_FOUND   If GpioService is not found
**/
EFI_STATUS
PtlPcdLinkDownEnable (
  IN GPIOV2_SERVICES    *GpioServices
  );

/**
  This function enables IMG CLKOUT native pin

  @param[in] GpioServices          Gpio Services
  @param[in] ImguClkOutPinIndex    The index of IMGU CLKOUT native pin

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableImguClkOut (
  IN  GPIOV2_SERVICES    *GpioServices,
  IN  UINT8              ImguClkOutPinIndex
  );

/**
  This function performs basic initialization IOM for AUX Layout in PEI phase after Policy produced at Pre-Mem phase.
  For those GPIO pins used for DP Aux orientation control and enabled, BIOS is responsible to -
  1. Configure Pad Mode (PMode) to function# associated with IOM_GPP*_*
     The Pad Mode here could be various per PCH design.
  2. BIOS Provide the following information for the DP Aux orientation bias control for provide the GPIO VwIndex,
     Bit Position, and VW Index information to IOM FW.
     I. GPIO endpoint IOSF-SB port ID (Pch Community port ID)
     II. VW index and data bit position
  @param[in]  GpioPad    - GPIO Pad
  @param[out] PchPortId  - GPIO Pad mapping GPCOM PortID number
  @param[out] Bits       - GPIO Pad mapping Bit position
  @param[out] VwIndex    - GPIO Pad mapping Virtual Wire Index number
**/
VOID
PtlGpioIomAuxOriSetting (
  UINT32   GpioPadNum,
  UINT16   *PortId,
  UINT8    *Bits,
  UINT8    *VwIndex
  );

/**
  This function Implements Tcss LSx Pins

  @param[in] Index   TCSS USBC Port Index
**/
VOID
PtlPcdTcssLsxPinEnable (
  IN  UINT8            Index
  );

/**
  This function implements TCSS LSx_OE pins

  @param[in] Index   TCSS USBC Port Index

**/
VOID
PtlPcdTcssLsxOePinEnable (
  IN  UINT8            Index
  );

/**
  This function sets TSN0 into native mode

  @param[in]  GpioServices   Gpio Services
  @param[in]  GpioServices   TSN controller index

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableTsnPins (
  IN  GPIOV2_SERVICES   *GpioServices,
  IN  UINT8             TsnIndex
  );

#endif // _PTL_PCD_GPIO_NATIVE_LIB_H_
