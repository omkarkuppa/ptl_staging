/** @file
  This module configures the memory controller timing parameters.

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
#include "MrcTimingConfiguration.h"
#include "MrcTimingConfigurationPrivate.h"
#include "MrcMemoryApi.h"
#include "MrcLpddr5.h"
#include "MrcDdr5.h"
#include "MrcMcApi.h"
#include "MrcTurnAround.h"
#include "BlueMrcJedecApi.h"  //for MrcGetTppd()
#include "MrcMemoryScrub.h"

/**
  This function sets up the following MC Timing
  registers: TC_PRE, TC_ACT, TC_ODT

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in]      Controller  - 0-based index to select the Controller.
  @param[in]      Channel     - 0-based index to select the Channel.

  @retval Nothing.

**/
static
void
SetTcPreActOdt (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel
  )
{
  const MrcInput    *Inputs;
  MrcOutput         *Outputs;
  MrcChannelOut     *ChannelOut;
  MrcTiming         *Timing;
  INT64             tWRPRE;
  INT64             tRDPRE;
  INT64             tREFSBRD;
  INT64             tRP;
  INT64             tRCDw;
  INT64             tCWL;
  INT64             tRPab;
  INT64             tRAS;
  INT64             nWRDiff;
  INT64             tFAW;
  INT64             tRRDsg;
  INT64             tRRDdg;
  INT64             DeratingExt;
  INT64             tCL;
  INT64             tPPD;
  INT64             AddTcwl;
  INT64             DecTcwl;
  INT64             Lp5BankMode;
  INT64             tccdByteCasDelta;
  INT64             tRDA2ACT;
  INT64             tWRA2ACT;
  UINT32            tRRD;
  UINT32            TckPs;
  BOOLEAN           IsLpddr;
  BOOLEAN           IsDdr5;
  BOOLEAN           IsLpddr5;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];
  Timing      = &ChannelOut->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  IsLpddr     = Outputs->IsLpddr;
  IsDdr5      = Outputs->IsDdr5;
  IsLpddr5    = Outputs->IsLpddr5;
  TckPs       = Outputs->tCKps;
  Lp5BankMode = 0;

  if (IsLpddr5) {
    Lp5BankMode = (INT64) MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  }

  // tRP is equal to tRCD
  tRP = Timing->tRCDtRP;
  if (IsLpddr5) {
    tRP = ((INT32) tRP) * 4;
  }

  tRCDw = MrcGetTrcdw (MrcData, Timing->tRCDtRP);

  if (IsLpddr) {
    tRPab = (INT32) (Timing->tRPab);
    if (IsLpddr5) {
      tRPab = ((INT32) tRPab) * 4;
    }
  } else {
    tRPab = tRP;
  }

  tRAS = Timing->tRAS;
  if (IsLpddr5) {
    tRAS = ((INT32) tRAS) * 4;
  }

  nWRDiff = GetnWRDiff (MrcData);

  tRDPRE = Timing->tRTP + MrcGetBurstLengthNMin (MrcData);

  // Store tRDPRE in units of tCK consitent with JEDEC Timings
   MrcData->Save.Data.SaGvOutputs.SaGvTiming[Outputs->SaGvPoint].IpTiming.tRDPRE = (UINT16) tRDPRE;

  if (IsLpddr5) {
    tRDPRE = ((INT32) tRDPRE) * 4;
  }

  tPPD = MrcGetTppd (MrcData);

  tFAW = Timing->tFAW;
  if (IsLpddr5) {
    tFAW = ((INT32) tFAW) * 4;
  }

  // Setup tRRD_sg and tRRD_dg based on DdrType.
  if (IsDdr5) {
    tRRDsg = Timing->tRRD_L;
    tRRDdg = Timing->tRRD_S;
  } else {
    // in LP5, as there is no concept of tRRD_sg or dg, so we set them to the same value
    tRRD = Timing->tRRD;
    if (IsLpddr5) {
      tRRD *= 4;
    }
    tRRDsg = tRRD;
    tRRDdg = tRRD;
  }

  DeratingExt = GetDeratingExt (MrcData);

  tCL = Timing->tCL;
  if (IsLpddr5) {
    tCL = ((INT32) tCL) * 4;
  }

  tCWL  = Timing->tCWL;
  if (IsLpddr5) {
    tCWL = ((INT32) tCWL) * 4;
  }

  tWRPRE = MrcGetTwrpre (MrcData, Timing->tCWL, Timing->tWR);

  tREFSBRD = 0;
  if (IsDdr5) {
    tREFSBRD = DIVIDECEIL (MRC_DDR5_tREFSBRD_PS, TckPs);
  } else if (IsLpddr5) {
    tREFSBRD = MrcGetTbpr2act (MrcData);
  }

  if (tREFSBRD != 0) {
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctREFSbRd, WriteToCache | PrintValue, &tREFSBRD);
  }

  tccdByteCasDelta = MrcGetTccdByteCasDelta (MrcData, Timing);


  AddTcwl = 0;

  if (IsLpddr5) {
    DecTcwl = 7;
  } else { // DDR5
    // 7 for (2N && Gear2), otherwise 5
    DecTcwl = ((MrcGetNMode (MrcData) == CA_2_NMODE) && (Outputs->GearMode == 0)) ? 7 : 5;
  }

  tRDA2ACT = MAX (tRAS - tRP, tRDPRE) + tRP;
  tWRA2ACT = MAX (tRAS - tRP, tWRPRE) + tRP;

  if (IsLpddr5) {
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccLp5BankMode,    WriteToCache | PrintValue, &Lp5BankMode);
  }
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRCD,            WriteToCache | PrintValue, &tRP);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRCDw,           WriteToCache | PrintValue, &tRCDw);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDA2ACT,        WriteToCache | PrintValue, &tRDA2ACT);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRA2ACT,        WriteToCache | PrintValue, &tWRA2ACT);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRP,             WriteToCache | PrintValue, &tRP);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRPab,           WriteToCache | PrintValue, &tRPab);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRAS,            WriteToCache | PrintValue, &tRAS);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctnWRDiff,        WriteToCache | PrintValue, &nWRDiff);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDPRE,          WriteToCache | PrintValue, &tRDPRE);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRPRE,          WriteToCache | PrintValue, &tWRPRE);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctFAW,            WriteToCache | PrintValue, &tFAW);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRRDsg,          WriteToCache | PrintValue, &tRRDsg);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRRDdg,          WriteToCache | PrintValue, &tRRDdg);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctLpDeratingExt,  WriteToCache | PrintValue, &DeratingExt);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCL,             WriteToCache | PrintValue, &tCL);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCWL,            WriteToCache | PrintValue, &tCWL);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctPPD,            WriteToCache | PrintValue, &tPPD);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCWLDec,         WriteToCache | PrintValue, &DecTcwl);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCWLAdd,         WriteToCache | PrintValue, &AddTcwl);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmtccdCasDelta,      WriteToCache | PrintValue, &tccdByteCasDelta);
  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function returns nWR_diff = nWR - tWR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetnWRDiff (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result = 0;
  BOOLEAN IsLpddr5 = MrcData->Outputs.IsLpddr5;
  UINT32 tWrConstant = (MrcData->Outputs.LpByteMode ? MRC_LPDDR5_x8_tWR : MRC_LPDDR5_x16_tWR);
  UINT32 nWR;
  UINT32 tWR;

  if (IsLpddr5) {
    // tWR = max (36ns, 3nCK) for x8 devices
    // tWR = max (34ns, 3nCK) for x16 devices
    tWR = DIVIDECEIL (tWrConstant, MrcData->Outputs.MemoryClock);
    tWR = MAX (tWR, MRC_LPDDR5_tWR_MIN);

    nWR = GetnWR (MrcData);
    Result = (nWR - tWR) * WCK_TO_CK_RATIO;
  }

  return Result;
}

