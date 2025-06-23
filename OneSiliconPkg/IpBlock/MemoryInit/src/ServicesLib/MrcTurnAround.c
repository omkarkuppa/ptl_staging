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
#include "MrcDdrIoDefines.h"
#include "MrcDdrIoUtils.h"
#include "MrcHalRegisterAccess.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcReset.h"
#include "MrcCommon.h"
#include "MrcDdrIoApi.h"
#include "MrcMcApi.h"

/**
  This function calculates the tRdRd (sg,dg,dr,dd) for LPDDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] DeltaRcvEnPiCode  - Largest Delta of RcvEnPiCode across Bytes across the Channel
  @param[out] tRdRdsg          - Return calculated tRdRdsg Timing
  @param[out] tRdRddg          - Return calculated tRdRddg Timing
  @param[out] tRdRddr          - Return calculated tRdRddr Timing
  @param[out] tRdRddd          - Return calculated tRdRddd Timing
**/
VOID
GetLpddr5tRdRd (
  IN   MrcParameters *const MrcData,
  IN   UINT32               DeltaRcvEnPiCode,
  OUT  UINT32               *tRdRdsg,
  OUT  UINT32               *tRdRddg,
  OUT  UINT32               *tRdRddr,
  OUT  UINT32               *tRdRddd
  )
{
  MrcOutput *Outputs;
  UINT32    Guardband;
  UINT32    ChFlightTime;
  UINT32    DramReadDriftPs;
  UINT32    DramReadDriftPI;
  INT32     PHClk;
  INT32     TA1;
  UINT8     RdPreambleT;
  UINT8     RdPreambleLow;
  UINT8     RdPostamble;
  UINT8     RightShift;
  UINT8     LeftShift;
  UINT8     tBLn_min;

  Outputs = &MrcData->Outputs;
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);
  RightShift = 9;
  LeftShift  = 2;
  Guardband  = 8;
  ChFlightTime = 0;

  DramReadDriftPs = GetLpddr5ReadDrift (MrcData);
  DramReadDriftPI = DIVIDECEIL ((DramReadDriftPs * PI_PER_TCK), PHClk);
  tBLn_min = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 2 : 0);
  MrcGetPrePostamble (MrcData, GetTimingRead, &RdPreambleT, &RdPreambleLow, &RdPostamble);
  //-----------------------------------
  // Calculate tRDRD
  //-----------------------------------
  // tRdRd(dr,dd) = RdPreamble + ChFlightTime (Stabilize Bus) + Delta RcvEnPICode (Nominal DRAM Read Timing) +
  // DramReadDrift (Change in DRAM Read Timing due to Voltage/Temp) + Guardband
  // tRdRdsg = tCCD_L - Spec Defined
  // tRdRddg = tCCD_S - Spec Defined

  if (Outputs->ReadPreamble == MRC_LP5_tRPRE_TOGGLE_4tWCK_2STATIC) {
    RdPreambleLow = 4; // tRDQS_PRE is MIN 2*tWCK and MAX 4*tWCK, so for TAT calculation we use 4 as the worst case
  }
  TA1 = PI_PER_TCK * (RdPreambleT + RdPreambleLow) + ChFlightTime + DeltaRcvEnPiCode + DramReadDriftPI + Guardband;
  *tRdRddr = ((TA1 >> RightShift) + 1 + tBLn_min);
  *tRdRddr = (*tRdRddr << LeftShift);
  *tRdRddd = *tRdRddr;
  *tRdRdsg = GetLpddrtCCDL (MrcData);
  *tRdRddg = GetLpddrtCCDS (MrcData);
}

