/** @file
  Implementation of LPDDR5 Specific functions, and constants.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
  JEDEC
**/
#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcHalRegisterAccess.h"
#include "MrcLpddr5Registers.h"
#include "MrcCommon.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"
#include "MrcLpddr5.h"
#include "MrcHalApi.h"
#include "MrcDdrIoDefines.h"
#include "MrcReset.h"

/**
  Check if Adaptive Refresh Management (ARFM) is supported in MR1

   @param[in]  MrcData        - Pointer to global MRC data
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - DIMM number

   @retval - FALSE if ARFM not supported (cannot check ARFM Level)
   @retval - TRUE means ARFM is supported (can check ARFM Level)
**/
BOOLEAN
MrcLpddr5IsArfmSupported (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  )
{
  MrcOutput                     *Outputs;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  LPDDR5_MODE_REGISTER_1_TYPE   Lpddr5MR1;
  BOOLEAN                       ArfmSupported;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  ArfmSupported   = FALSE;
  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    // Checking MR1 to see if ARFM supported across all ranks in dimm
    MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR1, MrrResult);
    Lpddr5MR1.Data8 = MrrResult[0];
    if (!Lpddr5MR1.Bits.ArfmSupport) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ARFM is not supported\n");
    } else {
      ArfmSupported = TRUE;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ARFM is supported\n");
    }
  }

  // RFM required if ARFM supported and ARFM level > 0 (and not disabled)
  return ArfmSupported;
}

/**
  Check if Refresh Management (RFM) is required by checking RFM Required bit in MR27

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number

   @retval - FALSE means RFM not required
   @retval - TRUE means RFM required
**/
BOOLEAN
MrcLpddr5IsRfmRequired (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  )
{
  MrcOutput                     *Outputs;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  BOOLEAN                       RfmRequired;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  LPDDR5_MODE_REGISTER_27_TYPE  Lpddr5MR27;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  RfmRequired = FALSE;
  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    // Checking MR27 reg to see if RFM required
    MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR27, MrrResult);
    Lpddr5MR27.Data8 = MrrResult[0];

    if (!Lpddr5MR27.Bits.RFM) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RFM is not required\n");
      continue;
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RFM required\n");
    RfmRequired = TRUE;
  }
  return RfmRequired;
}

/**
  Grab Refresh Management (RFM) threshold info from MR27 and MR57
  All of these threshold values are used in operations on Rolling Accumulated ACT (RAA) counter

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number
   @param[out] Raaimt          - RAA Initial Management Threshold (RAAIMT)
   @param[out] Raammt          - RAA Maximum Management Threshold (RAAMMT)
   @param[out] RefSub          - Decrement for RAA from RFMab or RFMpb commands (RAAIMT * RAADEC)
   @param[out] NormalRefSub    - Decrement for RAA from normal refresh commands (RAAIMT)

   @retval - mrcFail if bad RAAIMT value
   @retval - mrcSuccess if RFM threshold info gathered successfully
**/
MrcStatus
MrcLpddr5GetRfmThresholdInfo (
  IN MrcParameters *const MrcData,
  IN UINT32  Controller,
  IN UINT32  Channel,
  IN UINT32  Dimm,
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  )
{
  MrcStatus                     Status;
  MrcOutput                     *Outputs;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  UINT32                        RankRaaimt;
  UINT32                        RankRaammt;
  UINT32                        RankRefmSub;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  LPDDR5_MODE_REGISTER_27_TYPE  Lpddr5MR27;
  LPDDR5_MODE_REGISTER_57_TYPE  Lpddr5MR57;

  Status  = mrcSuccess;

  Outputs   = &MrcData->Outputs;
  Debug     = &Outputs->Debug;

  RfmSetupConfig->RaaimtOneDimm = MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_MAX;
  RfmSetupConfig->RaammtOneDimm = MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_MAX;
  RfmSetupConfig->RefSubOneDimm = MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_MAX;
  RfmSetupConfig->NormalRefSubOneDimm = MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_MAX;

  RankRaaimt    = RfmSetupConfig->RaaimtOneDimm;
  RankRaammt    = RfmSetupConfig->RaammtOneDimm;
  RankRefmSub   = RfmSetupConfig->RefSubOneDimm;
  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    // NOTE: ARFM Levels may possibly change these later, but we still need to
    // set RFM threshold values based on spec
    Status = MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR27, MrrResult);
    if (Status != mrcSuccess) {
      return Status;
    }

    Lpddr5MR27.Data8 = MrrResult[0];
    if (Lpddr5MR27.Bits.RAAIMT == 0) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Lpddr5 MR27.RAAIMT bad value: %u\n", Lpddr5MR27.Bits.RAAIMT);
      return mrcFail;
    }

    // Once RAA reaches this value, start refreshing -- RH_LOW_WM
    RankRaaimt = Lpddr5MR27.Bits.RAAIMT * 8;
    RfmSetupConfig->RaaimtOneDimm = MIN (RfmSetupConfig->RaaimtOneDimm, RankRaaimt);

    // Once RAA reaches this value, stop sending ACTs
    // RH_HIGH_WM will be lower than result of expression given to RankRaammt
    // RankRaa (RAAMMT) value will be later adjusted based on whether pTRR will be enabled:
    // RAAMMT-5 if pTRR is not enabled
    // RAAMMT-3 if pTRR is enabled
    RankRaammt = (Lpddr5MR27.Bits.RAAMULT + 1) * 2 * RankRaaimt;
    RfmSetupConfig->RaammtOneDimm = MIN (RfmSetupConfig->RaammtOneDimm, RankRaammt);

    // The amount to decrease RAA by during a normal refresh
    RfmSetupConfig->NormalRefSubOneDimm = MIN (RfmSetupConfig->NormalRefSubOneDimm, RankRaaimt);

    Status = MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR57, MrrResult);
    if (Status != mrcSuccess) {
      return Status;
    }

    // Gathering amount to decrease RAA by during RFMab or RFMpb command
    Lpddr5MR57.Data8 = MrrResult[0];
    switch (Lpddr5MR57.Bits.RAADEC) {
      case 0:
        RankRefmSub = RankRaaimt;
        break;
      case 1:
        RankRefmSub = UDIVIDEROUND (RankRaaimt * 15, 10);
        break;
      case 2:
        RankRefmSub = RankRaaimt * 2;
        break;
      case 3:
        RankRefmSub = RankRaaimt * 4;
        break;
      default:
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Lpddr5 MR57.RAADEC bad value: %u\n", Lpddr5MR57.Bits.RAADEC);
        return mrcFail;
    }
    RfmSetupConfig->RefSubOneDimm = MIN (RfmSetupConfig->RefSubOneDimm, RankRefmSub);
  }
  return Status;
}

