/** @file
  Implementation of the hardware abstraction layer DdrIo limits.

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

// Include files
#include "MrcDdrIoApi.h" // for prototypes of functions implemented here
#include "CMcAddress.h"

/**
  This function returns the register limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[in]  Rank      - Rank number within a channel (0-based).
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetDdrIoGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  IN  UINT32  const   Rank,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  )
{
  MrcStatus Status;
#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  MrcOutput *Outputs;
  MrcDebug  *Debug;
#endif
  INT64     Min;
  INT64     Max;
  UINT32    Wait;

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
#endif
  Status  = mrcSuccess;
  Wait    = 0;
  Min     = MRC_INT64_MIN;
  Max     = MRC_INT64_MAX;


  switch (Group) {
    case RecEnDelay:
      Min = DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_MIN;
      Max = DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_MAX;
      break;

    case RxDqsNDelay:
      Min = DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_MIN;
      Max = DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_MAX;
      break;

    case RxDqsPDelay:
      Min = DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_MIN;
      Max = DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_MAX;
      break;

    case RxVref:
    case RxDqVref:
      Min = DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_MIN;
      Max = DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_MAX;
      break;

    case RxVocRise:
      Min = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_MIN;
      Max = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_MAX;
      break;

    case RxDqsBitDelay:
      Min = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_MIN;
      Max = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_MAX;
      break;

    case RxDqsBitOffset:
      Min = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_MIN;
      Max = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_MAX;
      break;

    case RxRankMuxDelay:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_MAX;
      break;

    case TxDqsDelay:
      Min = DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_MIN;
      Max = DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_MAX;
      break;

    case TxDqDelay:
      Min = DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_MIN;
      Max = DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_MAX;
      break;

    case DefDrvEnLow:
      Min = DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_DefDrvEnLow_MIN;
      Max = DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_DefDrvEnLow_MAX;
      break;

    case TxRankMuxDelay:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_MAX;
      break;

    case TxR2RDqsEq:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_MAX;
      break;

    case TxR2RDqEq:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_MAX;
      break;

    case TxR2RDqsPi:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_MAX;
      break;

    case TxR2RDqPi:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_MAX;
      break;

    case RxR2RDqsOff:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_MAX;
      break;

    case RxR2RDqOff:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_MAX;
      break;

    case RxR2RRxPi:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_MAX;
      break;

    case RxR2RRcvEn:
      Min = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_MIN;
      Max = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_MAX;
      break;

    case TxDqBitDelay:
      Min = DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_MIN;
      Max = DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_MAX;
      break;

    case RecEnOffset:
      Min = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_MIN;
      Max = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_MAX;
      break;

    case RxDqsOffset:
      Min = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_MIN;
      Max = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_MAX;
      break;

    case RxVrefOffset:
      Min = DATA0CH0_CR_DDRCRADC_VrefOffset_MIN;
      Max = DATA0CH0_CR_DDRCRADC_VrefOffset_MAX;
      break;

    case TxDqsOffset:
      Min = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_MIN;
      Max = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_MAX;
      break;

    case TxDqOffset:
      Min = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_MIN;
      Max = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_MAX;
      break;

    case WrRetrainDeltaPiCode:
      Min = DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_MIN;
      Max = DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_MAX;
      break;

    case RdRetrainDeltaPiCode:
      Min = DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_MIN;
      Max = DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_MAX;
      break;

    case RxCompDqsDelayP:
      Min = DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_MIN;
      Max = DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_MAX;
      break;

    case RxCompDqsDelayN:
      Min = DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_MIN;
      Max = DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_MAX;
      break;

    case RxCompDqsOffset:
      Min = DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_MIN;
      Max = DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_MAX;
      break;

    case RoundTripDelay:
      if (MrcData->Outputs.IpModel == MrcTestEnvMc) {
        Min = 0;
      } else {
        Min = MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_MIN;
      }
      Max = MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_MAX;
      break;

    case TxDqFifoWrEnTcwlDelay:
      Min = MCMISCS_WRITECFGCH0_tcwl4txdqfifowren_MIN;
      Max = MCMISCS_WRITECFGCH0_tcwl4txdqfifowren_MAX;
      break;

    case TxDqFifoRdEnTcwlDelay:
      Min = MCMISCS_WRITECFGCH0_tcwl4txdqfiforden_MIN;
      Max = MCMISCS_WRITECFGCH0_tcwl4txdqfiforden_MAX;
      break;

    case SenseAmpDelay:
      Min = DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_MIN;
      Max = DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_MAX;
      break;

    case SenseAmpDuration:
      Min = DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_MIN;
      Max = DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_MAX;
      break;

    case McOdtDelay:
      Min = DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_MIN;
      Max = DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_MAX;
      break;

    case McOdtDuration:
      Min = DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_MIN;
      Max = DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_MAX;
      break;

    case CmdGrpPi:
      Min = CH0CCC_CR_DDRCRCCCPICODES_CAPICode_MIN;
      Max = CH0CCC_CR_DDRCRCCCPICODES_CAPICode_MAX;
      break;

    case CtlGrpPi:
      Min = CH0CCC_CR_DDRCRCCCPICODES_CTLPICode_MIN;
      Max = CH0CCC_CR_DDRCRCCCPICODES_CTLPICode_MAX;
      break;

    case WckGrpPi:
      Min = DDRDATA_SHARED0_CR_DDRCRCCCPICODES_CLKPICodeCh0_MIN;
      Max = DDRDATA_SHARED0_CR_DDRCRCCCPICODES_CLKPICodeCh0_MAX;
      break;

    case TxRonUp:
      Min = DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_RcompDrvUp_MIN;
      Max = DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_RcompDrvUp_MAX;
      break;

    case TxRonDn:
      Min = DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_RcompDrvDown_MIN;
      Max = DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_RcompDrvDown_MAX;
      break;

    default:
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s(%d) has no limits defined\n", GsmGtDebugStrings[Group], Group);
      Status = mrcWrongInputParameter;
      break;
  }

  // Null guard the pointers
  if (MinVal != NULL) {
    *MinVal = Min;
  }
  if (MaxVal != NULL) {
    *MaxVal = Max;
  }
  if (WaitTime != NULL) {
    *WaitTime = Wait;
  }

  return Status;
}

/**
  This function returns DDRIO Configuration group limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[in]  Rank      - Rank number within a channel (0-based).
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetDdrIoCfgGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  IN  UINT32  const   Rank,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  )
{
  MrcStatus Status;
#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  MrcDebug  *Debug;
#endif
  INT64     Min;
  INT64     Max;
  UINT32    Wait;

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Debug   = &MrcData->Outputs.Debug;
#endif
  Status  = mrcSuccess;
  Wait    = 0;
  Min     = MRC_INT64_MIN;
  Max     = MRC_INT64_MAX;

  switch (Group) {
    case GsmTrainWCkBL:
      Min = MCMISCS_DDRWCKCONTROL_TrainWCkBL_MIN;
      Max = MCMISCS_DDRWCKCONTROL_TrainWCkBL_MAX;
      break;

    case GsmTrainWCkMask:
      Min = MCMISCS_DDRWCKCONTROL_TrainWCkMask_MIN;
      Max = MCMISCS_DDRWCKCONTROL_TrainWCkMask_MAX;
      break;

    case GsmIocChNotPop:
      Min = DDRSCRAM_CR_DDRMISCCONTROL0_Channel_Not_Populated_MIN;
      Max = DDRSCRAM_CR_DDRMISCCONTROL0_Channel_Not_Populated_MAX;
      break;

    case GsmIocDqsMaskPulseCnt:
      Min = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_MIN;
      Max = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_MAX;
      break;

    case GsmIocDqsPulseCnt:
      Min = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_MIN;
      Max = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_MAX;
      break;

    case GsmIocDqOverrideData:
      Min = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_MIN;
      Max = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_MAX;
      break;

    case GsmIocDqOverrideEn:
      Min = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_MIN;
      Max = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_MAX;
      break;

    case GsmIocRankOverrideVal:
      Min = DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_MIN;
      Max = DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_MAX;
      break;

    case GsmIocDataDqsOdtParkMode:
      Min = DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_MIN;
      Max = DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_MAX;
      break;

    case GsmIocDataInvertNibble:
      Min = DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_MIN;
      Max = DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_MAX;
      break;

    case GsmDataDccRankEn:
      Min = DATA0CH0_CR_DCCFSMCONTROL_RankEn_MIN;
      Max = DATA0CH0_CR_DCCFSMCONTROL_RankEn_MAX;
      break;

    case GsmClkDccRankEn:
      Min = CH0CCC_CR_DCCFSMCONTROL_RankEn_MIN;
      Max = CH0CCC_CR_DCCFSMCONTROL_RankEn_MAX;
      break;

    case GsmWckDccRankEn:
      Min = DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_RankEn_MIN;
      Max = DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_RankEn_MAX;
      break;

    case GsmCompVccDdqTarget:
      Min = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_MIN;
      Max = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_MAX;
      break;

    case GsmInitCompleteOvrdVal:
      Min = DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_MIN;
      Max = DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_MAX;
      break;

    case GsmWorkPointAuxClk:
      Min = DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_MIN;
      Max = DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_MAX;
      break;

    case GsmWorkPointRatio:
      Min = DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_MIN;
      Max = DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_MAX;
      break;

    case GsmIocAuxClkOffAuxClk:
      Min = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_MIN;
      Max = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_MAX;
      break;

    case GsmIocQClkCalOff:
      Min = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_MIN;
      Max = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_MAX;
      break;

    case GsmIocRetrainSwizzleCtlRetrainEn:
      Min = DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_MIN;
      Max = DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_MAX;
      break;

    case GsmIocRetrainSwizzleCtlSerialMrr:
      Min = DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_MIN;
      Max = DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_MAX;
      break;

    case GsmIocRetrainInitPiCode:
      Min = DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_MIN;
      Max = DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_MAX;
      break;

    case GsmIocRetrainRoCount:
      Min = DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_MIN;
      Max = DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_MAX;
      break;

    case GsmIocRetrainCtlInitTrain:
      Min = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_MIN;
      Max = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_MAX;
      break;

    case GsmIocRetrainCtlDuration:
      Min = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_MIN;
      Max = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_MAX;
      break;

    case GsmIocRetrainCtlResetStatus:
      Min = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_MIN;
      Max = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_MAX;
      break;

    case GsmMiscPmMessage:
      Min = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_MIN;
      Max = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_MAX;
      break;

    case GsmMiscPmMessageWpSel:
      Min = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_MIN;
      Max = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_MAX;
      break;

    // Binary Max/Min
    case GsmIocCsGearDownEnable:
    case GsmIocCsGearDownForce:
    case GsmIocCsGearDownSrDramTrack:
    case GsmIocDfiCmdDecoderEn:
    case GsmSaxgEnable:
    case GsmSaxgReady:
    case GsmSaxgPwrGood:
    case GsmPChannelEn:
    case GsmSkipRestoreCR:
    case GsmSkipRetentionCR:
    case GsmMiscPmMessageRunBusy:
    case GsmWorkPointGear4:
    case GsmIocDisableTxDqs:
    case GsmIocIoReset:
    case GsmIocDccClkGateDisable:
    case GsmIocScramLp5Mode:
    case GsmIocDisDataIdlClkGate:
    case GsmIocDisIosfSbClkGate:
    case GsmIocScramEn:
    case GsmIocWrRetrainOvrd:
    case GsmIocDdr52NMode:
    case GsmIocCs2NReset:
    case GsmDdrReset:
    case GsmIocForceOdtOn:
    case GsmIocTxPiPwrDnDis:
    case GsmIocInternalClocksOn:
    case GsmIocForceRxAmpOn:
    case GsmIocDisRxSalOffWhenDrvEnHi:
    case GsmIocEnableLpMode4:
    case GsmDccPHClkPhaseEn:
    case GsmDccPHClkDutyCycleEn:
    case GsmCompCtrlPeriodicComp:
    case GsmTrainWCkPulse:
    case GsmWCkDiffLowInIdle:
    case GsmCompCtrlDVFSComp:
    case GsmCompCtrlCompDist:
    case GsmCompCtrlCompUpdate:
    case GsmIocPHClkOffsetFastCal:
    case GsmIocAuxOffsetFastCal:
    case GsmInitCompleteOvrd:
      Min = 0;
      Max = 1;
      break;

    default:
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s(%d) has no limits defined\n", GsmGtDebugStrings[Group], Group);
      Status = mrcWrongInputParameter;
      break;
  }

  // Null guard the pointers
  if (MinVal != NULL) {
    *MinVal = Min;
  }
  if (MaxVal != NULL) {
    *MaxVal = Max;
  }
  if (WaitTime != NULL) {
    *WaitTime = Wait;
  }

  return Status;
}
