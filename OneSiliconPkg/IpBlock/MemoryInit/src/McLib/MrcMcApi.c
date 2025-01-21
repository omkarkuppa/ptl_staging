/** @file
  This file contains code related to initializing and configuring the MC.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include "MrcAddressDecodeConfiguration.h"
#include "CMrcTypes.h"
#include "MrcHalRegisterAccess.h"
#include "MrcMcApi.h"
#include "MrcRefreshConfiguration.h"
#include "MrcSchedulerParameters.h"
#include "CMrcStartMemoryConfiguration.h"
#include "MrcTimingConfiguration.h"
#include "MrcMaintenance.h"
#include "MrcChipApi.h"
#include "MrcMcConfiguration.h"
#include "MrcPowerModes.h"
#include "MrcModeRegisterHandler.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcTurnAround.h"
#include "MrcDdrIoUtils.h"

/**
  This function initializes the Memory Controller: Scheduler, Timings, Address Decode,
  Odt Control, and refresh settings.

  @param[in, out] MrcData - MRC global data.

  @retval MrcStatus - mrcSuccess if successful or an error status.
**/
MrcStatus
MrcMcConfiguration (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  MrcDebug  *Debug;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  // TAT Programming in Timing Configuration needs OdtMatrix Set
  MrcSetOdtMatrix (MrcData, 1);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Timing Config\n");
  MrcTimingConfiguration (MrcData);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Refresh Config\n");
  MrcRefreshConfiguration (MrcData);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Scheduler parameters\n");
  MrcSchedulerParametersConfig (MrcData);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Address Decoding Config\n");
  MrcAdConfiguration (MrcData);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Maintenance Configuration\n");
  MrcMaintenanceConfig (MrcData);


  return mrcSuccess;
}

/**
  This function sets up MC set of registers for CS 1N or 2N modes

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     IsEnable - Whether it is 2N (MRC_ENABLE) or 1N (MRC_DISABLE).
**/
VOID
McCs2NEnableOrDisable (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN  IsEnable
  )
{
  INT64  GetSetVal;
  GetSetVal = (IsEnable) ? 1 : 0;

  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEnCsGearDown, ForceWriteCached | PrintValue, &GetSetVal);
}

/**
  This function sets up MC set of registers for CA 1N or 2N modes

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     IsEnable - Whether it is 2N (MRC_ENABLE) or 1N (MRC_DISABLE).
**/
VOID
McCa2NEnableOrDisable (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN  IsEnable
  )
{
  INT64  GetSetVal;
  GetSetVal = (IsEnable) ? 1 : 0;

  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCmdStretch, WriteCached | PrintValue, &GetSetVal);
}

/**
  This function forces Self Refresh Entry (with Frequency change) due to CS 2N Gear Down

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess self refresh entry was successful
  @retval mrcFail Timeout exceeded when trying to go into self refresh
**/
MrcStatus
MrcCs2NSrefEntry (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcInput     *Inputs;
  MRC_FUNCTION *MrcCall;
  INT64        GetSetVal;
  INT64        SelfRefreshState[MAX_CONTROLLER];
  UINT64       Timeout;
  UINT32       Controller;

  Inputs    = &MrcData->Inputs;
  MrcCall   = Inputs->Call.Func;

  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccForceSreWithFreqChange, WriteCached | PrintValue, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccForceSRState, WriteCached | PrintValue, &GetSetVal);

  //Initialize
  SelfRefreshState[cCONTROLLER0] = SelfRefreshState[cCONTROLLER1] = 1;

  Timeout = MrcCall->MrcGetCpuTime () + 1000;  // 1 sec timeout
  do {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      if (!MrcControllerExist (MrcData, Controller)) {
        continue;
      }
      MrcGetSetMc (MrcData, Controller, GsmMccSrState, ReadCached, &SelfRefreshState[Controller]);
    }
  } while (((SelfRefreshState[cCONTROLLER0] == 0) || (SelfRefreshState[cCONTROLLER1] == 0)) && (MrcCall->MrcGetCpuTime () < Timeout));
  if ((SelfRefreshState[cCONTROLLER0] == 0) || (SelfRefreshState[cCONTROLLER1] == 0)) {
    //Timeout exceeded and still not in Self Refresh
    return mrcFail;
  }

  GetSetVal = 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccForceSreWithFreqChange, WriteCached | PrintValue, &GetSetVal);

  return mrcSuccess;
}

