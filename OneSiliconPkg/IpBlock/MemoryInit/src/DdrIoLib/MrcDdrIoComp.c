/** @file
  This file contains code related to initializing and configuring the Rcomp

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include "MrcCommon.h"
#include "MrcMemoryApi.h"
#include "MrcDdrIoUtils.h"
#include "MrcDdrCommon.h"
#include "MrcDdrIoComp.h"
#include "MrcCrosser.h"
#include "MrcDdr5.h" // for TOdtValueDqDdr5

const UINT8 CompParamList[] = { RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk };

/**
  This function prints DdrIo COMP registers related to VccDdq.

  @param[in] MrcData - All the MRC global data.

**/
VOID
MrcPrintDdrIoCompVddq (
  IN MrcParameters* const MrcData
)
{
  MrcDebug* Debug;
  MrcOutput* Outputs;
  INT16       CompCodeUp;
  INT16       CompCodeDn;
  INT64       GetSetVal;
  GSM_GT      GlobalCompUp;
  GSM_GT      GlobalCompDn;
  UINT32      Index;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

  // Walk backwards here to match the earlier flow
  for (Index = (sizeof(CompParamList) / sizeof(CompParamList[0])); Index > 0; Index--) {
    // Return the new comp code (hardware output)
    switch (CompParamList[Index - 1]) {
      case RdOdt:
        GlobalCompUp = CompRcompOdtUp;
        GlobalCompDn = CompRcompOdtDn;
        break;
      case WrDS:
        GlobalCompUp = TxRonUp;
        GlobalCompDn = TxRonDn;
        break;
      case WrDSClk:
        GlobalCompUp = WrDSCodeUpClk;
        GlobalCompDn = WrDSCodeDnClk;
        break;
      case WrDSCmd:
        GlobalCompUp = WrDSCodeUpCmd;
        GlobalCompDn = WrDSCodeDnCmd;
        break;
      case WrDSCtl:
      default:
        GlobalCompUp = WrDSCodeUpCtl;
        GlobalCompDn = WrDSCodeDnCtl;
        break;
    }

    if (GlobalCompUp == CompRcompOdtUp) {
      // In Vss mode only odt down is valid
      if (MrcData->Outputs.OdtMode == MrcOdtModeVss) {
        GlobalCompUp = GlobalCompDn;
      } else {
        GlobalCompDn = GlobalCompUp;
      }
    }
    MrcGetSetNoScope(MrcData, GlobalCompUp, ReadUncached, &GetSetVal);
    CompCodeUp = (INT16)GetSetVal;
    MrcGetSetNoScope(MrcData, GlobalCompDn, ReadUncached, &GetSetVal);
    CompCodeDn = (INT16)GetSetVal;

    //To save code space, RdOdt parameter will print out Up and Down even though only one is valid
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  %8s:  Current Comp code (Up/Down): %3d/%3d\n",
      GlobalCompOffsetStr[CompParamList[Index - 1]],
      CompCodeUp,
      CompCodeDn
    );

    if ((CompCodeUp <= 0) || (CompCodeUp >= 63) || (CompCodeDn <= 0) || (CompCodeDn >= 63)) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "WARNING: COMP code is saturated !\n");
    }
  }
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
  This function programs DdrIo COMP registers related to VccDdq.

  @param[in] MrcData          - All the MRC global data.
  @param[in] Print            - Whether to print debug
  @param[in] VccddqVoltageMv  - Voltage in mV

  @retval Returns mrcSuccess if all resgisters configured.
