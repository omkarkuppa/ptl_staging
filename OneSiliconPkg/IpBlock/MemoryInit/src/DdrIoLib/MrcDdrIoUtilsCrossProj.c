/** @file
  This file contains code related to initializing and configuring the DDRIO.

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

**/


#include "MrcDdrIoUtils.h" // for prototypes
#include "MrcDdrIoUtilsPrivate.h" // for WrappedForceRcomp()
#include "CMrcStatsTracker.h" // for MrcStatsStartTimer()
#include "CMrcApi.h" // for MRC_WAIT_TIMEOUT
#include "MrcCommon.h" // for MrcReadCR()
#include "MrcHalRegisterAccess.h" // for MRC_DDR5_CMD_GRP_MAX
#include "MrcMemoryApi.h"  // for MrcGetTwpre()
#include "MrcDdr5.h" // for GetDdr5tDQSSDram()
#include "MrcDdrIoApi.h" // for PI_PER_TCK


// Strings for TGlobalCompOffset decoding
const char *GlobalCompOffsetStr[TGlobalCompOffsetMax] = {
  "RdOdt",
  "WrDS",
  "WrDSCmd",
  "WrDSCtl",
  "WrDSClk"
};

const INT8 DdrioChDelta[MAX_SYS_CHANNEL] = {0, 0, -1, -1, -2, -2, -1, -1};
const INT8 DdrioChDeltaCccIL[MAX_SYS_CHANNEL] = {0, 0, 0, 0, -1, -1, -1, -1};


/**
  This function calculates Guardband for drift in Rd DQS timing over time

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT32 GB - Guardband in Pico Seconds
**/
UINT32
MrcGetGBDrift (
  IN     MrcParameters *const MrcData
  )
{
  MrcOutput    *Outputs;
  BOOLEAN      IsLpddr5;
  BOOLEAN      IsGear4;
  BOOLEAN      Is3200Above;
  BOOLEAN      Is6400Above;
  UINT32       GB;
  UINT32       PhClkPortion;
  UINT32       Lp5TempDrift;
  UINT32       Lp5VoltDrift;

  Outputs         = &MrcData->Outputs;
  IsLpddr5        = Outputs->IsLpddr5;
  IsGear4         = Outputs->GearMode ? 1 : 0;
  Is3200Above     = Outputs->Frequency >= f3200;
  Is6400Above     = Outputs->Frequency >= f6400;

  // Guardband for drift in Rd DQS timing over time
  // LPDDR5: 1.5pS/C*80C + 3pS/mV*Vdd2*0.05 + 8%*PHCLK + 40pS
  // DDR5: tDQSCK_Max (30% tCK) + 8% PHCLK + 40pS = 38% PHCLK + 40pS
  //       ex: 1 tCK == 1 PHClk.
  PhClkPortion = ((Outputs->Qclkps >> (IsGear4 ? 1 : 0)) * THOUSAND_MULTIPLIER);
  GB = 40 * THOUSAND_MULTIPLIER;
  if (IsLpddr5) {
    //LP5TempDrift = ((DataRate>=3200MTs)? ((DataRate>=6400MTs)?1.4:1.5):1.8)pS/C * (EnDramWck2DQO?5:80)C //Results in pS
    Lp5TempDrift = (Is3200Above ? (Is6400Above ? 1400 : 1500) : 1800) * 80;
    //LP5VoltDrift = ((DataRate>=3200 MTs)? ((DataRate>=6400MTs)?2.5:3):5)pS/mV * 1065mV * (EnDramWck2DQO?0.03:0.05) // Results in pS
    Lp5VoltDrift = (Is3200Above ? (Is6400Above ? 25 : 30) : 50) * Outputs->Vdd2Mv * 5;
    // Add % of PhClk portion of GB
    GB += Lp5TempDrift + Lp5VoltDrift + (PhClkPortion * 8 / 100); // fS
  } else { // DDR5
    GB += PhClkPortion * 38 / 100; // fS
  }

  // Calculation is done in fs, convert back to ps to avoid error
  GB = DIVIDECEIL (GB, THOUSAND_MULTIPLIER);

  return GB;

}


/**
  Wrap Force Comp and Polls with stats timer to record time spent in the routine.

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     CompType - Enum of Comp being forced

  @retval mrcSuccess Poll returns expected
  @retval mrcDeviceBusy did not return expected.
**/
MrcStatus
ForceRcomp (
  IN OUT MrcParameters *const MrcData,
  IN     COMP_CYCLE_TYPE      CompType
  )
{
  MrcStatus Status;
  MRC_STATISTIC Timer = MRC_FORCE_RCOMP_TIME;

  MrcStatsStartTimer (MrcData, Timer);
  Status = WrappedForceRcomp (MrcData, CompType);
  MrcStatsEndTimer (MrcData, Timer);

  return Status;
}


