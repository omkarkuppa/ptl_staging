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
#ifndef __MrcRegisterPtl1Dxxx_h__
#define __MrcRegisterPtl1Dxxx_h__
#pragma pack(push, 1)


#define MC1_IBECC_ACTIVATE_REG                                         (0x0001D400)
//Duplicate of MC0_IBECC_ACTIVATE_REG

#define MC1_IBECC_STATUS_REG                                           (0x0001D404)
//Duplicate of MC0_IBECC_STATUS_REG

#define MC1_IBECC_IOSF_SB_EP_CTRL_REG                                  (0x0001D408)
//Duplicate of MC0_IBECC_IOSF_SB_EP_CTRL_REG

#define MC1_IBECC_PROTECT_ADDR_RANGE_0_REG                             (0x0001D40C)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC1_IBECC_PROTECT_ADDR_RANGE_1_REG                             (0x0001D410)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC1_IBECC_PROTECT_ADDR_RANGE_2_REG                             (0x0001D414)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC1_IBECC_PROTECT_ADDR_RANGE_3_REG                             (0x0001D418)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC1_IBECC_PROTECT_ADDR_RANGE_4_REG                             (0x0001D41C)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC1_IBECC_PROTECT_ADDR_RANGE_5_REG                             (0x0001D420)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC1_IBECC_PROTECT_ADDR_RANGE_6_REG                             (0x0001D424)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC1_IBECC_PROTECT_ADDR_RANGE_7_REG                             (0x0001D428)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC1_IBECC_DEFAULT_PG_CP_REG                                    (0x0001D430)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC1_IBECC_DEFAULT_PG_RAC_REG                                   (0x0001D438)
//Duplicate of MC0_IBECC_DEFAULT_PG_RAC_REG

#define MC1_IBECC_DEFAULT_PG_WAC_REG                                   (0x0001D440)
//Duplicate of MC0_IBECC_DEFAULT_PG_WAC_REG

#define MC1_IBECC_DEBUG_PG_CP_REG                                      (0x0001D448)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC1_IBECC_DEBUG_PG_RAC_REG                                     (0x0001D450)
//Duplicate of MC0_IBECC_DEFAULT_PG_RAC_REG

#define MC1_IBECC_DEBUG_PG_WAC_REG                                     (0x0001D458)
//Duplicate of MC0_IBECC_DEBUG_PG_WAC_REG

#define MC1_IBECC_OS_PG_CP_REG                                         (0x0001D460)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC1_IBECC_OS_PG_RAC_REG                                        (0x0001D468)
//Duplicate of MC0_IBECC_DEFAULT_PG_RAC_REG

#define MC1_IBECC_OS_PG_WAC_REG                                        (0x0001D470)
//Duplicate of MC0_IBECC_OS_PG_WAC_REG

#define MC1_IBECC_CLK_GATE_EN_0_REG                                    (0x0001D47C)
//Duplicate of MC0_IBECC_CLK_GATE_EN_0_REG

#define MC1_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REG                     (0x0001D480)
//Duplicate of MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REG

#define MC1_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REG                     (0x0001D484)
//Duplicate of MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REG

#define MC1_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REG                  (0x0001D488)
//Duplicate of MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REG

#define MC1_IBECC_CFI_BLOCK_REG                                        (0x0001D48C)
//Duplicate of MC0_IBECC_CFI_BLOCK_REG

#define MC1_IBECC_MBIST_POST_CTRL_WDB_REG                              (0x0001D490)
//Duplicate of MC0_IBECC_MBIST_POST_CTRL_WDB_REG

#define MC1_IBECC_MBIST_POST_CTRL_RDB_REG                              (0x0001D494)
//Duplicate of MC0_IBECC_MBIST_POST_CTRL_WDB_REG

#define MC1_IBECC_PARITY_CONTROL_REG                                   (0x0001D498)
//Duplicate of MC0_IBECC_PARITY_CONTROL_REG

#define MC1_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_REG                    (0x0001D500)
//Duplicate of MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_REG

#define MC1_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC1_REG                    (0x0001D504)
//Duplicate of MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_REG

#define MC1_IBECC_CFI_PORT_CONTROL_REG                                 (0x0001D508)
//Duplicate of MC0_IBECC_CFI_PORT_CONTROL_REG