**/
MrcStatus
DdrIoSetVddqImpactedCrs (
  IN MrcParameters *const MrcData,
  IN BOOLEAN  Print,
  IN UINT32   VccddqVoltageMv
 )
{
  MrcStatus         Status;
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  MrcDebug          *Debug;
  TOdtValueDqDdr5   Ddr5OdtTableIndex;
  INT64             GetSetVal;
  INT64             GetSetTxCANmosOnly;
  INT64             GetSetTxCTLNmosOnly;
  INT64             GetSetTxCLKNmosOnly;
  INT64             GetSetWckTxCLKNmosOnly;
  INT64             GetSetDisWckPupDcc;
  INT64             GetSetTxDqNmosOnly;
  UINT32            VccDdq;
  UINT32            Controller;
  UINT32            Channel;
  UINT32            Byte;
  UINT32            MaxChannel;
  UINT32            MaxByte;
  UINT32            FirstController;
  UINT32            FirstChannel;
  UINT32            Index;

  INT16             CompVrefUp;
  INT16             CompVrefDn;
  BOOLEAN           IsLpddr;
  GSM_GT            GlobalCompUp;
  GSM_GT            GlobalCompDn;

  Inputs     = &MrcData->Inputs;
  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;
  VccDdq     = VccddqVoltageMv;
  IsLpddr    = Outputs->IsLpddr;
  MaxChannel = Outputs->MaxChannels;
  MaxByte    = Outputs->SdramCount;
  FirstController = Outputs->FirstPopController;
  FirstChannel = Outputs->Controller[FirstController].FirstPopCh;
  Status = mrcSuccess;

  if (Inputs->ExtInputs.Ptr->SimicsFlag && (Inputs->IsDdrIoDtHalo)) {
    return mrcSuccess;
  }

  if (Outputs->IsDdr5) {
    Status = GetOdtTableIndex (MrcData, FirstController, FirstChannel, dDIMM0, &Ddr5OdtTableIndex);
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s: Retrieving Lpddr OdtTableIndex failed!\n", gErrString);
      return mrcFail;
    }
  }

  MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmIocCccCaVoltageSelect, ReadFromCache, &GetSetVal);
  //(VccDDQ < 500mV).  VccDDQ is determined by power training and HVM Vmin fuse
  GetSetTxCANmosOnly = ((VccDdq <= VDD_0_50) && (GetSetVal == 0)) ? 1 : 0;
  MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmIocCccCtlVoltageSelect, ReadFromCache, &GetSetVal);
  //(VccDDQ < 500mV).  VccDDQ is determined by power training and HVM Vmin fuse
  GetSetTxCTLNmosOnly = ((VccDdq <= VDD_0_50) && (GetSetVal == 0)) ? 1 : 0;
  MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmIocCccClkVoltageSelect, ReadFromCache, &GetSetVal);
  //(VccDDQ < 500mV).  VccDDQ is determined by power training and HVM Vmin fuse
  GetSetTxCLKNmosOnly = ((VccDdq <= VDD_0_50) && (GetSetVal == 0)) ? 1 : 0;
  MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmIocWckClkVoltageSelect, ReadFromCache, &GetSetVal);
  //(VccDDQ <= 500mV).  VccDDQ is determined by power training and HVM Vmin fuse
  GetSetWckTxCLKNmosOnly = ((VccDdq <= VDD_0_50) && (GetSetVal == 0)) ? 1 : 0;
  //[0] is set for LP5 and (Vddq >= 0.7v).
  GetSetDisWckPupDcc = (IsLpddr && (VccDdq >= VDD_0_70)) ? 1 : 0;
  //(VccDDQ < 500mV)
  GetSetTxDqNmosOnly = (VccDdq <= VDD_0_50) ? 1 : 0;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      //Program regardless of population
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmIocCccTxCANmosOnly, ForceWriteCached, &GetSetTxCANmosOnly);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmIocCccTxCTLNmosOnly, ForceWriteCached, &GetSetTxCTLNmosOnly);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmIocCccTxCLKNmosOnly, ForceWriteCached, &GetSetTxCLKNmosOnly);
      if (IsLpddr) {
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmIocWckTxCLKNmosOnly, ForceWriteCached, &GetSetWckTxCLKNmosOnly);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmIocDisWckPupDcc, ForceWriteCached, &GetSetDisWckPupDcc);
      }
      for (Byte = 0; Byte < MaxByte; Byte++) {
        MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocDataTxDqNmosOnly, ForceWriteCached, &GetSetTxDqNmosOnly);
      }
    }
  }

  MrcGetSetNoScope (MrcData, GsmIocCompTxCANmosOnly, ForceWriteCached, &GetSetTxCANmosOnly);
  MrcGetSetNoScope (MrcData, GsmIocCompTxCTLNmosOnly, ForceWriteCached, &GetSetTxCTLNmosOnly);
  MrcGetSetNoScope (MrcData, GsmIocCompTxCLKNmosOnly, ForceWriteCached, &GetSetTxCLKNmosOnly);
  MrcGetSetNoScope (MrcData, GsmIocCompTxDqNmosOnly, ForceWriteCached, &GetSetTxDqNmosOnly);

  MRC_DEBUG_MSG ((Print) ? Debug : NULL, MSG_LEVEL_NOTE, "Init COMP CRs\n");
  // Walk backwards here because RdOdt depends on WrDS.
  for (Index = (sizeof (CompParamList) / sizeof (CompParamList[0])); Index > 0; Index--) {

    switch (CompParamList[Index - 1]) {
      case RdOdt:
        GlobalCompUp = DqOdtVrefUp;
        GlobalCompDn = DqOdtVrefDn;
        break;

      case WrDS:
        GlobalCompUp = DqDrvVrefUp;
        GlobalCompDn = DqDrvVrefDn;
        break;

      case WrDSClk:
        GlobalCompUp = ClkDrvVrefUp;
        GlobalCompDn = ClkDrvVrefDn;
        break;

      case WrDSCmd:
        GlobalCompUp = CmdDrvVrefUp;
        GlobalCompDn = CmdDrvVrefDn;
        break;

      case WrDSCtl:
      default:
        GlobalCompUp = CtlDrvVrefUp;
        GlobalCompDn = CtlDrvVrefDn;
        break;
    }

    CalcUpDnVref (MrcData, CompParamList[Index - 1], Outputs->RcompTarget, Print, &CompVrefUp, &CompVrefDn); //Print Value

    GetSetVal = CompVrefUp;
    MrcGetSetNoScope (MrcData, GlobalCompUp, ForceWriteCached, &GetSetVal);

    GetSetVal = CompVrefDn;
    MrcGetSetNoScope (MrcData, GlobalCompDn, ForceWriteCached, &GetSetVal);

  }
  return Status;
}