/**
  This function forces Self Refresh Exit

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess self refresh exit was successful.
**/
MrcStatus
MrcCs2NForceSrExit (
  IN MrcParameters *const MrcData
  )
{
  INT64        GetSetVal;
  
  GetSetVal = 1;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccPureSrx, WriteCached, &GetSetVal);
  GetSetVal = 0;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccForceSRState, ForceWriteCached, &GetSetVal);

  MrcWait (MrcData, MRC_TIMER_1US);

  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccPureSrx, WriteCached, &GetSetVal);

  return mrcSuccess;
}

/**
  This function saves and restores Write Retrain and MR4 Periodic Reads as well as Self Refresh Exit runs with them for CS 2N Gear Down Mode.
  During saving portion, both Write Retrain and MR4 Periodic Reads will be disabled.

  @param[in, out] MrcData           - Include all MRC global data.
  @param[in, out] SaveData          - Pointer to storage struct to keep original values
  @param[in]      SaveOrRestore     - Selects between saving the values or restoring the values.
**/
void
SetMcForCs2nTransition (
  IN OUT MrcParameters *const MrcData,
  IN OUT MRC_DDR5_CS_GEARDOWN_SAVE *SaveData,
  IN     MrcSaveOrRestore     SaveOrRestore
  )
{
  INT64   GetSetVal;
  UINT32  FirstController;
  UINT32  FirstChannel;
  BOOLEAN IsRestore = (SaveOrRestore == MrcRestoreEnum);

  if (IsRestore) {
    GetSetVal = SaveData->TxRetrain;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEnWrRetraining, WriteToCache, &GetSetVal);
    GetSetVal = SaveData->Mr4;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccMR4Period, WriteToCache, &GetSetVal);
    GetSetVal = SaveData->SrxTxRetrain;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEnSrxWrRetraining, WriteToCache, &GetSetVal);
    GetSetVal = SaveData->SrxRead;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEnSrxTempRead, WriteToCache, &GetSetVal);
    GetSetVal = SaveData->MainGlbDrvGateDis;
    MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccMainGlbDrvGateDis, WriteToCache, &GetSetVal);
  } else {
    FirstController = MrcData->Outputs.FirstPopController;
    FirstChannel = MrcData->Outputs.Controller[FirstController].FirstPopCh;

    MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmMccEnWrRetraining, ReadFromCache, &GetSetVal);
    SaveData->TxRetrain = (INT32) GetSetVal;
    MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmMccMR4Period, ReadFromCache, &GetSetVal);
    SaveData->Mr4 = (INT32) GetSetVal;
    MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmMccEnSrxWrRetraining, ReadFromCache, &GetSetVal);
    SaveData->SrxTxRetrain = (INT32) GetSetVal;
    MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmMccEnSrxTempRead, ReadFromCache, &GetSetVal);
    SaveData->SrxRead = (INT32) GetSetVal;
    MrcGetSetMc (MrcData, FirstController, GsmMccMainGlbDrvGateDis, ReadFromCache, &GetSetVal);
    SaveData->MainGlbDrvGateDis = (INT32) GetSetVal;    

    GetSetVal = 0;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEnWrRetraining, WriteToCache, &GetSetVal);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccMR4Period, WriteToCache, &GetSetVal);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEnSrxWrRetraining, WriteToCache, &GetSetVal);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEnSrxTempRead, WriteToCache, &GetSetVal);

    GetSetVal = 1;
    MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccMainGlbDrvGateDis, WriteToCache, &GetSetVal);
  }

  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function returns type of DimmOdt for DDR5 based on OdtMatrix and Read/Write mode

  Returns OptDimmOdtPark if OdtMatrix is 0
          OptDimmOdtNomRd if OdtMatrix is 1 and OdtType is ReadOdt
          OptDimmOdtNomWr if OdtMatrix is 1 and OdtType is WriteOdt

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      Controller    - Controller to setup
  @param[in]      Channel       - Channel to setup
  @param[in]      Rank          - Rank to setup
  @param[in]      OdtType       - ReadOdt or WriteOdt
  @param[in]      NtRank        - Non Target Rank

  @retval OdtParam
