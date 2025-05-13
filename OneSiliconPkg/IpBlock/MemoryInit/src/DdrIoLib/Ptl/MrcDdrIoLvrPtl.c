/** @file
  This file contains code related to initializing and configuring the DDRIO LVR

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

#include "CMrcTypes.h"
#include "CMcAddress.h"
#include "MrcDdrIoDefines.h"
#include "MrcCommon.h"
#include "MrcDdrIoLvr.h"
#include "MrcDdrIoUtils.h"

#include "MrcDdrIoApi.h"
#include "MrcDdrIoPtl.h"
#include "MrcHalApi.h"
#include "MrcLpddr5.h"


/**
  This function sets up Voltage Sensor CRs for each TargetV
  and starts counter for point test

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Target  - Target SelLvr setting, see enum define
**/
VOID
MrcVoltageSensorTargetSetup (
  IN MrcParameters    *const MrcData,
  IN SelLvrSetting    Target
  )
{
  const MRC_FUNCTION *MrcCall;
  MrcInput    *Inputs;
  INT64       GetSetVal;
  INT64       GetSetEn;
  INT64       GetSetTargetVoltage;
  UINT32      PartitionIndex;
  UINT32      PartitionNum;
  UINT32      PartitionMax;
  UINT32      PartitionNumMax;
  UINT32      LvrSelCount;
  UINT32      SALUsePmos;
  UINT32      TempVar;
  UINT16      PartitionMask [LVR_SENSOR_PARTITIONS];
  BOOLEAN     DtHalo;
  PARTITION_TYPE    Partition [] = {PartitionPg, PartitionPll}; // LVR_VOLTAGE_SENSOR_PG_PARTITIONS

  Inputs = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  DtHalo = MrcData->Inputs.IsDdrIoDtHalo;
  PartitionMax = ARRAY_COUNT (Partition);
  LvrSelCount = 0;
  GetSetEn = 1;
  SALUsePmos = UDIVIDEROUND (SAL_USE_PMOS_MV * MRC_COMP_VREF_CALCULATION_VALUE, BG_REF_MV) - 1;
  MrcCall->MrcSetMem ((UINT8 *) PartitionMask, sizeof (PartitionMask), 0);

  PartitionNumMax = DtHalo ? MRC_PG_DT_NUM : MRC_PG_NUM;

  switch (Target) {
    case SelLvrVccIog:
      PartitionMask[PG_MASK]     = DtHalo ? SEL_LVR_VCCIOG_DT_PG_MASK   : SEL_LVR_VCCIOG_PG_MASK;
      break;

    case SelLvrVccClk:
      PartitionMask[PG_MASK]     = DtHalo ? SEL_LVR_VCCCLK_DT_PG_MASK   : SEL_LVR_VCCCLK_PG_MASK;
      break;

    case SelLvrVccDdq:
      PartitionMask[PG_MASK]     = DtHalo ? SEL_LVR_VCCDDQ_DT_PG_MASK   : SEL_LVR_VCCDDQ_PG_MASK;
      PartitionMask[COMP_MASK]    = SEL_LVR_COMP_MASK;
      break;

    default:
      break;
  }

  for (PartitionIndex = 0; PartitionIndex < PartitionMax; PartitionIndex++) {
    for (PartitionNum = 0; PartitionNum < PartitionNumMax; PartitionNum++) {
      if ((PartitionMask[PartitionIndex] & (1 << PartitionNum)) == 0) {
        continue;
      }
      GetSetTargetVoltage  = GetVTarget (MrcData, 0, Target);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrTarget0, WriteToCache, &GetSetTargetVoltage);
      TempVar = (UINT32) GetSetTargetVoltage;
      GetSetTargetVoltage  = GetVTarget (MrcData, 1, Target);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrTarget1, WriteToCache, &GetSetTargetVoltage);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrEnDiv, WriteToCache, &GetSetEn);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrEnSensor, WriteToCache, &GetSetEn);
      MrcFlushRegisterCachedData (MrcData);  // Flush to write for Target voltage and EnSensor bit set first to allow circuits to wake up

      TempVar = MAX (TempVar, (UINT32) GetSetTargetVoltage);
      GetSetVal = TempVar < SALUsePmos;
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrUsePSAL, WriteToCache, &GetSetVal);
      // Counter starts after (RstCount) followed by RdWrAnalogEn = 2
      GetSetVal = 2;
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrVoltageSensorCntStart, WriteToCache, &GetSetVal);
      // Voltage_Sensor_Duration Count = 2 ^ (5 + Duration) for a  maximum 2^12 cycles
      GetSetVal = 7;
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrVoltageSensorDuration, WriteToCache, &GetSetVal);
      GetSetVal = (INT64) ((LvrSelCount >> PartitionNum) & 1); // SelCount = [1 or 0]
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrSelCount, WriteToCache, &GetSetVal);
      GetSetVal = Target; // Selects Counter for (0: VccIOG, 1: VccDDQ, 2: VccClk, 3: VccDLL[SelCount])
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrSelLVR, WriteToCache, &GetSetVal);
      MrcFlushRegisterCachedData (MrcData);
    } // PartitionNum
  } // PartitionIndex
}

/**
  This function calculates the VddqControl Target value based on VccddqVoltage in Outputs structure
  @param[in] MrcData - All the MRC global data.
  @param[in] VccddqVoltageMv  - Voltage in mV
  @retval calculated value for Target field for VddqControl registers
**/
UINT32
GetVddqControlTarget (
  IN MrcParameters *const MrcData,
  IN UINT32   VccddqVoltageMv
 )
{
  MrcOutput         *Outputs;
  UINT32            VccDdq;
  UINT32            VccClk;
  UINT32            VccIog;
  UINT32            TempVar1;
  UINT32            TempVar2;
  UINT32            Data;

  Outputs = &MrcData->Outputs;
  VccDdq  = VccddqVoltageMv;
  VccClk  = Outputs->VccClkVoltage;
  VccIog  = Outputs->VccIogVoltage;

  TempVar1 = MAX (VccClk, VccIog);
  TempVar2 = MAX (TempVar1, VccDdq);
  Data = (TempVar2 > 925) ? 1 : 0;
  // Min(383, Rnd( VccDdq/(BGRef/386*IF(OCDivider,1.5,1)) -1) )
  TempVar1 = (Data ? 1500 : 1000) * BG_REF_MV;
  TempVar2 = THOUSAND_MULTIPLIER * VccDdq * MRC_COMP_VREF_CALCULATION_VALUE;
  Data = UDIVIDEROUND (TempVar2, TempVar1) - 1;
  Data = MIN (MAX_VCCDDQCONTROL_TARGET, Data);

  return Data;
}

/**
  This function Reset all LVR Voltage Sensor Counters

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Chop    - Swap inputs to SAL comparator and invert outputs to average input offset.
**/
VOID
MrcVoltageSensorResetCount (
  IN MrcParameters    *const MrcData,
  IN UINT8             Chop
  )
{
  static const PARTITION_TYPE Partition [] = {PartitionPg, PartitionPll}; // LVR_VOLTAGE_SENSOR_PG_PARTITIONS
  INT64     GetSetEn;
  INT64     LvrChop;
  UINT32    PartitionIndex;
  UINT32    PartitionMax;
  UINT32    ParMax[LVR_SENSOR_PARTITIONS];

  PartitionMax = ARRAY_COUNT (Partition);
  GetSetEn = 1;
  LvrChop = (INT64) Chop;

  // Setting NUM_DT for Partiton Max since it is larger than mobile NUM, so broadcast will still work for both skus
  ParMax[PG_PARTITION]    = MRC_PG_DT_NUM;
  ParMax[COMP_PARTITION]  = MRC_COMP_NUM;

  for (PartitionIndex = 0; PartitionIndex < PartitionMax; PartitionIndex++) {
    MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], ParMax[PartitionIndex], GsmLvrChop, WriteCached, &LvrChop);
    MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], ParMax[PartitionIndex], GsmLvrRstCount, WriteCached, &GetSetEn);
  } // PartitionIndex
}