/**
  This function does Force Comp and Polls

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     CompType - Enum of Comp being forced

  @retval mrcSuccess if Poll returns expected otherwise mrcDeviceBusy.
**/
MrcStatus
WrappedForceRcomp (
  IN OUT MrcParameters *const MrcData,
  IN     COMP_CYCLE_TYPE      CompType
  )
{
  const MRC_FUNCTION  *MrcCall;
  GSM_GT     GetSetEnum;
  INT64      GetSetVal;
  UINT64     Timeout;
  BOOLEAN    IsAllCompDone;
  BOOLEAN    Done;
  DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_STRUCT  PhyPmStatus0;

  MrcCall = MrcData->Inputs.Call.Func;

  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout

  do {
    PhyPmStatus0.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_REG);
    IsAllCompDone = (PhyPmStatus0.Bits.AllCompDone == DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AllCompDone_MAX);
  } while (!IsAllCompDone && (MrcCall->MrcGetCpuTime () < Timeout));

  if (!IsAllCompDone) {
    return mrcDeviceBusy;
  }

  GetSetVal = 1;
  switch (CompType) {
    case DvfsComp:
      GetSetEnum = GsmCompCtrlDVFSComp;
      break;

    case CompDist:
      GetSetEnum = GsmCompCtrlCompDist;
      break;

    case CompUpdate:
      GetSetEnum = GsmCompCtrlCompUpdate;
      break;
    // If DCC Comp is required to be run as part of Periodic Comp then call ForceRcomp with FullCompDccFix param
    case FullCompDccFix:
    case FullComp:
    default:
      GetSetEnum = GsmCompCtrlPeriodicComp;
      break;
  }
  MrcGetSetNoScope (MrcData, GetSetEnum, WriteCached, &GetSetVal);
  MrcFlushRegisterCachedData (MrcData);

  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout

  Done = FALSE;
  do {
    MrcGetSetNoScope (MrcData, GetSetEnum, ReadCached, &GetSetVal);
    Done = (GetSetVal == 0);
  } while (!Done && (MrcCall->MrcGetCpuTime () < Timeout));

  return (!Done) ? mrcDeviceBusy : mrcSuccess;
}

/**
  This function sets DDRIO IP Version, Derivative, Segment, Stepping based on DDRPHY_MISC_SAUG_CR_IPVERSION_REG.

  @param[in]  MrcData     - Include all MRC global data.

  @retval Nothing.
**/
MrcStatus
MrcSetupDdrIoIpInfo (
  OUT MrcParameters *const MrcData
  )
{
  MrcInput             *Inputs;
  MrcDebug             *Debug;
  MrcStatus            Status;
  MrcDdrIoIpVersion    *IpVersion;
  BOOLEAN              IsIpSegmentS;
  DDRPHY_MISC_SAUG_CR_IPVERSION_STRUCT   DdrMiscIpVersion;
  const char           *StepStr;

  Inputs          = &MrcData->Inputs;
  Debug           = &MrcData->Outputs.Debug;
  IpVersion       = &Inputs->DdrIoIpVersion;
  Status          = mrcSuccess;
  IsIpSegmentS    = FALSE;
  StepStr         = NULL;

  DdrMiscIpVersion.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_IPVERSION_REG);
  IpVersion->Bits.Version    = (UINT8)DdrMiscIpVersion.Bits.Version;
  IpVersion->Bits.Derivative = (UINT8)DdrMiscIpVersion.Bits.Derivative;
  IpVersion->Bits.Segment    = (UINT8)DdrMiscIpVersion.Bits.Segment;
  IpVersion->Bits.Stepping   = (UINT8)DdrMiscIpVersion.Bits.Subversion;

  if (IpVersion->Bits.Stepping > ipStepLast) {
    IpVersion->Bits.Stepping = ipStepLast;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s = %d\n", gWarnString, gMrcIpSteppingStr, ipStepLast);
  }

  if (IpVersion->Bits.Version != ipVerDdrIoLnlPtl) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s\n", gWarnString, gMrcIpVersionStr);
      Status  = mrcFail;
  }

  switch (IpVersion->Bits.Segment) {
    case ipSegDT_HALO:
      Inputs->IsDdrIoDtHalo = TRUE;
      IsIpSegmentS = TRUE;
      switch (IpVersion->Bits.Stepping) {
        default:
        case ipStepA0:
          Inputs->IsDdrIoDtA0 = TRUE;
          StepStr = "A0";
          break;
      }
      break;

    case DDRIO_PTLPUH_IP_SEGMENT:
      Inputs->IsDdrIoUlxUlt = TRUE;
      IsIpSegmentS = FALSE;
      switch (IpVersion->Bits.Stepping) {
        case ipStepB0:
          StepStr = "B0";
          Inputs->IsDdrIoMbB0 = TRUE;
          break;

        default:
        case ipStepA0:
          StepStr = "A0";
          Inputs->IsDdrIoMbA0 = TRUE;
          break;
      }
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s\n", gWarnString, gMrcIpSegmentStr);
      if (Inputs->ExtInputs.Ptr->SimicsFlag)
      {
        StepStr = "A0";
        Inputs->IsDdrIoMbA0 = TRUE;
      }
      else
      {
        Status  = mrcFail;
      }
      break;
  }

  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s%s\n", "PantherLake-", IsIpSegmentS ? "S" : "M" );
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "\nPHY IP Version: %d.%d.%d %s\n",
      IpVersion->Bits.Derivative,
      IpVersion->Bits.Segment,
      IpVersion->Bits.Version,
      (StepStr == NULL) ? "Unknown" : StepStr
    );
  }

  return Status;
}

