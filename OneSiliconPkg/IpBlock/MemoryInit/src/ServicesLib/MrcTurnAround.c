/** @file
  These functions implement the turnaround training algorithm.

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
#include "MrcTurnAround.h"
#include "MrcTurnAroundPrivate.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"
#include "Cpgc20TestCtl.h"
#include "MrcDdrIoDefines.h"
#include "MrcDdrIoUtils.h"
#include "MrcHalRegisterAccess.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcReset.h"
#include "BlueMrcIoTest.h"
#include "BlueMrcJedecApi.h"  // for MrcGetDramWriteDrift()
#include "MrcCommon.h"

/**
  This function calculates the LPDDR5 Turnaround timings based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] RxPathResults     - Holds the max/min Rx Path margins for each Channel
  @param[in] IsPreFuncTraining - TRUE if board flight delay impact from Read Leveling / Write Leveling is not known yet
  @param[OUT] TAT              - returns the turnaround timings based on DRAM Specification
**/
VOID
GetLpddr5DramTA (
  IN MrcParameters *const  MrcData,
  IN  RxPathMinMax *const  RxPathResults,
  IN  BOOLEAN              IsPreFuncTraining,
  OUT McTurnAroundTimings  *const TAT
  )
{
  MrcOutput      *Outputs;
  const MrcInput *Inputs;
  MrcTiming      *Timing;
  MrcChannelOut  *ChannelOut;
  UINT32 DeltaPiCode;
  UINT32 GB;
  UINT32 Guardband;
  MrcFrequency DdrFrequency;
  UINT32 ChFlightTime;
  UINT32 DramWr2Wr_nCK;
  UINT32 nCK;
  UINT32 tWCKDQOmax;
  UINT32 DramReadDriftPs;
  UINT32 DramReadDriftPI;
  INT32  PHClk;
  INT32  tWck2Ck_Max;
  INT32  tWck2Ck_Min;
  UINT32 tWTR_S;
  UINT32 tWTR_L;
  INT32  TA1;
  UINT32 TA2;
  UINT32 TA3;
  UINT16 tCL;
  UINT16 tCWL;
  INT8   tODTLon;
  UINT8  Controller;
  UINT8  Channel;
  UINT8  MaxChannel;
  UINT8  RdPreambleT;
  UINT8  RdPreambleLow;
  UINT8  RdPostamble;
  UINT8  WrPreambleT;
  UINT8  WrPreambleLow;
  UINT8  WrPostamble;
  UINT8  RightShift;
  UINT8  LeftShift;
  UINT8  tBLn_min;
  UINT8  tBLn_max;
  MRC_LP5_BANKORG Lp5BGOrg;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  MaxChannel = Outputs->MaxChannels;
  DdrFrequency  = Outputs->Frequency;
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, DdrFrequency);
  nCK = Outputs->IsLpddr5 ? (PHClk * 4) : PHClk;
  tWCKDQOmax = DdrFrequency >= f3200 ? MRC_LP5_tWCKDQO_HF_MAX : MRC_LP5_tWCKDQO_LF_MAX;
  tWck2Ck_Max = PHClk / 2;
  tWck2Ck_Min = -PHClk / 2;
  Guardband = 8;
  GB = (Guardband * PHClk) / PI_PER_TCK;
  DramWr2Wr_nCK = 0;
  tODTLon = MrcGetOdtlTiming (MrcData, DdrFrequency, LpWrOdt, LpOdtlOn);
  tBLn_min = Outputs->BurstLength + (DdrFrequency > f3200 ? 2 : 0);
  tBLn_max = Outputs->BurstLength + (DdrFrequency > f3200 ? 4 : 0);
  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);

  RightShift = 9;
  LeftShift =  2;

  MrcGetPrePostamble (MrcData, GetTimingRead, &RdPreambleT, &RdPreambleLow, &RdPostamble);
  MrcGetPrePostamble (MrcData, GetTimingWrite, &WrPreambleT, &WrPreambleLow, &WrPostamble);

  DramReadDriftPs = GetLpddr5ReadDrift (MrcData);
  DramReadDriftPI = DIVIDECEIL ((DramReadDriftPs * PI_PER_TCK), PHClk);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (Outputs->IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      Timing = &ChannelOut->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
      tCWL = Timing->tCWL;
      tCL = Timing->tCL;
      if (Lp5BGOrg == MrcLp5BgMode) {
        tWTR_S = Timing->tWTR_S;
        tWTR_L = Timing->tWTR_L;
      } else {
        tWTR_S = tWTR_L = Timing->tWTR;
      }
      if (IsPreFuncTraining) {
        DeltaPiCode = 16;
      } else {
        DeltaPiCode = (UINT32)(RxPathResults->MaxRcvEnPi[Controller][Channel] - RxPathResults->MinRcvEnPi[Controller][Channel]);
      }

      //-----------------------------------
      // Calculate tRDRD
      //-----------------------------------
      // tRdRd(dr,dd) = RdPreamble + ChFlightTime (Stabilize Bus) + Delta RcvEnPICode (Nominal DRAM Read Timing) +
      // DramReadDrift (Change in DRAM Read Timing due to Voltage/Temp) + Guardband
      // tRdRdsg = tCCD_L - Spec Defined
      // tRdRddg = tCCD_S - Spec Defined
      ChFlightTime = 0;
      if (Outputs->ReadPreamble == MRC_LP5_tRPRE_TOGGLE_4tWCK_2STATIC) {
        RdPreambleLow = 4; // tRDQS_PRE is MIN 2*tWCK and MAX 4*tWCK, so for TAT calculation we use 4 as the worst case
      }
      TA1 = PI_PER_TCK * (RdPreambleT + RdPreambleLow) + ChFlightTime + DeltaPiCode + DramReadDriftPI + Guardband;
      TAT->tRdRddr[Controller][Channel] = ((TA1 >> RightShift) + 1 + tBLn_min);
      TAT->tRdRddr[Controller][Channel] = (TAT->tRdRddr[Controller][Channel] << LeftShift);
      TAT->tRdRddd[Controller][Channel] = TAT->tRdRddr[Controller][Channel];
      TAT->tRdRdsg[Controller][Channel] = GetLpddrtCCDL (MrcData);
      TAT->tRdRddg[Controller][Channel] = GetLpddrtCCDS (MrcData);

      //-----------------------------------
      // Calculate tWRWR
      //-----------------------------------
      // TA1 > ( -tODTon + tWCK2CK_max + tWCKDQI_max + ChFlightTimePi + CK2CK + GB - tODTon_min)
      // TA2 > (1 + tODToff_max - tWCK2CKmin - tWCKDQImin + 0.5 UI + CK2CK + GB)
      // tWrWr(dr,dd) = MAX (TA1, TA2)
      // tWrWrsg = tCCD_L - Spec Defined
      // tWrWrdg = tCCD_S - Spec Defined
      ChFlightTime = 117; // Ps
      TA1 = tWck2Ck_Max + MRC_LP5_tWCKDQI_MF_MAX + MRC_LP5_TCK2CK + GB + ChFlightTime;
      TA1 -= MRC_LP5_tODT_ON_OFF_MIN;
      if (TA1 < 0) {
        TA1 = 0;
      }
      TA1 = -tODTLon + DIVIDECEIL (TA1, nCK);
      TA2 = MRC_LP5_tODT_ON_OFF_MAX -  tWck2Ck_Min - MRC_LP5_tWCKDQI_MIN + (PHClk / 4) + MRC_LP5_TCK2CK + GB;
      TA2 = 1 + DIVIDECEIL (TA2, nCK);
      DramWr2Wr_nCK = MAX ((UINT32)TA1, (UINT32)TA2);
      DramWr2Wr_nCK += tBLn_min;
      TAT->tWrWrdr[Controller][Channel] = (DramWr2Wr_nCK << LeftShift);
      TAT->tWrWrdd[Controller][Channel] = TAT->tWrWrdr[Controller][Channel];
      TAT->tWrWrsg[Controller][Channel] = GetLpddrtCCDL (MrcData);
      TAT->tWrWrdg[Controller][Channel] = GetLpddrtCCDS (MrcData);

      //-----------------------------------
      // Calculate tRDWR
      //-----------------------------------
      // tRdWr(sg,dg,dr,dd) < (RL - WL + tWCK2CK_max + tWCKDQO_Max + ChFlightTime + GB -tODTon - tODTon_min)
      // tRdWrsg - Use tBLn_max
      // tRdWr(dg,dr,dd) - Use tBLn_min
      TA1 = 2 * 580 + 117 - MRC_LP5_tODT_ON_OFF_MIN;
      ChFlightTime = MAX (0, TA1);
      TA1 = tWck2Ck_Max + tWCKDQOmax + ChFlightTime + MRC_LP5_TCK2CK + GB - MRC_LP5_tODT_ON_OFF_MIN;
      TA1 = DIVIDECEIL (TA1, nCK);
      TA1 += tCL - tCWL - tODTLon;
      TA2 = TA1 + tBLn_max; // Used for Sg
      TA3 = TA1 + tBLn_min; // Used for Dg/Dr
      TAT->tRdWrsg[Controller][Channel] = (TA2 << LeftShift);
      TAT->tRdWrdg[Controller][Channel] = (TA3 << LeftShift);
      TAT->tRdWrdr[Controller][Channel] = (TA3 << LeftShift);
      TAT->tRdWrdd[Controller][Channel] = TAT->tRdWrdr[Controller][Channel];

      //-----------------------------------
      // Calculate tWRRD
      //-----------------------------------
      // tWrRddr > (WL-RL) + 1 + (tODToff_max + ChFlightTime - tWCK2CK_Min - tWCKDQO_Min)/nCK
      // tWrRdsg = tCWL + tBln_max + tWTR_L - Spec Defined
      // tWrRddg = tCWL + tBln_min + tWTR_S - Spec Defined
      ChFlightTime = 0;
      TA1 = MRC_LP5_tODT_ON_OFF_MAX + ChFlightTime + GB + MRC_LP5_TCK2CK - tWck2Ck_Min - MRC_LP5_tWCKDQO_MIN;
      TA1 = DIVIDECEIL (TA1, nCK);
      TA1 += tCWL - tCL + 1 + tBLn_min;
      if (TA1 < 0) {
        TA1 = 0;
      }
      TAT->tWrRddr[Controller][Channel] = (TA1 << LeftShift);
      TAT->tWrRddd[Controller][Channel] =  TAT->tWrRddr[Controller][Channel];
      TAT->tWrRdsg[Controller][Channel] = (tCWL + tBLn_max + tWTR_L) << LeftShift;
      TAT->tWrRdsg[Controller][Channel] = MAX (TAT->tWrRddr[Controller][Channel], TAT->tWrRdsg[Controller][Channel]);
      TAT->tWrRddg[Controller][Channel] = (tCWL + tBLn_min + tWTR_S) << LeftShift;
      TAT->tWrRddg[Controller][Channel] = MAX (TAT->tWrRddr[Controller][Channel], TAT->tWrRddg[Controller][Channel]);
    }
  }
}

