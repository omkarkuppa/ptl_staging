/** @file
  This file contains all the MRC Sagv Programming

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

#include "CMrcTypes.h"
#include "MrcCommon.h"
#include "MrcSagv.h"
#include "MrcGeneral.h"
#include "MrcMemoryApi.h"
#include "MrcSaveRestore.h"
#include "MrcLpddr5.h"
#include "MrcPmaApi.h"
#include "MrcDdr5.h"
#include "MrcChipRouting.h"
#include "MrcRowHammer.h"

extern MrcFrequency Lp5SupportedFrequencies[];
extern MrcFrequency Ddr5SupportedFrequencies[];
extern UINT8 Lp5SupportedFrequenciesSize;
extern UINT8 Ddr5SupportedFrequenciesSize;

/**
  Takes in an input memory frequency (data rate) and calculates next supported frequency
  from the 'Supported Frequencies' tables that is less than or equal to the input frequency.

  Returns the input Freq value if the input Freq is lower than the min supported frequency.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Freq    - The frequency to

  @retval The supported frequency <= input Freq
**/
MrcFrequency
MrcGetNextSupportedFreq (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Freq
  )
{
  MrcOutput     *Outputs;
  BOOLEAN       IsLpddr5;
  BOOLEAN       IsDdr5;
  MrcFrequency  FreqOut;
  UINT32        Index;

  Outputs  = &MrcData->Outputs;
  IsLpddr5 = Outputs->IsLpddr5;
  IsDdr5   = Outputs->IsDdr5;

  FreqOut = Freq;
  if (IsLpddr5) {
    for (Index = 0; Index < Lp5SupportedFrequenciesSize; Index++) {
      if (Lp5SupportedFrequencies[Index] <= Freq) {
        FreqOut = Lp5SupportedFrequencies[Index];
        break;
      }
    }
  } else if (IsDdr5) {
    for (Index = 0; Index < Ddr5SupportedFrequenciesSize; Index++) {
      if (Ddr5SupportedFrequencies[Index] <= Freq) {
        FreqOut = Ddr5SupportedFrequencies[Index];
        break;
      }
    }
  }

  return FreqOut;
}

/**
  This function returns the current configuration of Frequency and Gear ratio based on the
  current SAGV point, DDR type, and SAGV input parameters: SaGvFreq, SaGvGear.

  It will update FreqOut and GearOut with the result.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  SaGvPoint   - Current operating SAGV point.
  @param[in]  FreqMax     - The maximum memory frequency allowed considering fuse, SPD, and topology limitations
  @param[in]  MaxQclkFreq - The maximum QCLK frequency supporte by the current hardware
  @param[out] FreqOut     - Pointer to return the SAGV point Frequency.
  @param[out] GearOut     - Pointer to return the SAGV point Gear.
**/
VOID
MrcGetSagvConfig (
  IN  MrcParameters *const  MrcData,
  IN  MrcSaGvPoint          SaGvPoint,
  IN  MrcFrequency          FreqMax,
  IN  UINT32                MaxQclkFreq,
  OUT MrcFrequency          *FreqOut,   OPTIONAL
  OUT BOOLEAN               *GearOut    OPTIONAL
  )
{
  MrcInput    *Inputs;
  MrcOutput   *Outputs;
  MrcDdrType  DdrType;
  SaGvTypes   SaGvType;
  UINT32      GearRatio;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
#ifdef MRC_DEBUG_PRINT
  MrcDebug    *Debug;
#endif

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  DdrType = Outputs->DdrType;
  ExtInputs = Inputs->ExtInputs.Ptr;
#ifdef MRC_DEBUG_PRINT
  Debug   = &Outputs->Debug;
#endif

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SAGV point %u\n", SaGvPoint);
  if (FreqOut == NULL) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s is NULL\n", gErrString, "FreqOut");
  } else if (DdrType >= MRC_DDR_TYPE_UNKNOWN) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: DdrType is %d\n", gWrongInputParam, DdrType);
  } else {
    if (ExtInputs->SaGvFreq[SaGvPoint]) {
      *FreqOut = ExtInputs->SaGvFreq[SaGvPoint];
    } else {
      SaGvType = MrcGetSaGvPointType (MrcData, SaGvPoint, FreqMax);
      MrcCalcSagvTypeConfig (MrcData, SaGvType, FreqMax, MaxQclkFreq, FreqOut, NULL);
      MRC_DEBUG_ASSERT (*FreqOut != 0, Debug, "Invalid SAGV %s: Point %d %s: %d\n", "Freq", SaGvPoint, "Freq", *FreqOut);
    }
  }

  if (GearOut == NULL) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s is NULL\n", gErrString, "GearOut");
  } else if (DdrType >= MRC_DDR_TYPE_UNKNOWN) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: DdrType is %d\n", gWrongInputParam, DdrType);
  } else {
    if (ExtInputs->SaGvGear[SaGvPoint]) {
      *GearOut = (ExtInputs->SaGvGear[SaGvPoint] == 4);
    } else {
      SaGvType = MrcGetSaGvPointType (MrcData, SaGvPoint, FreqMax);
      MrcCalcSagvTypeConfig (MrcData, SaGvType, FreqMax, MaxQclkFreq, NULL, &GearRatio);
      *GearOut = (GearRatio == 4);
      MRC_DEBUG_ASSERT (GearRatio != 0, Debug, "Invalid SAGV %s: Point %d %s: %d\n", "Gear", SaGvPoint, "Gear", GearRatio);
    }
    Outputs->GearMode = *GearOut;
  }
}

