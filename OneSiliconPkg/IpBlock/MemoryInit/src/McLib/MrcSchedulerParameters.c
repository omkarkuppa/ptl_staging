/** @file
  This module configures the memory controller scheduler.

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

// Include files
#include "MrcSchedulerParameters.h"
#include "MrcTimingConfiguration.h"
#include "MrcChipApi.h"
#include "MrcLpddr5.h"
#include "MrcMemoryApi.h"
#include "MrcHalApi.h"
#include "MrcMcSiSpecific.h"

// LP5 SplitACT enable BIOS knob defines input values as:
// 0 = Auto, 1 = Disable, 2 = Enable
#define LP5_SPLIT_ACT_ENABLE 2

typedef struct {
  GSM_GT Knob;
  UINT32 Delay;
  UINT8  Multiplier;
} McSchedulerPriorityCount;

static const McSchedulerPriorityCount PrCountConfig[] = {
  {GsmMccVc0LatencyGaurdTimerx16,     VC0_LATENCY_GUARD_TIMER_x16_PS,     16},
  {GsmMccVc1LatencyGuardTimerx8,      VC1_LATENCY_GUARD_TIMER_x8_PS,      8 },
  {GsmMccLatencySensitiveTimerx16,    LATENCY_SENSITIVE_TIMER_x16_PS,     16},
  {GsmMccVc1IsocLatencyGuardTimerx8,  VC1_ISOC_LATENCY_GUARD_TIMER_x8_PS, 8 }
};

/**
  This function enable/disables the Memory Controller low power management features.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] IsEnabled  - TRUE enables all PM features, FALSE disables them.

  @retval Nothing.
**/
void
MrcMcLowPowerMode (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsEnabled
  )
{
  MrcInput  *Inputs;
  INT64     GetSetVal;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  GetSetVal = (IsEnabled && (ExtInputs->LpMode & MRC_LPMODE0P5)) ? 1 : 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccIdleEnable, WriteCached | PrintValue, &GetSetVal);

  GetSetVal = (IsEnabled && (ExtInputs->LpMode & MRC_LPMODE2)) ? 1 : 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccPowerDownEnable, WriteCached | PrintValue, &GetSetVal);

  // For LPmode4 we must also enable GsmMccSelfRefreshEnable (LPMODE3)
  GetSetVal = (IsEnabled && ((ExtInputs->LpMode & MRC_LPMODE3) || (ExtInputs->LpMode4 != MrcLpMode4_Disable))) ? 1 : 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSelfRefreshEnable, WriteCached | PrintValue, &GetSetVal);

  if (IsEnabled && ExtInputs->LpMode4 != MrcLpMode4_Disable) {
    MrcMcLpmodeSelfRefreshLength (MrcData);
  }
}