**/
UINT8
MrcGetOdtParam (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT8                OdtType,
  IN UINT8                NtRank
  )
{
  MrcOutput        *Outputs;
  INT64            OdtMatrix;
  UINT8            NtOdt;

  Outputs = &MrcData->Outputs;

  MRC_DEBUG_ASSERT (Outputs->IsDdr5, &Outputs->Debug, "Unsupported DRAM Type: %d\n", Outputs->DdrType);

  MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, (OdtType == ReadOdt) ? GsmMccOdtMatrixRd : GsmMccOdtMatrixWr, ReadFromCache, &OdtMatrix);
  NtOdt = ((UINT8) OdtMatrix >> NtRank) & (MRC_BIT0);

  return ((NtOdt == 0) ? OptDimmOdtPark : ((OdtType == ReadOdt) ? OptDimmOdtNomRd : OptDimmOdtNomWr));
}

/**
  This function calculates the DDR5 Turnaround timings based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] TxPathMinMax      - Holds the max/min Tx Path margins for each Channel
  @param[in] IsPreFuncTraining - TRUE if board flight delay impact from Read Leveling / Write Leveling is not known yet
  @param[OUT] TAT              - Returns the turnaround timings based on DRAM Specification
**/
VOID
GetDdr5DramTA (
  IN MrcParameters *const  MrcData,
  IN  TxPathMinMax  *const TxPathResults,
  IN  RxPathMinMax  *const RxPathResults,
  IN  BOOLEAN              IsPreFuncTraining,
  OUT McTurnAroundTimings  *const TAT
  )
{
  MrcOutput      *Outputs;
  UINT32 Controller;
  UINT32 Channel;
  UINT32 Rank;
  UINT32 Byte;
  UINT32 DeltaRcvEnPiCode;
  UINT32 DeltaTxDqsPiCode;
  UINT32 WLTxDqs;
  UINT32 MaxWL;
  UINT32 MinWL;
  INT64  TxDqs;
  UINT8  MaxChannel;
  INT8   MinOdtWrOn;
  INT8   MinOdtRdOn;
  INT8   MaxOdtWrOff;
  INT8   MaxOdtRdOff;
  INT8   OdtWrOn;
  INT8   OdtWrOff;
  INT8   OdtRdOn;
  INT8   OdtRdOff;
  BOOLEAN NTODTWr;
  BOOLEAN NTODTRd;

  Outputs      = &MrcData->Outputs;
  MaxChannel   = Outputs->MaxChannels;
  DeltaTxDqsPiCode = 0;
  DeltaRcvEnPiCode = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      IsNtOdtSupported (MrcData, Controller, Channel, &NTODTWr, &NTODTRd);
      if (IsPreFuncTraining) {
        // Assume the worst case ODT Offsets
        MinOdtWrOn  = MRC_DDR5_WORST_CASE_ODT_WR_ON_OFFSET;
        MinOdtRdOn  = MRC_DDR5_WORST_CASE_ODT_RD_ON_OFFSET;
        MaxOdtWrOff = MRC_DDR5_WORST_CASE_ODT_WR_OFF_OFFSET;
        MaxOdtRdOff = MRC_DDR5_WORST_CASE_ODT_RD_OFF_OFFSET;

        DeltaTxDqsPiCode = DEFAULT_DELTA_TXDQS_PICODE; // Safe Value for Mc Init
        DeltaRcvEnPiCode = DEFAULT_DELTA_RCVEN_PICODE; // Safe Value for Mc Init
      } else {
        // Set Inital values, then search for worst case ODT offsets among ranks
        MinOdtWrOn  = 0xF;
        MinOdtRdOn  = 0xF;
        MaxOdtWrOff = -0xF;
        MaxOdtRdOff = -0xF;

        // Find Worst case ODT offsets among Ranks
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
            continue;
          }
          GetDdr5OdtOffsets (MrcData, Controller, Channel, Rank, NTODTWr, IsPreFuncTraining, &OdtWrOn, &OdtWrOff, &OdtRdOn, &OdtRdOff);
          MinOdtWrOn = MIN (OdtWrOn, MinOdtWrOn);
          MinOdtRdOn = MIN (OdtRdOn, MinOdtRdOn);
          MaxOdtWrOff = MAX (OdtWrOff, MaxOdtWrOff);
          MaxOdtRdOff = MAX (OdtRdOff, MaxOdtRdOff);
        }
        WLTxDqs = 0;
        for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
          MaxWL = 0;
          MinWL = MRC_UINT32_MAX;
          for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
            if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
              continue;
            }
            MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, TxDqsDelay, ReadCached, &TxDqs);
            MaxWL = MAX ((UINT32) TxDqs, MaxWL);
            MinWL = MIN ((UINT32) TxDqs, MinWL);
          }
          // CK2CK = Delta of Write Leveling TxDqs
          WLTxDqs = MAX (WLTxDqs, MaxWL - MinWL);
        }
        DeltaTxDqsPiCode = WLTxDqs;
        // DeltaPiCode = Delta of RcvEn
        DeltaRcvEnPiCode = (UINT32) (RxPathResults->MaxRcvEnPi[Controller][Channel] - RxPathResults->MinRcvEnPi[Controller][Channel]);
      }

      GetDdr5tRdRd (MrcData, DeltaRcvEnPiCode, &TAT->tRdRdsg[Controller][Channel], &TAT->tRdRddg[Controller][Channel], &TAT->tRdRddr[Controller][Channel], &TAT->tRdRddd[Controller][Channel]);
      GetDdr5tWrWr (MrcData, MaxOdtWrOff, MinOdtWrOn, &TAT->tWrWrsg[Controller][Channel], &TAT->tWrWrdg[Controller][Channel], &TAT->tWrWrdr[Controller][Channel], &TAT->tWrWrdd[Controller][Channel]);
      GetDdr5tRdWr (MrcData, DeltaTxDqsPiCode, MaxOdtRdOff, MinOdtWrOn, NTODTRd, &TAT->tRdWrsg[Controller][Channel], &TAT->tRdWrdg[Controller][Channel], &TAT->tRdWrdr[Controller][Channel], &TAT->tRdWrdd[Controller][Channel]);
      GetDdr5tWrRd (MrcData, DeltaTxDqsPiCode, MinOdtRdOn, MaxOdtWrOff, NTODTRd, &TAT->tWrRdsg[Controller][Channel], &TAT->tWrRddg[Controller][Channel], &TAT->tWrRddr[Controller][Channel], &TAT->tWrRddd[Controller][Channel]);
    }
  }
}

