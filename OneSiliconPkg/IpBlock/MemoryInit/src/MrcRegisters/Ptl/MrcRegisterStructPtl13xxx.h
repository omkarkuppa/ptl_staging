#ifndef __MrcRegisterStructPtl13xxx_h__
#define __MrcRegisterStructPtl13xxx_h__
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
typedef union {
  struct {
    UINT32 DDPCD                                   :  1;  // Bits 0:0
    UINT32 DDRSZ                                   :  2;  // Bits 2:1
    UINT32 FDEE                                    :  1;  // Bits 3:3
    UINT32 ECCDIS                                  :  1;  // Bits 4:4
    UINT32 ECC_EXIST                               :  1;  // Bits 5:5
    UINT32 IBECCDIS                                :  1;  // Bits 6:6
    UINT32 QCLK_GV_DISABLE                         :  1;  // Bits 7:7
    UINT32 Reserved_0                              :  1;  // Bits 8:8
    UINT32 DDR_OC_EN                               :  1;  // Bits 9:9
    UINT32 DDR_SLICE_DIS                           :  2;  // Bits 11:10
    UINT32 MAX_F_QCLK                              :  9;  // Bits 20:12
    UINT32 SMX_DIS                                 :  1;  // Bits 21:21
    UINT32 Reserved_1                              :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_CAPABILITIES_MISC_STRUCT;
typedef union {
  struct {
    UINT32 RESERVED0                               :  1;  // Bits 0:0
    UINT32 RESERVED1                               :  9;  // Bits 9:1
    UINT32 LPDDR5_EN                               :  1;  // Bits 10:10
    UINT32 MAX_F_LPDDR5                            :  9;  // Bits 19:11
    UINT32 DDR5_EN                                 :  1;  // Bits 20:20
    UINT32 MAX_F_DDR5                              :  9;  // Bits 29:21
    UINT32 Reserved                                :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_STRUCT;
typedef union {
  struct {
    UINT32 MOP_DIE_DENSITY                         :  3;  // Bits 2:0
    UINT32 MOP_RANKS                               :  1;  // Bits 3:3
    UINT32 MOP_PACKAGES                            :  2;  // Bits 5:4
    UINT32 MOP_VENDOR                              :  3;  // Bits 8:6
    UINT32 MOP_RESERVED                            :  3;  // Bits 11:9
    UINT32                                         :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_MOP_FUSES_STRUCT;
typedef union {
  struct {
    UINT32 MC0_EN                                  :  1;  // Bits 0:0
    UINT32 MC1_EN                                  :  1;  // Bits 1:1
    UINT32 IBECC0_EN                               :  1;  // Bits 2:2
    UINT32 IBECC1_EN                               :  1;  // Bits 3:3
    UINT32 CCE0_EN                                 :  1;  // Bits 4:4
    UINT32 CCE1_EN                                 :  1;  // Bits 5:5
    UINT32 DDR_TYPE                                :  3;  // Bits 8:6
    UINT32 TME_EN                                  :  1;  // Bits 9:9
    UINT32 Reserved                                :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_MEM_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 MC0_EN                                  :  1;  // Bits 0:0
    UINT32 MC1_EN                                  :  1;  // Bits 1:1
    UINT32 IBECC0_EN                               :  1;  // Bits 2:2
    UINT32 IBECC1_EN                               :  1;  // Bits 3:3
    UINT32 CCE0_EN                                 :  1;  // Bits 4:4
    UINT32 CCE1_EN                                 :  1;  // Bits 5:5
    UINT32 DDR_TYPE                                :  3;  // Bits 8:6
    UINT32 TME_EN                                  :  1;  // Bits 9:9
    UINT32                                         :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_MEM_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 QCLK_RATIO                              :  8;  // Bits 7:0
    UINT32 GEAR_TYPE                               :  1;  // Bits 8:8
    UINT32 DVFS_VDDQ                               :  5;  // Bits 13:9
    UINT32 VDDQ_CHANGE_ONLY                        :  1;  // Bits 14:14
    UINT32                                         :  3;  // Bits 17:15
    UINT32 MAX_BW_GBPS                             :  11;  // Bits 28:18
    UINT32 QCLK_WP_IDX                             :  2;  // Bits 30:29
    UINT32 RUN_BUSY                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_REQ_STRUCT;
typedef union {
  struct {
    UINT32 MRC_SAVE0                               :  1;  // Bits 0:0
    UINT32 MRC_SAVE1                               :  1;  // Bits 1:1
    UINT32 MRC_SAVE2                               :  1;  // Bits 2:2
    UINT32 MRC_SAVE3                               :  1;  // Bits 3:3
    UINT32 MEM_CONFIG_DONE                         :  1;  // Bits 4:4
    UINT32 MEM_CONFIG_DONE_ACK                     :  1;  // Bits 5:5
    UINT32 Reserved                                :  26;  // Bits 31:6
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_INIT_STATE_STRUCT;
typedef union {
  struct {
    UINT32 QCLK_RATIO                              :  8;  // Bits 7:0
    UINT32 GEAR_TYPE                               :  1;  // Bits 8:8
    UINT32                                         :  7;  // Bits 15:9
    UINT32 VCCMEM_VOLTAGE                          :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_DATA_STRUCT;
typedef union {
  struct {
    UINT32 RAMP_VCCSA_EN                           :  1;  // Bits 0:0
    UINT32 ZQ_IS_DISABLED_0                        :  1;  // Bits 1:1
    UINT32 ZQ_IS_DISABLED_1                        :  1;  // Bits 2:2
    UINT32 ZQ_IS_DISABLED_2                        :  1;  // Bits 3:3
    UINT32 ZQ_IS_DISABLED_3                        :  1;  // Bits 4:4
    UINT32 L_SHAPE_CONFIG                          :  1;  // Bits 5:5
    UINT32 EDVFSC_IS_ENABLED_0                     :  1;  // Bits 6:6
    UINT32 EDVFSC_IS_ENABLED_1                     :  1;  // Bits 7:7
    UINT32 EDVFSC_IS_ENABLED_2                     :  1;  // Bits 8:8
    UINT32 EDVFSC_IS_ENABLED_3                     :  1;  // Bits 9:9
    UINT32 Reserved                                :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_MAILBOX_STRUCT;
typedef union {
  struct {
    UINT32 ERROR_CODE                              :  8;  // Bits 7:0
    UINT32                                         :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_ERROR_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 RESERVED0                               :  1;  // Bits 0:0
    UINT32 SB_CGCTRL_CLKGATEDEF                    :  1;  // Bits 1:1
    UINT32 SB_CGCTRL_CLKGATEN                      :  1;  // Bits 2:2
    UINT32 SB_CGCTRL_IDLECNT                       :  8;  // Bits 10:3
    UINT32 MEM_UNLOCK_STS                          :  1;  // Bits 11:11
    UINT32 MEM_UNLOCK_STS_VALID                    :  1;  // Bits 12:12
    UINT32 DFX_DISABLE                             :  1;  // Bits 13:13
    UINT32 DeepSR_Exit_Latency                     :  10;  // Bits 23:14
    UINT32 MRC_START_ACK                           :  1;  // Bits 24:24
    UINT32 ALWAYS_CNT_CKE_LOW                      :  1;  // Bits 25:25
    UINT32 RESERVED1                               :  2;  // Bits 27:26
    UINT32 Reserved                                :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_MISC_ENABLES_STRUCT;
typedef union {
  struct {
    UINT32 QCLK_RATIO_0                            :  8;  // Bits 7:0
    UINT32 GEAR_TYPE                               :  1;  // Bits 8:8
    UINT32 DVFS_VDDQ                               :  5;  // Bits 13:9
    UINT32                                         :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_QCLK_POINT0_STRUCT;
typedef union {
  struct {
    UINT32 QCLK_RATIO_1                            :  8;  // Bits 7:0
    UINT32 GEAR_TYPE                               :  1;  // Bits 8:8
    UINT32 DVFS_VDDQ                               :  5;  // Bits 13:9
    UINT32                                         :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_QCLK_POINT1_STRUCT;
typedef union {
  struct {
    UINT32 QCLK_RATIO_2                            :  8;  // Bits 7:0
    UINT32 GEAR_TYPE                               :  1;  // Bits 8:8
    UINT32 DVFS_VDDQ                               :  5;  // Bits 13:9
    UINT32                                         :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_QCLK_POINT2_STRUCT;
typedef union {
  struct {
    UINT32 QCLK_RATIO_3                            :  8;  // Bits 7:0
    UINT32 GEAR_TYPE                               :  1;  // Bits 8:8
    UINT32 DVFS_VDDQ                               :  5;  // Bits 13:9
    UINT32                                         :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_QCLK_POINT3_STRUCT;
typedef union {
  struct {
    UINT32 MAX_BW_0_GBPS                           :  11;  // Bits 10:0
    UINT32                                         :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_STRUCT;
typedef union {
  struct {
    UINT32 MAX_BW_1_GBPS                           :  11;  // Bits 10:0
    UINT32                                         :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_STRUCT;
typedef union {
  struct {
    UINT32 MAX_BW_2_GBPS                           :  11;  // Bits 10:0
    UINT32                                         :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_STRUCT;
typedef union {
  struct {
    UINT32 MAX_BW_3_GBPS                           :  11;  // Bits 10:0
    UINT32                                         :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_STRUCT;
typedef union {
  struct {
    UINT32 MRC_REVISION                            :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_MRC_VERSION_STRUCT;
typedef union {
  struct {
    UINT32 COUNTDOWN_TIMER_CLOCKS                  :  31;  // Bits 30:0
    UINT32 RUN_BUSY                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_COUNTDOWN_TIMER_STRUCT;
typedef union {
  struct {
    UINT32 SAGTRAIN_ENABLE                         :  1;  // Bits 0:0
    UINT32 SAVE_RUN_BUSY                           :  1;  // Bits 1:1
    UINT32 RESTORE_RUN_BUSY                        :  1;  // Bits 2:2
    UINT32 DLY_COUNTER_STEP                        :  8;  // Bits 10:3
    UINT32 MC0_BLOCK_RUN_BUSY                      :  1;  // Bits 11:11
    UINT32 MC1_BLOCK_RUN_BUSY                      :  1;  // Bits 12:12
    UINT32 IBECC0_BLOCK_RUN_BUSY                   :  1;  // Bits 13:13
    UINT32 IBECC1_BLOCK_RUN_BUSY                   :  1;  // Bits 14:14
    UINT32 CCE0_BLOCK_RUN_BUSY                     :  1;  // Bits 15:15
    UINT32 CCE1_BLOCK_RUN_BUSY                     :  1;  // Bits 16:16
    UINT32 MC0_UNBLOCK_RUN_BUSY                    :  1;  // Bits 17:17
    UINT32 MC1_UNBLOCK_RUN_BUSY                    :  1;  // Bits 18:18
    UINT32 IBECC0_UNBLOCK_RUN_BUSY                 :  1;  // Bits 19:19
    UINT32 IBECC1_UNBLOCK_RUN_BUSY                 :  1;  // Bits 20:20
    UINT32 CCE0_UNBLOCK_RUN_BUSY                   :  1;  // Bits 21:21
    UINT32 CCE1_UNBLOCK_RUN_BUSY                   :  1;  // Bits 22:22
    UINT32 BLOCK_FORMAT_SEL                        :  2;  // Bits 24:23
    UINT32 PG_SEQUENCE_ENABLE                      :  1;  // Bits 25:25
    UINT32 CLK_SPINE_GATING_DISABLE                :  1;  // Bits 26:26
    UINT32 ONDD_BLOCK_RUN_BUSY                     :  1;  // Bits 27:27
    UINT32 ONDD_UNBLOCK_RUN_BUSY                   :  1;  // Bits 28:28
    UINT32 Reserved                                :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_DFX_MISC_ENABLES_STRUCT;
typedef union {
  struct {
    UINT32 VccSAGMEMMC0_STS                        :  1;  // Bits 0:0
    UINT32 VccSAGTrainMC0_STS                      :  1;  // Bits 1:1
    UINT32 VccSAGMEMMC1_STS                        :  1;  // Bits 2:2
    UINT32 VccSAGTrainMC1_STS                      :  1;  // Bits 3:3
    UINT32 VccSAGMEMIBECC0_STS                     :  1;  // Bits 4:4
    UINT32 VccSAGMEMIBECC1_STS                     :  1;  // Bits 5:5
    UINT32 VccSAGMEMCCE0_STS                       :  1;  // Bits 6:6
    UINT32 VccSAGMEMCCE1_STS                       :  1;  // Bits 7:7
    UINT32 VccSAGMEMMC0_RST_STS                    :  1;  // Bits 8:8
    UINT32 VccSAGTrainMC0_RST_STS                  :  1;  // Bits 9:9
    UINT32 VccSAGMEMMC1_RST_STS                    :  1;  // Bits 10:10
    UINT32 VccSAGTrainMC1_RST_STS                  :  1;  // Bits 11:11
    UINT32 VccSAGMEMIBECC0_RST_STS                 :  1;  // Bits 12:12
    UINT32 VccSAGMEMIBECC1_RST_STS                 :  1;  // Bits 13:13
    UINT32 VccSAGMEMCCE0_RST_STS                   :  1;  // Bits 14:14
    UINT32 VccSAGMEMCCE1_RST_STS                   :  1;  // Bits 15:15
    UINT32 CMI_PLL_LOCK_STS                        :  1;  // Bits 16:16
    UINT32 MC0_BLOCK_STS                           :  1;  // Bits 17:17
    UINT32 MC1_BLOCK_STS                           :  1;  // Bits 18:18
    UINT32 IBECC0_BLOCK_STS                        :  1;  // Bits 19:19
    UINT32 IBECC1_BLOCK_STS                        :  1;  // Bits 20:20
    UINT32 CCE0_BLOCK_STS                          :  1;  // Bits 21:21
    UINT32 CCE1_BLOCK_STS                          :  1;  // Bits 22:22
    UINT32 VccSAGMEMCMIPLL_RST_STS                 :  1;  // Bits 23:23
    UINT32 VccSAGMEMDDRPHYPMA_RST_STS              :  1;  // Bits 24:24
    UINT32 VccSAGMEMCMIPLL_STS                     :  1;  // Bits 25:25
    UINT32 VccSAGMEMDDRPHYPMA_STS                  :  1;  // Bits 26:26
    UINT32 VccSAGMEMONDD_RST_STS                   :  1;  // Bits 27:27
    UINT32 ONDD_BLOCK_STS                          :  1;  // Bits 28:28
    UINT32 VccSAGMEMONDD_STS                       :  1;  // Bits 29:29
    UINT32 Reserved                                :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_STRUCT;
typedef union {
  struct {
    UINT32 MC0                                     :  1;  // Bits 0:0
    UINT32 MC1                                     :  1;  // Bits 1:1
    UINT32 IBECC0                                  :  1;  // Bits 2:2
    UINT32 IBECC1                                  :  1;  // Bits 3:3
    UINT32 CCE0                                    :  1;  // Bits 4:4
    UINT32 CCE1                                    :  1;  // Bits 5:5
    UINT32 Reserved                                :  26;  // Bits 31:6
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_STRUCT;
typedef union {
  struct {
    UINT32 PG_Ramp                                 :  12;  // Bits 11:0
    UINT32 PG_Ramp_to_IPPwrGood                    :  12;  // Bits 23:12
    UINT32 DLY_AFTR_TGL_GLBDRV_EN                  :  4;  // Bits 27:24
    UINT32 Reserved                                :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_STRUCT;
typedef union {
  struct {
    UINT32                                         :  10;  // Bits 9:0
    UINT32 IPResetAssertion_to_PLLUnlock           :  7;  // Bits 16:10
    UINT32 PLLLOCK_to_GLBDRV_EN                    :  4;  // Bits 20:17
    UINT32 SAGV_PM12_ACK_to_PM9                    :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_STRUCT;
typedef union {
  struct {
    UINT32 ConductingDelay                         :  12;  // Bits 11:0
    UINT32                                         :  18;  // Bits 29:12
    UINT32 Skip_DFI_Timers                         :  1;  // Bits 30:30
    UINT32 Skip_PCHANNEL_Timers                    :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_STRUCT;
typedef union {
  struct {
    UINT32 DEBUG_MODE_EN                           :  1;  // Bits 0:0
    UINT32 DEFAULT_NS_STEP                         :  1;  // Bits 1:1
    UINT32 OVRD_NS_STEP                            :  1;  // Bits 2:2
    UINT32 DEBUG_FM_SEL                            :  3;  // Bits 5:3
    UINT32 OVRD_NS_SEL                             :  6;  // Bits 11:6
    UINT32 SKIP_PM_REQ_MC0                         :  1;  // Bits 12:12
    UINT32 SKIP_PM_REQ_MC1                         :  1;  // Bits 13:13
    UINT32 SKIP_PM_REQ_IBECC0                      :  1;  // Bits 14:14
    UINT32 SKIP_PM_REQ_IBECC1                      :  1;  // Bits 15:15
    UINT32 SKIP_PM_REQ_CCE0                        :  1;  // Bits 16:16
    UINT32 SKIP_PM_REQ_CCE1                        :  1;  // Bits 17:17
    UINT32 SKIP_PM_MSG_DDRPHY_PMA                  :  1;  // Bits 18:18
    UINT32 SKIP_PM_RSP_MC0                         :  1;  // Bits 19:19
    UINT32 SKIP_PM_RSP_MC1                         :  1;  // Bits 20:20
    UINT32 SKIP_PM_RSP_IBECC0                      :  1;  // Bits 21:21
    UINT32 SKIP_PM_RSP_IBECC1                      :  1;  // Bits 22:22
    UINT32 SKIP_PM_RSP_CCE0                        :  1;  // Bits 23:23
    UINT32 SKIP_PM_RSP_CCE1                        :  1;  // Bits 24:24
    UINT32 SKIP_PM_ACK_DDRPHY_PMA                  :  1;  // Bits 25:25
    UINT32 Skip_oppSR_PM_REQ_MC0                   :  1;  // Bits 26:26
    UINT32 Skip_oppSR_PM_REQ_MC1                   :  1;  // Bits 27:27
    UINT32 Skip_oppSR_PM_RSP_MC0                   :  1;  // Bits 28:28
    UINT32 Skip_oppSR_PM_RSP_MC1                   :  1;  // Bits 29:29
    UINT32 SKIP_DFI_INIT_COMPLETE                  :  1;  // Bits 30:30
    UINT32 SKIP_ONDD_ACK                           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_STRUCT;
typedef union {
  struct {
    UINT32 FM_SEL                                  :  3;  // Bits 2:0
    UINT32 FSM_STATE0_EN                           :  1;  // Bits 3:3
    UINT32 FSM_STATE0_SEL                          :  6;  // Bits 9:4
    UINT32 FSM_STATE0_DLY                          :  7;  // Bits 16:10
    UINT32 FSM_STATE1_EN                           :  1;  // Bits 17:17
    UINT32 FSM_STATE1_SEL                          :  6;  // Bits 23:18
    UINT32 FSM_STATE1_DLY                          :  7;  // Bits 30:24
    UINT32 FSM_RESUME_BP                           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_STRUCT;

typedef MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_STRUCT MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_1_STRUCT;

typedef MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_STRUCT MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_2_STRUCT;

typedef MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_STRUCT MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_3_STRUCT;

typedef MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_STRUCT MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_4_STRUCT;

typedef MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_STRUCT MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_5_STRUCT;

typedef MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_STRUCT MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_6_STRUCT;

typedef MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_STRUCT MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_7_STRUCT;
typedef union {
  struct {
    UINT32 SR_GROUP_ID_0                           :  4;  // Bits 3:0
    UINT32 SR_GROUP_ID_1                           :  4;  // Bits 7:4
    UINT32 SR_GROUP_ID_2                           :  4;  // Bits 11:8
    UINT32 SR_GROUP_ID_3                           :  4;  // Bits 15:12
    UINT32 SR_GROUP_ID_4                           :  4;  // Bits 19:16
    UINT32 SR_GROUP_ID_5                           :  4;  // Bits 23:20
    UINT32 SR_GROUP_ID_6                           :  4;  // Bits 27:24
    UINT32 SR_GROUP_ID_7                           :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_STRUCT;
typedef union {
  struct {
    UINT32 pllratio_override_en                    :  1;  // Bits 0:0
    UINT32 pllratio_override_val                   :  8;  // Bits 8:1
    UINT32 refclk_div_cfg_override_en              :  1;  // Bits 9:9
    UINT32 refclk_div_cfg_override_val             :  4;  // Bits 13:10
    UINT32 Reserved                                :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_STRUCT;
typedef union {
  struct {
    UINT32 MC0_ACK_STS                             :  1;  // Bits 0:0
    UINT32 MC1_ACK_STS                             :  1;  // Bits 1:1
    UINT32 IBECC0_ACK_STS                          :  1;  // Bits 2:2
    UINT32 IBECC1_ACK_STS                          :  1;  // Bits 3:3
    UINT32 CCE0_ACK_STS                            :  1;  // Bits 4:4
    UINT32 CCE1_ACK_STS                            :  1;  // Bits 5:5
    UINT32                                         :  26;  // Bits 31:6
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT MEMSS_PMA_CR_CCF_NCEVENTS_CR_LTCTRLSTS_STRUCT;
typedef union {
  struct {
    UINT32 MRC_FLOW_STS                            :  3;  // Bits 2:0
    UINT32 GLOBAL_FLOW_STS                         :  3;  // Bits 5:3
    UINT32 CLOCK_PWR_STS                           :  3;  // Bits 8:6
    UINT32 QCLK_CURRENT_IDX                        :  2;  // Bits 10:9
    UINT32 FUSE_PULL_DONE                          :  1;  // Bits 11:11
    UINT32 CMI_PLL_PLLRATIO                        :  8;  // Bits 19:12
    UINT32 CMI_PLL_REF_CLK_DIV                     :  4;  // Bits 23:20
    UINT32 GEAR_TYPE                               :  1;  // Bits 24:24
    UINT32 FSM_CURR_STATE                          :  6;  // Bits 30:25
    UINT32 Reserved                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_FLOW_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 SAGMemMC0PowerGood_en                   :  1;  // Bits 0:0
    UINT32 SAGTrainMC0PowerGood_en                 :  1;  // Bits 1:1
    UINT32 mc0_prim_reset_b_en                     :  1;  // Bits 2:2
    UINT32 mc0_side_reset_b_en                     :  1;  // Bits 3:3
    UINT32 mc0_train_reset_b_en                    :  1;  // Bits 4:4
    UINT32 mc0_credit_config_done_en               :  1;  // Bits 5:5
    UINT32 mc0_safe_mode_en                        :  1;  // Bits 6:6
    UINT32 mc0_q_restore_b_en                      :  1;  // Bits 7:7
    UINT32 SAGMemMC1PowerGood_en                   :  1;  // Bits 8:8
    UINT32 SAGTrainMC1PowerGood_en                 :  1;  // Bits 9:9
    UINT32 mc1_prim_reset_b_en                     :  1;  // Bits 10:10
    UINT32 mc1_side_reset_b_en                     :  1;  // Bits 11:11
    UINT32 mc1_train_reset_b_en                    :  1;  // Bits 12:12
    UINT32 mc1_credit_config_done_en               :  1;  // Bits 13:13
    UINT32 mc1_safe_mode_en                        :  1;  // Bits 14:14
    UINT32 mc1_q_restore_b_en                      :  1;  // Bits 15:15
    UINT32 SAGMemIBECC0PowerGood_en                :  1;  // Bits 16:16
    UINT32 ibecc0_prim_reset_b_en                  :  1;  // Bits 17:17
    UINT32 ibecc0_side_reset_b_en                  :  1;  // Bits 18:18
    UINT32 ibecc0_credit_config_done_en            :  1;  // Bits 19:19
    UINT32 ibecc0_safe_mode_en                     :  1;  // Bits 20:20
    UINT32 ibecc0_q_restore_b_en                   :  1;  // Bits 21:21
    UINT32 SAGMemIBECC1PowerGood_en                :  1;  // Bits 22:22
    UINT32 ibecc1_prim_reset_b_en                  :  1;  // Bits 23:23
    UINT32 ibecc1_side_reset_b_en                  :  1;  // Bits 24:24
    UINT32 ibecc1_credit_config_done_en            :  1;  // Bits 25:25
    UINT32 ibecc1_safe_mode_en                     :  1;  // Bits 26:26
    UINT32 ibecc1_q_restore_b_en                   :  1;  // Bits 27:27
    UINT32 cmi_pll_side_reset_b_en                 :  1;  // Bits 28:28
    UINT32 cmi_pll_vccdistpwrgood_en               :  1;  // Bits 29:29
    UINT32 cmi_pll_lock_en_en                      :  1;  // Bits 30:30
    UINT32 cmi_pll_clken_en                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_STRUCT;
typedef union {
  struct {
    UINT32 SAGMemCCE0PowerGood_en                  :  1;  // Bits 0:0
    UINT32 cce0_prim_reset_b_en                    :  1;  // Bits 1:1
    UINT32 cce0_side_reset_b_en                    :  1;  // Bits 2:2
    UINT32 cce0_credit_config_done_en              :  1;  // Bits 3:3
    UINT32 cce0_safe_mode_en                       :  1;  // Bits 4:4
    UINT32 cce0_q_restore_b_en                     :  1;  // Bits 5:5
    UINT32 SAGMemCCE1PowerGood_en                  :  1;  // Bits 6:6
    UINT32 cce1_prim_reset_b_en                    :  1;  // Bits 7:7
    UINT32 cce1_side_reset_b_en                    :  1;  // Bits 8:8
    UINT32 cce1_credit_config_done_en              :  1;  // Bits 9:9
    UINT32 cce1_safe_mode_en                       :  1;  // Bits 10:10
    UINT32 cce1_q_restore_b_en                     :  1;  // Bits 11:11
    UINT32 ddrphy_pma_side_reset_b_en              :  1;  // Bits 12:12
    UINT32 svrs_mem0_side_rst_b_en                 :  1;  // Bits 13:13
    UINT32 svrs_mem0_save_restore_trigger_en       :  1;  // Bits 14:14
    UINT32 svrs_mem1_side_rst_b_en                 :  1;  // Bits 15:15
    UINT32 svrs_mem1_save_restore_trigger_en       :  1;  // Bits 16:16
    UINT32 memsspma_go_prep_unprep_req_en          :  1;  // Bits 17:17
    UINT32 memsspma_go_prep_unprep_ack_en          :  1;  // Bits 18:18
    UINT32 memsspma_go_incgb_decgb_req_en          :  1;  // Bits 19:19
    UINT32 memsspma_go_incgb_decgb_ack_en          :  1;  // Bits 20:20
    UINT32 memsspma_pok_en                         :  1;  // Bits 21:21
    UINT32 SAGMemCMIPLLPowerGood_en                :  1;  // Bits 22:22
    UINT32 SAGMemDDRPHYPMAPowerGood_en             :  1;  // Bits 23:23
    UINT32 RESERVED                                :  1;  // Bits 24:24
    UINT32 ibecc0_en                               :  1;  // Bits 25:25
    UINT32 ibecc1_en                               :  1;  // Bits 26:26
    UINT32 memsspma_cmi_pll_keep_regulator_on_en   :  1;  // Bits 27:27
    UINT32                                         :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_STRUCT;
typedef union {
  struct {
    UINT32 tme_en                                  :  1;  // Bits 0:0
    UINT32 RESERVED1                               :  1;  // Bits 1:1
    UINT32 memsspma_resource_own_req_vccmemss_electrical_en:  1;  // Bits 2:2
    UINT32                                         :  1;  // Bits 3:3
    UINT32 ondd_block_req_en                       :  1;  // Bits 4:4
    UINT32 ondd_q_restore_b_en                     :  1;  // Bits 5:5
    UINT32 ondd_safe_mode_en                       :  1;  // Bits 6:6
    UINT32 ondd_side_reset_b_en                    :  1;  // Bits 7:7
    UINT32 ondd_prim_reset_b_en                    :  1;  // Bits 8:8
    UINT32 VCCMemssONDDPowerGood_en                :  1;  // Bits 9:9
    UINT32 SAGMemONDDPowerGood_en                  :  1;  // Bits 10:10
    UINT32 svrs_mem_switch2vccret_en               :  1;  // Bits 11:11
    UINT32 FDEE_en                                 :  1;  // Bits 12:12
    UINT32 ECCDIS_en                               :  1;  // Bits 13:13
    UINT32 CCE1_SLEEP_en                           :  1;  // Bits 14:14
    UINT32 CCE0_SLEEP_en                           :  1;  // Bits 15:15
    UINT32 IBECC1_SLEEP_en                         :  1;  // Bits 16:16
    UINT32 IBECC0_SLEEP_en                         :  1;  // Bits 17:17
    UINT32 MC1_SLEEP_en                            :  1;  // Bits 18:18
    UINT32 MC0_SLEEP_en                            :  1;  // Bits 19:19
    UINT32 CCE1_FORCE_CLKS_OFF_en                  :  1;  // Bits 20:20
    UINT32 CCE0_FORCE_CLKS_OFF_en                  :  1;  // Bits 21:21
    UINT32 IBECC1_FORCE_CLKS_OFF_en                :  1;  // Bits 22:22
    UINT32 IBECC0_FORCE_CLKS_OFF_en                :  1;  // Bits 23:23
    UINT32 MC1_FORCE_CLKS_OFF_en                   :  1;  // Bits 24:24
    UINT32 MC0_FORCE_CLKS_OFF_en                   :  1;  // Bits 25:25
    UINT32 PMA_FORCE_CLKS_OFF_en                   :  1;  // Bits 26:26
    UINT32 Reserved                                :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_STRUCT;
typedef union {
  struct {
    UINT32 SAGMemMC0PowerGood_val                  :  1;  // Bits 0:0
    UINT32 SAGTrainMC0PowerGood_val                :  1;  // Bits 1:1
    UINT32 mc0_prim_reset_b_val                    :  1;  // Bits 2:2
    UINT32 mc0_side_reset_b_val                    :  1;  // Bits 3:3
    UINT32 mc0_train_reset_b_val                   :  1;  // Bits 4:4
    UINT32 mc0_credit_config_done_val              :  1;  // Bits 5:5
    UINT32 mc0_safe_mode_val                       :  1;  // Bits 6:6
    UINT32 mc0_q_restore_b_val                     :  1;  // Bits 7:7
    UINT32 SAGMemMC1PowerGood_val                  :  1;  // Bits 8:8
    UINT32 SAGTrainMC1PowerGood_val                :  1;  // Bits 9:9
    UINT32 mc1_prim_reset_b_val                    :  1;  // Bits 10:10
    UINT32 mc1_side_reset_b_val                    :  1;  // Bits 11:11
    UINT32 mc1_train_reset_b_val                   :  1;  // Bits 12:12
    UINT32 mc1_credit_config_done_val              :  1;  // Bits 13:13
    UINT32 mc1_safe_mode_val                       :  1;  // Bits 14:14
    UINT32 mc1_q_restore_b_val                     :  1;  // Bits 15:15
    UINT32 SAGMemIBECC0PowerGood_val               :  1;  // Bits 16:16
    UINT32 ibecc0_prim_reset_b_val                 :  1;  // Bits 17:17
    UINT32 ibecc0_side_reset_b_val                 :  1;  // Bits 18:18
    UINT32 ibecc0_credit_config_done_val           :  1;  // Bits 19:19
    UINT32 ibecc0_safe_mode_val                    :  1;  // Bits 20:20
    UINT32 ibecc0_q_restore_b_val                  :  1;  // Bits 21:21
    UINT32 SAGMemIBECC1PowerGood_val               :  1;  // Bits 22:22
    UINT32 ibecc1_prim_reset_b_val                 :  1;  // Bits 23:23
    UINT32 ibecc1_side_reset_b_val                 :  1;  // Bits 24:24
    UINT32 ibecc1_credit_config_done_val           :  1;  // Bits 25:25
    UINT32 ibecc1_safe_mode_val                    :  1;  // Bits 26:26
    UINT32 ibecc1_q_restore_b_val                  :  1;  // Bits 27:27
    UINT32 cmi_pll_side_reset_b_val                :  1;  // Bits 28:28
    UINT32 cmi_pll_vccdistpwrgood_val              :  1;  // Bits 29:29
    UINT32 cmi_pll_lock_en_val                     :  1;  // Bits 30:30
    UINT32 cmi_pll_clken_val                       :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_STRUCT;
typedef union {
  struct {
    UINT32 SAGMemCCE0PowerGood_val                 :  1;  // Bits 0:0
    UINT32 cce0_prim_reset_b_val                   :  1;  // Bits 1:1
    UINT32 cce0_side_reset_b_val                   :  1;  // Bits 2:2
    UINT32 cce0_credit_config_done_val             :  1;  // Bits 3:3
    UINT32 cce0_safe_mode_val                      :  1;  // Bits 4:4
    UINT32 cce0_q_restore_b_val                    :  1;  // Bits 5:5
    UINT32 SAGMemCCE1PowerGood_val                 :  1;  // Bits 6:6
    UINT32 cce1_prim_reset_b_val                   :  1;  // Bits 7:7
    UINT32 cce1_side_reset_b_val                   :  1;  // Bits 8:8
    UINT32 cce1_credit_config_done_val             :  1;  // Bits 9:9
    UINT32 cce1_safe_mode_val                      :  1;  // Bits 10:10
    UINT32 cce1_q_restore_b_val                    :  1;  // Bits 11:11
    UINT32 ddrphy_pma_side_reset_b_val             :  1;  // Bits 12:12
    UINT32 svrs_mem0_side_rst_b_val                :  1;  // Bits 13:13
    UINT32 svrs_mem0_save_restore_trigger_val      :  1;  // Bits 14:14
    UINT32 svrs_mem1_side_rst_b_val                :  1;  // Bits 15:15
    UINT32 svrs_mem1_save_restore_trigger_val      :  1;  // Bits 16:16
    UINT32 memsspma_go_prep_unprep_req_val         :  1;  // Bits 17:17
    UINT32 memsspma_go_prep_unprep_ack_val         :  1;  // Bits 18:18
    UINT32 memsspma_go_incgb_decgb_req_val         :  1;  // Bits 19:19
    UINT32 memsspma_go_incgb_decgb_ack_val         :  1;  // Bits 20:20
    UINT32 memsspma_pok_val                        :  1;  // Bits 21:21
    UINT32 SAGMemCMIPLLPowerGood_val               :  1;  // Bits 22:22
    UINT32 SAGMemDDRPHYPMAPowerGood_val            :  1;  // Bits 23:23
    UINT32 RESERVED                                :  1;  // Bits 24:24
    UINT32 ibecc0_en_val                           :  1;  // Bits 25:25
    UINT32 ibecc1_en_val                           :  1;  // Bits 26:26
    UINT32 memsspma_cmi_pll_keep_regulator_on_val  :  1;  // Bits 27:27
    UINT32                                         :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_STRUCT;
typedef union {
  struct {
    UINT32 tme_en_val                              :  1;  // Bits 0:0
    UINT32 RESERVED1                               :  1;  // Bits 1:1
    UINT32 memsspma_resource_own_req_vccmemss_electrical_val:  1;  // Bits 2:2
    UINT32                                         :  1;  // Bits 3:3
    UINT32 ondd_block_req_val                      :  1;  // Bits 4:4
    UINT32 ondd_q_restore_b_val                    :  1;  // Bits 5:5
    UINT32 ondd_safe_mode_val                      :  1;  // Bits 6:6
    UINT32 ondd_side_reset_b_val                   :  1;  // Bits 7:7
    UINT32 ondd_prim_reset_b_val                   :  1;  // Bits 8:8
    UINT32 VCCMemssONDDPowerGood_val               :  1;  // Bits 9:9
    UINT32 SAGMemONDDPowerGood_val                 :  1;  // Bits 10:10
    UINT32 svrs_mem_switch2vccret_val              :  1;  // Bits 11:11
    UINT32 FDEE_val                                :  1;  // Bits 12:12
    UINT32 ECCDIS_val                              :  1;  // Bits 13:13
    UINT32 CCE1_SLEEP_val                          :  1;  // Bits 14:14
    UINT32 CCE0_SLEEP_val                          :  1;  // Bits 15:15
    UINT32 IBECC1_SLEEP_val                        :  1;  // Bits 16:16
    UINT32 IBECC0_SLEEP_val                        :  1;  // Bits 17:17
    UINT32 MC1_SLEEP_val                           :  1;  // Bits 18:18
    UINT32 MC0_SLEEP_val                           :  1;  // Bits 19:19
    UINT32 CCE1_FORCE_CLKS_OFF_val                 :  1;  // Bits 20:20
    UINT32 CCE0_FORCE_CLKS_OFF_val                 :  1;  // Bits 21:21
    UINT32 IBECC1_FORCE_CLKS_OFF_val               :  1;  // Bits 22:22
    UINT32 IBECC0_FORCE_CLKS_OFF_val               :  1;  // Bits 23:23
    UINT32 MC1_FORCE_CLKS_OFF_val                  :  1;  // Bits 24:24
    UINT32 MC0_FORCE_CLKS_OFF_val                  :  1;  // Bits 25:25
    UINT32 PMA_FORCE_CLKS_OFF_val                  :  1;  // Bits 26:26
    UINT32 Reserved                                :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_STRUCT;
typedef union {
  struct {
    UINT32 SAGMEMMC0_DFT_PowerUpParOverrideEn      :  1;  // Bits 0:0
    UINT32 SAGMEMMC0_DFT_PowerUpParOverrideVal     :  1;  // Bits 1:1
    UINT32 SAGMEMMC0_DFT_ForceOnParOverrideEn      :  1;  // Bits 2:2
    UINT32 SAGMEMMC0_DFT_ForceOnParOverrideVal     :  1;  // Bits 3:3
    UINT32 SAGMEMMC0_DFT_PG_Enable                 :  1;  // Bits 4:4
    UINT32 SAGMEMMC0_DFT_Master_mode               :  1;  // Bits 5:5
    UINT32 SAGMEMMC0_DFT_SingleStep                :  1;  // Bits 6:6
    UINT32 SAGMEMMC0_DFT_PwrGoodOverrideEn         :  1;  // Bits 7:7
    UINT32 SAGMEMMC0_DFT_PwrGoodOverrideVal        :  1;  // Bits 8:8
    UINT32 SAGMEMMC0_DFT_EarlyForceOnOverrideEn    :  1;  // Bits 9:9
    UINT32 SAGMEMMC0_DFT_EarlyForceOnOverrideVal   :  1;  // Bits 10:10
    UINT32 SAGMEMMC0_DFT_Skip_voltage_only         :  1;  // Bits 11:11
    UINT32 SAGMEMMC0_DFT_ovr_gated_fet_ack         :  1;  // Bits 12:12
    UINT32 Reserved_0                              :  1;  // Bits 13:13
    UINT32 SAGMEMMC1_DFT_PowerUpParOverrideEn      :  1;  // Bits 14:14
    UINT32 SAGMEMMC1_DFT_PowerUpParOverrideVal     :  1;  // Bits 15:15
    UINT32 SAGMEMMC1_DFT_ForceOnParOverrideEn      :  1;  // Bits 16:16
    UINT32 SAGMEMMC1_DFT_ForceOnParOverrideVal     :  1;  // Bits 17:17
    UINT32 SAGMEMMC1_DFT_PG_Enable                 :  1;  // Bits 18:18
    UINT32 SAGMEMMC1_DFT_Master_mode               :  1;  // Bits 19:19
    UINT32 SAGMEMMC1_DFT_SingleStep                :  1;  // Bits 20:20
    UINT32 SAGMEMMC1_DFT_PwrGoodOverrideEn         :  1;  // Bits 21:21
    UINT32 SAGMEMMC1_DFT_PwrGoodOverrideVal        :  1;  // Bits 22:22
    UINT32 SAGMEMMC1_DFT_EarlyForceOnOverrideEn    :  1;  // Bits 23:23
    UINT32 SAGMEMMC1_DFT_EarlyForceOnOverrideVal   :  1;  // Bits 24:24
    UINT32 SAGMEMMC1_DFT_Skip_voltage_only         :  1;  // Bits 25:25
    UINT32 SAGMEMMC1_DFT_ovr_gated_fet_ack         :  1;  // Bits 26:26
    UINT32 Reserved_1                              :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_STRUCT;
typedef union {
  struct {
    UINT32 SAGTrainMC0_DFT_PowerUpParOverrideEn    :  1;  // Bits 0:0
    UINT32 SAGTrainMC0_DFT_PowerUpParOverrideVal   :  1;  // Bits 1:1
    UINT32 SAGTrainMC0_DFT_ForceOnParOverrideEn    :  1;  // Bits 2:2
    UINT32 SAGTrainMC0_DFT_ForceOnParOverrideVal   :  1;  // Bits 3:3
    UINT32 SAGTRAINMC0_DFT_PG_Enable               :  1;  // Bits 4:4
    UINT32 SAGTRAINMC0_DFT_Master_mode             :  1;  // Bits 5:5
    UINT32 SAGTRAINMC0_DFT_SingleStep              :  1;  // Bits 6:6
    UINT32 SAGTrainMC0_DFT_PwrGoodOverrideEn       :  1;  // Bits 7:7
    UINT32 SAGTrainMC0_DFT_PwrGoodOverrideVal      :  1;  // Bits 8:8
    UINT32 SAGTrainMC0_DFT_EarlyForceOnOverrideEn  :  1;  // Bits 9:9
    UINT32 SAGTrainMC0_DFT_EarlyForceOnOverrideVal :  1;  // Bits 10:10
    UINT32 SAGTrainMC0_DFT_Skip_voltage_only       :  1;  // Bits 11:11
    UINT32 SAGTrainMC0_DFT_ovr_gated_fet_ack       :  1;  // Bits 12:12
    UINT32 Reserved_0                              :  2;  // Bits 14:13
    UINT32 SAGTrainMC1_DFT_PowerUpParOverrideEn    :  1;  // Bits 15:15
    UINT32 SAGTrainMC1_DFT_PowerUpParOverrideVal   :  1;  // Bits 16:16
    UINT32 SAGTrainMC1_DFT_ForceOnParOverrideEn    :  1;  // Bits 17:17
    UINT32 SAGTrainMC1_DFT_ForceOnParOverrideVal   :  1;  // Bits 18:18
    UINT32 SAGTRAINMC1_DFT_PG_Enable               :  1;  // Bits 19:19
    UINT32 SAGTRAINMC1_DFT_Master_mode             :  1;  // Bits 20:20
    UINT32 SAGTRAINMC1_DFT_SingleStep              :  1;  // Bits 21:21
    UINT32 SAGTrainMC1_DFT_PwrGoodOverrideEn       :  1;  // Bits 22:22
    UINT32 SAGTrainMC1_DFT_PwrGoodOverrideVal      :  1;  // Bits 23:23
    UINT32 SAGTrainMC1_DFT_EarlyForceOnOverrideEn  :  1;  // Bits 24:24
    UINT32 SAGTrainMC1_DFT_EarlyForceOnOverrideVal :  1;  // Bits 25:25
    UINT32 SAGTrainMC1_DFT_Skip_voltage_only       :  1;  // Bits 26:26
    UINT32 SAGTrainMC1_DFT_ovr_gated_fet_ack       :  1;  // Bits 27:27
    UINT32 Reserved_1                              :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_STRUCT;
typedef union {
  struct {
    UINT32 SAGMEMIBECC0_DFT_PowerUpParOverrideEn   :  1;  // Bits 0:0
    UINT32 SAGMEMIBECC0_DFT_PowerUpParOverrideVal  :  1;  // Bits 1:1
    UINT32 SAGMEMIBECC0_DFT_ForceOnParOverrideEn   :  1;  // Bits 2:2
    UINT32 SAGMEMIBECC0_DFT_ForceOnParOverrideVal  :  1;  // Bits 3:3
    UINT32 SAGMEMIBECC0_DFT_PG_Enable              :  1;  // Bits 4:4
    UINT32 SAGMEMIBECC0_DFT_Master_mode            :  1;  // Bits 5:5
    UINT32 SAGMEMIBECC0_DFT_SingleStep             :  1;  // Bits 6:6
    UINT32 Reserved_0                              :  1;  // Bits 7:7
    UINT32 SAGMEMIBECC0_DFT_PwrGoodOverrideEn      :  1;  // Bits 8:8
    UINT32 SAGMEMIBECC0_DFT_PwrGoodOverrideVal     :  1;  // Bits 9:9
    UINT32 SAGMEMIBECC0_DFT_EarlyForceOnOverrideEn :  1;  // Bits 10:10
    UINT32 SAGMEMIBECC0_DFT_EarlyForceOnOverrideVal:  1;  // Bits 11:11
    UINT32 SAGMEMIBECC0_DFT_Skip_voltage_only      :  1;  // Bits 12:12
    UINT32 SAGMEMIBECC0_DFT_ovr_gated_fet_ack      :  1;  // Bits 13:13
    UINT32 Reserved_1                              :  2;  // Bits 15:14
    UINT32 SAGMEMIBECC1_DFT_PowerUpParOverrideEn   :  1;  // Bits 16:16
    UINT32 SAGMEMIBECC1_DFT_PowerUpParOverrideVal  :  1;  // Bits 17:17
    UINT32 SAGMEMIBECC1_DFT_ForceOnParOverrideEn   :  1;  // Bits 18:18
    UINT32 SAGMEMIBECC1_DFT_ForceOnParOverrideVal  :  1;  // Bits 19:19
    UINT32 SAGMEMIBECC1_DFT_PG_Enable              :  1;  // Bits 20:20
    UINT32 SAGMEMIBECC1_DFT_Master_mode            :  1;  // Bits 21:21
    UINT32 SAGMEMIBECC1_DFT_SingleStep             :  1;  // Bits 22:22
    UINT32 Reserved_2                              :  1;  // Bits 23:23
    UINT32 SAGMEMIBECC1_DFT_PwrGoodOverrideEn      :  1;  // Bits 24:24
    UINT32 SAGMEMIBECC1_DFT_PwrGoodOverrideVal     :  1;  // Bits 25:25
    UINT32 SAGMEMIBECC1_DFT_EarlyForceOnOverrideEn :  1;  // Bits 26:26
    UINT32 SAGMEMIBECC1_DFT_EarlyForceOnOverrideVal:  1;  // Bits 27:27
    UINT32 SAGMEMIBECC1_DFT_Skip_voltage_only      :  1;  // Bits 28:28
    UINT32 SAGMEMIBECC1_DFT_ovr_gated_fet_ack      :  1;  // Bits 29:29
    UINT32 Reserved_3                              :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_STRUCT;
typedef union {
  struct {
    UINT32 SAGMEMCCE0_DFT_PowerUpParOverrideEn     :  1;  // Bits 0:0
    UINT32 SAGMEMCCE0_DFT_PowerUpParOverrideVal    :  1;  // Bits 1:1
    UINT32 SAGMEMCCE0_DFT_ForceOnParOverrideEn     :  1;  // Bits 2:2
    UINT32 SAGMEMCCE0_DFT_ForceOnParOverrideVal    :  1;  // Bits 3:3
    UINT32 SAGMEMCCE0_DFT_PG_Enable                :  1;  // Bits 4:4
    UINT32 SAGMEMCCE0_DFT_Master_mode              :  1;  // Bits 5:5
    UINT32 SAGMEMCCE0_DFT_SingleStep               :  1;  // Bits 6:6
    UINT32 Reserved_0                              :  1;  // Bits 7:7
    UINT32 SAGMEMCCE0_DFT_PwrGoodOverrideEn        :  1;  // Bits 8:8
    UINT32 SAGMEMCCE0_DFT_PwrGoodOverrideVal       :  1;  // Bits 9:9
    UINT32 SAGMEMCCE0_DFT_EarlyForceOnOverrideEn   :  1;  // Bits 10:10
    UINT32 SAGMEMCCE0_DFT_EarlyForceOnOverrideVal  :  1;  // Bits 11:11
    UINT32 SAGMEMCCE0_DFT_Skip_voltage_only        :  1;  // Bits 12:12
    UINT32 SAGMEMCCE0_DFT_ovr_gated_fet_ack        :  1;  // Bits 13:13
    UINT32 Reserved_1                              :  2;  // Bits 15:14
    UINT32 SAGMEMCCE1_DFT_PowerUpParOverrideEn     :  1;  // Bits 16:16
    UINT32 SAGMEMCCE1_DFT_PowerUpParOverrideVal    :  1;  // Bits 17:17
    UINT32 SAGMEMCCE1_DFT_ForceOnParOverrideEn     :  1;  // Bits 18:18
    UINT32 SAGMEMCCE1_DFT_ForceOnParOverrideVal    :  1;  // Bits 19:19
    UINT32 SAGMEMCCE1_DFT_PG_Enable                :  1;  // Bits 20:20
    UINT32 SAGMEMCCE1_DFT_Master_mode              :  1;  // Bits 21:21
    UINT32 SAGMEMCCE1_DFT_SingleStep               :  1;  // Bits 22:22
    UINT32 Reserved_2                              :  1;  // Bits 23:23
    UINT32 SAGMEMCCE1_DFT_PwrGoodOverrideEn        :  1;  // Bits 24:24
    UINT32 SAGMEMCCE1_DFT_PwrGoodOverrideVal       :  1;  // Bits 25:25
    UINT32 SAGMEMCCE1_DFT_EarlyForceOnOverrideEn   :  1;  // Bits 26:26
    UINT32 SAGMEMCCE1_DFT_EarlyForceOnOverrideVal  :  1;  // Bits 27:27
    UINT32 SAGMEMCCE1_DFT_Skip_voltage_only        :  1;  // Bits 28:28
    UINT32 SAGMEMCCE1_DFT_ovr_gated_fet_ack        :  1;  // Bits 29:29
    UINT32 Reserved_3                              :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_STRUCT;
typedef union {
  struct {
    UINT32 SAGMEMONDD_DFT_PowerUpParOverrideEn     :  1;  // Bits 0:0
    UINT32 SAGMEMONDD_DFT_PowerUpParOverrideVal    :  1;  // Bits 1:1
    UINT32 SAGMEMONDD_DFT_ForceOnParOverrideEn     :  1;  // Bits 2:2
    UINT32 SAGMEMONDD_DFT_ForceOnParOverrideVal    :  1;  // Bits 3:3
    UINT32 SAGMEMONDD_DFT_PG_Enable                :  1;  // Bits 4:4
    UINT32 SAGMEMONDD_DFT_Master_mode              :  1;  // Bits 5:5
    UINT32 SAGMEMONDD_DFT_SingleStep               :  1;  // Bits 6:6
    UINT32 SAGMEMONDD_DFT_PwrGoodOverrideEn        :  1;  // Bits 7:7
    UINT32 SAGMEMONDD_DFT_PwrGoodOverrideVal       :  1;  // Bits 8:8
    UINT32 SAGMEMONDD_DFT_EarlyForceOnOverrideEn   :  1;  // Bits 9:9
    UINT32 SAGMEMONDD_DFT_EarlyForceOnOverrideVal  :  1;  // Bits 10:10
    UINT32 SAGMEMONDD_DFT_Skip_voltage_only        :  1;  // Bits 11:11
    UINT32 SAGMEMONDD_DFT_ovr_gated_fet_ack        :  1;  // Bits 12:12
    UINT32 Reserved_1                              :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_STRUCT;
typedef union {
  struct {
    UINT32 SAGMEMMC0_DFT_CurrentState              :  4;  // Bits 3:0
    UINT32 SAGMEMMC1_DFT_CurrentState              :  4;  // Bits 7:4
    UINT32 SAGTrainMC0_DFT_CurrentState            :  4;  // Bits 11:8
    UINT32 SAGTrainMC1_DFT_CurrentState            :  4;  // Bits 15:12
    UINT32 SAGMEMIBECC0_DFT_CurrentState           :  4;  // Bits 19:16
    UINT32 SAGMEMIBECC1_DFT_CurrentState           :  4;  // Bits 23:20
    UINT32 SAGMEMCCE0_DFT_CurrentState             :  4;  // Bits 27:24
    UINT32 SAGMEMCCE1_DFT_CurrentState             :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_STRUCT;
typedef union {
  struct {
    UINT32 SAGMEMMC0_DFT_PG_in_Trans               :  1;  // Bits 0:0
    UINT32 SAGMEMMC1_DFT_PG_in_Trans               :  1;  // Bits 1:1
    UINT32 SAGTrainMC0_DFT_PG_in_Trans             :  1;  // Bits 2:2
    UINT32 SAGTrainMC1_DFT_PG_in_Trans             :  1;  // Bits 3:3
    UINT32 SAGMEMIBECC0_DFT_PG_in_Trans            :  1;  // Bits 4:4
    UINT32 SAGMEMIBECC1_DFT_PG_in_Trans            :  1;  // Bits 5:5
    UINT32 SAGMEMCCE0_DFT_PG_in_Trans              :  1;  // Bits 6:6
    UINT32 SAGMEMCCE1_DFT_PG_in_Trans              :  1;  // Bits 7:7
    UINT32 SAGMEMONDD_DFT_PG_in_Trans              :  1;  // Bits 8:8
    UINT32 SAGMEMONDD_DFT_CurrentState             :  4;  // Bits 12:9
    UINT32                                         :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_STRUCT;
typedef union {
  struct {
    UINT32 SAGMEMMC0_scan_iso_en                   :  1;  // Bits 0:0
    UINT32 SAGMEMMC0_fscan_byprst_b                :  1;  // Bits 1:1
    UINT32 SAGMEMMC0_fscan_rstbypen                :  1;  // Bits 2:2
    UINT32 SAGMEMMC1_scan_iso_en                   :  1;  // Bits 3:3
    UINT32 SAGMEMMC1_fscan_byprst_b                :  1;  // Bits 4:4
    UINT32 SAGMEMMC1_fscan_rstbypen                :  1;  // Bits 5:5
    UINT32 SAGTrainMC0_scan_iso_en                 :  1;  // Bits 6:6
    UINT32 SAGTrainMC0_fscan_byprst_b              :  1;  // Bits 7:7
    UINT32 SAGTrainMC0_fscan_rstbypen              :  1;  // Bits 8:8
    UINT32 SAGTrainMC1_scan_iso_en                 :  1;  // Bits 9:9
    UINT32 SAGTrainMC1_fscan_byprst_b              :  1;  // Bits 10:10
    UINT32 SAGTrainMC1_fscan_rstbypen              :  1;  // Bits 11:11
    UINT32 SAGMEMIBECC0_scan_iso_en                :  1;  // Bits 12:12
    UINT32 SAGMEMIBECC0_fscan_byprst_b             :  1;  // Bits 13:13
    UINT32 SAGMEMIBECC0_fscan_rstbypen             :  1;  // Bits 14:14
    UINT32 SAGMEMIBECC1_scan_iso_en                :  1;  // Bits 15:15
    UINT32 SAGMEMIBECC1_fscan_byprst_b             :  1;  // Bits 16:16
    UINT32 SAGMEMIBECC1_fscan_rstbypen             :  1;  // Bits 17:17
    UINT32 SAGMEMCCE0_scan_iso_en                  :  1;  // Bits 18:18
    UINT32 SAGMEMCCE0_fscan_byprst_b               :  1;  // Bits 19:19
    UINT32 SAGMEMCCE0_fscan_rstbypen               :  1;  // Bits 20:20
    UINT32 SAGMEMCCE1_scan_iso_en                  :  1;  // Bits 21:21
    UINT32 SAGMEMCCE1_fscan_byprst_b               :  1;  // Bits 22:22
    UINT32 SAGMEMCCE1_fscan_rstbypen               :  1;  // Bits 23:23
    UINT32 SAGMEMHBO0_scan_iso_en                  :  1;  // Bits 24:24
    UINT32 SAGMEMHBO0_fscan_byprst_b               :  1;  // Bits 25:25
    UINT32 SAGMEMHBO0_fscan_rstbypen               :  1;  // Bits 26:26
    UINT32 SAGMEMHBO1_scan_iso_en                  :  1;  // Bits 27:27
    UINT32 SAGMEMHBO1_fscan_byprst_b               :  1;  // Bits 28:28
    UINT32 SAGMEMHBO1_fscan_rstbypen               :  1;  // Bits 29:29
    UINT32 Reserved                                :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_STRUCT;
typedef union {
  struct {
    UINT32 FORCE_IND                               :  1;  // Bits 0:0
    UINT32 VCCMEMSS_ELECTRICAL                     :  1;  // Bits 1:1
    UINT32 MEMUP_SERVICE                           :  1;  // Bits 2:2
    UINT32 MEMCLK_SERVICE                          :  1;  // Bits 3:3
    UINT32 WARM_RESET_SERVICE                      :  1;  // Bits 4:4
    UINT32 Reserved                                :  27;  // Bits 31:5
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 QCLK_CURRENT_IDX                        :  2;  // Bits 1:0
    UINT32 CMI_PLL_PLLRATIO                        :  8;  // Bits 9:2
    UINT32 GEAR_TYPE                               :  1;  // Bits 10:10
    UINT32 DVFS_VDDQ                               :  5;  // Bits 15:11
    UINT32 FORCE_IND                               :  1;  // Bits 16:16
    UINT32 VCCMEMSS_ELECTRICAL                     :  1;  // Bits 17:17
    UINT32 MEMUP_SERVICE                           :  1;  // Bits 18:18
    UINT32 MEMCLK_SERVICE                          :  1;  // Bits 19:19
    UINT32 WARM_RESET_SERVICE                      :  1;  // Bits 20:20
    UINT32 Reserved                                :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_PUNIT_REQ_DATA_STRUCT;
typedef union {
  struct {
    UINT32 ddrphy_PM12_Message_Start_to_nComplete  :  16;  // Bits 15:0
    UINT32 ddrphy_PM12_Message_nStart_to_Complete  :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_STRUCT;
typedef union {
  struct {
    UINT32 ddrphy_PM_Message_Preq_to_Paccept       :  16;  // Bits 15:0
    UINT32 ddrphy_PM_Messages_nPreq_to_nPaccept    :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_STRUCT;
typedef union {
  struct {
    UINT32 PCHANNEL_FSM_STATUS                     :  1;  // Bits 0:0
    UINT32 PCHANNEL_REQ_TO_ACC_TIMEOUT             :  1;  // Bits 1:1
    UINT32 PCHANNEL_nREQ_TO_nACC_TIMEOUT           :  1;  // Bits 2:2
    UINT32                                         :  1;  // Bits 3:3
    UINT32 PCH_DENY_STS                            :  2;  // Bits 5:4
    UINT32                                         :  2;  // Bits 7:6
    UINT32 PCHANNEL_PACTIVE_VAL                    :  8;  // Bits 15:8
    UINT32 DFI_FSM_STATUS                          :  1;  // Bits 16:16
    UINT32 DFI_START_TO_nCOMPLETE_TIMEOUT          :  1;  // Bits 17:17
    UINT32 DFI_nSTART_TO_COMPLETE_TIMEOUT          :  1;  // Bits 18:18
    UINT32 PCHANNEL_BAD_PSTATE_REQ_STS             :  1;  // Bits 19:19
    UINT32                                         :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 Count : 32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP1_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP2_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP3_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC1_GVP0_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC1_GVP1_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC1_GVP2_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC1_GVP3_STRUCT;
typedef union {
  struct {
    UINT32 SAGV_OPP_SR_DISABLE                     :  1;  // Bits 0:0
    UINT32 SAGV_VRCG_DISABLE                       :  1;  // Bits 1:1
    UINT32 SAGV_FORCE_FAKE_GV_EN                   :  1;  // Bits 2:2
    UINT32 AON_SBEP_CLKREQ                         :  1;  // Bits 3:3
    UINT32 PKGC3_PM3_NOT_PM6                       :  1;  // Bits 4:4
    UINT32 PKGC8_PM3_NOT_PM6                       :  1;  // Bits 5:5
    UINT32 DDRPHY_PM_MSG_RUN_BUSY                  :  1;  // Bits 6:6
    UINT32 DDRPHY_PM_MSG_SEL                       :  4;  // Bits 10:7
    UINT32 RETENTION_WAIT_CYCLES                   :  4;  // Bits 14:11
    UINT32 RETENTION_RUN_BUSY                      :  1;  // Bits 15:15
    UINT32 RETENTION_IP_SEL                        :  6;  // Bits 21:16
    UINT32 RETENTION_DIRECTION                     :  1;  // Bits 22:22
    UINT32 RETENTION_SLEEP_2_POWERGOOD_DLY         :  4;  // Bits 26:23
    UINT32 TELEMETRY_CNTRS_EN                      :  1;  // Bits 27:27
    UINT32 SKIP_MEMPLL_LOCK_UNLOCK                 :  1;  // Bits 28:28
    UINT32 SKIP_MC_ISO                             :  1;  // Bits 29:29
    UINT32 SKIP_MC_GV_RESTORE                      :  1;  // Bits 30:30
    UINT32 CBIT_IGNORE_ONDD_UNBLOCK_ACK            :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_MISC_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT PKG_MC_C0_LO_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT PKG_MC_C0_HI_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT PKG_MC_C0_RATIO_LO_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT PKG_MC_C0_RATIO_HI_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT SAGV_PERFCOUNTER_0_LO_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT SAGV_PERFCOUNTER_0_HI_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT SAGV_PERFCOUNTER_1_LO_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT SAGV_PERFCOUNTER_1_HI_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT SAGV_PERFCOUNTER_2_LO_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT SAGV_PERFCOUNTER_2_HI_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT SAGV_PERFCOUNTER_3_LO_STRUCT;

typedef MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_STRUCT SAGV_PERFCOUNTER_3_HI_STRUCT;
typedef union {
  struct {
    UINT32 gv_in_progress_b_en                     :  1;  // Bits 0:0
    UINT32 pma_phy_preq_en                         :  1;  // Bits 1:1
    UINT32 pma_phy_pstate_en                       :  1;  // Bits 2:2
    UINT32                                         :  2;  // Bits 4:3
    UINT32 dfi_init_start_en                       :  1;  // Bits 5:5
    UINT32 dfi_frequency_en                        :  1;  // Bits 6:6
    UINT32                                         :  4;  // Bits 10:7
    UINT32 NOC_BYPASS_EN                           :  1;  // Bits 11:11
    UINT32 mc0_immediate_sleep_level_rsp           :  1;  // Bits 12:12
    UINT32 mc1_immediate_sleep_level_rsp           :  1;  // Bits 13:13
    UINT32 cce0_immediate_sleep_level_rsp          :  1;  // Bits 14:14
    UINT32 cce1_immediate_sleep_level_rsp          :  1;  // Bits 15:15
    UINT32 ibecc0_immediate_sleep_level_rsp        :  1;  // Bits 16:16
    UINT32 ibecc1_immediate_sleep_level_rsp        :  1;  // Bits 17:17
    UINT32 mctrain_clkgate_ovrd_en                 :  1;  // Bits 18:18
    UINT32 cce0_shutoff_ovrd_en                    :  1;  // Bits 19:19
    UINT32 cce1_shutoff_ovrd_en                    :  1;  // Bits 20:20
    UINT32 global_clken_ovrd_ovrd_en               :  1;  // Bits 21:21
    UINT32 Reserved                                :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_STRUCT;
typedef union {
  struct {
    UINT32 gv_in_progress_b_val                    :  1;  // Bits 0:0
    UINT32 pma_phy_preq_val                        :  1;  // Bits 1:1
    UINT32 pma_phy_pstate_val                      :  3;  // Bits 4:2
    UINT32 dfi_init_start_val                      :  1;  // Bits 5:5
    UINT32 dfi_frequency_val                       :  5;  // Bits 10:6
    UINT32 NOC_BYPASS_VAL                          :  1;  // Bits 11:11
    UINT32 mc0_immediate_sleep_level_rsp           :  1;  // Bits 12:12
    UINT32 mc1_immediate_sleep_level_rsp           :  1;  // Bits 13:13
    UINT32 cce0_immediate_sleep_level_rsp          :  1;  // Bits 14:14
    UINT32 cce1_immediate_sleep_level_rsp          :  1;  // Bits 15:15
    UINT32 ibecc0_immediate_sleep_level_rsp        :  1;  // Bits 16:16
    UINT32 ibecc1_immediate_sleep_level_rsp        :  1;  // Bits 17:17
    UINT32 mctrain_clkgate_ovrd_val                :  1;  // Bits 18:18
    UINT32 cce0_shutoff_ovrd_val                   :  1;  // Bits 19:19
    UINT32 cce1_shutoff_ovrd_val                   :  1;  // Bits 20:20
    UINT32 global_clken_ovrd_ovrd_val              :  1;  // Bits 21:21
    UINT32 Reserved                                :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_STRUCT;
typedef union {
  struct {
    UINT32 phy_pma_paccept_en                      :  1;  // Bits 0:0
    UINT32 phy_pma_pdeny_en                        :  1;  // Bits 1:1
    UINT32 phy_pma_dfi_init_complete_en            :  1;  // Bits 2:2
    UINT32 dfi_init_complete_initialization_en     :  1;  // Bits 3:3
    UINT32 pll_lock_ack_en                         :  1;  // Bits 4:4
    UINT32 pll_early_lock_ack_en                   :  1;  // Bits 5:5
    UINT32 mc0_side_POK_en                         :  1;  // Bits 6:6
    UINT32 mc1_side_POK_en                         :  1;  // Bits 7:7
    UINT32 cce0_side_POK_en                        :  1;  // Bits 8:8
    UINT32 cce1_side_POK_en                        :  1;  // Bits 9:9
    UINT32 ibecc0_side_POK_en                      :  1;  // Bits 10:10
    UINT32 ibecc1_side_POK_en                      :  1;  // Bits 11:11
    UINT32 ondd_side_POK_en                        :  1;  // Bits 12:12
    UINT32 pll_IP_READY_en                         :  1;  // Bits 13:13
    UINT32 svrs_mem0_IP_READY_en                   :  1;  // Bits 14:14
    UINT32 svrs_mem1_IP_READY_en                   :  1;  // Bits 15:15
    UINT32 ondd_IP_READY_en                        :  1;  // Bits 16:16
    UINT32 ddrio_fuse_pull_done_en                 :  1;  // Bits 17:17
    UINT32 ondd_block_ack_en                       :  1;  // Bits 18:18
    UINT32 Reserved                                :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_STRUCT;
typedef union {
  struct {
    UINT32 phy_pma_paccept_val                     :  1;  // Bits 0:0
    UINT32 phy_pma_pdeny_val                       :  1;  // Bits 1:1
    UINT32 phy_pma_dfi_init_complete_val           :  1;  // Bits 2:2
    UINT32                                         :  1;  // Bits 3:3
    UINT32 pll_lock_ack_val                        :  1;  // Bits 4:4
    UINT32 pll_early_lock_ack_val                  :  1;  // Bits 5:5
    UINT32 mc0_side_POK_val                        :  1;  // Bits 6:6
    UINT32 mc1_side_POK_val                        :  1;  // Bits 7:7
    UINT32 cce0_side_POK_val                       :  1;  // Bits 8:8
    UINT32 cce1_side_POK_val                       :  1;  // Bits 9:9
    UINT32 ibecc0_side_POK_val                     :  1;  // Bits 10:10
    UINT32 ibecc1_side_POK_val                     :  1;  // Bits 11:11
    UINT32 ondd_side_POK_val                       :  1;  // Bits 12:12
    UINT32 pll_IP_READY_val                        :  1;  // Bits 13:13
    UINT32 svrs_mem0_IP_READY_val                  :  1;  // Bits 14:14
    UINT32 svrs_mem1_IP_READY_val                  :  1;  // Bits 15:15
    UINT32 ondd_IP_READY_val                       :  1;  // Bits 16:16
    UINT32 ddrio_fuse_pull_done_val                :  1;  // Bits 17:17
    UINT32 ondd_block_ack_val                      :  1;  // Bits 18:18
    UINT32 Reserved                                :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_STRUCT;
typedef union {
  struct {
    UINT32 DATA                                    :  26;  // Bits 25:0
    UINT32 RESERVED0                               :  1;  // Bits 26:26
    UINT32 PKGC_EXIT_Counter_En                    :  1;  // Bits 27:27
    UINT32 PKGC_ENTRY_Counter_En                   :  1;  // Bits 28:28
    UINT32 SAGV_Counter_En                         :  1;  // Bits 29:29
    UINT32 RESERVED1                               :  1;  // Bits 30:30
    UINT32 OPP_SR_DISABLE_Counter_En               :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GLOBAL_B2B_FLOW_DELAY_STRUCT;
typedef union {
  struct {
    UINT32 MC0_SKIP_BISR                           :  1;  // Bits 0:0
    UINT32 MC1_SKIP_BISR                           :  1;  // Bits 1:1
    UINT32 CCE0_SKIP_BISR                          :  1;  // Bits 2:2
    UINT32 CCE1_SKIP_BISR                          :  1;  // Bits 3:3
    UINT32 IBECC0_SKIP_BISR                        :  1;  // Bits 4:4
    UINT32 IBECC1_SKIP_BISR                        :  1;  // Bits 5:5
    UINT32 ONDD_SKIP_BISR                          :  1;  // Bits 6:6
    UINT32 SRFSM0_SKIP_BISR                        :  1;  // Bits 7:7
    UINT32 SRFSM1_SKIP_BISR                        :  1;  // Bits 8:8
    UINT32                                         :  7;  // Bits 15:9
    UINT32 MC0_BISR_ACK_STS                        :  1;  // Bits 16:16
    UINT32 MC1_BISR_ACK_STS                        :  1;  // Bits 17:17
    UINT32 CCE0_BISR_ACK_STS                       :  1;  // Bits 18:18
    UINT32 CCE1_BISR_ACK_STS                       :  1;  // Bits 19:19
    UINT32 IBECC0_BISR_ACK_STS                     :  1;  // Bits 20:20
    UINT32 IBECC1_BISR_ACK_STS                     :  1;  // Bits 21:21
    UINT32 ONDD_BISR_ACK_STS                       :  1;  // Bits 22:22
    UINT32 SRFSM0_BISR_ACK_STS                     :  1;  // Bits 23:23
    UINT32 SRFSM1_BISR_ACK_STS                     :  1;  // Bits 24:24
    UINT32 DDRPHY_BISR_ACK_STS                     :  1;  // Bits 25:25
    UINT32 DDRPHY_SR_BISR_ACK_STS                  :  1;  // Bits 26:26
    UINT32 Reserved                                :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_BISR_STRUCT;
typedef union {
  struct {
    UINT32 CLKS_FORCE_ON_CFG                       :  1;  // Bits 0:0
    UINT32 FORCE_ON_RST_CNTR_DLY                   :  8;  // Bits 8:1
    UINT32 FORCE_ON_PWRG_CNTR_DLY                  :  8;  // Bits 16:9
    UINT32 PKGC_ENTRY_PM8_PARALEL_TO_BLOCK_IPS     :  1;  // Bits 17:17
    UINT32 PKGC_RST_ENTRY_SEND_PM6_AFTER_ASSRT_RST_TO_IPS:  1;  // Bits 18:18
    UINT32 PKGC_EXIT_GATHER_PM0_ACK_BEFORE_DASSRT_RST_TO_IPS:  1;  // Bits 19:19
    UINT32 PWRGATE_TM_CBIT_USE_OLD_RTL_FOR_GATE_ACKS_ARC:  1;  // Bits 20:20
    UINT32 CBIT_CLKGATE_MCTRAIN_EN                 :  1;  // Bits 21:21
    UINT32 Reserved                                :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_MISC_1_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_MC_IN_SR_COUNT_MC0_STRUCT;

typedef MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_STRUCT MEMSS_PMA_CR_MC_IN_SR_COUNT_MC1_STRUCT;
typedef union {
  struct {
    UINT32 SKIP_PM_DFI_MSG_DDRPHY_PMA              :  1;  // Bits 0:0
    UINT32 SKIP_PM8_RST_ENTRY                      :  1;  // Bits 1:1
    UINT32 CB_VIRTUAL_INTERRUPT_EN                 :  1;  // Bits 2:2
    UINT32 RE_VIRTUAL_INTERRUPT_EN                 :  1;  // Bits 3:3
    UINT32 PCE_VIRTUAL_INTERRUPT_EN                :  1;  // Bits 4:4
    UINT32 PCX_VIRTUAL_INTERRUPT_EN                :  1;  // Bits 5:5
    UINT32 SAGV_VIRTUAL_INTERRUPT_EN               :  1;  // Bits 6:6
    UINT32                                         :  25;  // Bits 31:7
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_STRUCT;
typedef union {
  struct {
    UINT32 PG_TM_PWR_DOWN_DLY                      :  8;  // Bits 7:0
    UINT32 PG_TM_PWR_UP_DLY                        :  8;  // Bits 15:8
    UINT32 RST_TM_ASRT_RST_DLY                     :  8;  // Bits 23:16
    UINT32 RST_TM_DASRT_RST_DLY                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_STRUCT;
typedef union {
  struct {
    UINT32 MRC_START_AUTO_ACK                      :  1;  // Bits 0:0
    UINT32                                         :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_HVM_OVERRIDE_STRUCT;
typedef union {
  struct {
    UINT32 DIS_IPS_PWRGOOD_TO_SLEEP_DLY            :  8;  // Bits 7:0
    UINT32 DIS_IPS_SLEEP_TO_PWRDOWN_DLY            :  8;  // Bits 15:8
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_STRUCT;
typedef union {
  struct {
    UINT32 SHUTOFF_WAIT_CYCLES                     :  4;  // Bits 3:0
    UINT32 SHUTOFF_RUN_BUSY                        :  1;  // Bits 4:4
    UINT32 SHUTOFF_IP_SEL                          :  6;  // Bits 10:5
    UINT32 SHUTOFF_DIRECTION                       :  1;  // Bits 11:11
    UINT32 Reserved                                :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SURVIVABILITY_MISC_2_STRUCT;
typedef union {
  struct {
    UINT32 SPARE_CBITS                             :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_SPARE_CBITS_STRUCT;
typedef union {
  struct {
    UINT32 QCLK_RATIO                              :  8;  // Bits 7:0
    UINT32 GEAR_TYPE                               :  1;  // Bits 8:8
    UINT32 VDDQ_V                                  :  5;  // Bits 13:9
    UINT32 QCLK_WP_IDX                             :  2;  // Bits 15:14
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_STRUCT;

#pragma pack(pop)
#endif