/**
  This function configures the memory controller scheduler.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
void
MrcSchedulerParametersConfig (
  IN MrcParameters *const MrcData
  )
{
  static const MRC_GSM_INIT_TYPE_8 CmiSourceKnobs[] = {
    {GsmMccCmiSourceId0, CMI_SOURCE_ID0},
    {GsmMccCmiSourceId1, CMI_SOURCE_ID1},
    {GsmMccCmiSourceId2, CMI_SOURCE_ID2},
    {GsmMccCmiSourceId3, CMI_SOURCE_ID3},
  };
  const MrcInput  *Inputs;
  MrcOutput       *Outputs;
  MrcDebug        *Debug;
  MrcChannelOut   *ChannelOut;
  MrcDimmOut      *DimmOut;
  MrcTiming       *Timing;
  MrcDdrType      DdrType;
  INT64           GetSetVal;
  INT64           GetSetDis;
  INT64           GetSetEn;
  INT64           FreqPoint;
  INT64           CmdStretch;
  INT64           DramTechnology;
  INT64           tCKCKEH;
  INT64           tCPDED;
  UINT32          Offset;
  UINT32          Channel;
  UINT32          IpChannel;
  UINT32          Controller;
  UINT32          tCL;
  UINT32          tCWL;
  UINT32          NMode;
  UINT32          Data32;
  UINT32          tRDWRdg;
  UINT32          tRDRDdg;
  INT64           tCSLCK;
  INT64           tWckStop;
  INT64           tWckOff;
  INT64           tCKFSPX;
  INT64           WckDfiOffset;
  INT64           RdWckAsync;
  INT64           WrWckAsync;
  UINT8           tWCKPRE_TOGGLE_FS;
  UINT8           tWCKPRE_TOGGLE_RD;
  UINT8           tWCKPRE_TOGGLE_WR;
  UINT8           tWCK_STOP;
  UINT8           AddressMirror;
  UINT32          SerializeZq;
  UINT8           Dimm;
  BOOLEAN         GearMode;
  UINT8           Index;
  BOOLEAN         IsLpddr5;
  BOOLEAN         IsLpddr;
  BOOLEAN         IsDdr5;
  BOOLEAN         IsCkdSupported;
  const MRC_EXT_INPUTS_TYPE               *ExtInputs;
  MC0_CH0_CR_SC_WPQ_THRESHOLD_STRUCT      ScWpqThreshold;
  MC0_CH0_CR_WMM_READ_CONFIG_STRUCT       WmmReadConfig;
  MC0_CH0_CR_ZQCAL_CONTROL_UARCH_STRUCT   ZQcalControl;

  Inputs        = &MrcData->Inputs;
  Outputs       = &MrcData->Outputs;
  ExtInputs     = Inputs->ExtInputs.Ptr;
  Debug         = &Outputs->Debug;
  DdrType       = Outputs->DdrType;
  IsLpddr5      = Outputs->IsLpddr5;
  IsDdr5        = Outputs->IsDdr5;
  IsLpddr       = Outputs->IsLpddr;
  GearMode      = Outputs->GearMode;
  NMode         = MrcGetNMode (MrcData);
  GetSetDis     = 0;
  GetSetEn      = 1;
  IsCkdSupported = Outputs->IsCkdSupported;

  GetSetVal = MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_DEF;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccDis2cByp, WriteToCache | PrintValue, &GetSetVal);

  // only in LPDDR5 due HW bug found by design
  if (ExtInputs->RowPressEn && IsLpddr) {
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccMcCbRowPressDisAutoPre, WriteToCache | PrintValue, &GetSetEn);
  }

  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSchedDisClkGateLocal, WriteToCache | PrintValue, &GetSetDis);
  MrcFlushRegisterCachedData (MrcData);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccDisStarvedPriorityOnNewReq, WriteToCache | PrintValue, &GetSetEn);

  MrcMcLowPowerMode (MrcData, TRUE);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccIdleEnable, WriteToCache, &GetSetDis);

  for (Index = 0; Index < ARRAY_COUNT (PrCountConfig); Index++) {
    Data32 = DIVIDECEIL (PrCountConfig[Index].Delay,  Outputs->Dclkps);
    // convert to x16 dclk
    GetSetVal = DIVIDECEIL (Data32, PrCountConfig[Index].Multiplier);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, PrCountConfig[Index].Knob, WriteToCache, &GetSetVal);
  }

  GetSetVal = 0x10;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccDelayPMAckCycles, WriteToCache, &GetSetVal);


  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccAllowRHDebtSR, WriteToCache, &GetSetDis);

  for (Index = 0; Index < ARRAY_COUNT (CmiSourceKnobs); Index++) {
    GetSetVal = CmiSourceKnobs[Index].Value;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, CmiSourceKnobs[Index].Knob, WriteToCache, &GetSetVal);
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!(MrcControllerExist (MrcData, Controller))) {
      continue;
    }

    // ZQ Command Serialize Options:
    //   DDR5 do not serialize ZQ commands.
    //   LPDDR5 is always serialized regardless of layout.
    SerializeZq = (IsLpddr5) ? 0 : ExtInputs->SharedZqPin;

    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      Timing = &Outputs->Timing[ExtInputs->MemoryProfile];

      IpChannel = LP_IP_CH (IsLpddr, Channel);

      // Disable APD/PPD while MRC is executed to prevent from PDE being sent on bus.
      // These fields are properly initialized in MrcPowerDownConfig.
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccAPD, WriteToCache, &GetSetDis);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPPD, WriteToCache, &GetSetDis);

      // Set the Command N mode and Command Rate Limit
      // Command stretch:
      //   00 - 1N
      //   01 - 2N
      // DDR5 supports 1N and 2N mode in gear 2 and only 2N mode in gear 4
      if (IsDdr5) {
        if (NMode == 1) {
          CmdStretch = GearMode ? 1 : 0;  // Use 2N Mode in Gear4 and 1N mode in Gear2
        } else {
          CmdStretch = NMode - 1;
        }
      } else if (IsLpddr) {
        CmdStretch = 0;
      }

      AddressMirror = 0;
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        if (MrcRankExist (MrcData, Controller, Channel, Dimm * MAX_RANK_IN_DIMM)) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DimmOut->AddressMirrored) {
            AddressMirror |= (1 << Dimm);
          }
        }
      }

      // Enable Tristate before starting training
      FreqPoint = Outputs->SaGvPoint;

      GetSetVal = AddressMirror;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccAddrMirror,     WriteToCache | PrintValue, &GetSetVal);
      GetSetVal = GearMode ? 1 : 0;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccGearMode,       WriteToCache | PrintValue, &GetSetVal);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCmdStretch,     WriteToCache | PrintValue, &CmdStretch);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccFreqPoint,      WriteToCache | PrintValue, &FreqPoint);
      tCPDED = MrcGetTcpded (MrcData);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCPDED,          WriteToCache | PrintValue, &tCPDED);
      tCKCKEH = MrcGetTckckeh (MrcData);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCKCKEH,         WriteToCache | PrintValue, &tCKCKEH);

      GetSetVal = SerializeZq;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccZqSerialize,    WriteToCache | PrintValue, &GetSetVal);

      DramTechnology = DdrType;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccDramType, WriteToCache | PrintValue, &DramTechnology);
      GetSetVal = 0;
      // Make sure ddr5_1dpc feature is turned off - it might be on from the previous SAGV point
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccOneDpc, WriteToCache | PrintValue, &GetSetVal);
      GetSetVal = (IsCkdSupported) ? 1 : 0;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccDdr5CkdEnable, WriteToCache | PrintValue, &GetSetVal);

      // Configure WCK parameters
      if (IsLpddr5) {
        tCL = Timing->tCL;
        tCWL = Timing->tCWL;
        // Ranks must be present incrementally.  Thus we can check the bit mask > 1 to determine that we have more than 1 rank present.
        GetSetVal = (ChannelOut->ValidRankBitMask > 1) ? 1 : 2;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccLp5WckMode, WriteToCache | PrintValue, &GetSetVal);
        if (GetSetVal == MrcLp5WckDynamic) {
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccLp5WckFastMode, WriteCached | PrintValue, &GetSetDis);
        }
        // tWCKPRE_TOGGLE_FS = tWCKENL_FS + tWCKPRE_Static
        tWCKPRE_TOGGLE_FS = MrcGetWckEnlFs (MrcData) + MrcGetWckPreStatic (MrcData);  // Programmed in tCK units

        // tWCKPRE_TOGGLE_RD = tWCKENL_RD + tWCKPRE_Static = RL + 1 - tWCKPRE_total_RD + tWCKPRE_Static
        tWCKPRE_TOGGLE_RD = (UINT8) tCL + 1 - MrcGetWckPreRdTotal (MrcData) + MrcGetWckPreStatic (MrcData);  // Programmed in tCK units

        // tWCKPRE_TOGGLE_WR = tWCKENL_WR + tWCKPRE_Static = WL + 1 - tWCKPRE_total_WR + tWCKPRE_Static
        tWCKPRE_TOGGLE_WR = (UINT8) tCWL + 1 - MrcGetWckPreWrTotal (MrcData) + MrcGetWckPreStatic (MrcData);  // Programmed in tCK units

        // tWCK_STOP
        tWCK_STOP = 0;

        GetSetVal = 0;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWckEnlFs,       WriteToCache | PrintValue, &GetSetVal);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWckEnlRd,       WriteToCache | PrintValue, &GetSetVal);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWckEnlWr,       WriteToCache | PrintValue, &GetSetVal);
        GetSetVal = tWCKPRE_TOGGLE_FS;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWckPreToggleFs, WriteToCache | PrintValue, &GetSetVal);
        GetSetVal = tWCKPRE_TOGGLE_RD;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWckPreToggleRd, WriteToCache | PrintValue, &GetSetVal);
        GetSetVal = tWCKPRE_TOGGLE_WR;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWckPreToggleWr, WriteToCache | PrintValue, &GetSetVal);
        GetSetVal = tWCK_STOP;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWckDfiStop,     WriteToCache | PrintValue, &GetSetVal);
        WckDfiOffset = 4;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWckDfiOffset,   WriteToCache | PrintValue, &WckDfiOffset);
        tCSLCK = MrcGetTcsclk (MrcData);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttCSLCK,   WriteToCache | PrintValue, &tCSLCK);
        tWckStop = MrcGetTwckstop (MrcData);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttWckStop, WriteToCache | PrintValue, &tWckStop);
        GetSetVal = MAX (tWckStop, tCSLCK);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCasStopAdditionalGap, WriteToCache | PrintValue, &GetSetVal);
        RdWckAsync = MrcGetTrdWckAsyncGap (MrcData, tCL);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRdWckAsyncGap,        WriteToCache | PrintValue, &RdWckAsync);
        WrWckAsync = MrcGetTwrWckAsyncGap (MrcData, tCWL);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccWrWckAsyncGap,        WriteToCache | PrintValue, &WrWckAsync);
        tWckOff = MrcGetTwckoff (MrcData);
        MrcGetSetMcCh (MrcData, Controller, Channel, GSmMcttWckOff,              WriteToCache | PrintValue, &tWckOff);
        tCKFSPX = MrcGetTckfspx (MrcData);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttCkFspX,              WriteToCache | PrintValue, &tCKFSPX);
      }

      // Set Opportunistic Read for Write Major Mode
      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_WMM_READ_CONFIG_REG, MC1_CH0_CR_WMM_READ_CONFIG_REG, Controller, MC0_CH1_CR_WMM_READ_CONFIG_REG, IpChannel);
      WmmReadConfig.Data = MrcReadCR (MrcData, Offset);
      MrcWriteCR (MrcData, Offset, WmmReadConfig.Data);

      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_WPQ_THRESHOLD_REG, MC1_CH0_CR_SC_WPQ_THRESHOLD_REG, Controller, MC0_CH1_CR_SC_WPQ_THRESHOLD_REG, IpChannel);
      ScWpqThreshold.Data = MrcReadCR (MrcData, Offset);

      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDWRdg, ReadFromCache | PrintValue, &GetSetVal);
      tRDWRdg = (UINT32) GetSetVal;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdg, ReadFromCache | PrintValue, &GetSetVal);
      tRDRDdg = (UINT32) GetSetVal;

      ScWpqThreshold.Bits.phs_allowed_under_high_wm = DIVIDECEIL (tRDWRdg, tRDRDdg);

      if (IsLpddr5) {
        tRDRDdg = ((UINT32) tRDRDdg) / 4; // Convert from WCK to tCK
      }
      ScWpqThreshold.Bits.read_ph_weight = UDIVIDEROUND (Timing->tRCDtRP, tRDRDdg);

      MrcWriteCR (MrcData, Offset, ScWpqThreshold.Data);

      // Only for debug purposes we want to disable ZQ CAL
      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_ZQCAL_CONTROL_UARCH_REG, MC1_CH0_CR_ZQCAL_CONTROL_UARCH_REG, Controller, MC0_CH1_CR_ZQCAL_CONTROL_UARCH_REG, IpChannel);
      ZQcalControl.Data = MrcReadCR (MrcData, Offset);

      if (Outputs->IsZqDisabled) {
        ZQcalControl.Bits.dis_zq = 1;
      }
      MrcWriteCR (MrcData, Offset, ZQcalControl.Data);
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MC%u C%u: ZQcalControl = %08Xh\n", Controller, Channel, ZQcalControl.Data);

      if (Outputs->IsZqDisabled) {
        // Disable ZQ on SRX: PM_SREF_CONTROL.EN_SRX_ZQCAL = 0
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSrxZqcal,  WriteToCache | PrintValue, &GetSetDis);
      }


    } // Channel


  } // Controller

  GetSetVal = (IsLpddr) ? 0 : 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccPageOpenPolicyMaxCount, WriteCached, &GetSetVal);
  GetSetVal = (ExtInputs->RowPressEn) ? 1 : 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccPageOpenPolicyEn, WriteCached, &GetSetVal);

  if ((MrcData->Inputs.IsCs2NRequested) && (Outputs->Frequency >= f7200) && (MrcGetNMode (MrcData) == CA_2_NMODE)) {
    // We expect to get out of green with CS Geardown enabled
    {
      GetSetVal = 1;
      MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEnCsGearDown, ForceWriteCached, &GetSetVal);
    }
  }

  MrcFlushRegisterCachedData (MrcData);

  if (IsLpddr) {
    GetSetVal = 0;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccLpddrCurrentFsp, WriteCached, &GetSetVal);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccLpddrCurrentFspTrk, WriteCached, &GetSetVal);
  }

  GetSetVal = 1;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableDclk, WriteCached, &GetSetVal);

  if (ExtInputs->Lp5SplitACTEnable) {
    // If Lp5SplitACTEnable == 1: set SCHED_THIRD_CBIT.disable_split_act = 1
    // If Lp5SplitACTEnable == 2: set SCHED_THIRD_CBIT.disable_split_act = 0
    GetSetVal = (ExtInputs->Lp5SplitACTEnable == LP5_SPLIT_ACT_ENABLE) ? 0 : 1;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccDisableSplitAct, WriteCached | PrintValue, &GetSetVal);
  }

}

/**
  Set ODT Logic behavior.

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      Profile - Behavior type. Currently Profile 1 is supported

  @retval none
**/
void
MrcSetOdtMatrix (
  IN OUT MrcParameters *const MrcData,
  IN UINT8                    Profile
  )
{
  MrcDebug      *Debug;
  MrcOutput     *Outputs;
  MrcInput      *Inputs;
  MrcChannelOut *ChannelOut;
  MrcDimmOut    *DimmOut;
  INT64         EnableOdtMatrix;
  UINT32        Controller;
  UINT32        Dimm;
  UINT32        MaxDimm;
  UINT32        Offset;
  UINT8         Channel;
  MC0_CH0_CR_SC_ODT_MATRIX_STRUCT  OdtMatrix;

  Outputs       = &MrcData->Outputs;
  Inputs        = &MrcData->Inputs;
  Debug         = &Outputs->Debug;
  MaxDimm       = Outputs->MaxDimm;

  if (!Outputs->IsDdr5) {
    return;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      EnableOdtMatrix = 0;
      if (!Inputs->EnableOdtMatrix) {
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnableOdtMatrix, WriteCached, &EnableOdtMatrix);
        continue;
      }
      OdtMatrix.Data = 0;
      switch (Profile) {
        case 1:
          if (ChannelOut->DimmCount == 2) {
            // Assert NomWr on all non-target ranks, NomRd on both ranks of the non-target DIMM
            OdtMatrix.Bits.Write_Rank_0 = RANK_1_2_3_MASK;
            OdtMatrix.Bits.Write_Rank_1 = RANK_0_2_3_MASK;
            OdtMatrix.Bits.Write_Rank_2 = RANK_0_1_3_MASK;
            OdtMatrix.Bits.Write_Rank_3 = RANK_0_1_2_MASK;
            OdtMatrix.Bits.Read_Rank_0 = RANK_2_3_MASK;
            OdtMatrix.Bits.Read_Rank_1 = RANK_2_3_MASK;
            OdtMatrix.Bits.Read_Rank_2 = RANK_0_1_MASK;
            OdtMatrix.Bits.Read_Rank_3 = RANK_0_1_MASK;
          } else {
            // For DDR5 1DPC we want to flip from RTT_PARK to RTT_NOM_WR
            OdtMatrix.Bits.Write_Rank_0 = RANK_1_MASK;
            OdtMatrix.Bits.Write_Rank_1 = RANK_0_MASK;
          }
          break;

        case 0:
          // set odt pins to zero.
          break;

        default:
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MrcSetOdtMatrix : Unknown Odt Matrix Profile\n");
          break;
      } // switch Profile

      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_ODT_MATRIX_REG, MC1_CH0_CR_SC_ODT_MATRIX_REG, Controller, MC0_CH1_CR_SC_ODT_MATRIX_REG, Channel);
      MrcWriteCR (MrcData, Offset, OdtMatrix.Data);

      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];

      for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
        DimmOut = &ChannelOut->Dimm[Dimm];
        if (DimmOut->Status != DIMM_PRESENT) {
          continue;
        }
        //enable_odt_matrix is 1 only 2R system
        EnableOdtMatrix |= ((DimmOut->RankInDimm > 1) ? 1 : 0);
      }
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnableOdtMatrix, WriteCached | PrintValue, &EnableOdtMatrix);
    } // Channel
  } // Controller
}