/**
  Determine if the SaGv configuration is allowed.

  @param[in] MrcData - include all the MRC general data.

  @retval mrcSuccess - If SaGv Config is allowed
          mrcFailure - Anything else
**/
MrcStatus
MrcSaGvBoundsCheck (
  IN  MrcParameters *const  MrcData
  )
{
  MrcStatus           Status;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  BOOLEAN             IsSaGvFixedPoint;
  UINT8               Index;
  static const UINT8  LegalBounds[] = {
    MRC_SAGV_MASK_0_1,
    MRC_SAGV_MASK_0_1_2,
    MRC_SAGV_MASK_0_1_2_3
  };

  Outputs           = &MrcData->Outputs;
  Debug             = &Outputs->Debug;
  Status            = mrcFail;
  IsSaGvFixedPoint  = MrcIsSaGvFixed (MrcData);

  if (!MrcIsSaGvEnabled (MrcData)) {
#ifdef MRC_DEBUG_PRINT
    // Disabled and not a single point: Warn user there selection will be ignored
    if (!IsSaGvFixedPoint) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SAGV is Disabled: SaGv WP Mask is ignored\n");
    }
#endif
    // Disabled and a single point: Success
    return mrcSuccess;
  }

  // Enabled and a Single Point: Fail
  if (IsSaGvFixedPoint) {
    MRC_DEBUG_ASSERT (FALSE, Debug, "\nMRC SaGv Bounds Check Failed: Enabled and a single point.\n");
    return mrcFail;
  }

  // Enabled and not a single point: legal point logic
  for (Index = 0; Index < ARRAY_COUNT(LegalBounds); Index++) {
    if (LegalBounds[Index] == MrcData->Inputs.ExtInputs.Ptr->SaGvWpMask) {
      Status = mrcSuccess;
      break;
    }
  }

  return Status;
}

