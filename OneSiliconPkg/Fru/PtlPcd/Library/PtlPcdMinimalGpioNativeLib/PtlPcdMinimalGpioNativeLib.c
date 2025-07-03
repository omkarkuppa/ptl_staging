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

#include <Library/Ptl/PcdMinimalGpioNativeLib/PtlPcdMinimalGpioNativeLib.h>
#include <Library/Ptl/PtlPcdP2SbSocLib.h>
#include <GpioV2ControllerInterface.h>
#include <Library/PcdInfoLib.h>

/**
 * Get the Gpio Community
 *
 * @param CommunityIndex       Community Index
 * @return GPIOV2_COMMUNITY*
 */
GPIOV2_COMMUNITY*
GetGpioCommunity (
  UINT32 CommunityIndex
  )
{
  return PtlPcdGpioGetCommunity (CommunityIndex);
}

/**
 * Get the Number of GPIO Communities.
 *
 * @return * UINT32
 */
UINT32
GetCommunitiesNum (
  VOID
  )
{
  return PtlPcdGpioGetCommunitiesNum ();
}

/**
 * Get the Gpio Topology
 *
 * @return GPIOV2_COMMUNITY*
 */
GPIOV2_COMMUNITY*
GetGpioTopology (
  VOID
  )
{
  return PtlPcdGpioGetCommunities ();
}

/**
 * Get Gpio Native Signals.
 *
 * @return GPIOV2_NATIVE_SIGNAL_DATA*
 */
GPIOV2_NATIVE_SIGNAL_DATA*
GetNativeSignals (
  VOID
  )
{
  return PtlPcdGpioGetNativeSignals ();
}

/**
 * Get Gpio Native Signals Number.
 *
 * @return UINT32
 */
UINT32
GetNativeSignalsNum (
  VOID
  )
{
  return PtlPcdGpioGetNativeSignalsNum ();
}

/**
  Checks if the given GPIO pad is valid.

  @param[in]  GpioPad         GPIO pad to validate.
  @param[out] IsValid         TRUE if pad is valid, FALSE otherwise.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
IsPadValid (
  IN  GPIOV2_COMMUNITY  *GpioCommunity,
  IN  GPIOV2_PAD        GpioPad
  )
{
  UINT32 GroupIndex;
  UINT32 PadIndex;


  if (GpioCommunity == NULL) {
    ASSERT(FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GroupIndex     = GPIOV2_PAD_GET_GROUP_INDEX (GpioPad);
  PadIndex       = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  if (GroupIndex >= GpioCommunity->GroupsNum) {
    return EFI_INVALID_PARAMETER;
  }
  if (PadIndex >= GpioCommunity->Groups[GroupIndex].PadsNum) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
  Get GPIO pad data from the GPIO community.

  @param[in]  GpioCommunity   Pointer to the GPIO community.
  @param[in]  GpioPad         The GPIO pad to get data for.
  @param[out] GpioPadData     Pointer to the structure that will hold the pad data.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  */
EFI_STATUS
EFIAPI
GetPadDataFromPad (
  IN  GPIOV2_COMMUNITY  *GpioCommunity,
  IN  GPIOV2_PAD        GpioPad,
  OUT GPIOV2_PAD_DATA   **GpioPadData
  )
{
  UINT32           CommunityIndex;
  UINT32           GroupIndex;
  UINT32           PadIndex;

  if (GpioCommunity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CommunityIndex      = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  GroupIndex          = GPIOV2_PAD_GET_GROUP_INDEX     (GpioPad);
  PadIndex            = GPIOV2_PAD_GET_PAD_INDEX(GpioPad);

  if (PadIndex >= GpioCommunity[CommunityIndex].Groups[GroupIndex].PadsNum) {
    return EFI_INVALID_PARAMETER;
  }

  *GpioPadData = &GpioCommunity[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex];

  return EFI_SUCCESS;
}

/**
  Set GPIO pad mode for given pad.

  @param[in] GpioCommunity   Pointer to the GPIO community.
  @param[in] GpioPad         The GPIO pad to set mode for.
  @param[in] PadMode         The mode to set for the GPIO pad.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host
  */
EFI_STATUS
EFIAPI
SetPadMode (
  IN GPIOV2_COMMUNITY  *GpioCommunity,
  IN GPIOV2_PAD        GpioPad,
  IN GPIOV2_PAD_MODE   PadMode
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS   CommunityAccess;
  P2SB_CONTROLLER                 P2SbController;
  EFI_STATUS                      Status;
  UINT32                          RegisterOffset;
  UINT32                          GroupIndex;
  UINT32                          PadIndex;
  UINT32                          RegisterValue;
  UINT32                          AndValue;
  UINT32                          OrValue;

  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  RegisterOffset = GpioCommunity->Groups[GroupIndex].RegisterOffsets.PadOwn + (PadIndex * 0x04);

  Status = PtlPcdGetFirstP2SbController (&P2SbController);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get P2SB controller, Status = %r\n", __FUNCTION__, Status));
    ASSERT(FALSE);
    return Status;
  }

  BuildP2SbSidebandAccess (
    &P2SbController,
    (P2SB_PID) GpioCommunity->Pid,
    0,
    P2SbMemory,
    P2SbMmioAccess,
    TRUE,
    &CommunityAccess
  );

  RegisterValue = CommunityAccess.Access.Read32 (
    &CommunityAccess.Access,
    RegisterOffset
  );

  if ((GPIOV2_PAD_OWN)(((RegisterValue & GPIOV2_PAD_OWNERSHIP_MASK_REV1) << 1) | 0x01) == GpioV2PadOwnHost) {
    AndValue = (UINT32)~(GPIOV2_PAD_MODE_MASK << GPIOV2_PAD_MODE_DW0_POS);
    OrValue  = ((PadMode >> 1) & GPIOV2_PAD_MODE_MASK) << GPIOV2_PAD_MODE_DW0_POS;
    RegisterOffset = GpioCommunity->Groups[GroupIndex].RegisterOffsets.Dw0 + (PadIndex * 0x04);
    CommunityAccess.Access.AndThenOr32 (
      &CommunityAccess.Access,
      RegisterOffset,
      AndValue,
      OrValue
    );
    return EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_ERROR, "%a: Pad is not owned by Host, access denied\n", __FUNCTION__));
    return EFI_ACCESS_DENIED;
  }
}