/**
  Configure 1DPC performance feature.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
void
MrcMcOneDpcConfig (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput       *Outputs;
  MrcInput        *Inputs;
  MrcDebug        *Debug;
  MrcChannelOut   *ChannelOut;
  MrcDimmOut      *DimmOut;
  UINT32          Controller;
  UINT32          Channel;
  UINT32          Dimm;
  UINT32          DimmMask;
  UINT32          Offset;
  INT64           GetSetVal;
  INT64           RhEnDimm0;
  INT64           RhEnDimm1;
  INT64           SubchSdramWidth;
  MC0_CH0_CR_SC_ODT_MATRIX_STRUCT  OdtMatrix;
  MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_STRUCT RoundTripDelayReg;

  Outputs  = &MrcData->Outputs;
  Inputs   = &MrcData->Inputs;
  Debug    = &Outputs->Debug;

  if (Outputs->DdrType != MRC_DDR_TYPE_DDR5) { // This is DDR5-only feature
    return;
  }

  // Block XARB to prevent MC from sending out PDE commands triggered by the rank occupancy change
  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockXarb, WriteToCache, &GetSetVal);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockCke,  WriteToCache, &GetSetVal);
  MrcFlushRegisterCachedData (MrcData);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      if (ChannelOut->DimmCount != 1) {  // Must be 1DPC
        continue;
      }
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmOut = &ChannelOut->Dimm[Dimm];
        if ((DimmOut->Status != DIMM_PRESENT) || (DimmOut->RankInDimm != 2)) { // Must be 2R DIMM
          continue;
        }
        DimmMask = 1 << Dimm;

        if ((Inputs->ExtInputs.Ptr->DdrOneDpc & DimmMask) == 0) { // If feature is disabled for this DIMM
          continue;
        }

        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%u C%d DIMM%d: Enabling 1dpc_split_ranks\n", Controller, Channel, Dimm);
        // We have a single 2R DIMM on this channel and going to enable ddr5_1dpc feature on this channel
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhEnableDimm0, ReadCached | PrintValue, &RhEnDimm0);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhEnableDimm1, ReadCached | PrintValue, &RhEnDimm1);
        if (RhEnDimm0 || RhEnDimm1) {
          //Allow the MC to use subchannel 1 scheduling resources for the second rank. Should always be on for 1DPC in 2R mode.
          GetSetVal = 1; //Enable
          //We aren't sure which DIMM will be populated, so enable both.
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhEnableDimm0, WriteToCache | PrintValue, &GetSetVal);
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhEnableDimm1, WriteToCache | PrintValue, &GetSetVal);
        }

        // Force CKE on all ranks, to avoid CKE going down during enabling of this feature
        GetSetVal = 0x0F;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCkeOn, WriteCached | PrintValue, &GetSetVal);

        // Enable remapped rank in MC_INIT_STATE.Rank_occupancy, to avoid CKE going down during this flow
        GetSetVal = (Dimm == 0) ? 0x0B : 0x0D; // '1011' or '1101'
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRankOccupancy, WriteCached | PrintValue, &GetSetVal);

        GetSetVal = DimmMask;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccOneDpc, WriteCached | PrintValue, &GetSetVal);

        // MC_INIT_STATE.Rank_occupancy must be '1001'
        GetSetVal = 9;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRankOccupancy, WriteCached | PrintValue, &GetSetVal);

        // If Address Mirror enabled on this DIMM, must enable it on the other DIMM as well
        if (DimmOut->AddressMirrored) {
          GetSetVal = 3;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccAddrMirror, WriteToCache | PrintValue, &GetSetVal);
        }

        // In TC_RDRD, TC_RDWR, TC_WRRD and TC_WRWR configure to the *_dd field the same value as to the _dr field.
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdr, ReadFromCache | PrintValue, &GetSetVal);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdd, WriteCached   | PrintValue, &GetSetVal);

        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDWRdr, ReadFromCache | PrintValue, &GetSetVal);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDWRdd, WriteCached   | PrintValue, &GetSetVal);

        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRRDdr, ReadFromCache | PrintValue, &GetSetVal);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRRDdd, WriteCached   | PrintValue, &GetSetVal);

        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRWRdr, ReadFromCache | PrintValue, &GetSetVal);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRWRdd, WriteCached   | PrintValue, &GetSetVal);

        // In ODT_MATRIX configure the fields according to the rank occupancy (i.e. if ranks 0,1 exist set the matrix fields for ranks 0,3 instead)
        // Note that inside each field you still indicate which ODT pins are required to be set, so set bits 0,1 and not 0,3
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_ODT_MATRIX_REG, MC1_CH0_CR_SC_ODT_MATRIX_REG, Controller, MC0_CH1_CR_SC_ODT_MATRIX_REG, Channel);
        OdtMatrix.Data = MrcReadCR (MrcData, Offset);
        if (Dimm == 0) {
          OdtMatrix.Bits.Write_Rank_3 = OdtMatrix.Bits.Write_Rank_1;
        } else { // DIMM1
          OdtMatrix.Bits.Write_Rank_0 = OdtMatrix.Bits.Write_Rank_2;
        }
        OdtMatrix.Bits.Write_Rank_1 = 0;
        OdtMatrix.Bits.Write_Rank_2 = 0;
        MrcWriteCR (MrcData, Offset, OdtMatrix.Data);

        // REUT_CH_MISC_*_CTRL_0_0_0_MCHBAR need to be configured based on rank occupancy when the overrides are used.
        // Adjust CKE mask to 1001. MC is using ranks 0 and 3 when ddr5_1dpc is enabled.
        GetSetVal = 9;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCkeOverride, WriteCached | PrintValue, &GetSetVal);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCkeOn,       WriteCached | PrintValue, &GetSetVal);


        // If DIMM0: Roundtrip latency of Rank 3 should be set to the roundtrip latency of Rank 1
        // If DIMM1: Roundtrip latency of Rank 0 should be set to the roundtrip latency of Rank 2
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_REG, MC1_CH0_CR_SC_ROUNDTRIP_LATENCY_REG, Controller, MC0_CH1_CR_SC_ROUNDTRIP_LATENCY_REG, Channel);
        RoundTripDelayReg.Data = MrcReadCR (MrcData, Offset);
        if (Dimm == 0) {
          RoundTripDelayReg.Bits.Rank_3_latency = RoundTripDelayReg.Bits.Rank_1_latency;
        } else { // DIMM1
          RoundTripDelayReg.Bits.Rank_0_latency = RoundTripDelayReg.Bits.Rank_2_latency;
        }
        MrcWriteCR (MrcData, Offset, RoundTripDelayReg.Data);

        // DDR5 Keep same DIMM atributes for both "SUBCH0/1"
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch0SdramWidth, ReadCached, &SubchSdramWidth);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch1SdramWidth, WriteToCache, &SubchSdramWidth);

        // When MRH is used (DDR_MR_COMMNAD_0_0_0_MCHBAR), it should be programmed to access either rank 0 or rank 3 (and not 1 or 2).
        // See WrappedMrcRunMrh() which calls GetMrhFsmRankMaskDdr5()

      } // for Dimm
    } // for Channel
  } // for Controller
  MrcFlushRegisterCachedData (MrcData);
  MrcInvalidateRegisterCachedData (MrcData);

  // Unblock XARB
  GetSetVal = 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockXarb, WriteToCache, &GetSetVal);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockCke,  WriteToCache, &GetSetVal);
  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function configures the MC to drive the WCK signals to DDRIO allowing WCK to park differentially low when in IDLE.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] IsCbtEnter - Defines if the call is for a CBT enter (TRUE) or CBT exit (FALSE)
**/
VOID
MrcMcLpddrCbtWckMode (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              IsCbtEnter
  )
{
  INT64 GetSetVal;

  GetSetVal = (IsCbtEnter ? 1 : 0);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccWCKDiffLowInIdle, WriteCached, &GetSetVal);
}