/**
  This function calculates the LPDDR5 Turnaround timings based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] RxPathResults     - Holds the max/min Rx Path margins for each Channel
  @param[in] IsPreFuncTraining - TRUE if board flight delay impact from Read Leveling / Write Leveling is not known yet
  @param[out] TAT              - returns the turnaround timings based on DRAM Specification
**/
VOID
GetLpddr5DramTA (
  IN MrcParameters *const  MrcData,
  IN  RxPathMinMax *const  RxPathResults,
  IN  BOOLEAN              IsPreFuncTraining,
  OUT McTurnAroundTimings  *const TAT
  )
{
  MrcOutput *Outputs;
  UINT32    DeltaRcvEnPiCode;
  UINT8     Controller;
  UINT8     Channel;
  UINT8     MaxChannel;

  Outputs     = &MrcData->Outputs;
  MaxChannel  = Outputs->MaxChannels;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (Outputs->IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      if (IsPreFuncTraining) {
        DeltaRcvEnPiCode = 16;
      } else {
        DeltaRcvEnPiCode = (UINT32)(RxPathResults->MaxRcvEnPi[Controller][Channel] - RxPathResults->MinRcvEnPi[Controller][Channel]);
      }

      GetLpddr5tRdRd (MrcData, DeltaRcvEnPiCode, &TAT->tRdRdsg[Controller][Channel], &TAT->tRdRddg[Controller][Channel], &TAT->tRdRddr[Controller][Channel], &TAT->tRdRddd[Controller][Channel]);
      GetLpddr5tWrWr (MrcData, &TAT->tWrWrsg[Controller][Channel], &TAT->tWrWrdg[Controller][Channel], &TAT->tWrWrdr[Controller][Channel], &TAT->tWrWrdd[Controller][Channel]);
      GetLpddr5tRdWr (MrcData, &TAT->tRdWrsg[Controller][Channel], &TAT->tRdWrdg[Controller][Channel], &TAT->tRdWrdr[Controller][Channel], &TAT->tRdWrdd[Controller][Channel]);
      GetLpddr5tWrRd (MrcData, &TAT->tWrRdsg[Controller][Channel], &TAT->tWrRddg[Controller][Channel], &TAT->tWrRddr[Controller][Channel], &TAT->tWrRddd[Controller][Channel]);
    }
  }
}

