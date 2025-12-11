/** @file
  This file provides necessary GPIO functionality for PTL PCD.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef PTL_PCD_MINIMAL_GPIO_NATIVE_LIB_H
#define PTL_PCD_MINIMAL_GPIO_NATIVE_LIB_H

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <GpioV2Pad.h>
#include <Register/GpioRegs.h>
#include <Register/GpioV2ChipsetId.h>
#include <Register/GpioV2PcdPins.h>
#include <Library/PcdGpioTopologyLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PcdLib.h>
#include <GpioV2Signals.h>

/**
  This procedure will set GPIO Driver IRQ number

  @param[in] Irq                     Irq number

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdMinimalGpioSetIrq (
  IN UINT8              Irq
  );

/**
  The function performs GPIO Power Management programming.

  @param[in] Irq                     Irq number

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdMinimalGpioConfigurePm (
  VOID
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
PtlMinimalGpioIomAuxOriSetting (
  UINT32   GpioPadNum,
  UINT16   *PortId,
  UINT8    *Bits,
  UINT8    *VwIndex
  );

/**
  This function provides CNVi CRF presence strap pin

  @param[in]  GpioServices        Gpio Services

  @retval     BOOLEAN             True: Device is present
                                  False : Device is absent
**/
BOOLEAN
PtlPcdMinimalGpioGetCnviCrfStrapPinStatus (
  VOID
  );

/**
  Returns the GPIO pad for a given minimal native signal.

  @param[in] Signal   The minimal native signal to look up.

  @retval GPIOV2_PAD  The pad associated with the signal, or GPIOV2_PAD_NONE if not found.
**/
GPIOV2_PAD
PtlPcdMinimalGpioGetNativePadByFunction (
  IN GPIOV2_SIGNAL Signal
  );

/**
  Returns the GPIO pad for a given minimal native signal with PinMux support.

  @param[in] Signal   The minimal native signal to look up.
  @param[in] PinMux   GPIO Native pin mux platform config.

  @retval GPIOV2_PAD  The pad associated with the signal, or GPIOV2_PAD_NONE if not found.
**/
GPIOV2_PAD
PtlPcdMinimalGpioGetNativePadByFunctionAndPinMux (
  IN GPIOV2_SIGNAL Signal,
  IN UINT32        PinMux
  );

/**
  This procedure will set GPIO pad to native function based on provided native function
  and platform muxing selection (if needed) - minimal version.

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioV2Signals
  @param[in]  PinMux              GPIO Native pin mux platform config.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdMinimalGpioSetNativePadByFunction (
  IN GPIOV2_SIGNAL Signal,
  IN UINT32        PinMux
  );

/**
  This procedure will set GPIO pad reset config (minimal version).

  @param[in] GpioPad              GPIO Pad
  @param[in] ResetConfig          Reset configuration to set

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid pad number
  @retval EFI_NOT_FOUND           Reset config not found in mapping table
**/
EFI_STATUS
PtlPcdMinimalGpioSetResetConfig (
  IN GPIOV2_PAD           GpioPad,
  IN GPIOV2_RESET_CONFIG  ResetConfig
  );

/**
  This procedure reads current GPIO Pad input state (minimal version).

  @param[in] GpioPad              GPIO Pad
  @param[out] InputState          Pointer to a buffer for input state

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid pad number or NULL pointer
**/
EFI_STATUS
PtlPcdMinimalGpioGetRx (
  IN  GPIOV2_PAD       GpioPad,
  OUT GPIOV2_PAD_STATE *InputState
  );

/**
  This procedure will set GPIO pad direction (minimal version).

  @param[in] GpioPad              GPIO Pad
  @param[in] Direction            Direction to set (Input, Output, None, InOut, InInv, InInvOut)

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid pad number
**/
EFI_STATUS
PtlPcdMinimalGpioSetDirection (
  IN GPIOV2_PAD           GpioPad,
  IN GPIOV2_DIRECTION     Direction
  );

/**
  Atomically configure a GPIO pad's mode, output state, direction, and reset config in a safe sequence.
  Output state is set before direction to avoid glitches on CLKREQ# and similar critical signals.

  @param[in]  GpioPad      GPIO Pad to configure
  @param[in]  PadMode      Pad mode to set
  @param[in]  OutputState  Output state to set (if direction is output) - set BEFORE direction
  @param[in]  Direction    Direction to set (Input, Output, etc.)
  @param[in]  ResetConfig  Reset configuration to set

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid pad number or parameter
  @retval EFI_NOT_FOUND           Reset config not found in mapping table
*/
EFI_STATUS
PtlPcdMinimalGpioConfigurePadAtomic (
  IN GPIOV2_PAD           GpioPad,
  IN GPIOV2_PAD_MODE      PadMode,
  IN GPIOV2_PAD_STATE     OutputState,
  IN GPIOV2_DIRECTION     Direction,
  IN GPIOV2_RESET_CONFIG  ResetConfig
  );

#endif // PTL_PCD_MINIMAL_GPIO_NATIVE_LIB_H
