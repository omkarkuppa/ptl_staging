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