#define MC1_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_REG                     (0x0001D50C)
//Duplicate of MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_REG

#define MC1_IBECC_ECC_VC0_RD_REQCOUNT_REG                              (0x0001D520)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_ECC_VC1_RD_REQCOUNT_REG                              (0x0001D528)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_ECC_VC0_WR_REQCOUNT_REG                              (0x0001D530)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_ECC_VC1_WR_REQCOUNT_REG                              (0x0001D538)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_NOECC_VC0_RD_REQCOUNT_REG                            (0x0001D540)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_NOECC_VC1_RD_REQCOUNT_REG                            (0x0001D548)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_NOECC_VC0_WR_REQCOUNT_REG                            (0x0001D550)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_NOECC_VC1_WR_REQCOUNT_REG                            (0x0001D558)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_REQUEST_ARB_CONFIG_REG                               (0x0001D564)
//Duplicate of MC0_IBECC_REQUEST_ARB_CONFIG_REG

#define MC1_IBECC_RESPONSE_ARB_CONFIG_REG                              (0x0001D568)
//Duplicate of MC0_IBECC_REQUEST_ARB_CONFIG_REG

#define MC1_IBECC_ECC_ERROR_LOG_REG                                    (0x0001D570)
//Duplicate of MC0_IBECC_ECC_ERROR_LOG_REG

#define MC1_IBECC_PARITY_ERR_LOG_REG                                   (0x0001D578)
//Duplicate of MC0_IBECC_PARITY_ERR_LOG_REG

#define MC1_IBECC_ECC_INJ_ADDR_MASK_REG                                (0x0001D580)
//Duplicate of MC0_IBECC_ECC_INJ_ADDR_MASK_REG

#define MC1_IBECC_ECC_INJ_ADDR_BASE_REG                                (0x0001D588)
//Duplicate of MC0_IBECC_ECC_INJ_ADDR_MASK_REG

#define MC1_IBECC_PARITY_ERR_INJ_REG                                   (0x0001D590)
//Duplicate of MC0_IBECC_PARITY_ERR_INJ_REG

#define MC1_IBECC_CONTROL_REG                                          (0x0001D594)
//Duplicate of MC0_IBECC_CONTROL_REG

#define MC1_IBECC_ECC_INJ_CONTROL_REG                                  (0x0001D598)
//Duplicate of MC0_IBECC_ECC_INJ_CONTROL_REG

#define MC1_IBECC_MBIST_POST_CTRL_RSB_REG                              (0x0001D5B8)
//Duplicate of MC0_IBECC_MBIST_POST_CTRL_WDB_REG

#define MC1_IBECC_ECC_VC0_SYND_RD_REQCOUNT_REG                         (0x0001D5C0)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_ECC_VC1_SYND_RD_REQCOUNT_REG                         (0x0001D5C8)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_ECC_VC0_SYND_WR_REQCOUNT_REG                         (0x0001D5D0)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_ECC_VC1_SYND_WR_REQCOUNT_REG                         (0x0001D5D8)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC1_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_REG                   (0x0001D5E0)
//Duplicate of MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_REG

#define MC1_IBECC_BLOCK_BIDS_REG                                       (0x0001D5E4)
//Duplicate of MC0_IBECC_BLOCK_BIDS_REG

#define MC1_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG                         (0x0001D5E8)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC1_IBECC_ECC_STORAGE_ADDR_RANGE_1_REG                         (0x0001D5EC)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC1_IBECC_ECC_STORAGE_ADDR_RANGE_2_REG                         (0x0001D5F0)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC1_IBECC_ECC_STORAGE_ADDR_RANGE_3_REG                         (0x0001D5F4)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC1_IBECC_ECC_STORAGE_ADDR_RANGE_4_REG                         (0x0001D5F8)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC1_IBECC_ECC_STORAGE_ADDR_RANGE_5_REG                         (0x0001D5FC)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC1_IBECC_ECC_STORAGE_ADDR_RANGE_6_REG                         (0x0001D600)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC1_IBECC_ECC_STORAGE_ADDR_RANGE_7_REG                         (0x0001D604)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC1_IBECC_MDST_TRIM_FUSE_REG                                   (0x0001D608)
//Duplicate of MC0_IBECC_MDST_TRIM_FUSE_REG