/**
  Determine if SaGvWpMask is a fixed point.

  @param[in] MrcData - include all the MRC general data.

  @retval TRUE  - If SaGv is Fixed Point.
          FALSE - If SaGv is anything else.
**/
BOOLEAN
MrcIsSaGvFixed (
  IN  MrcParameters *const  MrcData
  )
{
  const MrcInput  *Inputs;
  MRC_EXT_INPUTS_TYPE  *ExtInputs;

  Inputs  = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  if ((ExtInputs->SaGvWpMask == 0) || (MrcCountBitsEqOne (ExtInputs->SaGvWpMask) == 1)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Save the effective SaGv configuration for the current SaGv Point
  for output to the wrapper.

  @param[in,out] MrcData - include all the MRC general data.
**/
void
MrcSaveSagvOutputs (
  IN OUT MrcParameters *const  MrcData
  )
{
  const MrcInput    *Inputs;
  MrcOutput     *Outputs;
  MrcSaGvOutput *SaGvOutputs;
  INT64         MaxMemoryBandwidth;
  const MRC_FUNCTION      *MrcCall;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  SaGvOutputs = &MrcData->Save.Data.SaGvOutputs;

  MrcCall = Inputs->Call.Func;

  SaGvOutputs->SaGvTiming[Outputs->SaGvPoint].DataRate = Outputs->Frequency;

  MrcGetSetFreqIndex (MrcData, Outputs->SaGvPoint, GsmPmaMaxBandwidth, ReadNoCache, &MaxMemoryBandwidth);
  SaGvOutputs->SaGvTiming[Outputs->SaGvPoint].MaxMemoryBandwidth = (UINT16) MaxMemoryBandwidth;

  // Assume that all channels use the same timings
  MrcCall->MrcCopyMem (
    (UINT8 *) &SaGvOutputs->SaGvTiming[Outputs->SaGvPoint].JedecTiming,
    (UINT8 *) &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile],
    sizeof (MrcTiming)
    );

  // Save the effective SaGv configuration for output to the wrapper
  SaGvOutputs->SaGvPointMask |= (MRC_BIT0 << Outputs->SaGvPoint);
  SaGvOutputs->NumSaGvPointsEnabled = MrcCountBitsEqOne (SaGvOutputs->SaGvPointMask);
}

/**
  SA GV flow for the cold boot

  @param[in] MrcData - include all the MRC general data.

  @retval mrcStatus if succeeded.
**/
MrcStatus
MrcSaGvSwitch (
  IN  MrcParameters *const  MrcData
  )
{
  MrcDebug            *Debug;
  MrcOutput           *Outputs;
  MrcStatus           Status;
  UINT32              SBClock;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  DDRPHY_MISC_SAUG_CR_PHYPMOVRD_STRUCT  PhyPmOvrd;

  Outputs     = &MrcData->Outputs;
  ExtInputs   = MrcData->Inputs.ExtInputs.Ptr;
  Debug       = &Outputs->Debug;
  Status      = mrcSuccess;

  if (ExtInputs->TargetedRowRefreshMode < TrrDisabled || ExtInputs->DramRfmMode < DramRfmDisabled) {
    MrcRhPtrrLfsrConfig (MrcData);
  }

  // Send PM14 to initiate save MC reg values to MC's SRAM
  Status = MrcSendPmMessage (MrcData, PM_MESSAGE_14, PM_MSG_MC);
  if (Status != mrcSuccess) {
    return Status;
  }

  // Send PM14 to initiate save of PHY reg values to PHY's SRAM
  Status = MrcSendPmMessage (MrcData, PM_MESSAGE_14, PM_MSG_PHY);
  // Reset GsmSkipRetentionCR, PchannelEn, GsmSaxgEnable at Last SAGV point or within the same boot if SAGV is disabled
  if ((Outputs->SaGvPoint == Outputs->SaGvLast) || !MrcIsSaGvEnabled (MrcData)) {
    MrcPmCfgCrAccess (MrcData, FALSE, FALSE);

    // Enable Periodic Compensation
    if (ExtInputs->EnPeriodicComp == 1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Enable Periodic Compensation\n");
      PhyPmOvrd.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD_REG);
      // MaxMin( ROUNDUP( LOG(1280uS/SBClock,2)-12, 0), 0, 13)
      SBClock = 1000000 / 400; // 400 MHz SBClock
      PhyPmOvrd.Bits.EnPeriodicComp = RANGE (MrcLog2Round (MrcData, 1280 * 1000000/SBClock) - 12, 0 ,13);
      MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD_REG, PhyPmOvrd.Data);
    }
  }

  if (ExtInputs->RealtimeMemoryFrequency && (Outputs->SaGvPoint != MrcSaGvPoint0)) {
    /*
     * MRC-Comment:RestrictedBegin
     * HSD 15014944558
     * Todo: need talk to PCU to force to run at MrcSaGvPoint0
     * Perhaps need add mailbox codes
     * MRC-Comment:RestrictedEnd
     */
  }

  return Status;
}

