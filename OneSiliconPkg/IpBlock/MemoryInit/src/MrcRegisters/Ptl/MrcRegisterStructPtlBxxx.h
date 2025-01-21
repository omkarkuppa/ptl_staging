#ifndef __MrcRegisterStructPtlBxxx_h__
#define __MrcRegisterStructPtlBxxx_h__

/**
@file
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
    UINT32 SRE_DLY                                 :  16;  // Bits 15:0
    UINT32 RSVD18_16                               :  3;  // Bits 18:16
    UINT32 DYN_SR_EN                               :  1;  // Bits 19:19
    UINT32 RSVD24_20                               :  5;  // Bits 24:20
    UINT32 PHY_IC_QUAL_EN                          :  1;  // Bits 25:25
    UINT32 DFI_INIT_START_CTRL                     :  2;  // Bits 27:26
    UINT32 SRE_WFC                                 :  1;  // Bits 28:28
    UINT32 MSRE                                    :  1;  // Bits 29:29
    UINT32 MSRX                                    :  1;  // Bits 30:30
    UINT32 SRE_WO_PD                               :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_SR0_STRUCT;
typedef union {
  struct {
    UINT32 LPMODE0P5_WAKEUP_TIME                   :  4;  // Bits 3:0
    UINT32 LPMODE2_WAKEUP_TIME                     :  4;  // Bits 7:4
    UINT32 LPMODE3_WAKEUP_TIME                     :  4;  // Bits 11:8
    UINT32 LPMODE4_WAKEUP_TIME                     :  4;  // Bits 15:12
    UINT32 TOGGLE_SR_WAKEUP_TIME                   :  1;  // Bits 16:16
    UINT32 RSVD23_17                               :  7;  // Bits 23:17
    UINT32 LP_REQ_TMO_DLY                          :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_LPCTRL0_STRUCT;
typedef union {
  struct {
    UINT32 LPMODE0P5_MIN_RES                       :  8;  // Bits 7:0
    UINT32 LPMODE2_MIN_RES                         :  8;  // Bits 15:8
    UINT32 LPMODE3_MIN_RES                         :  8;  // Bits 23:16
    UINT32 LPMODE4_MIN_RES                         :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_LPCTRL1_STRUCT;
typedef union {
  struct {
    UINT32 REF_WM                                  :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 REF_RATE                                :  3;  // Bits 6:4
    UINT32 RSVD25_7                                :  19;  // Bits 25:7
    UINT32 MREF                                    :  1;  // Bits 26:26
    UINT32 FLUSH_REF                               :  1;  // Bits 27:27
    UINT32 POSTPONED_REFS                          :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_REF_STRUCT;
typedef union {
  struct {
    UINT32 ZQCAL_INTV                              :  14;  // Bits 13:0
    UINT32 RSVD15_14                               :  2;  // Bits 15:14
    UINT32 ZQCAL_EN                                :  1;  // Bits 16:16
    UINT32 ZQCAL_MODE                              :  1;  // Bits 17:17
    UINT32 RSVD18                                  :  1;  // Bits 18:18
    UINT32 MZQCAL_START                            :  1;  // Bits 19:19
    UINT32 RSVD31_20                               :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_ZQCAL_STRUCT;
typedef union {
  struct {
    UINT32 DQS2DQ_CAL_INTV                         :  14;  // Bits 13:0
    UINT32 RSVD16_14                               :  3;  // Bits 16:14
    UINT32 DQS2DQ_CAL_OSC_RT                       :  10;  // Bits 26:17
    UINT32 DQS2DQ_CAL_EN                           :  1;  // Bits 27:27
    UINT32 MDQS2DQ_CAL_START                       :  1;  // Bits 28:28
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_DQS2DQ_CAL_STRUCT;
typedef union {
  struct {
    UINT32 WCK2DQO_CAL_INTV                        :  14;  // Bits 13:0
    UINT32 RSVD16_14                               :  3;  // Bits 16:14
    UINT32 WCK2DQO_CAL_OSC_RT                      :  10;  // Bits 26:17
    UINT32 WCK2DQO_CAL_EN                          :  1;  // Bits 27:27
    UINT32 MWCK2DQO_CAL_START                      :  1;  // Bits 28:28
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_WCK2DQO_CAL_STRUCT;
typedef union {
  struct {
    UINT32 CBT                                     :  1;  // Bits 0:0
    UINT32 RCVEN_CAL                               :  1;  // Bits 1:1
    UINT32 RD_DQCAL                                :  1;  // Bits 2:2
    UINT32 WR_LEVELING                             :  1;  // Bits 3:3
    UINT32 PDA                                     :  1;  // Bits 4:4
    UINT32 DQS2DQ_FIFO_CAL                         :  1;  // Bits 5:5
    UINT32 CBT_PH                                  :  1;  // Bits 6:6
    UINT32 DQ_LOOPBACK                             :  1;  // Bits 7:7
    UINT32 RSVD23_8                                :  16;  // Bits 23:8
    UINT32 BLOCK_ACT                               :  1;  // Bits 24:24
    UINT32 BLOCK_WRDATA                            :  1;  // Bits 25:25
    UINT32 BLOCK_RDDATA                            :  4;  // Bits 29:26
    UINT32 RSVD30                                  :  1;  // Bits 30:30
    UINT32 CAPTURE_FB_DATA                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_TRAIN_STRUCT;
typedef union {
  struct {
    UINT32 TRAIN_FB_DATA0                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_TRAIN_FB_DATA0_STRUCT;
typedef union {
  struct {
    UINT32 TRAIN_FB_DATA1                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_TRAIN_FB_DATA1_STRUCT;
typedef union {
  struct {
    UINT32 RSVD9_0                                 :  10;  // Bits 9:0
    UINT32 CS_MODE                                 :  2;  // Bits 11:10
    UINT32 CS_VAL                                  :  4;  // Bits 15:12
    UINT32 RSVD23_16                               :  8;  // Bits 23:16
    UINT32 TURN_WCK_ON                             :  1;  // Bits 24:24
    UINT32 TURN_WCK_OFF                            :  1;  // Bits 25:25
    UINT32 ASSERT_DFI_WRDATA_EN                    :  1;  // Bits 26:26
    UINT32 ASSERT_DFI_RDDATA_EN                    :  1;  // Bits 27:27
    UINT32 SET_DFI_XDATA_CS                        :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_MCTRL_STRUCT;
typedef union {
  struct {
    UINT32 RSVD3_0                                 :  4;  // Bits 3:0
    UINT32 CS_MASK                                 :  2;  // Bits 5:4
    UINT32 RSVD7_6                                 :  2;  // Bits 7:6
    UINT32 MULTI_CYCLE_CS                          :  1;  // Bits 8:8
    UINT32 RSVD15_9                                :  7;  // Bits 15:9
    UINT32 CA_MASK                                 :  4;  // Bits 19:16
    UINT32 RSVD31_20                               :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_DRAM_CMDS_STRUCT;
typedef union {
  struct {
    UINT32 RPT_CMD_EN                              :  1;  // Bits 0:0
    UINT32 RPT_NONSTOP                             :  1;  // Bits 1:1
    UINT32 RPT_MPAUSE                              :  1;  // Bits 2:2
    UINT32 RSVD7_3                                 :  5;  // Bits 7:3
    UINT32 RPT_CMD_DLY                             :  8;  // Bits 15:8
    UINT32 RPT_CMD_CNT                             :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_DRAM_CMDS1_STRUCT;
typedef union {
  struct {
    UINT32 DATA                                    :  28;  // Bits 27:0
    UINT32 CS                                      :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_DRAM_CMD_STRUCT;
typedef union {
  struct {
    UINT32 OP                                      :  8;  // Bits 7:0
    UINT32 MA                                      :  8;  // Bits 15:8
    UINT32 RSVD19_16                               :  4;  // Bits 19:16
    UINT32 CMD                                     :  2;  // Bits 21:20
    UINT32 RSVD23_22                               :  2;  // Bits 23:22
    UINT32 CS                                      :  4;  // Bits 27:24
    UINT32 FIFO_LEVEL                              :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_PS_DRAM_CMD_STRUCT;
typedef union {
  struct {
    UINT32 MRR_DATA0                               :  8;  // Bits 7:0
    UINT32 MRR_DATA1                               :  8;  // Bits 15:8
    UINT32 MRR_DATA2                               :  8;  // Bits 23:16
    UINT32 MRR_DATA3                               :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_MRRD0_STRUCT;
typedef union {
  struct {
    UINT32 MRR_DATA4                               :  8;  // Bits 7:0
    UINT32 MRR_DATA5                               :  8;  // Bits 15:8
    UINT32 MRR_DATA6                               :  8;  // Bits 23:16
    UINT32 MRR_DATA7                               :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_MRRD1_STRUCT;
typedef union {
  struct {
    UINT32 MRR_DATA8                               :  8;  // Bits 7:0
    UINT32 MRR_DATA9                               :  8;  // Bits 15:8
    UINT32 RSVD31_16                               :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_MRRD2_STRUCT;
typedef union {
  struct {
    UINT32 RSVD1_0                                 :  2;  // Bits 1:0
    UINT32 MNT_EN                                  :  1;  // Bits 2:2
    UINT32 RSVD4_3                                 :  2;  // Bits 4:3
    UINT32 DFI_DDR5_2N_CS_POS                      :  1;  // Bits 5:5
    UINT32 EARLY_DFI_CA_VALID                      :  1;  // Bits 6:6
    UINT32 RSVD27_7                                :  21;  // Bits 27:7
    UINT32 WR_DBI_EN                               :  1;  // Bits 28:28
    UINT32 RD_DBI_EN                               :  1;  // Bits 29:29
    UINT32 RSVD30                                  :  1;  // Bits 30:30
    UINT32 DUNIT_INIT_DN                           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_DOC_STRUCT;
typedef union {
  struct {
    UINT32 IN_PD                                   :  1;  // Bits 0:0
    UINT32 IN_SR                                   :  1;  // Bits 1:1
    UINT32 RSVD2                                   :  1;  // Bits 2:2
    UINT32 ALL_BANKS_CLOSED                        :  1;  // Bits 3:3
    UINT32 ALL_QUEUES_EMPTY                        :  1;  // Bits 4:4
    UINT32 CMDQ_EMPTY                              :  1;  // Bits 5:5
    UINT32 WDB_EMPTY                               :  1;  // Bits 6:6
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 RD_IN_PROGRESS                          :  4;  // Bits 11:8
    UINT32 RSVD15_12                               :  4;  // Bits 15:12
    UINT32 WCK_ON                                  :  1;  // Bits 16:16
    UINT32 RSVD19_17                               :  3;  // Bits 19:17
    UINT32 MEM_REQ                                 :  1;  // Bits 20:20
    UINT32 REF_REQ                                 :  1;  // Bits 21:21
    UINT32 ZQCAL_REQ                               :  1;  // Bits 22:22
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 DQS2DQ_CAL_REQ                          :  1;  // Bits 24:24
    UINT32 WCK2DQO_CAL_REQ                         :  1;  // Bits 25:25
    UINT32 PHYUPD_REQ                              :  1;  // Bits 26:26
    UINT32 RSVD27                                  :  1;  // Bits 27:27
    UINT32 PHY_INIT_COMPLETE                       :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 R0_BANK_STATUS                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_BANK_STATUS0_STRUCT;
typedef union {
  struct {
    UINT32 R1_BANK_STATUS                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_BANK_STATUS1_STRUCT;
typedef union {
  struct {
    UINT32 R2_BANK_STATUS                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_BANK_STATUS2_STRUCT;
typedef union {
  struct {
    UINT32 R3_BANK_STATUS                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_BANK_STATUS3_STRUCT;
typedef union {
  struct {
    UINT32 DCMDQ_RST                               :  1;  // Bits 0:0
    UINT32 DTMR_RST                                :  1;  // Bits 1:1
    UINT32 DWRBUF_RST                              :  1;  // Bits 2:2
    UINT32 DRDBUF_RST                              :  1;  // Bits 3:3
    UINT32 RSVD4                                   :  1;  // Bits 4:4
    UINT32 MTG_RST                                 :  1;  // Bits 5:5
    UINT32 RSVD7_6                                 :  2;  // Bits 7:6
    UINT32 CLR_BANK_STATUS                         :  1;  // Bits 8:8
    UINT32 CLR_MRR_DATA                            :  1;  // Bits 9:9
    UINT32 RSVD10                                  :  1;  // Bits 10:10
    UINT32 CLR_TRAIN_FB_DATA                       :  1;  // Bits 11:11
    UINT32 RSVD15_12                               :  4;  // Bits 15:12
    UINT32 IGNORE_PHYUPD_REQ                       :  1;  // Bits 16:16
    UINT32 RSVD18_17                               :  2;  // Bits 18:17
    UINT32 DUNIT2DRAM_PROP_DLY                     :  8;  // Bits 26:19
    UINT32 RSVD28_27                               :  2;  // Bits 28:27
    UINT32 EVEN_PHASE_CMD                          :  1;  // Bits 29:29
    UINT32 RSVD30                                  :  1;  // Bits 30:30
    UINT32 BG_SWAP_DIS                             :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_MISC0_STRUCT;
typedef union {
  struct {
    UINT32 BONUS0                                  :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_BONUS0_STRUCT;
typedef union {
  struct {
    UINT32 BONUS1                                  :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_BONUS1_STRUCT;
typedef union {
  struct {
    UINT32 DGB_DPHASE_SEL                          :  2;  // Bits 1:0
    UINT32 DGB_D2XCLK_MODE                         :  1;  // Bits 2:2
    UINT32 DGB_TX_1XCLK_ALIGN                      :  1;  // Bits 3:3
    UINT32 DGB_PPHASE_SEL                          :  2;  // Bits 5:4
    UINT32 DGB_P2XCLK_MODE                         :  1;  // Bits 6:6
    UINT32 DGB_RX_1XCLK_ALIGN                      :  1;  // Bits 7:7
    UINT32 DGB_TX_WPTR_INIT_VALUE                  :  4;  // Bits 11:8
    UINT32 RSVD12                                  :  1;  // Bits 12:12
    UINT32 DGB_TX_RPTR_INIT_VALUE                  :  4;  // Bits 16:13
    UINT32 RSVD17                                  :  1;  // Bits 17:17
    UINT32 DGB_RX_WPTR_INIT_VALUE                  :  4;  // Bits 21:18
    UINT32 RSVD22                                  :  1;  // Bits 22:22
    UINT32 DGB_RX_RPTR_INIT_VALUE                  :  4;  // Bits 26:23
    UINT32 RSVD30_27                               :  4;  // Bits 30:27
    UINT32 DGB_FIFO_PTR_EN                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_D_CR_DGB_STRUCT;
typedef union {
  struct {
    UINT32 START                                   :  1;  // Bits 0:0
    UINT32 NONSTOP                                 :  1;  // Bits 1:1
    UINT32 CATG_MODE                               :  3;  // Bits 4:2
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 SEL_PERIOD                              :  8;  // Bits 15:8
    UINT32 SEL_CS                                  :  4;  // Bits 19:16
    UINT32 SEL_PHASE                               :  2;  // Bits 21:20
    UINT32 RSVD31_22                               :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 CAPAT_ROTATE_EN                         :  1;  // Bits 0:0
    UINT32 RST_CAPAT_ON_ROTATE                     :  1;  // Bits 1:1
    UINT32 VICTIM_CAPAT_SEL                        :  1;  // Bits 2:2
    UINT32 AGGRESSOR_CAPAT_SEL                     :  1;  // Bits 3:3
    UINT32 CAPAT_ROTATE_RATE                       :  16;  // Bits 19:4
    UINT32 RSVD23_20                               :  4;  // Bits 23:20
    UINT32 CAPAT_PARITY_EN                         :  1;  // Bits 24:24
    UINT32 CAPAT_PARITY_TYPE                       :  1;  // Bits 25:25
    UINT32 CALC_PARITY_WITH_CA13                   :  1;  // Bits 26:26
    UINT32 CAPAT_PARITY_PIN                        :  3;  // Bits 29:27
    UINT32 RSVD31_30                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_CTRL1_STRUCT;
typedef union {
  struct {
    UINT32 CATG_RUNTIME                            :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_CTRL2_STRUCT;
typedef union {
  struct {
    UINT32 CA_SEL_PPAT                             :  14;  // Bits 13:0
    UINT32 RSVD14                                  :  1;  // Bits 14:14
    UINT32 CA_SEL_PPAT_EN                          :  1;  // Bits 15:15
    UINT32 CA_DSEL_PPAT                            :  14;  // Bits 29:16
    UINT32 RSVD30                                  :  1;  // Bits 30:30
    UINT32 CA_DSEL_PPAT_EN                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_CAPPATSEL_STRUCT;
typedef union {
  struct {
    UINT32 CA0_PATSEL                              :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 CA1_PATSEL                              :  3;  // Bits 6:4
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 CA2_PATSEL                              :  3;  // Bits 10:8
    UINT32 RSVD11                                  :  1;  // Bits 11:11
    UINT32 CA3_PATSEL                              :  3;  // Bits 14:12
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 CA4_PATSEL                              :  3;  // Bits 18:16
    UINT32 RSVD19                                  :  1;  // Bits 19:19
    UINT32 CA5_PATSEL                              :  3;  // Bits 22:20
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 CA6_PATSEL                              :  3;  // Bits 26:24
    UINT32 RSVD31_27                               :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_CAPATSEL_STRUCT;
typedef union {
  struct {
    UINT32 CS0_PATSEL                              :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 CS1_PATSEL                              :  3;  // Bits 6:4
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 CS2_PATSEL                              :  3;  // Bits 10:8
    UINT32 RSVD11                                  :  1;  // Bits 11:11
    UINT32 CS3_PATSEL                              :  3;  // Bits 14:12
    UINT32 RSVD31_15                               :  17;  // Bits 31:15
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_CSPATSEL_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT0_0                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSP0_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT0_1                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSP0_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT1_0                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSP1_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT1_1                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSP1_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT2_0                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSP2_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT2_1                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSP2_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT3_0                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSP3_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT3_1                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSP3_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_PAT0_LEN                         :  2;  // Bits 1:0
    UINT32 RSVD3_2                                 :  2;  // Bits 3:2
    UINT32 SERIAL_PAT1_LEN                         :  2;  // Bits 5:4
    UINT32 RSVD7_6                                 :  2;  // Bits 7:6
    UINT32 SERIAL_PAT2_LEN                         :  2;  // Bits 9:8
    UINT32 RSVD11_10                               :  2;  // Bits 11:10
    UINT32 SERIAL_PAT3_LEN                         :  2;  // Bits 13:12
    UINT32 RSVD31_14                               :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_SSPL_STRUCT;
typedef union {
  struct {
    UINT32 LFSR0_POLY                              :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 LFSR1_POLY                              :  3;  // Bits 6:4
    UINT32 RSVD31_7                                :  25;  // Bits 31:7
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_LFSR_POLY_STRUCT;
typedef union {
  struct {
    UINT32 LFSR0_SEED                              :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_LFSR0_SEED_STRUCT;
typedef union {
  struct {
    UINT32 LFSR1_SEED                              :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_LFSR1_SEED_STRUCT;
typedef union {
  struct {
    UINT32 CA_DC                                   :  14;  // Bits 13:0
    UINT32 RSVD15_14                               :  2;  // Bits 15:14
    UINT32 CA_INV                                  :  14;  // Bits 29:16
    UINT32 RSVD31_30                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_CA_DCINVC_STRUCT;
typedef union {
  struct {
    UINT32 CS_DC                                   :  4;  // Bits 3:0
    UINT32 CS_INV                                  :  4;  // Bits 7:4
    UINT32 RSVD31_8                                :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_CATG_CS_DCINVC_STRUCT;
typedef union {
  struct {
    UINT32 START                                   :  1;  // Bits 0:0
    UINT32 NONSTOP                                 :  1;  // Bits 1:1
    UINT32 MANUAL_PAUSE                            :  1;  // Bits 2:2
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 SINGLE_RD_MODE                          :  1;  // Bits 4:4
    UINT32 SINGLE_RD_PAUSED                        :  1;  // Bits 5:5
    UINT32 SINGLE_RD_CONTINUE                      :  1;  // Bits 6:6
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 REQ_SIZE                                :  1;  // Bits 8:8
    UINT32 RSVD31_9                                :  23;  // Bits 31:9
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_MAIN_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 CMD_CNT                                 :  10;  // Bits 9:0
    UINT32 RSVD11_10                               :  2;  // Bits 11:10
    UINT32 CMD_TYPE                                :  3;  // Bits 14:12
    UINT32 LAST_ENTRY                              :  1;  // Bits 15:15
    UINT32 WDATA_TYPE                              :  2;  // Bits 17:16
    UINT32 RDATA_TYPE                              :  2;  // Bits 19:18
    UINT32 ROW_OFFSET                              :  4;  // Bits 23:20
    UINT32 ERRCHK_DIS                              :  1;  // Bits 24:24
    UINT32 RSVD27_25                               :  3;  // Bits 27:25
    UINT32 ADDR_MODE                               :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CE0_STRUCT;
typedef union {
  struct {
    UINT32 CMD_CNT                                 :  10;  // Bits 9:0
    UINT32 RSVD11_10                               :  2;  // Bits 11:10
    UINT32 CMD_TYPE                                :  3;  // Bits 14:12
    UINT32 LAST_ENTRY                              :  1;  // Bits 15:15
    UINT32 WDATA_TYPE                              :  2;  // Bits 17:16
    UINT32 RDATA_TYPE                              :  2;  // Bits 19:18
    UINT32 ROW_OFFSET                              :  4;  // Bits 23:20
    UINT32 ERRCHK_DIS                              :  1;  // Bits 24:24
    UINT32 RSVD27_25                               :  3;  // Bits 27:25
    UINT32 ADDR_MODE                               :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CE1_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE2_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE3_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE4_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE5_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE6_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE7_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE8_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE9_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE10_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE11_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE12_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE13_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE14_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG0_CR_MTG_CE15_STRUCT;
typedef union {
  struct {
    UINT32 CMD_ENTRY_DLY                           :  16;  // Bits 15:0
    UINT32 CMD_LOOP_DLY                            :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CDLY_STRUCT;
typedef union {
  struct {
    UINT32 CMD_LOOP_CNT                            :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CLC_STRUCT;
typedef union {
  struct {
    UINT32 CMD_RAND_MODE                           :  2;  // Bits 1:0
    UINT32 RSVD15_2                                :  14;  // Bits 15:2
    UINT32 CMD_RAND_WIDTH                          :  4;  // Bits 19:16
    UINT32 CMD_RAND_SEED                           :  10;  // Bits 29:20
    UINT32 RSVD31_30                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CRANDC_STRUCT;
typedef union {
  struct {
    UINT32 MWR_MODE                                :  2;  // Bits 1:0
    UINT32 RSVD3_2                                 :  2;  // Bits 3:2
    UINT32 MWR_RAND_SEED                           :  10;  // Bits 13:4
    UINT32 RSVD31_14                               :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_MWRC_STRUCT;
typedef union {
  struct {
    UINT32 MWR_ORDER                               :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_MWRO_STRUCT;
typedef union {
  struct {
    UINT32 RANK_MIN_ADDR                           :  2;  // Bits 1:0
    UINT32 RSVD3_2                                 :  2;  // Bits 3:2
    UINT32 BANK_MIN_ADDR                           :  5;  // Bits 8:4
    UINT32 RSVD10_9                                :  2;  // Bits 10:9
    UINT32 COL_MIN_ADDR                            :  10;  // Bits 20:11
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 LRANK_MIN_ADDR                          :  4;  // Bits 27:24
    UINT32 RSVD31_28                               :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_ARMIN0_STRUCT;
typedef union {
  struct {
    UINT32 ROW_MIN_ADDR                            :  18;  // Bits 17:0
    UINT32 RSVD31_18                               :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_ARMIN1_STRUCT;
typedef union {
  struct {
    UINT32 RANK_MAX_ADDR                           :  2;  // Bits 1:0
    UINT32 RSVD3_2                                 :  2;  // Bits 3:2
    UINT32 BANK_MAX_ADDR                           :  5;  // Bits 8:4
    UINT32 RSVD10_9                                :  2;  // Bits 10:9
    UINT32 COL_MAX_ADDR                            :  10;  // Bits 20:11
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 LRANK_MAX_ADDR                          :  4;  // Bits 27:24
    UINT32 RSVD31_28                               :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_ARMAX0_STRUCT;
typedef union {
  struct {
    UINT32 ROW_MAX_ADDR                            :  18;  // Bits 17:0
    UINT32 RSVD31_18                               :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_ARMAX1_STRUCT;
typedef union {
  struct {
    UINT32 L2P_RANK_MAP0                           :  2;  // Bits 1:0
    UINT32 L2P_RANK_MAP1                           :  2;  // Bits 3:2
    UINT32 L2P_RANK_MAP2                           :  2;  // Bits 5:4
    UINT32 L2P_RANK_MAP3                           :  2;  // Bits 7:6
    UINT32 L2P_RANK_MAP4                           :  2;  // Bits 9:8
    UINT32 L2P_RANK_MAP5                           :  2;  // Bits 11:10
    UINT32 L2P_RANK_MAP6                           :  2;  // Bits 13:12
    UINT32 L2P_RANK_MAP7                           :  2;  // Bits 15:14
    UINT32 L2P_RANK_MAP8                           :  2;  // Bits 17:16
    UINT32 L2P_RANK_MAP9                           :  2;  // Bits 19:18
    UINT32 L2P_RANK_MAP10                          :  2;  // Bits 21:20
    UINT32 L2P_RANK_MAP11                          :  2;  // Bits 23:22
    UINT32 RSVD30_24                               :  7;  // Bits 30:24
    UINT32 L2P_RANK_MAP_EN                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_L2PRM_STRUCT;
typedef union {
  struct {
    UINT32 ADDR_ORDER_MODE                         :  1;  // Bits 0:0
    UINT32 RSVD3_1                                 :  3;  // Bits 3:1
    UINT32 ADDR_ORDER                              :  4;  // Bits 7:4
    UINT32 BANK_INC_ORDER                          :  3;  // Bits 10:8
    UINT32 RSVD11                                  :  1;  // Bits 11:11
    UINT32 RANK_INC                                :  1;  // Bits 12:12
    UINT32 RSVD13                                  :  1;  // Bits 13:13
    UINT32 BANK_INC                                :  2;  // Bits 15:14
    UINT32 RSVD16                                  :  1;  // Bits 16:16
    UINT32 ROW_INC                                 :  4;  // Bits 20:17
    UINT32 RSVD21                                  :  1;  // Bits 21:21
    UINT32 COL_INC                                 :  3;  // Bits 24:22
    UINT32 RSVD27_25                               :  3;  // Bits 27:25
    UINT32 ALWAYS_INC_RANK_ADDR                    :  1;  // Bits 28:28
    UINT32 ALWAYS_INC_BANK_ADDR                    :  1;  // Bits 29:29
    UINT32 ALWAYS_INC_ROW_ADDR                     :  1;  // Bits 30:30
    UINT32 ALWAYS_INC_COL_ADDR                     :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_AO_STRUCT;
typedef union {
  struct {
    UINT32 ADDR_RAND_MODE                          :  1;  // Bits 0:0
    UINT32 RSVD4_1                                 :  4;  // Bits 4:1
    UINT32 BANK_RAND_EN                            :  1;  // Bits 5:5
    UINT32 ROW_RAND_EN                             :  1;  // Bits 6:6
    UINT32 COL_RAND_EN                             :  1;  // Bits 7:7
    UINT32 RSVD11_8                                :  4;  // Bits 11:8
    UINT32 RAND_RANK_WIDTH                         :  2;  // Bits 13:12
    UINT32 RSVD14                                  :  1;  // Bits 14:14
    UINT32 RAND_BANK_WIDTH                         :  3;  // Bits 17:15
    UINT32 RSVD18                                  :  1;  // Bits 18:18
    UINT32 RAND_ROW_WIDTH                          :  5;  // Bits 23:19
    UINT32 RSVD24                                  :  1;  // Bits 24:24
    UINT32 RAND_COL_WIDTH                          :  4;  // Bits 28:25
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_ARC_STRUCT;
typedef union {
  struct {
    UINT32 RSVD6_0                                 :  7;  // Bits 6:0
    UINT32 BANK_RAND_SEED                          :  5;  // Bits 11:7
    UINT32 RSVD13_12                               :  2;  // Bits 13:12
    UINT32 COL_RAND_SEED                           :  10;  // Bits 23:14
    UINT32 RSVD31_24                               :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_ARS0_STRUCT;
typedef union {
  struct {
    UINT32 ROW_RAND_SEED                           :  18;  // Bits 17:0
    UINT32 RSVD31_18                               :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_ARS1_STRUCT;
typedef union {
  struct {
    UINT32 RST_ADDR_PCE                            :  1;  // Bits 0:0
    UINT32 RST_DQPAT_PCE                           :  1;  // Bits 1:1
    UINT32 RST_DMPAT_PCE                           :  1;  // Bits 2:2
    UINT32 RSVD11_3                                :  9;  // Bits 11:3
    UINT32 RST_ADDR_EN                             :  1;  // Bits 12:12
    UINT32 RST_DQPAT_EN                            :  1;  // Bits 13:13
    UINT32 RST_DMPAT_EN                            :  1;  // Bits 14:14
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 RST_ADDR_DPAT_THRES                     :  10;  // Bits 25:16
    UINT32 RSVD31_26                               :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_RADC_STRUCT;
typedef union {
  struct {
    UINT32 DQ0_PATSEL                              :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 DQ1_PATSEL                              :  3;  // Bits 6:4
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 DQ2_PATSEL                              :  3;  // Bits 10:8
    UINT32 RSVD11                                  :  1;  // Bits 11:11
    UINT32 DQ3_PATSEL                              :  3;  // Bits 14:12
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 DQ4_PATSEL                              :  3;  // Bits 18:16
    UINT32 RSVD19                                  :  1;  // Bits 19:19
    UINT32 DQ5_PATSEL                              :  3;  // Bits 22:20
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 DQ6_PATSEL                              :  3;  // Bits 26:24
    UINT32 RSVD27                                  :  1;  // Bits 27:27
    UINT32 DQ7_PATSEL                              :  3;  // Bits 30:28
    UINT32 RSVD31                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQPATSEL0_STRUCT;
typedef union {
  struct {
    UINT32 DQ8_PATSEL                              :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 DQ9_PATSEL                              :  3;  // Bits 6:4
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 DQ10_PATSEL                             :  3;  // Bits 10:8
    UINT32 RSVD11                                  :  1;  // Bits 11:11
    UINT32 DQ11_PATSEL                             :  3;  // Bits 14:12
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 DQ12_PATSEL                             :  3;  // Bits 18:16
    UINT32 RSVD19                                  :  1;  // Bits 19:19
    UINT32 DQ13_PATSEL                             :  3;  // Bits 22:20
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 DQ14_PATSEL                             :  3;  // Bits 26:24
    UINT32 RSVD27                                  :  1;  // Bits 27:27
    UINT32 DQ15_PATSEL                             :  3;  // Bits 30:28
    UINT32 RSVD31                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQPATSEL1_STRUCT;
typedef union {
  struct {
    UINT32 RSVD1_0                                 :  2;  // Bits 1:0
    UINT32 DQ_BYTE2_MAP                            :  1;  // Bits 2:2
    UINT32 DQ_BYTE3_MAP                            :  1;  // Bits 3:3
    UINT32 RSVD7_4                                 :  4;  // Bits 7:4
    UINT32 ECC_DQ_BYTE_MAP                         :  1;  // Bits 8:8
    UINT32 RSVD31_9                                :  23;  // Bits 31:9
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_UPDQBM_STRUCT;
typedef union {
  struct {
    UINT32 DM0_PATSEL                              :  2;  // Bits 1:0
    UINT32 RSVD3_2                                 :  2;  // Bits 3:2
    UINT32 DM1_PATSEL                              :  2;  // Bits 5:4
    UINT32 RSVD7_6                                 :  2;  // Bits 7:6
    UINT32 DM2_PATSEL                              :  2;  // Bits 9:8
    UINT32 RSVD11_10                               :  2;  // Bits 11:10
    UINT32 DM3_PATSEL                              :  2;  // Bits 13:12
    UINT32 RSVD31_14                               :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DMPATSEL_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT0_0                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDQP0_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT0_1                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDQP0_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT1_0                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDQP1_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT1_1                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDQP1_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT2_0                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDQP2_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT2_1                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDQP2_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT3_0                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDQP3_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT3_1                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDQP3_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DMPAT0_0                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDMP0_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DMPAT0_1                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDMP0_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DMPAT1_0                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDMP1_0_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DMPAT1_1                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDMP1_1_STRUCT;
typedef union {
  struct {
    UINT32 SERIAL_DQPAT0_LEN                       :  2;  // Bits 1:0
    UINT32 RSVD3_2                                 :  2;  // Bits 3:2
    UINT32 SERIAL_DQPAT1_LEN                       :  2;  // Bits 5:4
    UINT32 RSVD7_6                                 :  2;  // Bits 7:6
    UINT32 SERIAL_DQPAT2_LEN                       :  2;  // Bits 9:8
    UINT32 RSVD11_10                               :  2;  // Bits 11:10
    UINT32 SERIAL_DQPAT3_LEN                       :  2;  // Bits 13:12
    UINT32 RSVD15_14                               :  2;  // Bits 15:14
    UINT32 SERIAL_DMPAT0_LEN                       :  2;  // Bits 17:16
    UINT32 RSVD19_18                               :  2;  // Bits 19:18
    UINT32 SERIAL_DMPAT1_LEN                       :  2;  // Bits 21:20
    UINT32 RSVD31_22                               :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_SDPL_STRUCT;
typedef union {
  struct {
    UINT32 DQ_LFSR0_POLY                           :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 DQ_LFSR1_POLY                           :  3;  // Bits 6:4
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 DQ_LFSR2_POLY                           :  3;  // Bits 10:8
    UINT32 RSVD11                                  :  1;  // Bits 11:11
    UINT32 DQ_LFSR3_POLY                           :  3;  // Bits 14:12
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 DM_LFSR0_POLY                           :  3;  // Bits 18:16
    UINT32 RSVD19                                  :  1;  // Bits 19:19
    UINT32 DM_LFSR1_POLY                           :  3;  // Bits 22:20
    UINT32 RSVD31_23                               :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DLFSR_POLY_STRUCT;
typedef union {
  struct {
    UINT32 DQM_LFSRX_POLY                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DLFSRX_POLY_STRUCT;
typedef union {
  struct {
    UINT32 DQ_LFSR0_SEED                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQLFSR0_SEED_STRUCT;
typedef union {
  struct {
    UINT32 DQ_LFSR1_SEED                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQLFSR1_SEED_STRUCT;
typedef union {
  struct {
    UINT32 DQ_LFSR2_SEED                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQLFSR2_SEED_STRUCT;
typedef union {
  struct {
    UINT32 DQ_LFSR3_SEED                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQLFSR3_SEED_STRUCT;
typedef union {
  struct {
    UINT32 DM_LFSR0_SEED                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DMLFSR0_SEED_STRUCT;
typedef union {
  struct {
    UINT32 DM_LFSR1_SEED                           :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DMLFSR1_SEED_STRUCT;
typedef union {
  struct {
    UINT32 DQPAT_MODE                              :  4;  // Bits 3:0
    UINT32 DQPAT_RATE                              :  8;  // Bits 11:4
    UINT32 VICTIM0_DQPAT_SEL                       :  1;  // Bits 12:12
    UINT32 VICTIM1_DQPAT_SEL                       :  1;  // Bits 13:13
    UINT32 AGGRESSOR0_DQPAT_SEL                    :  1;  // Bits 14:14
    UINT32 AGGRESSOR1_DQPAT_SEL                    :  1;  // Bits 15:15
    UINT32 RST_DQPAT_ON_ROTATE                     :  1;  // Bits 16:16
    UINT32 RSVD18_17                               :  2;  // Bits 18:17
    UINT32 STATIC_DMPAT_EN                         :  1;  // Bits 19:19
    UINT32 SERIAL_CHAR_EN                          :  4;  // Bits 23:20
    UINT32 RSVD27_24                               :  4;  // Bits 27:24
    UINT32 DMPAT_AS_ECCPAT                         :  1;  // Bits 28:28
    UINT32 ECC_UI_OV                               :  2;  // Bits 30:29
    UINT32 RSVD31                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQC_STRUCT;
typedef union {
  struct {
    UINT32 DQ_DC_0                                 :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQDCC0_STRUCT;
typedef union {
  struct {
    UINT32 DQ_DC_1                                 :  8;  // Bits 7:0
    UINT32 RSVD31_8                                :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQDCC1_STRUCT;
typedef union {
  struct {
    UINT32 DQ_INV_0                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQINVC0_STRUCT;
typedef union {
  struct {
    UINT32 DQ_INV_1                                :  8;  // Bits 7:0
    UINT32 RSVD31_8                                :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DQINVC1_STRUCT;
typedef union {
  struct {
    UINT32 DM_DC                                   :  4;  // Bits 3:0
    UINT32 RSVD15_4                                :  12;  // Bits 15:4
    UINT32 DM_INV                                  :  4;  // Bits 19:16
    UINT32 RSVD31_20                               :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DMDCINVC_STRUCT;
typedef union {
  struct {
    UINT32 CHAN_SEL                                :  2;  // Bits 1:0
    UINT32 DATABUF_SEL                             :  2;  // Bits 3:2
    UINT32 DATABUF_INIT_WPTR_VAL                   :  5;  // Bits 8:4
    UINT32 RSVD9                                   :  1;  // Bits 9:9
    UINT32 DATABUF_INIT_RPTR_VAL                   :  5;  // Bits 14:10
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 DATABUF_CUR_WPTR_VAL                    :  5;  // Bits 20:16
    UINT32 RSVD21                                  :  1;  // Bits 21:21
    UINT32 DATABUF_CUR_RPTR_VAL                    :  5;  // Bits 26:22
    UINT32 RSVD31_27                               :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DBAC_STRUCT;
typedef union {
  struct {
    UINT32 DATABUF_ACC                             :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_DBA_STRUCT;
typedef union {
  struct {
    UINT32 LR_RANK_ADDR                            :  2;  // Bits 1:0
    UINT32 RSVD3_2                                 :  2;  // Bits 3:2
    UINT32 LR_BANK_ADDR                            :  5;  // Bits 8:4
    UINT32 RSVD10_9                                :  2;  // Bits 10:9
    UINT32 LR_COL_ADDR                             :  10;  // Bits 20:11
    UINT32 RSVD31_21                               :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_LRA0_STRUCT;
typedef union {
  struct {
    UINT32 LR_ROW_ADDR                             :  18;  // Bits 17:0
    UINT32 RSVD31_18                               :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_LRA1_STRUCT;
typedef union {
  struct {
    UINT32 ERR_CHK_EN                              :  4;  // Bits 3:0
    UINT32 ERR_CAPT_MODE                           :  4;  // Bits 7:4
    UINT32 CHK_DM_BYTE                             :  1;  // Bits 8:8
    UINT32 DBI_ERR_CHK_EN                          :  1;  // Bits 9:9
    UINT32 RSVD11_10                               :  2;  // Bits 11:10
    UINT32 CLR_ALL_ERRS                            :  4;  // Bits 15:12
    UINT32 CLR_WR_DATABUF                          :  1;  // Bits 16:16
    UINT32 RSVD19_17                               :  3;  // Bits 19:17
    UINT32 CLR_EXP_DATABUF                         :  4;  // Bits 23:20
    UINT32 CLR_LR_DATABUF                          :  4;  // Bits 27:24
    UINT32 CLR_LR_ADDRBUF                          :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_GEN_ERRCFG_STRUCT;
typedef union {
  struct {
    UINT32 GENERAL_ERR                             :  4;  // Bits 3:0
    UINT32 ALL_DQ_ERR                              :  4;  // Bits 7:4
    UINT32 STOPPED_ON_ERR                          :  4;  // Bits 11:8
    UINT32 PAUSED_ON_ERR                           :  4;  // Bits 15:12
    UINT32 CH0_RANK_ERR                            :  4;  // Bits 19:16
    UINT32 CH1_RANK_ERR                            :  4;  // Bits 23:20
    UINT32 CH2_RANK_ERR                            :  4;  // Bits 27:24
    UINT32 CH3_RANK_ERR                            :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_GEN_ERR_STRUCT;
typedef union {
  struct {
    UINT32 CH0_BYTE_LANE_ERR                       :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 CH1_BYTE_LANE_ERR                       :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 CH2_BYTE_LANE_ERR                       :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 CH3_BYTE_LANE_ERR                       :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_BYTE_LANE_ERR_STRUCT;
typedef union {
  struct {
    UINT32 MRD_TMO_DLY                             :  24;  // Bits 23:0
    UINT32 RSVD27_24                               :  4;  // Bits 27:24
    UINT32 MRD_ERR                                 :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_MRD_ERR_STRUCT;
typedef union {
  struct {
    UINT32 STOP_ON_ALL_DQ_ERR                      :  1;  // Bits 0:0
    UINT32 STOPAUSE_ON_ERR_EN                      :  1;  // Bits 1:1
    UINT32 STOPAUSE_ON_ERR                         :  1;  // Bits 2:2
    UINT32 STOPAUSE_ERR_CNTR_SEL                   :  1;  // Bits 3:3
    UINT32 STOPAUSE_ON_ALL_BEC                     :  2;  // Bits 5:4
    UINT32 RSVD7_6                                 :  2;  // Bits 7:6
    UINT32 IGNORE_N_READS                          :  8;  // Bits 15:8
    UINT32 STOPAUSE_THRES                          :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_ERRCFG_STRUCT;
typedef union {
  struct {
    UINT32 ERR_CNT                                 :  16;  // Bits 15:0
    UINT32 RSVD27_16                               :  12;  // Bits 27:16
    UINT32 ERR_CNT_TYPE                            :  3;  // Bits 30:28
    UINT32 ERR_CNT_EN                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_ERRCNT0_STRUCT;
typedef union {
  struct {
    UINT32 ERR_CNT                                 :  16;  // Bits 15:0
    UINT32 RSVD27_16                               :  12;  // Bits 27:16
    UINT32 ERR_CNT_TYPE                            :  3;  // Bits 30:28
    UINT32 ERR_CNT_EN                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_ERRCNT1_STRUCT;
typedef union {
  struct {
    UINT32 DATA_BYTE_ERR_MASK_0                    :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BEM0_STRUCT;
typedef union {
  struct {
    UINT32 DATA_BYTE_ERR_MASK_1                    :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BEM1_STRUCT;
typedef union {
  struct {
    UINT32 ECC_BYTE_ERR_MASK                       :  16;  // Bits 15:0
    UINT32 RSVD31_16                               :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BEM2_STRUCT;
typedef union {
  struct {
    UINT32 UI_ERR_MASK                             :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_UIEM_STRUCT;
typedef union {
  struct {
    UINT32 DQ_LANE_ERR_MASK_0                      :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_LEM0_STRUCT;
typedef union {
  struct {
    UINT32 DQ_LANE_ERR_MASK_1                      :  8;  // Bits 7:0
    UINT32 RSVD31_8                                :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_LEM1_STRUCT;
typedef union {
  struct {
    UINT32 CACHELINE_ERR_MASK                      :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_CLEM_STRUCT;
typedef union {
  struct {
    UINT32 DATA_BYTE_ERR_0                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BYTE_ERR0_STRUCT;
typedef union {
  struct {
    UINT32 DATA_BYTE_ERR_1                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BYTE_ERR1_STRUCT;
typedef union {
  struct {
    UINT32 ECC_BYTE_ERR                            :  16;  // Bits 15:0
    UINT32 RSVD31_16                               :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BYTE_ERR2_STRUCT;
typedef union {
  struct {
    UINT32 UI_ERR                                  :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_UI_ERR_STRUCT;
typedef union {
  struct {
    UINT32 EVEN_UI_DQ_LANE_ERR_0                   :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_EUILANE_ERR0_STRUCT;
typedef union {
  struct {
    UINT32 EVEN_UI_DQ_LANE_ERR_1                   :  8;  // Bits 7:0
    UINT32 RSVD31_8                                :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_EUILANE_ERR1_STRUCT;
typedef union {
  struct {
    UINT32 ODD_UI_DQ_LANE_ERR_0                    :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_OUILANE_ERR0_STRUCT;
typedef union {
  struct {
    UINT32 ODD_UI_DQ_LANE_ERR_1                    :  8;  // Bits 7:0
    UINT32 RSVD31_8                                :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_OUILANE_ERR1_STRUCT;
typedef union {
  struct {
    UINT32 BYTE_LANE0_ERR_CNT                      :  16;  // Bits 15:0
    UINT32 BYTE_LANE1_ERR_CNT                      :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BLEC0_STRUCT;
typedef union {
  struct {
    UINT32 BYTE_LANE2_ERR_CNT                      :  16;  // Bits 15:0
    UINT32 BYTE_LANE3_ERR_CNT                      :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BLEC1_STRUCT;
typedef union {
  struct {
    UINT32 BYTE_LANE4_ERR_CNT                      :  16;  // Bits 15:0
    UINT32 BYTE_LANE_ERR_CNTR_EN                   :  5;  // Bits 20:16
    UINT32 RSVD31_21                               :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_BLEC2_STRUCT;
typedef union {
  struct {
    UINT32 DQ0_ERR_CNT                             :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ1_ERR_CNT                             :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ2_ERR_CNT                             :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ3_ERR_CNT                             :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC0_STRUCT;
typedef union {
  struct {
    UINT32 DQ4_ERR_CNT                             :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ5_ERR_CNT                             :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ6_ERR_CNT                             :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ7_ERR_CNT                             :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC1_STRUCT;
typedef union {
  struct {
    UINT32 DQ8_ERR_CNT                             :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ9_ERR_CNT                             :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ10_ERR_CNT                            :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ11_ERR_CNT                            :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC2_STRUCT;
typedef union {
  struct {
    UINT32 DQ12_ERR_CNT                            :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ13_ERR_CNT                            :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ14_ERR_CNT                            :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ15_ERR_CNT                            :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC3_STRUCT;
typedef union {
  struct {
    UINT32 DQ16_ERR_CNT                            :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ17_ERR_CNT                            :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ18_ERR_CNT                            :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ19_ERR_CNT                            :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC4_STRUCT;
typedef union {
  struct {
    UINT32 DQ20_ERR_CNT                            :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ21_ERR_CNT                            :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ22_ERR_CNT                            :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ23_ERR_CNT                            :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC5_STRUCT;
typedef union {
  struct {
    UINT32 DQ24_ERR_CNT                            :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ25_ERR_CNT                            :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ26_ERR_CNT                            :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ27_ERR_CNT                            :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC6_STRUCT;
typedef union {
  struct {
    UINT32 DQ28_ERR_CNT                            :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ29_ERR_CNT                            :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ30_ERR_CNT                            :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ31_ERR_CNT                            :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC7_STRUCT;
typedef union {
  struct {
    UINT32 DQ32_ERR_CNT                            :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ33_ERR_CNT                            :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ34_ERR_CNT                            :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ35_ERR_CNT                            :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC8_STRUCT;
typedef union {
  struct {
    UINT32 DQ36_ERR_CNT                            :  5;  // Bits 4:0
    UINT32 RSVD7_5                                 :  3;  // Bits 7:5
    UINT32 DQ37_ERR_CNT                            :  5;  // Bits 12:8
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 DQ38_ERR_CNT                            :  5;  // Bits 20:16
    UINT32 RSVD23_21                               :  3;  // Bits 23:21
    UINT32 DQ39_ERR_CNT                            :  5;  // Bits 28:24
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MTG0_CR_MTG_CH0_DQEC9_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCFG_STRUCT MTG0_CR_MTG_CH1_ERRCFG_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT0_STRUCT MTG0_CR_MTG_CH1_ERRCNT0_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT1_STRUCT MTG0_CR_MTG_CH1_ERRCNT1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM0_STRUCT MTG0_CR_MTG_CH1_BEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM1_STRUCT MTG0_CR_MTG_CH1_BEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM2_STRUCT MTG0_CR_MTG_CH1_BEM2_STRUCT;

typedef MTG0_CR_MTG_CH0_UIEM_STRUCT MTG0_CR_MTG_CH1_UIEM_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM0_STRUCT MTG0_CR_MTG_CH1_LEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM1_STRUCT MTG0_CR_MTG_CH1_LEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_CLEM_STRUCT MTG0_CR_MTG_CH1_CLEM_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR0_STRUCT MTG0_CR_MTG_CH1_BYTE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR1_STRUCT MTG0_CR_MTG_CH1_BYTE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR2_STRUCT MTG0_CR_MTG_CH1_BYTE_ERR2_STRUCT;

typedef MTG0_CR_MTG_CH0_UI_ERR_STRUCT MTG0_CR_MTG_CH1_UI_ERR_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR0_STRUCT MTG0_CR_MTG_CH1_EUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR1_STRUCT MTG0_CR_MTG_CH1_EUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR0_STRUCT MTG0_CR_MTG_CH1_OUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR1_STRUCT MTG0_CR_MTG_CH1_OUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC0_STRUCT MTG0_CR_MTG_CH1_BLEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC1_STRUCT MTG0_CR_MTG_CH1_BLEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC2_STRUCT MTG0_CR_MTG_CH1_BLEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC0_STRUCT MTG0_CR_MTG_CH1_DQEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC1_STRUCT MTG0_CR_MTG_CH1_DQEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC2_STRUCT MTG0_CR_MTG_CH1_DQEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC3_STRUCT MTG0_CR_MTG_CH1_DQEC3_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC4_STRUCT MTG0_CR_MTG_CH1_DQEC4_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC5_STRUCT MTG0_CR_MTG_CH1_DQEC5_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC6_STRUCT MTG0_CR_MTG_CH1_DQEC6_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC7_STRUCT MTG0_CR_MTG_CH1_DQEC7_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC8_STRUCT MTG0_CR_MTG_CH1_DQEC8_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC9_STRUCT MTG0_CR_MTG_CH1_DQEC9_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCFG_STRUCT MTG0_CR_MTG_CH2_ERRCFG_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT0_STRUCT MTG0_CR_MTG_CH2_ERRCNT0_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT1_STRUCT MTG0_CR_MTG_CH2_ERRCNT1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM0_STRUCT MTG0_CR_MTG_CH2_BEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM1_STRUCT MTG0_CR_MTG_CH2_BEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM2_STRUCT MTG0_CR_MTG_CH2_BEM2_STRUCT;

typedef MTG0_CR_MTG_CH0_UIEM_STRUCT MTG0_CR_MTG_CH2_UIEM_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM0_STRUCT MTG0_CR_MTG_CH2_LEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM1_STRUCT MTG0_CR_MTG_CH2_LEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_CLEM_STRUCT MTG0_CR_MTG_CH2_CLEM_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR0_STRUCT MTG0_CR_MTG_CH2_BYTE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR1_STRUCT MTG0_CR_MTG_CH2_BYTE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR2_STRUCT MTG0_CR_MTG_CH2_BYTE_ERR2_STRUCT;

typedef MTG0_CR_MTG_CH0_UI_ERR_STRUCT MTG0_CR_MTG_CH2_UI_ERR_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR0_STRUCT MTG0_CR_MTG_CH2_EUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR1_STRUCT MTG0_CR_MTG_CH2_EUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR0_STRUCT MTG0_CR_MTG_CH2_OUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR1_STRUCT MTG0_CR_MTG_CH2_OUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC0_STRUCT MTG0_CR_MTG_CH2_BLEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC1_STRUCT MTG0_CR_MTG_CH2_BLEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC2_STRUCT MTG0_CR_MTG_CH2_BLEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC0_STRUCT MTG0_CR_MTG_CH2_DQEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC1_STRUCT MTG0_CR_MTG_CH2_DQEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC2_STRUCT MTG0_CR_MTG_CH2_DQEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC3_STRUCT MTG0_CR_MTG_CH2_DQEC3_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC4_STRUCT MTG0_CR_MTG_CH2_DQEC4_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC5_STRUCT MTG0_CR_MTG_CH2_DQEC5_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC6_STRUCT MTG0_CR_MTG_CH2_DQEC6_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC7_STRUCT MTG0_CR_MTG_CH2_DQEC7_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC8_STRUCT MTG0_CR_MTG_CH2_DQEC8_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC9_STRUCT MTG0_CR_MTG_CH2_DQEC9_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCFG_STRUCT MTG0_CR_MTG_CH3_ERRCFG_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT0_STRUCT MTG0_CR_MTG_CH3_ERRCNT0_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT1_STRUCT MTG0_CR_MTG_CH3_ERRCNT1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM0_STRUCT MTG0_CR_MTG_CH3_BEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM1_STRUCT MTG0_CR_MTG_CH3_BEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM2_STRUCT MTG0_CR_MTG_CH3_BEM2_STRUCT;

typedef MTG0_CR_MTG_CH0_UIEM_STRUCT MTG0_CR_MTG_CH3_UIEM_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM0_STRUCT MTG0_CR_MTG_CH3_LEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM1_STRUCT MTG0_CR_MTG_CH3_LEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_CLEM_STRUCT MTG0_CR_MTG_CH3_CLEM_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR0_STRUCT MTG0_CR_MTG_CH3_BYTE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR1_STRUCT MTG0_CR_MTG_CH3_BYTE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR2_STRUCT MTG0_CR_MTG_CH3_BYTE_ERR2_STRUCT;

typedef MTG0_CR_MTG_CH0_UI_ERR_STRUCT MTG0_CR_MTG_CH3_UI_ERR_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR0_STRUCT MTG0_CR_MTG_CH3_EUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR1_STRUCT MTG0_CR_MTG_CH3_EUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR0_STRUCT MTG0_CR_MTG_CH3_OUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR1_STRUCT MTG0_CR_MTG_CH3_OUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC0_STRUCT MTG0_CR_MTG_CH3_BLEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC1_STRUCT MTG0_CR_MTG_CH3_BLEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC2_STRUCT MTG0_CR_MTG_CH3_BLEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC0_STRUCT MTG0_CR_MTG_CH3_DQEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC1_STRUCT MTG0_CR_MTG_CH3_DQEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC2_STRUCT MTG0_CR_MTG_CH3_DQEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC3_STRUCT MTG0_CR_MTG_CH3_DQEC3_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC4_STRUCT MTG0_CR_MTG_CH3_DQEC4_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC5_STRUCT MTG0_CR_MTG_CH3_DQEC5_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC6_STRUCT MTG0_CR_MTG_CH3_DQEC6_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC7_STRUCT MTG0_CR_MTG_CH3_DQEC7_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC8_STRUCT MTG0_CR_MTG_CH3_DQEC8_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC9_STRUCT MTG0_CR_MTG_CH3_DQEC9_STRUCT;

typedef DUNIT0_CR_MEMCFG_STRUCT DUNIT1_CR_D_CR_MEMCFG_STRUCT;

typedef DUNIT0_CR_DTR0_STRUCT DUNIT1_CR_D_CR_DTR0_STRUCT;

typedef DUNIT0_CR_DTR1_STRUCT DUNIT1_CR_D_CR_DTR1_STRUCT;

typedef DUNIT0_CR_DTR2_STRUCT DUNIT1_CR_D_CR_DTR2_STRUCT;

typedef DUNIT0_CR_DTR3_STRUCT DUNIT1_CR_D_CR_DTR3_STRUCT;

typedef DUNIT0_CR_DTR4_STRUCT DUNIT1_CR_D_CR_DTR4_STRUCT;

typedef DUNIT0_CR_DTR5_STRUCT DUNIT1_CR_D_CR_DTR5_STRUCT;

typedef DUNIT0_CR_DTR6_STRUCT DUNIT1_CR_D_CR_DTR6_STRUCT;

typedef DUNIT0_CR_DTR7_STRUCT DUNIT1_CR_D_CR_DTR7_STRUCT;

typedef DUNIT0_CR_DTR8_STRUCT DUNIT1_CR_D_CR_DTR8_STRUCT;

typedef DUNIT0_CR_DTR9_STRUCT DUNIT1_CR_D_CR_DTR9_STRUCT;

typedef DUNIT0_CR_DTR10_STRUCT DUNIT1_CR_D_CR_DTR10_STRUCT;

typedef DUNIT0_CR_DTR11_STRUCT DUNIT1_CR_D_CR_DTR11_STRUCT;

typedef DUNIT0_CR_DTR12_STRUCT DUNIT1_CR_D_CR_DTR12_STRUCT;

typedef DUNIT0_CR_DTR13_STRUCT DUNIT1_CR_D_CR_DTR13_STRUCT;

typedef DUNIT0_CR_DTR14_STRUCT DUNIT1_CR_D_CR_DTR14_STRUCT;

typedef DUNIT0_CR_DTR15_STRUCT DUNIT1_CR_D_CR_DTR15_STRUCT;

typedef DUNIT0_CR_DTR16_STRUCT DUNIT1_CR_D_CR_DTR16_STRUCT;

typedef DUNIT0_CR_DTR17_STRUCT DUNIT1_CR_D_CR_DTR17_STRUCT;

typedef DUNIT0_CR_ODT1_STRUCT DUNIT1_CR_D_CR_ODT1_STRUCT;

typedef DUNIT0_CR_PD0_STRUCT DUNIT1_CR_D_CR_PD0_STRUCT;

typedef DUNIT0_CR_PD2_STRUCT DUNIT1_CR_D_CR_PD2_STRUCT;

typedef DUNIT0_CR_D_CR_SR0_STRUCT DUNIT1_CR_D_CR_SR0_STRUCT;

typedef DUNIT0_CR_D_CR_LPCTRL0_STRUCT DUNIT1_CR_D_CR_LPCTRL0_STRUCT;

typedef DUNIT0_CR_D_CR_LPCTRL1_STRUCT DUNIT1_CR_D_CR_LPCTRL1_STRUCT;

typedef DUNIT0_CR_D_CR_REF_STRUCT DUNIT1_CR_D_CR_REF_STRUCT;

typedef DUNIT0_CR_D_CR_ZQCAL_STRUCT DUNIT1_CR_D_CR_ZQCAL_STRUCT;

typedef DUNIT0_CR_D_CR_DQS2DQ_CAL_STRUCT DUNIT1_CR_D_CR_DQS2DQ_CAL_STRUCT;

typedef DUNIT0_CR_D_CR_WCK2DQO_CAL_STRUCT DUNIT1_CR_D_CR_WCK2DQO_CAL_STRUCT;

typedef DUNIT0_CR_D_CR_TRAIN_STRUCT DUNIT1_CR_D_CR_TRAIN_STRUCT;

typedef DUNIT0_CR_D_CR_TRAIN_FB_DATA0_STRUCT DUNIT1_CR_D_CR_TRAIN_FB_DATA0_STRUCT;

typedef DUNIT0_CR_D_CR_TRAIN_FB_DATA1_STRUCT DUNIT1_CR_D_CR_TRAIN_FB_DATA1_STRUCT;

typedef DUNIT0_CR_D_CR_MCTRL_STRUCT DUNIT1_CR_D_CR_MCTRL_STRUCT;

typedef DUNIT0_CR_D_CR_DRAM_CMDS_STRUCT DUNIT1_CR_D_CR_DRAM_CMDS_STRUCT;

typedef DUNIT0_CR_D_CR_DRAM_CMDS1_STRUCT DUNIT1_CR_D_CR_DRAM_CMDS1_STRUCT;

typedef DUNIT0_CR_D_CR_DRAM_CMD_STRUCT DUNIT1_CR_D_CR_DRAM_CMD_STRUCT;

typedef DUNIT0_CR_D_CR_PS_DRAM_CMD_STRUCT DUNIT1_CR_D_CR_PS_DRAM_CMD_STRUCT;

typedef DUNIT0_CR_D_CR_MRRD0_STRUCT DUNIT1_CR_D_CR_MRRD0_STRUCT;

typedef DUNIT0_CR_D_CR_MRRD1_STRUCT DUNIT1_CR_D_CR_MRRD1_STRUCT;

typedef DUNIT0_CR_D_CR_MRRD2_STRUCT DUNIT1_CR_D_CR_MRRD2_STRUCT;

typedef DUNIT0_CR_D_CR_DOC_STRUCT DUNIT1_CR_D_CR_DOC_STRUCT;

typedef DUNIT0_CR_D_CR_STATUS_STRUCT DUNIT1_CR_D_CR_STATUS_STRUCT;

typedef DUNIT0_CR_D_CR_BANK_STATUS0_STRUCT DUNIT1_CR_D_CR_BANK_STATUS0_STRUCT;

typedef DUNIT0_CR_D_CR_BANK_STATUS1_STRUCT DUNIT1_CR_D_CR_BANK_STATUS1_STRUCT;

typedef DUNIT0_CR_D_CR_BANK_STATUS2_STRUCT DUNIT1_CR_D_CR_BANK_STATUS2_STRUCT;

typedef DUNIT0_CR_D_CR_BANK_STATUS3_STRUCT DUNIT1_CR_D_CR_BANK_STATUS3_STRUCT;

typedef DUNIT0_CR_D_CR_MISC0_STRUCT DUNIT1_CR_D_CR_MISC0_STRUCT;

typedef DUNIT0_CR_D_CR_BONUS0_STRUCT DUNIT1_CR_D_CR_BONUS0_STRUCT;

typedef DUNIT0_CR_D_CR_BONUS1_STRUCT DUNIT1_CR_D_CR_BONUS1_STRUCT;

typedef DUNIT0_CR_D_CR_DGB_STRUCT DUNIT1_CR_D_CR_DGB_STRUCT;

typedef DUNIT0_CR_CATG_CTRL0_STRUCT DUNIT1_CR_CATG_CTRL0_STRUCT;

typedef DUNIT0_CR_CATG_CTRL1_STRUCT DUNIT1_CR_CATG_CTRL1_STRUCT;

typedef DUNIT0_CR_CATG_CTRL2_STRUCT DUNIT1_CR_CATG_CTRL2_STRUCT;

typedef DUNIT0_CR_CATG_CAPPATSEL_STRUCT DUNIT1_CR_CATG_CAPPATSEL_STRUCT;

typedef DUNIT0_CR_CATG_CAPATSEL_STRUCT DUNIT1_CR_CATG_CAPATSEL_STRUCT;

typedef DUNIT0_CR_CATG_CSPATSEL_STRUCT DUNIT1_CR_CATG_CSPATSEL_STRUCT;

typedef DUNIT0_CR_CATG_SSP0_0_STRUCT DUNIT1_CR_CATG_SSP0_0_STRUCT;

typedef DUNIT0_CR_CATG_SSP0_1_STRUCT DUNIT1_CR_CATG_SSP0_1_STRUCT;

typedef DUNIT0_CR_CATG_SSP1_0_STRUCT DUNIT1_CR_CATG_SSP1_0_STRUCT;

typedef DUNIT0_CR_CATG_SSP1_1_STRUCT DUNIT1_CR_CATG_SSP1_1_STRUCT;

typedef DUNIT0_CR_CATG_SSP2_0_STRUCT DUNIT1_CR_CATG_SSP2_0_STRUCT;

typedef DUNIT0_CR_CATG_SSP2_1_STRUCT DUNIT1_CR_CATG_SSP2_1_STRUCT;

typedef DUNIT0_CR_CATG_SSP3_0_STRUCT DUNIT1_CR_CATG_SSP3_0_STRUCT;

typedef DUNIT0_CR_CATG_SSP3_1_STRUCT DUNIT1_CR_CATG_SSP3_1_STRUCT;

typedef DUNIT0_CR_CATG_SSPL_STRUCT DUNIT1_CR_CATG_SSPL_STRUCT;

typedef DUNIT0_CR_CATG_LFSR_POLY_STRUCT DUNIT1_CR_CATG_LFSR_POLY_STRUCT;

typedef DUNIT0_CR_CATG_LFSR0_SEED_STRUCT DUNIT1_CR_CATG_LFSR0_SEED_STRUCT;

typedef DUNIT0_CR_CATG_LFSR1_SEED_STRUCT DUNIT1_CR_CATG_LFSR1_SEED_STRUCT;

typedef DUNIT0_CR_CATG_CA_DCINVC_STRUCT DUNIT1_CR_CATG_CA_DCINVC_STRUCT;

typedef DUNIT0_CR_CATG_CS_DCINVC_STRUCT DUNIT1_CR_CATG_CS_DCINVC_STRUCT;

typedef MTG0_CR_MTG_MAIN_CTRL_STRUCT MTG1_CR_MTG_MAIN_CTRL_STRUCT;

typedef MTG0_CR_MTG_CE0_STRUCT MTG1_CR_MTG_CE0_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE1_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE2_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE3_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE4_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE5_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE6_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE7_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE8_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE9_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE10_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE11_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE12_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE13_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE14_STRUCT;

typedef MTG0_CR_MTG_CE1_STRUCT MTG1_CR_MTG_CE15_STRUCT;

typedef MTG0_CR_MTG_CDLY_STRUCT MTG1_CR_MTG_CDLY_STRUCT;

typedef MTG0_CR_MTG_CLC_STRUCT MTG1_CR_MTG_CLC_STRUCT;

typedef MTG0_CR_MTG_CRANDC_STRUCT MTG1_CR_MTG_CRANDC_STRUCT;

typedef MTG0_CR_MTG_MWRC_STRUCT MTG1_CR_MTG_MWRC_STRUCT;

typedef MTG0_CR_MTG_MWRO_STRUCT MTG1_CR_MTG_MWRO_STRUCT;

typedef MTG0_CR_MTG_ARMIN0_STRUCT MTG1_CR_MTG_ARMIN0_STRUCT;

typedef MTG0_CR_MTG_ARMIN1_STRUCT MTG1_CR_MTG_ARMIN1_STRUCT;

typedef MTG0_CR_MTG_ARMAX0_STRUCT MTG1_CR_MTG_ARMAX0_STRUCT;

typedef MTG0_CR_MTG_ARMAX1_STRUCT MTG1_CR_MTG_ARMAX1_STRUCT;

typedef MTG0_CR_MTG_L2PRM_STRUCT MTG1_CR_MTG_L2PRM_STRUCT;

typedef MTG0_CR_MTG_AO_STRUCT MTG1_CR_MTG_AO_STRUCT;

typedef MTG0_CR_MTG_ARC_STRUCT MTG1_CR_MTG_ARC_STRUCT;

typedef MTG0_CR_MTG_ARS0_STRUCT MTG1_CR_MTG_ARS0_STRUCT;

typedef MTG0_CR_MTG_ARS1_STRUCT MTG1_CR_MTG_ARS1_STRUCT;

typedef MTG0_CR_MTG_RADC_STRUCT MTG1_CR_MTG_RADC_STRUCT;

typedef MTG0_CR_MTG_DQPATSEL0_STRUCT MTG1_CR_MTG_DQPATSEL0_STRUCT;

typedef MTG0_CR_MTG_DQPATSEL1_STRUCT MTG1_CR_MTG_DQPATSEL1_STRUCT;

typedef MTG0_CR_MTG_UPDQBM_STRUCT MTG1_CR_MTG_UPDQBM_STRUCT;

typedef MTG0_CR_MTG_DMPATSEL_STRUCT MTG1_CR_MTG_DMPATSEL_STRUCT;

typedef MTG0_CR_MTG_SDQP0_0_STRUCT MTG1_CR_MTG_SDQP0_0_STRUCT;

typedef MTG0_CR_MTG_SDQP0_1_STRUCT MTG1_CR_MTG_SDQP0_1_STRUCT;

typedef MTG0_CR_MTG_SDQP1_0_STRUCT MTG1_CR_MTG_SDQP1_0_STRUCT;

typedef MTG0_CR_MTG_SDQP1_1_STRUCT MTG1_CR_MTG_SDQP1_1_STRUCT;

typedef MTG0_CR_MTG_SDQP2_0_STRUCT MTG1_CR_MTG_SDQP2_0_STRUCT;

typedef MTG0_CR_MTG_SDQP2_1_STRUCT MTG1_CR_MTG_SDQP2_1_STRUCT;

typedef MTG0_CR_MTG_SDQP3_0_STRUCT MTG1_CR_MTG_SDQP3_0_STRUCT;

typedef MTG0_CR_MTG_SDQP3_1_STRUCT MTG1_CR_MTG_SDQP3_1_STRUCT;

typedef MTG0_CR_MTG_SDMP0_0_STRUCT MTG1_CR_MTG_SDMP0_0_STRUCT;

typedef MTG0_CR_MTG_SDMP0_1_STRUCT MTG1_CR_MTG_SDMP0_1_STRUCT;

typedef MTG0_CR_MTG_SDMP1_0_STRUCT MTG1_CR_MTG_SDMP1_0_STRUCT;

typedef MTG0_CR_MTG_SDMP1_1_STRUCT MTG1_CR_MTG_SDMP1_1_STRUCT;

typedef MTG0_CR_MTG_SDPL_STRUCT MTG1_CR_MTG_SDPL_STRUCT;

typedef MTG0_CR_MTG_DLFSR_POLY_STRUCT MTG1_CR_MTG_DLFSR_POLY_STRUCT;

typedef MTG0_CR_MTG_DLFSRX_POLY_STRUCT MTG1_CR_MTG_DLFSRX_POLY_STRUCT;

typedef MTG0_CR_MTG_DQLFSR0_SEED_STRUCT MTG1_CR_MTG_DQLFSR0_SEED_STRUCT;

typedef MTG0_CR_MTG_DQLFSR1_SEED_STRUCT MTG1_CR_MTG_DQLFSR1_SEED_STRUCT;

typedef MTG0_CR_MTG_DQLFSR2_SEED_STRUCT MTG1_CR_MTG_DQLFSR2_SEED_STRUCT;

typedef MTG0_CR_MTG_DQLFSR3_SEED_STRUCT MTG1_CR_MTG_DQLFSR3_SEED_STRUCT;

typedef MTG0_CR_MTG_DMLFSR0_SEED_STRUCT MTG1_CR_MTG_DMLFSR0_SEED_STRUCT;

typedef MTG0_CR_MTG_DMLFSR1_SEED_STRUCT MTG1_CR_MTG_DMLFSR1_SEED_STRUCT;

typedef MTG0_CR_MTG_DQC_STRUCT MTG1_CR_MTG_DQC_STRUCT;

typedef MTG0_CR_MTG_DQDCC0_STRUCT MTG1_CR_MTG_DQDCC0_STRUCT;

typedef MTG0_CR_MTG_DQDCC1_STRUCT MTG1_CR_MTG_DQDCC1_STRUCT;

typedef MTG0_CR_MTG_DQINVC0_STRUCT MTG1_CR_MTG_DQINVC0_STRUCT;

typedef MTG0_CR_MTG_DQINVC1_STRUCT MTG1_CR_MTG_DQINVC1_STRUCT;

typedef MTG0_CR_MTG_DMDCINVC_STRUCT MTG1_CR_MTG_DMDCINVC_STRUCT;

typedef MTG0_CR_MTG_DBAC_STRUCT MTG1_CR_MTG_DBAC_STRUCT;

typedef MTG0_CR_MTG_DBA_STRUCT MTG1_CR_MTG_DBA_STRUCT;

typedef MTG0_CR_MTG_LRA0_STRUCT MTG1_CR_MTG_LRA0_STRUCT;

typedef MTG0_CR_MTG_LRA1_STRUCT MTG1_CR_MTG_LRA1_STRUCT;

typedef MTG0_CR_MTG_GEN_ERRCFG_STRUCT MTG1_CR_MTG_GEN_ERRCFG_STRUCT;

typedef MTG0_CR_MTG_GEN_ERR_STRUCT MTG1_CR_MTG_GEN_ERR_STRUCT;

typedef MTG0_CR_MTG_BYTE_LANE_ERR_STRUCT MTG1_CR_MTG_BYTE_LANE_ERR_STRUCT;

typedef MTG0_CR_MTG_MRD_ERR_STRUCT MTG1_CR_MTG_MRD_ERR_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCFG_STRUCT MTG1_CR_MTG_CH0_ERRCFG_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT0_STRUCT MTG1_CR_MTG_CH0_ERRCNT0_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT1_STRUCT MTG1_CR_MTG_CH0_ERRCNT1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM0_STRUCT MTG1_CR_MTG_CH0_BEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM1_STRUCT MTG1_CR_MTG_CH0_BEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM2_STRUCT MTG1_CR_MTG_CH0_BEM2_STRUCT;

typedef MTG0_CR_MTG_CH0_UIEM_STRUCT MTG1_CR_MTG_CH0_UIEM_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM0_STRUCT MTG1_CR_MTG_CH0_LEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM1_STRUCT MTG1_CR_MTG_CH0_LEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_CLEM_STRUCT MTG1_CR_MTG_CH0_CLEM_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR0_STRUCT MTG1_CR_MTG_CH0_BYTE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR1_STRUCT MTG1_CR_MTG_CH0_BYTE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR2_STRUCT MTG1_CR_MTG_CH0_BYTE_ERR2_STRUCT;

typedef MTG0_CR_MTG_CH0_UI_ERR_STRUCT MTG1_CR_MTG_CH0_UI_ERR_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR0_STRUCT MTG1_CR_MTG_CH0_EUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR1_STRUCT MTG1_CR_MTG_CH0_EUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR0_STRUCT MTG1_CR_MTG_CH0_OUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR1_STRUCT MTG1_CR_MTG_CH0_OUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC0_STRUCT MTG1_CR_MTG_CH0_BLEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC1_STRUCT MTG1_CR_MTG_CH0_BLEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC2_STRUCT MTG1_CR_MTG_CH0_BLEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC0_STRUCT MTG1_CR_MTG_CH0_DQEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC1_STRUCT MTG1_CR_MTG_CH0_DQEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC2_STRUCT MTG1_CR_MTG_CH0_DQEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC3_STRUCT MTG1_CR_MTG_CH0_DQEC3_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC4_STRUCT MTG1_CR_MTG_CH0_DQEC4_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC5_STRUCT MTG1_CR_MTG_CH0_DQEC5_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC6_STRUCT MTG1_CR_MTG_CH0_DQEC6_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC7_STRUCT MTG1_CR_MTG_CH0_DQEC7_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC8_STRUCT MTG1_CR_MTG_CH0_DQEC8_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC9_STRUCT MTG1_CR_MTG_CH0_DQEC9_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCFG_STRUCT MTG1_CR_MTG_CH1_ERRCFG_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT0_STRUCT MTG1_CR_MTG_CH1_ERRCNT0_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT1_STRUCT MTG1_CR_MTG_CH1_ERRCNT1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM0_STRUCT MTG1_CR_MTG_CH1_BEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM1_STRUCT MTG1_CR_MTG_CH1_BEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM2_STRUCT MTG1_CR_MTG_CH1_BEM2_STRUCT;

typedef MTG0_CR_MTG_CH0_UIEM_STRUCT MTG1_CR_MTG_CH1_UIEM_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM0_STRUCT MTG1_CR_MTG_CH1_LEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM1_STRUCT MTG1_CR_MTG_CH1_LEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_CLEM_STRUCT MTG1_CR_MTG_CH1_CLEM_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR0_STRUCT MTG1_CR_MTG_CH1_BYTE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR1_STRUCT MTG1_CR_MTG_CH1_BYTE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR2_STRUCT MTG1_CR_MTG_CH1_BYTE_ERR2_STRUCT;

typedef MTG0_CR_MTG_CH0_UI_ERR_STRUCT MTG1_CR_MTG_CH1_UI_ERR_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR0_STRUCT MTG1_CR_MTG_CH1_EUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR1_STRUCT MTG1_CR_MTG_CH1_EUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR0_STRUCT MTG1_CR_MTG_CH1_OUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR1_STRUCT MTG1_CR_MTG_CH1_OUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC0_STRUCT MTG1_CR_MTG_CH1_BLEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC1_STRUCT MTG1_CR_MTG_CH1_BLEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC2_STRUCT MTG1_CR_MTG_CH1_BLEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC0_STRUCT MTG1_CR_MTG_CH1_DQEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC1_STRUCT MTG1_CR_MTG_CH1_DQEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC2_STRUCT MTG1_CR_MTG_CH1_DQEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC3_STRUCT MTG1_CR_MTG_CH1_DQEC3_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC4_STRUCT MTG1_CR_MTG_CH1_DQEC4_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC5_STRUCT MTG1_CR_MTG_CH1_DQEC5_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC6_STRUCT MTG1_CR_MTG_CH1_DQEC6_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC7_STRUCT MTG1_CR_MTG_CH1_DQEC7_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC8_STRUCT MTG1_CR_MTG_CH1_DQEC8_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC9_STRUCT MTG1_CR_MTG_CH1_DQEC9_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCFG_STRUCT MTG1_CR_MTG_CH2_ERRCFG_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT0_STRUCT MTG1_CR_MTG_CH2_ERRCNT0_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT1_STRUCT MTG1_CR_MTG_CH2_ERRCNT1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM0_STRUCT MTG1_CR_MTG_CH2_BEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM1_STRUCT MTG1_CR_MTG_CH2_BEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM2_STRUCT MTG1_CR_MTG_CH2_BEM2_STRUCT;

typedef MTG0_CR_MTG_CH0_UIEM_STRUCT MTG1_CR_MTG_CH2_UIEM_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM0_STRUCT MTG1_CR_MTG_CH2_LEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM1_STRUCT MTG1_CR_MTG_CH2_LEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_CLEM_STRUCT MTG1_CR_MTG_CH2_CLEM_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR0_STRUCT MTG1_CR_MTG_CH2_BYTE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR1_STRUCT MTG1_CR_MTG_CH2_BYTE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR2_STRUCT MTG1_CR_MTG_CH2_BYTE_ERR2_STRUCT;

typedef MTG0_CR_MTG_CH0_UI_ERR_STRUCT MTG1_CR_MTG_CH2_UI_ERR_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR0_STRUCT MTG1_CR_MTG_CH2_EUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR1_STRUCT MTG1_CR_MTG_CH2_EUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR0_STRUCT MTG1_CR_MTG_CH2_OUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR1_STRUCT MTG1_CR_MTG_CH2_OUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC0_STRUCT MTG1_CR_MTG_CH2_BLEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC1_STRUCT MTG1_CR_MTG_CH2_BLEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC2_STRUCT MTG1_CR_MTG_CH2_BLEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC0_STRUCT MTG1_CR_MTG_CH2_DQEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC1_STRUCT MTG1_CR_MTG_CH2_DQEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC2_STRUCT MTG1_CR_MTG_CH2_DQEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC3_STRUCT MTG1_CR_MTG_CH2_DQEC3_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC4_STRUCT MTG1_CR_MTG_CH2_DQEC4_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC5_STRUCT MTG1_CR_MTG_CH2_DQEC5_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC6_STRUCT MTG1_CR_MTG_CH2_DQEC6_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC7_STRUCT MTG1_CR_MTG_CH2_DQEC7_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC8_STRUCT MTG1_CR_MTG_CH2_DQEC8_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC9_STRUCT MTG1_CR_MTG_CH2_DQEC9_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCFG_STRUCT MTG1_CR_MTG_CH3_ERRCFG_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT0_STRUCT MTG1_CR_MTG_CH3_ERRCNT0_STRUCT;

typedef MTG0_CR_MTG_CH0_ERRCNT1_STRUCT MTG1_CR_MTG_CH3_ERRCNT1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM0_STRUCT MTG1_CR_MTG_CH3_BEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM1_STRUCT MTG1_CR_MTG_CH3_BEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_BEM2_STRUCT MTG1_CR_MTG_CH3_BEM2_STRUCT;

typedef MTG0_CR_MTG_CH0_UIEM_STRUCT MTG1_CR_MTG_CH3_UIEM_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM0_STRUCT MTG1_CR_MTG_CH3_LEM0_STRUCT;

typedef MTG0_CR_MTG_CH0_LEM1_STRUCT MTG1_CR_MTG_CH3_LEM1_STRUCT;

typedef MTG0_CR_MTG_CH0_CLEM_STRUCT MTG1_CR_MTG_CH3_CLEM_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR0_STRUCT MTG1_CR_MTG_CH3_BYTE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR1_STRUCT MTG1_CR_MTG_CH3_BYTE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BYTE_ERR2_STRUCT MTG1_CR_MTG_CH3_BYTE_ERR2_STRUCT;

typedef MTG0_CR_MTG_CH0_UI_ERR_STRUCT MTG1_CR_MTG_CH3_UI_ERR_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR0_STRUCT MTG1_CR_MTG_CH3_EUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_EUILANE_ERR1_STRUCT MTG1_CR_MTG_CH3_EUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR0_STRUCT MTG1_CR_MTG_CH3_OUILANE_ERR0_STRUCT;

typedef MTG0_CR_MTG_CH0_OUILANE_ERR1_STRUCT MTG1_CR_MTG_CH3_OUILANE_ERR1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC0_STRUCT MTG1_CR_MTG_CH3_BLEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC1_STRUCT MTG1_CR_MTG_CH3_BLEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_BLEC2_STRUCT MTG1_CR_MTG_CH3_BLEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC0_STRUCT MTG1_CR_MTG_CH3_DQEC0_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC1_STRUCT MTG1_CR_MTG_CH3_DQEC1_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC2_STRUCT MTG1_CR_MTG_CH3_DQEC2_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC3_STRUCT MTG1_CR_MTG_CH3_DQEC3_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC4_STRUCT MTG1_CR_MTG_CH3_DQEC4_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC5_STRUCT MTG1_CR_MTG_CH3_DQEC5_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC6_STRUCT MTG1_CR_MTG_CH3_DQEC6_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC7_STRUCT MTG1_CR_MTG_CH3_DQEC7_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC8_STRUCT MTG1_CR_MTG_CH3_DQEC8_STRUCT;

typedef MTG0_CR_MTG_CH0_DQEC9_STRUCT MTG1_CR_MTG_CH3_DQEC9_STRUCT;
typedef union {
  struct {
    UINT32 GLOBAL_MPTU_START                       :  1;  // Bits 0:0
    UINT32 GLOBAL_MPTU_START_DIS                   :  1;  // Bits 1:1
    UINT32 RSVD15_2                                :  14;  // Bits 15:2
    UINT32 MPTU0_START_DLY                         :  8;  // Bits 23:16
    UINT32 MPTU1_START_DLY                         :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MPTUSS_CR_MSS_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 CHAN_WIDTH                              :  2;  // Bits 1:0
    UINT32 RSVD4_2                                 :  3;  // Bits 4:2
    UINT32 DFI_WCK_CTRL                            :  1;  // Bits 5:5
    UINT32 DFI_DRAM_CLK_DIS_CTRL                   :  1;  // Bits 6:6
    UINT32 DFI_INIT_START_SEL                      :  1;  // Bits 7:7
    UINT32 CHAN_EN                                 :  8;  // Bits 15:8
    UINT32 DFI_PHYUPD_EN                           :  8;  // Bits 23:16
    UINT32 DFI_LP_CTRL_EN                          :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MPTUSS_CR_MSS_CTRL1_STRUCT;
typedef union {
  struct {
    UINT32 RSVD15_0                                :  16;  // Bits 15:0
    UINT32 TX_CH0_MPTU_MAP                         :  2;  // Bits 17:16
    UINT32 TX_CH1_MPTU_MAP                         :  2;  // Bits 19:18
    UINT32 TX_CH2_MPTU_MAP                         :  2;  // Bits 21:20
    UINT32 TX_CH3_MPTU_MAP                         :  2;  // Bits 23:22
    UINT32 TX_CH4_MPTU_MAP                         :  2;  // Bits 25:24
    UINT32 TX_CH5_MPTU_MAP                         :  2;  // Bits 27:26
    UINT32 TX_CH6_MPTU_MAP                         :  2;  // Bits 29:28
    UINT32 TX_CH7_MPTU_MAP                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MPTUSS_CR_MSS_CTRL2_STRUCT;
typedef union {
  struct {
    UINT32 MPTU0_CH0_RX_EN                         :  1;  // Bits 0:0
    UINT32 MPTU0_CH0_RX_MAP                        :  3;  // Bits 3:1
    UINT32 MPTU0_CH1_RX_EN                         :  1;  // Bits 4:4
    UINT32 MPTU0_CH1_RX_MAP                        :  3;  // Bits 7:5
    UINT32 MPTU0_CH2_RX_EN                         :  1;  // Bits 8:8
    UINT32 MPTU0_CH2_RX_MAP                        :  3;  // Bits 11:9
    UINT32 MPTU0_CH3_RX_EN                         :  1;  // Bits 12:12
    UINT32 MPTU0_CH3_RX_MAP                        :  3;  // Bits 15:13
    UINT32 MPTU1_CH0_RX_EN                         :  1;  // Bits 16:16
    UINT32 MPTU1_CH0_RX_MAP                        :  3;  // Bits 19:17
    UINT32 MPTU1_CH1_RX_EN                         :  1;  // Bits 20:20
    UINT32 MPTU1_CH1_RX_MAP                        :  3;  // Bits 23:21
    UINT32 MPTU1_CH2_RX_EN                         :  1;  // Bits 24:24
    UINT32 MPTU1_CH2_RX_MAP                        :  3;  // Bits 27:25
    UINT32 MPTU1_CH3_RX_EN                         :  1;  // Bits 28:28
    UINT32 MPTU1_CH3_RX_MAP                        :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MPTUSS_CR_MSS_CTRL3_STRUCT;



#pragma pack(pop)
#endif /* __MrcRegisterStructPtlBxxx_h__ */







