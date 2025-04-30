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
#include "MrcMcSiSpecific.h"

/**
  This function sets up the following MC Timing
  registers: TC_PRE, TC_ACT, TC_ODT

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in]      Controller  - 0-based index to select the Controller.
  @param[in]      Channel     - 0-based index to select the Channel.

  @retval Nothing.

**/
void
SetTcPreActOdt (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel
  )
{
  const MrcInput    *Inputs;
  MrcOutput         *Outputs;
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
  Timing      = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  IsLpddr     = Outputs->IsLpddr;
  IsDdr5      = Outputs->IsDdr5;
  IsLpddr5    = Outputs->IsLpddr5;
  TckPs       = Outputs->tCKps;
  Lp5BankMode = 0;

  if (IsLpddr5) {
    Lp5BankMode = (INT64) MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  }

  tRP      = Timing->tRCDtRP; // tRP is equal to tRCD
  tRCDw    = MrcGetTrcdw (MrcData, Timing->tRCDtRP);
  tRAS     = Timing->tRAS;
  tRPab    = IsLpddr ? (INT32) (Timing->tRPab) : tRP;
  // LP5: SPD Processing aligns Timing->tWR to nWR according to JEDEC MR2 table; hence the nWR_diff should be zero
  nWRDiff  = 0;
  tRDPRE   = Timing->tRTP;
  tPPD     = MrcGetTppd (MrcData);
  tFAW     = Timing->tFAW;
  tCL      = Timing->tCL;
  tCWL     = Timing->tCWL;
  tWRPRE   = MrcGetTwrpre (MrcData, Timing->tCWL, Timing->tWR);
  tREFSBRD = Outputs->IsDdr5 ? DIVIDECEIL (MRC_DDR5_tREFSBRD_PS, TckPs) : MrcGetTbpr2act (MrcData);
  tccdByteCasDelta = MrcGetTccdByteCasDelta (MrcData, Timing);
  DeratingExt = GetDeratingExt (MrcData);

  if (IsLpddr5) {
    tRDPRE += MrcGetBurstLengthNMin (MrcData);
  }
  // Store tRDPRE in units of tCK consitent with JEDEC Timings
  MrcData->Save.Data.SaGvOutputs.SaGvTiming[Outputs->SaGvPoint].IpTiming.tRDPRE = (UINT16) tRDPRE;

  if (IsLpddr5) {
    // Scale to WCK for Lpddr5
    tRP      = ((INT32) tRP)      * WCK_TO_CK_RATIO;
    tRCDw    = ((INT32) tRCDw)    * WCK_TO_CK_RATIO;
    tRAS     = ((INT32) tRAS)     * WCK_TO_CK_RATIO;
    tRPab    = ((INT32) tRPab)    * WCK_TO_CK_RATIO;
    tRDPRE   = ((INT32) tRDPRE)   * WCK_TO_CK_RATIO;
    tPPD     = ((INT32) tPPD)     * WCK_TO_CK_RATIO;
    tFAW     = ((INT32) tFAW)     * WCK_TO_CK_RATIO;
    tCL      = ((INT32) tCL)      * WCK_TO_CK_RATIO;
    tCWL     = ((INT32) tCWL)     * WCK_TO_CK_RATIO;
    tWRPRE   = ((INT32) tWRPRE)   * WCK_TO_CK_RATIO;
    tREFSBRD = ((INT32) tREFSBRD) * WCK_TO_CK_RATIO;
  }

  // Setup tRRD_sg and tRRD_dg based on DdrType.
  if (IsDdr5) {
    tRRDsg = Timing->tRRD_L;
    tRRDdg = Timing->tRRD_S;
  } else {
    // in LP5, as there is no concept of tRRD_sg or dg, so we set them to the same value
    tRRD = Timing->tRRD;
    if (IsLpddr5) {
      tRRD = ((UINT32) tRRD) * WCK_TO_CK_RATIO;
    }
    tRRDsg = tRRD;
    tRRDdg = tRRD;
  }

  if (tREFSBRD != 0) {
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctREFSbRd, WriteToCache | PrintValue, &tREFSBRD);
  }

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
  INT64           tOSCO;
  INT64           tMRR;
  INT64           tPREMRR;
  INT64           tVRCGEnable;
  INT64           tVRCGDisable;
  BOOLEAN         IsLpddr5;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Timing      = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  DdrType     = Outputs->DdrType;
  tCK         = Timing->tCK;
  IsLpddr5    = Outputs->IsLpddr5;
  DdrFreq     = Outputs->Frequency;

  tXP = MrcGetTxp (DdrType, tCK);
  if (IsLpddr5) {
    // More conservative approach is used, optimization is done in SAGV Final when CS ODT is enabled
    // tXP = MAX(tPDXCSODTON, 3nCK) = MAX(20ns, 3nck)
    tXP = MAX (DIVIDECEIL (MRC_LPDDR5_tPDXCSODTON, tCK), MRC_LP5_tXP_MIN_NCK);
    tXP = ((UINT32) tXP) * WCK_TO_CK_RATIO;
  }

  if (DdrType == MRC_DDR_TYPE_DDR5) {
    tCSL = (Timing->NMode == 2) ? MRC_DDR5_tCSL_SREXIT_2N : MRC_DDR5_tCSL_SREXIT_1N;

    tCA2CS = MrcGetTcacsh (MrcData);
  } else {
    tCSL = MrcGetTcsl (MrcData);

    tCA2CS = MAX (MrcGetTcacsh (MrcData), MrcGetTckckeh (MrcData));
  }
  tCKE = MrcGetTcke (DdrType, tCK);
  tCSH    = MrcGetTcsh (MrcData);
  tPRPDEN = MrcGetTprpden (MrcData, DdrFreq);
  tRDPDEN = MrcGetTrdpden (MrcData, Timing->tCL);
  tWRPDEN = MrcGetTwrpden (MrcData, Timing->tCWL, Timing->tWR);
  tOSCO   = MrcGetTosco (MrcData);

  if (IsLpddr5) {
    // Scale to WCK for Lpddr5
    tCKE    = ((UINT32) tCKE)    * WCK_TO_CK_RATIO;
    tCA2CS  = ((UINT32) tCA2CS)  * WCK_TO_CK_RATIO;
    tCSL    = ((UINT32) tCSL)    * WCK_TO_CK_RATIO;
    tCSH    = ((UINT32) tCSH)    * WCK_TO_CK_RATIO;
    tPRPDEN = ((UINT32) tPRPDEN) * WCK_TO_CK_RATIO;
    tRDPDEN = ((UINT32) tRDPDEN) * WCK_TO_CK_RATIO;
    tWRPDEN = ((UINT32) tWRPDEN) * WCK_TO_CK_RATIO;
    tOSCO   = ((UINT32) tOSCO)   * WCK_TO_CK_RATIO;
  }

  // tPRPDEN must be programmed to a minimum of 4
  tPRPDEN = MAX (tPRPDEN, MRC_tPRPDEN_MIN);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCKE,    WriteToCache | PrintValue, &tCKE);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctXP,     WriteToCache | PrintValue, &tXP);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctPRPDEN, WriteToCache | PrintValue, &tPRPDEN);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDPDEN, WriteToCache | PrintValue, &tRDPDEN);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctWRPDEN, WriteToCache | PrintValue, &tWRPDEN);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCSH,    WriteToCache | PrintValue, &tCSH);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCSL,    WriteToCache | PrintValue, &tCSL);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttOSCO,  WriteToCache | PrintValue, &tOSCO);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctCA2CS,  WriteToCache | PrintValue, &tCA2CS);


  tMRR = MrcGetTmrr (MrcData, Timing->tCL);

  // LP5 only
  if (IsLpddr5) {
    tPREMRR = MAX (tMRR, MrcGetWrToMrr (MrcData, Timing->tCWL, Timing->tWTR, Timing->tWTR_L));
    tPREMRR = ((UINT32) tPREMRR) * WCK_TO_CK_RATIO;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttPREMRR, WriteToCache | PrintValue, &tPREMRR);
    tMRR = ((UINT32) tMRR) * WCK_TO_CK_RATIO;
  } else {
    MrcExtendTmrr (MrcData, &tMRR);
  }

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttMRR,  WriteToCache | PrintValue, &tMRR);

  // Setup tVRCG_ENABLE and tVRCG_DISABLE
  tVRCGEnable  = MrcGetTvrcgEnable (MrcData);
  tVRCGDisable = MrcGetTvrcgDisable (MrcData);
  if (IsLpddr5) {
    tVRCGEnable  = ((UINT32) tVRCGEnable)  * WCK_TO_CK_RATIO;
    tVRCGDisable = ((UINT32) tVRCGDisable) * WCK_TO_CK_RATIO;
  }
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctVrcgEnable,  WriteToCache | PrintValue, &tVRCGEnable);
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
  MrcTurnAroundTimingsPreTraining (MrcData);

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

/**
  This function returns the tccd_32_byte_cas_delta value.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] Timing   - Timing Settings.

  @returns The tccd_32_byte_cas_delta value for the specified configuration.
**/
UINT32
MrcGetTccdByteCasDelta (
  IN MrcParameters *const MrcData,
  IN MrcTiming     *Timing
  )
{
  UINT32 Result = 0;
  UINT32 tCCD_L_WR;
  UINT32 tCCD_L_WR2;

  MrcOutput *Outputs = &MrcData->Outputs;
  if (Outputs->IsLpddr5) {
    if (Outputs->Frequency <= f3200) {
      Result = 8;
    } else if (Outputs->Frequency < f10667) {
      Result = 16;
    } else {
      Result = 4;
    }
  } else if (Outputs->IsDdr5) {
    tCCD_L_WR  = Timing->tCCD_L_WR;
    tCCD_L_WR2 = tCCD_L_WR / 2;
    Result = tCCD_L_WR - tCCD_L_WR2;
  }

  return Result;
}