/**
  This function returns nWR latency.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR parameter.
**/
UINT32
GetnWR (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result = 0;
  INT8 Index;
  MrcFrequency Frequency = MrcData->Outputs.Frequency;
  BOOLEAN IsLpByteMode = MrcData->Outputs.LpByteMode;

  if (MrcData->Outputs.IsLpddr5) {
    for (Index = JEDEC_LPDDR5_nWR_LATENCY_ARRAY_SIZE - 1; 0 <= Index; Index--) {
      if (Frequency <= Lpddr5nWrLatency[Index].Frequency) {
        Result = IsLpByteMode ? Lpddr5nWrLatency[Index].nWRx8 : Lpddr5nWrLatency[Index].nWRx16;
      } else {
        break;
      }
    }
  }
  return Result;
}

/**
  This function returns derating_ext value which is added to tRAS, tRRD, tRP and tRCD when LPDDR5 is hot.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns derating_ext parameter.
**/
UINT64
GetDeratingExt (
  IN MrcParameters *const MrcData
  )
{
  UINT64 Result = 0;
  UINT32 TckPs = MrcData->Outputs.tCKps;

  if (MrcData->Outputs.IsLpddr5) {
    Result = DIVIDECEIL (DERATE_TIMING_PS, TckPs) * WCK_TO_CK_RATIO;
    Result = MIN (Result, MAX_DERATE_VALUE);
  }

  return Result;
}