#define MC1_IBECC_ECC_ERR_COUNT_REG                                    (0x0001D610)
//Duplicate of MC0_IBECC_ECC_ERR_COUNT_REG

#define MC1_IBECC_RESTRICTED_PG_CP_REG                                 (0x0001D618)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC1_IBECC_RESTRICTED_PG_RAC_REG                                (0x0001D620)
//Duplicate of MC0_IBECC_DEFAULT_PG_RAC_REG

#define MC1_IBECC_RESTRICTED_PG_WAC_REG                                (0x0001D628)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC1_IBECC_MEMORY_INIT_CONTROL_REG                              (0x0001D630)
//Duplicate of MC0_IBECC_MEMORY_INIT_CONTROL_REG

#define MC1_IBECC_P1_DATA_TX_CNT_REG                                   (0x0001D634)
//Duplicate of MC0_IBECC_P1_DATA_TX_CNT_REG

#define MC1_IBECC_P1_DATA_RX_CNT_REG                                   (0x0001D638)
//Duplicate of MC0_IBECC_P1_DATA_RX_CNT_REG

#define MC1_IBECC_P0_DATA_TX_CNT_REG                                   (0x0001D63C)
//Duplicate of MC0_IBECC_P1_DATA_TX_CNT_REG

#define MC1_IBECC_P0_DATA_RX_CNT_REG                                   (0x0001D640)
//Duplicate of MC0_IBECC_P1_DATA_RX_CNT_REG

#define MC1_IBECC_ADDR_HASH_REG                                        (0x0001D644)
//Duplicate of MC0_IBECC_ADDR_HASH_REG

#define MC1_IBECC_IP_VERSION_0_0_0_IBECCHBAR_REG                       (0x0001D648)
//Duplicate of MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_REG

#define MC1_IBECC_CFI_HOST_REG                                         (0x0001D64C)
//Duplicate of MC0_IBECC_CFI_HOST_REG

#define MC1_MAD_INTER_CHANNEL_REG                                      (0x0001D800)
//Duplicate of MC0_MAD_INTER_CHANNEL_REG

#define MC1_MAD_INTRA_CHANNEL_0_REG                                    (0x0001D804)
//Duplicate of MC0_MAD_INTRA_CHANNEL_0_REG

#define MC1_MAD_INTRA_CHANNEL_1_REG                                    (0x0001D808)
//Duplicate of MC0_MAD_INTRA_CHANNEL_0_REG

#define MC1_MAD_DRAM_HASH_0_REG                                        (0x0001D80C)
//Duplicate of MC0_MAD_DRAM_HASH_0_REG

#define MC1_MAD_DRAM_HASH_1_REG                                        (0x0001D810)
//Duplicate of MC0_MAD_DRAM_HASH_0_REG

#define MC1_MCDECS_MISC_REG                                            (0x0001D818)
//Duplicate of MC0_MCDECS_MISC_REG

#define MC1_MCDECS_CBIT_REG                                            (0x0001D81C)
//Duplicate of MC0_MCDECS_CBIT_REG

#define MC1_MAD_CHANNEL_HASH_REG                                       (0x0001D824)
//Duplicate of MC0_MAD_CHANNEL_HASH_REG

#define MC1_MAD_SUB_CHANNEL_HASH_REG                                   (0x0001D828)
//Duplicate of MC0_MAD_SUB_CHANNEL_HASH_REG

#define MC1_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_REG                       (0x0001D82C)
//Duplicate of MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_REG

#define MC1_MC_INIT_STATE_G_REG                                        (0x0001D830)
//Duplicate of MC0_MC_INIT_STATE_G_REG

#define MC1_MC_IP_VERSION_REG                                          (0x0001D834)
//Duplicate of MC0_MC_IP_VERSION_REG

#define MC1_MC_RTL_VERSION_REG                                         (0x0001D838)
//Duplicate of MC0_MC_RTL_VERSION_REG

#define MC1_PM_OPP_SREF_IDLE_TIMER_REG                                 (0x0001D83C)
//Duplicate of MC0_PM_OPP_SREF_IDLE_TIMER_REG

