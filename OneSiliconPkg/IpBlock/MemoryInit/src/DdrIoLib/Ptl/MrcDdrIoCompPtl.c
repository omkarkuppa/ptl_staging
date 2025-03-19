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
#include "MrcDdr5.h" // for TOdtValueDqDdr5

// RCOMP target values for { RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk } - per DDR Type
const UINT16 RcompTargetLpddr5Type3[MAX_RCOMP_TARGETS]    = { 39, 25, 30, 30, 30 };
const UINT16 RcompTargetLpddr5Type4_1R[MAX_RCOMP_TARGETS] = { 39, 25, 30, 30, 25 };
const UINT16 RcompTargetLpddr5Type4_2R[MAX_RCOMP_TARGETS] = { 39, 25, 30, 30, 30 };

const UINT16 RcompTargetPDdr5[MAX_RCOMP_TARGETS]          = { 60, 27, 25, 25, 25 };

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
  The function preforms a frequency switch Rcomp

  @param[in, out] MrcData - MRC global data.
**/
VOID
FreqSwitchComp (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput     *Outputs;
  INT64  PHClkDutyCycleEnable;
  INT64  PHClkPhaseEn;
  INT64  DataDccRankEn;
  INT64  ClkDccRankEn;
  INT64  WckDccRankEn;
  UINT32 Override;
  INT64  GetSetDis;
  UINT8  FirstController;
  UINT8  FirstChannel;
  UINT8  FirstRank[MAX_CONTROLLER][MAX_CHANNEL];
  DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT CompDqsDelayControl;

  Outputs           = &MrcData->Outputs;
  GetSetDis = 0;
  FirstController   = Outputs->FirstPopController;
  FirstChannel      = Outputs->Controller[FirstController].FirstPopCh;
  // Get the first rank index for each channel
  // per-channel results will be stored at this index
  GetFirstRank (MrcData, FirstRank);

  // Save Data
  MrcGetSetPartitionBlock (MrcData, PartitionPll, MRC_IGNORE_ARG, GsmDccPHClkDutyCycleEn, ReadFromCache , &PHClkDutyCycleEnable);
  MrcGetSetPartitionBlock (MrcData, PartitionPll, MRC_IGNORE_ARG, GsmDccPHClkPhaseEn, ReadFromCache , &PHClkPhaseEn);
  MrcGetSetChStrb (MrcData, FirstController, FirstChannel, 0, GsmDataDccRankEn, ReadFromCache , &DataDccRankEn);
  MrcGetSetCcc (MrcData, FirstController, FirstChannel, FirstRank[FirstController][FirstChannel], MRC_IGNORE_ARG, GsmClkDccRankEn, ReadFromCache , &ClkDccRankEn);
  MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmWckDccRankEn, ReadFromCache , &WckDccRankEn);
  CompDqsDelayControl.Data = MrcReadCR (MrcData, DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_REG);
  Override = CompDqsDelayControl.Bits.Override;

  // Disable FSM's to reduce unnecessary change
  MrcGetSetPartitionBlock (MrcData, PartitionPll, MRC_IGNORE_ARG, GsmDccPHClkDutyCycleEn, WriteToCache, &GetSetDis);
  MrcGetSetPartitionBlock (MrcData, PartitionPll, MRC_IGNORE_ARG, GsmDccPHClkPhaseEn, WriteToCache, &GetSetDis);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmDataDccRankEn, WriteToCache, &GetSetDis);
  MrcGetSetCcc (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, MRC_IGNORE_ARG, GsmClkDccRankEn, WriteToCache, &GetSetDis);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmWckDccRankEn, WriteToCache, &GetSetDis);

  MrcFlushRegisterCachedData (MrcData);

  CompDqsDelayControl.Bits.Override = 1;
  MrcWriteCrMulticast (MrcData, DATASHARED_CR_DDRCRCOMPDQSDELAYCONTROL_REG, CompDqsDelayControl.Data);

  ForceRcomp (MrcData, FullComp);

  // Restore Data
  MrcGetSetPartitionBlock (MrcData, PartitionPll, MRC_IGNORE_ARG, GsmDccPHClkDutyCycleEn, WriteToCache, &PHClkDutyCycleEnable);
  MrcGetSetPartitionBlock (MrcData, PartitionPll, MRC_IGNORE_ARG, GsmDccPHClkPhaseEn, WriteToCache, &PHClkPhaseEn);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmDataDccRankEn, WriteToCache, &DataDccRankEn);
  MrcGetSetCcc (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, MRC_IGNORE_ARG, GsmClkDccRankEn, WriteToCache, &ClkDccRankEn);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmWckDccRankEn, WriteToCache, &WckDccRankEn);

  MrcFlushRegisterCachedData (MrcData);

  CompDqsDelayControl.Bits.Override = Override;
  MrcWriteCrMulticast (MrcData, DATASHARED_CR_DDRCRCOMPDQSDELAYCONTROL_REG, CompDqsDelayControl.Data);
}



/**
  Calculate the frequency using QclkRatio and gear set in PHY WORKPOINT0 register

  @param[in]      MrcData      - Include all MRC global data.
  @param[in, out] MemoryClock  - The current memory clock.
  @param[in, out] Ratio        - The current memory ratio setting.

  @retval: The current memory frequency.
**/
MrcFrequency
MrcGetPhyCurrentMemoryFrequency (
  MrcParameters* const   MrcData,
  UINT32* const          MemoryClock,
  MrcClockRatio* const   Ratio
  )
{
  INT64                      GetSetVal;
  UINT32                     QclkRatioData;
  INT64                      QclkGearData;
  MrcOutput                  *Outputs;
  MrcSaGvPoint               SaGvPoint;

  Outputs = &MrcData->Outputs;
  SaGvPoint = Outputs->SaGvPoint;

  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmWorkPointRatio, ReadCached | PrintValue, &GetSetVal);
  QclkRatioData = (UINT32) GetSetVal;
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmWorkPointGear4, ReadCached | PrintValue, &QclkGearData);

  if (QclkGearData == 0) {
    QclkRatioData *= 2; // In gear2 the actual ratio is twice the ratio in the register
  }
  if (QclkGearData == 1) {
    QclkRatioData *= 4; // In gear4 the actual ratio is 4x the ratio in the register
  }

  if (MemoryClock != NULL) {
    *MemoryClock = MrcRatioToClock(MrcData, (MrcClockRatio)QclkRatioData);
  }
  if (Ratio != NULL) {
    *Ratio = (MrcClockRatio)QclkRatioData;
  }

  return MrcRatioToFrequency (
    MrcData,
    (MrcClockRatio)QclkRatioData
  );
}