/**
  This function calculates the tWRWR (sg,dg,dr,dd) for LPDDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[out] tWrWrsg          - Return calculated tWRWRdg Timing
  @param[out] tWrWrdg          - Return calculated tWRWRdg Timing
  @param[out] tWrWrdr          - Return calculated tWRWRdr Timing
  @param[out] tWrWrdd          - Return calculated tWRWRdd Timing
**/
VOID
GetLpddr5tWrWr (
  IN   MrcParameters *const MrcData,
  OUT  UINT32               *tWrWrsg,
  OUT  UINT32               *tWrWrdg,
  OUT  UINT32               *tWrWrdr,
  OUT  UINT32               *tWrWrdd
  )
{
  MrcOutput *Outputs;
  UINT32    Guardband;
  UINT32    GB;
  UINT32    ChFlightTime;
  UINT32    nCK;
  INT32     TA1;
  UINT32    TA2;
  INT32     PHClk;
  INT32     tWck2Ck_Max;
  INT32     tWck2Ck_Min;
  UINT8     LeftShift;
  UINT8     tBLn_min;
  UINT32    DramWr2Wr_nCK;
  INT8      tODTLon;

  Outputs = &MrcData->Outputs;
  LeftShift  = 2;
  Guardband  = 8;
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);
  GB = (Guardband * PHClk) / PI_PER_TCK;
  tWck2Ck_Max = PHClk / 2;
  tWck2Ck_Min = -PHClk / 2;
  nCK = PHClk * 4;
  tODTLon = MrcGetOdtlTiming (MrcData, Outputs->Frequency, LpWrOdt, LpOdtlOn);
  tBLn_min = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 2 : 0);
  //-----------------------------------
  // Calculate tWRWR
  //-----------------------------------
  // DQ ODT Enabled case:
  // JEDEC Definition
  // tWRWR_dr = ODTLoff + RU(tODToff(max)/tCK) - ODTLon
  // ODTLon  = tCWL - tODTLon (MrcGetOdtlTiming already returns a negative value, hence using "+" below)
  // ODTLoff = tCWL + BL/n_min + RU(tWCK2DQI(max)/tCK)
  // tWRWR_dr = BL/n_min + RU(tWCK2DQI(max)/tCK) + tWCK2CK_max + RU(tODToff(max)/tCK) - tODTLon
  // tWCK2CK_max = 0.5 * tWCK = 1UI
  //
  // Design Definition
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
  *tWrWrdr = (DramWr2Wr_nCK << LeftShift);
  *tWrWrdd = *tWrWrdr;
  *tWrWrsg = GetLpddrtCCDL (MrcData);
  *tWrWrdg = GetLpddrtCCDS (MrcData);
}

/**
  This function calculates the tRDWR (sg,dg,dr,dd) for LPDDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[out] tRdWrsg          - Return calculated tRDWRsg Timing
  @param[out] tRdWrdg          - Return calculated tRDWRdg Timing
  @param[out] tRdWrdr          - Return calculated tRDWRdr Timing
  @param[out] tRdWrdd          - Return calculated tRDWRdd Timing
**/
VOID
GetLpddr5tRdWr (
  IN   MrcParameters *const MrcData,
  OUT  UINT32               *tRdWrsg,
  OUT  UINT32               *tRdWrdg,
  OUT  UINT32               *tRdWrdr,
  OUT  UINT32               *tRdWrdd
  )
{
  const MrcInput* Inputs;
  MrcOutput *Outputs;
  MrcTiming *Timing;
  UINT32    Guardband;
  UINT32    GB;
  UINT32    ChFlightTime;
  UINT32    nCK;
  INT32     TA1;
  UINT32    TA2;
  UINT32    TA3;
  INT32     PHClk;
  INT32     tWck2Ck_Max;
  UINT8     LeftShift;
  UINT8     tBLn_min;
  UINT8     tBLn_max;
  UINT32    tWCKDQOmax;
  UINT16    tCL;
  UINT16    tCWL;
  INT8      tODTLon;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Timing  = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  LeftShift  = 2;
  Guardband  = 8;
  tCWL = Timing->tCWL;
  tCL = Timing->tCL;
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);
  GB = (Guardband * PHClk) / PI_PER_TCK;
  tWck2Ck_Max = PHClk / 2;
  nCK = PHClk * 4;
  tWCKDQOmax = MrcLpddrIsLowFreq (Outputs->Frequency) ? MRC_LP5_tWCKDQO_LF_MAX : MRC_LP5_tWCKDQO_HF_MAX;
  tODTLon = MrcGetOdtlTiming (MrcData, Outputs->Frequency, LpWrOdt, LpOdtlOn);
  tBLn_min = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 2 : 0);
  tBLn_max = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 4 : 0);

  //-----------------------------------
  // Calculate tRDWR
  //-----------------------------------
  // JEDEC Definition:
  // DQ ODT Enabled case:
  // tRDWR_sr = RL + BL/n_max + RU(tWCK2DQO(max)/tCK) + RD(tRPST/tCK) - ODTLon - RD(tODTon(min)/tCK) + 1
  // RD(tRPST/tCK) = 0
  // tODTon(min) = 1.5ns
  // ODTLon = tCWL - tODTLon (MrcGetOdtlTiming already returns a negative value, hence using "+" below)
  //
  // Design Definition:
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
  *tRdWrsg = (TA2 << LeftShift);
  *tRdWrdg = (TA3 << LeftShift);
  *tRdWrdr = (TA3 << LeftShift);
  *tRdWrdd = *tRdWrdr;
}