/**
  This function returns tCCD_L for LPDDR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetLpddrtCCDL (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result;
  if (MrcData->Outputs.Frequency > f3200) {
    Result = TCCD_L_LP5_GT3200;
  } else {
    Result = TCCD_LP5_FREQ;
  }

  return Result;
}
/**
  This function returns tCCD_S for LPDDR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetLpddrtCCDS (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result;
  if (MrcData->Outputs.Frequency > f3200) {
    Result = TCCD_S_LP5_GT3200;
  } else {
    Result = TCCD_LP5_FREQ;
  }

  return Result;
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
  MrcTiming      *Timing;
  MrcChannelOut  *ChannelOut;
  const MrcInput *Inputs;
  UINT32 Controller;
  UINT32 Channel;
  UINT32 Rank;
  UINT32 Byte;
  UINT32 DeltaPiCode;
  MrcFrequency DdrFrequency;
  UINT32 PHClk;
  UINT32 CK2CK;
  UINT32 tDQSCK;
  UINT32 tDQSCKi;
  UINT32 ChFlightTime;
  UINT32 WLTxDqs;
  UINT32 MaxWL;
  UINT32 MinWL;
  UINT32 ChFlightTimePI;
  UINT32 tCCD_L;
  UINT32 tCCD_S;
  UINT32 tCCD_L_WR;
  UINT32 tWTR_S;
  UINT32 tWTR_L;
  INT64  TxDqs;
  INT32  tDQSSMax;
  INT32  tDQSSMin;
  INT32  TA1;
  INT32  TA2;
  INT32  TA3;
  UINT16 tCL;
  UINT16 tCWL;
  UINT8  MaxChannel;
  UINT8  Guardband;
  UINT8  RdPreambleT;
  UINT8  RdPreambleLow;
  UINT8  RdPostamble;
  UINT8  WrPreambleT;
  UINT8  WrPreambleLow;
  UINT8  WrPostamble;
  UINT8  RightShift;
  UINT8  BurstLength;
  UINT8  OdtTypeIndex;
  UINT8  NtRankIndex;
  UINT8  OdtParamSelect;
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
  UINT8  NtRankSelect[MaxNtOdt];
  UINT8  OdtType[ODT_TYPE_MAX] = {WriteOdt, ReadOdt};
  UINT8  tADCMin;
  UINT8  tADCMax;

  Outputs      = &MrcData->Outputs;
  Inputs       = &MrcData->Inputs;
  MaxChannel   = Outputs->MaxChannels;
  DdrFrequency = Outputs->Frequency;
  PHClk        = UDIVIDEROUND (FREQ_TO_TCK_PS, DdrFrequency);
  Guardband    = 8;
  RightShift = 7;
  CK2CK = 0;
  BurstLength = Outputs->BurstLength;
  tCCD_S = MRC_DDR5_tCCD_ALL_FREQ;

  MrcGetPrePostamble (MrcData, GetTimingRead, &RdPreambleT, &RdPreambleLow, &RdPostamble);
  MrcGetPrePostamble (MrcData, GetTimingWrite, &WrPreambleT, &WrPreambleLow, &WrPostamble);
  Ddr5GetTDqsCK (MrcData, &tDQSCK, &tDQSCKi);
  // Convert to Pi ticks
  tDQSCK = (tDQSCK * PI_PER_TCK) / 1000;
  tDQSCKi = (tDQSCKi * PI_PER_TCK) / 1000;
  GetDdr5tDQSS (MrcData, &tDQSSMax, &tDQSSMin, WrPreambleLow);
  MrcGetDdr5Tadc (DdrFrequency, &tADCMin, &tADCMax);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      Timing = &ChannelOut->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
      tCWL = Timing->tCWL;
      tCL = Timing->tCL;
      tCCD_L = Timing->tCCD_L;
      tWTR_S = Timing->tWTR_S;
      tWTR_L = Timing->tWTR_L;
      tCCD_L_WR = Timing->tCCD_L_WR;
      WLTxDqs = 0;
      if (IsPreFuncTraining) {
        // Assume the worst case ODT Offsets
        MinOdtWrOn  = -4;
        MinOdtRdOn  = -3;
        MaxOdtWrOff = 4;
        MaxOdtRdOff = 3;
      } else {
        // Set Inital values, then search for worst case ODT offsets among ranks
        MinOdtWrOn  = 0xF;
        MinOdtRdOn  = 0xF;
        MaxOdtWrOff = -0xF;
        MaxOdtRdOff = -0xF;
      }
      // Non Target Settings are assumed off, then determined if they should be enabled below
      NTODTWr = FALSE;
      NTODTRd = FALSE;
      // Find Worst case ODT offsets among Ranks
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        // Determine if Non Target ODT is supported on any Rank/Dimm
        GetNtRankSelection (Rank, NtRankSelect);
        for (OdtTypeIndex = 0; OdtTypeIndex < 2; OdtTypeIndex++) {
          for (NtRankIndex = 0; NtRankIndex < MaxNtOdt; NtRankIndex++) {
            OdtParamSelect = MrcGetOdtParam (MrcData, Controller, Channel, Rank, OdtType[OdtTypeIndex], NtRankSelect[NtRankIndex]);
            // If any Rank / Dimm supports NTODT, then NTODT values should be used in calculating the TAT values
            if (OdtParamSelect == OptDimmOdtNomWr) {
              NTODTWr = TRUE;
            } else if (OdtParamSelect == OptDimmOdtNomRd) {
              NTODTRd = TRUE;
            }
          }
        }
        GetDdr5OdtOffsets (MrcData, Controller, Channel, Rank, NTODTWr, IsPreFuncTraining, &OdtWrOn, &OdtWrOff, &OdtRdOn, &OdtRdOff);
        MinOdtWrOn = MIN (OdtWrOn, MinOdtWrOn);
        MinOdtRdOn = MIN (OdtRdOn, MinOdtRdOn);
        MaxOdtWrOff = MAX (OdtWrOff, MaxOdtWrOff);
        MaxOdtRdOff = MAX (OdtRdOff, MaxOdtRdOff);
      }
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
      if (IsPreFuncTraining) {
        CK2CK = 16; // Safe Value for Mc Init
      } else {
        CK2CK = WLTxDqs;
      }

      // DeltaPiCode = Delta of RcvEn
      if (IsPreFuncTraining) {
        DeltaPiCode = 16; // Safe Value for Mc Init
      } else {
        DeltaPiCode = (UINT32) (RxPathResults->MaxRcvEnPi[Controller][Channel] - RxPathResults->MinRcvEnPi[Controller][Channel]);
      }

      //-----------------------------------
      // Calculate tRDRD
      //-----------------------------------
      // tRdRddr = tRdRddd = RdPreamble + ChFlightTimePI + DeltaPICode + ReadDrift + tADC_Max +  GuardBand
      // tRdRdsg = tCCD_L;
      // tRdRddg = tCCD_S
      ChFlightTimePI = 0;
      TA1 = PI_PER_TCK * (RdPreambleT + RdPreambleLow) + ChFlightTimePI + DeltaPiCode + (tDQSCKi / 2) + tADCMax + Guardband;
      TAT->tRdRddr[Controller][Channel] = (TA1 >> RightShift) + 1;
      TAT->tRdRddr[Controller][Channel] += BurstLength;
      TAT->tRdRddd[Controller][Channel] = TAT->tRdRddr[Controller][Channel];
      TAT->tRdRdsg[Controller][Channel] = tCCD_L;
      TAT->tRdRddg[Controller][Channel] = tCCD_S;

      //-----------------------------------
      // Calculate tWRWR
      //-----------------------------------
      // tWrWrdr = tWrWrdd = ODT_Wr_OFF - ODT_Wr_On
      // tWrWrsg = tCCD_L_WR;
      // tWrWrdg = tCCD_S
      TA1 = PI_PER_TCK * (MaxOdtWrOff - MinOdtWrOn);
      TAT->tWrWrdr[Controller][Channel] = (TA1 >> RightShift) + 1;
      TAT->tWrWrdr[Controller][Channel] += BurstLength;
      TAT->tWrWrdd[Controller][Channel] = TAT->tWrWrdr[Controller][Channel];
      TAT->tWrWrsg[Controller][Channel] = tCCD_L_WR;
      TAT->tWrWrdg[Controller][Channel] = tCCD_S;

      //-----------------------------------
      // Calculate tRDWR
      //-----------------------------------
      // TA1 > (RL-WL+WrPreamble) + tRPST + tADCMax + ChFlightTimePI + CK2CK - tDQSoffsetmin - tDQSSmin + GB
      // TA2 = (RL-WL-ODT_WR_On) + (NTODT_Rd ? NTODT_Rd_Off : 0) + (CK2CK + ChFlightTimePI + Guardband)
      // tRdWr(sg,dg,dr,dd) = MAX (TA1, TA2)
      ChFlightTime = 2 * 960;
      ChFlightTimePI = (ChFlightTime * PI_PER_TCK) / PHClk;
      TA1 = ChFlightTimePI - tADCMin;
      ChFlightTimePI = MAX (0, TA1);
      TA1 = (PI_PER_TCK * (tCL - tCWL + WrPreambleLow + WrPreambleT)) + tADCMax + ChFlightTimePI + CK2CK;
      TA1 += Guardband - MRC_DDR5_tDQSOFFSET_MIN - tDQSSMin;
      TA2 = (PI_PER_TCK * (tCL - tCWL - MinOdtWrOn)) + 128 * (NTODTRd ? MaxOdtRdOff : 0) + CK2CK + ChFlightTimePI + Guardband;
      TA3 = (MAX ((UINT32)TA1, (UINT32)TA2) >> RightShift) + 1 + BurstLength;
      TAT->tRdWrdr[Controller][Channel] = TA3;
      TAT->tRdWrdd[Controller][Channel] = TA3;
      TAT->tRdWrsg[Controller][Channel] = TA3;
      TAT->tRdWrdg[Controller][Channel] = TA3;

      //-----------------------------------
      // Calculate tWRRD
      //-----------------------------------
      // TA1 = (WL-RL+RdPreamble+1) + (tDQSoffsetmax + tDQSSmax + tWPST + ChFlightTime + CK2CK -tADCmin + GB))
      //   tWPST = 0 since its included in BL
      // TA2 = WL - RL + ODT_WR_Off + (NTODT_Rd ? NTODT_Rd_On : - tDQSCKmin - tDQSQmin + tADCmax) + CK2CK + ChFlightTime + Guardband)
      // tWrRdsg = tCWL + tWTR_L + BurstLength;
      // tWrRddg = tCWL + tWTR_S + BurstLength
      // DQS channel Flight time
      ChFlightTime = 0;
      TA1 = (PI_PER_TCK * (tCWL - tCL + RdPreambleLow + RdPreambleT + 1)) + MRC_DDR5_tDQSOFFSET_MAX + tDQSSMax + WrPostamble + ChFlightTime + CK2CK - tADCMin + Guardband;
      // DQ channel flight time
      ChFlightTime = 312; // pS
      ChFlightTimePI = (ChFlightTime * PI_PER_TCK) / PHClk;
      TA2 = (PI_PER_TCK * MaxOdtWrOff) + (NTODTRd ? (128*MinOdtRdOn) : (tADCMax - MRC_DDR5_tDQSQ_MIN - tDQSCK)) + CK2CK + ChFlightTimePI + Guardband;
      TA2 += PI_PER_TCK * (tCWL - tCL);
      // Avoid negative value in tWrRddr calculation
      TA1 = MAX (0, TA1);
      TA2 = MAX (0, TA2);
      TAT->tWrRddr[Controller][Channel] = (MAX ((UINT32)TA1, (UINT32)TA2) >> RightShift) + 1;
      TAT->tWrRddr[Controller][Channel] += BurstLength;
      TAT->tWrRddd[Controller][Channel] = TAT->tWrRddr[Controller][Channel];
      TAT->tWrRdsg[Controller][Channel] = tCWL + tWTR_L + BurstLength;
      TAT->tWrRdsg[Controller][Channel] = MAX (TAT->tWrRddr[Controller][Channel], TAT->tWrRdsg[Controller][Channel]);
      TAT->tWrRddg[Controller][Channel] = tCWL + tWTR_S + BurstLength;
      TAT->tWrRddg[Controller][Channel] = MAX (TAT->tWrRddr[Controller][Channel], TAT->tWrRddg[Controller][Channel]);
    }
  }
}

/**
  This function calculates the Wr2Wr Tunraround timing based on training results

  @param[in]  MrcData - Include all MRC global data.
  @param[in]  TxPathMinMax - Holds the max/min Tx Path margins for each Channel
  @param[OUT] PHYWr2Wr_PI - returns the Wr2Wr turnaround timings
**/
VOID
GetPhyWr2WrTA (
  IN  MrcParameters *const MrcData,
  IN  TxPathMinMax  *const TxPathResults,
  OUT UINT32               PHYWr2Wr_PI[MAX_CONTROLLER][MAX_CHANNEL]
  )
{
  MrcOutput *Outputs;
  UINT32  MaxPiCode;
  UINT32  MinPiCode;
  UINT32  DeltaPiCode;
  UINT32  PHClk;
  UINT32  DramWriteDriftPs;
  UINT32  DramWriteDriftPI;
  UINT32  PIXover;
  INT32   TA1;
  UINT8   Controller;
  UINT8   Channel;
  UINT8   MaxChannel;
  UINT8   Guardband;
  UINT8   WrPreambleT;
  UINT8   WrPreambleLow;
  UINT8   WrPostamble;
  UINT8   RightShift;
  UINT8   LeftShift;
  UINT8   BurstLength;
  UINT8   tBLn_min;
  BOOLEAN IsDdr5;
  BOOLEAN IsGear4;

  Outputs = &MrcData->Outputs;
  IsGear4 = Outputs->GearMode ? 1 : 0;
  IsDdr5 = Outputs->IsDdr5;
  MaxChannel = Outputs->MaxChannels;
  Guardband = 8;
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);
  DramWriteDriftPs = MrcGetDramWriteDrift (MrcData);
  DramWriteDriftPI = DIVIDECEIL ((DramWriteDriftPs * PI_PER_TCK), PHClk);
  MinPiCode = MRC_UINT32_MAX;
  MaxPiCode = 0;
  RightShift = 7 + (IsDdr5 ? 0 : 2);
  LeftShift = IsDdr5 ? 0 : 2;
  BurstLength = Outputs->BurstLength;
  tBLn_min = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 2 : 0);

  MrcGetPrePostamble (MrcData, GetTimingWrite, &WrPreambleT, &WrPreambleLow, &WrPostamble);
  // Wr2Wr Rank to Rank
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (Outputs->IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      MinPiCode = MIN (MinPiCode, ((TxPathResults->MinTxDq[Controller][Channel] + TxPathResults->MinTxDqBit[Controller][Channel])));
      MaxPiCode = MAX (MaxPiCode, ((UINT32) (TxPathResults->MaxTxDq[Controller][Channel] + TxPathResults->MaxTxDqBit[Controller][Channel])));
      if (IsDdr5) {
        // PHYWr2Wr_PI = 128*( (Gear4?3:2) + 0.75 + PIXover + 1  +  WrPreambleT + WrPreambleLow) + DeltaPICode + GuardBand
        //  DeltaPICode = DeltaPICode = MAXRANK(TXCONTROL0RANK{PopRanks}.TxDqsDelayPi) - MINRANK(TXCONTROL0RANK{PopRanks}.TxDqsDelayPi)
        //  PIXover = Gear4 ? MAXRANK( (TXCONTROL0RANK{PopRanks}.TxDqsDelayPi>>7) & 0x1 ) : 0
        DeltaPiCode = TxPathResults->DeltaPiCode[Controller][Channel];
        PIXover = (IsGear4 ? (TxPathResults->MaxTxDqs[Controller][Channel] >> 7 & 0x1) : 0) + WrPreambleT + WrPostamble;
      } else {
        // PHYWr2Wr_PI = 128*( (Gear4?3:2) + 0.75 + PIXover + 1) + DeltaPICode + DramWriteDriftPI + GuardBand
        //  MaxPICode = MAXRANK(TXCONTROL0RANK{PopRanks}.TxDqDelay) + MAXLANE( DDRDATADQRank{PopRanks}Lane{7:0}.TxDQPerBitDeskew ))
        //  MinPICode = MINRANK(TXCONTROL0RANK{PopRanks}.TxDqDelay) + MINLANE( DDRDATADQRank{PopRanks}Lane{7:0}.TxDQPerBitDeskew ))
        //  DeltaPICode = MaxPICode - MinPICode
        //  PIXover = MAXRANK((TXCONTROL0RANK{PopRanks}.TxDqsDelayPi & BitMask) + MAXLANE(DDRDATADQRank{PopRanks}Lane{7:0}.TxDQPerBitDeskew)) >> 7
        DeltaPiCode = MaxPiCode - MinPiCode;
        PIXover = ((TxPathResults->MaxTxDqs[Controller][Channel] & (IsGear4 ? 0xFF : 0x7F)) + (UINT32) TxPathResults->MaxTxDqBit[Controller][Channel]) >> 7;
      }
      TA1 = (IsGear4 ? 3 : 2) + 1 + PIXover;
      TA1 = (TA1 * 100) + 75;
      TA1 = (TA1 * PI_PER_TCK) / 100;
      TA1 += (DeltaPiCode + Guardband + (IsDdr5 ? 0 : DramWriteDriftPI));
      PHYWr2Wr_PI[Controller][Channel] = ((TA1 >> RightShift) + 1);
      PHYWr2Wr_PI[Controller][Channel] = (PHYWr2Wr_PI[Controller][Channel] + (IsDdr5 ? BurstLength : tBLn_min)) << LeftShift;
    }
  }
}

