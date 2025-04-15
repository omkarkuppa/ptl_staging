/** @file

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


#include "MrcMcApi.h" // for prototypes of functions implemented here
#include "MrcRegisterPtlExxx.h" // for MC0_CH0_CR_TC_PRE_trppb_MIN
#include "MrcRegisterPtlDxxx.h" // for MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_MIN
#include "MrcDebugPrint.h"  // for MRC_DEBUG_MSG()
#include "MrcHalRegisterAccess.h"  // for GsmGtDebugStrings[]
#include "CMcAddress.h"

/**
  This function returns MC Timing group limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetMcTimingGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  )
{
  MrcStatus      Status;
  MrcDebug       *Debug;
  INT64          Max;
  INT64          Min;
  UINT32         Wait;

  Debug   = &MrcData->Outputs.Debug;
  Status  = mrcSuccess;
  Wait    = 0;
  Min     = MRC_INT64_MIN;
  Max     = MRC_INT64_MAX;

  switch (Group) {
    case GsmMctRP:
      Min = MC0_CH0_CR_TC_PRE_trppb_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PRE_trppb_WID)-1;
      break;

    case GsmMctRFCpb:
      Min = MC0_CH0_CR_TC_RFTP_tRFCpb_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RFTP_tRFCpb_WID)-1;
      break;

    case GsmMctRFM:
      Min = MC0_CH0_CR_TC_REFM_tRFM_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_REFM_tRFM_WID)-1;
      break;

    case GsmMctRPab:
      Min = MC0_CH0_CR_TC_PRE_trpab_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PRE_trpab_WID)-1;
      break;

    case GsmMctRAS:
      Min = MC0_CH0_CR_TC_PRE_tras_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PRE_tras_WID)-1;
      break;

    case GsmMctnWRDiff:
      Min = MC0_CH0_CR_TC_PRE_nWR_diff_MIN;
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PRE_nWR_diff_WID) - 1;
      break;

    case GsmMctRDPRE:
      Min = MC0_CH0_CR_TC_PRE_trdpre_MIN;
      Max = MAX_VALUE (MC0_CH0_CR_TC_PRE_trdpre_WID);
      break;

    case GsmMctPPD:
      Min = 2; // Hardcode to support DDR5
      Max = MC0_CH0_CR_TC_PRE_tppd_MAX;
      break;

    case GsmMctWRPRE:
      Min = MC0_CH0_CR_TC_PRE_twrpre_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PRE_twrpre_WID)-1;
      break;

    case GsmMctFAW:
      Min = MC0_CH0_CR_TC_ACT_tFAW_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_ACT_tFAW_WID)-1;
      break;

    case GsmMctRRDsg:
      Min = MC0_CH0_CR_TC_ACT_tRRD_sg_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_ACT_tRRD_sg_WID)-1;
      break;

    case GsmMctRRDdg:
      Min = MC0_CH0_CR_TC_ACT_tRRD_dg_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_ACT_tRRD_dg_WID)-1;
      break;

    case GsmMctVrcgEnable:
    case GsmMctVrcgDisable:
      Min = MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_MIN;
      Max = MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_MAX;
      break;

    case GsmMctRCD:
      Min = MC0_CH0_CR_TC_ACT_tRCD_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_ACT_tRCD_WID)-1;
      break;

    case GsmMctRCDw:
      Min = MC0_CH0_CR_TC_ACT_tRCDW_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_ACT_tRCDW_WID)-1;
      break;

    case GsmMctRDA2ACT:
      Min = MC0_CH0_CR_TC_ACT_RDA2ACT_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_ACT_RDA2ACT_WID)-1;
      break;

    case GsmMctWRA2ACT:
      Min = MC0_CH0_CR_TC_ACT_WRA2ACT_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_ACT_WRA2ACT_WID)-1;
      break;

    case GsmMctREFSbRd:
      Min = MC0_CH0_CR_TC_RFTP_tREFSBRD_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RFTP_tREFSBRD_WID)-1;
      break;

    case GsmMctLpDeratingExt:
      Min = MC0_CH0_CR_TC_PRE_derating_ext_MIN;
      Max = MC0_CH0_CR_TC_PRE_derating_ext_MAX;
      break;

    case GsmMctRDRDsg:
      Min = MC0_CH0_CR_TC_RDRD_tRDRD_sg_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RDRD_tRDRD_sg_WID)-1;
      break;

    case GsmMctRDRDdg:
      Min = MC0_CH0_CR_TC_RDRD_tRDRD_dg_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RDRD_tRDRD_dg_WID)-1;
      break;

    case GsmMctRDRDdr:
      Min = MC0_CH0_CR_TC_RDRD_tRDRD_dr_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RDRD_tRDRD_dr_WID)-1;
      break;

    case GsmMctRDRDdd:
      Min = MC0_CH0_CR_TC_RDRD_tRDRD_dd_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RDRD_tRDRD_dd_WID)-1;
      break;

    case GsmMctRDWRsg:
      Min = MC0_CH0_CR_TC_RDWR_tRDWR_sg_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RDWR_tRDWR_sg_WID)-1;
      break;

    case GsmMctRDWRdg:
      Min = MC0_CH0_CR_TC_RDWR_tRDWR_dg_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RDWR_tRDWR_dg_WID)-1;
      break;

    case GsmMctRDWRdr:
      Min = MC0_CH0_CR_TC_RDWR_tRDWR_dr_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RDWR_tRDWR_dr_WID)-1;
      break;

    case GsmMctRDWRdd:
      Min = MC0_CH0_CR_TC_RDWR_tRDWR_dd_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_RDWR_tRDWR_dd_WID)-1;
      break;

    case GsmMctWRRDsg:
      Min = MC0_CH0_CR_TC_WRRD_tWRRD_sg_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_WRRD_tWRRD_sg_WID)-1;
      break;

    case GsmMctWRRDdg:
      Min = MC0_CH0_CR_TC_WRRD_tWRRD_dg_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_WRRD_tWRRD_dg_WID)-1;
      break;

    case GsmMctWRRDdr:
      Min = MC0_CH0_CR_TC_WRRD_tWRRD_dr_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_WRRD_tWRRD_dr_WID)-1;
      break;

    case GsmMctWRRDdd:
      Min = MC0_CH0_CR_TC_WRRD_tWRRD_dd_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_WRRD_tWRRD_dd_WID)-1;
      break;

    case GsmMctWRWRsg:
      Min = MC0_CH0_CR_TC_WRWR_tWRWR_sg_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_WRWR_tWRWR_sg_WID)-1;
      break;

    case GsmMctWRWRdg:
      Min = MC0_CH0_CR_TC_WRWR_tWRWR_dg_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_WRWR_tWRWR_dg_WID)-1;
      break;

    case GsmMctWRWRdr:
      Min = MC0_CH0_CR_TC_WRWR_tWRWR_dr_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_WRWR_tWRWR_dr_WID)-1;
      break;

    case GsmMctWRWRdd:
      Min = MC0_CH0_CR_TC_WRWR_tWRWR_dd_MIN;
      Max = MAX_VALUE (MC0_CH0_CR_TC_WRWR_tWRWR_dd_WID);
      break;

    case GsmtccdCasDelta:
      Min = MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_WID)-1;
      break;

    case GsmMctCL:
      Min = MC0_CH0_CR_TC_CAS_tCL_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_CAS_tCL_WID)-1;
      break;

    case GsmMctCWL:
      Min = MC0_CH0_CR_TC_CAS_tCWL_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_CAS_tCWL_WID)-1;
      break;

    case GsmMctCWLAdd:
      Min = MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_MIN;
      Max = MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_MAX;
      break;

    case GsmMctCWLDec:
      Min = MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_MIN;
      Max = MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_MAX;
      break;

    case GsmMctCKE:
      Min = MC0_CH0_CR_TC_PWRDN_tcke_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PWRDN_tcke_WID)-1;
      break;

    case GsmMctXP:
      Min = MC0_CH0_CR_TC_PWRDN_txp_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PWRDN_txp_WID)-1;
      break;

    case GsmMctPRPDEN:
      Min = MC0_CH0_CR_TC_PWRDN_tprpden_MIN;
      Max = MC0_CH0_CR_TC_PWRDN_tprpden_MAX;
      break;

    case GsmMctRDPDEN:
      Min = MC0_CH0_CR_TC_PWRDN_trdpden_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PWRDN_trdpden_WID)-1;
      break;

    case GsmMctWRPDEN:
      Min = MC0_CH0_CR_TC_PWRDN_twrpden_MIN;
      // Use field width max to support LP5 8400
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PWRDN_twrpden_WID)-1;
      break;

    case GsmMctCA2CS:
      Min = MC0_CH0_CR_TC_PWRDN_tcacsh_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PWRDN_tcacsh_WID)-1;
      break;

    case GsmMctCSL:
      Min = MC0_CH0_CR_TC_PWRDN_tcsl_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PWRDN_tcsl_WID)-1;
      break;

    case GsmMctCSH:
      Min = MC0_CH0_CR_TC_PWRDN_tcsh_MIN;
      // Use field width max to support DDR5 max frequencies
      Max = (MRC_BIT0 << MC0_CH0_CR_TC_PWRDN_tcsh_WID)-1;
      break;

    case GsmMctXSDLL:
      Min = MC0_CH0_CR_TC_SREXITTP_tXSDLL_MIN;
      Max = MC0_CH0_CR_TC_SREXITTP_tXSDLL_MAX;
      break;

    case GsmMctXSR:
      Min = MC0_CH0_CR_TC_SREXITTP_tXSR_MIN;
      Max = MC0_CH0_CR_TC_SREXITTP_tXSR_MAX;
      break;

    case GsmMctMrhAfterCommandDelay:
      Min = MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_MIN;
      Max = MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_MAX;
      break;

    case GsmMctLpMode4SrxDelay:
      Min = MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_MIN;
      Max = MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_MAX;
      break;

    case GsmMctSR:
      Min = MC0_CH0_CR_TC_SREXITTP_tSR_MIN;
      Max = MC0_CH0_CR_TC_SREXITTP_tSR_MAX;
      break;

    case GsmMctXSRDs:
      Min = MC0_CH0_CR_TC_SREXITTP_tXSR_DS_MIN;
      Max = MC0_CH0_CR_TC_SREXITTP_tXSR_DS_MAX;
      break;

    case GsmMctMRD:
      Min = MC0_CH0_CR_TC_MR_tMRD_MIN;
      Max = MC0_CH0_CR_TC_MR_tMRD_MAX;
      break;

    case GsmMctZQCAL:
      Min = MC0_CH0_CR_TC_ZQCAL_tZQCAL_MIN;
      Max = MAX_VALUE (MC0_CH0_CR_TC_ZQCAL_tZQCAL_WID);
      break;

    case GsmMcttZQLatch:
      Min = MC0_CH0_CR_TC_ZQCAL_tZQLATCH_MIN;
      Max = MC0_CH0_CR_TC_ZQCAL_tZQLATCH_MAX;
      break;

    case GsmMctZQCalPeriod:
      Min = MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_MIN;
      Max = MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_MAX;
      break;

    case GsmMctCPDED:
      Min = MC0_CH0_CR_TC_PWRDN_tcpded_MIN;
      Max = MC0_CH0_CR_TC_PWRDN_tcpded_MAX;
      break;

    case GsmMctCKCKEH:
      Min = MC0_CH0_CR_TC_PWRDN_tckckeh_MIN;
      Max = MC0_CH0_CR_TC_PWRDN_tckckeh_MAX;
      break;

    case GsmMctSrIdleTimer:
      Min = MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_MIN;
      Max = MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_MAX;
      break;

    case GsmMctSrefDelayQsync:
      Min = MC0_MCDECS_SECOND_CBIT_delay_qsync_MIN;
      Max = MC0_MCDECS_SECOND_CBIT_delay_qsync_MAX;
      break;

    case GsmMctREFI:
      Min = MC0_CH0_CR_TC_RFTP_tREFI_MIN;
      Max = MC0_CH0_CR_TC_RFTP_tREFI_MAX;
      break;

    case GsmMctRFC:
      Min = MC0_CH0_CR_TC_RFTP_tRFC_MIN;
      Max = MC0_CH0_CR_TC_RFTP_tRFC_MAX;
      break;

    case GsmMcDdr5Rir:
      Min = MC0_CH0_CR_RFP_DDR5_RIR_MIN;
      Max = MC0_CH0_CR_RFP_DDR5_RIR_MAX;
      break;

    case GsmMctRefreshHpWm:
      Min = MC0_CH0_CR_RFP_Refresh_HP_WM_MIN;
      Max = MC0_CH0_CR_RFP_Refresh_HP_WM_MAX;
      break;

    case GsmMctRefreshPanicWm:
      Min = MC0_CH0_CR_RFP_Refresh_panic_wm_MIN;
      Max = 8;
      // Max = MC0_CH0_CR_TC_RFP_Refresh_panic_wm_MAX;
      break;

    case GsmMcttOSCO:
      Min = MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_MIN;
      Max = MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_MAX;
      break;

    case GsmMcttPREMRR:
      Min = MC0_CH0_CR_TC_MR_PREMRR_MIN;
      Max = MC0_CH0_CR_TC_MR_PREMRR_MAX;
      break;

    case GsmMctWrOsclRuntime:
      Min = MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_MIN;
      Max = MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_MAX;
      break;

    case GsmMcttMRR:
      Min = MC0_CH0_CR_TC_MR_tMRR_MIN;
      Max = MC0_CH0_CR_TC_MR_tMRR_MAX;
      break;

    case GsmMcttCSLCK:
      Min = MC0_CH0_CR_TC_WCK_tCSLCK_MIN;
      Max = MAX_VALUE (MC0_CH0_CR_TC_WCK_tCSLCK_WID);
      break;

    case GsmMcttWckStop:
      Min = MC0_CH0_CR_TC_WCK_tWCKSTOP_MIN;
      Max = MAX_VALUE (MC0_CH0_CR_TC_WCK_tWCKSTOP_WID);
      break;

    case GsmMctWckEnlFs:
      Min = MC0_CH0_CR_WCK_CONFIG_twck_en_fs_MIN;
      Max = MC0_CH0_CR_WCK_CONFIG_twck_en_fs_MAX;
      break;

    case GsmMctWckEnlRd:
    case GsmMctWckEnlWr:
      Min = MC0_CH0_CR_WCK_CONFIG_twck_en_rd_MIN;
      Max = MC0_CH0_CR_WCK_CONFIG_twck_en_rd_MAX;
      break;

    case GsmMctWckPreToggleFs:
    case GsmMctWckPreToggleRd:
    case GsmMctWckPreToggleWr:
      Min = MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_MIN;
      Max = MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_MAX;
      break;

    case GsmMctWckDfiStop:
      Min = MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_MIN;
      Max = MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_MAX;
      break;

    case GsmMctWckDfiOffset:
      Min = MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_MIN;
      Max = MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_MAX;
      break;

    case GSmMcttWckOff:
      Min = MC0_CH0_CR_TC_WCK_twckoff_MIN;
      Max = MC0_CH0_CR_TC_WCK_twckoff_MAX;
      break;

    case GsmMcttCkFspX:
      Min = MC0_CH0_CR_TC_WCK_tckfspx_MIN;
      Max = MAX_VALUE (MC0_CH0_CR_TC_WCK_tckfspx_WID);
      break;

    default:
      Status = mrcWrongInputParameter;
      break;
  }

  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Group %s(%d) has no limits defined\n", GsmGtDebugStrings[Group], Group);
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
  This function returns MC Config register limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetMcConfigGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  )
{
  MrcStatus      Status;
  MrcDebug       *Debug;
  INT64          Max;
  INT64          Min;
  UINT32         Wait;
  BOOLEAN        IsDefined;

  Debug        = &MrcData->Outputs.Debug;
  Status       = mrcSuccess;
  Wait         = 0;
  Min          = MRC_INT64_MIN;
  Max          = MRC_INT64_MAX;
  IsDefined    = TRUE;

  switch (Group) {
    case GsmMccDramType:
      Min = MC0_CH0_CR_SC_GS_CFG_dram_technology_MIN;
      Max = MC0_CH0_CR_SC_GS_CFG_dram_technology_MAX;
      break;

    case GsmMccCmdStretch:
      Min = MC0_CH0_CR_SC_GS_CFG_cmd_stretch_MIN;
      Max = MC0_CH0_CR_SC_GS_CFG_cmd_stretch_MAX;
      break;

    case GsmMccAddrMirror:
      Min = MC0_CH0_CR_SC_GS_CFG_address_mirror_MIN;
      Max = MC0_CH0_CR_SC_GS_CFG_address_mirror_MAX;
      break;

    case GsmMccFreqPoint:
      Min = MC0_CH0_CR_SC_GS_CFG_frequency_point_MIN;
      Max = MC0_CH0_CR_SC_GS_CFG_frequency_point_MAX;
      break;

    case GsmMccEccMode:
      Min = MC0_MAD_INTER_CHANNEL_ECC_MIN;
      Max = MC0_MAD_INTER_CHANNEL_ECC_MAX;
      // Wait 4 usec after enabling the ECC IO, needed by HW
      Wait = 4;
      break;

    case GsmMccAddrDecodeDdrType:
      Min = MC0_MAD_INTER_CHANNEL_DDR_TYPE_MIN;
      Max = MC0_MAD_INTER_CHANNEL_DDR_TYPE_MAX;
      break;

    case GsmMccCh0Size:
    case GsmMccCh1Size:
      Min = MC0_MAD_INTER_CHANNEL_CH_0_SIZE_MIN;
      Max = MC0_MAD_INTER_CHANNEL_CH_0_SIZE_MAX;
      break;

    case GsmMccWCKDiffLowInIdle:
      Min = MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_MIN;
      Max = MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_MAX;
      break;

    case GsmMccCpgcActive:
      Min = MC0_MC_CPGC_CONTROL_cpgc_active_MIN;
      Max = MC0_MC_CPGC_CONTROL_cpgc_active_MAX;
      break;

    case GsmMccHashLsb:
      Min = MC0_MAD_MC_HASH_Hash_LSB_MIN;
      Max = MC0_MAD_MC_HASH_Hash_LSB_MAX;
      break;

    case GsmMccHashZone1Start:
      Min = MC0_MAD_MC_HASH_Zone1_start_MIN;
      Max = MC0_MAD_MC_HASH_Zone1_start_MAX;
      break;

    case GsmMccSubch0SdramWidth:
      Min = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_MIN;
      Max = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_MAX;
      break;

    case GsmMccSubch0RankCnt:
      Min = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_MIN;
      Max = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_MAX;
      break;

    case GsmMccSubch0Density:
    case GsmMccSubch1Density:
      Min = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_MIN;
      Max = MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_MAX;
      break;

    case GsmMccSubch1SdramWidth:
      Min = MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_MIN;
      Max = MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_MAX;
      break;

    case GsmMccSubch1RankCnt:
      Min = MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_MIN;
      Max = MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_MAX;
      break;

    case GsmMccRankOccupancy:
      Min = MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_MIN;
      Max = MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_MAX;
      break;

    case GsmMccRefreshAbrRelease:
      Min = MC0_CH0_CR_RFP_Refresh_ABR_release_MIN;
      Max = MC0_CH0_CR_RFP_Refresh_ABR_release_MAX;
      break;

    case GsmMccHashMask:
      Min = MC0_MAD_CHANNEL_HASH_HASH_MASK_MIN;
      Max = MC0_MAD_CHANNEL_HASH_HASH_MASK_MAX;
      break;

    case GsmMccLsbMaskBit:
      Min = MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_MIN;
      Max = MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_MAX;
      break;

    case GsmMccOneDpc:
      Min = MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_MIN;
      Max = MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_MAX;
      break;

    case GsmMccCkeOverride:
    case GsmMccCkeOn:
    case GsmMccCsOverride0:
    case GsmMccCsOverrideVal0:
    case GsmMccCsOverride1:
    case GsmMccCsOverrideVal1:
      Min = MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_MIN;
      Max = MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_MAX;
      break;

    case GsmMccRrdValidValue:
      Min = MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_MIN;
      Max = MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_MAX;
      break;

    case GsmMccMinRefRate:
      Min = MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_MIN;
      Max = MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_MAX;
      break;

    case GsmMccMR4Period:
      Min = MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_MIN;
      Max = MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_MAX;
      break;

    case GsmMccDeswizzleByte:
      Min = MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_MIN;
      Max = MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_MAX;
      break;

    case GsmMccDeswizzleBit:
      Min = MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_MIN;
      Max = MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_MAX;
      break;

    case GsmMccMrrResult:
      Min = MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_MIN;
      Max = MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_MAX;
      break;

    case GsmMccLp5BankMode:
      Min = MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_MIN;
      Max = MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_MAX;
      break;

    case GsmMccLp5WckMode:
      Min = MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_MIN;
      Max = MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_MAX;
      break;

    case GsmMccLp5WckFastMode:
      Min = MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_MIN;
      Max = MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_MAX;
      break;

    case GsmMccRefreshRankMask:
      Min = MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_MIN;
      Max = MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_MAX;
      break;

    case GsmMccRhLfsr0:
    case GsmMccRhLfsr1:
      Min = MC0_CH0_CR_RH_TRR_LFSR0_LFSR_MIN;
      Max = MC0_CH0_CR_RH_TRR_LFSR0_LFSR_MAX;
      break;

    case GsmMccRhTRRDimmEnable:
      Min = MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_MIN;
      Max = MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_MAX;
      break;

    case GsmMccRhDrfmEnable:
      Min = MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_MIN;
      Max = MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_MAX;
      break;

    case GsmMccRhLfsr0Mask:
    case GsmMccRhLfsr1Mask:
      Min = MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_MIN;
      Max = MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_MAX;
      break;

    case GsmMccRhMA1Swizzle:
    case GsmMccRhMA2Swizzle:
    case GsmMccMicronFSwizzle:
    case GsmMccMicronRSwizzle:
      Min = MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_MIN;
      Max = MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_MAX;
      break;

    case GsmMccBrc:
      Min = MC0_CH0_CR_RH_TRR_CONTROL_BRC_MIN;
      Max = MC0_CH0_CR_RH_TRR_CONTROL_BRC_MAX;
      break;

    case GsmMccRhHighWM:
      Min = MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_MIN;
      Max = MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_MAX;
      break;

    case GsmMccRhLowWM:
      Min = MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_MIN;
      Max = MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_MAX;
      break;

    case GsmMccRhNormalRefSub:
      Min = MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_MIN;
      Max = MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_MAX;
      break;

    case GsmMccRhRefmSub:
      Min = MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_MIN;
      Max = MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_MAX;
      break;

    case GsmMccOdtMatrixRd:
    case GsmMccOdtMatrixWr:
      Min = MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_MIN;
      Max = MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_MAX;
      break;

    case GsmMccWrtThreshold:
      Min = MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_MIN;
      Max = MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_MAX;
      break;

    case GsmMccPdwnIdleCounterSubch0:
    case GsmMccPdwnIdleCounterSubch1:
      Min = MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_MIN;
      Max = MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_MAX;
      break;

    case GsmMccTTIdleCounter:
      Min = MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_MIN;
      Max = MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_MAX;
      break;

    case GsmMccRetrainPeriod:
      Min = MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_MIN;
      Max = MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_MAX;
      break;

    case GsmMccVC0ReadEntries:
    case GsmMccVC1ReadEntries:
      Min = MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_MIN;
      Max = MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_MAX;
      break;

    case GsmMccRdWckAsyncGap:
    case GsmMccWrWckAsyncGap:
      Min = MC0_CH0_CR_TC_WCK_rd_wck_async_gap_MIN;
      Max = MC0_CH0_CR_TC_WCK_rd_wck_async_gap_MAX;
      break;

    case GsmMccCasStopAdditionalGap:
      Min = MC0_CH0_CR_TC_WCK_casstop_additional_gap_MIN;
      Max = MAX_VALUE (MC0_CH0_CR_TC_WCK_casstop_additional_gap_WID);
      break;

    case GsmMccVc0LatencyGaurdTimerx16:
    case GsmMccLatencySensitiveTimerx16:
      Min = MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_MIN;
      Max = MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_MAX;
      break;

    case GsmMccVc1LatencyGuardTimerx8:
    case GsmMccVc1IsocLatencyGuardTimerx8:
      Min = MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_MIN;
      Max = MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_MAX;
      break;

    case GsmMccCmiSourceId0:
    case GsmMccCmiSourceId1:
    case GsmMccCmiSourceId2:
    case GsmMccCmiSourceId3:
      Min = MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_MIN;
      Max = MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_MAX;
      break;

    case GsmMccDelayPMAckCycles:
      Min = MC0_PM_CONTROL_delay_PM_Ack_cycles_MIN;
      Max = MC0_PM_CONTROL_delay_PM_Ack_cycles_MAX;
      break;

    case GsmMccForceSRState:
      Min = MC0_NORMALMODE_CFG_Force_SR_State_MIN;
      Max = MC0_NORMALMODE_CFG_Force_SR_State_MAX;
      break;

    case GsmMccBg0Hash:
      Min = MC0_MAD_DRAM_HASH_0_bg0hash_MIN;
      Max = MC0_MAD_DRAM_HASH_0_bg0hash_MAX;
      break;

    case GsmScPbrEcsRefabPeriod:
      Min = MC0_CH0_CR_SC_PBR_ECS_REFab_Period_MIN;
      Max = MC0_CH0_CR_SC_PBR_ECS_REFab_Period_MAX;
      break;

    case GsmMccSelfRefreshLength:
      Min = MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_MIN;
      Max = MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_MAX;
      break;

    case GsmMccMcMntsSpareRw:
      Min = MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_MIN;
      Max = MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_MAX;
      break;

    case GsmMccMcMntsRfFastSleepDisable:
    case GsmScPbrEcsRefabEnable:
    case GsmMccEnSrxWrRetraining:
    case GsmMccEnWrRetraining:
    case GsmMccEnRdRetraining:
    case GsmMccSelfRefreshEnable:
    case GsmMccPowerDownEnable:
    case GsmMccIdleEnable:
    case GsmMccAllowRHDebtSR:
    case GsmMccOppSrefEnable:
    case GsmMccRhRefmEn:
    case GsmMccRhEnableDimm0:
    case GsmMccRhEnableDimm1:
    case GsmDisAllCplInterleave:
    case GsmDisVc1CplInterleave:
    case GsmMccExtendedBankHash:
    case GsmMccInOrderIngress:
    case GsmMccZqSerialize:
    case GsmMccGearMode:
    case GsmMccMultiCycCmd:
    case GsmMccCpgcInOrder:
    case GsmMccBlockXarb:
    case GsmMccBlockCke:
    case GsmMccEnableOdtMatrix:
    case GsmMccCmdTriStateDis:
    case GsmMccEnhancedInterleave:
    case GsmMccEnableRefresh:
    case GsmMccSrState:
    case GsmMccForceSreWithFreqChange:
    case GsmMccEnableDclk:
    case GsmMccPureSrx:
    case GsmMccMcSrx:
    case GsmMccEnRefTypeDisplay:
    case GsmMccPbrDis:
    case GsmMccRrdValidTrigger:
    case GsmMccRrdValidOverflow:
    case GsmMccRrdValidSign:
    case GsmMccLpddrCurrentFsp:
    case GsmMccLpddrCurrentFspTrk:
    case GsmMccDisIosfSbClkGate:
    case GsmMccCkDisTristate:
    case GsmMccAllowOppRefBelowWrtThreshold:
    case GsmMccDisCkett:
    case GsmMccAPD:
    case GsmMccPPD:
    case GsmMccEnSrxTempRead:
    case GsmMccSrxZqcal:
    case GsmMccMainGlbDrvGateDis:
    case GsmMccSchedGlbDrvGateDis:
    case GsmMccPageIdleTimerDisable:
    case GsmMccSpineGateDis:
    case GsmMccDis2cByp:
    case GsmMccSchedDisClkGateLocal:
    case GsmMccDisAsyncOdt:
    case GsmMccDisStarvedPriorityOnNewReq:
    case GsmMccDisableSplitAct:
    case GsmMccDisSchedsClkGate:
    case GsmMccHashEnabled:
    case GsmMccEcDis:
    case GsmMccEccGranularity:
    case GsmMccAutoPrechargeEn:
    case GsmMccPdwnLpmodeEnable:
    case GsmMccEnCsGearDown:
    case GsmMccDdr5CkdEnable:
    case GsmMccDisLpddr5RdwrInterleaving:
    case GsmMccEccCorrectionDisable:
    case GsmMccMcCbRowPressDisAutoPre:
    case GsmMccPageOpenPolicyMaxCount:
    case GsmMccPageOpenPolicyEn:
    case GsmMccCfiE2eParityEnRdata:
    case GsmMccCfiE2eParityEnRdCmplHeader:
    case GsmMccCfiE2eParityEn:
      Min = 0;
      Max = 1;
      break;

    default:
      IsDefined = FALSE;
      break;
  }

  if (!IsDefined) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Group %s(%d) has no limits defined\n", GsmGtDebugStrings[Group], Group);
    Status = mrcWrongInputParameter;
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
  This function returns PMA register limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - PMA group to access.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus
**/
MrcStatus
MrcGetPmaGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  )
{
  MrcStatus Status;
  MrcDebug  *Debug;
  INT64     Max;
  INT64     Min;
  UINT32    Wait;

  Debug   = &MrcData->Outputs.Debug;
  Status  = mrcSuccess;
  Wait    = 0;
  Min     = MRC_INT64_MIN;
  Max     = MRC_INT64_MAX;

  switch (Group) {
    case GsmPmaEnableMc:
    case GsmPmaEnableIbecc:
    case GsmPmaEnableCce:
    case GsmPmaEnableTme:
    case GsmPmaGearTypeReq:
    case GsmPmaRunBusy:
    case GsmPmaMrcSave:
    case GsmPmaMemConfigDone:
    case GsmPmaMemConfigDoneAck:
    case GsmPmaGearTypeData:
    case GsmPmaVddqChangeOnly:
      Min = 0;
      Max = 1;
      break;

    case GsmPmaMaxBandwidth:

      Min = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_MIN;
      Max = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_MAX;
      break;

    case GsmPmaDdrType:
      Min = MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_MIN;
      Max = MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_MAX;
      break;

    case GsmPmaQclkRatioReq:
    case GsmPmaQclkRatioData:
      Min = MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_MIN;
      Max = MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_MAX;
      break;

    case GsmPmaDvfsVddq:
      Min = MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_MIN;
      Max = MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_MAX;
      break;

    case GsmPmaMaxBwGbps:
      Min = MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_MIN;
      Max = MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_MAX;
      break;

    case GsmPmaQclkWpIdx:
      Min = MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_MIN;
      Max = MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_MAX;
      break;

     case GsmPmaErrorCode:
      Min = MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_MIN;
      Max = MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_MAX;
      break;

     case GsmPmaBiosMailboxReserved:
      Min = MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_MIN;
      Max = MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_MAX;
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Group %s(%d) has no limits defined\n", GsmGtDebugStrings[Group], Group);
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