/**
  Check if Directed Refresh Management (DRFM) is supported based on MR1

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number

   @retval - FALSE mean DRFM is not supported
   @retval - TRUE means DRFM is supported
**/
BOOLEAN
MrcLpddr5IsDrfmSupported (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  )
{
  MrcOutput                     *Outputs;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  LPDDR5_MODE_REGISTER_1_TYPE   Lpddr5MR1;
  BOOLEAN                       DrfmSupported;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  DrfmSupported = FALSE;
  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    // Checking MR1 reg to see if DRFM supported
    MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR1, MrrResult);
    Lpddr5MR1.Data8 = MrrResult[0];
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MR1: 0x%02X, MR1.DrfmSupprt: %u\n", Lpddr5MR1.Data8, Lpddr5MR1.Bits.DrfmSupport);

    if (!Lpddr5MR1.Bits.DrfmSupport) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DRFM is not supported\n");
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DRFM supported\n");
      DrfmSupported = TRUE;
    }
  }

  return DrfmSupported;
}

/**
  Configure Adaptive Refresh Management (ARFM) Levels in MR57 based on BIOS Inputs

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number

   @retval - mrcSuccess if ARFM configured correctly
**/
MrcStatus
MrcLpddr5ConfigArfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  )
{
  MrcStatus                     Status;
  MrcInput                      *Inputs;
  MRC_EXT_INPUTS_TYPE           *ExtInputs;
  MrcOutput                     *Outputs;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  LPDDR5_MODE_REGISTER_57_TYPE  Lpddr5MR57;

  Status  = mrcSuccess;
  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;

  if (ExtInputs->DramRfmMode == DramRfmDisabled) {
    return Status;
  }

  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    // Change ARFM level based on input
    Status = MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR57, MrrResult);
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "ARFM Level not configured - %s.\n", gErrString);
      return Status;
    }

    Lpddr5MR57.Data8 = MrrResult[0];
    Lpddr5MR57.Bits.ARFMLevel = ExtInputs->DramRfmMode;
    Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR57, Lpddr5MR57.Data8, TRUE);
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "ARFM Level not configured - %s.\n", gErrString);
      return Status;
    }
  }

  return Status;
}

