/** @file
  Implementation of the hardware abstraction layer DdrIo hashes.

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
#include "MrcDdrIoApi.h"  // for prototype declarations
#include "MrcHalRegisterAccess.h" // for function prototypes and interface definitions
#include "CMcAddress.h"

/**
  This function generates the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]  MrcData       - Pointer to global data.
  @param[in]  Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Controller    - Memory controller in the processor socket (0-based).
  @param[in]  Channel       - DDR Channel Number within the memory controller (0-based).
  @param[in]  Rank          - Rank number within a channel (0-based).
  @param[in]  Strobe        - Dqs data group within the rank (0-based).
  @param[in]  Lane          - Lane index within the group (0-based).
  @param[in]  FreqIndex     - Index supporting multiple operating frequencies. (Not used in Client CPU's)
  @param[in]  Group         - DDRIO group to access.
  @param[out] HashVal       - Pointer to return the MMIO access instruction.
  @param[out] VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus
**/
MrcStatus
MrcGetDdrIoHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  )
{
#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  MrcDebug  *Debug;
#endif
  MrcOutput   *Outputs;
  MrcStatus   Status;
  UINT32      CrOffset;
  UINT32      LogicalIndex;
  UINT32      ChMod2;
  BOOLEAN     IsLpddr5;
  MRC_REGISTER_HASH_STRUCT *HashPtr;

  static const UINT32 RoundtripDelayHsh[ROUNDTRIP_DELAY_MAX] = {
    MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_HSH, MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_1_latency_HSH,
    MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_2_latency_HSH, MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_3_latency_HSH
  };
  static const UINT32 RdReTrainDeltaHsh[MAX_RANK_IN_CHANNEL] = {
    DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_HSH, DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR1_HSH,
  };
  static const UINT32 WrReTrainDeltaHsh[MAX_RANK_IN_CHANNEL] = {
    DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_HSH, DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR1_HSH,
  };

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Debug     = &MrcData->Outputs.Debug;
#endif
  Status    = mrcSuccess;
  HashPtr   = (MRC_REGISTER_HASH_STRUCT *) HashVal;
  CrOffset  = MRC_UINT32_MAX;
  Outputs   = &MrcData->Outputs;
  IsLpddr5  = Outputs->IsLpddr5;
  ChMod2    = Channel % 2;

  switch (Group) {
    case RecEnDelay:
      HashPtr->Data = DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_HSH;
      break;

    case RxDqsNDelay:
      HashPtr->Data = DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_HSH;
      break;

    case RxDqsPDelay:
      HashPtr->Data = DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_HSH;
      break;

    case RxVref:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_HSH;
      break;

    case RxDqVref:
      switch (Lane) {
        case 0:
          HashPtr->Data = DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref0_HSH;
          break;
        case 1:
          HashPtr->Data = DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref1_HSH;
          break;
        case 2:
          HashPtr->Data = DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref2_HSH;
          break;
        case 3:
          HashPtr->Data = DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref3_HSH;
          break;
        case 4:
          HashPtr->Data = DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref4_HSH;
          break;
        case 5:
          HashPtr->Data = DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref5_HSH;
          break;
        case 6:
          HashPtr->Data = DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref6_HSH;
          break;
        case 7:
          HashPtr->Data = DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref7_HSH;
          break;

        default:
          Status = mrcWrongInputParameter;
      }
      break;

    case RxVocRise:
      HashPtr->Data = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_HSH;
      break;

    case RxRankMuxDelay:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_HSH;
      break;

    case RxDqsBitDelay:
      HashPtr->Data = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_HSH;
      break;

    case RxDqsBitOffset:
      HashPtr->Data = DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_HSH;
      break;

    case TxDqsDelay:
      HashPtr->Data = DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_HSH;
      break;

    case TxDqDelay:
      HashPtr->Data = DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_HSH;
      break;

    case DefDrvEnLow:
      HashPtr->Data = DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_DefDrvEnLow_HSH;
      break;

    case TxRankMuxDelay:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_HSH;
      break;

    case TxR2RDqsEq:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_HSH;
      break;

    case TxR2RDqEq:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_HSH;
      break;

    case TxR2RDqsPi:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_HSH;
      break;

    case TxR2RDqPi:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_HSH;
      break;

    case RxR2RDqsOff:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_HSH;
      break;

    case RxR2RDqOff:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_HSH;
      break;

    case RxR2RRxPi:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_HSH;
      break;

    case RxR2RRcvEn:
      HashPtr->Data = DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_HSH;
      break;

    case TxDqBitDelay:
      HashPtr->Data = DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_HSH;
      break;

    case RecEnOffset:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_HSH;
      break;

    case RxDqsOffset:
      HashPtr->Data =  DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_HSH;
      break;

    case RxVrefOffset:
      HashPtr->Data =  DATA0CH0_CR_DDRCRADC_VrefOffset_HSH;
      break;

    case TxDqsOffset:
      HashPtr->Data =  DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_HSH;
      break;

    case TxDqOffset:
      HashPtr->Data =  DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_HSH;
      break;

    case WrRetrainDeltaPiCode:
      HashPtr->Data = WrReTrainDeltaHsh[Rank];
      break;

    case RdRetrainDeltaPiCode:
      HashPtr->Data = RdReTrainDeltaHsh[Rank];
      break;

    case RxCompDqsDelayP:
      HashPtr->Data = DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_HSH;
      break;

    case RxCompDqsDelayN:
      HashPtr->Data = DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_HSH;
      break;

    case RxCompDqsOffset:
      HashPtr->Data = DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_HSH;
      break;

    case RoundTripDelay:
      if (IsLpddr5){
        LogicalIndex = (2 * (Channel % 2)) + (4 * (Rank / 2)) + (Rank % 2);
      } else {
        // DDR5
        LogicalIndex = Rank % 4;
      }
      if (LogicalIndex < ROUNDTRIP_DELAY_MAX) {
        HashPtr->Data = RoundtripDelayHsh[LogicalIndex];
      } else {
        Status = mrcWrongInputParameter;
      }
      break;

    case TxDqFifoWrEnTcwlDelay:
      HashPtr->Data = MCMISCS_WRITECFGCH0_tcwl4txdqfifowren_HSH;
      break;

    case TxDqFifoRdEnTcwlDelay:
      HashPtr->Data = MCMISCS_WRITECFGCH0_tcwl4txdqfiforden_HSH;
      break;

    case SenseAmpDelay:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_HSH;
      break;

    case SenseAmpDuration:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_HSH;
      break;

    case McOdtDelay:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_HSH;
      break;

    case McOdtDuration:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_HSH;
      break;

    case CmdGrpPi:
      HashPtr->Data = CH0CCC_CR_DDRCRCCCPICODES_CAPICode_HSH;
      break;

    case CtlGrpPi:
      HashPtr->Data = CH0CCC_CR_DDRCRCCCPICODES_CTLPICode_HSH;
      break;

    case CsPerBitCcc:
      HashPtr->Data = DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_PiCode_HSH;
      break;

    case WckGrpPi:
      if (ChMod2 == 0) {
        HashPtr->Data = DDRDATA_SHARED0_CR_DDRCRCCCPICODES_CLKPICodeCh0_HSH;
      } else {
        HashPtr->Data = DDRDATA_SHARED0_CR_DDRCRCCCPICODES_CLKPICodeCh1_HSH;
      }
      break;

    case TxRonUp:
      HashPtr->Data = DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_RcompDrvUp_HSH;
      break;

    case TxRonDn:
      HashPtr->Data = DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_RcompDrvDown_HSH;
      break;

    default:
      Status = mrcWrongInputParameter;
      break;
  }

  if (Status == mrcSuccess) {
    CrOffset = MrcGetDdrIoRegOffset (MrcData, Group, Socket, Controller, Channel, Rank, Strobe, Lane, FreqIndex, VolatileMask);
  }

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  if (Status != mrcSuccess) {
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Error! Hash is undefined. Group %s(%d) Channel %u Rank %u Strobe %u: 0x%x\n",
      GsmGtDebugStrings[Group], Group, Channel, Rank, Strobe, CrOffset);
  }