/**
  This function reads all LVR Voltage Sensor Counters to determine if CounterPass based on counter limits
  and prints out the CounterPass status

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Chop    - SAL comparator and invert outputs to compare counters pass/fail test.
**/
VOID
MrcVoltageSensorCounter (
  IN      MrcParameters   *const MrcData,
  IN      UINT8           Chop
  )
{
  static const PARTITION_TYPE Partition [] = {PartitionPg, PartitionPll}; // LVR_VOLTAGE_SENSOR_PG_PARTITIONS
  const MrcInput      *Inputs;
  const MRC_FUNCTION  *MrcCall;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  BOOLEAN             LvrPgCounterPass[LVR_COUNTERS][MRC_PG_DT_NUM];
  BOOLEAN             LvrCompCounterPass[LVR_COUNTERS];
  BOOLEAN             *CounterPass0;
  BOOLEAN             *CounterPass1;
  UINT8               CounterIndex;
  INT64               GetSetCount0;
  INT64               GetSetCount1;
  UINT32              PartitionIndex;
  UINT32              PartitionNum;
  UINT32              PartitionMax;
  UINT32              ParMax[LVR_SENSOR_PARTITIONS];
  BOOLEAN             DtHalo;

  DtHalo = MrcData->Inputs.IsDdrIoDtHalo;
  PartitionMax = ARRAY_COUNT (Partition);
  Inputs = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  MrcCall->MrcSetMem ((UINT8 *) LvrPgCounterPass, sizeof (LvrPgCounterPass), TRUE);
  MrcCall->MrcSetMem ((UINT8 *) LvrCompCounterPass, sizeof (LvrCompCounterPass), TRUE);

  ParMax[PG_PARTITION]    = (DtHalo) ? MRC_PG_DT_NUM : MRC_PG_NUM;
  ParMax[COMP_PARTITION]  = MRC_COMP_NUM;

  // Voltage Sensor Read Report
  for (PartitionIndex = 0; PartitionIndex < PartitionMax; PartitionIndex++) {
    for (PartitionNum = 0; PartitionNum < ParMax[PartitionIndex]; PartitionNum++) {
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrVoltageSensorCount0, ReadCached, &GetSetCount0);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrVoltageSensorCount1, ReadCached, &GetSetCount1);
      if (Partition[PartitionIndex] == PartitionPg) {
        CounterPass0 = &LvrPgCounterPass[COUNTER_0][PartitionNum];
        CounterPass1 = &LvrPgCounterPass[COUNTER_1][PartitionNum];
      } else {
        CounterPass0 = &LvrCompCounterPass[COUNTER_0];
        CounterPass1 = &LvrCompCounterPass[COUNTER_1];
      }
      *CounterPass0 &= (Chop ? (GetSetCount0 > COUNTER_PASS_LIMIT) : (GetSetCount0 < -COUNTER_PASS_LIMIT));
      *CounterPass1 &= (Chop ? (GetSetCount1 < -COUNTER_PASS_LIMIT) : (GetSetCount1 > COUNTER_PASS_LIMIT));
    } // PartitionNum
  } // PartitionIndex

  for (PartitionIndex = 0; PartitionIndex < (PartitionMax - 1); PartitionIndex++) {
    for (PartitionNum = 0; PartitionNum < ParMax[PartitionIndex]; PartitionNum++) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n\t\t\t%dPG%d", PartitionIndex, PartitionNum);
    } // PartitionNum
  } // PartitionNum
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\t\t\tCOMP\t(counts)\n");

  for (CounterIndex = 0; CounterIndex < LVR_COUNTERS; CounterIndex++) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Chop%d:Count%d", Chop, CounterIndex);
    for (PartitionIndex = 0; PartitionIndex < (PartitionMax - 1); PartitionIndex++) {
      for (PartitionNum = 0; PartitionNum < ParMax[PartitionIndex]; PartitionNum++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\t\t\t%d", LvrPgCounterPass[CounterIndex][PartitionNum]);
      } // PartitionNum
    } // PartitionNum
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\t\t\t%d\t\t\t%d\n", CounterIndex, LvrCompCounterPass[CounterIndex]);
  } // CounterIndex
}

/**
  This function sets up, Current Sensor Registers for each Target LVR

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  Target        - Target SelLvr setting, see enum define
**/
VOID
MrcCurrentSensorTargetSetup (
  IN MrcParameters    *const MrcData,
  IN SelLvrSetting    Target
  )
{
  const MRC_FUNCTION *MrcCall;
  MrcInput      *Inputs;
  MrcOutput     *Outputs;
  INT64         GetSetVal;
  INT64         GetSetEn;
  INT64         CurrentSensorDuration;
  INT64         SelLvr;
  UINT32        PartitionIndex;
  UINT32        PartitionNum;
  UINT32        PartitionMax;
  UINT32        PartitionNumMax;
  UINT32        Gear4;
  UINT32        PHClk;
  UINT32        QClk;
  UINT32        SBClock;
  UINT32        BurstLengthQclk;
  UINT32        BGMode;
  UINT32        Multiplier;
  BOOLEAN       DtHalo;
  BOOLEAN       LvrCompTarget;
  UINT16        PartitionMask [LVR_SENSOR_PARTITIONS];
  PARTITION_TYPE    Partition [] = {PartitionDataShared, PartitionCccShared}; // LVR_SENSOR_PARTITIONS

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  DtHalo = Inputs->IsDdrIoDtHalo;
  PartitionMax = ARRAY_COUNT (Partition);
  MrcCall->MrcSetMem ((UINT8 *) PartitionMask, sizeof (PartitionMask), 0);

  Gear4           = Outputs->GearMode ? 1 : 0;
  PHClk           = DIVIDECEIL (FREQ_TO_TCK_PS, Outputs->Frequency);
  QClk            = Gear4 ? (PHClk * 2) : PHClk;
  SBClock         = 1000000 / 400;
  BurstLengthQclk = Outputs->BurstLength * (Outputs->IsLpddr5 ? 4 : 1) / (1 + Gear4);
  // LPDDR5 is in BGMode or not
  BGMode = (Outputs->IsLpddr5 && (MrcGetBankBgOrg (MrcData, Outputs->Frequency) == MrcLp5BgMode)) ? 1 : 0;
  Multiplier = 10;

  GetSetEn = 1;
  SelLvr   = 0;
  LvrCompTarget = FALSE;
  PartitionNumMax = MRC_DATA_DT_NUM;

  // Measure current for the duration of a CPGC RMT Test.
  // RndDown(Log(CPGC_NumCachelines* BurstLengthInQClk*(1+0.5*BGMode)*QClk/SBClock, 2))
  CurrentSensorDuration = UDIVIDEROUND (CPGC_RMT_CL_NUMBER * QClk * BurstLengthQclk * (Multiplier * (2 + BGMode) / 2), Multiplier);
  CurrentSensorDuration = UDIVIDEROUND ((UINT32) CurrentSensorDuration, SBClock);
  CurrentSensorDuration = (INT64) MrcLog2RoundDown (MrcData, ((UINT32) CurrentSensorDuration));

  // Current Sensor Setup Registers
  switch (Target) {
    case SelLvrVccIog:
      PartitionMask[DATA_MASK]  = CUR_SENS_SEL_LVR_VCCIOG_DATA_MASK;
      PartitionMask[CCC_MASK]   = DtHalo ? CUR_SENS_SEL_LVR_VCCIOG_DT_CCC_MASK : CUR_SENS_SEL_LVR_VCCIOG_CCC_MASK;
      break;

    case SelLvrVccClk:
      PartitionMask[DATA_MASK]  = DtHalo ? (CUR_SENS_SEL_LVR_VCCCLK_DT_DATA_MASK | CUR_SENS_SEL_LVR_NBIASFF_DT_DATA_MASK) : CUR_SENS_SEL_LVR_NBIASFF_DATA_MASK;
      PartitionMask[CCC_MASK]   = DtHalo ? CUR_SENS_SEL_LVR_VCCCLK_DT_CCC_MASK  : CUR_SENS_SEL_LVR_VCCCLK_CCC_MASK;
      LvrCompTarget = DtHalo ? TRUE : FALSE;
      break;

    case SelLvrVccDdq:
      PartitionMask[DATA_MASK]  = DtHalo ? CUR_SENS_SEL_LVR_VCCDDQ_DT_DATA_MASK : CUR_SENS_SEL_LVR_VCCDDQ_DATA_MASK;
      PartitionMask[CCC_MASK]   = DtHalo ? CUR_SENS_SEL_LVR_VCCDDQ_DT_CCC_MASK : CUR_SENS_SEL_LVR_VCCDDQ_CCC_MASK;
      LvrCompTarget = DtHalo ? FALSE : TRUE;
      break;

    default:
      break;
  }

  SelLvr = (UINT32) (Target + 1);

  for (PartitionIndex = 0; PartitionIndex < PartitionMax; PartitionIndex++) {
    for (PartitionNum = 0; PartitionNum < PartitionNumMax; PartitionNum++) {
      if ((PartitionMask[PartitionIndex] & (1 << PartitionNum)) == 0) {
        continue;
      }
      GetSetVal = 2;
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrCurrentSensorCntStart, WriteToCache, &GetSetVal);
      GetSetVal = 14;
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrCurrentSensorAmpHyster, WriteToCache, &GetSetVal);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrCurrentSensorEnChop, WriteToCache, &GetSetEn);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrCurrentSensorEnSensor, WriteToCache, &GetSetEn);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrCurrentSensorDuration, WriteToCache, &CurrentSensorDuration);
      MrcGetSetPartitionBlock (MrcData, Partition[PartitionIndex], PartitionNum, GsmLvrCurrentSensorSelLVR, WriteToCache, &SelLvr);
    } // PartitionNum
  } // PartitionIndex

  if (LvrCompTarget) {
    GetSetVal = 2;
    MrcGetSetIndex (MrcData, MRC_COMP_NUM, GsmCompLvrCurrentSensorCntStart, WriteToCache, &GetSetVal);
    GetSetVal = 14;
    MrcGetSetIndex (MrcData, MRC_COMP_NUM, GsmCompLvrCurrentSensorAmpHyster, WriteToCache, &GetSetVal);
    MrcGetSetIndex (MrcData, MRC_COMP_NUM, GsmCompLvrCurrentSensorEnChop, WriteToCache, &GetSetEn);
    MrcGetSetIndex (MrcData, MRC_COMP_NUM, GsmCompLvrCurrentSensorEnSensor, WriteToCache, &GetSetEn);
    MrcGetSetIndex (MrcData, MRC_COMP_NUM, GsmCompLvrCurrentSensorDuration, WriteToCache, &CurrentSensorDuration);
    MrcGetSetIndex (MrcData, MRC_COMP_NUM, GsmCompLvrCurrentSensorSelLVR, WriteToCache, &SelLvr);
  }

  // FF Current Sensor
  GetSetVal = 1;
  MrcGetSetPartitionBlock (MrcData, PartitionCcc, MRC_CCC_NUM, GsmCccVccClkFFCurrentSensorCntStart, WriteToCache, &GetSetVal);
  MrcGetSetPartitionBlock (MrcData, PartitionCcc, MRC_CCC_NUM, GsmCccVccClkFFCurrentSensorDuration, WriteToCache, &CurrentSensorDuration);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmDataVccClkFFCurrentSensorCntStart, WriteToCache, &GetSetVal);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmDataVccClkFFCurrentSensorDuration, WriteToCache, &CurrentSensorDuration);
  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function sets up Point Test through all targets for Voltage Sensor

  @param[in]  MrcData  - Pointer to global MRC data.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcVoltageSensorSetup (
  IN MrcParameters    *const MrcData
  )
{
  SelLvrSetting  LvrTarget;

  for (LvrTarget = SelLvrVccIog; LvrTarget < SelLvrVccMax; LvrTarget++) {
    MrcVoltageSensorTargetSetup (MrcData, LvrTarget);
  }
  return mrcSuccess;
}