/**
  Get the max CMD Groups per channel associated with the current memory technology

  @param[in] MrcData  - Pointer to global MRC data.

  @return The maximum number of CMD Groups per channel for the current memory technology
**/
UINT8
MrcGetCmdGroupMax (
  MrcParameters *const MrcData
  )
{
  return (MrcData->Outputs.IsDdr5) ? MRC_DDR5_CMD_GRP_MAX: 1;
}


/**
  This function will set the DDRSCRAM_CR_DDRLASTCR_REG and poll for InitComplete bits to be high.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus  - mrcDeviceBusy if poll InitComplete hits timeout.
                     - mrcSuccess otherwise.
**/
MrcStatus
MrcSetLastCR (
  IN MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION *MrcCall;
  MrcInput           *Inputs;
  MrcDebug           *Debug;
  UINT64              Timeout;
  BOOLEAN             IsInitComplete;
  DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_STRUCT  PhyPmStatus0;

  Inputs            = &MrcData->Inputs;
  MrcCall           = Inputs->Call.Func;
  Debug             = &MrcData->Outputs.Debug;

  MrcWriteCR (MrcData, DDRSCRAM_CR_DDRLASTCR_REG, 1);
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout
  // Poll InitComplete to be high after programming LastCR
  do {
    PhyPmStatus0.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_REG);
    IsInitComplete = (PhyPmStatus0.Bits.InitComplete == DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_InitComplete_MAX);
    if (Inputs->ExtInputs.Ptr->SimicsFlag != 0) {
      IsInitComplete = 1;
    }
    // Error out if exceeds timeout
    if (MrcCall->MrcGetCpuTime() > Timeout) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s Timeout polling InitComplete\n", gErrString);
      return mrcDeviceBusy;
    }
  } while (!IsInitComplete);

  return mrcSuccess;
}