#endif // MRC_HAL_DEBUG

  HashPtr->Bits.Offset = CrOffset;

  return Status;
}

/**
  This function generates the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]  MrcData       - Pointer to global data.
  @param[in]  Socket        - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]  Controller    - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]  Channel       - PHY channel (0 based).
  @param[in]  Rank          - Rank number within a channel (0-based).
  @param[in]  Strobe        - Dqs data group within the rank (0-based).
  @param[in]  Lane          - Lane index within the group (0-based).
  @param[in]  FreqIndex     - Index supporting multiple operating frequencies. (Not used in Client CPU's)
  @param[in]  Group         - DDRIO group to access.
  @param[out] HashVal       - Pointer to return the MMIO access instruction.
  @param[out] VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus
**/
MrcStatus
MrcGetDdrIoCfgHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
)
{
  MrcStatus Status;
#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  MrcDebug  *Debug;
#endif
  UINT32     CrOffset;
  MRC_REGISTER_HASH_STRUCT *HashPtr;

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Debug     = &MrcData->Outputs.Debug;
#endif
  Status    = mrcSuccess;
  HashPtr   = (MRC_REGISTER_HASH_STRUCT *) HashVal;
  HashPtr->Data = MRC_UINT32_MAX;
  CrOffset  = MRC_UINT32_MAX;

  switch (Group) {
    case GsmIocDisDataIdlClkGate:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL0_DataClkGateDisAtIdle_HSH;
      break;

    case GsmIocScramLp5Mode:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL0_Lp5Mode_HSH;
      break;

    case GsmIocChNotPop:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL0_Channel_Not_Populated_HSH;
      break;

    case GsmIocDfiCmdDecoderEn:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL0_dficmddecode_en_HSH;
      break;

    case GsmIocDisIosfSbClkGate:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL0_dis_iosf_sb_clk_gate_HSH;
      break;

    case GsmIocScramEn:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL0_ScramEn_HSH;
      break;

    case GsmIocWrRetrainOvrd:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL0_WrRetrainOvrd_HSH;
      break;

    case GsmIocDdr52NMode:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL0_DDR52NMode_HSH;
      break;

    case GsmIocCs2NReset:
      HashPtr->Data = DDRSCRAM_CR_CS_GEARDOWN_CSGearDownReset_HSH;
      break;

    case GsmDdrReset:
      HashPtr->Data = DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL4_DramReset_HSH;
      break;

    case GsmIocDqsMaskPulseCnt:
      HashPtr->Data = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_HSH;
      break;

    case GsmIocDqsPulseCnt:
      HashPtr->Data = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_HSH;
      break;

    case GsmIocDqOverrideData:
      HashPtr->Data = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_HSH;
      break;

    case GsmIocDqOverrideEn:
      HashPtr->Data = DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_HSH;
      break;

    case GsmIocRankOverrideEn:
      HashPtr->Data = DATA0CH0_CR_DATATRAINFEEDBACK_RankOvrd_HSH;
      break;

    case GsmIocRankOverrideVal:
      HashPtr->Data = DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_HSH;
      break;

    case GsmDataDccRankEn:
      HashPtr->Data = DATA0CH0_CR_DCCFSMCONTROL_RankEn_HSH;
      break;

   case GsmIocAuxClkOffAuxClk:
     switch (FreqIndex) {
       case MrcSaGvPoint0:
         HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_HSH;
         break;
       case MrcSaGvPoint1:
         HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal1_spare_HSH;
         break;
       case MrcSaGvPoint2:
         HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal2_spare_HSH;
         break;
       case MrcSaGvPoint3:
         HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal3_spare_HSH;
          break;
        default:
          MRC_HAL_DEBUG_MSG(Debug, MSG_LEVEL_HAL, "Work Point Index Undefined Group %s(%d) Index %u\n", GsmGtDebugStrings[Group], Group, FreqIndex);
          HashPtr->Data = MRC_UINT32_MAX;
          break;
      }
      break;

   case GsmIocQClkCalOff:
     switch (FreqIndex) {
       case MrcSaGvPoint0:
         HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_HSH;
         break;
       case MrcSaGvPoint1:
         HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal1_spare_HSH;
         break;
       case MrcSaGvPoint2:
         HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal2_spare_HSH;
         break;
       case MrcSaGvPoint3:
         HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal3_spare_HSH;
         break;
       default:
         MRC_HAL_DEBUG_MSG(Debug, MSG_LEVEL_HAL, "Work Point Index Undefined Group %s(%d) Index %u\n", GsmGtDebugStrings[Group], Group, FreqIndex);
         HashPtr->Data = MRC_UINT32_MAX;
         break;
     }
     break;

    case GsmIocPHClkOffsetFastCal:
      HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkOffsetFastCal_HSH;
      break;

    case GsmIocAuxOffsetFastCal:
      HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxOffsetFastCal_HSH;
      break;

    case GsmClkDccRankEn:
      HashPtr->Data = CH0CCC_CR_DCCFSMCONTROL_RankEn_HSH;
      break;
      
    case GsmWckDccRankEn:
      HashPtr->Data = DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_RankEn_HSH;
      break;

    case GsmCompVccDdqTarget:
      HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_HSH;
      break;

    case GsmInitCompleteOvrd:
      HashPtr->Data = DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrd_HSH;
      break;

    case GsmInitCompleteOvrdVal:
      HashPtr->Data = DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_HSH;
      break;

    case GsmSaxgEnable:
      HashPtr->Data = DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Enable_HSH;
      break;

    case GsmSaxgReady:
      HashPtr->Data = DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Ready_HSH;
      break;

    case GsmSaxgPwrGood:
      HashPtr->Data = DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAXGPwrGood_HSH;
      break;

    case GsmPChannelEn:
      HashPtr->Data = DDRPHY_MISC_PMA_SAUG_CR_PMCFG_PChannelEn_HSH;
      break;

    case GsmSkipRestoreCR:
      HashPtr->Data = DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRestoreCR_HSH;
      break;

    case GsmSkipRetentionCR:
      HashPtr->Data = DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRetentionCR_HSH;
      break;

    case GsmWorkPointAuxClk:
      switch (FreqIndex) {
        case MrcSaGvPoint0:
        case MrcSaGvPoint1:
        case MrcSaGvPoint2:
        case MrcSaGvPoint3:
          HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_HSH;
          break;

      default:
        MRC_HAL_DEBUG_MSG(Debug, MSG_LEVEL_HAL, "Work Point Index Undefined Group %s(%d) Index %u\n", GsmGtDebugStrings[Group], Group, Index);
        HashPtr->Data = MRC_UINT32_MAX;
        break;
      }
      break;

    case GsmWorkPointRatio:
      switch (FreqIndex) {
        case MrcSaGvPoint0:
        case MrcSaGvPoint1:
        case MrcSaGvPoint2:
        case MrcSaGvPoint3:
          HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_HSH;
          break;

        default:
          MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Work Point Index Undefined Group %s(%d) Index %u\n", GsmGtDebugStrings[Group], Group, Index);
          HashPtr->Data = MRC_UINT32_MAX;
          break;
      }
      break;

    case GsmWorkPointGear4:
      switch (FreqIndex) {
        case MrcSaGvPoint0:
        case MrcSaGvPoint1:
        case MrcSaGvPoint2:
        case MrcSaGvPoint3:
            HashPtr->Data = DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Gear4_HSH;
            break;

        default:
          MRC_HAL_DEBUG_MSG(Debug, MSG_LEVEL_HAL, "Work Point Index Undefined Group %s(%d) Index %u\n", GsmGtDebugStrings[Group], Group, Index);
          HashPtr->Data = MRC_UINT32_MAX;
          break;
      }
      break;

    case GsmIocForceOdtOn:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL0_OdtForceQDrvEn_HSH;
      break;

    case GsmIocTxPiPwrDnDis:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL0_TxPiOn_HSH;
      break;

    case GsmIocInternalClocksOn:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL0_InternalClocksOn_HSH;
      break;

    case GsmIocDataDqsOdtParkMode:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_HSH;
      break;

    case GsmIocForceRxAmpOn:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL0_ForceRxOn_HSH;
      break;

    case GsmIocDisRxSalOffWhenDrvEnHi:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL0_DisRxSalOffWhenDrvEnHi_HSH;
      break;

    case GsmIocDataInvertNibble:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_HSH;
      break;

     case GsmIocDisableTxDqs:
      HashPtr->Data = DATA0CH0_CR_DDRCRDATACONTROL1_DisableTxDqs_HSH;
      break;

    case GsmIocRetrainSwizzleCtlRetrainEn:
      HashPtr->Data = DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_HSH;
      break;

    case GsmIocRetrainSwizzleCtlSerialMrr:
      HashPtr->Data = DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_HSH;
      break;

    case GsmIocRetrainInitPiCode:
      HashPtr->Data = DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_HSH;
      break;

    case GsmIocRetrainRoCount:
      HashPtr->Data = DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_HSH;
      break;

    case GsmIocRetrainCtlInitTrain:
      HashPtr->Data = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_HSH;
      break;

    case GsmIocRetrainCtlDuration:
      HashPtr->Data = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_HSH;
      break;

    case GsmIocRetrainCtlResetStatus:
      HashPtr->Data = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_HSH;
      break;

    case GsmIocIoReset:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL1_io_train_rst_HSH;
      break;

    case GsmIocDccClkGateDisable:
      HashPtr->Data = DDRSCRAM_CR_DDRMISCCONTROL1_dccclkgatedisable_HSH;
      break;

    case GsmDccPHClkPhaseEn:
      HashPtr->Data = DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_PHClkPhaseEn_HSH;
      break;

    case GsmDccPHClkDutyCycleEn:
      HashPtr->Data = DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_PHClkDutyCycleEn_HSH;
      break;

    case GsmCompCtrlPeriodicComp:
      HashPtr->Data = MCMISCS_COMPCONTROL_forcecomp_HSH;
      break;

    case GsmTrainWCkPulse:
      HashPtr->Data = MCMISCS_DDRWCKCONTROL_TrainWCkPulse_HSH;
      break;

    case GsmTrainWCkBL:
      HashPtr->Data = MCMISCS_DDRWCKCONTROL_TrainWCkBL_HSH;
      break;

    case GsmTrainWCkMask:
      HashPtr->Data = MCMISCS_DDRWCKCONTROL_TrainWCkMask_HSH;
      break;

    case GsmWCkDiffLowInIdle:
      HashPtr->Data = MCMISCS_DDRWCKCONTROL_WCkDiffLowInIdle_HSH;
      break;

    case GsmCompCtrlDVFSComp:
      HashPtr->Data = MCMISCS_COMPCONTROL_ForceDVFSComp_HSH;
      break;

    case GsmCompCtrlCompDist:
      HashPtr->Data = MCMISCS_COMPCONTROL_ForceCompDist_HSH;
      break;

    case GsmCompCtrlCompUpdate:
      HashPtr->Data = MCMISCS_COMPCONTROL_ForceCompUpdate_HSH;
      break;

    case GsmIocEnableLpMode4:
      HashPtr->Data = DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_enable_lpmode4_HSH;
      break;

    case GsmMiscPmMessage:
      HashPtr->Data = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_HSH;
      break;

    case GsmMiscPmMessageRunBusy:
      HashPtr->Data = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_RunBusy_HSH;
      break;

    case GsmMiscPmMessageWpSel:
      HashPtr->Data = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_HSH;
      break;

    case GsmIocCsGearDownEnable:
      HashPtr->Data = DDRSCRAM_CR_CS_GEARDOWN_EnCSGearDown_HSH;
      break;

    case GsmIocCsGearDownForce:
      HashPtr->Data = DDRSCRAM_CR_CS_GEARDOWN_ForceCSGearDown_HSH;
      break;

    case GsmIocCsGearDownSrDramTrack:
      HashPtr->Data = DDRSCRAM_CR_CS_GEARDOWN_EnSRDRAMTrack_HSH;
      break;

    // These are unused.  Need to finish HAL if these fields are accessed.
    default:
      Status = mrcWrongInputParameter;
      break;
  }

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  if (Status != mrcSuccess) {
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Hash is undefined. Group %s(%d) Channel %u Rank %u Strobe %u\n",
      GsmGtDebugStrings[Group], Group, Channel, Rank, Strobe);
  }
#endif // MRC_HAL_DEBUG
  if (Status == mrcSuccess) {
    CrOffset = MrcGetDdrIoConfigOffsets (MrcData, Group, Channel, Rank, Strobe, Lane, FreqIndex, VolatileMask);
  }

  HashPtr->Bits.Offset  = CrOffset;

  return Status;
}