#define MC1_PWM_TOTAL_REQCOUNT_REG                                     (0x0001D840)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC1_PWM_PROGRAMMABLE_REQCOUNT_0_REG                            (0x0001D848)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC1_PWM_PROGRAMMABLE_REQCOUNT_1_REG                            (0x0001D850)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC1_PWM_RDCAS_COUNT_REG                                        (0x0001D858)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC1_PM_OPP_SREF_ENABLE_REG                                     (0x0001D860)
//Duplicate of MC0_PM_OPP_SREF_ENABLE_REG

#define MC1_READ_OCCUPANCY_COUNT_REG                                   (0x0001D868)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC1_STALL_DRAIN_REG                                            (0x0001D874)
//Duplicate of MC0_STALL_DRAIN_REG

#define MC1_IPC_MC_ARB_REG                                             (0x0001D878)
//Duplicate of MC0_IPC_MC_ARB_REG

#define MC1_IPC_MC_DEC_ARB_REG                                         (0x0001D87C)
//Duplicate of MC0_IPC_MC_ARB_REG

#define MC1_QUEUE_CREDIT_C_REG                                         (0x0001D880)
//Duplicate of MC0_QUEUE_CREDIT_C_REG

#define MC1_ECC_INJ_ADDR_COMPARE_REG                                   (0x0001D888)
//Duplicate of MC0_ECC_INJ_ADDR_COMPARE_REG

#define MC1_PWM_WRCAS_COUNT_REG                                        (0x0001D8A0)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC1_PWM_COMMAND_COUNT_REG                                      (0x0001D8A8)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC1_TOLUD_REG                                                  (0x0001D8BC)
//Duplicate of MC0_TOLUD_REG

#define MC1_PWM_ACT_COUNT_REG                                          (0x0001D8C0)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC1_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG                         (0x0001D900)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC1_GDXC_DDR_SYS_ADD_FILTER_MASK_1_REG                         (0x0001D908)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC1_GDXC_DDR_SYS_ADD_FILTER_MATCH_0_REG                        (0x0001D910)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC1_GDXC_DDR_SYS_ADD_FILTER_MATCH_1_REG                        (0x0001D918)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC1_GDXC_DDR_SYS_ADD_TRIGGER_MASK_REG                          (0x0001D920)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC1_GDXC_DDR_SYS_ADD_TRIGGER_MATCH_REG                         (0x0001D928)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC1_SC_QOS_REG                                                 (0x0001D930)
//Duplicate of MC0_SC_QOS_REG

#define MC1_MCMAINS_GLOBAL_DRIVER_GATE_CFG_REG                         (0x0001D938)
//Duplicate of MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_REG

#define MC1_PM_CONTROL_REG                                             (0x0001D93C)
//Duplicate of MC0_PM_CONTROL_REG

#define MC1_CLOCK_SPINE_GATE_CFG_REG                                   (0x0001D940)
//Duplicate of MC0_CLOCK_SPINE_GATE_CFG_REG

#define MC1_PWM_COUNTERS_DURATION_REG                                  (0x0001D948)
//Duplicate of MC0_PWM_COUNTERS_DURATION_REG

#define MC1_MCDECS_SECOND_CBIT_REG                                     (0x0001D954)
//Duplicate of MC0_MCDECS_SECOND_CBIT_REG

#define MC1_ECC_INJ_ADDR_MASK_REG                                      (0x0001D958)
//Duplicate of MC0_ECC_INJ_ADDR_MASK_REG

#define MC1_SC_QOS2_REG                                                (0x0001D960)
//Duplicate of MC0_SC_QOS2_REG

#define MC1_SC_QOS3_REG                                                (0x0001D968)
//Duplicate of MC0_SC_QOS3_REG

#define MC1_NORMALMODE_CFG_REG                                         (0x0001D96C)
//Duplicate of MC0_NORMALMODE_CFG_REG

#define MC1_MC_CPGC_CONTROL_REG                                        (0x0001D970)
//Duplicate of MC0_MC_CPGC_CONTROL_REG

#define MC1_MC_CPGC_CONTROL_UARCH_REG                                  (0x0001D974)
//Duplicate of MC0_MC_CPGC_CONTROL_UARCH_REG

