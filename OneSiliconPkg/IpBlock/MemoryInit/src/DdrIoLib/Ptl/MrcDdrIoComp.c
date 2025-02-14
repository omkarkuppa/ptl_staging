/** @file
  This file contains code related to initializing and configuring the Rcomp

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

#include "MrcCommon.h"
#include "MrcMemoryApi.h"
#include "MrcDdrIoUtils.h"
#include "MrcDdrCommon.h"
#include "MrcDdrIoComp.h"
#include "MrcCrosser.h"

// RCOMP target values for { RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk } - per DDR Type
const UINT16 RcompTargetLpddr5Type3[MAX_RCOMP_TARGETS]    = { 40, 27, 30, 30, 30 };
const UINT16 RcompTargetLpddr5Type4_1R[MAX_RCOMP_TARGETS] = { 40, 27, 30, 30, 25 };
const UINT16 RcompTargetLpddr5Type4_2R[MAX_RCOMP_TARGETS] = { 40, 27, 30, 30, 30 };

const UINT16 RcompTargetPDdr5[MAX_RCOMP_TARGETS]          = { 60, 27, 25, 25, 25 };

const UINT8 CompParamList[] = { RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk };

/**
  This function determines default RcompTargets based on CPU type and DDR Type

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Profile - MemoryProfile

  @retval Returns pointer to RcompTarget
**/
const UINT16 *
MrcGetDefaultRcompTarget (
  IN OUT MrcParameters *const MrcData,
  IN     MrcProfile Profile
  )
{
  const MrcInput        *Inputs;
  const MrcBoardInputs  *BoardDetails;
  MrcOutput             *Outputs;
  MrcDebug              *Debug;
  BOOLEAN               IsTypicalBoard;
  const UINT16          *RcompTarget;

  Inputs          = &MrcData->Inputs;
  BoardDetails    = &Inputs->ExtInputs.Ptr->BoardDetails;
  IsTypicalBoard  = BoardDetails->BoardStackUp == 0;
  Outputs         = &MrcData->Outputs;
  Debug           = &Outputs->Debug;
  RcompTarget     = NULL;

  /*
   * Use DdrType as index to find the default RcompTarget
   */
  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      if (IsTypicalBoard) {
        RcompTarget = (Outputs->MaxRanks == 2) ? RcompTargetLpddr5Type4_2R : RcompTargetLpddr5Type4_1R;
      } else {
        RcompTarget = RcompTargetLpddr5Type3;
      }
      break;

    case MRC_DDR_TYPE_DDR5:
      RcompTarget = RcompTargetPDdr5;
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s: %d\n", gErrString, gUnsupportedTechnology, Outputs->DdrType);
      break;
  }

  return RcompTarget;
}

/**
  This function calculates the Slew Rate target CR's based on the requested
  slope. It does not run a compensation cycle and does not do limit checking of the CR range.

    ScompLockPerLimit = 16 * TargetSlope
    ScompLockCycles   = (2 * QClk < ScompLockPerLimit) ? 4 : ( (QClk < ScompLockPerLimit) ? 2 : 1 )
    ScompCells        = MIN (15, RndDn (ScompLockCycles * QClk / ( 2 * TargetSlope)) - 1)
    PhaseCycle        = (ScompLockCycles == 1) ? 0 : 1
    CompDiv2En        = (ScompLockCycles == 4) ? 1 : 0

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  SlopePs    - Slew Rate slope in picoseconds.
  @param[out] CompCells  - Return location for calculated Comp Cells value.
  @param[out] CompPC     - Return location for calculated Phase/Cycle config.
  @param[out] CompDiv2En - Return location for calculated clock divider config.

  @retval MrcStatus - mrcWrongInputParameter if CompCells, CompPC or CompDiv2En is NULL.
                    - mrcFail if Target SlopePs is too low to be supported in 16 delay cells.
                    - mrcSuccess otherwise.
**/
MrcStatus
MrcCalcScomp (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                SlopePs,
  OUT UINT32                *CompCells,
  OUT UINT32                *CompPC,
  OUT UINT32                *CompDiv2En
  )
{
  UINT32 QclkPs;
  UINT32 ScompLockPerLimit;
  UINT32 ScompLockCycles;

  ScompLockCycles = 0;

  if ((CompCells == NULL) || (CompDiv2En == NULL) || (CompPC == NULL)) {
    MRC_DEBUG_ASSERT (1==0, &MrcData->Outputs.Debug, "%s: %s\n", __FUNCTION__, gNullPtrErrStr);
    return mrcWrongInputParameter;
  }

  QclkPs = MrcData->Outputs.Qclkps;
  ScompLockPerLimit = 16 * SlopePs;

  if ((2 * QclkPs) < ScompLockPerLimit) {
    ScompLockCycles = 4;
    *CompDiv2En = 1;
    *CompPC = 1;
  } else if (QclkPs < ScompLockPerLimit) {
    ScompLockCycles = 2;
    *CompDiv2En = 0;
    *CompPC = 1;
  } else if (DIVIDECEIL (QclkPs, 2) < ScompLockPerLimit) {
    ScompLockCycles = 1;
    *CompDiv2En = 0;
    *CompPC = 0;
  } else {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Invalid Scomp Target: Target value is too low to support in 16 delay cells\n");
    return mrcFail;
  }

  *CompCells = UDIVIDEROUND ((ScompLockCycles * QclkPs), (2 * SlopePs)) - 1;
  *CompCells = MIN (*CompCells, 15);

  return mrcSuccess;
}