/**
  This function sets up the TC_PWRDN register.

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in]      Controller  - 0-based index to select the Controller.
  @param[in]      Channel     - 0-based index to select the Channel.

  @retval Nothing.
**/
static
void
SetTcPwrdwn (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel
  )
{
  const MrcInput  *Inputs;
  MrcOutput       *Outputs;
  MrcChannelOut   *ChannelOut;
  MrcTiming       *Timing;
  MrcDdrType      DdrType;
  MrcFrequency    DdrFreq;
  INT64           tCKE;
  INT64           tXP;
  INT64           tPRPDEN;
  INT64           tRDPDEN;
  INT64           tWRPDEN;
  INT64           tCA2CS;
  INT64           tCSL;
  INT64           tCSH;
  UINT32          tCK;
  UINT32          Data32;
  INT64           tOSCO;
  INT64           tMRR;
  INT64           tPREMRR;
  INT64           tVRCGEnable;
  INT64           tVRCGDisable;
  BOOLEAN         IsLpddr5;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];
  Timing      = &ChannelOut->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  DdrType     = Outputs->DdrType;
  tCK         = Timing->tCK;
  IsLpddr5    = Outputs->IsLpddr5;
  DdrFreq     = Outputs->Frequency;

  Data32 = MrcGetTcke (DdrType, tCK);
  if (IsLpddr5) {
    // Scale to WCK for Lpddr5
    Data32 *= 4;
  }
  tCKE = Data32;

  Data32 = MrcGetTxp (DdrType, tCK);
  if (IsLpddr5) {
    // More conservative approach is used, optimization is done in SAGV Final when CS ODT is enabled
    // tXP = MAX(tPDXCSODTON, 3nCK) = MAX(20ns, 3nck)
    Data32 = MAX (DIVIDECEIL (MRC_LPDDR5_tPDXCSODTON, tCK), MRC_LP5_tXP_MIN_NCK);
    Data32 *= 4;
  }
  tXP = Data32;

  if (DdrType == MRC_DDR_TYPE_DDR5) {
    tCSL = (Timing->NMode == 2) ? MRC_DDR5_tCSL_SREXIT_2N : MRC_DDR5_tCSL_SREXIT_1N;

    tCA2CS = MrcGetTcacsh (MrcData);
  } else {
    tCSL = MrcGetTcsl (MrcData);

    tCA2CS = MAX (MrcGetTcacsh (MrcData), MrcGetTckckeh (MrcData));
  }

  tCSH = MrcGetTcsh (MrcData);

  tPRPDEN = MrcGetTprpden (MrcData, DdrFreq);
  tRDPDEN = MrcGetTrdpden (MrcData, Timing->tCL);
  tWRPDEN = MrcGetTwrpden (MrcData, Timing->tCWL, Timing->tWR);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCKE,    WriteToCache | PrintValue, &tCKE);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctXP,     WriteToCache | PrintValue, &tXP);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctPRPDEN, WriteToCache | PrintValue, &tPRPDEN);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDPDEN, WriteToCache | PrintValue, &tRDPDEN);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRPDEN, WriteToCache | PrintValue, &tWRPDEN);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCSH,    WriteToCache | PrintValue, &tCSH);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCSL,    WriteToCache | PrintValue, &tCSL);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCA2CS,  WriteToCache | PrintValue, &tCA2CS);

  tOSCO = MrcGetTosco (MrcData);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttOSCO, WriteToCache | PrintValue, &tOSCO);
  tMRR = MrcGetTmrr (MrcData, Timing->tCL);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttMRR,  WriteToCache | PrintValue, &tMRR);
  // LP5 only
  if (IsLpddr5) {
    tPREMRR = MAX (tMRR, MrcGetWrToMrr (MrcData, Timing->tCWL, Timing->tWTR, Timing->tWTR_L));
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttPREMRR, WriteToCache | PrintValue, &tPREMRR);
  }

  // Setup tVRCG_ENABLE and tVRCG_DISABLE
  tVRCGEnable  = MrcGetTvrcgEnable (MrcData);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctVrcgEnable,  WriteToCache | PrintValue, &tVRCGEnable);
  tVRCGDisable = MrcGetTvrcgDisable (MrcData);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctVrcgDisable, WriteToCache | PrintValue, &tVRCGDisable);

  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function configures the memory controller timings.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
