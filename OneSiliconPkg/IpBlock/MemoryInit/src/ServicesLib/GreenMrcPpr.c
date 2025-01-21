/** @file
  This file contains functions to find and track DRAM Failing addresses for the
  Post Package Repair feature.

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

#include "MrcPpr.h"
#include "MrcCommon.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcReset.h"
#include "MrcGeneral.h"
#include "MrcDunitApi.h"
#include "GreenMrcHalApi.h"

/**
  Set up the controller for Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the current state of the controller.

  @retval None
**/
void
MrcDdr5PprControllerSetup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  )
{
  INT64 GetSetDis = 0;
  UINT32 Mptu;

  Mptu = GetMptuInstance (Controller, Channel);
  // Save the value of MNT_EN
  MrcGetSetMptu (MrcData, Mptu, GsmMptuMntEn, ReadCached, &SaveData->SaveVal0);

  // Disable maintenance operations
  MrcGetSetMptu (MrcData, Mptu, GsmMptuMntEn, WriteCached, &GetSetDis);
}


/**
  Cleanup the MC configuration after Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the controller state prior to PPR.

  @retval None
**/
void
MrcDdr5PprControllerTeardown (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  )
{
  UINT32 Mptu;

  Mptu = GetMptuInstance (Controller, Channel);
  // Restore the value of MNT_EN
  MrcGetSetMptu (MrcData, Mptu, GsmMptuMntEn, WriteCached, &SaveData->SaveVal0);
}

/**
  Set up the controller for Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the current state of the controller.

  @retval None
**/
void
MrcLpddr5PprControllerSetup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  )
{
  const MrcInput      *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  const MRC_FUNCTION  *MrcCall;
  INT64 WckOnStatus;
  UINT32 Mptu;
  UINT64              Timeout;
  INT64               GetSetDis;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &Outputs->Debug;
  GetSetDis = 0;

  Mptu = GetMptuInstance (Controller, Channel);

  // Save the value of MNT_EN
  MrcGetSetMptu (MrcData, Mptu, GsmMptuMntEn, ReadCached, &SaveData->SaveVal0);

  // Disable maintenance operations
  MrcGetSetMptu (MrcData, Mptu, GsmMptuMntEn, WriteCached, &GetSetDis);

  // Get the WCK status
  MrcGetSetMptu (MrcData, Mptu, GsmMptuStatusWckOn, ReadNoCache, &WckOnStatus);

  if (WckOnStatus == 1) {
    // Stop WCK on all Channels
    MrcIssueCas (MrcData, Controller, Channel, MAX_RANK_IN_CHANNEL, MrhCasWck2CkSyncOff, FALSE);

    // Verify that WCK has stopped
    Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT;   // 10 seconds timeout
    do {
      MrcGetSetMptu (MrcData, Mptu, GsmMptuStatusWckOn, ReadNoCache, &WckOnStatus);
    } while (WckOnStatus == 1 && (MrcCall->MrcGetCpuTime () < Timeout));
    // WCK is still on, print an error message
    MRC_DEBUG_ASSERT (WckOnStatus == 0, Debug, "%s %s %s\n", gErrString, __FUNCTION__, gTimeout);
  }
}

/**
  Cleanup the MC configuration after Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the controller state prior to PPR.

  @retval None
**/
void
MrcLpddr5PprControllerTeardown (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  )
{
  UINT32 Mptu;

  Mptu = GetMptuInstance (Controller, Channel);
  // Restore the value of MNT_EN
  MrcGetSetMptu (MrcData, Mptu, GsmMptuMntEn, WriteCached, &SaveData->SaveVal0);
}