/**
  This function stes up Current Sensor registers setup for all Target LVR

  @param[in]  MrcData - Pointer to global MRC data.
**/
VOID
MrcCurrentSensorSetup (
  IN MrcParameters  *const MrcData
  )
{
  SelLvrSetting  LvrTarget;

  for (LvrTarget = SelLvrVccIog; LvrTarget < SelLvrVccMax; LvrTarget++) {
    MrcCurrentSensorTargetSetup (MrcData, LvrTarget);
  }
}

/**
  This function enables Current Sensor counter for all Target LVR
  and selects the Test Case to run for FF Current Sensor

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  TestNum - Select Test Case for FF Current Sensor, see enum define
                        Test cases available:
                          Test Case 0    ,   Test Case 1 ,    Test Case 2
                        {DATA_FFPH_DRV_PI, DATA_FFPI_DATA, DATA_FFPH_DRV_G}
                        {DATA_FFPI_READ,   DATA_FFRX_DRV,  DATA_FFPH_DRV_G}
                        {CCC_FFPH_DRV_PI,  CCC_FFPI_CCC,   CCC_FFPH_DRV_G }
**/
VOID
MrcCurrentSensorEnable (
  IN MrcParameters  *const MrcData,
  IN UINT8           TestNum
  )
{
  MrcOutput     *Outputs;
  INT64         GetSetVal;
  INT64         GetSetEn;
  UINT8         SelCntData0Array [] = {DATA_FFPH_DRV_PI, DATA_FFPI_DATA, DATA_FFPH_DRV_G};
  UINT8         SelCntData1Array [] = {DATA_FFPI_READ,   DATA_FFRX_DRV,  DATA_FFPH_DRV_G};
  UINT8         SelCntCCCArray   [] = {CCC_FFPH_DRV_PI,  CCC_FFPI_CCC,   CCC_FFPH_DRV_G };
  UINT8         Controller;
  UINT8         Channel;
  UINT8         Byte;
  UINT8         PartitionByte;
  UINT8         MaxChannels;
  UINT32        PartitionNum;
  UINT32        PartitionNumMax;
  UINT32        CccPartitionNumMax;
  BOOLEAN       DtHalo;

  Outputs = &MrcData->Outputs;
  DtHalo = MrcData->Inputs.IsDdrIoDtHalo;
  GetSetEn = 1;
  MaxChannels = Outputs->MaxChannels;
  PartitionNumMax = MRC_DATA_DT_NUM;
  CccPartitionNumMax = (DtHalo) ? MRC_CCC_SHARED_DT_NUM : MRC_CCC_SHARED_MOBILE_NUM;

  // DDRCCC_DdrCrVccCLKFFCurrentSensor.SelCnt = SelCntCCCArray[TestNum]
  GetSetVal = SelCntCCCArray[TestNum];
  MrcGetSetPartitionBlock (MrcData, PartitionCcc, MRC_CCC_NUM, GsmCccVccClkFFCurrentSensorSelCnt, WriteToCache, &GetSetVal);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
        // Map this channel/byte to a physical partition
        MrcCalcDataPartition (MrcData, Controller * MaxChannels + Channel, Byte, &PartitionByte);
        // DDRData[Byte0]_DdrCrVccCLKFFCurrentSensor.SelCnt = SelCntData0Array[TestNum]
        // DDRData[Byte1]_DdrCrVccCLKFFCurrentSensor.SelCnt = SelCntData1Array[TestNum]
        GetSetVal = (PartitionByte == 0) ? SelCntData0Array[TestNum] : SelCntData1Array[TestNum];
        MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmDataVccClkFFCurrentSensorSelCnt, WriteToCache, &GetSetVal);
      } // Byte
    } // Channel
  } // Controller

  MrcFlushRegisterCachedData (MrcData);

  for (PartitionNum = 0; PartitionNum < PartitionNumMax; PartitionNum++) {
    MrcGetSetPartitionBlock (MrcData, PartitionDataShared, PartitionNum, GsmLvrCurrentSensorEnCnt, WriteCached, &GetSetEn); // Data LVR Counter Enable
    if (PartitionNum < CccPartitionNumMax) {
      MrcGetSetPartitionBlock (MrcData, PartitionCccShared, PartitionNum, GsmLvrCurrentSensorEnCnt, WriteCached, &GetSetEn); // CCC LVR Counter Enable
    }
  }
  MrcGetSetIndex (MrcData, MRC_COMP_NUM, GsmCompLvrCurrentSensorEnCnt, WriteCached, &GetSetEn); // Comp LVR Counter Enable
  MrcGetSetPartitionBlock (MrcData, PartitionCcc, MRC_CCC_NUM, GsmCccVccClkFFCurrentSensorEnCnt, WriteCached, &GetSetEn); // CCC VccClkFF Counter Enable
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmDataVccClkFFCurrentSensorEnCnt, WriteCached, &GetSetEn); // Data VccClkFF Counter Enable
}

