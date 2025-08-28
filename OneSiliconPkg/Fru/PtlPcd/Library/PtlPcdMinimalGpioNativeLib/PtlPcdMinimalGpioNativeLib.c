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
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/IoLib.h>
#include "Register/GpioRegs.h"
#include "PtlPcdMinimalGpio.h"

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