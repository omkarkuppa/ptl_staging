/** @file
  This module sets the memory controller refresh parameters.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
///
/// Include files
///
#include "MrcRefreshConfiguration.h"
#include "MrcRefreshConfigurationPrivate.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcMcApi.h"
#include "BlueMrcJedecApi.h"  // for MrcGetTrfm()
#include "MrcSpdProcessingDefs.h"

/**
  This function configures refresh parameters:
    tREFI, tRFC, tREFIx9, tRFCpb

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
**/
void
SetTcRfpRftpReg (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  const MrcOutput *Outputs;
  const MrcInput  *Inputs;
  const MrcTiming *TimingOut;
  MrcProfile      Profile;
  INT64           GetSetVal;
  UINT32          tRefi;
  UINT32          MathTemp;
  BOOLEAN         IsLpddr5;
  BOOLEAN         IsDdr5;
  const MRC_EXT_INPUTS_TYPE  *ExtInputs;

  Inputs    = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Profile   = ExtInputs->MemoryProfile;
  Outputs   = &MrcData->Outputs;
  TimingOut = &Outputs->Timing[Profile];

  IsLpddr5  = Outputs->IsLpddr5;
  IsDdr5    = Outputs->IsDdr5;

  tRefi = (TimingOut->tREFI);
  if (IsLpddr5) {
    tRefi *= 4;
  }

  GetSetVal = tRefi;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctREFI, WriteToCache | PrintValue, &GetSetVal);

  if (IsDdr5) {
    GetSetVal = TimingOut->tRFC2;
  } else {
    MathTemp = TimingOut->tRFC;
    if (IsLpddr5) {
      MathTemp *= 4;
    }
    GetSetVal = MathTemp;
  }
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRFC, WriteToCache | PrintValue, &GetSetVal);

  //
  // Set REFRESH_PANIC_WM and REFRESH_HP_WM to support watermark finer granularity.
  //
  GetSetVal = Outputs->RefreshHpWm;
  GetSetVal = MIN (REFRESH_HP_WM_HIGH, GetSetVal);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRefreshHpWm, WriteToCache | PrintValue, &GetSetVal);
  GetSetVal = Outputs->RefreshHpWm;
  GetSetVal = MIN (REFRESH_ABR_RELEASE, GetSetVal);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRefreshAbrRelease, WriteToCache | PrintValue, &GetSetVal);

  GetSetVal = Outputs->RefreshPanicWm;
  GetSetVal = MIN (REFRESH_PANIC_WM_HIGH, GetSetVal);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRefreshPanicWm, WriteToCache | PrintValue, &GetSetVal);

  GetSetVal = (ExtInputs->PerBankRefresh == 0) ? 1 : 0;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPbrDis, WriteToCache | PrintValue, &GetSetVal);

  MathTemp = TimingOut->tRFCpb;
  if (IsLpddr5) {
    MathTemp *= 4;
  }
  GetSetVal = MathTemp;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRFCpb, WriteToCache | PrintValue, &GetSetVal);

  MathTemp = MrcGetTrfm (MrcData, Controller, Channel);
  if (IsLpddr5) {
    MathTemp *= 4;
  }
  GetSetVal = MathTemp;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRFM, WriteToCache | PrintValue, &GetSetVal);

  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function configures the Self Refresh Timing Parameters for the MC:
  tXS-offset, tXSDLL, and tMOD.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