/**
  This function calculates the tWRRD (sg,dg,dr,dd) for LPDDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[out] tWrRdsg          - Return calculated tWRRDsg Timing
  @param[out] tWrRddg          - Return calculated tWRRDdg Timing
  @param[out] tWrRddr          - Return calculated tWRRDdr Timing
  @param[out] tWrRddd          - Return calculated tWRRDdd Timing
**/
VOID
GetLpddr5tWrRd (
  IN   MrcParameters *const MrcData,
  OUT  UINT32               *tWrRdsg,
  OUT  UINT32               *tWrRddg,
  OUT  UINT32               *tWrRddr,
  OUT  UINT32               *tWrRddd
  )
{
  const MrcInput* Inputs;
  MrcOutput *Outputs;
  MrcTiming *Timing;
  //UINT32    Guardband;
  //UINT32    GB;
  //UINT32    ChFlightTime;
  UINT32    nCK;
  INT32     TA1;
  INT32     PHClk;
  //INT32     tWck2Ck_Min;
  UINT8     LeftShift;
  UINT8     tBLn_min;
  UINT8     tBLn_max;
  UINT16    tCL;
  UINT16    tCWL;
  UINT32    tWTR_S;
  UINT32    tWTR_L;
  MRC_LP5_BANKORG Lp5BGOrg;
   INT8     ODTLoff;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Timing  = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  LeftShift  = 2;
 // Guardband  = 8;
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);
  //GB = (Guardband * PHClk) / PI_PER_TCK;
  //tWck2Ck_Min = -PHClk / 2;
  nCK = PHClk * 4;
  tBLn_min = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 2 : 0);
  tBLn_max = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 4 : 0);
  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  ODTLoff = MrcGetOdtlTiming (MrcData, Outputs->Frequency, LpWrOdt, LpOdtlOff);

  tCWL = Timing->tCWL;
  tCL = Timing->tCL;
  if (Lp5BGOrg == MrcLp5BgMode) {
    tWTR_S = Timing->tWTR_S;
    tWTR_L = Timing->tWTR_L;
  } else {
    tWTR_S = tWTR_L = Timing->tWTR;
  }

  //-----------------------------------
  // Calculate tWRRD - Currently using JEDEC Definition
  //-----------------------------------
  // DQ ODT Enabled case:
  // JEDEC Definition
  // tWRRD_dr = ODTLoff + RU(tODToff(max)/tCK) - RL
  // ODTLoff = tCWL + BL/n_min + RU(tWCK2DQI(max)/tCK)
  // tWRRD_dr = tCWL - RL + BL/n_min + RU(tWCK2DQI(max)/tCK) + RU(tODToff(max)/tCK)

  TA1 = MRC_LP5_tODT_ON_OFF_MAX;
  TA1 = DIVIDECEIL (TA1, nCK);
  TA1 += tCWL - tCL +  ODTLoff; // ODTLOff includes BL/n_min


  // Design Definition
  // tWrRddr > (WL-RL) + 1 + (tODToff_max + ChFlightTime - tWCK2CK_Min - tWCKDQO_Min)/nCK
  // tWrRdsg = tCWL + tBln_max + tWTR_L - Spec Defined
  // tWrRddg = tCWL + tBln_min + tWTR_S - Spec Defined
  //ChFlightTime = 0;
  //TA1 = MRC_LP5_tODT_ON_OFF_MAX + ChFlightTime + GB + MRC_LP5_TCK2CK - tWck2Ck_Min - MRC_LP5_tWCKDQO_MIN;
  //TA1 = DIVIDECEIL (TA1, nCK);
  //TA1 += tCWL - tCL + 1 + tBLn_min;
  //if (TA1 < 0) {
  //  TA1 = 0;
  //}
  *tWrRddr = (TA1 << LeftShift);
  *tWrRddd =  *tWrRddr;
  *tWrRdsg = (tCWL + tBLn_max + tWTR_L) << LeftShift;
  *tWrRdsg = MAX (*tWrRddr, *tWrRdsg);
  *tWrRddg = (tCWL + tBLn_min + tWTR_S) << LeftShift;
  *tWrRddg = MAX (*tWrRddr, *tWrRddg);
}