/**
  This function calculates the Turnaround timings based on training results and DRAM Spec

  @param[in] MrcData - Include all MRC global data.
  @param[in] IsPreFuncTraining - TRUE if board flight delay impact from Read Leveling / Write Leveling is not known yet

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
MrcStatus
SetTurnAroundTiming (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsPreFuncTraining
  )
{
  MrcOutput *Outputs;
  const MrcInput *Inputs;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcDebug *Debug;
  const MRC_FUNCTION *MrcCall;
  INT64 GetSetVal;
  UINT32 Controller;
  UINT32 Channel;
  UINT32 PHYRd2Rd_PI[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 PHYWr2Wr_PI[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 RoundTo;
  UINT32 *TatPtr;
  UINT8  MaxChannel;
  UINT8  TatIndex;
  BOOLEAN IsLpddr5;
  TxPathMinMax TxPathResults;
  RxPathMinMax RxPathResults;
  McTurnAroundTimings TAT;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Debug = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  IsLpddr5 = Outputs->IsLpddr5;
  MaxChannel = Outputs->MaxChannels;

  // The order of TAT knobs needs to align to the TAT order within the structure McTurnAroundTimings
  static const GSM_GT TATKnobs[TAT_COUNT] = { GsmMctRDRDsg, GsmMctRDRDdg, GsmMctWRWRsg, GsmMctWRWRdg,
                                              GsmMctRDWRsg, GsmMctRDWRdg, GsmMctWRRDsg, GsmMctWRRDdg,
                                              GsmMctRDRDdr, GsmMctRDRDdd, GsmMctWRWRdr, GsmMctWRWRdd,
                                              GsmMctRDWRdr, GsmMctRDWRdd, GsmMctWRRDdr, GsmMctWRRDdd };

  UINT8 TATDeltaInputs[TAT_COUNT];

  // The order of TAT Delta Inputs needs to align to the TAT order within the structure McTurnAroundTimings
  TATDeltaInputs[0] = ExtInputs->Trrsg;
  TATDeltaInputs[1] = ExtInputs->Trrdg;
  TATDeltaInputs[2] = ExtInputs->Twwsg;
  TATDeltaInputs[3] = ExtInputs->Twwdg;
  TATDeltaInputs[4] = ExtInputs->Trwsg;
  TATDeltaInputs[5] = ExtInputs->Trwdg;
  TATDeltaInputs[6] = ExtInputs->Twrsg;
  TATDeltaInputs[7] = ExtInputs->Twrdg;
  TATDeltaInputs[8] = ExtInputs->Trrdr;
  TATDeltaInputs[9] = ExtInputs->Trrdd;
  TATDeltaInputs[10] = ExtInputs->Twwdr;
  TATDeltaInputs[11] = ExtInputs->Twwdd;
  TATDeltaInputs[12] = ExtInputs->Trwdr;
  TATDeltaInputs[13] = ExtInputs->Trwdd;
  TATDeltaInputs[14] = ExtInputs->Twrdr;
  TATDeltaInputs[15] = ExtInputs->Twrdd;

  MrcCall->MrcSetMem ((UINT8 *) &TxPathResults, sizeof (TxPathResults), 0);
  MrcCall->MrcSetMem ((UINT8 *) &RxPathResults, sizeof (RxPathResults), 0);
  MrcCall->MrcSetMem ((UINT8 *) &TAT, sizeof (TAT), 0);

  if (!IsPreFuncTraining) {
    // Find Max/Min Values for Rx/Tx Path parameters
    GetTxPathMinMax (MrcData, &TxPathResults);
    GetRxPathMinMax (MrcData, &RxPathResults);

    // Calculate TAT values constrained by PHY and training values
    GetPhyRd2RdTA (MrcData, &RxPathResults, PHYRd2Rd_PI);
    GetPhyWr2WrTA (MrcData, &TxPathResults, PHYWr2Wr_PI);
  }

  // Find DRAM Specified TAT values
  if (IsLpddr5) {
    GetLpddr5DramTA (MrcData, &RxPathResults, IsPreFuncTraining, &TAT);
  } else {
    GetDdr5DramTA (MrcData, &TxPathResults, &RxPathResults, IsPreFuncTraining, &TAT);
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (Outputs->IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      if (!IsPreFuncTraining) {
        MRC_DEBUG_MSG (
          Debug,
          MSG_LEVEL_NOTE,
          "Mc%d.Ch%d: PhyRdRd:%d tRdRd:%d, PhyWrWr:%d tWrWr:%d \n",
          Controller,
          Channel,
          PHYRd2Rd_PI[Controller][Channel],
          TAT.tRdRddr[Controller][Channel],
          PHYWr2Wr_PI[Controller][Channel],
          TAT.tWrWrdr[Controller][Channel]
        );
        TAT.tRdRddr[Controller][Channel] = MAX (PHYRd2Rd_PI[Controller][Channel], TAT.tRdRddr[Controller][Channel]);
        TAT.tRdRddd[Controller][Channel] = TAT.tRdRddr[Controller][Channel];
        TAT.tWrWrdr[Controller][Channel] = MAX (PHYWr2Wr_PI[Controller][Channel], TAT.tWrWrdr[Controller][Channel]);
        TAT.tWrWrdd[Controller][Channel] = TAT.tWrWrdr[Controller][Channel];
      }

      for (TatIndex = 0; TatIndex < TAT_COUNT; TatIndex++) {
        // This loops depends on the ordering of elements within the stucture McTurnAroundTimings.
        // The SG/DG turnaround timings are the first elements, and the RD/DD are the last elements
        TatPtr = &TAT.tRdRdsg[Controller][Channel] + (TatIndex * (sizeof (TAT.tRdRdsg) / sizeof (UINT32)));

        // Add safe values when calculating TAT values before MRC training
        if (IsPreFuncTraining) {
          // Add +8 for Different Rank / Different Dimm TAT values for McInit
          if (TatIndex >= TAT_SG_DG_MAX) {
            *TatPtr += TAT_SAFE_OFFSET_DR;
          }
        } else {
          if (IsLpddr5 && Outputs->Frequency == f2667 && (TatIndex == 0 || TatIndex == 1)) {
            *TatPtr += 3;
          }
          // After optimizing TAT values after training, add on additonal TAT delay based on Input Parameter
          if (Inputs->TatDelta != 0) {
            *TatPtr += Inputs->TatDelta;
          }
        }

        if (TATDeltaInputs[TatIndex] != 0) {
          *TatPtr += (TATDeltaInputs[TatIndex] - 128);
        }

        if (IsLpddr5) {
          // WCK is configured to be 4:1 CK.  Align timings to this.
          RoundTo = 4;
          *TatPtr = RoundTo * (DIVIDECEIL ((UINT32) *TatPtr, RoundTo));
        }

        GetSetVal = *TatPtr;
        MrcGetSetMcCh (MrcData, Controller, Channel, TATKnobs[TatIndex], WriteToCache | PrintValue, &GetSetVal);

      }
    }
  }
  MrcFlushRegisterCachedData (MrcData);

  return mrcSuccess;
}

/**
  This function implements Turn Around Timing Optimization.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
MrcStatus
MrcTurnAroundTimingOptimization (
  IN MrcParameters *const MrcData
  )
{
  return SetTurnAroundTiming (MrcData, FALSE);
}