/**
  This function calculates the Up/Dn values for the Param (RdOdt/WrDS/WrDSCmd/WrDSCtl/WrDSClk).

  @param[in]   MrcData - All the MRC global data.
  @param[in]   Param   - Parameter to calculate Up/Dn values
  @param[in]   RcompTarget - RcompTarget Override
  @param[in]   Print       - Print out or not
  @param[out]  UpValue     - Value for Up component.
  @param[out]  DnValue     - Value for Dn component.

  @retval Returns mrcSuccess if Param is supported otherwise mrcFail
**/
MrcStatus
CalcUpDnVref (
  IN MrcParameters *const MrcData,
  IN UINT32    Param,
  IN UINT16    RcompTarget[MAX_RCOMP_TARGETS],
  IN BOOLEAN   Print,
  OUT INT16    *UpValue,
  OUT INT16    *DnValue
 )
{
  MrcInput      *Inputs;
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  INT16         UpVal;
  INT16         DnVal;
  UINT16        RcompResistor;
  UINT16        R_targ;
  UINT16        RtargForCalculations;
  UINT16        DqRon;
  UINT16        DqRodt;
  UINT16        *RcompTargetPtr;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs  = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  RcompTargetPtr  = (RcompTarget == NULL) ? Outputs->RcompTarget : RcompTarget;
  DqRon           = RcompTargetPtr[WrDS];
  DqRodt          = RcompTargetPtr[RdOdt];
  RcompResistor   = ExtInputs->RcompResistor; // External resistor (100 Ohm)

  if (Param > WrDSClk || ((UpValue == NULL) && (DnValue == NULL))) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Wrong Parameter\n", gErrString);
    return mrcFail;
  }

  switch (Param) {
    case RdOdt:
      // DqOdtVrefUp = 193 * (Rext / (Rext + Rtarg_pup))
      // DqOdtVrefDn = 193 * (Rtarg_pdn / (DqRon_pup + Rtarg_pdn))
      R_targ = DqRodt;
      RtargForCalculations = R_targ;
      break;

    case WrDS:
      // DqDrvVrefUp = 193 * (Rext / (Rext + Rtarg_pup))
      // DqDrvVrefDn = 96
      R_targ = DqRon;
      RtargForCalculations = R_targ;
      break;

    case WrDSCmd:
      // CmdDrvVrefUp = 193 * (Rext / (Rext + Rtarg_pup*2))
      // CmdDrvVrefDn = 96
      R_targ = RcompTargetPtr[WrDSCmd];
      RtargForCalculations = 2 * R_targ;
      break;

    case WrDSCtl:
      // CmdDrvVrefUp = 193 * (Rext / (Rext + Rtarg_pup*2))
      // CmdDrvVrefDn = 96
      R_targ = RcompTargetPtr[WrDSCtl];
      RtargForCalculations = 2 * R_targ;
      break;

    case WrDSClk:
    default:
      // ClkDrvVrefDn = 193 * (Rext / (Rext + Rtarg_pup))
      // ClkDrvVrefUp = 96
      R_targ = RcompTargetPtr[WrDSClk];
      RtargForCalculations = R_targ;
      break;
  }

  switch (Param) {
    case RdOdt:
      DnVal = (INT16) (MRC_RCOMP_VREF_CALCULATION_VALUE * R_targ) / (DqRon + R_targ);
      break;

    case WrDS:
    case WrDSCmd:
    case WrDSCtl:
    case WrDSClk:
    default:
      DnVal = 96;
      break;
  }

  UpVal = (INT16) (MRC_RCOMP_VREF_CALCULATION_VALUE * RcompResistor) / (RcompResistor + RtargForCalculations);

  if (Print) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "%8s: Target value: %3d ReferenceR: %3d VrefOffset (Up/Down): %3d/%3d\n",
      GlobalCompOffsetStr[Param],
      R_targ,
      RcompResistor,
      UpVal,
      DnVal);
  }

  if (UpValue != NULL) {
    *UpValue = UpVal;
  }
  if (DnValue != NULL) {
    *DnValue = DnVal;
  }

  return mrcSuccess;
}