/**
  This function calculates the tRdRd (sg,dg,dr,dd) for DDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] DeltaRcvEnPiCode  - Largest Delta of RcvEnPiCode across Bytes across the Channel
  @param[out] tRdRdsg          - Return calculated tRdRdsg Timing
  @param[out] tRdRddg          - Return calculated tRdRddg Timing
  @param[out] tRdRddr          - Return calculated tRdRddr Timing
  @param[out] tRdRddd          - Return calculated tRdRddd Timing
**/
VOID
GetDdr5tRdRd (
  IN MrcParameters *const  MrcData,
  IN   UINT32              DeltaRcvEnPiCode,
  OUT  UINT32              *tRdRdsg,
  OUT  UINT32              *tRdRddg,
  OUT  UINT32              *tRdRddr,
  OUT  UINT32              *tRdRddd
  )
{
  const MrcInput* Inputs;
  MrcOutput *Outputs;
  MrcTiming *Timing;
  INT32  TA1;
  UINT32 ChFlightTimePI;
  UINT32 tCCD_L;
  UINT32 tCCD_S;
  UINT32 tDQSCK;
  UINT32 tDQSCKi;
  UINT8  RdPreambleT;
  UINT8  RdPreambleLow;
  UINT8  RdPostamble;
  UINT8  tADCMin;
  UINT8  tADCMax;
  UINT8  Guardband;
  UINT8  RightShift;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Timing  = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  tCCD_L = Timing->tCCD_L;
  tCCD_S = MRC_DDR5_tCCD_ALL_FREQ;
  Ddr5GetTDqsCK (MrcData, &tDQSCK, &tDQSCKi);
  // Convert to Pi ticks
  tDQSCKi = (tDQSCKi * PI_PER_TCK) / 1000;
  MrcGetDdr5Tadc (Outputs->Frequency, &tADCMin, &tADCMax);
  Guardband = 8;
  RightShift = 7;

  MrcGetPrePostamble (MrcData, GetTimingRead, &RdPreambleT, &RdPreambleLow, &RdPostamble);
  //-----------------------------------
  // Calculate tRDRD
  //-----------------------------------
  // tRdRddr = tRdRddd = RdPreamble + ChFlightTimePI + DeltaPICode + ReadDrift + tADC_Max +  GuardBand
  // tRdRdsg = tCCD_L;
  // tRdRddg = tCCD_S
  ChFlightTimePI = 0;
  TA1 = PI_PER_TCK * (RdPreambleT + RdPreambleLow) + ChFlightTimePI + DeltaRcvEnPiCode + (tDQSCKi / 2) + tADCMax + Guardband;
  *tRdRddr = (TA1 >> RightShift) + 1;
  *tRdRddr += Outputs->BurstLength;
  *tRdRddd = *tRdRddr;
  *tRdRdsg = tCCD_L;
  *tRdRddg = tCCD_S;
}

/**
  This function calculates the tWRWR (sg,dg,dr,dd) for DDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] MaxOdtWrOff       - Max ODT WR Off Offset across all Ranks of a channel (MR37:OP[5:3])
  @param[in] MinOdtWrOn        - Max ODT WR On Offset across all Ranks of a channel MR37:OP[2:0])
  @param[out] tWrWrsg          - Return calculated tWRWRsg Timing
  @param[out] tWrWrdg          - Return calculated tWRWRdg Timing
  @param[out] tWrWrdr          - Return calculated tWRWRdr Timing
  @param[out] tWrWrdd          - Return calculated tWRWRdd Timing
**/
VOID
GetDdr5tWrWr (
  IN MrcParameters *const  MrcData,
  IN   INT32               MaxOdtWrOff,
  IN   INT32               MinOdtWrOn,
  OUT  UINT32              *tWrWrsg,
  OUT  UINT32              *tWrWrdg,
  OUT  UINT32              *tWrWrdr,
  OUT  UINT32              *tWrWrdd
  )
{
  const MrcInput* Inputs;
  MrcOutput *Outputs;
  MrcTiming *Timing;
  INT32  TA1;
  UINT32 tCCD_S;
  UINT32 tCCD_L_WR;
  UINT8  RightShift;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Timing  = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  tCCD_S = MRC_DDR5_tCCD_ALL_FREQ;
  tCCD_L_WR = Timing->tCCD_L_WR;
  RightShift = 7;

  //-----------------------------------
  // Calculate tWRWR
  //-----------------------------------
  // tWrWrdr = tWrWrdd = ODT_Wr_OFF - ODT_Wr_On
  // tWrWrsg = tCCD_L_WR;
  // tWrWrdg = tCCD_S
  TA1 = PI_PER_TCK * (MaxOdtWrOff - MinOdtWrOn);
  *tWrWrdr = (TA1 >> RightShift) + 1;
  *tWrWrdr += Outputs->BurstLength;
  *tWrWrdd = *tWrWrdr;
  *tWrWrsg = tCCD_L_WR;
  *tWrWrdg = tCCD_S;
}

