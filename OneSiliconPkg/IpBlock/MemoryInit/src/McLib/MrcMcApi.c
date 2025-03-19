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
  This function sets the Turnaround Timings based on the incoming Input Struct (TAT)

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      TAT           - Struct to hold TAT values
  @param[in]      Controller    - Controller to setup
  @param[in]      Channel       - Channel to setup
  @param[in]      IsPreTraining - BOOLEAN to indicate if TAT values should be programmed for Pre Trianing or Post Trianing TAT Optimization

  @retval None
**/
VOID
SetDramTatTimings (
  IN MrcParameters* const  MrcData,
  IN TurnAroundTimings     *TAT,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  BOOLEAN              IsPreTraining
  )
{
  MrcOutput *Outputs;
  const MrcInput *Inputs;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  INT64 GetSetVal;
  UINT32 RoundTo;
  UINT32 *TatPtr;
  UINT8  TatIndex;
  BOOLEAN IsLpddr5;

  // The order of TAT knobs needs to align to the TAT order within the structure McTurnAroundTimings
  static const GSM_GT TATKnobs[TAT_COUNT] = { GsmMctRDRDsg, GsmMctRDRDdg, GsmMctWRWRsg, GsmMctWRWRdg,
                                              GsmMctRDWRsg, GsmMctRDWRdg, GsmMctWRRDsg, GsmMctWRRDdg,
                                              GsmMctRDRDdr, GsmMctRDRDdd, GsmMctWRWRdr, GsmMctWRWRdd,
                                              GsmMctRDWRdr, GsmMctRDWRdd, GsmMctWRRDdr, GsmMctWRRDdd };

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  IsLpddr5 = Outputs->IsLpddr5;
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

  for (TatIndex = 0; TatIndex < TAT_COUNT; TatIndex++) {
    // This loops depends on the ordering of elements within the stucture McTurnAroundTimings.
    // The SG/DG turnaround timings are the first elements, and the RD/DD are the last elements
    TatPtr = &TAT->tRdRdsg + (TatIndex * (sizeof (TAT->tRdRdsg) / sizeof (UINT32)));

    // Add safe values when calculating TAT values before MRC training
    if (IsPreTraining) {
      // Add +8 for Different Rank / Different Dimm TAT values for McInit
      if (TatIndex >= TAT_SG_DG_MAX) {
        *TatPtr += TAT_SAFE_OFFSET_DR;
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

    if (!IsPreTraining && IS_MC_SUB_CH (Outputs->IsLpddr, Channel)) {
      // For LP5 Subchannels (Channels 1 and 3) compare TAT value with even channel and program the largest
      // In PreTraining case, all channels have the same TAT values, so this is not needed (and the caller is using MAX_CONTROLLER/MAX_CHANNEL)
      MrcGetSetMcCh (MrcData, Controller, Channel - 1, TATKnobs[TatIndex], ReadCached, &GetSetVal);
      if (GetSetVal < *TatPtr) {
        GetSetVal = *TatPtr;
        MrcGetSetMcCh (MrcData, Controller, Channel - 1, TATKnobs[TatIndex], WriteToCache | PrintValue, &GetSetVal);
      }
    } else {
      GetSetVal = *TatPtr;
      MrcGetSetMcCh (MrcData, Controller, Channel, TATKnobs[TatIndex], WriteToCache | PrintValue, &GetSetVal);
    }
  }
  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function saves / restores MC Address Decoder values.

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      SaveOrRestore - Selects between saving the values or restoring the values.
  @param[in, out] SavedValues   - Stores saved values.

  @retval Nothing.
**/
VOID
MrcMcAddressDecoderValuesSaveRestore (
  IN     MrcParameters *const MrcData,
  IN     MrcSaveOrRestore     SaveOrRestore,
  IN OUT MRC_MC_AD_SAVE       *SavedValues
  )
{
  const MRC_FUNCTION  *MrcCall;
  BOOLEAN IsLpddr;
  UINT8   MaxChannels;
  UINT8   Controller;
  UINT8   Channel;
  UINT32  GetSetMode;

  MrcCall = MrcData->Inputs.Call.Func;
  IsLpddr = MrcData->Outputs.IsLpddr;
  MaxChannels = MrcData->Outputs.MaxChannels;
  GetSetMode  = (SaveOrRestore == MrcSaveEnum) ? ReadCached : WriteCached;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      if (!(MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        continue;
      }
      // Backup / Restore the MAD values
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch0RankCnt,    GetSetMode, &SavedValues->Subch0RankCnt[Controller][Channel]);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch1RankCnt,    GetSetMode, &SavedValues->Subch1RankCnt[Controller][Channel]);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch0SdramWidth, GetSetMode, &SavedValues->Subch0SdramWidth[Controller][Channel]);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch1SdramWidth, GetSetMode, &SavedValues->Subch1SdramWidth[Controller][Channel]);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch0Density,    GetSetMode, &SavedValues->Subch0Density[Controller][Channel]);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch1Density,    GetSetMode, &SavedValues->Subch1Density[Controller][Channel]);
    }
  }
  if (SaveOrRestore == MrcSaveEnum) {
    SavedValues->Mc5_Control = MrcCall->MrcReadMsr64 (MRC_MSR_IA32_MC5_CTL);
    MrcCall->MrcWriteMsr64 (MRC_MSR_IA32_MC5_CTL, 0);
  } else {
    MrcCall->MrcWriteMsr64 (MRC_MSR_IA32_MC5_STATUS, 0);
    MrcCall->MrcWriteMsr64 (MRC_MSR_IA32_MC5_MISC, 0);
    MrcCall->MrcWriteMsr64 (MRC_MSR_IA32_MC5_CTL, SavedValues->Mc5_Control);
  }
}