void
MrcTimingConfiguration (
  IN MrcParameters *const MrcData
  )
{
  UINT32      Controller;
  UINT32      Channel;
  BOOLEAN     IsLpddr;
  INT64       GetSetVal;

  IsLpddr = MrcData->Outputs.IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      if (MrcData->Outputs.NeedManualParamDivide) {
        GetSetVal = 1;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccNoGear4ParamDivide, WriteCached | PrintValue, &GetSetVal);
      }
      // Setup TC_PRE, TC_ACT and TC_ODT registers
      SetTcPreActOdt (MrcData, Controller, Channel);
      // Setup TC_PWRDWN register
      SetTcPwrdwn (MrcData, Controller, Channel);
    }
  }
  // Setup turnaround timings
  SetTurnAroundTiming (MrcData, TRUE);

  // Check RawCard Types and adjust Read ODT if needed
  //RdOdtStretch (MrcData);
}

#if 0
/**
  This function sets up the Read ODTD values based on RawCard types and adjusts the tDRRD2RD, tDDRD2RD, tDRRD2WR and tDDRD2WR

  @param[in, out] MrcData - Include all MRC global data.
**/
void
RdOdtStretch (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcDebug              *Debug;
  const MrcInput        *Inputs;
  MrcOutput             *Outputs;
  MrcChannelOut         *ChannelOut;
  UINT32                Controller;
  UINT32                Channel;
  UINT8                 RankMaskCh;
  INT8                  OdtStretch;
#if SUPPORT_SODIMM == SUPPORT
  MrcDimmOut            *DimmOut;
  BOOLEAN               SoDimm;
  UINT8                 Dimm;
  UINT8                 DimmRawCardType[MAX_DIMMS_IN_CHANNEL];
#endif //SUPPORT_SODIMM == SUPPORT

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  // Only DDR5 supports 2DPC. Return out for other technologies and Ulx/Ult.
  if (Outputs->IsLpddr5 || (Inputs->IsMcUlxUlt)) {
    return;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || (ChannelOut->DimmCount != 2)) {
        // Skip any channels that do not have 2 DIMMs populated
        continue;
      }

      RankMaskCh = ChannelOut->ValidRankBitMask;
      // Start with the most aggressive setting
      OdtStretch = 6;

    #if SUPPORT_SODIMM == SUPPORT
      SoDimm = FALSE;
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmRawCardType[Dimm] = 0;
        DimmOut = &ChannelOut->Dimm[Dimm];
      if ((DimmOut->ModuleType == SoDimmMemoryPackage) && (SoDimm == FALSE)) {
          SoDimm = TRUE;
        }
        if (SoDimm) {
          DimmRawCardType[Dimm] = DimmOut->ReferenceRawCard;
        }
      }

      if (SoDimm) {
        if ((DimmRawCardType[0] == rcF || DimmRawCardType[1] == rcF)
            && (DimmRawCardType[0] != DimmRawCardType[1])) {
          OdtStretch = 7;
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,"Rd Odt Stretch F\n");
        }
      }
    #endif //SUPPORT_SODIMM == SUPPORT
      // Program Rdodtd value
      UpdateTAParamOffset (MrcData, Controller, Channel, 0, rdodtd, OdtStretch, 1, 1, RankMaskCh);
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Selected RdOdtD Offset for channel %d is = %d\n", Channel, OdtStretch);

    #if SUPPORT_SODIMM == SUPPORT
      if (OdtStretch > 6) {
        MrcTatStretch (MrcData, Controller, Channel, rdodtd, OdtStretch - 6, TRUE);
      } else {
        // @todo Is this needed for ICL?
        // Add 1 for ODT read duration for 2DPC
        UpdateTAParamOffset (MrcData, Controller, Channel, 0, rdodtd, 7, 1, 1, RankMaskCh);
      }
    #endif //SUPPORT_SODIMM == SUPPORT
    }
  }
}
#endif