/**
  This function reads Current Sensor counter for all Target LVR and FF Current Sensor
  to calculate Analog Power and FF Power and prints out results for each test case.

  @param[in]  MrcData - Pointer to global MRC data.
**/
VOID
MrcCurrentSensorCounter (
  IN MrcParameters  *const MrcData
  )
{
                          // Array is PhDrvPi, PiData, PIRead, RxDrv, PhDrvG, PiCCC
    static const UINT32  Wstatic[] = {   6,      16,     24,     34,   193,    28};
    static const UINT32  Wdyn[]    = { 250,       1,      1,      1,     4,     1};
                                     // Array is  0:None, 1:VccIOG, 2:VccDDQ, 3:VccCLK,NbiasFF, 4:VccDLL[0], 5:VccDLL[1]
    static const INT8   CurrentSensorLvrMap[] = {  -1,     0,        1,        4,                3,           2};
    const MrcInput      *Inputs;
    const MRC_FUNCTION  *MrcCall;
    MrcOutput           *Outputs;
    MrcDebug            *Debug;
    INT8                SelLVR;
    INT8                Index;
    INT64               GetSetVal;
    INT64               GetSetDuration;
    INT64               GetSetCount;
    INT64               GetSetSelLvr;
    INT64               GetSetSelCnt;
    INT32               Coeff[COEFF_NUM];
    INT32               Coefficients;
    INT32               Scale;
    INT32               Multiplier;
    INT32               FBTotal[FB_PARTITION_MAX];
    INT32               FBNum[FB_PARTITION_MAX];
    UINT8               Controller;
    UINT8               Channel;
    UINT8               Byte;
    UINT8               Partition;
    UINT8               MaxChannels;
    UINT8               PartitionByte;
    UINT32              ICcc;
    UINT32              PartitionNum;
    UINT32              PartitionNumMax;
    UINT32              CccPartitionNumMax;
    UINT32              FFTotal[FF_PARTITION_MAX];
    UINT32              FFCode[FF_CODE_MAX];
    UINT32              Wleg;
    BOOLEAN             DtHalo;

    Outputs = &MrcData->Outputs;
    Debug = &Outputs->Debug;
    Inputs = &MrcData->Inputs;
    MrcCall = Inputs->Call.Func;
    MrcCall->MrcSetMem ((UINT8 *) FBTotal, sizeof (FBTotal), 0);
    MrcCall->MrcSetMem ((UINT8 *) FFTotal, sizeof (FFTotal), 0);
    MrcCall->MrcSetMem ((UINT8 *) FBNum, sizeof (FBNum), 0);
    MrcCall->MrcSetMem ((UINT8 *) Coeff, sizeof (Coeff), 0);
    DtHalo = Inputs->IsDdrIoDtHalo;
    PartitionNumMax = (DtHalo) ? MRC_DATA_DT_NUM : MRC_DATA_MOBILE_NUM;
    CccPartitionNumMax = (DtHalo) ? MRC_CCC_SHARED_DT_NUM : MRC_CCC_SHARED_MOBILE_NUM;
    MaxChannels = Outputs->MaxChannels;
    Index = -1;
    Multiplier = 1000000000;
    ICcc = 0;
    Scale = 1;
    Coefficients = 0;

    MrcGetSetNoScope (MrcData, GsmFFCodePhDrvG, ReadCached, &GetSetVal);
    FFCode[PhDrvG] = (UINT32) GetSetVal;

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n\t\t\tVccIOG\t\t\tVccDDQ\t\t\tVccCLK\t\t\tVccDLL[0]\t\t\tVccDLL[1]\t(mA)\n");
    // Current Sensor Read Report
    for (PartitionNum = 0; PartitionNum < PartitionNumMax; PartitionNum++) {
      MrcGetSetPartitionBlock (MrcData, PartitionDataShared, PartitionNum, GsmLvrCurrentSensorDuration, ReadCached, &GetSetDuration);
      // 2^( DdrCrLVRCurrentSensor.Duration-14) where Coeff values assume a duration of 2^14
      GetSetDuration -= COEFF_DURATION;
      Scale = (INT32) (GetSetDuration < 0 ? 1 : (1 << GetSetDuration));
      // Coeff array is [1.9e-07/Scale^2, 0.02/Scale, -0.123]
      Coeff[0] = 190 / (Scale * Scale);
      Coeff[1] = 20000000 / Scale;
      Coeff[2] = -123000000;
      MrcGetSetPartitionBlock (MrcData, PartitionDataShared, PartitionNum, GsmLvrCurrentSensorCount, ReadCached, &GetSetCount);
      MrcGetSetPartitionBlock (MrcData, PartitionDataShared, PartitionNum, GsmLvrCurrentSensorSelLVR, ReadCached, &GetSetSelLvr);
      SelLVR = CurrentSensorLvrMap[(INT8) GetSetSelLvr];
      // Based on MTS_MAS_100 Table 2 (MP): Scale Data by 4x
      Scale = 4;
      if (SelLVR >= 0) {
        // Calculate PartitionDataShared FBTotal[SelLVR] += Scale * (Coeff[0]*Count^2 + Coeff[1]*Count + Coeff[2])
        Coefficients = DIVIDEROUND ((Coeff[0] * (((INT32) GetSetCount) * ((INT32) GetSetCount)) + Coeff[1] * ((INT32) GetSetCount) + Coeff[2]), Multiplier);
        FBTotal[(INT32) GetSetSelLvr - 1] += Scale * (Coefficients);  // Result in mA
        FBNum[(INT32) GetSetSelLvr - 1] += Scale;
      }
      if (PartitionNum < CccPartitionNumMax) {
        MrcGetSetPartitionBlock (MrcData, PartitionCccShared, PartitionNum, GsmLvrCurrentSensorCount, ReadCached, &GetSetCount);
        MrcGetSetPartitionBlock (MrcData, PartitionCccShared, PartitionNum, GsmLvrCurrentSensorSelLVR, ReadCached, &GetSetSelLvr);
        SelLVR = CurrentSensorLvrMap[(INT8) GetSetSelLvr];
        // Scale CCC by 2x unless its measuring VccClk (Scale by 1x)
        Scale = ((GetSetSelLvr - 1) == SelLvrVccClk) ? 1 : 2;
        if (SelLVR >= 0) {
          // Calculate PartitionCccShared FBTotal[SelLVR] += Scale * (Coeff[0]*Count^2 + Coeff[1]*Count + Coeff[2])
          Coefficients = DIVIDEROUND ((Coeff[0] * (((INT32) GetSetCount) * ((INT32) GetSetCount)) + Coeff[1] * ((INT32) GetSetCount) + Coeff[2]), Multiplier);
          FBTotal[(INT32) GetSetSelLvr - 1] += Scale * (Coefficients);  // Result in mA
          FBNum[(INT32) GetSetSelLvr - 1] += Scale;
        }
      } // CccPartitionNumMax
    } // PartitionNum
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Analog\t\t\t%d\t\t\t%d\t\t\t%d\t\t\t%d\t\t\t%d\n", FBTotal[0], FBTotal[1], FBTotal[2], FBTotal[3], FBTotal[4]);

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n\t\t\tData_B0\t\t\tData_B1\t\t\tCCC\t(mA)\n");
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        if (!MrcChannelExist (MrcData, Controller, Channel)) {
          continue;
        }
        for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
          // Map this channel/byte to a physical partition
          Partition = MrcCalcDataPartition (MrcData, Controller * MaxChannels + Channel, Byte, &PartitionByte);
          MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmDataVccClkFFCurrentSensorCount, ReadCached, &GetSetCount);
          MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmDataVccClkFFCurrentSensorSelCnt, ReadCached, &GetSetSelCnt);
          // Wleg = FFCode*Wdyn + Wstatic
          Wleg = DIVIDECEIL((FFCode[(UINT32) GetSetSelCnt] * Wdyn[(UINT32) GetSetSelCnt] + Wstatic[(UINT32) GetSetSelCnt]), WREF_LEGS_NUM);
          FFTotal[(UINT32) PartitionByte] += ((UINT32) GetSetCount) * Wleg;
          if (Index < ((INT8) Partition)) {
            Index = (INT8) Partition;
            MrcGetSetPartitionBlock (MrcData, PartitionCcc, (UINT32) Index, GsmCccVccClkFFCurrentSensorCount, ReadCached, &GetSetCount);
            MrcGetSetPartitionBlock (MrcData, PartitionCcc, (UINT32) Index, GsmCccVccClkFFCurrentSensorSelCnt, ReadCached, &GetSetSelCnt);
            ICcc = (UINT32) (GetSetSelCnt + (4 * (GetSetSelCnt == CCC_FFPI_CCC)) + (2 * (GetSetSelCnt == CCC_FFPH_DRV_G)));
            Wleg = DIVIDECEIL((FFCode[ICcc] * Wdyn[ICcc] + Wstatic[ICcc]), WREF_LEGS_NUM);
            FFTotal[PI_CCC] = ((UINT32) GetSetCount) * Wleg;
          }
        } // Byte
      } // Channel
    } // Controller
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "VccClkFF\t\t\t%d\t\t\t%d\t\t\t%d\n", FFTotal[0], FFTotal[1], FFTotal[2]);
}

