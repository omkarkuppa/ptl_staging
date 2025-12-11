/** @file
  This file provides minimal GPIO functionality for PTL PCD.

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

#include <Library/Ptl/PcdMinGpioTopologyLib/PtlPcdMinGpioTopologyLib.h>
#include <GpioV2ControllerInterface.h>
#include <GpioV2Config.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/IoLib.h>
#include "Register/GpioRegs.h"
#include "PtlPcdMinimalGpio.h"

GPIOV2_RESET_CONFIG GppPadRstCfgMap[] = {
      GpioV2ResetResume,    // 00b
      GpioV2ResetHostDeep,  // 01b
      GpioV2ResetHost,      // 10b
      GpioV2ResetGlobal     // 11b
    };

/**
  This procedure will set GPIO Driver IRQ number

  @param[in] Irq                     Irq number

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdMinimalGpioSetIrq (
  IN UINT8              Irq
  )
{
  P2SB_CONTROLLER                 P2SbController;
  UINT32                          MiscCfg;
  UINT32                          CommunityIndex;
  P2SB_PORT_16_ID                 Pid;
  EFI_STATUS                      Status;

  for (CommunityIndex = 0; CommunityIndex < PtlPcdMinGpioGetCommunitiesNum (); CommunityIndex++) {
    Status = GetP2SbControllerForCommunity (
              CommunityIndex,
              &P2SbController,
              &Pid
              );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to get P2SB controller for community %d\n", CommunityIndex));
      return Status;
    }
    MiscCfg = MmioRead32 (P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, R_GPIO_PCR_MISCCFG));

    MiscCfg &= (UINT32)~(B_GPIO_PCR_MISCCFG_IRQ_ROUTE);
    MiscCfg |= (UINT32)Irq << N_GPIO_PCR_MISCCFG_IRQ_ROUTE;

    MmioWrite32 ( P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, R_GPIO_PCR_MISCCFG), MiscCfg);
  }
  return EFI_SUCCESS;
}

/**
  The function performs GPIO Power Management programming.

  @param[in] Irq                     Irq number

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdMinimalGpioConfigurePm (
  VOID
  )
{
  P2SB_CONTROLLER                 P2SbController;
  UINT32                          MiscCfg;
  UINT32                          CommunityIndex;
  P2SB_PORT_16_ID                 Pid;
  EFI_STATUS                      Status;

  for (CommunityIndex = 0; CommunityIndex < PtlPcdMinGpioGetCommunitiesNum (); CommunityIndex++) {
    Status = GetP2SbControllerForCommunity (
      CommunityIndex,
      &P2SbController,
      &Pid
      );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to get P2SB controller for community %d\n", CommunityIndex));
      return Status;
    }
    MiscCfg = MmioRead32 (P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, R_GPIO_PCR_MISCCFG));

    //
    // Enable MISCCFG.GPVNNREQEn, MISCCFG.GPPGCBDPCGEn, MISCCFG.GPSIDEDPCGEn, MISCCFG.GPRCOMPCDLCGEn,
    // MISCCFG.GPRTCDLCGEn, MISCCFG.GPDLCGEn and MISCCFG.GPDPCGEn for GPIO communities
    //
    MiscCfg |= (B_GPIO_PCR_MISCCFG_GPVNNREQEN     |
                B_GPIO_PCR_MISCCFG_GPPGCBDPCGEN   |
                B_GPIO_PCR_MISCCFG_GPSIDEDPCGEN   |
                B_GPIO_PCR_MISCCFG_GPRCOMPCDLCGEN |
                B_GPIO_PCR_MISCCFG_GPRTCDLCGEN    |
                B_GPIO_PCR_MISCCFG_GPDLCGEN       |
                B_GPIO_PCR_MISCCFG_GPDPCGEN);
    MmioWrite32 ( P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, R_GPIO_PCR_MISCCFG), MiscCfg);
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                      Status;
  UINT32                          Index;
  UINT32                          CommunityIndex;
  UINT32                          SignalDataArraySize;
  GPIOV2_MIN_NATIVE_SIGNAL_DATA   *SignalData;


  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if ((GpioPadNum == 0) || (PortId == NULL) || (Bits == NULL) || (VwIndex == NULL)) {
    //
    // The parameter data is invalid.
    //
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR Wrong  GpioPadNum = 0x%x PortId =%x, Bits =%x, VwIndex=%x \n", __FUNCTION__, GpioPadNum, PortId, Bits, VwIndex));
    return;
  }

  GetPcdMinNativeSignalData (&SignalData, &SignalDataArraySize);
  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPadNum);

  if (CommunityIndex >= PtlPcdMinGpioGetCommunitiesNum ()) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR Wrong Gpio community of gpio pad 0x%x\n", __FUNCTION__, GpioPadNum));
    return;
  }

  for (Index = 0; Index < SignalDataArraySize; Index++) {
    if (SignalData[Index].Pad != GpioPadNum) {
      continue;
    }
    if (SignalData[Index].Signal.Fields.Interface == GPIOV2_PERIPHERAL_INTERFACE_IOM) {
      Status = GpioMinSetPadMode (GpioPadNum, SignalData[Index].PadMode);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR failed to set the pad mode\n", __FUNCTION__));
        return;
      }

      *PortId = (UINT16)PtlPcdMinGpioGetCommunityPid (CommunityIndex);
      *Bits = (UINT8)SignalData[Index].BitPosition;
      *VwIndex = (UINT8)SignalData[Index].VwIndex;
      break;
    } else {
      DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR GPIO pad 0x%x is not IOM interface\n", __FUNCTION__, GpioPadNum));
      return;
    }
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return;
}

/**
  This function provides CNVi CRF presence strap pin

  @param[in]  GpioServices        Gpio Services

  @retval     BOOLEAN             True: Device is present
                                  False : Device is absent
**/
BOOLEAN
PtlPcdMinimalGpioGetCnviCrfStrapPinStatus (
  VOID
  )
{
  UINT32                          GpioDw2Value;
  UINT32                          GpioDw2Offset;
  P2SB_CONTROLLER                 P2SbController;
  EFI_STATUS                      Status;
  UINT32                          CommunityIndex;
  GPIOV2_PAD                      CrfStrapPin;
  UINT32                          GpioDw0Offset;
  P2SB_PORT_16_ID                 Pid;

  CrfStrapPin = GPIOV2_PTL_PCD_XXGPP_F_2;


  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (CrfStrapPin);
  if (CommunityIndex >= PtlPcdMinGpioGetCommunitiesNum ()) {
    DEBUG ((DEBUG_ERROR, "Invalid Community Index: %d\n", CommunityIndex));
    return FALSE;
  }

  PtlPcdMinGpioGetDw0RegOffset (CrfStrapPin, &GpioDw0Offset);
  GpioDw2Offset = GpioDw0Offset + 0x08;

  Status = GetP2SbControllerForCommunity (
            CommunityIndex,
            &P2SbController,
            &Pid
            );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to get P2SB controller for community %d\n", CommunityIndex));
      return FALSE;
    }

  // Reading register value
  GpioDw2Value = MmioRead32 (P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid,GpioDw2Offset));

  //
  // CNV_RGI_DT is used to strap presence of the CRF, which sampling immediately after RSMRST# de-assert.
  // It is pulled down by the CRF as long as RF_RESET_B = 0
  //
  if (GpioDw2Value & B_GPIO_PCR_PINSTRAPVAL) {
    return FALSE;
  } else {
    DEBUG((DEBUG_INFO, " CRF Device is Present.\n"));
    return TRUE;
  }
}