/**
  This function caclulates the IoChNotPop mask based on the PHY
  DdrMiscControl0.Channel_Not_Populated register field definition.

  Assumtions:
    MrcData->Outputs.MaxChannels is never zero

  @param[in] MrcData - Include all MRC global data.

  @return The Channel_Not_Populated bit mask based on the current
          platform memory population.
**/
UINT8
MrcCalcIoChNotPopMask (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput         *Outputs;
  UINT8             IoChNotPop;
  UINT8             MaxChannels;
  UINT32            Index;
  UINT8             McIndex;
  UINT8             ChIndex;
  UINT8             RankPresent;

  Outputs     = &MrcData->Outputs;
  MaxChannels = Outputs->MaxChannels;

  // The Channel_Not_Populated bits to set differ depending on the channel width:
  //   - In 64b channel, set 0-3 (ch0) or 4-7 (ch1).
  //   - In 32b channel, set 0-1 (ch0), 2-3 (ch1), 4-5(ch2) & 6-7 (ch3).
  //   - In 16b channel, set 0 (ch0), 1 (ch1), 2 (ch2), ..
  IoChNotPop = 0;
  for (Index = 0; Index < MRC_CCC_NUM; Index++) {
    McIndex = (UINT8)(Index / MAX_CHANNEL);
    ChIndex = (Index / (MAX_CHANNEL/MaxChannels)) % MaxChannels;
    RankPresent = Outputs->Controller[McIndex].Channel[ChIndex].ValidRankBitMask;
    if (RankPresent == 0) {
      IoChNotPop |= (1 << Index);
    }
  }

  return IoChNotPop;
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
  This routine computes the read/write fifo pointer delays

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  tCWL              - Write Latency for current channel
  @param[in]  Controller        - Current Controller
  @param[in]  Channel           - Current Channel
  @param[out] tCWL4TxDqFifoWrEn - Pointer to the write TX DQ fifo delay
  @param[out] tCWL4TxDqFifoRdEn - Pointer to the read TX DQ fifo delay

  @retval N/A.
**/
VOID
MrcGetTxDqFifoDelay (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8         tCWL,
  IN     UINT8         Controller,
  IN     UINT8         Channel,
  OUT    UINT32        *tCWL4TxDqFifoWrEn,
  OUT    UINT32        *tCWL4TxDqFifoRdEn
  )
{
  MrcOutput  *Outputs;
  MrcInput   *Inputs;
  UINT8      Index;
  UINT8      WrPreambleT;
  UINT8      WrPreambleLow;
  UINT8      WrPreambleVar;
  BOOLEAN    IsGear4;
  BOOLEAN    IsLpddr;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  IsLpddr = Outputs->IsLpddr;
  IsGear4 = (Outputs->GearMode) ? 1 : 0;
  MrcGetPrePostamble (MrcData, GetTimingWrite, &WrPreambleT, &WrPreambleLow, NULL);

  *tCWL4TxDqFifoWrEn = tCWL;  // @todo_lnl should also include a programmable delay from Dunit
  *tCWL4TxDqFifoRdEn = tCWL;

  // tcwl4txdqfifowren = ((tech==DDRPHY_LPDDR5) ? ((gears==2 ? ((cwls*4)-'h8) : ((cwls*2)-8)) -4):
  //                       ((gears==2)?(cwls-6):(cwls/2-6))-4)
  // tcwl4txdqfifowren = (tcwl4txdqfifowren) + (tcwl4txdqfifowren%2);
  //
  // wr_preamble = WrPreambleT + WrPreambleLow - 1
  // tcwl4txdqfiforden = (tech==DDRPHY_LPDDR5) ? ((gears==2)? (Write_latency*4-7) : (write_latency*2-7)) :
  //                      ((gears==2) ? ((write_latency-'h7)-(wr_preamble) ): (write_latency/2-'h7-((wr_preamble+1)/2) ));
  //
  if (IsLpddr) {
    Index = (Controller * MAX_CHANNEL) + Channel;
    *tCWL4TxDqFifoWrEn *= (IsGear4 ? 2 : 4);
    *tCWL4TxDqFifoWrEn -= 12;

    *tCWL4TxDqFifoRdEn *= (IsGear4 ? 2 : 4);
    *tCWL4TxDqFifoRdEn -= 7;

    *tCWL4TxDqFifoRdEn += ((Index >= 6) ? 1 : 0);
  } else { // Ddr5
    Index = (Controller * MAX_CHANNEL) + (Channel * 2);
    *tCWL4TxDqFifoWrEn /= (IsGear4 ? 2 : 1);
    *tCWL4TxDqFifoWrEn -= 6;
    *tCWL4TxDqFifoWrEn -= 4;

    WrPreambleVar = WrPreambleT + WrPreambleLow - 1;
    *tCWL4TxDqFifoRdEn /= (IsGear4 ? 2 : 1);
    *tCWL4TxDqFifoRdEn -= 7;
    *tCWL4TxDqFifoRdEn -= IsGear4 ? ((WrPreambleVar + 1) / 2) : WrPreambleVar;
    *tCWL4TxDqFifoRdEn += (Outputs->Frequency == f3200) ? (IsGear4 ? 1 : 0) : 0;
  }
  *tCWL4TxDqFifoWrEn += *tCWL4TxDqFifoWrEn % 2;
  *tCWL4TxDqFifoRdEn += Inputs->ExtInputs.Ptr->CccPinsInterleaved ?  DdrioChDeltaCccIL[Index] : DdrioChDelta[Index];
}

/**
  Display MR value from the host struct

  @param[in] MrcData       - Include all MRC global data.
  @param[in] MrAddr         - MR Address.
**/
VOID
DisplayMRContentFromHost (
  IN MrcParameters *const MrcData,
  IN MrcModeRegister      MrAddr
  )
{
  MrcChannelOut   *ChannelOut;
  MrcOutput       *Outputs;
  UINT8  Controller;
  UINT8  Channel;
  UINT8  Rank;
  UINT8  Data;
  UINT8  MrIndex;
  Outputs = &MrcData->Outputs;

  MrIndex = MrcMrAddrToIndex (MrcData, &MrAddr);

  if (MrIndex >= mrIndexMax) {
    MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "\nMrPtr argument is out of bounds\n");
    // Prevent accessing index out of bound later in function
    return;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (MrcRankExist (MrcData, Controller, Channel, Rank) == 0) {
          continue;
        }
        Data = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR[MrIndex];
        MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "MC%u.C%u.R%u MR%d: 0x%X\n", Controller, Channel, Rank, MrAddr, Data);
      }
    } // Channel
  } // Controller
}


/**
  This function Sets InitComplete Override before sending PM message and Restores InitComplete Override after PM message is sent.

  @param[in] MrcData              - All the MRC global data.
  @param[in] LpJedecFreqSwitch    - Boolean indicator that the InitComplete Override is for the LPDDR frequency switch flow.
  @param[in] IsOverrideMode       - Boolean to indicate Set or Restore mode.
  @param[in] InitCompleteValSave  - Pointer to save / restore InitComplete Override Value

  @retval Returns mrcSuccess if InitComple Override is successful, returns mrcFail if InitComplete Override pointer is NULL
**/
MrcStatus
InitCompleteOverride (
  IN MrcParameters *const MrcData,
  IN BOOLEAN       IsLpJedecFreqSwitch,
  IN BOOLEAN       IsOverrideMode,
  OUT INT64        *InitCompleteValSave
 )
{
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  INT64               InitCompleteOvrdValSave;
  INT64               GetSetVal;

  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;

  if (InitCompleteValSave == NULL) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: invalid to InitCompleteOvrdVal", gNullPtrErrStr);
    return mrcFail;
  }

  if (IsOverrideMode) { // Set Mode
    MrcGetSetNoScope (MrcData, GsmInitCompleteOvrdVal, ReadFromCache, &InitCompleteOvrdValSave);
    *InitCompleteValSave = InitCompleteOvrdValSave;
    GetSetVal = 1;
    MrcGetSetNoScope (MrcData, GsmInitCompleteOvrd, WriteToCache, &GetSetVal);
    GetSetVal = 0;
    MrcGetSetNoScope (MrcData, GsmInitCompleteOvrdVal, WriteToCache, &GetSetVal);
  } else { // Restore Mode
    InitCompleteOvrdValSave = *InitCompleteValSave;
    if (!IsLpJedecFreqSwitch) {
      MrcGetSetNoScope (MrcData, GsmInitCompleteOvrdVal, WriteToCache, &InitCompleteOvrdValSave);
    }
    GetSetVal = 0;
    MrcGetSetNoScope (MrcData, GsmInitCompleteOvrd, WriteToCache, &GetSetVal);
  }
  MrcFlushRegisterCachedData (MrcData);

  return mrcSuccess;
}


