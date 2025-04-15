/** @file
  Implementation of hashes used by the harware abstraction layer code.

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

*/

#include "MrcMcApi.h"  // for prototype declarations
#include "MrcHalRegisterAccess.h"  // for MRC_REGISTER_HASH_STRUCT
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
MrcGetMcTimingHash (
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
  MRC_REGISTER_HASH_STRUCT *HashPtr;
  UINT32    CrOffset;

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Debug   = &MrcData->Outputs.Debug;
#endif
  Status   = mrcSuccess;
  HashPtr  = (MRC_REGISTER_HASH_STRUCT *) HashVal;
  CrOffset = MRC_UINT32_MAX;

  switch (Group) {
    case GsmMctRCD:
      HashPtr->Data = MC0_CH0_CR_TC_ACT_tRCD_HSH;
      break;

    case GsmMctRCDw:
      HashPtr->Data = MC0_CH0_CR_TC_ACT_tRCDW_HSH;
      break;

    case GsmMctRDA2ACT:
      HashPtr->Data = MC0_CH0_CR_TC_ACT_RDA2ACT_HSH;
      break;

    case GsmMctWRA2ACT:
      HashPtr->Data = MC0_CH0_CR_TC_ACT_WRA2ACT_HSH;
      break;

    case GsmMctRP:
      HashPtr->Data = MC0_CH0_CR_TC_PRE_trppb_HSH;
      break;

    case GsmMctRFCpb:
      HashPtr->Data = MC0_CH0_CR_TC_RFTP_tRFCpb_HSH;
      break;

    case GsmMctRFM:
      HashPtr->Data = MC0_CH0_CR_TC_REFM_tRFM_HSH;
      break;

    case GsmMctDRFM:
      HashPtr->Data = MC0_CH0_CR_TC_REFM_tDRFM_HSH;
      break;

    case GsmMctRPab:
      HashPtr->Data = MC0_CH0_CR_TC_PRE_trpab_HSH;
      break;

    case GsmMctRAS:
      HashPtr->Data = MC0_CH0_CR_TC_PRE_tras_HSH;
      break;

    case GsmMctnWRDiff:
      HashPtr->Data = MC0_CH0_CR_TC_PRE_nWR_diff_HSH;
      break;

    case GsmMctRDPRE:
      HashPtr->Data = MC0_CH0_CR_TC_PRE_trdpre_HSH;
      break;

    case GsmMctPPD:
      HashPtr->Data = MC0_CH0_CR_TC_PRE_tppd_HSH;
      break;

    case GsmMctWRPRE:
      HashPtr->Data = MC0_CH0_CR_TC_PRE_twrpre_HSH;
      break;

    case GsmMctFAW:
      HashPtr->Data = MC0_CH0_CR_TC_ACT_tFAW_HSH;
      break;

    case GsmMctRRDsg:
      HashPtr->Data = MC0_CH0_CR_TC_ACT_tRRD_sg_HSH;
      break;

    case GsmMctRRDdg:
      HashPtr->Data = MC0_CH0_CR_TC_ACT_tRRD_dg_HSH;
      break;

    case GsmMctVrcgEnable:
      HashPtr->Data = MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_HSH;
      break;

    case GsmMctVrcgDisable:
      HashPtr->Data = MC0_CH0_CR_DVFSQ_CTL_tVRCG_DISABLE_HSH;
      break;

    case GsmMctREFSbRd:
      HashPtr->Data = MC0_CH0_CR_TC_RFTP_tREFSBRD_HSH;
      break;

    case GsmMctLpDeratingExt:
      HashPtr->Data = MC0_CH0_CR_TC_PRE_derating_ext_HSH;
      break;

    case GsmMctRDRDsg:
      HashPtr->Data = MC0_CH0_CR_TC_RDRD_tRDRD_sg_HSH;
      break;

    case GsmMctRDRDdg:
      HashPtr->Data = MC0_CH0_CR_TC_RDRD_tRDRD_dg_HSH;
      break;

    case GsmMctRDRDdr:
      HashPtr->Data = MC0_CH0_CR_TC_RDRD_tRDRD_dr_HSH;
      break;

    case GsmMctRDRDdd:
      HashPtr->Data = MC0_CH0_CR_TC_RDRD_tRDRD_dd_HSH;
      break;

    case GsmMctRDWRsg:
      HashPtr->Data = MC0_CH0_CR_TC_RDWR_tRDWR_sg_HSH;
      break;

    case GsmMctRDWRdg:
      HashPtr->Data = MC0_CH0_CR_TC_RDWR_tRDWR_dg_HSH;
      break;

    case GsmMctRDWRdr:
      HashPtr->Data = MC0_CH0_CR_TC_RDWR_tRDWR_dr_HSH;
      break;

    case GsmMctRDWRdd:
      HashPtr->Data = MC0_CH0_CR_TC_RDWR_tRDWR_dd_HSH;
      break;

    case GsmMctWRRDsg:
      HashPtr->Data = MC0_CH0_CR_TC_WRRD_tWRRD_sg_HSH;
      break;

    case GsmMctWRRDdg:
      HashPtr->Data = MC0_CH0_CR_TC_WRRD_tWRRD_dg_HSH;
      break;

    case GsmMctWRRDdr:
      HashPtr->Data = MC0_CH0_CR_TC_WRRD_tWRRD_dr_HSH;
      break;

    case GsmMctWRRDdd:
      HashPtr->Data = MC0_CH0_CR_TC_WRRD_tWRRD_dd_HSH;
      break;

    case GsmMctWRWRsg:
      HashPtr->Data = MC0_CH0_CR_TC_WRWR_tWRWR_sg_HSH;
      break;

    case GsmMctWRWRdg:
      HashPtr->Data = MC0_CH0_CR_TC_WRWR_tWRWR_dg_HSH;
      break;

    case GsmMctWRWRdr:
      HashPtr->Data = MC0_CH0_CR_TC_WRWR_tWRWR_dr_HSH;
      break;

    case GsmMctWRWRdd:
      HashPtr->Data = MC0_CH0_CR_TC_WRWR_tWRWR_dd_HSH;
      break;

    case GsmtccdCasDelta:
      HashPtr->Data = MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_HSH;
      break;

    case GsmMctCL:
      HashPtr->Data = MC0_CH0_CR_TC_CAS_tCL_HSH;
      break;

    case GsmMctCWL:
      HashPtr->Data = MC0_CH0_CR_TC_CAS_tCWL_HSH;
      break;

    case GsmMctCWLDec:
      HashPtr->Data = MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_HSH;
      break;

    case GsmMctCWLAdd:
      HashPtr->Data = MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_HSH;
      break;

    case GsmMctCKE:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_tcke_HSH;
      break;

    case GsmMctXP:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_txp_HSH;
      break;

    case GsmMctPRPDEN:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_tprpden_HSH;
      break;

    case GsmMctRDPDEN:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_trdpden_HSH;
      break;

    case GsmMctWRPDEN:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_twrpden_HSH;
      break;

    case GsmMctCA2CS:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_tcacsh_HSH;
      break;

    case GsmMctCSL:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_tcsl_HSH;
      break;

    case GsmMctCSH:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_tcsh_HSH;
      break;

    case GsmMctXSDLL:
      HashPtr->Data = MC0_CH0_CR_TC_SREXITTP_tXSDLL_HSH;
      break;

    case GsmMctXSR:
      HashPtr->Data = MC0_CH0_CR_TC_SREXITTP_tXSR_HSH;
      break;

    case GsmMctMrhAfterCommandDelay:
      HashPtr->Data = MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_HSH;
      break;

    case GsmMctLpMode4SrxDelay:
      HashPtr->Data = MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_HSH;
      break;

    case GsmMctSR:
      HashPtr->Data = MC0_CH0_CR_TC_SREXITTP_tSR_HSH;
      break;

    case GsmMctXSRDs:
      HashPtr->Data = MC0_CH0_CR_TC_SREXITTP_tXSR_DS_HSH;
      break;

    case GsmMctMRD:
      HashPtr->Data = MC0_CH0_CR_TC_MR_tMRD_HSH;
      break;

    case GsmMctZQCAL:
      HashPtr->Data = MC0_CH0_CR_TC_ZQCAL_tZQCAL_HSH;
      break;

    case GsmMcttZQLatch:
      HashPtr->Data = MC0_CH0_CR_TC_ZQCAL_tZQLATCH_HSH;
      break;

    case GsmMctZQCalPeriod:
      HashPtr->Data = MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_HSH;
      break;

    case GsmMctCPDED:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_tcpded_HSH;
      break;

    case GsmMctCKCKEH:
      HashPtr->Data = MC0_CH0_CR_TC_PWRDN_tckckeh_HSH;
      break;

    case GsmMctSrIdleTimer:
      HashPtr->Data = MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_HSH;
      break;

    case GsmMctSrefDelayQsync:
      HashPtr->Data = MC0_MCDECS_SECOND_CBIT_delay_qsync_HSH;
      break;

    case GsmMctREFI:
      HashPtr->Data = MC0_CH0_CR_TC_RFTP_tREFI_HSH;
      break;

    case GsmMctRFC:
      HashPtr->Data = MC0_CH0_CR_TC_RFTP_tRFC_HSH;
      break;

    case GsmMcDdr5Rir:
      HashPtr->Data = MC0_CH0_CR_RFP_DDR5_RIR_HSH;
      break;

    case GsmMctRefreshHpWm:
      HashPtr->Data = MC0_CH0_CR_RFP_Refresh_HP_WM_HSH;
      break;

    case GsmMctRefreshPanicWm:
      HashPtr->Data = MC0_CH0_CR_RFP_Refresh_panic_wm_HSH;
      break;

    case GsmMcttOSCO:
      HashPtr->Data = MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_HSH;
      break;

    case GsmMcttPREMRR:
      HashPtr->Data = MC0_CH0_CR_TC_MR_PREMRR_HSH;
      break;

    case GsmMctWrOsclRuntime:
      HashPtr->Data = MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_HSH;
      break;

    case GsmMcttMRR:
      HashPtr->Data = MC0_CH0_CR_TC_MR_tMRR_HSH;
      break;

    case GsmMcttCSLCK:
      HashPtr->Data = MC0_CH0_CR_TC_WCK_tCSLCK_HSH;
      break;

    case GsmMcttWckStop:
      HashPtr->Data = MC0_CH0_CR_TC_WCK_tWCKSTOP_HSH;
      break;

    case GSmMcttWckOff:
      HashPtr->Data = MC0_CH0_CR_TC_WCK_twckoff_HSH;
      break;

    case GsmMcttCkFspX:
      HashPtr->Data = MC0_CH0_CR_TC_WCK_tckfspx_HSH;
      break;

    case GsmMctWckEnlFs:
      HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_twck_en_fs_HSH;
      break;

    case GsmMctWckEnlRd:
      HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_twck_en_rd_HSH;
      break;

    case GsmMctWckEnlWr:
      HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_twck_en_wr_HSH;
      break;

    case GsmMctWckPreToggleFs:
      HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_twck_toggle_fs_HSH;
      break;

    case GsmMctWckPreToggleRd:
      HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_HSH;
      break;

    case GsmMctWckPreToggleWr:
      HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_twck_toggle_wr_HSH;
      break;

    case GsmMctWckDfiStop:
      HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_HSH;
      break;

    case GsmMctWckDfiOffset:
      HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_HSH;
      break;

    default:
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Hash undefined. Group %s(%d) Channel %u\n", GsmGtDebugStrings[Group], Group,Channel);
      HashPtr->Data = MRC_UINT32_MAX;
      Status = mrcWrongInputParameter;
      break;
  }

  if (Status == mrcSuccess) {
    CrOffset = MrcGetMcTimingRegOffset (MrcData, Group, Socket, Controller, Channel, FreqIndex, VolatileMask);
  }

  HashPtr->Bits.Offset = CrOffset;

  return Status;
}