/**
  This function calculates the LVR DAC Encode from input voltage

  @param[in]  - Target voltage value
  @param[in]  - Bandgap reference
  @param[in]  - OCDivider reference
  @param[in]  - Multiplier to handle voltage passed-in scaling
  @retval Returns LVR DAC Encode
**/
UINT32
MrcDACEncode (
  IN UINT32        Vref,
  IN UINT32        BGRef,
  IN BOOLEAN       OCDivider,
  IN UINT32        Multiplier
  )
{
  UINT32    TempVar;

  TempVar = MAX (UDIVIDEROUND (Vref * 386, (BGRef * (OCDivider ? (UINT32) (15 * (Multiplier/10)) : Multiplier)) / Multiplier) - Multiplier, 0);
  TempVar = MIN (UDIVIDEROUND (TempVar, Multiplier), 383);

  return TempVar;
}

/**
  This function calculates the VTarget for all 3 target (VccIOG, VccDDQ and VccCLK)
  it supports default voltage tolerance only

  @param[in, out] MrcData - All the MRC global data.
  @param[in]              - Target voltage value (0: view0, 1: view1)
  @param[in]              - Target Rail (0: VccIOG, 1: VccDDQ, 3: VccClk)
  @retval Returns VTarget
**/
UINT32
GetVTarget (
  IN MrcParameters *const MrcData,
  IN UINT32               TargetView,
  IN UINT32               TargetRail
)
{
  MrcOutput     *Outputs;
  UINT32        SensorOffset;
  UINT32        DcLvrError;
  UINT32        Target;
  UINT32        VTarget;
  UINT32        Multiplier;
  UINT32        Vtol1;
  UINT32        Vtol2;

  Outputs      = &MrcData->Outputs;
  SensorOffset = 8;
  DcLvrError   = 22;
  Multiplier   = 100;

  //Vtol1 = (IamIOG ? 0.03 : (IamDDQ ? 0.04 : (IamCLK ? 0.02 ? Err) ) )
  switch (TargetRail) {
    case 0: //VccIOG
      Vtol1 = 3;
      Target = Outputs->VccIogVoltage;
      break;
    case 1: //VccDDQ
      Vtol1 = 4;
      Target = Outputs->VccddqVoltage;
      break;
    case 2: //VccCLK
      Vtol1 = 2;
      Target = Outputs->VccClkVoltage;
      break;
    default:
      return 0;
  }

  //Vtol2 = (22mV DC LVR Error + 8mV Sensor Offset) / (1000 mV BGRef / 386)
  //Multiply by 100 since Vtol1 is multiply by 100
  //MrcDACEncode function includes DAC factor (1000 mV BGRef / 386) for Volt2
  Vtol2 = Multiplier * (DcLvrError + SensorOffset);

  //Target0 = Vnom * (1-Vtol1) - Vtol2
  //Target1 = Vnom * (1+Vtol1) + Vtol2
  if (TargetView == 0) {
    VTarget = MrcDACEncode ((((Multiplier - Vtol1) * Target) - Vtol2), BG_REF_MV, 0, Multiplier);
  } else {
    VTarget = MrcDACEncode ((((Multiplier + Vtol1) * Target) + Vtol2), BG_REF_MV, 0, Multiplier);
  }

  return VTarget;
}

/**
  Calculate VrefSel for VccDist according to the formula
  in PHY MRC User Guide Calibration Spec.

  @param[in] MrcData       - All the MRC global data.
  @param[in] SelVdd2Ladder - SelVdd2Ladder value that was programmed in WP register
  @param[in] VccTarg       - Target voltage.

  @retval VrefSel value
**/
UINT32
CalculateVrefSelVccDist (
  IN MrcParameters *const MrcData,
  IN UINT32        SelVdd2Ladder,
  IN UINT32        VccTarg
  )
{
  MrcOutput *Outputs;

  UINT32 VccRef, VrefSel;

  Outputs = &MrcData->Outputs;

  // VccRef = SELVDD2LADDER ? VccDD2 : 0.8
  // LVR Target Code = (2/3) * VccTarg * 192 / VccRef
  VccRef = SelVdd2Ladder ? Outputs->Vdd2Mv : BG_REF_MV;
  VrefSel = UDIVIDEROUND (2 * VccTarg * 192, 3 * VccRef);

  return VrefSel;
}

/**
  Calculate VrefSel for VccClk* and VccIog* according to the formula
  in PHY MRC User Guide Calibration Spec.

  @param[in] MrcData       - All the MRC global data.
  @param[in] SelVdd2Ladder - SelVdd2Ladder value that was programmed in WP register
  @param[in] VccTarg       - Target voltage.

  @retval VrefSel value
**/
UINT32
CalculateVrefSel (
  IN MrcParameters *const MrcData,
  IN UINT32        SelVdd2Ladder,
  IN UINT32        VccTarg
  )
{
  MrcOutput *Outputs;

  UINT32 VccRef, VrefSel;

  Outputs = &MrcData->Outputs;

  // VccRef = SELVDD2LADDER ? VccDD2 : 0.8
  // LVR Target Code = (4/5) * VccTarg * 192 / VccRef
  VccRef = SelVdd2Ladder ? Outputs->Vdd2Mv : BG_REF_MV;
  VrefSel = UDIVIDEROUND (4 * VccTarg * 192, 5 * VccRef);

  return VrefSel;
}