/**
  Get signal information for a given signal.

  @param[in]  Signal          The GPIO signal to get information for.
  @param[out] SignalData      Pointer to the structure that will hold the signal data.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
  @retval EFI_NOT_FOUND           Signals not found
  */
EFI_STATUS
EFIAPI
GetSignalInfoForSignal (
  IN  UINT32                     Signal,
  OUT GPIOV2_NATIVE_SIGNAL_DATA  *SignalData
  )
{
  GPIOV2_NATIVE_SIGNAL_DATA      *NativeSignalData;
  GPIOV2_NATIVE_SIGNAL_DATA       *SignalDataIter;
  UINT32                         Index;
  UINT32                         NativeSignalsCount;

  if (SignalData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NativeSignalData = GetNativeSignals ();
  if (NativeSignalData == NULL) {
    return EFI_NOT_FOUND;
  }

  NativeSignalsCount = GetNativeSignalsNum ();

  for (Index = 0; Index < NativeSignalsCount; Index++) {
    SignalDataIter = &NativeSignalData[Index];
    if (SignalDataIter->Signal.Value == Signal) {
      *SignalData = *SignalDataIter;
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

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
  EFI_STATUS                      Status;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   CommunityAccess;
  GPIOV2_COMMUNITY                *GpioCommunity;
  UINT32                          MiscCfg;
  UINT32                          CommunityIndex;

  Status = PtlPcdGetFirstP2SbController (&P2SbController);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get P2SB controller, Status = %r\n", __FUNCTION__, Status));
    ASSERT(FALSE);
    return Status;
  }

  for (CommunityIndex = 0; CommunityIndex < GetCommunitiesNum (); CommunityIndex++) {
    GpioCommunity = GetGpioCommunity(CommunityIndex);
    if (GpioCommunity == NULL) {
      DEBUG ((DEBUG_ERROR, "%a: GpioCommunity is NULL for CommunityIndex = %u\n", __FUNCTION__, CommunityIndex));
      ASSERT(FALSE);
      return EFI_INVALID_PARAMETER;
    }

    BuildP2SbSidebandAccess (
      &P2SbController,
      (P2SB_PID) GpioCommunity->Pid,
      0,
      P2SbMemory,
      P2SbMmioAccess,
      TRUE,
      &CommunityAccess
    );

    MiscCfg = CommunityAccess.Access.Read32 (&CommunityAccess.Access, R_GPIO_PCR_MISCCFG);

    MiscCfg &= (UINT32)~(B_GPIO_PCR_MISCCFG_IRQ_ROUTE);
    MiscCfg |= (UINT32)Irq << N_GPIO_PCR_MISCCFG_IRQ_ROUTE;

    CommunityAccess.Access.Write32 (
      &CommunityAccess.Access,
      R_GPIO_PCR_MISCCFG,
      MiscCfg
    );
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
  EFI_STATUS                      Status;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   CommunityAccess;
  GPIOV2_COMMUNITY                *GpioCommunity;
  UINT32                          MiscCfg;
  UINT32                          CommunityIndex;

  Status = PtlPcdGetFirstP2SbController (&P2SbController);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get P2SB controller, Status = %r\n", __FUNCTION__, Status));
    ASSERT(FALSE);
    return Status;
  }

  for (CommunityIndex = 0; CommunityIndex < GetCommunitiesNum (); CommunityIndex++) {
    GpioCommunity = GetGpioCommunity(CommunityIndex);
    if (GpioCommunity == NULL) {
      DEBUG ((DEBUG_ERROR, "%a: GpioCommunity is NULL for CommunityIndex = %u\n", __FUNCTION__, CommunityIndex));
      ASSERT(FALSE);
      return EFI_INVALID_PARAMETER;
    }

    BuildP2SbSidebandAccess (
      &P2SbController,
      (P2SB_PID) GpioCommunity->Pid,
      0,
      P2SbMemory,
      P2SbMmioAccess,
      TRUE,
      &CommunityAccess
    );

    MiscCfg = CommunityAccess.Access.Read32 (&CommunityAccess.Access, R_GPIO_PCR_MISCCFG);

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

    CommunityAccess.Access.Write32 (
      &CommunityAccess.Access,
      R_GPIO_PCR_MISCCFG,
      MiscCfg
    );
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
  EFI_STATUS                  Status;
  GPIOV2_PAD_DATA             *GpioPadData;
  UINT32                      Index;
  UINT32                      CommunityIndex;
  GPIOV2_NATIVE_SIGNAL_DATA   SignalData;
  GPIOV2_COMMUNITY            *GpioCommunity;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if ((GpioPadNum == 0) || (PortId == NULL) || (Bits == NULL) || (VwIndex == NULL)) {
    //
    // The paratemeter data is invalid.
    //
    return;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPadNum);

  if (CommunityIndex >= GetCommunitiesNum()) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR Wrong Gpio community of gpio pad 0x%x\n", __FUNCTION__, GpioPadNum));
    return;
  }

  GpioCommunity = GetGpioCommunity (CommunityIndex);

  Status = IsPadValid (GpioCommunity, GpioPadNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR Invalid GPIO pad definition 0x%x\n", __FUNCTION__, GpioPadNum));
    return;
  }

  Status = GetPadDataFromPad (GpioCommunity, GpioPadNum, &GpioPadData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR getting pad data for gpio 0x%x\n", __FUNCTION__, GpioPadNum));
    return;
  }

  for (Index = 0; Index < GpioPadData->PadSignalsNum; Index++) {
    if (GpioPadData->PadSignals[Index].Signal.Fields.Interface == GPIOV2_PERIPHERAL_INTERFACE_IOM) {
      Status = SetPadMode (GpioCommunity, GpioPadNum, GpioPadData->PadSignals[Index].PadMode);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR failed to set the pad mode\n", __FUNCTION__));
        return;
      }

      Status = GetSignalInfoForSignal (GpioPadData->PadSignals[Index].Signal.Value, &SignalData);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR getting signal info\n", __FUNCTION__));
        return;
      }

      *PortId = (UINT16)GpioCommunity->Pid;
      *Bits = (UINT8)SignalData.BitPosition;
      *VwIndex = (UINT8)SignalData.VwIndex;
      break;
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
  GPIOV2_COMMUNITY                *GpioCommunity;
  UINT32                          GpioDw2Value;
  UINT32                          GpioDw2Offset;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   CommunityAccess;
  EFI_STATUS                      Status;
  UINT32                          CommunityIndex;
  GPIOV2_PAD                      CrfStrapPin;
  UINT32                          GroupIndex;
  UINT32                          PadIndex;

    CrfStrapPin = GPIOV2_PTL_PCD_XXGPP_F_2;

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (CrfStrapPin);
  if (CommunityIndex >= GetCommunitiesNum ()) {
    DEBUG((DEBUG_ERROR, "Invalid Community Index %x for CrfStrapPin %x\n", CommunityIndex, CrfStrapPin));
    return FALSE;
  }

  GpioCommunity = GetGpioCommunity (CommunityIndex);

  if (GpioCommunity == NULL) {
    DEBUG((DEBUG_ERROR, "GpioCommunity is NULL for CrfStrapPin %x\n", CrfStrapPin));
    return FALSE;
  }

  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (CrfStrapPin);
  if (GroupIndex >= GpioCommunity->GroupsNum) {
    DEBUG((DEBUG_ERROR, "Invalid Group Index %x for CrfStrapPin %x\n", GroupIndex, CrfStrapPin));
    return FALSE;
  }

  Status = PtlPcdGetFirstP2SbController (&P2SbController);
  DEBUG((DEBUG_INFO, "%a: PtlPcdGetFirstP2SbController Status = %r\n", __FUNCTION__, Status));
  if (EFI_ERROR(Status)) {
    ASSERT(FALSE);
    return FALSE;
  }

  BuildP2SbSidebandAccess (
    &P2SbController,
    (P2SB_PID) GpioCommunity->Pid,
    0,
    P2SbMemory,
    P2SbMmioAccess,
    TRUE,
    &CommunityAccess
  );

  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (CrfStrapPin);

  GpioDw2Offset = GpioCommunity->Groups[GroupIndex].RegisterOffsets.Dw0 + 0x08 + PadIndex * 0x10;

  // Reading register value
  GpioDw2Value = CommunityAccess.Access.Read32 (
    &CommunityAccess.Access,
    GpioDw2Offset
    );
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