**/
void
SetTcSrftpReg (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  MrcInput      *Inputs;
  MrcOutput     *Outputs;
  MrcFrequency  Frequency;
  MrcTiming     *Timing;
  INT64         tXSDLL;
  INT64         tMRD;
  INT64         tXSR;
  UINT8         TckWckScale;

  Inputs     = &MrcData->Inputs;
  Outputs    = &MrcData->Outputs;
  Frequency  = Outputs->Frequency;
  Timing     = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  TckWckScale = ((Outputs->IsLpddr5) ? 4 : 1);

  tMRD = MrcGetTmod (MrcData, Timing->tCK) * TckWckScale; // if LP5 Scale to WCK;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctMRD,    WriteToCache | PrintValue, &tMRD);

  tXSR = MrcGetTxsr (MrcData, Timing) * TckWckScale; // if LP5 Scale to WCK
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctXSR,    WriteToCache | PrintValue, &tXSR);

  tXSDLL = MrcGetTxsdll (MrcData, Frequency, tXSR);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctXSDLL,  WriteToCache | PrintValue, &tXSDLL);

  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function configures the TC-ZQCAL register and its fields tZQCS and tZQCS_PERIOD.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to work on.

  @retval Nothing.
**/
static
void
SetTcZqCalReg (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  MrcInput  *Inputs;
  MrcOutput *Outputs;
  MrcTiming *Timing;
  INT64     tZQCS;
  INT64     ZQCSPeriod;
  INT64     tZQCAL;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Timing   = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];

  tZQCS       = MrcGetTzqcs (MrcData, Timing->tCK);
  ZQCSPeriod  = (Outputs->IsLpddr) ? ZQCS_PERIOD_LPDDR : ZQCS_PERIOD_DDR5;
  tZQCAL      = MrcGetTzqcal (MrcData, Timing->tCK);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctZQCAL, WriteToCache | PrintValue, &tZQCAL);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttZQLatch, WriteToCache | PrintValue, &tZQCS);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctZQCalPeriod, WriteToCache | PrintValue, &ZQCSPeriod);
  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function executes the refresh configuration process.

  @param[in] MrcData         - Include all MRC global data.

  @retval Nothing.
**/
void
MrcRefreshConfiguration (
  IN MrcParameters *const MrcData
  )
{
  INT64       GetSetEn;
//  INT64       SrIdleCount;
  INT64       tSR;
  UINT32      Controller;
  UINT32      Channel;
  BOOLEAN     IsLpddr;
  INT64       GetSetVal;

  IsLpddr = MrcData->Outputs.IsLpddr;
//  SrIdleCount = SELF_REFRESH_IDLE_COUNT;
  GetSetEn = 1;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      continue;
    }
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      SetTcRfpRftpReg (MrcData, Controller, Channel);
      SetTcSrftpReg (MrcData, Controller, Channel);
      SetTcZqCalReg (MrcData, Controller, Channel);

      tSR = MrcGetTsr (MrcData);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctSR, WriteToCache | PrintValue, &tSR);

      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnRefTypeDisplay,     WriteToCache | PrintValue, &GetSetEn);
    }
//    MrcGetSetMc (MrcData, Controller, GsmMctSrIdleTimer, WriteToCache | PrintValue, &SrIdleCount);

    GetSetVal = 0;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCkeOverride, WriteToCache, &GetSetVal);
  }
  MrcFlushRegisterCachedData (MrcData);
  // MrcEcsConfig (MrcData);
}