/**
  Configure Directed Refresh Management (DRFM) Enable and Blast Radius Configuration (BRC) through MR75

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number
   @param[out] BrcSupported   - True if BRC supported by the DIMM

   @retval - mrcSuccess if DRFM successfully configured
**/
MrcStatus
MrcLpddr5ConfigDrfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  OUT BOOLEAN *BrcSupported
  )
{
  MrcStatus                     Status;
  MrcInput                      *Inputs;
  MRC_EXT_INPUTS_TYPE           *ExtInputs;
  MrcOutput                     *Outputs;
  MrcChannelOut                 *ChannelOut;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  LPDDR5_MODE_REGISTER_75_TYPE  Lpddr5MR75;
  UINT8                         *MrPtr;

  Status  = mrcSuccess;
  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  Debug   = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Outputs->DrfmBrc = ExtInputs->DrfmBrc;

  *BrcSupported = TRUE;
  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }
    // Getting/Setting BRC info and Enabling DRFM in MR75
    Status = MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR75, MrrResult);
    if (Status != mrcSuccess) {
      return Status;
    }

    Lpddr5MR75.Data8 = MrrResult[0];
    Lpddr5MR75.Bits.DrfmEnable = 1;

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC%u C%u Dimm%u R%u Blast Radius Configuration (BRC):\n", Controller, Channel, Dimm, Rank);
    if (!Lpddr5MR75.Bits.BrcSupported) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tBRC is not supported.\n");
      Outputs->DrfmBrc = DrfmBrcDisabled;
      *BrcSupported = FALSE;
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tBRC supported. BRC set to Blast Radius Level %d\n", Outputs->DrfmBrc);
      Lpddr5MR75.Bits.BlastRadiusConfig = ConvertDrfmBrcToTMrcJedecBrcType (Outputs->DrfmBrc);
    }

    Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR75, Lpddr5MR75.Data8, TRUE);
    if (Status != mrcSuccess) {
      return Status;
    }

    // Update MR75 host struct
    ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
    MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;
    MrPtr[mrIndexMR75] = Lpddr5MR75.Data8;

    // This routine is called on even Channels only
    // Send MR75 on the corresponding odd channels and update host structure for them
    if (MrcChannelExist (MrcData, Controller, Channel + 1)) {
      Status = MrcIssueMrw (MrcData, Controller, Channel + 1, Rank, mrMR75, Lpddr5MR75.Data8, TRUE);
      if (Status != mrcSuccess) {
        return Status;
      }

      ChannelOut = &Outputs->Controller[Controller].Channel[Channel + 1];
      MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;
      MrPtr[mrIndexMR75] = Lpddr5MR75.Data8;
    }
  }

  return Status;
}