/**
  This function adjusts the relevant TAT when changing Read or Write ODT duration.
  TAT list rdOdt = { drrd2rd, ddrd2rd, drrd2wr, ddrd2wr, srrd2wr_sg, srrd2wr_dg };
  TAT list wrOdt = { drwr2wr, ddwr2wr, drwr2rd, ddwr2rd, srwr2rd_sg, srwr2rd_dg };

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in]      Controller  - Controller select.
  @param[in]      Channel     - Channel select.
  @param[in]      OdtType     - rdodtd or wrodtd.
  @param[in]      Offset      - signed offset.
  @param[in]      UpdateHost  - Switch to update the host struct with the new value.

  @retval Nothing.
**/
void
MrcTatStretch (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               Controller,
  IN     UINT32               Channel,
  IN     UINT8                OdtType,
  IN     INT8                 Offset,
  IN     BOOLEAN              UpdateHost
  )
{
  INT64   GetSetVal;
  UINT32  GsmMode;

  GsmMode = ForceWriteOffsetUncached;
  if (UpdateHost) {
    GsmMode |= WriteCached;
  }

#if 0 // This code is used for register cache debug only.
#ifdef MRC_DEBUG_PRINT
  GsmMode |= PrintValue;
#endif
#endif

  GetSetVal = Offset;
  //@todo: Optimize using arrays and only one GetSet Call.
  if (OdtType == rdodtd) {
    // Program Different Rank RD 2 RD value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdr, GsmMode, &GetSetVal);

    // Program Different DIMM RD 2 RD value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdd, GsmMode, &GetSetVal);

    // Program Different Rank RD 2 WR value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDWRdr, GsmMode, &GetSetVal);

    // Program Different DIMM RD 2 WR value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDWRdd, GsmMode, &GetSetVal);

    // Program Same Rank RD 2 WR sg value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDWRsg, GsmMode, &GetSetVal);

    // Program Same Rank RD 2 WR dg value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDWRdg, GsmMode, &GetSetVal);
  } else if (OdtType == wrodtd) {
    // Program Different Rank WR 2 WR value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRWRdr, GsmMode, &GetSetVal);

    // Program Different DIMM WR 2 WR value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRWRdd, GsmMode, &GetSetVal);

    // Program Different Rank WR 2 RD value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRRDdr, GsmMode, &GetSetVal);

    // Program Different DIMM WR 2 RD value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRRDdd, GsmMode, &GetSetVal);

    // Program Same Rank WR 2 RD sg value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRRDsg, GsmMode, &GetSetVal);

    // Program Same Rank WR 2 RD dg value
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRRDdg, GsmMode, &GetSetVal);
  }
}