/**
  Perform the required steps to exit self refresh in Cold Boot after MC takes control over DFI, DRAM needs to come out of self-refresh
   1. Set MC_INIT_STATE_G.pure_srx to 1 to avoid any additional maintenance flows. MRC needs to clear pure_srx=0 after SRX is complete and before issuing any SR related actions.
   2. Set Force_SR_State to 0b11 to force MC to exit self refresh by sending SRX command and stay out of self refresh (should be enabled when pure_srx mode is set).

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcSelfRefreshExitPostDfi (
  IN MrcParameters *const MrcData
  )
{
  INT64 GetSetEn;
  INT64 GetSetDis;
  INT64 GetSetVal;

  GetSetEn  = 1;
  GetSetDis = 0;

  MrcCkeOnProgramming (MrcData);
  MrcCkeOverrideProgramming (MrcData);

  //Enable Pure SRX
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccPureSrx, WriteCached, &GetSetEn);

  GetSetVal = 0x3;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccForceSRState, WriteCached, &GetSetVal);

  MrcWait (MrcData, 100*MRC_TIMER_1NS);

  //Disable Pure SRX
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccPureSrx, WriteCached, &GetSetDis);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccForceSRState, WriteCached, &GetSetDis);

  // set SRX_RESET = 0
  MrcSelfRefreshExit (MrcData, FALSE);

  return mrcSuccess;
}

/**
  Perform the required steps to exit self refresh in S3/Warm reset:
   1. Set MC_INIT_STATE_G.pure_srx to 1.
   2. Set MCHBAR_CHx_CR_MC_INIT_STATE.SRX_reset to 1.
    - MRC does not need to pull DIMMs out of self-refresh by
      asserting CKE high, and issue ZQ Long.
    - MC will do these steps automatically when MRC will switch
      it to Normal mode.

  @param[in] MrcData         - Include all MRC global data.
  @param[in] IsSrxResetSet   - Decides whether we set or clear srx_reset

  @retval mrcSuccess
**/
MrcStatus
MrcSelfRefreshExit (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsSrxResetSet
  )
{
  INT64           GetSetVal;
  INT64           GetSetDisable;

  GetSetVal = IsSrxResetSet ? 1 : 0;

  if ((IsSrxResetSet) && (MrcGetBootMode (MrcData) == bmWarm)) {
    GetSetDisable = 0;
    MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccCpgcActive, ForceWriteCached | PrintValue, &GetSetDisable);
  }

  // Indicate to MC that we are going to exit Self Refresh when we go to Normal Mode
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccMcSrx, ForceWriteCached | PrintValue, &GetSetVal);

  return mrcSuccess;
}

#if 0
/**
  This function provides DDR5 Error Check and Scrub (ECS) implementation.

  DDR5 ECS mode allows the DRAM to internally read, correct single bit errors, and write back corrected data bits to the array (scrub errors)
  while providing transparency to error counts.
  PTL MC only supports the automatic mode (Manual mode is not supported): The operation is done internal to DRAM.

  Enable "error scrubbing" DDR5 feature done by configuring the following registers:
    rfp_0_0_0_mchbar.ecs_refab_enable
    rfp_0_0_0_mchbar.ecs_refab_period

  @param[in] MrcData - Include all MRC global data.
**/
VOID
MrcEcsConfig (
  IN     MrcParameters* const MrcData
  )
{
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  const MrcOutput           *Outputs;
  const MrcControllerOut    *ControllerOut;
  const MrcChannelOut       *ChannelOut;
  const MrcDimmOut          *DimmOut;
  BOOLEAN                   IsDdr5;
  INT64                     GetSetVal;
  UINT32                    Controller;
  UINT32                    Channel;
  UINT32                    Dimm;
  UINT32                    TEcsInit;
  UINT32                    tRefi;

  ExtInputs = MrcData->Inputs.ExtInputs.Ptr;
  Outputs   = &MrcData->Outputs;
  IsDdr5    = Outputs->IsDdr5;

  if (IsDdr5) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          GetSetVal = 1;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmScPbrEcsRefabEnable, WriteCached | PrintValue, &GetSetVal);

          switch (DimmOut->DensityIndex) {
            case MrcDensity16Gb:
              TEcsInit = MRC_DDR5_tECSinit_16Gb_US;
              break;
            case MrcDensity24Gb:
              TEcsInit = MRC_DDR5_tECSinit_24Gb_US;
              break;
            case MrcDensity32Gb:
            default:  // Default value required by MC arch
              TEcsInit = MRC_DDR5_tECSinit_32Gb_US;
              break;
          }
          tRefi = Outputs->Timing[ExtInputs->MemoryProfile].tREFI;   // in tCK units
          GetSetVal = (TEcsInit * 1000 * 1000) / (tRefi * Outputs->tCKps) - 9;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmScPbrEcsRefabPeriod, WriteCached | PrintValue, &GetSetVal);
        } // Dimm
      } // Channel
    } // Controller
  }
}
#endif