/**
  This function caluculates the Min and Max TxFifo Limits

  @param[in, out]    MrcData  - Include all MRC global data.
  @param[out]        MinLimit - Min Fifo Limit in tCK
  @param[out]        MaxLimit - Max Fifo Limit in tCK
**/
VOID
TxFifoLimitstCK (
  IN OUT MrcParameters *const MrcData,
  OUT    UINT32        *const MinLimit,
  OUT    UINT32        *const MaxLimit
  )
{
  MrcOutput *Outputs;
  MrcInput  *Inputs;
  MrcChannelOut *ChannelOut;
  MrcTiming     *Timing;
  INT64     tCWL;
  UINT8     Twpre;
  UINT8     WrPreambleOffset;
  UINT32    FirstController;
  UINT32    FirstChannel;
  BOOLEAN   IsGear4;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel = Outputs->Controller[FirstController].FirstPopCh;
  ChannelOut = &Outputs->Controller[FirstController].Channel[FirstChannel];
  Timing  = &ChannelOut->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  IsGear4 = Outputs->GearMode ? 1 : 0;

  // Account for Write Preamble:
  // 2 tCK => No change for MinMaxLimit values
  // 3 tCK => MinMaxLimitValue -= 1
  // 4 tCK => MinMaxLimitValue -= 2
  Twpre = MrcGetTwpre (MrcData);
  WrPreambleOffset = Twpre - 2;

  // set tCWL in terms of Qclks
  tCWL = Timing->tCWL / (IsGear4 ? 2 : 1);
  *MinLimit = (UINT32) tCWL - PHY_FIFO_CONSTANT + 1 - WrPreambleOffset;
  // Max Limit needs to account for FIFO range of 16 QClK
  *MaxLimit = (UINT32) tCWL - PHY_FIFO_CONSTANT + 1 - WrPreambleOffset + (WRT_FIFO_RANGE_UI / (IsGear4 ? 4 : 2));

  // Put in terms of tCK
  *MinLimit *= IsGear4 ? 2 : 1;
  *MaxLimit *= IsGear4 ? 2 : 1;
}


/**
  This function returns the delay of RxDqsPerBitDeskew and RxDQPerBitDeskewOffset for a specific Byte.
  Based on the input param "Sign", the composite MIN or MAX margin will be found for all bits within a byte

  @param[in] MrcData              - Pointer to MRC global data.
  @param[in] Controller           - Current Controller
  @param[in] Channel              - Current Channel
  @param[in] Rank                 - Current Rank
  @param[in] Byte                 - Current Byte
  @param[out] BitDelayRxDqs       - MIN or MAX RxDqPerBitDeskew across all bits
  @param[out] BitOffsetRxDqs      - MIN or MAX RxDQPerBitDeskewOffset across all bits
  @param[in] Sign                 - Sign of the delay (0 - negative/min, 1 - positive/max).

**/
VOID
MrcCalcCurrentRxBitDelay (
  IN OUT   MrcParameters *const MrcData,
  IN const UINT8                Controller,
  IN const UINT8                Channel,
  IN const UINT8                Rank,
  IN const UINT8                Byte,
  OUT      INT32         *const BitDelayRxDqs,
  OUT      INT32         *const BitOffsetRxDqs,
  IN const UINT8                Sign
  )
{
  UINT8               Bit;
  INT64               CurrentBitDeskew;
  INT64               CurrentBitDeskewOffset;
  INT64               BitDeskewLimitMax;
  INT64               BitDeskewOffsetLimitMax;
  INT64               BitDeskewLimitMin;
  INT64               BitDeskewOffsetLimitMin;

  // Find RxDqsBitDeskew and RxDqsBitOffset limits
  MrcGetSetLimits (MrcData, RxDqsBitDelay, 0, &BitDeskewLimitMin, &BitDeskewLimitMax, NULL);
  MrcGetSetLimits (MrcData, RxDqsBitOffset, 0, &BitDeskewOffsetLimitMin, &BitDeskewOffsetLimitMax, NULL);

  for (Bit = 0; Bit < MAX_BITS; Bit++) {
    if (!MrcBitExist (Controller, Byte, Bit)) {
      continue; // DDR5 ECC byte has only 4 bits DQ
    }
    MrcGetSetBit (MrcData, Controller, Channel, Rank, Byte, Bit, RxDqsBitDelay, ReadFromCache, &CurrentBitDeskew);
    MrcGetSetBit (MrcData, Controller, Channel, Rank, Byte, Bit, RxDqsBitOffset, ReadFromCache, &CurrentBitDeskewOffset);

    // Find Min RxDQPerBitDeskew and CurrentBitDeskewOffset
    if (Sign == 0) {
      if (CurrentBitDeskew < BitDeskewLimitMax) {
        BitDeskewLimitMax = CurrentBitDeskew;
      }
      if (CurrentBitDeskewOffset < BitDeskewOffsetLimitMax) {
        BitDeskewOffsetLimitMax = CurrentBitDeskewOffset;
      }
    } else { // Find Max RxDQPerBitDeskew and CurrentBitDeskewOffset
      if (CurrentBitDeskew > BitDeskewLimitMin) {
        BitDeskewLimitMin = CurrentBitDeskew;
      }
      if (CurrentBitDeskewOffset > BitDeskewOffsetLimitMin) {
        BitDeskewOffsetLimitMin = CurrentBitDeskewOffset;
      }
    }
  } // Bit
  if (Sign == 0) {
    *BitDelayRxDqs = (INT32) BitDeskewLimitMax;
    *BitOffsetRxDqs = (INT32) BitDeskewOffsetLimitMax;
  } else {
    *BitDelayRxDqs = (INT32) BitDeskewLimitMin;
    *BitOffsetRxDqs = (INT32) BitDeskewOffsetLimitMin;
  }
}