/**
  Returns the GPIO pad for a given minimal native signal.
  If the signal can be enabled on multiple pads, returns GPIOV2_PAD_NONE.

  @param[in] Signal   The minimal native signal to look up.

  @retval GPIOV2_PAD  The pad associated with the signal with encoded native function,
                      or GPIOV2_PAD_NONE if not found or if signal can be on multiple pads.
**/
GPIOV2_PAD
PtlPcdMinimalGpioGetNativePadByFunction (
  IN GPIOV2_SIGNAL Signal
  )
{
  GPIOV2_MIN_NATIVE_SIGNAL_DATA   *SignalData;
  UINT32                          SignalDataArraySize;
  UINT32                          Index;
  GPIOV2_PAD                      GpioPad;
  UINT32                          FoundPadsCounter;

  if (GetPcdMinNativeSignalData (&SignalData, &SignalDataArraySize) != EFI_SUCCESS) {
    return GPIOV2_PAD_NONE;
  }

  FoundPadsCounter = 0;
  GpioPad = GPIOV2_PAD_NONE;

  for (Index = 0; Index < SignalDataArraySize; Index++) {
    if (SignalData[Index].Signal.Value == Signal.Value) {
      if (FoundPadsCounter > 0) {
        // Multiple pads found for this signal - return GPIOV2_PAD_NONE
        return GPIOV2_PAD_NONE;
      }
      // Encode the pad mode into the returned pad value
      GpioPad = GPIOV2_PAD_SET_NATIVE_FUNCTION (SignalData[Index].Pad, ((SignalData[Index].PadMode >> 1) & GPIOV2_PAD_MODE_MASK));
      FoundPadsCounter++;
    }
  }
  
  return GpioPad;
}