/**
  This function runs Comp Update and then a Full Comp cycle.

  @param[in] MrcData  - Pointer to global MRC data.
  @param[in] CompType - Enum of Comp being forced

  @retval    MrcStatus - Return status from ForceRcomp.
**/
MrcStatus
MrcDccForceComp (
  IN MrcParameters *const  MrcData,
  IN COMP_CYCLE_TYPE       CompType
  )
{
  MrcStatus  Status;

  Status = ForceRcomp (MrcData, CompUpdate);
  Status |= ForceRcomp (MrcData, CompType);

  return Status;
}


/**
  This function prints DdrIo COMP registers related to VccDdq.

  @param[in] MrcData - All the MRC global data.

**/
VOID
MrcPrintDdrIoCompVddq (
  IN MrcParameters* const MrcData
)
{
  MrcDebug* Debug;
  MrcOutput* Outputs;
  INT16       CompCodeUp;
  INT16       CompCodeDn;
  INT64       GetSetVal;
  GSM_GT      GlobalCompUp;
  GSM_GT      GlobalCompDn;
  UINT32      Index;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

  // Walk backwards here to match the earlier flow
  for (Index = (sizeof(CompParamList) / sizeof(CompParamList[0])); Index > 0; Index--) {
    // Return the new comp code (hardware output)
    switch (CompParamList[Index - 1]) {
      case RdOdt:
        GlobalCompUp = CompRcompOdtUp;
        GlobalCompDn = CompRcompOdtDn;
        break;
      case WrDS:
        GlobalCompUp = TxRonUp;
        GlobalCompDn = TxRonDn;
        break;
      case WrDSClk:
        GlobalCompUp = WrDSCodeUpClk;
        GlobalCompDn = WrDSCodeDnClk;
        break;
      case WrDSCmd:
        GlobalCompUp = WrDSCodeUpCmd;
        GlobalCompDn = WrDSCodeDnCmd;
        break;
      case WrDSCtl:
      default:
        GlobalCompUp = WrDSCodeUpCtl;
        GlobalCompDn = WrDSCodeDnCtl;
        break;
    }

    if (GlobalCompUp == CompRcompOdtUp) {
      // In Vss mode only odt down is valid
      if (MrcData->Outputs.OdtMode == MrcOdtModeVss) {
        GlobalCompUp = GlobalCompDn;
      } else {
        GlobalCompDn = GlobalCompUp;
      }
    }
    MrcGetSetNoScope(MrcData, GlobalCompUp, ReadUncached, &GetSetVal);
    CompCodeUp = (INT16)GetSetVal;
    MrcGetSetNoScope(MrcData, GlobalCompDn, ReadUncached, &GetSetVal);
    CompCodeDn = (INT16)GetSetVal;

    //To save code space, RdOdt parameter will print out Up and Down even though only one is valid
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  %8s:  Current Comp code (Up/Down): %3d/%3d\n",
      GlobalCompOffsetStr[CompParamList[Index - 1]],
      CompCodeUp,
      CompCodeDn
    );

    if ((CompCodeUp <= 0) || (CompCodeUp >= 63) || (CompCodeDn <= 0) || (CompCodeDn >= 63)) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "WARNING: COMP code is saturated !\n");
    }
  }
}