/**
  This function returns the current RxDqsDelay for N and P on CompDqsDelay.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Current Controller
  @param[in] Channel    - Current Channel
  @param[in] Byte       - Current Byte
  @param[out] DqsPDelay - Current RxDqsP Comp Delay
  @param[out] DqsNDelay - Current RxDqsP Comp Delay
**/
VOID
MrcCalcCurrentCompDelayRx (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                Byte,
  OUT    INT32         *const DqsPDelay,
  OUT    INT32         *const DqsNDelay
  )
{
  INT64  RxDqsP;
  INT64  RxDqsN;
  INT64  RxDqsOffset;

  MrcGetSetChStrb (MrcData, Controller, Channel, Byte, RxCompDqsDelayP,  ReadCached,  &RxDqsP);
  MrcGetSetChStrb (MrcData, Controller, Channel, Byte, RxCompDqsDelayN,  ReadCached,  &RxDqsN);
  MrcGetSetChStrb (MrcData, Controller, Channel, Byte, RxCompDqsOffset,  ReadCached,  &RxDqsOffset);

  RxDqsP -= (INT32) ((UINT8) RxDqsOffset * 64);
  RxDqsN -= (INT32) ((UINT8) RxDqsOffset * 64);

  if (DqsPDelay != NULL) {
    *DqsPDelay = (INT32) RxDqsP;
  }
  if (DqsNDelay != NULL) {
    *DqsNDelay = (INT32) RxDqsN;
  }
}