/**
  This function provides sum of low frequency SAGV points

  @param[out] MrcData     - The Mrc Host data structure

  @retval LowFrequencySagvSum Sum of low frequency SAGV points
**/
UINT32
MrcGetLowFrequencySagvPointsSum (
  IN MrcParameters  *MrcData
  )
{
  MrcInput *Inputs;
  UINT16   SaGvPoint;
  UINT32   LowFrequencySagvSum;

  Inputs  = &MrcData->Inputs;

  LowFrequencySagvSum = 0;

  for (SaGvPoint = 0; SaGvPoint < MAX_SAGV_POINTS; SaGvPoint++) {
    if (Inputs->ExtInputs.Ptr->SaGvFreq[SaGvPoint] <= 3200) {
      LowFrequencySagvSum++;
    }
  }

  return LowFrequencySagvSum;
}

/**
  This function fills in the MRS FSM to finalize the SAGV configuration for normal operation.

  @param[in] MrcData - The global host structure

  @retval mrcSuccess.
**/
MrcStatus
MrcSaGvFinal (
  IN     MrcParameters *const MrcData
  )
{
  MrcStatus Status;
  MrcOutput *Outputs;
  INT64     GetSetVal;
  BOOLEAN   IsOptimizeTxp = FALSE;
  BOOLEAN   IsRirEnabled = FALSE;

  Status  = mrcSuccess;
  Outputs = &MrcData->Outputs;

  if (Outputs->IsDdr5) {
    // Use multicycle MPC command only during boot.
    // After boot, configure DRAM and MC for 1tCK MPC
    DisableMcMulticycleCs (MrcData, FALSE);
  }

  // Update MR11[OP7] - CsOdtOp, if supported by DRAM
  if ((Outputs->IsLpddr5) && (Outputs->LpX)) {
    Status = MrcMr11WriteCsOdtOp (MrcData);
    if (Status != mrcSuccess) {
      return Status;
    }
  }

  Status = MrcIfOptimizeTxp(MrcData, &IsOptimizeTxp);
  if (Status != mrcSuccess) {
    return Status;
  }

  if (IsOptimizeTxp) {
    // This is supported only for LP5, in MC tXP is in nWCK uints so we need convert nCK to nWCK
    GetSetVal = MrcGetTxp (Outputs->DdrType, Outputs->MemoryClock) * WCK_TO_CK_RATIO;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctXP, WriteCached, &GetSetVal);
  }
  // Update DDR5 MR4[OP3] Refresh Interval Rate Indicator if supported by DRAM
  if (Outputs->IsDdr5) {
    Status = MrcTurnOnRirIfSupported (MrcData, &IsRirEnabled);
    if (Status != mrcSuccess) {
      return Status;
    }
    GetSetVal = IsRirEnabled;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMcDdr5Rir, WriteCached, &GetSetVal);
  }

  Status |= MrcFinalizeMrSeq (MrcData, MRC_PRINTS_ON);

  if (Outputs->IsLpddr) {
    {
      // MRC Controls the FSP-OP switch on bmCold and bmFast boot flows as part of JEDEC Reset \ JEDEC Init.
      // JEDEC spec requires MR16 VRCG="VREF Fast Response (high current)" mode any time FSP-OP is switched.
      // VRCG must be set back to "Normal Operation" at the end of MRC
      // Configuring MrhDelay for 250ns as it's the maximum for LP5.
      GetSetVal = DIVIDECEIL (MRC_LP_tFC_LONG_NS * 1000, Outputs->Wckps);
      MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctMrhAfterCommandDelay, WriteCached, &GetSetVal);
      MrcSetFspVrcg (MrcData, ALL_RANK_MASK, LpVrcgNormal, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8);

      GetSetVal = 0;
      MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctMrhAfterCommandDelay, WriteCached, &GetSetVal);
    }
  }

  // Save the output SaGv config data required by BIOS
  MrcSaveSagvOutputs (MrcData);

  return Status;
}