#define MC1_PARITYERRLOG_REG                                           (0x0001D9A0)
//Duplicate of MC0_PARITYERRLOG_REG

#define MC1_PARITY_ERR_INJ_REG                                         (0x0001D9A8)
//Duplicate of MC0_PARITY_ERR_INJ_REG

#define MC1_PARITY_CONTROL_REG                                         (0x0001D9B4)
//Duplicate of MC0_PARITY_CONTROL_REG

#define MC1_MAD_MC_HASH_REG                                            (0x0001D9B8)
//Duplicate of MC0_MAD_MC_HASH_REG

#define MC1_PMON_UNIT_CONTROL_REG                                      (0x0001D9C8)
//Duplicate of MC0_PMON_UNIT_CONTROL_REG

#define MC1_PMON_UNIT_STATUS_REG                                       (0x0001D9D0)
//Duplicate of MC0_PMON_UNIT_STATUS_REG

#define MC1_PMON_COUNTER_CONTROL_0_REG                                 (0x0001D9D8)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC1_PMON_COUNTER_CONTROL_1_REG                                 (0x0001D9E0)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC1_PMON_COUNTER_CONTROL_2_REG                                 (0x0001D9E8)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC1_PMON_COUNTER_CONTROL_3_REG                                 (0x0001D9F0)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC1_PMON_COUNTER_CONTROL_4_REG                                 (0x0001D9F8)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC1_PMON_COUNTER_DATA_0_REG                                    (0x0001DA00)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC1_PMON_COUNTER_DATA_1_REG                                    (0x0001DA08)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC1_PMON_COUNTER_DATA_2_REG                                    (0x0001DA10)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC1_PMON_COUNTER_DATA_3_REG                                    (0x0001DA18)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC1_PMON_COUNTER_DATA_4_REG                                    (0x0001DA20)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC1_OS_TELEMETRY_CONTROL_REG                                   (0x0001DA30)
//Duplicate of MC0_OS_TELEMETRY_CONTROL_REG

#define MC1_PM_OPP_SR_POLICY_0_REG                                     (0x0001DA34)
//Duplicate of MC0_PM_OPP_SR_POLICY_0_REG

#define MC1_PM_OPP_SR_POLICY_1_REG                                     (0x0001DA38)
//Duplicate of MC0_PM_OPP_SR_POLICY_0_REG

#define MC1_MC_CFI_FABRIC_CONTROL_REG                                  (0x0001DA40)
//Duplicate of MC0_MC_CFI_FABRIC_CONTROL_REG

#define MC1_MC_CFI_CONTROL_REG                                         (0x0001DA44)
//Duplicate of MC0_MC_CFI_CONTROL_REG

#define MC1_PM_REQ_FORCE_DFX_REG                                       (0x0001DA48)
//Duplicate of MC0_PM_REQ_FORCE_DFX_REG

#define MC1_DDRPL_CFG2_DTF_REG                                         (0x0001DA4C)
//Duplicate of MC0_DDRPL_CFG2_DTF_REG

#define MC1_SC_QOS_UARCH_REG                                           (0x0001DA50)
//Duplicate of MC0_SC_QOS_UARCH_REG

#define MC1_DDRPL_CFG_DTF_REG                                          (0x0001DF00)
//Duplicate of MC0_DDRPL_CFG_DTF_REG

#define MC1_DDRPL_DEBUG_DTF_REG                                        (0x0001DF08)
//Duplicate of MC0_DDRPL_DEBUG_DTF_REG

#define MC1_DDRPL_VISA_LANES_REG                                       (0x0001DF0C)
//Duplicate of MC0_DDRPL_VISA_LANES_REG

#define MC1_DDRPL_VISA_CFG_DTF_REG                                     (0x0001DF10)
//Duplicate of MC0_DDRPL_VISA_CFG_DTF_REG

#define MC1_DDRPL_DRAMADDRESS_FILTER_REG                               (0x0001DF14)
//Duplicate of MC0_DDRPL_DRAMADDRESS_FILTER_REG

#define MC1_DDRPL_DRAMADDRESS_FILTER2_REG                              (0x0001DF18)
//Duplicate of MC0_DDRPL_DRAMADDRESS_FILTER2_REG
#pragma pack(pop)
#endif