/**
  This function calculates the tRDWR (sg,dg,dr,dd) for DDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] DeltaTxDqsPiCode  - Largest Delta of TxDqsPiCode across Bytes across the Channel
  @param[in] MaxOdtRdOff       - Max ODT Rd Off Offset across all Ranks of a channel (MR39:OP[5:3])
  @param[in] MinOdtWrOn        - Max ODT WR On Offset across all Ranks of a channel MR37:OP[2:0])
  @param[out] NTODTRd          - TRUE if NT RD ODT is enabled on any ranks within the Channel
  @param[out] tRDWRsg          - Return calculated tRDWRsg Timing
  @param[out] tRDWRdg          - Return calculated tRDWRdg Timing
  @param[out] tRDWRdr          - Return calculated tRDWRdr Timing
  @param[out] tRDWRdd          - Return calculated tRDWRdd Timing
**/
VOID
GetDdr5tRdWr (
  IN MrcParameters *const  MrcData,
  IN  UINT32               DeltaTxDqsPiCode,
  IN  INT32                MaxOdtRdOff,
  IN  INT32                MinOdtWrOn,
  IN  BOOLEAN              NTODTRd,
  OUT  UINT32              *tRdWrsg,
  OUT  UINT32              *tRdWrdg,
  OUT  UINT32              *tRdWrdr,
  OUT  UINT32              *tRdWrdd
  )
{
  const MrcInput* Inputs;
  MrcOutput *Outputs;
  MrcTiming *Timing;
  INT32  TA1;
  INT32  TA2;
  INT32  TA3;
  UINT8  RightShift;
  UINT32 ChFlightTimePI;
  UINT32 ChFlightTime;
  UINT32 PHClk;
  UINT16 tCL;
  UINT16 tCWL;
  UINT8  WrPreambleT;
  UINT8  WrPreambleLow;
  UINT8  WrPostamble;
  UINT8  RdPostamble;
  UINT8  tADCMin;
  UINT8  tADCMax;
  UINT8  Guardband;
  INT32  tDQSSMax;
  INT32  tDQSSMin;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Timing  = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);
  tCWL = Timing->tCWL;
  tCL = Timing->tCL;
  MrcGetDdr5Tadc (Outputs->Frequency, &tADCMin, &tADCMax);
  RightShift = 7;
  Guardband = 8;

  MrcGetPrePostamble (MrcData, GetTimingRead, NULL, NULL, &RdPostamble);
  MrcGetPrePostamble (MrcData, GetTimingWrite, &WrPreambleT, &WrPreambleLow, &WrPostamble);
  GetDdr5tDQSS (MrcData, &tDQSSMax, &tDQSSMin, WrPreambleLow);

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
  TA1 = (PI_PER_TCK * (tCL - tCWL + WrPreambleLow + WrPreambleT + RdPostamble)) + tADCMax + ChFlightTimePI + DeltaTxDqsPiCode;
  TA1 += Guardband - MRC_DDR5_tDQSOFFSET_MIN - tDQSSMin;
  TA2 = (PI_PER_TCK * (tCL - tCWL - MinOdtWrOn)) + 128 * (NTODTRd ? MaxOdtRdOff : 0) + DeltaTxDqsPiCode + ChFlightTimePI + Guardband;
  TA3 = (MAX ((UINT32)TA1, (UINT32)TA2) >> RightShift) + 1 + Outputs->BurstLength;
  *tRdWrdr = TA3;
  *tRdWrdd = TA3;
  *tRdWrsg = TA3;
  *tRdWrdg = TA3;
}