/**
  This function find the Min/Max Rx Path delays within a channel for all populated channels
  The parameters used are RxDqsNDelay, RxDqsPDelay, BitDelayRxDqs, RxDqsPComp, and RecEnDelay

  @param[in] MrcData - Include all MRC global data.
  @param[out] TxPath - Returns Min/Max delays for each channel
**/
VOID
GetRxPathMinMax (
  IN OUT MrcParameters *const MrcData,
  OUT    RxPathMinMax  *const RxPath
  )
{
  MrcOutput  *Outputs;
  UINT8      Controller;
  UINT8      Channel;
  UINT8      Byte;
  UINT8      Rank;
  UINT8      MaxChannel;
  INT32      BitOffsetRxDqs;
  INT32      BitDelayRxDqs;
  INT32      RxDqsPComp;
  INT32      RxDqsNComp;
  UINT32     BitDelay;
  INT64      RxDqsN;
  INT64      RxDqsP;
  INT64      RcvEnPi;
  UINT32     MaxRcvEnInByte;
  UINT32     MinRcvEnInByte;
  UINT32     DeltaRcvEnPi;
  UINT32     DeltaDqsPi;
  UINT32     DqsPPi;
  UINT32     DqsNPi;
  UINT32     MaxPiCode;
  UINT32     MinPiCode;

  Outputs = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      // Set defualt Min values
      RxPath->MinRcvEnPi[Controller][Channel] = MRC_UINT32_MAX;
      RxPath->MinRxDqsBit[Controller][Channel] = MRC_UINT32_MAX;
      RxPath->MinRxDqsN[Controller][Channel] = MRC_UINT32_MAX;
      RxPath->MinRxDqsP[Controller][Channel] = MRC_UINT32_MAX;
      RxPath->MinRxDqsNComp[Controller][Channel] = MRC_UINT32_MAX;
      RxPath->MinRxDqsPComp[Controller][Channel] = MRC_UINT32_MAX;

      for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
        MaxRcvEnInByte = MaxPiCode = 0;
        MinRcvEnInByte = MinPiCode = MRC_UINT32_MAX;
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
            continue;
          }
          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, RxDqsNDelay, ReadCached, &RxDqsN);
          RxPath->MaxRxDqsN[Controller][Channel] = MAX ((UINT32)RxDqsN, RxPath->MaxRxDqsN[Controller][Channel]);
          RxPath->MinRxDqsN[Controller][Channel] = MIN ((UINT32)RxDqsN, RxPath->MinRxDqsN[Controller][Channel]);

          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, RxDqsPDelay, ReadCached, &RxDqsP);
          RxPath->MaxRxDqsP[Controller][Channel] = MAX ((UINT32)RxDqsP, RxPath->MaxRxDqsP[Controller][Channel]);
          RxPath->MinRxDqsP[Controller][Channel] = MIN ((UINT32)RxDqsP, RxPath->MinRxDqsP[Controller][Channel]);

          MrcCalcCurrentRxBitDelay (MrcData, Controller, Channel, Rank, Byte, &BitDelayRxDqs, &BitOffsetRxDqs, 0);
          BitDelay = (UINT32)BitDelayRxDqs - (BitOffsetRxDqs >> 1);
          RxPath->MinRxDqsBit[Controller][Channel] = MIN (BitDelay, RxPath->MinRxDqsBit[Controller][Channel]);
          DqsNPi = (UINT32) (RxDqsN + BitDelay);

          MrcCalcCurrentRxBitDelay (MrcData, Controller, Channel, Rank, Byte, &BitDelayRxDqs, &BitOffsetRxDqs, 1);
          BitDelay = (UINT32)BitDelayRxDqs + (BitOffsetRxDqs >> 1);
          RxPath->MaxRxDqsBit[Controller][Channel] = MAX (BitDelay, RxPath->MaxRxDqsBit[Controller][Channel]);
          DqsPPi = (UINT32) (RxDqsP + BitDelay);

          MaxPiCode = MAX (MAX (DqsPPi, DqsNPi), MaxPiCode);
          MinPiCode = MIN (MIN (DqsPPi, DqsNPi), MinPiCode);

          MrcCalcCurrentCompDelayRx (MrcData, Controller, Channel, Byte, &RxDqsPComp, &RxDqsNComp);
          RxPath->MinRxDqsPComp[Controller][Channel] = MIN ((UINT32)RxDqsPComp, RxPath->MinRxDqsPComp[Controller][Channel]);
          RxPath->MinRxDqsNComp[Controller][Channel] = MIN ((UINT32)RxDqsNComp, RxPath->MinRxDqsNComp[Controller][Channel]);

          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, RecEnDelay, ReadCached, &RcvEnPi);
          RxPath->MinRcvEnPi[Controller][Channel] = MIN ((UINT32)RcvEnPi, RxPath->MinRcvEnPi[Controller][Channel]);
          RxPath->MaxRcvEnPi[Controller][Channel] = MAX ((UINT32)RcvEnPi, RxPath->MaxRcvEnPi[Controller][Channel]);

          MaxRcvEnInByte = MAX ((UINT32) RcvEnPi, MaxRcvEnInByte);
          MinRcvEnInByte = MIN ((UINT32) RcvEnPi, MinRcvEnInByte);
        }
        DeltaRcvEnPi = MaxRcvEnInByte - MinRcvEnInByte;
        DeltaDqsPi = MaxPiCode - MinPiCode;
        RxPath->DeltaPiCode[Controller][Channel] = MAX (DeltaRcvEnPi + DeltaDqsPi, RxPath->DeltaPiCode[Controller][Channel]);
      }
    }
  }
}


