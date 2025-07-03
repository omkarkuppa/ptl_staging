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

#endif // PTL_PCD_MINIMAL_GPIO_NATIVE_LIB_H