/**
  This procedure will get GPIO native pad based on provided native function and platform muxing selection (if needed).

  @param[in]  Signal          Signal for a specific native signal. Please refer to GpioV2Signals
  @param[in]  PinMux          GPIO Native pin mux platform config.
                              Set to 0 if signal can only be on a single pad.
                              Otherwise, should be the complete encoded GPIO pad.

  @retval  NativePad          GPIO pad with encoded native function
**/
GPIOV2_PAD
PtlPcdMinimalGpioGetNativePadByFunctionAndPinMux (
  IN GPIOV2_SIGNAL Signal,
  IN UINT32        PinMux
  )
{
  GPIOV2_PAD  GpioPad;

  GpioPad = PtlPcdMinimalGpioGetNativePadByFunction (Signal);

  if (GpioPad != GPIOV2_PAD_NONE) {
    //
    // For situations where native signal can be enabled only on
    // a single pad then PinMux argument should be set to 0.
    //
    ASSERT (PinMux == GPIOV2_PAD_NONE);
  } else {
    //
    // If native signal can be enabled on multiple pads
    // then PinMux argument needs to be configured to platform specific value
    //
    GpioPad = PinMux;
  }

  return GpioPad;
}

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
  )
{
  GPIOV2_PAD       GpioPad;
  EFI_STATUS       Status;
  GpioPad = PtlPcdMinimalGpioGetNativePadByFunctionAndPinMux (Signal, PinMux);

  if (GpioPad == GPIOV2_PAD_NONE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioMinSetPadMode (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

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
  )
{
  UINT32                  Dw0RegisterOffset;
  UINT32                  Dw0Value;
  P2SB_CONTROLLER         P2SbController;
  P2SB_PORT_16_ID         Pid;
  UINT32                  CommunityIndex;
  EFI_STATUS              Status;

  if (GpioPad == GPIOV2_PAD_NONE) {
    return EFI_INVALID_PARAMETER;
  }

  if (Direction == GpioV2DirDefault) {
    return EFI_SUCCESS;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  Status = GetP2SbControllerForCommunity (CommunityIndex, &P2SbController, &Pid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get P2SB controller for community %d\n", CommunityIndex));
    return Status;
  }

  //
  // Get DW0 Register Offset
  //
  PtlPcdMinGpioGetDw0RegOffset (GpioPad, &Dw0RegisterOffset);

  //
  // Read current DW0 value
  //
  Dw0Value = MmioRead32 (P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, Dw0RegisterOffset));

  //
  // Configure Direction and Input Inversion
  //
  switch (Direction) {
    case GpioV2DirIn:
      // Input: Enable RX, Disable TX, No Inversion
      Dw0Value |= B_GPIO_PCR_TXDIS;   // Disable TX
      Dw0Value &= ~B_GPIO_PCR_RXDIS;  // Enable RX
      Dw0Value &= ~B_GPIO_PCR_RXINV;  // Disable RX Inversion
      break;
    case GpioV2DirInInv:
      // Input with Inversion: Enable RX, Disable TX, Enable Inversion
      Dw0Value |= B_GPIO_PCR_TXDIS;   // Disable TX
      Dw0Value &= ~B_GPIO_PCR_RXDIS;  // Enable RX
      Dw0Value |= B_GPIO_PCR_RXINV;   // Enable RX Inversion
      break;
    case GpioV2DirOut:
      // Output: Disable RX, Enable TX, No Inversion
      Dw0Value &= ~B_GPIO_PCR_TXDIS;  // Enable TX
      Dw0Value |= B_GPIO_PCR_RXDIS;   // Disable RX
      Dw0Value &= ~B_GPIO_PCR_RXINV;  // Disable RX Inversion
      break;
    case GpioV2DirNone:
      // Neither: Disable both TX and RX, No Inversion
      Dw0Value |= B_GPIO_PCR_TXDIS;   // Disable TX
      Dw0Value |= B_GPIO_PCR_RXDIS;   // Disable RX
      Dw0Value &= ~B_GPIO_PCR_RXINV;  // Disable RX Inversion
      break;
    case GpioV2DirInOut:
      // Both: Enable TX and RX, No Inversion
      Dw0Value &= ~B_GPIO_PCR_TXDIS;  // Enable TX
      Dw0Value &= ~B_GPIO_PCR_RXDIS;  // Enable RX
      Dw0Value &= ~B_GPIO_PCR_RXINV;  // Disable RX Inversion
      break;
    case GpioV2DirInInvOut:
      // Both with Inversion: Enable TX and RX, Enable Inversion
      Dw0Value &= ~B_GPIO_PCR_TXDIS;  // Enable TX
      Dw0Value &= ~B_GPIO_PCR_RXDIS;  // Enable RX
      Dw0Value |= B_GPIO_PCR_RXINV;   // Enable RX Inversion
      break;
    default:
      break;
  }

  //
  // Write back the modified DW0 value
  //
  MmioWrite32 (P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, Dw0RegisterOffset), Dw0Value);

  return EFI_SUCCESS;
}

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
  )
{
  UINT32                  Dw0RegisterOffset;
  UINT32                  Dw0Value;
  UINT32                  AndValue;
  UINT32                  OrValue;
  P2SB_CONTROLLER         P2SbController;
  P2SB_PORT_16_ID         Pid;
  UINT32                  CommunityIndex;
  EFI_STATUS              Status;
  UINT8                   PadRstIndex;

  if (GpioPad == GPIOV2_PAD_NONE) {
    return EFI_INVALID_PARAMETER;
  }

  if (ResetConfig == GpioV2ResetDefault) {
    return EFI_SUCCESS;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  Status = GetP2SbControllerForCommunity (CommunityIndex, &P2SbController, &Pid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get P2SB controller for community %d\n", CommunityIndex));
    return Status;
  }

  //
  // Get DW0 Register Offset
  //
  PtlPcdMinGpioGetDw0RegOffset (GpioPad, &Dw0RegisterOffset);

  //
  // Read current DW0 value
  //
  Dw0Value = MmioRead32 (P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, Dw0RegisterOffset));

  //
  // Configure Reset Config
  // For PTL PCD PCIe SRCCLKREQ pads (GPP group), use GPP reset config mapping:
  // PadRstCfg[1:0]:  00b = Resume (Powergood)
  //                  01b = HostDeep (Deep GPIO Reset)
  //                  10b = Host (GPIO Reset) <- GpioV2ResetHost
  //                  11b = Global (Global Reset)
  //
  // Find the index in the mapping table
  for (PadRstIndex = 0; PadRstIndex <= GPIOV2_PAD_RESET_CONFIG_MASK; PadRstIndex++) {
    if (GppPadRstCfgMap[PadRstIndex] == ResetConfig) {
      break;
    }
  }
  
  if (PadRstIndex > GPIOV2_PAD_RESET_CONFIG_MASK) {
    DEBUG ((DEBUG_ERROR, "[%a] ResetConfig = 0x%x not found in mapping table!\n", __FUNCTION__, ResetConfig));
    return EFI_NOT_FOUND;
  }
  
  // Program the reset config field (bits 31:30)
  AndValue = (UINT32)~(B_GPIO_PCR_RST_CONF);
  OrValue = ((UINT32)PadRstIndex << N_GPIO_PCR_RST_CONF);
  Dw0Value = (Dw0Value & AndValue) | OrValue;

  //
  // Write back the modified DW0 value
  //
  MmioWrite32 (P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, Dw0RegisterOffset), Dw0Value);

  return EFI_SUCCESS;
}

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
  )
{
  UINT32               Dw0RegisterOffset;
  UINT32               Dw0Value;
  P2SB_CONTROLLER      P2SbController;
  P2SB_PORT_16_ID      Pid;
  UINT32               CommunityIndex;
  EFI_STATUS           Status;

  if ( InputState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  Status = GetP2SbControllerForCommunity (CommunityIndex, &P2SbController, &Pid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get P2SB controller for community %d\n", CommunityIndex));
    return Status;
  }

  //
  // Get DW0 Register Offset
  //
  PtlPcdMinGpioGetDw0RegOffset (GpioPad, &Dw0RegisterOffset);

  //
  // Read DW0 register value
  //
  Dw0Value = MmioRead32 (P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, Dw0RegisterOffset));

  //
  // Extract RX State (bit 1)
  //
  if (Dw0Value & B_GPIO_PCR_RX_STATE) {
    *InputState = GpioV2StateHigh;
  } else {
    *InputState = GpioV2StateLow;
  }

  return EFI_SUCCESS;
}