/**
  This function programs the Vcc LVR registers

  @param[in, out] MrcData - All the MRC global data.
  @retval Returns none
**/
VOID
SetVccLvr (
  IN MrcParameters *const MrcData
 )
{
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  MrcSaveData       *SaveData;
  MrcSaGvPoint      SaGvPoint;
  MrcLvrSaveRestore *LvrSaveRestorePtr;
  UINT32            Offset;
  UINT32            TempVar1;
  UINT32            TempVar2;
  UINT32            VrefSel;
  UINT32            VrefSelClk;
  UINT32            VrefSelIog;
  BOOLEAN           EnOverclockingForLVR;
  UINT32            VccClk;
  UINT32            VccIog;
  UINT32            Index;
  UINT32            SBClock;
  BOOLEAN           IsLpddr5;
  UINT32            SelVdd2LadderVccClk;
  UINT32            SelVdd2LadderVccIog;
  BOOLEAN           IsColdBoot;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_STRUCT VccIogControl;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_STRUCT VccDdqControl;
  DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT       RptChCrMisc;
  DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_STRUCT           WorkPoint0;
  DDRCCC_SBMEM_CR_PM_MISC_CTRL_STRUCT                 PmMiscCtrl;
  DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT            DllCbTune0;
  DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_STRUCT              PmaOvrd;
  DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_STRUCT               PmaOvrd3;
  DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT            DataVccClkWp0;
  DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT             CccVccClkWp0;
  DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT            DataVccIogWp0;
  DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT             CccVccIogWp0;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP0_STRUCT    CompVccIog40Wp0;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP0_STRUCT      CompVccIogWp0;
  DDRPHY_MISC_SAUG_CR_LVR_CTRL1_STRUCT                LvrCtrl1;
  DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_STRUCT       PllFuseVirtual;
  DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT                 DdrCrVssHi;
  DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_STRUCT              VccClkPmMiscCtrl;

  Inputs            = &MrcData->Inputs;
  Outputs           = &MrcData->Outputs;
  SaveData          = &MrcData->Save.Data;
  IsLpddr5          = Outputs->IsLpddr5;
  SaGvPoint         = Outputs->SaGvPoint;
  VccClk            = Outputs->VccClkVoltage;
  VccIog            = Outputs->VccIogVoltage;
  LvrSaveRestorePtr = &SaveData->LvrSaveRestore[SaGvPoint];
  Offset            = 0;
  EnOverclockingForLVR = (Outputs->IsDdr5 && (Outputs->Frequency > f6400));
  SBClock              = 1000000 / 400;
  IsColdBoot = (Inputs->BootMode == bmCold);

  // vcciog/vccclk: SELVDD2LADDER = (VccTargMv + 50 < 1000) ? 0 : 1 ## VccTargMv can be as high as 800 * 5/4 = 1000 before Vref Saturation
  // Using 50mV guardband 
  SelVdd2LadderVccClk = (VccClk + 50 < 1000) ? 0 : 1;
  SelVdd2LadderVccIog = (VccIog + 50 < 1000) ? 0 : 1;
  VrefSelClk = CalculateVrefSel (MrcData, SelVdd2LadderVccClk, VccClk);
  VrefSelIog = CalculateVrefSel (MrcData, SelVdd2LadderVccIog, VccIog);
  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "clklvr_vrefsel: %u\n, ioglvr_vrefsel: %u\n",  VrefSelClk, VrefSelIog);

  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP1_REG, SaGvPoint);
  VccIogControl.Data = MrcReadCR (MrcData, Offset);

  VccIogControl.Bits.Disable = 0;
  VccIogControl.Bits.RemoteSense = 0;
  VccIogControl.Bits.OCDivider   = EnOverclockingForLVR ? 1 : 0;
  // Min(383, Rnd( VccIog/(BGRef/386*IF(OCDivider,1.5,1)) -1) )
  TempVar1 = (VccIogControl.Bits.OCDivider ? 1500 : 1000);
  TempVar2 = UDIVIDEROUND (THOUSAND_MULTIPLIER * VccIog * MRC_COMP_VREF_CALCULATION_VALUE, TempVar1 * BG_REF_MV) - 1;
  VccIogControl.Bits.Target = MIN (383, TempVar2);
  VccIogControl.Bits.BiasAdj   = 7;
  VccIogControl.Bits.AdaptiveZeroAdj1 = 2;
  VccIogControl.Bits.AdaptiveZeroAdj2 = 12;
  TempVar1 = 80000 / (16 * SBClock);
  TempVar2 = MrcLog2Round (MrcData, TempVar1);
  VccIogControl.Bits.LockTimer = RANGE (TempVar2, 0, 7);
  VccIogControl.Bits.LvrPM = 1;
  MrcWriteCR (MrcData, Offset, VccIogControl.Data);

  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP1_REG, SaGvPoint);
  VccDdqControl.Data = MrcReadCR (MrcData, Offset);
  VccDdqControl.Bits.Disable = 0;
  //VccDDQ is always bypassed in DDR5 mode
  VccDdqControl.Bits.Bypass = (Outputs->IsDdr5) ? 1 : 0;
  VccDdqControl.Bits.RemoteSense = 0;
  VccDdqControl.Bits.OCDivider   = EnOverclockingForLVR ? 1 : 0;
  VccDdqControl.Bits.Target = GetVddqControlTarget (MrcData, Outputs->VccddqVoltage);
  VccDdqControl.Bits.BiasAdj = 7;
  VccDdqControl.Bits.AdaptiveZeroAdj1 = 2;
  VccDdqControl.Bits.AdaptiveZeroAdj2 = 12;
  TempVar1 = 80000 / (16 * SBClock);
  TempVar2 = MrcLog2Round (MrcData, TempVar1);
  VccDdqControl.Bits.LockTimer = RANGE (TempVar2, 0, 7);
  VccDdqControl.Bits.LvrPM = 1;
  MrcWriteCR (MrcData, Offset, VccDdqControl.Data);

  // Override Work Point update fields to let the settings propagate into LVR
  PmaOvrd.Data = MrcReadCR (MrcData, DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG);
  PmaOvrd.Bits.WorkPointUpdateOvrd = 1;
  PmaOvrd.Bits.WorkPointUpdateOvrdVal = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG, PmaOvrd.Data);

  PmaOvrd3.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG);
  PmaOvrd3.Bits.IOGLVRWorkPointUpdateOvrdEn = 1;
  PmaOvrd3.Bits.IOGLVRWorkPointUpdateOvrdVal = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG, PmaOvrd3.Data);

  LvrCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG);
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrdEn = 1;
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrd = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG, LvrCtrl1.Data);

  // VccDist
  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_REG, DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT1_REG, SaGvPoint);
  WorkPoint0.Data = MrcReadCR (MrcData, Offset);

  // vccdist: SELVDD2LADDER = (VccTargMv + 50 < 1200) ? 0 : 1 ## VccTargMv can be as high as 800 * 3/2 = 1200 before Vref Saturation
  // Using 50mV guardband 
  WorkPoint0.Bits.DISTGLVRSelVdd2Ladder = (VccClk + 50 < 1200) ? 0 : 1; // 1: for Vccdd2_hv, 0: for Bgvref (0.8V)

  VrefSel = CalculateVrefSelVccDist (MrcData, WorkPoint0.Bits.DISTGLVRSelVdd2Ladder, VccClk);
  if (IsColdBoot) {
    WorkPoint0.Bits.DISTGLVRVrefSel = VrefSel;
    LvrSaveRestorePtr->WorkPointDISTGLVRVrefSel = (UINT8) WorkPoint0.Bits.DISTGLVRVrefSel;
  } else {
    WorkPoint0.Bits.DISTGLVRVrefSel = LvrSaveRestorePtr->WorkPointDISTGLVRVrefSel;
  }
  WorkPoint0.Bits.Gear4 = Outputs->GearMode ? 1 : 0;
  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "DISTGLVRVrefSel: %u\n", WorkPoint0.Bits.DISTGLVRVrefSel);
  MrcWriteCR (MrcData, Offset, WorkPoint0.Data);

  for (Index = 0; Index < MRC_DATA_MOBILE_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    // VccClk Data
    Offset = OFFSET_CALC_MC_CH (DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG, DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP0_REG, Index,
                                DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP1_REG, SaGvPoint);
    DataVccClkWp0.Data = MrcReadCR (MrcData, Offset);
    DataVccClkWp0.Bits.clklvr_selvdd2_ladder = SelVdd2LadderVccClk;
    if (IsColdBoot) {
      DataVccClkWp0.Bits.clklvr_vrefsel        = VrefSelClk;
      DataVccClkWp0.Bits.clklvr_rxdllb1vrefsel = VrefSelClk;
      DataVccClkWp0.Bits.clklvr_rxdllb0vrefsel = VrefSelClk;
      LvrSaveRestorePtr->DataVccClkWpLvrVrefSel[Index]        = (UINT8) DataVccClkWp0.Bits.clklvr_vrefsel;
      LvrSaveRestorePtr->DataVccClkWpLvrRxdllb1vrefsel[Index] = (UINT8) DataVccClkWp0.Bits.clklvr_rxdllb1vrefsel;
      LvrSaveRestorePtr->DataVccClkWpLvrRxdllb0vrefsel[Index] = (UINT8) DataVccClkWp0.Bits.clklvr_rxdllb0vrefsel;
    } else {
      DataVccClkWp0.Bits.clklvr_vrefsel        = LvrSaveRestorePtr->DataVccClkWpLvrVrefSel[Index];
      DataVccClkWp0.Bits.clklvr_rxdllb1vrefsel = LvrSaveRestorePtr->DataVccClkWpLvrRxdllb1vrefsel[Index];
      DataVccClkWp0.Bits.clklvr_rxdllb0vrefsel = LvrSaveRestorePtr->DataVccClkWpLvrRxdllb0vrefsel[Index];
    }
    MrcWriteCR (MrcData, Offset, DataVccClkWp0.Data);

    // VccIog Data
    Offset = OFFSET_CALC_MC_CH (DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG, DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP0_REG, Index,
                                DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP1_REG, SaGvPoint);
    DataVccIogWp0.Data = MrcReadCR (MrcData, Offset);
    DataVccIogWp0.Bits.ioglvr_selvdd2_ladder = SelVdd2LadderVccIog;
    if (IsColdBoot) {
      DataVccIogWp0.Bits.ioglvr_vrefsel = VrefSelIog;
      LvrSaveRestorePtr->DataVccIogWpLvrVrefSel[Index] = (UINT8) DataVccIogWp0.Bits.ioglvr_vrefsel;
    } else {
      DataVccIogWp0.Bits.ioglvr_vrefsel = LvrSaveRestorePtr->DataVccIogWpLvrVrefSel[Index];
    }
    MrcWriteCR (MrcData, Offset, DataVccIogWp0.Data);
  }

  for (Index = 0; Index < MRC_CCC_SHARED_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    // VccClk CCC
    Offset = OFFSET_CALC_MC_CH (DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG, DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP0_REG, Index,
                                DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP1_REG, SaGvPoint);
    CccVccClkWp0.Data = MrcReadCR (MrcData, Offset);
    CccVccClkWp0.Bits.clklvr_selvdd2_ladder = SelVdd2LadderVccClk;
    if (IsColdBoot) {
      CccVccClkWp0.Bits.clklvr_vrefsel = VrefSelClk;
      LvrSaveRestorePtr->CccVccClkWpLvrVrefSel[Index] = (UINT8) CccVccClkWp0.Bits.clklvr_vrefsel;
    } else {
      CccVccClkWp0.Bits.clklvr_vrefsel = LvrSaveRestorePtr->CccVccClkWpLvrVrefSel[Index];
    }
    MrcWriteCR (MrcData, Offset, CccVccClkWp0.Data);

    // VccIog CCC
    Offset = OFFSET_CALC_MC_CH (DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP0_REG, DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP0_REG, Index,
                                DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP1_REG, SaGvPoint);
    CccVccIogWp0.Data = MrcReadCR (MrcData, Offset);
    CccVccIogWp0.Bits.ioglvr_selvdd2_ladder = SelVdd2LadderVccIog;
    if (IsColdBoot) {
      CccVccIogWp0.Bits.ioglvr_vrefsel = VrefSelIog;
      LvrSaveRestorePtr->CccVccIogWpLvrVrefSel[Index] = (UINT8) CccVccIogWp0.Bits.ioglvr_vrefsel;
    } else {
      CccVccIogWp0.Bits.ioglvr_vrefsel = LvrSaveRestorePtr->CccVccIogWpLvrVrefSel[Index];
    }
    MrcWriteCR (MrcData, Offset, CccVccIogWp0.Data);
  }

  // VccIog COMP
  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP1_REG, SaGvPoint);
  CompVccIogWp0.Data = MrcReadCR (MrcData, Offset);
  CompVccIogWp0.Bits.ioglvr_selvdd2_ladder = SelVdd2LadderVccIog;
  if (IsColdBoot) {
    CompVccIogWp0.Bits.ioglvr_vrefsel = VrefSelIog;
    LvrSaveRestorePtr->CompVccIogWpLvrVrefSel = (UINT8) CompVccIogWp0.Bits.ioglvr_vrefsel;
  } else {
    CompVccIogWp0.Bits.ioglvr_vrefsel = LvrSaveRestorePtr->CompVccIogWpLvrVrefSel;
  }
  MrcWriteCR (MrcData, Offset, CompVccIogWp0.Data);

  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP1_REG, SaGvPoint);
  CompVccIog40Wp0.Data = MrcReadCR (MrcData, Offset);
  CompVccIog40Wp0.Bits.ioglvr_selvdd2_ladder = SelVdd2LadderVccIog;
  if (IsColdBoot) {
    CompVccIog40Wp0.Bits.ioglvr_vrefsel = VrefSelIog;
    LvrSaveRestorePtr->CompVccIog40WpLvrVrefSel = (UINT8) CompVccIog40Wp0.Bits.ioglvr_vrefsel;
  } else {
    CompVccIog40Wp0.Bits.ioglvr_vrefsel = LvrSaveRestorePtr->CompVccIog40WpLvrVrefSel;
  }
  MrcWriteCR (MrcData, Offset, CompVccIog40Wp0.Data);

  // Restore the overrides
  PmaOvrd.Data = MrcReadCR (MrcData, DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG);
  PmaOvrd.Bits.WorkPointUpdateOvrd = 0;
  PmaOvrd.Bits.WorkPointUpdateOvrdVal = 0;
  MrcWriteCR (MrcData, DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG, PmaOvrd.Data);

  PmaOvrd3.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG);
  PmaOvrd3.Bits.IOGLVRWorkPointUpdateOvrdEn = 0;
  PmaOvrd3.Bits.IOGLVRWorkPointUpdateOvrdVal = 0;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG, PmaOvrd3.Data);

  LvrCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG);
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrdEn = 0;
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrd = 0;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG, LvrCtrl1.Data);

  for (Index = 0; Index < MRC_CCC_SHARED_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRCCC_SBMEM0_CR_PM_MISC_CTRL_REG, DDRCCC_SBMEM1_CR_PM_MISC_CTRL_REG, Index);
    PmMiscCtrl.Data = MrcReadCR (MrcData, Offset);
    PmMiscCtrl.Bits.EnIOGDDQGLpMode4Mask = 1;
    PmMiscCtrl.Bits.IOG_LVREn_MaskforLp4 = 1;
    PmMiscCtrl.Bits.IOG_LVRVrefEn_MaskforLp4 = 1;
    MrcWriteCR (MrcData, Offset, PmMiscCtrl.Data);
  }

  for (Index = 0; Index < MRC_DATA_MOBILE_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG, DDRDATA_SBMEM1_CR_DDRCRDLLCBTUNE0_REG, Index);
    DllCbTune0.Data = MrcReadCR (MrcData, Offset);
    DllCbTune0.Bits.bw4 = 9;
    MrcWriteCR (MrcData, Offset, DllCbTune0.Data);

    Offset = OFFSET_CALC_CH (DDRDATA_SBMEM0_CR_DDRCRTXDLLCBTUNE0_REG, DDRDATA_SBMEM1_CR_DDRCRTXDLLCBTUNE0_REG, Index);
    DllCbTune0.Data = MrcReadCR (MrcData, Offset);
    DllCbTune0.Bits.bw4 = 9;
    MrcWriteCR (MrcData, Offset, DllCbTune0.Data);
  }

  for (Index = 0; Index < MRC_VCCCLK_MOBILE_NUM; Index++) {
    if (!MrcGetHwControllerExists (MrcData, Index)) {
      continue;
    }
    Offset = OFFSET_CALC_MC_CH (DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG, DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP0_REG, Index,
                                DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP1_REG, SaGvPoint);
    RptChCrMisc.Data = MrcReadCR (MrcData, Offset);
    RptChCrMisc.Bits.TLTermEn2TLClken = 4;
    RptChCrMisc.Bits.TLClken2TLTermEn = 4;
    if (!Inputs->IsDdrIoMbA0) {
      RptChCrMisc.Bits.LVRPwrGoodQualTLTermEn      = 1;
      RptChCrMisc.Bits.LVRPwrGoodQualTLTermEnonLP4 = 1;
    }
    MrcWriteCR (MrcData, Offset, RptChCrMisc.Data);

    if (!Inputs->IsDdrIoMbA0) {
      Offset = OFFSET_CALC_CH (DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_REG, DDRVCCCLK_SBMEM1_CR_PMMISC_CTRL_REG, Index);
      VccClkPmMiscCtrl.Data = MrcReadCR (MrcData, Offset);
      VccClkPmMiscCtrl.Bits.Spare = 2;
      MrcWriteCR (MrcData, Offset, VccClkPmMiscCtrl.Data);
    }
  }

  PllFuseVirtual.Data = MrcReadCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_REG);
  PllFuseVirtual.Bits.PLLEnLow = 0;
  PllFuseVirtual.Bits.LockCountP_M = 1;
  PllFuseVirtual.Bits.ColdLockTime = 0x100;
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_REG, PllFuseVirtual.Data);

  DdrCrVssHi.Data = 0;
  DdrCrVssHi.Bits.vsshilvr_vrefsel = IsLpddr5 ? 4 : 5;
  MrcWriteCrMulticast (MrcData, DDRCCC_SBMEM_CR_DDRCRVSSHI_REG, DdrCrVssHi.Data);
  MrcWriteCrMulticast (MrcData, DDRDATA_SBMEM_CR_DDRCRVSSHI_REG, DdrCrVssHi.Data);
  MrcWriteCrMulticast (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVSSHI_REG, DdrCrVssHi.Data);
}