/**
  This function returns the hash used to execute the Get/Set function.
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
MrcGetMcConfigHash (
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
  MRC_REGISTER_HASH_STRUCT *HashPtr;
  UINT32    CrOffset;
  BOOLEAN   IsDefined;
  UINT32    RrdValidIndex;
  MrcOutput *Outputs;
  BOOLEAN   IsLpddr;
  UINT8     MrrHshRank;
  static const UINT32 RrdValidTriggerHsh[ROUNDTRIP_DELAY_MAX] = {
    MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_trigger_HSH, MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_trigger_HSH,
    MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_trigger_HSH, MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_trigger_HSH
  };
  static const UINT32 RrdValidOverflowHsh[ROUNDTRIP_DELAY_MAX] = {
    MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_overflow_HSH, MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_overflow_HSH,
    MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_overflow_HSH, MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_overflow_HSH
  };
  static const UINT32 RrdValidValueHsh[ROUNDTRIP_DELAY_MAX] = {
    MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_HSH, MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_value_HSH,
    MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_value_HSH, MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_value_HSH
  };
  static const UINT32 RrdValidSignHsh[ROUNDTRIP_DELAY_MAX] = {
    MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_sign_HSH, MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_sign_HSH,
    MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_sign_HSH, MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_sign_HSH
  };

  static const UINT32 DeswizzleBitHsh[2][5] = { // 2 Bytes, 5 Bits per Byte
    { MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_HSH,   MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_1_HSH,
      MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_2_HSH,   MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_3_HSH,  MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_4_HSH },
    { MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_8_HSH,   MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_9_HSH,
      MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_10_HSH,  MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_11_HSH,  MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_12_HSH }
  };

  static const UINT32 MrrResultHsh[MAX_RANK_IN_DIMM][MAX_BYTE_IN_DDR5_CHANNEL] = {
    { MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_HSH, MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D1_HSH,
      MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D2_HSH, MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D3_HSH },
    { MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D0_HSH, MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D1_HSH,
      MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D2_HSH, MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D3_HSH }
    };

  static const UINT32 OdtMatrixRdHsh[MAX_RANK_IN_CHANNEL] = {
    MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_HSH, MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_1_HSH,
  };

  static const UINT32 OdtMatrixWrHsh[MAX_RANK_IN_CHANNEL] = {
    MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_0_HSH, MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_1_HSH,
  };

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Debug   = &MrcData->Outputs.Debug;
#endif

  Outputs       = &MrcData->Outputs;
  Status       = mrcSuccess;
  HashPtr      = (MRC_REGISTER_HASH_STRUCT *) HashVal;
  CrOffset     = MRC_UINT32_MAX;
  IsDefined    = TRUE;
  IsLpddr      = Outputs->IsLpddr;

  if (IsLpddr) {
    // Note: Channel is a system-level channel here, because the register fields are per subch in LP5 (like RoundTripDelay)
    // Lpddr expects Rank to only be 0 or 1
    RrdValidIndex = (MAX_RANK_IN_SUBCHANNEL * (Channel % MAX_SUB_CHANNEL)) + Rank;
  } else {
    RrdValidIndex = Rank % MAX_RANK_IN_CHANNEL;
  }

  switch (Group) {
  case GsmMccDramType:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_dram_technology_HSH;
    break;

  case GsmMccCmdStretch:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_cmd_stretch_HSH;
    break;

  case GsmMccAddrMirror:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_address_mirror_HSH;
    break;

  case GsmMccCmdTriStateDis:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_UARCH_disable_tristate_HSH;
    break;

  case GsmMccFreqPoint:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_frequency_point_HSH;
    break;

  case GsmMccEnableOdtMatrix:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_enable_odt_matrix_HSH;
    break;

  case GsmMccGearMode:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_gear_mode_HSH;
    break;

  case GsmMccOneDpc:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_HSH;
    break;

  case GsmMccMultiCycCmd:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_multicyccmd_HSH;
    break;

  case GsmMccEnhancedInterleave:
    HashPtr->Data = MC0_MAD_DRAM_HASH_0_eim_HSH;
    break;

  case GsmMccEccMode:
    HashPtr->Data = MC0_MAD_INTER_CHANNEL_ECC_HSH;
    break;

  case GsmMccExtendedBankHash:
    HashPtr->Data = MC0_MAD_DRAM_HASH_0_ebh_HSH;
    break;

  case GsmMccBg0Hash:
    HashPtr->Data = MC0_MAD_DRAM_HASH_0_bg0hash_HSH;
    break;

  case GsmMccAddrDecodeDdrType:
    HashPtr->Data = MC0_MAD_INTER_CHANNEL_DDR_TYPE_HSH;
    break;

  case GsmMccCh0Size:
    HashPtr->Data = MC0_MAD_INTER_CHANNEL_CH_0_SIZE_HSH;
    break;

  case GsmMccCh1Size:
    HashPtr->Data = MC0_MAD_INTER_CHANNEL_CH_1_SIZE_HSH;
    break;

  case GsmMccCpgcActive:
    HashPtr->Data = MC0_MC_CPGC_CONTROL_cpgc_active_HSH;
    break;

  case GsmMccInOrderIngress:
    HashPtr->Data = MC0_MC_CPGC_CONTROL_UARCH_in_order_ingress_HSH;
    break;

  case GsmMccHashEnabled:
    HashPtr->Data = MC0_MAD_MC_HASH_Hash_enabled_HSH;
    break;

  case GsmMccHashLsb:
    HashPtr->Data = MC0_MAD_MC_HASH_Hash_LSB_HSH;
    break;

  case GsmMccHashZone1Start:
    HashPtr->Data = MC0_MAD_MC_HASH_Zone1_start_HSH;
    break;

  case GsmMccSubch0SdramWidth:
    HashPtr->Data = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_HSH;
    break;

  case GsmMccSubch0RankCnt:
    HashPtr->Data = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_HSH;
    break;

  case GsmMccSubch0Density:
    HashPtr->Data = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_HSH;
    break;

   case GsmMccSubch1SdramWidth:
    HashPtr->Data = MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_HSH;
    break;

  case GsmMccSubch1RankCnt:
    HashPtr->Data = MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_HSH;
    break;

  case GsmMccSubch1Density:
    HashPtr->Data = MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_DENSITY_HSH;
    break;

  case GsmMccEnableRefresh:
    HashPtr->Data = MC0_MC_INIT_STATE_G_refresh_enable_HSH;
    break;

  case GsmMccEnableDclk:
    HashPtr->Data = MC0_MC_INIT_STATE_G_dclk_enable_HSH;
    break;

  case GsmMccPureSrx:
    HashPtr->Data = MC0_MC_INIT_STATE_G_pure_srx_HSH;
    break;

  case GsmMccSrState:
    HashPtr->Data = MC0_MC_INIT_STATE_G_sr_state_HSH;
    break;

  case GsmMccForceSreWithFreqChange:
    HashPtr->Data = MC0_CH0_CR_PM_SREF_CONTROL_force_sre_with_freq_change_HSH;
    break;

  case GsmMccCpgcInOrder:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_cpgc_in_order_HSH;
    break;

  case GsmMccBlockXarb:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_xarb_HSH;
    break;

  case GsmMccBlockCke:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_cke_HSH;
    break;

  case GsmMccRankOccupancy:
    HashPtr->Data = MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_HSH;
    break;

  case GsmMccMcSrx:
    HashPtr->Data = MC0_CH0_CR_MC_INIT_STATE_SRX_reset_HSH;
    break;

  case GsmMccEnRefTypeDisplay:
    HashPtr->Data = MC0_CH0_CR_MC_REFRESH_STAGGER_En_Ref_Type_Display_HSH;
    break;

  case GsmMccHashMask:
    HashPtr->Data = MC0_MAD_CHANNEL_HASH_HASH_MASK_HSH;
    break;

  case GsmMccLsbMaskBit:
    HashPtr->Data = MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_HSH;
    break;

  case GsmMccCkDisTristate:
    HashPtr->Data = MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ck_tristate_HSH;
    break;

  case GsmMccDisStarvedPriorityOnNewReq:
    HashPtr->Data = MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starved_prio_on_new_req_HSH;
    break;

  case GsmMccPbrDis:
    HashPtr->Data = MC0_CH0_CR_RFP_PBR_Disable_HSH;
    break;

  case GsmMccRefreshAbrRelease:
    HashPtr->Data = MC0_CH0_CR_RFP_Refresh_ABR_release_HSH;
    break;

  case GsmMccCkeOverride:
    HashPtr->Data = MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_HSH;
    break;

  case GsmMccCkeOn:
    HashPtr->Data = MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_On_HSH;
    break;

  case GsmMccCsOverride0:
    HashPtr->Data = MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_0_HSH;
    break;

  case GsmMccCsOverrideVal0:
    HashPtr->Data = MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_0_HSH;
    break;

  case GsmMccCsOverride1:
    HashPtr->Data = MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_1_HSH;
    break;

  case GsmMccCsOverrideVal1:
    HashPtr->Data = MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_1_HSH;
    break;

  case GsmMccMinRefRate:
    HashPtr->Data = MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_HSH;
    break;

  case GsmMccMR4Period:
    HashPtr->Data = MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_HSH;
    break;

  case GsmMccLpddrCurrentFsp:
    HashPtr->Data = MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_HSH;
    break;

  case GsmMccLpddrCurrentFspTrk:
    HashPtr->Data = MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_tracking_HSH;
    break;

  case GsmMccZqSerialize:
    HashPtr->Data = MC0_CH0_CR_ZQCAL_CONTROL_serialize_zq_HSH;
    break;

  case GsmMccLp5BankMode:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_HSH;
    break;

  case GsmMccLp5WckMode:
    HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_HSH;
    break;

  case GsmMccLp5WckFastMode:
    HashPtr->Data = MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_HSH;
    break;

  case GsmDisVc1CplInterleave:
    HashPtr->Data = MC0_MC_CFI_CONTROL_dis_VC1_cpl_interleave_HSH;
    break;

  case GsmDisAllCplInterleave:
    HashPtr->Data = MC0_MC_CFI_CONTROL_dis_all_cpl_interleave_HSH;
    break;

  case GsmMccRhLfsr0:
  case GsmMccRhLfsr1:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_LFSR0_LFSR_HSH;
    break;

  case GsmMccRhTRRDimmEnable:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_HSH;
    break;

  case GsmMccRhDrfmEnable:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_HSH;
    break;

  case GsmMccRhLfsr0Mask:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_HSH;
    break;

  case GsmMccRhLfsr1Mask:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_LFSR_1_MASK_HSH;
    break;

  case GsmMccRhMA1Swizzle:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_HSH;
    break;

  case GsmMccRhMA2Swizzle:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_MA2_Swizzling_HSH;
    break;

  case GsmMccMicronFSwizzle:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_Micron_F_Swizzling_HSH;
    break;

  case GsmMccMicronRSwizzle:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_Micron_R_Swizzling_HSH;
    break;

  case GsmMccBrc:
    HashPtr->Data = MC0_CH0_CR_RH_TRR_CONTROL_BRC_HSH;
    break;

  case GsmMccRhEnableDimm0:
    HashPtr->Data = MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM0_HSH;
    break;

  case GsmMccRhEnableDimm1:
    HashPtr->Data = MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM1_HSH;
    break;

  case GsmMccRhHighWM:
    HashPtr->Data = MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_HSH;
    break;

  case GsmMccRhLowWM:
    HashPtr->Data = MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_HSH;
    break;

  case GsmMccRhNormalRefSub:
    HashPtr->Data = MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_HSH;
    break;

  case GsmMccRhRefmSub:
    HashPtr->Data = MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_HSH;
    break;

  case GsmMccRhRefmEn:
    HashPtr->Data = MC0_CH0_CR_ROWHAMMER_CTL_REFm_EN_HSH;
    break;

  case GsmMccAllowRHDebtSR:
    HashPtr->Data = MC0_MCDECS_SECOND_CBIT_Allow_RH_Debt_in_SR_HSH;
    break;

  case GsmMccOppSrefEnable:
    HashPtr->Data = MC0_PM_OPP_SREF_ENABLE_ALLOW_OPP_SR_HSH;
    break;

  case GsmMccForceSRState:
    HashPtr->Data = MC0_NORMALMODE_CFG_Force_SR_State_HSH;
    break;

  case GsmMccVC0ReadEntries:
    HashPtr->Data = MC0_CH0_CR_MC_RDB_CREDITS_VC0_reads_entries_HSH;
    break;

  case GsmMccVC1ReadEntries:
    HashPtr->Data = MC0_CH0_CR_MC_RDB_CREDITS_VC1_reads_entries_HSH;
    break;

  case GsmMccSpineGateDis:
    HashPtr->Data = MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GATE_DIS_HSH;
    break;

  case GsmMccMainGlbDrvGateDis:
  case GsmMccSchedGlbDrvGateDis:
    HashPtr->Data = MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_DRV_GATE_DIS_HSH;
    break;

  case GsmMccPageIdleTimerDisable:
    HashPtr->Data = MC0_CH0_CR_SCHED_CBIT_dis_pt_it_HSH;
    break;

  case GsmMccOdtMatrixRd:
    if (Rank >= MAX_RANK_IN_CHANNEL) {
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "%s Group %s(%d) Index R%u\n", gMrcStatusStr[mrcRankNotSupported], GsmGtDebugStrings[Group], Group, Rank);
      HashPtr->Data = MRC_UINT32_MAX;
      Status = mrcWrongInputParameter;
    } else {
      HashPtr->Data = OdtMatrixRdHsh[Rank];
    }
    break;

  case GsmMccOdtMatrixWr:
    if (Rank >= MAX_RANK_IN_CHANNEL) {
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "%s Group %s(%d) Index R%u\n", gMrcStatusStr[mrcRankNotSupported], GsmGtDebugStrings[Group], Group, Rank);
      HashPtr->Data = MRC_UINT32_MAX;
      Status = mrcWrongInputParameter;
    } else {
      HashPtr->Data = OdtMatrixWrHsh[Rank];
    }
    break;

  case GsmMccCasStopAdditionalGap:
    HashPtr->Data = MC0_CH0_CR_TC_WCK_casstop_additional_gap_HSH;
    break;

  case GsmMccRdWckAsyncGap:
    HashPtr->Data = MC0_CH0_CR_TC_WCK_rd_wck_async_gap_HSH;
    break;

  case GsmMccWrWckAsyncGap:
    HashPtr->Data = MC0_CH0_CR_TC_WCK_wr_wck_async_gap_HSH;
    break;

  case GsmMccEnSrxWrRetraining:
    HashPtr->Data = MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_WR_RETRAINING_HSH;
    break;

  case GsmMccEnWrRetraining:
    HashPtr->Data = MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_WR_RETRAINING_HSH;
    break;

  case GsmMccEnRdRetraining:
    HashPtr->Data = MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_RD_RETRAINING_HSH;
    break;

  case GsmMccRetrainPeriod:
    HashPtr->Data = MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_HSH;
    break;

  case GsmMccVc0LatencyGaurdTimerx16:
    HashPtr->Data = MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_HSH;
    break;

  case GsmMccVc1LatencyGuardTimerx8:
    HashPtr->Data = MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_HSH;
    break;

  case GsmMccVc1IsocLatencyGuardTimerx8:
    HashPtr->Data = MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_Isoc_latency_guard_timer_x8_HSH;
    break;

  case GsmMccLatencySensitiveTimerx16:
    HashPtr->Data = MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Sensitive_Timer_x16_HSH;
    break;

  case GsmMccSelfRefreshEnable:
    HashPtr->Data = MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_enable_HSH;
    break;

  case GsmMccSelfRefreshLength:
    HashPtr->Data = MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_HSH;
    break;

  case GsmMccPowerDownEnable:
    HashPtr->Data = MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_enable_HSH;
    break;

  case GsmMccIdleEnable:
    HashPtr->Data = MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_enable_HSH;
    break;

  case GsmMccDis2cByp:
    HashPtr->Data = MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_HSH;
    break;

  case GsmMccSchedDisClkGateLocal:
    HashPtr->Data = MC0_CH0_CR_SCHED_CBIT_dis_clk_gate_HSH;
    break;

  case GsmMccDisAsyncOdt:
    HashPtr->Data = MC0_CH0_CR_SCHED_SECOND_CBIT_dis_async_odt_HSH;
    break;

  case GsmMccDisableSplitAct:
    HashPtr->Data = MC0_CH0_CR_SCHED_THIRD_CBIT_disable_split_act_HSH;
    break;

  case GsmMccDisSchedsClkGate:
    HashPtr->Data = MC0_CH0_CR_SCHED_THIRD_CBIT_dis_scheds_clk_gate_HSH;
    break;

  case GsmMccCmiSourceId0:
    HashPtr->Data = MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_HSH;
    break;

  case GsmMccCmiSourceId1:
    HashPtr->Data = MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID1_HSH;
    break;

  case GsmMccCmiSourceId2:
    HashPtr->Data = MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID2_HSH;
    break;

  case GsmMccCmiSourceId3:
    HashPtr->Data = MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID3_HSH;
    break;

  case GsmMccDelayPMAckCycles:
    HashPtr->Data = MC0_PM_CONTROL_delay_PM_Ack_cycles_HSH;
    break;

  case GsmMccWCKDiffLowInIdle:
    HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_HSH;
    break;

  case GsmMccRrdValidTrigger:
    if (RrdValidIndex < ROUNDTRIP_DELAY_MAX) {
      HashPtr->Data = RrdValidTriggerHsh[RrdValidIndex];
    }
    break;

  case GsmMccRrdValidOverflow:
    if (RrdValidIndex < ROUNDTRIP_DELAY_MAX) {
      HashPtr->Data = RrdValidOverflowHsh[RrdValidIndex];
    }
    break;

  case GsmMccRrdValidValue:
    if (RrdValidIndex < ROUNDTRIP_DELAY_MAX) {
      HashPtr->Data = RrdValidValueHsh[RrdValidIndex];
    }
    break;

  case GsmMccRrdValidSign:
    if (RrdValidIndex < ROUNDTRIP_DELAY_MAX) {
      HashPtr->Data = RrdValidSignHsh[RrdValidIndex];
    }
    break;

  case GsmMccDeswizzleByte:
    switch (Strobe) {
      case 0:
        HashPtr->Data = MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_HSH;
        break;

      case 1:
        HashPtr->Data = MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_1_HSH;
        break;

      case 2:
        HashPtr->Data = MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_2_HSH;
        break;

      case 3:
        HashPtr->Data = MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_3_HSH;
        break;

      default:
        MRC_HAL_DEBUG_MSG(Debug, MSG_LEVEL_HAL, "Byte device Undefined Group %s(%d) Index %u\n", GsmGtDebugStrings[Group], Group, Strobe);
        HashPtr->Data = MRC_UINT32_MAX;
        Status = mrcWrongInputParameter;
        break;
    }
    break;

    case GsmMccDeswizzleBit:
      if ((Strobe < 2) && (Lane < 5)) {
        HashPtr->Data = DeswizzleBitHsh[Strobe][Lane];
      } else {
        Status = mrcWrongInputParameter;
      }
      break;

    case GsmMccMrrResult:
      if (Rank >= MAX_RANK_IN_CHANNEL) {
        MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "%s Group %s(%d) Index R%u\n", gMrcStatusStr[mrcRankNotSupported], GsmGtDebugStrings[Group], Group, Rank);
        HashPtr->Data = MRC_UINT32_MAX;
        Status = mrcWrongInputParameter;
      } else {
        MrrHshRank = Rank % MAX_RANK_IN_DIMM;
        HashPtr->Data = MrrResultHsh[MrrHshRank][Strobe];
      }
      break;

    case GsmMccRefreshRankMask:
      HashPtr->Data = MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_HSH;
      break;

    case GsmMccDisIosfSbClkGate:
      HashPtr->Data = MC0_MCDECS_CBIT_dis_iosf_sb_clk_gate_HSH;
      break;

    case GsmMccAllowOppRefBelowWrtThreshold:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_allow_opp_ref_below_write_threhold_HSH;
      break;

    case GsmMccWrtThreshold:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_HSH;
      break;

    case GsmMccPdwnIdleCounterSubch0:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_HSH;
      break;

    case GsmMccPdwnIdleCounterSubch1:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch1_HSH;
      break;

    case GsmMccPdwnLpmodeEnable:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_lpmode_enable_HSH;
      break;

    case GsmMccDisCkett:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_dis_cke_tt_HSH;
      break;

    case GsmMccTTIdleCounter:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_HSH;
      break;

    case GsmMccAPD:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_APD_HSH;
      break;

    case GsmMccPPD:
      HashPtr->Data = MC0_CH0_CR_PM_PDWN_CONFIG_PPD_HSH;
      break;

    case GsmMccEnSrxTempRead:
      HashPtr->Data = MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_TEMP_READ_HSH;
      break;

    case GsmMccSrxZqcal:
      HashPtr->Data = MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_ZQCAL_HSH;
      break;

    case GsmMccEcDis:
      HashPtr->Data = MC0_IBECC_CONTROL_EC_DIS_HSH;
      break;

    case GsmMccEccGranularity:
      HashPtr->Data = MC0_IBECC_CONTROL_ECC_GRANULARITY_HSH;
      break;

    case GsmMccAutoPrechargeEn:
      HashPtr->Data = MC0_CH0_CR_AUTO_PRE_CONTROL_AUTO_PRE_EN_HSH;
      break;

    case GsmMccEnCsGearDown:
      HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_cs_geardown_enable_HSH;
      break;

    case GsmMccCfiE2eParityEnRdata:
      HashPtr->Data = MC0_PARITY_CONTROL_RDATA_PARITY_EN_HSH;
      break;

    case GsmMccCfiE2eParityEnRdCmplHeader:
      HashPtr->Data = MC0_PARITY_CONTROL_RDCPL_HEADER_PARITY_EN_HSH;
      break;

    case GsmMccCfiE2eParityEn:
      HashPtr->Data = MC0_PARITY_CONTROL_PARITY_EN_HSH;
      break;

    case GsmScPbrEcsRefabEnable:
      HashPtr->Data = MC0_CH0_CR_SC_PBR_ECS_REFab_Enable_HSH;
      break;

    case GsmScPbrEcsRefabPeriod:
      HashPtr->Data = MC0_CH0_CR_SC_PBR_ECS_REFab_Period_HSH;
      break;

    case GsmMccDdr5CkdEnable:
      HashPtr->Data = MC0_CH0_CR_SC_GS_CFG_DDR5_CKD_enable_HSH;
      break;

    case GsmMccDisLpddr5RdwrInterleaving:
      HashPtr->Data = MC0_CH0_CR_MCSCHEDS_SPARE_dis_lpddr5_rdwr_interleaving_HSH;
      break;

    case GsmMccEccCorrectionDisable:
      HashPtr->Data = MC0_CH0_CR_ECC_DFT_ECC_correction_disable_HSH;
      break;

    case GsmMccMcMntsSpareRw:
      HashPtr->Data = MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_HSH;
      break;

    case GsmMccMcMntsRfFastSleepDisable:
      HashPtr->Data = MC0_CH0_CR_MCMNTS_SPARE_rf_fast_sleep_disable_HSH;
      break;

    case GsmMccMcCbRowPressDisAutoPre:
      HashPtr->Data = MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_autopre_new_req_miss_HSH;
      break;

    case GsmMccPageOpenPolicyMaxCount:
      HashPtr->Data = MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_MaxCount_HSH;
      break;

    case GsmMccPageOpenPolicyEn:
      HashPtr->Data = MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_Enable_HSH;
      break;

    default:
      IsDefined = FALSE;
      break;
  }

  if(!IsDefined) {
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Hash undefined. Group %s(%d) Channel %u\n", GsmGtDebugStrings[Group], Group, Channel);
    HashPtr->Data = MRC_UINT32_MAX;
    Status = mrcWrongInputParameter;
  }

  if (Status == mrcSuccess) {
    CrOffset = MrcGetMcConfigRegOffset (MrcData, Group, Socket, Controller, Channel, Rank, Strobe, FreqIndex, VolatileMask);
  }

  HashPtr->Bits.Offset = CrOffset;

  return Status;
}


/**
  This function returns the hash used to execute the Get/Set function.
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
MrcGetPmaHash (
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
  MRC_REGISTER_HASH_STRUCT *HashPtr;
  UINT32 CrOffset;

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Debug   = &MrcData->Outputs.Debug;
#endif
  Status  = mrcSuccess;
  HashPtr = (MRC_REGISTER_HASH_STRUCT *) HashVal;
  CrOffset = MRC_UINT32_MAX;

  switch (Group) {
    case GsmPmaEnableMc:
      HashPtr->Data = (Controller) ? MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC1_EN_HSH : MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC0_EN_HSH;
      break;

    case GsmPmaEnableIbecc:
      HashPtr->Data = (Controller) ? MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC1_EN_HSH : MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC0_EN_HSH;
      break;

    case GsmPmaEnableCce:
      HashPtr->Data = (Controller) ? MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE1_EN_HSH : MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE0_EN_HSH;
      break;

    case GsmPmaEnableTme:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_MEM_CONFIG_TME_EN_HSH;
      break;

    case GsmPmaDdrType:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_HSH;
      break;

    case GsmPmaMaxBandwidth:
      switch (FreqIndex) {
        case MrcSaGvPoint0:
          HashPtr->Data = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_HSH;
          break;

        case MrcSaGvPoint1:
          HashPtr->Data = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_MAX_BW_1_GBPS_HSH;
          break;

        case MrcSaGvPoint2:
          HashPtr->Data = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_MAX_BW_2_GBPS_HSH;
          break;

        case MrcSaGvPoint3:
          HashPtr->Data = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_MAX_BW_3_GBPS_HSH;
          break;

        default:
          MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "PMA Max Bandwidth Undefined Group %s(%d) FreqIndex %u\n", GsmGtDebugStrings[Group], Group, FreqIndex);
          HashPtr->Data = MRC_UINT32_MAX;
          Status = mrcWrongInputParameter;
          break;
      }
      break;

    case GsmPmaQclkRatioReq:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_HSH;
      break;

    case GsmPmaGearTypeReq:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_REQ_GEAR_TYPE_HSH;
      break;

    case GsmPmaDvfsVddq:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_HSH;
      break;

    case GsmPmaVddqChangeOnly:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_REQ_VDDQ_CHANGE_ONLY_HSH;
      break;

    case GsmPmaMaxBwGbps:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_HSH;
      break;

    case GsmPmaQclkWpIdx:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_HSH;
      break;

    case GsmPmaRunBusy:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_REQ_RUN_BUSY_HSH;
      break;

    case GsmPmaMrcSave:
      switch (FreqIndex) {
        case MrcSaGvPoint0:
          HashPtr->Data = MEMSS_PMA_CR_INIT_STATE_MRC_SAVE0_HSH;
          break;

        case MrcSaGvPoint1:
          HashPtr->Data = MEMSS_PMA_CR_INIT_STATE_MRC_SAVE1_HSH;
          break;

        case MrcSaGvPoint2:
          HashPtr->Data = MEMSS_PMA_CR_INIT_STATE_MRC_SAVE2_HSH;
          break;

        case MrcSaGvPoint3:
          HashPtr->Data = MEMSS_PMA_CR_INIT_STATE_MRC_SAVE3_HSH;
          break;

        default:
          MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "PMA MrcSave FreqIndex Undefined Group %s(%d) FreqIndex %u\n", GsmGtDebugStrings[Group], Group, FreqIndex);
          HashPtr->Data = MRC_UINT32_MAX;
          Status = mrcWrongInputParameter;
          break;
      }
      break;

    case GsmPmaMemConfigDone:
      HashPtr->Data = MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_HSH;
      break;

    case GsmPmaMemConfigDoneAck:
      HashPtr->Data = MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_ACK_HSH;
      break;

    case GsmPmaQclkRatioData:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_DATA_QCLK_RATIO_HSH;
      break;

    case GsmPmaGearTypeData:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_DATA_GEAR_TYPE_HSH;
      break;

    case GsmPmaErrorCode:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_HSH;
      break;

    case GsmPmaBiosMailboxReserved:
      HashPtr->Data = MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_HSH;
      break;

    default:
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Hash undefined. Group %s(%d) Channel %u\n", GsmGtDebugStrings[Group], Group, Channel);
      HashPtr->Data = MRC_UINT32_MAX;
      Status = mrcWrongInputParameter;
      break;
  }

  if (Status == mrcSuccess) {
    CrOffset = MrcGetPmaRegOffset (MrcData, Group, FreqIndex, VolatileMask);
  }

  HashPtr->Bits.Offset = CrOffset;

  return Status;
}