/**
  Atomically configure a GPIO pad: PadMode, OutputState, Direction, ResetConfig.
  This ensures glitch-free configuration for sensitive signals like CLKREQ#.

  @param[in] GpioPad      GPIO Pad
  @param[in] PadMode      Pad mode to set
  @param[in] OutputState  Output state (if output)
  @param[in] Direction    Direction to set
  @param[in] ResetConfig  Reset config to set
  @retval EFI_SUCCESS     The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid pad number
*/
EFI_STATUS
PtlPcdMinimalGpioConfigurePadAtomic(
  GPIOV2_PAD           GpioPad,
  UINT32               PadMode,
  GPIOV2_PAD_STATE     OutputState,
  GPIOV2_DIRECTION     Direction,
  GPIOV2_RESET_CONFIG  ResetConfig
  )
{
  EFI_STATUS           Status;
  UINT32               Dw0RegisterOffset;
  UINT32               Dw0Value;
  UINT32               CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX(GpioPad);
  P2SB_CONTROLLER      P2SbController;
  P2SB_PORT_16_ID      Pid;

  DEBUG ((DEBUG_INFO, "%a: Entry - GpioPad=0x%x, PadMode=0x%x, OutputState=%d, Direction=%d, ResetConfig=%d\n", __FUNCTION__, GpioPad, PadMode, OutputState, Direction, ResetConfig));
  // Set PadMode first
  Status = GpioMinSetPadMode(GpioPad, PadMode);
  DEBUG ((DEBUG_INFO, "%a: GpioMinSetPadMode Status=%r\n", __FUNCTION__, Status));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Set output state unconditionally (like SetTx)
  Status = GetP2SbControllerForCommunity(CommunityIndex, &P2SbController, &Pid);
  DEBUG ((DEBUG_INFO, "%a: GetP2SbControllerForCommunity Status=%r\n", __FUNCTION__, Status));
  if (EFI_ERROR(Status)) {
    return Status;
  }
  PtlPcdMinGpioGetDw0RegOffset(GpioPad, &Dw0RegisterOffset);
  Dw0Value = MmioRead32(P2SB_PCR_ADDRESS(P2SbController.Mmio, Pid.PortId.LocalPid, Dw0RegisterOffset));
  if (OutputState == GpioV2StateHigh) {
    Dw0Value |= B_GPIO_PCR_TX_STATE;
  } else {
    Dw0Value &= ~B_GPIO_PCR_TX_STATE;
  }
  MmioWrite32(P2SB_PCR_ADDRESS(P2SbController.Mmio, Pid.PortId.LocalPid, Dw0RegisterOffset), Dw0Value);
  DEBUG ((DEBUG_INFO, "%a: Set OutputState=%d on GpioPad=0x%x\n", __FUNCTION__, OutputState, GpioPad));

  // Set direction
  Status = PtlPcdMinimalGpioSetDirection(GpioPad, Direction);
  DEBUG ((DEBUG_INFO, "%a: PtlPcdMinimalGpioSetDirection Status=%r\n", __FUNCTION__, Status));
  if (EFI_ERROR(Status)) return Status;

  // Set reset config
  Status = PtlPcdMinimalGpioSetResetConfig(GpioPad, ResetConfig);
  DEBUG ((DEBUG_INFO, "%a: PtlPcdMinimalGpioSetResetConfig Status=%r\n", __FUNCTION__, Status));
  return Status;
}