/**
  Set the final VccIog value into Outputs->VccIogVoltage

  @param[in, out] MrcData - All the MRC global data.
**/
VOID
SetVccIo (
  IN MrcParameters *const MrcData
  )
{
  MrcInput  *Inputs;
  MrcOutput *Outputs;
  MrcDebug  *Debug;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  if (Outputs->IsDdr5) {
    Outputs->VccIogVoltage = 800;
  } else {
    if (Inputs->IsDdrIoMbA0) {
      if (Outputs->Frequency < f4800) {
        Outputs->VccIogVoltage = 780;
      } else if (Outputs->Frequency <= f7467) {
        Outputs->VccIogVoltage = 800;
      } else {
        Outputs->VccIogVoltage = 850;
      }
    } else {
      if (Outputs->Frequency <= f7467) {
        Outputs->VccIogVoltage = 750;
      } else if (Outputs->Frequency <= f8533) {
        Outputs->VccIogVoltage = 850;
      } else {
        Outputs->VccIogVoltage = 850;
      }
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "VccIo(mV) = %u\n", Outputs->VccIogVoltage);
}

/**
  This function calculates the VccClk using frequency table. Sets the final value into Outputs->VccClkVoltage

  @param[in, out] MrcData - All the MRC global data.
  @retval Returns none
**/
VOID
SetVccClk (
  IN MrcParameters *const MrcData
)
{
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  MrcFrequency  DdrFrequency;
  UINT32        VccClk;
  BOOLEAN       IsDdr5;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  IsDdr5 = Outputs->IsDdr5;
  DdrFrequency = Outputs->Frequency;

  if (IsDdr5) {
    VccClk = 750;
  } else {
    if (DdrFrequency <= f7467) {
      VccClk = 750;
    } else if (DdrFrequency <= f8533) {
      VccClk = 850;
    } else {
      VccClk = 850;
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "VccClk(mV) = %d\n", VccClk);
  Outputs->VccClkVoltage = VccClk;
}

/**
  This function sets TlineTermination

  @param[in, out] MrcData - All the MRC global data.

**/
VOID
SetTlineTermination (
  IN MrcParameters *const MrcData
 )
{
}

/**
  static_pma1 sequence.

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcStaticPma1 (
  IN OUT MrcParameters *const MrcData
  )
{
  DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_STRUCT   UgbControl;
  DDRPHY_MISC_SAUG_CR_PG_CTRL1_STRUCT         PgCtrl1;
  DDRPHY_MISC_SAUG_CR_PG_CTRL2_STRUCT         PgCtrl2;
  DDRPHY_MISC_SAUG_CR_PG_CTRL3_STRUCT         PgCtrl3;
  DDRPHY_MISC_SAUG_CR_LVR_CTRL0_STRUCT        LvrCtrl0;
  DDRPHY_MISC_SAUG_CR_LVR_CTRL1_STRUCT        LvrCtrl1;
  DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_STRUCT     PchDfiCtrl;
  DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_STRUCT       PhyPmovrd4;
  DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_STRUCT       PhyPmOvrd3;

  UgbControl.Data = MrcReadCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_REG);
  UgbControl.Bits.UGBRampUpDelay  = 0;
  UgbControl.Bits.BGBiasTrim      = 0;
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_REG, UgbControl.Data);

  PgCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL1_REG);
  PgCtrl1.Bits.VccDDQG_prampictl  = 6;
  PgCtrl1.Bits.VccDD2G_prampictl  = 6;
  PgCtrl1.Bits.VccSAG_prampictl   = 4;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL1_REG, PgCtrl1.Data);

  PgCtrl2.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL2_REG);
  PgCtrl2.Bits.LVRPwrGoodFSMCntDelay = 2;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL2_REG, PgCtrl2.Data);

  PgCtrl3.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL3_REG);
  PgCtrl3.Bits.VccSAXG_prampictl = 4;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL3_REG, PgCtrl3.Data);

  LvrCtrl0.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL0_REG);
  LvrCtrl0.Bits.IOG_LVRPwrGoodOnWhileRelockOvrd = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL0_REG, LvrCtrl0.Data);

  LvrCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG);
  LvrCtrl1.Bits.VccClk_LVRPwrGoodOnWhileRelockOvrd = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG, LvrCtrl1.Data);

  PchDfiCtrl.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_REG);
  PchDfiCtrl.Bits.LpMode4IntPropDelay = 2;
  PchDfiCtrl.Bits.LPM4DisSAGPwrDn = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_REG, PchDfiCtrl.Data);

  PhyPmovrd4.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_REG);
  PhyPmovrd4.Bits.LPM4ClkEnOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4CrResetOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4RetCriClkEnOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4RetSRSBClkDisOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4RetRestoreOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4RetCtrlOvrdEn = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_REG, PhyPmovrd4.Data);

  PhyPmOvrd3.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG);
  PhyPmOvrd3.Bits.pgddr5mode = MrcData->Outputs.IsDdr5 ? 1 : 0;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG, PhyPmOvrd3.Data);
}

#ifdef HVM_MODE
/**
  This function set LVR Override Enable/Disable.

  @param[in] MrcData  - Pointer to MRC global data.
  @param[in] Override - LVR override enable/disable.
**/
VOID
MrcSetLvrOverride (
  IN MrcParameters *const MrcData,
  IN UINT32               Override
  )
{
  DDRPHY_MISC_SAUG_CR_LVR_CTRL0_STRUCT LvrCtrl0;
  DDRPHY_MISC_SAUG_CR_LVR_CTRL1_STRUCT LvrCtrl1;
  DDRPHY_MISC_SAUG_CR_LVR_CTRL2_STRUCT LvrCtrl2;

  LvrCtrl0.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL0_REG);
  LvrCtrl0.Bits.IOG_LVROvrdModeEn = Override;
  LvrCtrl0.Bits.IOG_LVREnableOvrd = Override;
  LvrCtrl0.Bits.IOG_PwrGoodOvrd = Override;
  LvrCtrl0.Bits.IOG_LVROffOvrd = Override;
  LvrCtrl0.Bits.IOG_LVRVrefEnOvrd = Override;
  LvrCtrl0.Bits.IOG_LVRPwrGoodOnWhileRelockOvrd = Override;
  LvrCtrl0.Bits.IOG_LVRVrefEnOvrdEn = Override;
  LvrCtrl0.Bits.IOG_LVRWPUpdateOvrd = Override;
  LvrCtrl0.Bits.IOG_LVRWPUpdateOvrdEn = Override;
  LvrCtrl0.Bits.IOG_LVRRelockDoneOvrd = Override;
  LvrCtrl0.Bits.IOG_GV_LVROvrdModeEn = Override;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL0_REG, LvrCtrl0.Data);

  LvrCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG);
  LvrCtrl1.Bits.VccClk_LVROvrdModeEn = Override;
  LvrCtrl1.Bits.VccClk_LVREnableOvrd = Override;
  LvrCtrl1.Bits.VccClk_PwrGoodOvrd = Override;
  LvrCtrl1.Bits.VccClk_LVROffOvrd = Override;
  LvrCtrl1.Bits.VccClk_LVRVrefEnOvrd = Override;
  LvrCtrl1.Bits.VccClk_LVRPwrGoodOnWhileRelockOvrd = Override;
  LvrCtrl1.Bits.VccClk_LVRVrefEnOvrdEn = Override;
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrd = Override;
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrdEn = Override;
  LvrCtrl1.Bits.VccClk_LVRRelockDoneOvrd = Override;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG, LvrCtrl1.Data);

  LvrCtrl2.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL2_REG);
  LvrCtrl2.Bits.VssHi_LVROvrdModeEn = Override;
  LvrCtrl2.Bits.VssHi_LVREnableOvrd = Override;
  LvrCtrl2.Bits.VssHi_PwrGoodOvrd = Override;
  LvrCtrl2.Bits.VssHi_LVROffOvrd = Override;
  LvrCtrl2.Bits.VssHi_LVRReadyOvrd = Override;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL2_REG, LvrCtrl2.Data);
}

/**
  This function set re-lock mode, configure override and set Vcc Lvr and Pma1 for HVM runs.

  @param[in] MrcData - Pointer to MRC global data.

  @retval  MrcStatus -  If succeeded, return mrcSuccess
**/
MrcStatus
MrcVccLvrInit (
  IN MrcParameters *const MrcData
  )
{
  // Set LVR override
  MrcSetLvrOverride (MrcData, MRC_ENABLE);

  MrcStaticPma1 (MrcData);
  SetVccLvr (MrcData);

  // Clear LVR override
  MrcSetLvrOverride (MrcData, MRC_DISABLE);

  return mrcSuccess;
}
#endif
