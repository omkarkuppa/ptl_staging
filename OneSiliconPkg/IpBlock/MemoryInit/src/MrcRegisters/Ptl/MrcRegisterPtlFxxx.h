#ifndef __MrcRegisterPtlFxxx_h__
#define __MrcRegisterPtlFxxx_h__
/** @file
  This file was automatically generated. Modify at your own risk.
  Note that no error checking is done in these functions so ensure that the correct values are passed.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#pragma pack(push, 1)


#define MC0_BC_CR_TC_PRE_REG                                           (0x0000F000)
//Duplicate of MC0_CH0_CR_TC_PRE_REG

#define MC0_BC_CR_TC_RDRD_REG                                          (0x0000F00C)
//Duplicate of MC0_CH0_CR_TC_RDRD_REG

#define MC0_BC_CR_TC_RDWR_REG                                          (0x0000F010)
//Duplicate of MC0_CH0_CR_TC_RDWR_REG

#define MC0_BC_CR_TC_WRRD_REG                                          (0x0000F014)
//Duplicate of MC0_CH0_CR_TC_WRRD_REG

#define MC0_BC_CR_TC_WRWR_REG                                          (0x0000F018)
//Duplicate of MC0_CH0_CR_TC_WRWR_REG

#define MC0_BC_CR_PM_ADAPTIVE_CKE_REG                                  (0x0000F01C)
//Duplicate of MC0_CH0_CR_PM_ADAPTIVE_CKE_REG

#define MC0_BC_CR_SC_ROUNDTRIP_LATENCY_REG                             (0x0000F020)
//Duplicate of MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_REG

#define MC0_BC_CR_PM_PDWN_CONFIG_UARCH_REG                             (0x0000F024)
//Duplicate of MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_REG

#define MC0_BC_CR_SCHED_CBIT_REG                                       (0x0000F028)
//Duplicate of MC0_CH0_CR_SCHED_CBIT_REG

#define MC0_BC_CR_SCHED_SECOND_CBIT_REG                                (0x0000F02C)
//Duplicate of MC0_CH0_CR_SCHED_SECOND_CBIT_REG

#define MC0_BC_CR_DFT_MISC_REG                                         (0x0000F030)
//Duplicate of MC0_CH0_CR_DFT_MISC_REG

#define MC0_BC_CR_SC_PCIT_REG                                          (0x0000F034)
//Duplicate of MC0_CH0_CR_SC_PCIT_REG

#define MC0_BC_CR_ECC_DFT_REG                                          (0x0000F038)
//Duplicate of MC0_CH0_CR_ECC_DFT_REG

#define MC0_BC_CR_ECC_DFT_UARCH_REG                                    (0x0000F03C)
//Duplicate of MC0_CH0_CR_ECC_DFT_UARCH_REG

#define MC0_BC_CR_PM_PDWN_CONFIG_REG                                   (0x0000F040)
//Duplicate of MC0_CH0_CR_PM_PDWN_CONFIG_REG

#define MC0_BC_CR_ECCERRLOG0_REG                                       (0x0000F048)
//Duplicate of MC0_CH0_CR_ECCERRLOG0_REG

#define MC0_BC_CR_ECCERRLOG1_REG                                       (0x0000F04C)
//Duplicate of MC0_CH0_CR_ECCERRLOG1_REG

#define MC0_BC_CR_TC_PWRDN_REG                                         (0x0000F050)
//Duplicate of MC0_CH0_CR_TC_PWRDN_REG

#define MC0_BC_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_REG                     (0x0000F058)
//Duplicate of MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_REG

#define MC0_BC_CR_QUEUE_ENTRY_DISABLE_IPQ_REG                          (0x0000F05C)
//Duplicate of MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_REG

#define MC0_BC_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_REG                      (0x0000F060)
//Duplicate of MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_REG

#define MC0_BC_CR_SC_WDBWM_REG                                         (0x0000F068)
//Duplicate of MC0_CH0_CR_SC_WDBWM_REG

#define MC0_BC_CR_TC_CAS_REG                                           (0x0000F070)
//Duplicate of MC0_CH0_CR_TC_CAS_REG

#define MC0_BC_CR_MCSCHEDS_SPARE_REG                                   (0x0000F078)
//Duplicate of MC0_CH0_CR_MCSCHEDS_SPARE_REG

#define MC0_BC_CR_TC_MPC_REG                                           (0x0000F07C)
//Duplicate of MC0_CH0_CR_TC_MPC_REG

#define MC0_BC_CR_SC_ODT_MATRIX_REG                                    (0x0000F080)
//Duplicate of MC0_CH0_CR_SC_ODT_MATRIX_REG

#define MC0_BC_CR_DFT_BLOCK_REG                                        (0x0000F084)
//Duplicate of MC0_CH0_CR_DFT_BLOCK_REG

#define MC0_BC_CR_SC_GS_CFG_REG                                        (0x0000F088)
//Duplicate of MC0_CH0_CR_SC_GS_CFG_REG

#define MC0_BC_CR_SC_GS_CFG_UARCH_REG                                  (0x0000F08C)
//Duplicate of MC0_CH0_CR_SC_GS_CFG_UARCH_REG

#define MC0_BC_CR_SC_PH_THROTTLING_0_REG                               (0x0000F090)
//Duplicate of MC0_CH0_CR_SC_PH_THROTTLING_0_REG

#define MC0_BC_CR_SC_PH_THROTTLING_1_REG                               (0x0000F094)
//Duplicate of MC0_CH0_CR_SC_PH_THROTTLING_1_REG

#define MC0_BC_CR_SC_PH_THROTTLING_2_REG                               (0x0000F098)
//Duplicate of MC0_CH0_CR_SC_PH_THROTTLING_2_REG

#define MC0_BC_CR_SC_PH_THROTTLING_3_REG                               (0x0000F09C)
//Duplicate of MC0_CH0_CR_SC_PH_THROTTLING_3_REG

#define MC0_BC_CR_SC_WPQ_THRESHOLD_REG                                 (0x0000F0A0)
//Duplicate of MC0_CH0_CR_SC_WPQ_THRESHOLD_REG

#define MC0_BC_CR_SC_PR_CNT_CONFIG_REG                                 (0x0000F0A8)
//Duplicate of MC0_CH0_CR_SC_PR_CNT_CONFIG_REG

#define MC0_BC_CR_REUT_CH_MISC_CKE_CS_CTRL_REG                         (0x0000F0B0)
//Duplicate of MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_REG

#define MC0_BC_CR_SC_GS_CFG_TRAINING_REG                               (0x0000F0B4)
//Duplicate of MC0_CH0_CR_SC_GS_CFG_TRAINING_REG

#define MC0_BC_CR_SPID_LOW_POWER_CTL_REG                               (0x0000F0B8)
//Duplicate of MC0_CH0_CR_SPID_LOW_POWER_CTL_REG

#define MC0_BC_CR_SCHED_THIRD_CBIT_REG                                 (0x0000F0C0)
//Duplicate of MC0_CH0_CR_SCHED_THIRD_CBIT_REG

#define MC0_BC_CR_DEADLOCK_BREAKER_REG                                 (0x0000F0C4)
//Duplicate of MC0_CH0_CR_DEADLOCK_BREAKER_REG

#define MC0_BC_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG_REG                  (0x0000F0D0)
//Duplicate of MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_REG

#define MC0_BC_CR_SC_BLOCKING_RULES_CFG_REG                            (0x0000F0E0)
//Duplicate of MC0_CH0_CR_SC_BLOCKING_RULES_CFG_REG

#define MC0_BC_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REG             (0x0000F0E8)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REG

#define MC0_BC_CR_PWM_DDR_SUBCH1_REQ_OCCUPANCY_COUNTER_REG             (0x0000F0F0)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REG

#define MC0_BC_CR_TR_RRDVALID_CTRL_REG                                 (0x0000F100)
//Duplicate of MC0_CH0_CR_TR_RRDVALID_CTRL_REG

#define MC0_BC_CR_TR_RRDVALID_DATA_REG                                 (0x0000F104)
//Duplicate of MC0_CH0_CR_TR_RRDVALID_DATA_REG

#define MC0_BC_CR_TC_WCK_REG                                           (0x0000F108)
//Duplicate of MC0_CH0_CR_TC_WCK_REG

#define MC0_BC_CR_WMM_READ_CONFIG_REG                                  (0x0000F110)
//Duplicate of MC0_CH0_CR_WMM_READ_CONFIG_REG

#define MC0_BC_CR_MC2PHY_BGF_CTRL_REG                                  (0x0000F114)
//Duplicate of MC0_CH0_CR_MC2PHY_BGF_CTRL_REG

#define MC0_BC_CR_SC_ADAPTIVE_PCIT_REG                                 (0x0000F118)
//Duplicate of MC0_CH0_CR_SC_ADAPTIVE_PCIT_REG

#define MC0_BC_CR_ROWHAMMER_CTL_REG                                    (0x0000F120)
//Duplicate of MC0_CH0_CR_ROWHAMMER_CTL_REG

#define MC0_BC_CR_MERGE_REQ_READS_PQ_REG                               (0x0000F128)
//Duplicate of MC0_CH0_CR_MERGE_REQ_READS_PQ_REG

#define MC0_BC_CR_DFI_CTL_REG                                          (0x0000F12C)
//Duplicate of MC0_CH0_CR_DFI_CTL_REG

#define MC0_BC_CR_AUTO_PRE_CONTROL_REG                                 (0x0000F130)
//Duplicate of MC0_CH0_CR_AUTO_PRE_CONTROL_REG

#define MC0_BC_CR_TC_ACT_REG                                           (0x0000F138)
//Duplicate of MC0_CH0_CR_TC_ACT_REG

#define MC0_BC_CR_QUEUE_ENTRY_DISABLE_RPQ_REG                          (0x0000F140)
//Duplicate of MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_REG

#define MC0_BC_CR_WCK_CONFIG_REG                                       (0x0000F148)
//Duplicate of MC0_CH0_CR_WCK_CONFIG_REG

#define MC0_BC_CR_SC_WDBWM_UARCH_REG                                   (0x0000F158)
//Duplicate of MC0_CH0_CR_SC_WDBWM_UARCH_REG

#define MC0_BC_CR_ROWHAMMER_CTL_UARCH_REG                              (0x0000F15C)
//Duplicate of MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REG

#define MC0_BC_CR_RH_TRR_CONTROL_REG                                   (0x0000F400)
//Duplicate of MC0_CH0_CR_RH_TRR_CONTROL_REG

#define MC0_BC_CR_REUT_CH_MISC_REFRESH_CTRL_REG                        (0x0000F404)
//Duplicate of MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_REG

#define MC0_BC_CR_REUT_CH_MISC_ZQ_CTRL_REG                             (0x0000F408)
//Duplicate of MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_REG

#define MC0_BC_CR_TC_REFM_REG                                          (0x0000F40C)
//Duplicate of MC0_CH0_CR_TC_REFM_REG

#define MC0_BC_CR_RH_TRR_CONTROL_UARCH_REG                             (0x0000F410)
//Duplicate of MC0_CH0_CR_RH_TRR_CONTROL_UARCH_REG

#define MC0_BC_CR_DDR_MR_COMMAND_REG                                   (0x0000F414)
//Duplicate of MC0_CH0_CR_DDR_MR_COMMAND_REG

#define MC0_BC_CR_SCH0_DDR_MR_RESULT_LOW_REG                           (0x0000F418)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_REG

#define MC0_BC_CR_SCH0_DDR_MR_RESULT_HIGH_REG                          (0x0000F41C)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_REG

#define MC0_BC_CR_SCH0_DDR_MR_RESULT_ECC_REG                           (0x0000F420)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_REG

#define MC0_BC_CR_RANK_TEMPERATURE_REG                                 (0x0000F424)
//Duplicate of MC0_CH0_CR_RANK_TEMPERATURE_REG

#define MC0_BC_CR_PM_CONFIG_THERM_STATUS_REG                           (0x0000F428)
//Duplicate of MC0_CH0_CR_PM_CONFIG_THERM_STATUS_REG

#define MC0_BC_CR_MC_RDB_CREDITS_REG                                   (0x0000F42C)
//Duplicate of MC0_CH0_CR_MC_RDB_CREDITS_REG

#define MC0_BC_CR_RH_TRR_ADDRESS_REG                                   (0x0000F430)
//Duplicate of MC0_CH0_CR_RH_TRR_ADDRESS_REG

#define MC0_BC_CR_MC_INIT_STATE_UARCH_REG                              (0x0000F434)
//Duplicate of MC0_CH0_CR_MC_INIT_STATE_UARCH_REG

#define MC0_BC_CR_RFP_REG                                              (0x0000F438)
//Duplicate of MC0_CH0_CR_RFP_REG

#define MC0_BC_CR_RFP_UARCH_REG                                        (0x0000F440)
//Duplicate of MC0_CH0_CR_RFP_UARCH_REG

#define MC0_BC_CR_MC_REFRESH_STAGGER_REG                               (0x0000F444)
//Duplicate of MC0_CH0_CR_MC_REFRESH_STAGGER_REG

#define MC0_BC_CR_TC_ZQCAL_REG                                         (0x0000F448)
//Duplicate of MC0_CH0_CR_TC_ZQCAL_REG

#define MC0_BC_CR_ZQCAL_CONTROL_REG                                    (0x0000F44C)
//Duplicate of MC0_CH0_CR_ZQCAL_CONTROL_REG

#define MC0_BC_CR_SCH1_DDR_MR_RESULT_LOW_REG                           (0x0000F450)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_REG

#define MC0_BC_CR_MC_INIT_STATE_REG                                    (0x0000F454)
//Duplicate of MC0_CH0_CR_MC_INIT_STATE_REG

#define MC0_BC_CR_WDB_VISA_SEL_REG                                     (0x0000F458)
//Duplicate of MC0_CH0_CR_WDB_VISA_SEL_REG

#define MC0_BC_CR_REF_FSM_STATUS_REG                                   (0x0000F460)
//Duplicate of MC0_CH0_CR_REF_FSM_STATUS_REG

#define MC0_BC_CR_PM_DIMM_ACT_ENERGY_REG                               (0x0000F468)
//Duplicate of MC0_CH0_CR_PM_DIMM_ACT_ENERGY_REG

#define MC0_BC_CR_PM_DIMM_RD_ENERGY_REG                                (0x0000F46C)
//Duplicate of MC0_CH0_CR_PM_DIMM_RD_ENERGY_REG

#define MC0_BC_CR_PM_DIMM_WR_ENERGY_REG                                (0x0000F470)
//Duplicate of MC0_CH0_CR_PM_DIMM_WR_ENERGY_REG

#define MC0_BC_CR_PM_SREF_CONTROL_REG                                  (0x0000F474)
//Duplicate of MC0_CH0_CR_PM_SREF_CONTROL_REG

#define MC0_BC_CR_SC_WR_DELAY_REG                                      (0x0000F478)
//Duplicate of MC0_CH0_CR_SC_WR_DELAY_REG

#define MC0_BC_CR_READ_RETURN_DFT_REG                                  (0x0000F47C)
//Duplicate of MC0_CH0_CR_READ_RETURN_DFT_REG

#define MC0_BC_CR_SCH0_BIT_DESWIZZLE_REG                               (0x0000F480)
//Duplicate of MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG

#define MC0_BC_CR_SCH1_BIT_DESWIZZLE_REG                               (0x0000F484)
//Duplicate of MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG

#define MC0_BC_CR_SC_PBR_REG                                           (0x0000F488)
//Duplicate of MC0_CH0_CR_SC_PBR_REG

#define MC0_BC_CR_DVFSQ_CTL_REG                                        (0x0000F48C)
//Duplicate of MC0_CH0_CR_DVFSQ_CTL_REG

#define MC0_BC_CR_TC_MR_REG                                            (0x0000F490)
//Duplicate of MC0_CH0_CR_TC_MR_REG

#define MC0_BC_CR_SCH1_DDR_MR_RESULT_HIGH_REG                          (0x0000F498)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_REG

#define MC0_BC_CR_SCH1_DDR_MR_RESULT_ECC_REG                           (0x0000F49C)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_REG

#define MC0_BC_CR_TC_RFTP_REG                                          (0x0000F4A0)
//Duplicate of MC0_CH0_CR_TC_RFTP_REG

#define MC0_BC_CR_MRS_CR_MAILBOX_REG                                   (0x0000F4A8)
//Duplicate of MC0_CH0_CR_MRS_CR_MAILBOX_REG

#define MC0_BC_CR_SCH0_BYTE_DESWIZZLE_REG                              (0x0000F4B8)
//Duplicate of MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG

#define MC0_BC_CR_SCH1_BYTE_DESWIZZLE_REG                              (0x0000F4BC)
//Duplicate of MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG

#define MC0_BC_CR_TC_SREXITTP_REG                                      (0x0000F4C0)
//Duplicate of MC0_CH0_CR_TC_SREXITTP_REG

#define MC0_BC_CR_RETRAINING_OSCL_PARAMS_REG                           (0x0000F4C8)
//Duplicate of MC0_CH0_CR_RETRAINING_OSCL_PARAMS_REG

#define MC0_BC_CR_MRH_GENERIC_COMMAND_REG                              (0x0000F4CC)
//Duplicate of MC0_CH0_CR_MRH_GENERIC_COMMAND_REG

#define MC0_BC_CR_TC_RETRAINING_OSCL_REG                               (0x0000F4D0)
//Duplicate of MC0_CH0_CR_TC_RETRAINING_OSCL_REG

#define MC0_BC_CR_FORCE_DRAM_MNT_FLOW_REG                              (0x0000F4D4)
//Duplicate of MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REG

#define MC0_BC_CR_WDB_RD_WR_DFX_DATA_REG                               (0x0000F4D8)
//Duplicate of MC0_CH0_CR_WDB_RD_WR_DFX_DATA_REG

#define MC0_BC_CR_WDB_RD_WR_DFX_CTL_REG                                (0x0000F4E0)
//Duplicate of MC0_CH0_CR_WDB_RD_WR_DFX_CTL_REG

#define MC0_BC_CR_WDB_MBIST_0_REG                                      (0x0000F4E8)
//Duplicate of MC0_CH0_CR_WDB_MBIST_0_REG

#define MC0_BC_CR_WDB_MBIST_1_REG                                      (0x0000F4EC)
//Duplicate of MC0_CH0_CR_WDB_MBIST_0_REG

#define MC0_BC_CR_RDB_MBIST_REG                                        (0x0000F4F8)
//Duplicate of MC0_CH0_CR_WDB_MBIST_0_REG

#define MC0_BC_CR_ECC_INJECT_COUNT_REG                                 (0x0000F4FC)
//Duplicate of MC0_CH0_CR_ECC_INJECT_COUNT_REG

#define MC0_BC_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_REG                    (0x0000F500)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_REG

#define MC0_BC_CR_PWM_DDR_SUBCH1_RDDATA_COUNTER_REG                    (0x0000F508)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_REG

#define MC0_BC_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_REG                    (0x0000F510)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_REG

#define MC0_BC_CR_PWM_DDR_SUBCH1_WRDATA_COUNTER_REG                    (0x0000F518)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_REG

#define MC0_BC_CR_WDB_CAPTURE_STATUS_REG                               (0x0000F520)
//Duplicate of MC0_CH0_CR_WDB_CAPTURE_STATUS_REG

#define MC0_BC_CR_WDB_CAPTURE_CTL_REG                                  (0x0000F524)
//Duplicate of MC0_CH0_CR_WDB_CAPTURE_CTL_REG

#define MC0_BC_CR_MRH_CONFIG_REG                                       (0x0000F528)
//Duplicate of MC0_CH0_CR_MRH_CONFIG_REG

#define MC0_BC_CR_PAGEOPEN_POLICY_LIMIT_REG                            (0x0000F52C)
//Duplicate of MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_REG

#define MC0_BC_CR_RH_TRR_LFSR0_REG                                     (0x0000F530)
//Duplicate of MC0_CH0_CR_RH_TRR_LFSR0_REG

#define MC0_BC_CR_RH_TRR_LFSR1_REG                                     (0x0000F534)
//Duplicate of MC0_CH0_CR_RH_TRR_LFSR0_REG

#define MC0_BC_CR_MRS_FSM_CONTROL_REG                                  (0x0000F5D0)
//Duplicate of MC0_CH0_CR_MRS_FSM_CONTROL_REG

#define MC0_BC_CR_MRS_FSM_CONTROL_UARCH_REG                            (0x0000F5D4)
//Duplicate of MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_REG

#define MC0_BC_CR_MRS_FSM_RUN_REG                                      (0x0000F5D8)
//Duplicate of MC0_CH0_CR_MRS_FSM_RUN_REG

#define MC0_BC_CR_PL_AGENT_CFG_DTF_REG                                 (0x0000F5E0)
//Duplicate of MC0_CH0_CR_PL_AGENT_CFG_DTF_REG

#define MC0_BC_CR_MCMNTS_SPARE2_REG                                    (0x0000F5F8)
//Duplicate of MC0_CH0_CR_MCMNTS_SPARE2_REG

#define MC0_BC_CR_MCMNTS_SPARE_REG                                     (0x0000F5FC)
//Duplicate of MC0_CH0_CR_MCMNTS_SPARE_REG

#define MC0_BC_CR_ZQCAL_CONTROL_UARCH_REG                              (0x0000F600)
//Duplicate of MC0_CH0_CR_ZQCAL_CONTROL_UARCH_REG
#pragma pack(pop)
#endif