/**
  This function calculates the Dq Rtarg_pup value using Vref_pupcode formula:
  Vref_pupcode = 193 * (Rext / (Rext + Rtarg_pup))
  Rtarg_pup = Rext * ((193 / Vref_pupcode) - 1)

  @param[in] MrcData          - All the MRC global data.
  @param[in] DqVrefUpValue    - The DqVrefUp value.

  @retval Returns Dq Rtarg_pup value
**/
UINT32
CalcDQRodtValueFromDqOdtVrefUp (
  IN MrcParameters *const MrcData,
  IN UINT32  DqVrefUpValue
 )
{
  MrcInput            *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  UINT32              RtargPup;

  MRC_DEBUG_ASSERT ((DqVrefUpValue > 0), &MrcData->Outputs.Debug, "%s - DqVrefUpValue == 0\n", gErrString);

  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  // Vref_pupcode = 193 * (Rext / (Rext + Rtarg_pup))
  // Rtarg_pup = Rext * ((193 / Vref_pupcode) - 1)
  RtargPup = ExtInputs->RcompResistor * ((TWO_DECIMAL_PLACES * MRC_RCOMP_VREF_CALCULATION_VALUE / DqVrefUpValue) - TWO_DECIMAL_PLACES);
  RtargPup /= TWO_DECIMAL_PLACES;

  return RtargPup;
}


/**
  Run the comp engine continuously. If the comp completes before the timeout, run it again.
  @param[in, out] MrcData     - Include all MRC global data.
  @param[in]      TestLength  - Run time in us
  @retval mrcSuccess Poll returns expected
  @retval mrcDeviceBusy did not return expected.
**/
MrcStatus
ForceRcompContinuous (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               TestLength
  )
{
  UINT32 TimeStep;
  UINT32 Step;
  UINT32 RunTime = 0;
  UINT32 NumWaitsToClearBusyBit;
  INT64 GetSetEn = 1;
  INT64 GetSetVal;
  BOOLEAN Busy;

  Step = 100 * MRC_TIMER_1NS;
  TimeStep = (TestLength * MRC_TIMER_1US) / Step; // Convert to step size

  do {
    MrcGetSetNoScope (MrcData, GsmCompCtrlPeriodicComp, ForceWriteNoCache, &GetSetEn);
    NumWaitsToClearBusyBit = 0;
    do {
      RunTime++;
      // It's possible to exit this loop when the timer expires but the comp hasn't yet cleared the Busy bit (ie in the middle of a normal operation)
      // NumWaitsToClearBusyBit tracks the amount of steps needed for one iteration of a comp cycle and adds it to the final timeout to ensure that the
      // comp has adequite time to finish a regular cycle.
      NumWaitsToClearBusyBit++;
      MrcWait (MrcData, Step);

      MrcGetSetNoScope (MrcData, GsmCompCtrlPeriodicComp, ReadNoCache, &GetSetVal);
      Busy = (GetSetVal == 1);
    } while (Busy && (RunTime <= TimeStep));
  } while ((RunTime + NumWaitsToClearBusyBit) <= TimeStep);

  if (Busy) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s: Comp failed to complete within %dus!\n", gErrString, TestLength);
    return mrcDeviceBusy;
  }
  return mrcSuccess;
}