/**
  This function will issue the JEDEC init MR sequence for LPDDR5.
  If RestoreMRs is set, the sequence will use the MR values saved in
  the MRC global data.  Otherwise, an initial value is used.
  Flow:
   1. Set Low frequency (1100)
   2. Send FSP-OP[0] MR2 to program RL of the high frequency
     - This is needed for DQ mapping step of ECT
   3. Set FSP-WR = 1, FSP-OP = 0
   4. Send all MRs
   5. If ECT_Done
     a. Set High frequency
     b. Set FSP-OP = 1

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcJedecInitLpddr5 (
  IN  MrcParameters *const  MrcData
  )
{
  const MRC_FUNCTION      *MrcCall;
  const MrcInput          *Inputs;
  MrcStatus               Status;
  MrcChannelOut           *ChannelOut;
  MrcOutput               *Outputs;
  MrcDebug                *Debug;
  MrcModeRegister         CurMrAddr;
  UINT32                  Channel;
  UINT32                  Controller;
  UINT8                   FspPoint;
  UINT32                  Rank;
  UINT32                  OutIdx;
  UINT16                  CurDelay;
  UINT8                   *DataPtr;
  UINT8                   MrIndex;
  UINT8                   Index;
  UINT8                   FspWrite;
  GmfLpddr5DelayType      CurDelayType;
  MRC_GEN_MRS_FSM_MR_TYPE *GenMrsFsmMr;
  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM];
  BOOLEAN                 IsRowHammerEnabled;

  static const UINT8  MrAddress[] = {
    mrMR16,
#ifndef HVM_MODE
    mrMR13,
#endif
    mrMR20, mrMR17, mrMR25, mrMR10, mrMR11, mrMR12, mrMR12b, mrMR14, mrMR15, mrMR18,
    mrMR3,  mrMR1,  mrMR2,  mrMR19, mrMR21, mrMR28, mrMR30,  mrMR69, mrMR37, mrMR41,
    mrMR75, mrMR24, mrMR58
  };

  // Due to the 6400 boundary crossing rule in LP5 JEDEC spec, MRs cannot be updated from the current FSP.
  // Hence JEDEC Init should program each FSP point from the opposite point.
  // E.g. FSP0 MR's are programmed from FSP1, and then FSP1 MR's are programmed from FSP0.
  //
  //  The sequence is:
  //  After JEDEC RESET MR16 contains default values: FSP-OP = 0, FSP-WR = 0, VRCG = 0
  //  1. Send MR16: FSP-OP = 1, FSP-WR = 0, VRCG = 1 (FSP switch), wait tFC
  //  2. Send MR's to FSP0 (from FSP1)
  //  3. Send MR16: FSP-OP = 0, FSP-WR = 1, VRCG = 1 (FSP switch), wait tFC
  //  4. Send MR's to FSP1 (from FSP0)
  //  5. Switch FSP-OP to 1 (keep FSP-WR = 1, VRCG = 1) - this is done after this routine, in MrcLpddrSwitchToHigh()

  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;
  Inputs   = &MrcData->Inputs;
  MrcCall  = Inputs->Call.Func;
  Status   = mrcSuccess;

  // In case this routine is used for Fast flow, need to enable VRCG first (as it may not be set in MR16 in the host struct)
  // In Cold flow, VRCG is set inside InitMrwLpddr5()
  if (Inputs->BootMode == bmFast) {
    MrcSetFspVrcg (MrcData, ALL_RANK_MASK, LpVrcgHighCurrent, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8);
  }

  // First FSP1, then FSP0
  for (FspPoint = 1; FspPoint != 0xFF; FspPoint--) {
    // Clear out our array
    MrcCall->MrcSetMem ((UINT8 *) MrData, sizeof (MrData), 0);

    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          if (MrcRankExist (MrcData, Controller, Channel, Rank) == 0) {
            continue;
          }
          // Build array based on MR value in host structure.
          for (Index = 0, OutIdx = 0; Index < ARRAY_COUNT (MrAddress); Index++) {
            CurMrAddr = MrAddress[Index];

            // Do not program MR75 if DRFM is not supported
            IsRowHammerEnabled = MrcData->Save.Data.IsRowHammerConfigured[Controller][Channel][dDIMM0];
            if ((CurMrAddr == mrMR75) && (!IsRowHammerEnabled)) {
              continue;
            }

            if ((CurMrAddr == mrMR12b) && (!Outputs->LpByteMode)) {
              continue;
            }

            MrIndex = MrcMrAddrToIndex (MrcData, &CurMrAddr);
            if (MrIndex < MAX_MR_IN_DIMM) {
              Status = Lpddr5GmfDelayType (
                MrcData,
                CurMrAddr,
                &CurDelayType,
                Index == (ARRAY_COUNT (MrAddress) - 1)
              );
              if (Status != mrcSuccess) {
                break;
              }

              DataPtr = &ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR[MrIndex];

              if (CurMrAddr == mrMR16) {
                FspWrite = (FspPoint == 0) ? LpFspWePoint1 : LpFspWePoint0;
                MrcLpddr5SetMr16 (MrcData, FspWrite, FspPoint, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, DataPtr);
                CurDelayType = (UINT16)GmfLpddr5Delay_tFC;
              }

              GenMrsFsmMr = &MrData[Controller][Channel][Rank][OutIdx];
              GenMrsFsmMr->PdaMr = FALSE;
              GenMrsFsmMr->MrData = *DataPtr;
              GenMrsFsmMr->MrAddr = CurMrAddr;
              GenMrsFsmMr->Valid  = TRUE;
              GenMrsFsmMr->DelayIndex = (GmfTimingIndex) CurDelayType;
              MrcGetGmfDelayTiming (MrcData, GenMrsFsmMr->DelayIndex, &CurDelay);
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, (!Outputs->JedecInitDone) ? "MC%d.C%d.R%d MR%d: 0x%X Delay: %u\n" : "", Controller, Channel, Rank, CurMrAddr, *DataPtr, CurDelay);
            } else {
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MR index(%d) exceeded MR array length(%d)\n", MrIndex, MAX_MR_IN_DIMM);
              Status = mrcWrongInputParameter;
              break;
            }
            OutIdx++;
          } // Index
          if (Status != mrcSuccess) {
            break;
          }
        } // Rank
        if (Status != mrcSuccess) {
          break;
        }
      } // Channel
      if (Status != mrcSuccess) {
        break;
      }
    } // Controller

    // Program Generic MRS FSM Per Controller/Channel
    Status = MrcGenMrsFsmConfig (MrcData, MrData, FALSE, NULL);
    if(Status != mrcSuccess) {
      return Status;
    }

    // Run Generic FSM
    // Since we use the MRS FSM, we've configured it to send ZQ at the end of the sequence.
    Status = MrcGenMrsFsmRun (MrcData);
    if (Status != mrcSuccess) {
      return Status;
    }
  } //FspPoint

  return Status;
}

/**
  If WCK sync is required (WCK Always On mode and after WCK leveling):
   - Issue CAS WS_OFF on all channels / ranks, followed by CAS WS_FS

  @param[in] MrcData    - Pointer to MRC global data.
**/
VOID
MrcLp5WckOffAndSync (
  IN MrcParameters *const MrcData
  )
{
  // Not used in Blue MRC
}