/**
  This function calculates the tWRRD (sg,dg,dr,dd) for DDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] DeltaTxDqsPiCode  - Largest Delta of TxDqsPiCode across Bytes across the Channel
  @param[in] MinOdtRdOn        - Max ODT Rd On Offset across all Ranks of a channel (MR39:OP[2:0])
  @param[in] MaxOdtWrOff       - Max ODT WR Off Offset across all Ranks of a channel MR37:OP[5:3])
  @param[out] NTODTRd          - TRUE if NT RD ODT is enabled on any ranks within the Channel
  @param[out] tWRRDsg          - Return calculated tWRRDsg Timing
  @param[out] tWRRDdg          - Return calculated tWRRDdg Timing
  @param[out] tWRRDdr          - Return calculated tWRRDdr Timing
  @param[out] tWRRDdd          - Return calculated tWRRDdd Timing
**/
VOID
GetDdr5tWrRd (
  IN MrcParameters *const  MrcData,
  IN  UINT32               DeltaTxDqsPiCode,
  IN  INT8                 MinOdtRdOn,
  IN  INT8                 MaxOdtWrOff,
  IN  BOOLEAN              NTODTRd,
  OUT  UINT32              *tWrRdsg,
  OUT  UINT32              *tWrRddg,
  OUT  UINT32              *tWrRddr,
  OUT  UINT32              *tWrRddd
  )
{
  const MrcInput* Inputs;
  MrcOutput *Outputs;
  MrcTiming *Timing;
  INT32  TA1;
  INT32  TA2;
  UINT8  RightShift;
  UINT32 ChFlightTimePI;
  UINT32 ChFlightTime;
  UINT32 tDQSCK;
  UINT32 tDQSCKi;
  UINT32 PHClk;
  UINT16 tCL;
  UINT16 tCWL;
  UINT32 tWTR_S;
  UINT32 tWTR_L;
  UINT8  RdPreambleT;
  UINT8  RdPreambleLow;
  UINT8  RdPostamble;
  UINT8  WrPreambleT;
  UINT8  WrPreambleLow;
  UINT8  WrPostamble;
  UINT8  tADCMin;
  UINT8  tADCMax;
  UINT8  Guardband;
  INT32  tDQSSMax;
  INT32  tDQSSMin;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Timing  = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  PHClk = UDIVIDEROUND (FREQ_TO_TCK_PS, Outputs->Frequency);
  tCWL = Timing->tCWL;
  tCL = Timing->tCL;
  tWTR_S = Timing->tWTR_S;
  tWTR_L = Timing->tWTR_L;
  MrcGetDdr5Tadc (Outputs->Frequency, &tADCMin, &tADCMax);
  Ddr5GetTDqsCK (MrcData, &tDQSCK, &tDQSCKi);
  // Convert to Pi ticks
  tDQSCKi = (tDQSCKi * PI_PER_TCK) / 1000;
  RightShift = 7;
  Guardband = 8;

  MrcGetPrePostamble (MrcData, GetTimingRead, &RdPreambleT, &RdPreambleLow, &RdPostamble);
  MrcGetPrePostamble (MrcData, GetTimingWrite, &WrPreambleT, &WrPreambleLow, &WrPostamble);
  GetDdr5tDQSS (MrcData, &tDQSSMax, &tDQSSMin, WrPreambleLow);

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
 TA1 = (PI_PER_TCK * (tCWL - tCL + RdPreambleLow + RdPreambleT + 1)) + MRC_DDR5_tDQSOFFSET_MAX + tDQSSMax + (WrPostamble * PI_PER_TCK)  + ChFlightTime + DeltaTxDqsPiCode - tADCMin + Guardband;
 // DQ channel flight time
 ChFlightTime = 312; // pS
 ChFlightTimePI = (ChFlightTime * PI_PER_TCK) / PHClk;
 TA2 = (PI_PER_TCK * MaxOdtWrOff) + (NTODTRd ? (128*MinOdtRdOn) : (tADCMax - MRC_DDR5_tDQSQ_MIN - tDQSCK)) + DeltaTxDqsPiCode + ChFlightTimePI + Guardband;
 TA2 += PI_PER_TCK * (tCWL - tCL);
 // Avoid negative value in tWrRddr calculation
 TA1 = MAX (0, TA1);
 TA2 = MAX (0, TA2);
 *tWrRddr = (MAX ((UINT32)TA1, (UINT32)TA2) >> RightShift) + 1;
 *tWrRddr += Outputs->BurstLength;
 *tWrRddd = *tWrRddr;
 *tWrRdsg = tCWL + tWTR_L + Outputs->BurstLength;
 *tWrRdsg = MAX (*tWrRddr, *tWrRdsg);
 *tWrRddg = tCWL + tWTR_S + Outputs->BurstLength;
 *tWrRddg = MAX (*tWrRddr, *tWrRddg);
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
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      MinPiCode = MIN (MinPiCode, ((TxPathResults->MinTxDq[Controller][Channel] + TxPathResults->MinTxDqBit[Controller][Channel])));
      MaxPiCode = MAX (MaxPiCode, ((UINT32) (TxPathResults->MaxTxDq[Controller][Channel] + TxPathResults->MaxTxDqBit[Controller][Channel])));
      if (IsDdr5) {
        // PHYWr2Wr_PI = 128*( (Gear4?3:2) + 0.75 + PIXover + 1 + WRPRE + WRPST) + DeltaPICode + GuardBand
        //  DeltaPICode = DeltaPICode = MAXRANK(TXCONTROL0RANK{PopRanks}.TxDqsDelayPi) - MINRANK(TXCONTROL0RANK{PopRanks}.TxDqsDelayPi)
        //  PIXover = Gear4 ? MAXRANK( (TXCONTROL0RANK{PopRanks}.TxDqsDelayPi>>7) & 0x1 ) : 0
        DeltaPiCode = TxPathResults->DeltaPiCode[Controller][Channel];
        PIXover = (IsGear4 ? (TxPathResults->MaxTxDqs[Controller][Channel] >> 7 & 0x1) : 0) + WrPreambleT + WrPreambleLow + WrPostamble;
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
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
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
  This function gets the Pre Training Turnaround Values

  @param[in]  MrcData - Include all MRC global data.
  @param[out] TAT     - return struct which holds TAT values

  @retval None
**/
VOID
GetDramTatPreTraining (
  IN  MrcParameters *const   MrcData,
  OUT TurnAroundTimings      *TAT
  )
{
  MrcOutput *Outputs;
  UINT8     FirstController;
  UINT8     FirstChannel;
  BOOLEAN   NTODTWr;
  BOOLEAN   NTODTRd;

  Outputs = &MrcData->Outputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel    = Outputs->Controller[FirstController].FirstPopCh;

  if (Outputs->IsDdr5) {
    IsNtOdtSupported (MrcData, FirstController, FirstChannel, &NTODTWr, &NTODTRd);
    GetDdr5tRdRd (MrcData, DEFAULT_DELTA_RCVEN_PICODE, &TAT->tRdRdsg, &TAT->tRdRddg, &TAT->tRdRddr, &TAT->tRdRddd);
    GetDdr5tWrWr (MrcData, MRC_DDR5_WORST_CASE_ODT_WR_OFF_OFFSET, MRC_DDR5_WORST_CASE_ODT_WR_ON_OFFSET, &TAT->tWrWrsg, &TAT->tWrWrdg, &TAT->tWrWrdr, &TAT->tWrWrdd);
    GetDdr5tRdWr (MrcData, DEFAULT_DELTA_TXDQS_PICODE, MRC_DDR5_WORST_CASE_ODT_RD_OFF_OFFSET, MRC_DDR5_WORST_CASE_ODT_WR_ON_OFFSET, NTODTRd, &TAT->tRdWrsg, &TAT->tRdWrdg, &TAT->tRdWrdr, &TAT->tRdWrdd);
    GetDdr5tWrRd (MrcData, DEFAULT_DELTA_TXDQS_PICODE, MRC_DDR5_WORST_CASE_ODT_RD_ON_OFFSET, MRC_DDR5_WORST_CASE_ODT_WR_OFF_OFFSET, NTODTRd, &TAT->tWrRdsg, &TAT->tWrRddg, &TAT->tWrRddr, &TAT->tWrRddd);
  } else {
    GetLpddr5tRdRd (MrcData, (UINT32) DEFAULT_DELTA_RCVEN_PICODE, &TAT->tRdRdsg, &TAT->tRdRddg, &TAT->tRdRddr, &TAT->tRdRddd);
    GetLpddr5tWrWr (MrcData, &TAT->tWrWrsg, &TAT->tWrWrdg, &TAT->tWrWrdr, &TAT->tWrWrdd);
    GetLpddr5tRdWr (MrcData, &TAT->tRdWrsg, &TAT->tRdWrdg, &TAT->tRdWrdr, &TAT->tRdWrdd);
    GetLpddr5tWrRd (MrcData, &TAT->tWrRdsg, &TAT->tWrRddg, &TAT->tWrRddr, &TAT->tWrRddd);
  }
}

/**
  This function gets the Post Training Turnaround Values

  @param[in]  MrcData       - Include all MRC global data.
  @param[in]  RxPathResults - Struct contianing Min/Max Data for Rx Data Path
  @param[in]  TxPathResults - Struct contianing Min/Max Data for Tx Data Path
  @param[in]  Controller    - Current Controller
  @param[in]  Channel       - Current Channel
  @param[out] TAT           - return struct which holds TAT values

  @retval None
**/
VOID
GetDramTatPostTraining (
  IN  MrcParameters *const MrcData,
  IN  RxPathMinMax         *RxPathResults,
  IN  TxPathMinMax         *TxPathResults,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  OUT TurnAroundTimings    *TAT
  )
{
  MrcOutput      *Outputs;
  UINT32 DeltaRcvEnPiCode;
  UINT32 DeltaTxDqsPiCode;
  UINT32 Rank;
  UINT32 Byte;
  UINT32 WLTxDqs;
  UINT32 MaxWL;
  UINT32 MinWL;
  INT64  TxDqs;
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

  // DeltaPiCode = Delta of RcvEn
  DeltaRcvEnPiCode = (UINT32) (RxPathResults->MaxRcvEnPi[Controller][Channel] - RxPathResults->MinRcvEnPi[Controller][Channel]);
  if (Outputs->IsDdr5) {
    IsNtOdtSupported (MrcData, Controller, Channel, &NTODTWr, &NTODTRd);
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
      GetDdr5OdtOffsets (MrcData, Controller, Channel, Rank, NTODTWr, FALSE, &OdtWrOn, &OdtWrOff, &OdtRdOn, &OdtRdOff);
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

    GetDdr5tRdRd (MrcData, DeltaRcvEnPiCode, &TAT->tRdRdsg, &TAT->tRdRddg, &TAT->tRdRddr, &TAT->tRdRddd);
    GetDdr5tWrWr (MrcData, MaxOdtWrOff, MinOdtWrOn, &TAT->tWrWrsg, &TAT->tWrWrdg, &TAT->tWrWrdr, &TAT->tWrWrdd);
    GetDdr5tRdWr (MrcData, DeltaTxDqsPiCode, MaxOdtRdOff, MinOdtWrOn, NTODTRd, &TAT->tRdWrsg, &TAT->tRdWrdg, &TAT->tRdWrdr, &TAT->tRdWrdd);
    GetDdr5tWrRd (MrcData, DeltaTxDqsPiCode, MinOdtRdOn, MaxOdtWrOff, NTODTRd, &TAT->tWrRdsg, &TAT->tWrRddg, &TAT->tWrRddr, &TAT->tWrRddd);
  } else {
    GetLpddr5tRdRd (MrcData, DeltaRcvEnPiCode, &TAT->tRdRdsg, &TAT->tRdRddg, &TAT->tRdRddr, &TAT->tRdRddd);
    GetLpddr5tWrWr (MrcData, &TAT->tWrWrsg, &TAT->tWrWrdg, &TAT->tWrWrdr, &TAT->tWrWrdd);
    GetLpddr5tRdWr (MrcData, &TAT->tRdWrsg, &TAT->tRdWrdg, &TAT->tRdWrdr, &TAT->tRdWrdd);
    GetLpddr5tWrRd (MrcData, &TAT->tWrRdsg, &TAT->tWrRddg, &TAT->tWrRddr, &TAT->tWrRddd);
  }
}

/**
  This function gets and sets the Pre Training Turnaround Values

  @param[in]  MrcData - Include all MRC global data.

  @retval None
**/
VOID
MrcTurnAroundTimingsPreTraining (
  IN  MrcParameters *const MrcData
  )
{
  const MrcInput  *Inputs;
  const MRC_FUNCTION *MrcCall;
  TurnAroundTimings  TAT;

  Inputs    = &MrcData->Inputs;
  MrcCall   = Inputs->Call.Func;

  MrcCall->MrcSetMem ((UINT8 *) &TAT, sizeof (TAT), 0);
  GetDramTatPreTraining (MrcData, &TAT);
  SetDramTatTimings (MrcData, &TAT, MAX_CONTROLLER, MAX_CHANNEL, TRUE);
}

/**
  This function implements Turn Around Timing Optimization.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
MrcStatus
MrcTurnAroundTimingOptimization (
  IN  MrcParameters *const   MrcData
  )
{
  MrcOutput *Outputs;
  const MrcInput  *Inputs;
  const MRC_FUNCTION *MrcCall;
  MrcDebug *Debug;
  TurnAroundTimings  TAT;
  TxPathMinMax TxPathResults;
  RxPathMinMax RxPathResults;
  UINT32 PHYRd2Rd_PI[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 PHYWr2Wr_PI[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 Controller;
  UINT32 Channel;

  Inputs    = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  MrcCall   = Inputs->Call.Func;
  Debug     = &Outputs->Debug;
  MrcCall->MrcSetMem ((UINT8 *) &TxPathResults, sizeof (TxPathResults), 0);
  MrcCall->MrcSetMem ((UINT8 *) &RxPathResults, sizeof (RxPathResults), 0);
  MrcCall->MrcSetMem ((UINT8 *) &TAT, sizeof (TAT), 0);

  // Find Max/Min Values for Rx/Tx Path parameters
  GetTxPathMinMax (MrcData, &TxPathResults);
  GetRxPathMinMax (MrcData, &RxPathResults);

  // Calculate TAT values constrained by PHY and training values
  GetPhyRd2RdTA (MrcData, &RxPathResults, PHYRd2Rd_PI);
  GetPhyWr2WrTA (MrcData, &TxPathResults, PHYWr2Wr_PI);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      GetDramTatPostTraining (MrcData, &RxPathResults, &TxPathResults, Controller, Channel, &TAT);

      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        "Mc%d.Ch%d: PhyRdRd:%d tRdRd:%d, PhyWrWr:%d tWrWr:%d \n",
        Controller,
        Channel,
        PHYRd2Rd_PI[Controller][Channel],
        TAT.tRdRddr,
        PHYWr2Wr_PI[Controller][Channel],
        TAT.tWrWrdr
      );
      TAT.tRdRddr = MAX (PHYRd2Rd_PI[Controller][Channel], TAT.tRdRddr);
      TAT.tRdRddd = TAT.tRdRddr;
      TAT.tWrWrdr = MAX (PHYWr2Wr_PI[Controller][Channel], TAT.tWrWrdr);
      TAT.tWrWrdd= TAT.tWrWrdr;

      SetDramTatTimings (MrcData, &TAT, Controller, Channel, FALSE);
    }
  }
  return mrcSuccess;
}