/**
  This function calculates the Rd2Rd Tunraround timing based on training results
  Phy constraint that tells us what the min value acceptable by phy

  @param[in] MrcData - Include all MRC global data.
  @param[in] RxPathResults - Holds the max/min Rx Path margins for each Channel
  @param[OUT] PHYRd2Rd_PI  - returns the Rd2Rd turnaround timings
**/
VOID
GetPhyRd2RdTA (
  IN  MrcParameters *const MrcData,
  IN  RxPathMinMax  *const RxPathResults,
  OUT UINT32              PHYRd2Rd_PI[MAX_CONTROLLER][MAX_CHANNEL]
  )
{
  MrcOutput  *Outputs;
  UINT32  DeltaPiCode;
  UINT32  DramReadDriftPs;
  UINT32  DramReadDriftPI;
  UINT32  PHClk;
  UINT32  tDqsCki;
  INT32   TA1;
  UINT8   Controller;
  UINT8   Channel;
  UINT8   MaxChannel;
  UINT8   Guardband;
  UINT8   RdPreambleT;
  UINT8   RdPreambleLow;
  UINT8   RdPostamble;
  UINT8   RightShift;
  UINT8   LeftShift;
  BOOLEAN IsDdr5;

  Outputs = &MrcData->Outputs;
  IsDdr5 = Outputs->IsDdr5;
  MaxChannel = Outputs->MaxChannels;
  Guardband = 8;
  RightShift = 7 + (IsDdr5 ? 0 : 2);
  LeftShift = IsDdr5 ? 0 : 2;
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);

  MrcGetPrePostamble (MrcData, GetTimingRead, &RdPreambleT, &RdPreambleLow, &RdPostamble);

  DramReadDriftPs = GetLpddr5ReadDrift (MrcData);
  DramReadDriftPI = DIVIDECEIL ((DramReadDriftPs * PI_PER_TCK), PHClk);

  // Rd2Rd Rank to Rank
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (Outputs->IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      DeltaPiCode = RxPathResults->DeltaPiCode[Controller][Channel];
      if (IsDdr5) {
        Ddr5GetTDqsCK (MrcData, NULL, &tDqsCki);
        // Convert to Pi ticks
        tDqsCki = (tDqsCki * PI_PER_TCK) / 1000;
        DeltaPiCode += (tDqsCki/2);
      } else {
        DeltaPiCode += DramReadDriftPI;
      }
      // PHYRd2Rd_PI = DeltaPICode + 128* (3.5 + RdPreambleLow+RdPreambleT) + GuardBand
      TA1 = PI_PER_TCK * (3500 + (RdPreambleT * 1000) + (RdPreambleLow * 1000));
      TA1 /= 1000;
      TA1 += DeltaPiCode + Guardband;
      PHYRd2Rd_PI[Controller][Channel] = ((TA1 >> RightShift) + 1);
      PHYRd2Rd_PI[Controller][Channel] = (PHYRd2Rd_PI[Controller][Channel] + Outputs->BurstLength) << LeftShift;
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
            *TatPtr += MC_INIT_SAFE_OFFSET_DR;
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