/**
  This function find the Min/Max Tx Path delays within a channel for all populated channels
  The parameters used are TxDqsDelay, TxDqDelay, and TxDqBitDelay

  @param[in] MrcData - Include all MRC global data.
  @param[out] TxPath - Returns Min/Max delays for each channel
**/
VOID
GetTxPathMinMax (
  IN OUT MrcParameters *const MrcData,
  OUT    TxPathMinMax  *const TxPath
  )
{
  MrcOutput  *Outputs;
  UINT8      Controller;
  UINT8      Channel;
  UINT8      Byte;
  UINT8      Bit;
  UINT8      Rank;
  UINT8      MaxChannel;
  INT64      TxDqs;
  INT64      TxDq;
  INT64      TxDqBit;
  INT64      MinDqBit;
  INT64      MaxDqBit;
  UINT32     MaxTxDqsInByte;
  UINT32     MinTxDqsInByte;
  UINT32     DeltaTxDqs;

  Outputs = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      // Set defualt Min values
      TxPath->MinTxDqs[Controller][Channel] = MRC_UINT32_MAX;
      TxPath->MinTxDq[Controller][Channel] = MRC_UINT32_MAX;
      TxPath->MinTxDqBit[Controller][Channel] = MRC_UINT32_MAX;
      MinDqBit = MRC_UINT32_MAX;
      MaxDqBit = 0;
      for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
        MaxTxDqsInByte = 0;
        MinTxDqsInByte = MRC_UINT32_MAX;
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
            continue;
          }
          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, TxDqsDelay, ReadCached, &TxDqs);
          TxPath->MaxTxDqs[Controller][Channel] = MAX ((UINT32) TxDqs, TxPath->MaxTxDqs[Controller][Channel]);
          TxPath->MinTxDqs[Controller][Channel] = MIN ((UINT32) TxDqs, TxPath->MinTxDqs[Controller][Channel]);
          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, TxDqDelay, ReadCached, &TxDq);
          TxPath->MaxTxDq[Controller][Channel] = MAX ((UINT32) TxDq, TxPath->MaxTxDq[Controller][Channel]);
          TxPath->MinTxDq[Controller][Channel] = MIN ((UINT32) TxDq, TxPath->MinTxDq[Controller][Channel]);
          for (Bit = 0; Bit < MAX_BITS; Bit++) {
            if (!MrcBitExist (Controller, Byte, Bit)) {
              continue; // DDR5 ECC byte has only 4 bits DQ
            }
            // Read Current Bit Value
            MrcGetSetBit (MrcData, Controller, Channel, Rank, Byte, Bit, TxDqBitDelay, ReadCached, &TxDqBit);
            MinDqBit = MIN (TxDqBit, MinDqBit);
            MaxDqBit = MAX (TxDqBit, MaxDqBit);
          }
          TxPath->MaxTxDqBit[Controller][Channel] = MAX ((UINT32) MaxDqBit, TxPath->MaxTxDqBit[Controller][Channel]);
          TxPath->MinTxDqBit[Controller][Channel] = MIN ((UINT32) MinDqBit, TxPath->MinTxDqBit[Controller][Channel]);
          MaxTxDqsInByte = MAX ((UINT32) TxDqs, MaxTxDqsInByte);
          MinTxDqsInByte = MIN ((UINT32) TxDqs, MinTxDqsInByte);
        }
        DeltaTxDqs = MaxTxDqsInByte - MinTxDqsInByte;
        TxPath->DeltaPiCode[Controller][Channel] = MAX (DeltaTxDqs, TxPath->DeltaPiCode[Controller][Channel]);
      }
    }
  }
}


/**
  This function calculates the DDR5 timings tDQSS which takes into account JEDEC Spec and SOC requirments

  @param[in]  MrcData      - Include all MRC global data.
  @param[out] tDQSSMax     - Max Voltage/Temperature drift window of first rising DQS pre-amble edge relative to CWL CK edge
  @param[out] tDQSSMin     - Min Voltage/Temperature drift window of first rising DQS pre-amble edge relative to CWL CK edge
  @param[in] WrPreambleLow - Write Preamble low time
**/
VOID
GetDdr5tDQSS (
  IN   MrcParameters *const MrcData,
  OUT  INT32                *tDQSSMax,
  OUT  INT32                *tDQSSMin,
  IN   UINT8                WrPreambleLow
  )
{
  MrcOutput    *Outputs;
  INT32         tDQSMinMax;
  UINT32        TempCo;
  UINT32        PHClk;
  UINT32        tDQSSTemp;

  Outputs = &MrcData->Outputs;
  PHClk   = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);

  GetDdr5tDQSSDram (MrcData, &tDQSMinMax, WrPreambleLow);

  tDQSMinMax = (tDQSMinMax * PI_PER_TCK) / 1000;
  TempCo = ((RANGE (Outputs->VccClkVoltage, 650, 850) - 650)*100) / 800;

  tDQSSTemp = ((45 - TempCo) * 90 + 3000);
  tDQSSTemp /= 100;
  // Convert to PI ticks
  tDQSSTemp = (tDQSSTemp * PI_PER_TCK / PHClk) + 11;
  if (tDQSSMax != NULL && tDQSSMin != NULL) {
    *tDQSSMax = tDQSMinMax + tDQSSTemp;
    *tDQSSMin = -*tDQSSMax;
  }
}

/**
  This function sets the Cs2NReset field for when switching CS NMode or during Jedec Reset w/o PM6/13

  @param[in] MrcData - Include all MRC global data.
**/
VOID
DdrioCs2NReset (
  IN MrcParameters *const MrcData
  )
{
  INT64 GetSetEn = 1;

  MrcGetSetNoScope (MrcData, GsmIocCs2NReset, ForceWriteCached, &GetSetEn);
}

/**
  This function sets up DDRIO set of registers for CS 1N or 2N modes

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  IsEnable - Whether it is 2N (MRC_ENABLE) or 1N (MRC_DISABLE).
**/
VOID
DdrIoCs2NEnableOrDisable (
  IN MrcParameters *const MrcData,
  IN BOOLEAN IsEnable
  )
{
  INT64 GetSetVal;

  GetSetVal = IsEnable ? 1 : 0;
  MrcGetSetNoScope (MrcData, GsmIocCsGearDownEnable, WriteCached, &GetSetVal);
  MrcGetSetNoScope (MrcData, GsmIocCsGearDownSrDramTrack, WriteCached, &GetSetVal);
  MrcGetSetNoScope (MrcData, GsmIocCsGearDownForce, ForceWriteCached, &GetSetVal);

  if (!IsEnable) {
    DdrioCs2NReset (MrcData);
  }
}

