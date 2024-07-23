#ifndef __MrcRegisterStructPtlDxxx_h__
#define __MrcRegisterStructPtlDxxx_h__
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
    UINT32 IBECC_EN                                :  1;  // Bits 0:0
    UINT32 RSVD                                    :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_ACTIVATE_STRUCT;
typedef union {
  struct {
    UINT32 IBECC_IDLE                              :  1;  // Bits 0:0
    UINT32 RSVD                                    :  5;  // Bits 5:1
    UINT32 RPQ_FULL                                :  1;  // Bits 6:6
    UINT32 RPQ_EMPTY                               :  1;  // Bits 7:7
    UINT32 IPQ_FULL                                :  1;  // Bits 8:8
    UINT32 IPQ_EMPTY                               :  1;  // Bits 9:9
    UINT32 WPQ_FULL                                :  1;  // Bits 10:10
    UINT32 WPQ_EMPTY                               :  1;  // Bits 11:11
    UINT32 ECC_READ_TRACKER_FULL                   :  1;  // Bits 12:12
    UINT32 ECC_READ_TRACKER_EMPTY                  :  1;  // Bits 13:13
    UINT32 PROTECTED_WRITE_EXISTS                  :  1;  // Bits 14:14
    UINT32 VC0_PROTECTED_READ_EXISTS               :  1;  // Bits 15:15
    UINT32 VC1_PROTECTED_READ_EXISTS               :  1;  // Bits 16:16
    UINT32 WAR_HAZARD_EXISTS                       :  1;  // Bits 17:17
    UINT32 RAW_HAZARD_EXISTS                       :  1;  // Bits 18:18
    UINT32 WAW_HAZARD_EXISTS                       :  1;  // Bits 19:19
    UINT32 CFI_PORT0_RX_STATE                      :  2;  // Bits 21:20
    UINT32 CFI_PORT0_TX_STATE                      :  2;  // Bits 23:22
    UINT32 CFI_PORT1_RX_STATE                      :  2;  // Bits 25:24
    UINT32 CFI_PORT1_TX_STATE                      :  2;  // Bits 27:26
    UINT32 RSVD2                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 CGCTRL_CLKGATEDEF                       :  1;  // Bits 0:0
    UINT32 RSVD0                                   :  7;  // Bits 7:1
    UINT32 CGCTRL_CLKGATEN                         :  1;  // Bits 8:8
    UINT32 RSVD1                                   :  7;  // Bits 15:9
    UINT32 CGCTRL_IDLECNT                          :  8;  // Bits 23:16
    UINT32 RSVD2                                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_IOSF_SB_EP_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 BASE                                    :  14;  // Bits 13:0
    UINT32 RSVD0                                   :  1;  // Bits 14:14
    UINT32 RSVD1                                   :  1;  // Bits 15:15
    UINT32 MASK                                    :  14;  // Bits 29:16
    UINT32 RANGE_INIT_STATUS                       :  1;  // Bits 30:30
    UINT32 RANGE_EN                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT;

typedef MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT MC0_IBECC_PROTECT_ADDR_RANGE_1_STRUCT;

typedef MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT MC0_IBECC_PROTECT_ADDR_RANGE_2_STRUCT;

typedef MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT MC0_IBECC_PROTECT_ADDR_RANGE_3_STRUCT;

typedef MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT MC0_IBECC_PROTECT_ADDR_RANGE_4_STRUCT;

typedef MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT MC0_IBECC_PROTECT_ADDR_RANGE_5_STRUCT;

typedef MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT MC0_IBECC_PROTECT_ADDR_RANGE_6_STRUCT;

typedef MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT MC0_IBECC_PROTECT_ADDR_RANGE_7_STRUCT;
typedef union {
  struct {
    UINT32 SAI_MASKLow                             :  32;  // Bits 31:0
    UINT32 SAI_MASKHigh                            :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_DEFAULT_PG_CP_STRUCT;
typedef union {
  struct {
    UINT32 SAI_MASKLow                             :  32;  // Bits 31:0
    UINT32 SAI_MASKHigh                            :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_DEFAULT_PG_RAC_STRUCT;
typedef union {
  struct {
    UINT32 SAI_MASKLow                             :  32;  // Bits 31:0
    UINT32 SAI_MASKHigh                            :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_DEFAULT_PG_WAC_STRUCT;

typedef MC0_IBECC_DEFAULT_PG_CP_STRUCT MC0_IBECC_DEBUG_PG_CP_STRUCT;

typedef MC0_IBECC_DEFAULT_PG_RAC_STRUCT MC0_IBECC_DEBUG_PG_RAC_STRUCT;
typedef union {
  struct {
    UINT32 SAI_MASKLow                             :  32;  // Bits 31:0
    UINT32 SAI_MASKHigh                            :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_DEBUG_PG_WAC_STRUCT;

typedef MC0_IBECC_DEFAULT_PG_CP_STRUCT MC0_IBECC_OS_PG_CP_STRUCT;

typedef MC0_IBECC_DEFAULT_PG_RAC_STRUCT MC0_IBECC_OS_PG_RAC_STRUCT;
typedef union {
  struct {
    UINT32 SAI_MASKLow                             :  32;  // Bits 31:0
    UINT32 SAI_MASKHigh                            :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_OS_PG_WAC_STRUCT;
typedef union {
  struct {
    UINT32 CLK_GATE_EN                             :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_CLK_GATE_EN_0_STRUCT;
typedef union {
  struct {
    UINT32 RESERVED0                               :  16;  // Bits 15:0
    UINT32 REQ_WR_MAX_CREDIT                       :  8;  // Bits 23:16
    UINT32 REQ_RD_MAX_CREDIT                       :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_STRUCT;
typedef union {
  struct {
    UINT32 RESERVED0                               :  16;  // Bits 15:0
    UINT32 REQ_WR_MAX_CREDIT                       :  8;  // Bits 23:16
    UINT32 REQ_RD_MAX_CREDIT                       :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_STRUCT;
typedef union {
  struct {
    UINT32 REQ_WR_SHARED_CREDIT                    :  8;  // Bits 7:0
    UINT32 RESERVED1                               :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 P0_RX_REQ_BLOCK                         :  1;  // Bits 0:0
    UINT32 P0_RX_DATA_BLOCK                        :  1;  // Bits 1:1
    UINT32 P0_TX_RSP_CRD_BLOCK                     :  1;  // Bits 2:2
    UINT32 P0_TX_DATA_CRD_BLOCK                    :  1;  // Bits 3:3
    UINT32 P1_RX_RSP_BLOCK                         :  1;  // Bits 4:4
    UINT32 P1_RX_DATA_BLOCK                        :  1;  // Bits 5:5
    UINT32 P1_TX_REQ_CRD_BLOCK                     :  1;  // Bits 6:6
    UINT32 P1_TX_DATA_CRD_BLOCK                    :  1;  // Bits 7:7
    UINT32 RSVD                                    :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_CFI_BLOCK_STRUCT;
typedef union {
  struct {
    UINT32 PASS                                    :  1;  // Bits 0:0
    UINT32 COMPLETE                                :  1;  // Bits 1:1
    UINT32 RSVD1                                   :  6;  // Bits 7:2
    UINT32 MBIST_FAIL_INJECT                       :  1;  // Bits 8:8
    UINT32 RSVD2                                   :  22;  // Bits 30:9
    UINT32 RUN_BUSY                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_MBIST_POST_CTRL_WDB_STRUCT;

typedef MC0_IBECC_MBIST_POST_CTRL_WDB_STRUCT MC0_IBECC_MBIST_POST_CTRL_RDB_STRUCT;
typedef union {
  struct {
    UINT32 RADDR_PARITY_CHK_EN                     :  1;  // Bits 0:0
    UINT32 RDATA_PARITY_CHK_EN                     :  1;  // Bits 1:1
    UINT32 RHEADER_PARITY_CHK_EN                   :  1;  // Bits 2:2
    UINT32 WHEADER_PARITY_CHK_EN                   :  1;  // Bits 3:3
    UINT32 WDATA_PARITY_CHK_EN                     :  1;  // Bits 4:4
    UINT32 RSVD0                                   :  3;  // Bits 7:5
    UINT32 DIS_PARITY_PCIE_ERROR                   :  1;  // Bits 8:8
    UINT32 DIS_PARITY_LOG                          :  1;  // Bits 9:9
    UINT32 RSVD1                                   :  21;  // Bits 30:10
    UINT32 PARITY_CHK_EN                           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_PARITY_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 RESERVED0                               :  16;  // Bits 15:0
    UINT32 WR_RSP_MAX_CREDIT                       :  8;  // Bits 23:16
    UINT32 RD_DATA_MAX_CREDIT                      :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_STRUCT;

typedef MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_STRUCT MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC1_STRUCT;
typedef union {
  struct {
    UINT32 force_cfi_clkreq                        :  1;  // Bits 0:0
    UINT32 peer_clkack_bypass                      :  1;  // Bits 1:1
    UINT32 cfi_clkack_bypass                       :  1;  // Bits 2:2
    UINT32 force_peerclkreq                        :  1;  // Bits 3:3
    UINT32 cfi_link_mode_ovr_en                    :  1;  // Bits 4:4
    UINT32 cfi_link_mode_val                       :  1;  // Bits 5:5
    UINT32 RSVD_1                                  :  1;  // Bits 6:6
    UINT32 Cfi_clk_HYST_threshold                  :  7;  // Bits 13:7
    UINT32 cfi_link_clk_detect_hold                :  4;  // Bits 17:14
    UINT32 vc1_read_prio_in_mask                   :  1;  // Bits 18:18
    UINT32 vc1_write_prio_in_mask                  :  1;  // Bits 19:19
    UINT32 P0_disconnect                           :  1;  // Bits 20:20
    UINT32 P0_connect                              :  1;  // Bits 21:21
    UINT32 P0_RX_block_con                         :  1;  // Bits 22:22
    UINT32 P1_disconnect                           :  1;  // Bits 23:23
    UINT32 P1_connect                              :  1;  // Bits 24:24
    UINT32 P1_RX_block_con                         :  1;  // Bits 25:25
    UINT32 P1_TxDATACntEn                          :  1;  // Bits 26:26
    UINT32 P0_TxDATACntEn                          :  1;  // Bits 27:27
    UINT32 P1_RxDATACntEn                          :  1;  // Bits 28:28
    UINT32 P0_RxDATACntEn                          :  1;  // Bits 29:29
    UINT32 RSVD_0                                  :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_CFI_PORT_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 GRACE_CNTR_MAX                          :  8;  // Bits 7:0
    UINT32 CLKGATE_DISABLE                         :  1;  // Bits 8:8
    UINT32 RSVD                                    :  23;  // Bits 31:9
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 COUNTLow                                :  32;  // Bits 31:0
    UINT32 COUNTHigh                               :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_ECC_VC1_RD_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_ECC_VC0_WR_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_ECC_VC1_WR_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_NOECC_VC0_RD_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_NOECC_VC1_RD_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_NOECC_VC0_WR_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_NOECC_VC1_WR_REQCOUNT_STRUCT;
typedef union {
  struct {
    UINT32 VC0_RD_PARK_SIZE                        :  4;  // Bits 3:0
    UINT32 VC1_RD_PARK_SIZE                        :  4;  // Bits 7:4
    UINT32 VC0_WR_PARK_SIZE                        :  4;  // Bits 11:8
    UINT32 VC1_WR_PARK_SIZE                        :  4;  // Bits 15:12
    UINT32 RSVD                                    :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_REQUEST_ARB_CONFIG_STRUCT;

typedef MC0_IBECC_REQUEST_ARB_CONFIG_STRUCT MC0_IBECC_RESPONSE_ARB_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 RSVD0                                   :  3;  // Bits 2:0
    UINT32 CERR_OVERFLOW                           :  1;  // Bits 3:3
    UINT32 MERR_OVERFLOW                           :  1;  // Bits 4:4
    UINT32 ERRADDLow                               :  27;  // Bits 31:5
    UINT32 ERRADDHigh                              :  7;  // Bits 38:32
    UINT32 RSVD1                                   :  7;  // Bits 45:39
    UINT32 ERRSYND                                 :  16;  // Bits 61:46
    UINT32 CERRSTS                                 :  1;  // Bits 62:62
    UINT32 MERRSTS                                 :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_ECC_ERROR_LOG_STRUCT;
typedef union {
  struct {
    UINT32 RSVD1                                   :  4;  // Bits 3:0
    UINT32 PERR_OVERFLOW                           :  1;  // Bits 4:4
    UINT32 ERR_ADDRESSLow                          :  27;  // Bits 31:5
    UINT32 ERR_ADDRESSHigh                         :  7;  // Bits 38:32
    UINT32 RSVD2                                   :  21;  // Bits 59:39
    UINT32 ERR_TYPE                                :  3;  // Bits 62:60
    UINT32 ERR_STS                                 :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_PARITY_ERR_LOG_STRUCT;
typedef union {
  struct {
    UINT32 RSVD1                                   :  5;  // Bits 4:0
    UINT32 ADDRESSLow                              :  27;  // Bits 31:5
    UINT32 ADDRESSHigh                             :  7;  // Bits 38:32
    UINT32 RSVD2                                   :  25;  // Bits 63:39
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_ECC_INJ_ADDR_MASK_STRUCT;

typedef MC0_IBECC_ECC_INJ_ADDR_MASK_STRUCT MC0_IBECC_ECC_INJ_ADDR_BASE_STRUCT;
typedef union {
  struct {
    UINT32 WR_DATA_ERR_EN                          :  8;  // Bits 7:0
    UINT32 RD_ADDR_ERR_EN                          :  1;  // Bits 8:8
    UINT32 WR_HDR_ERR_EN                           :  2;  // Bits 10:9
    UINT32 RD_DATA_ERR_EN                          :  8;  // Bits 18:11
    UINT32 RD_HDR_ERR_EN                           :  2;  // Bits 20:19
    UINT32 RSVD3                                   :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_PARITY_ERR_INJ_STRUCT;
typedef union {
  struct {
    UINT32 OPERATION_MODE                          :  2;  // Bits 1:0
    UINT32 EC_DIS                                  :  1;  // Bits 2:2
    UINT32 DIS_PCIE_ERROR                          :  1;  // Bits 3:3
    UINT32 DIS_MCA_LOG                             :  1;  // Bits 4:4
    UINT32 CLKGT_DIS                               :  1;  // Bits 5:5
    UINT32 PM_RSP_WAIT_ON_DISCONNECT               :  1;  // Bits 6:6
    UINT32 FIXED_WINDOW                            :  1;  // Bits 7:7
    UINT32 SYN_REQ_PRIO_OVERRIDE                   :  1;  // Bits 8:8
    UINT32 DIS_HAZARD                              :  1;  // Bits 9:9
    UINT32 DISABLE_FASTSLEEP                       :  1;  // Bits 10:10
    UINT32 RSVD_11                                 :  1;  // Bits 11:11
    UINT32 ECC_GRANULARITY                         :  1;  // Bits 12:12
    UINT32 RSVD_13                                 :  1;  // Bits 13:13
    UINT32 DIS_PERF_COUNTERS                       :  1;  // Bits 14:14
    UINT32 DISABLE_MULTIPLE_PCH_MSG                :  1;  // Bits 15:15
    UINT32 CFI_IDLE_TIME                           :  8;  // Bits 23:16
    UINT32 RSVD_24                                 :  1;  // Bits 24:24
    UINT32 RSB_ENABLE                              :  1;  // Bits 25:25
    UINT32 DIS_CFI_EARLY_VALID_GEN                 :  2;  // Bits 27:26
    UINT32 RSVD_28                                 :  1;  // Bits 28:28
    UINT32 DIS_INIT_BLOCK                          :  1;  // Bits 29:29
    UINT32 INIT_BLOCK_PROT_ONLY                    :  1;  // Bits 30:30
    UINT32 RSVD_31                                 :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 ECC_INJECT                              :  3;  // Bits 2:0
    UINT32 RSVD1                                   :  5;  // Bits 7:3
    UINT32 COUNT                                   :  8;  // Bits 15:8
    UINT32 RSVD2                                   :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_ECC_INJ_CONTROL_STRUCT;

typedef MC0_IBECC_MBIST_POST_CTRL_WDB_STRUCT MC0_IBECC_MBIST_POST_CTRL_RSB_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_ECC_VC0_SYND_RD_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_ECC_VC1_SYND_RD_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_ECC_VC0_SYND_WR_REQCOUNT_STRUCT;

typedef MC0_IBECC_ECC_VC0_RD_REQCOUNT_STRUCT MC0_IBECC_ECC_VC1_SYND_WR_REQCOUNT_STRUCT;
typedef union {
  struct {
    UINT32 MAX_VC0_ENTRIES                         :  6;  // Bits 5:0
    UINT32 MAX_VC1_ENTRIES                         :  6;  // Bits 11:6
    UINT32 RSVD                                    :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_STRUCT;
typedef union {
  struct {
    UINT32 BLOCK_PROTECTED_VC0_RD_REQ              :  1;  // Bits 0:0
    UINT32 BLOCK_PROTECTED_VC1_RD_REQ              :  1;  // Bits 1:1
    UINT32 BLOCK_PROTECTED_VC0_WR_REQ              :  1;  // Bits 2:2
    UINT32 BLOCK_PROTECTED_VC1_WR_REQ              :  1;  // Bits 3:3
    UINT32 BLOCK_UNPROTECTED_VC0_RD_REQ            :  1;  // Bits 4:4
    UINT32 BLOCK_UNPROTECTED_VC1_RD_REQ            :  1;  // Bits 5:5
    UINT32 BLOCK_UNPROTECTED_VC0_WR_REQ            :  1;  // Bits 6:6
    UINT32 BLOCK_UNPROTECTED_VC1_WR_REQ            :  1;  // Bits 7:7
    UINT32 BLOCK_PROTECTED_VC0_RD_CPL              :  1;  // Bits 8:8
    UINT32 BLOCK_NON_PROTECTED_VC0_RD_CPL          :  1;  // Bits 9:9
    UINT32 BLOCK_PROTECTED_VC1_RD_CPL              :  1;  // Bits 10:10
    UINT32 BLOCK_NON_PROTECTED_VC1_RD_CPL          :  1;  // Bits 11:11
    UINT32 BLOCK_INIT_WR_REQ                       :  1;  // Bits 12:12
    UINT32 BLOCK_PROTECTED_VC0_WR_RSP              :  1;  // Bits 13:13
    UINT32 BLOCK_UNPROTECTED_VC0_WR_RSP            :  1;  // Bits 14:14
    UINT32 BLOCK_PROTECTED_VC1_WR_RSP              :  1;  // Bits 15:15
    UINT32 BLOCK_UNPROTECTED_VC1_WR_RSP            :  1;  // Bits 16:16
    UINT32 RSVD                                    :  15;  // Bits 31:17
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_BLOCK_BIDS_STRUCT;
typedef union {
  struct {
    UINT32 ADDRESS                                 :  15;  // Bits 14:0
    UINT32 RSVD                                    :  17;  // Bits 31:15
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT;

typedef MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT MC0_IBECC_ECC_STORAGE_ADDR_RANGE_1_STRUCT;

typedef MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT MC0_IBECC_ECC_STORAGE_ADDR_RANGE_2_STRUCT;

typedef MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT MC0_IBECC_ECC_STORAGE_ADDR_RANGE_3_STRUCT;

typedef MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT MC0_IBECC_ECC_STORAGE_ADDR_RANGE_4_STRUCT;

typedef MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT MC0_IBECC_ECC_STORAGE_ADDR_RANGE_5_STRUCT;

typedef MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT MC0_IBECC_ECC_STORAGE_ADDR_RANGE_6_STRUCT;

typedef MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT MC0_IBECC_ECC_STORAGE_ADDR_RANGE_7_STRUCT;
typedef union {
  struct {
    UINT32 MDST_SBC                                :  2;  // Bits 1:0
    UINT32 MDST_STBYP                              :  1;  // Bits 2:2
    UINT32 MDST_WMCE                               :  2;  // Bits 4:3
    UINT32 MDST_RMCE                               :  2;  // Bits 6:5
    UINT32 MDST_MCE                                :  1;  // Bits 7:7
    UINT32 MDST_TRIM_FUSE                          :  13;  // Bits 20:8
    UINT32 RSVD                                    :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_MDST_TRIM_FUSE_STRUCT;
typedef union {
  struct {
    UINT32 ECC_ERR_COUNTLow                        :  32;  // Bits 31:0
    UINT32 ECC_ERR_COUNTHigh                       :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_ECC_ERR_COUNT_STRUCT;

typedef MC0_IBECC_DEFAULT_PG_CP_STRUCT MC0_IBECC_RESTRICTED_PG_CP_STRUCT;

typedef MC0_IBECC_DEFAULT_PG_RAC_STRUCT MC0_IBECC_RESTRICTED_PG_RAC_STRUCT;

typedef MC0_IBECC_DEFAULT_PG_CP_STRUCT MC0_IBECC_RESTRICTED_PG_WAC_STRUCT;
typedef union {
  struct {
    UINT32 INIT_RANGE_0                            :  1;  // Bits 0:0
    UINT32 INIT_RANGE_1                            :  1;  // Bits 1:1
    UINT32 INIT_RANGE_2                            :  1;  // Bits 2:2
    UINT32 INIT_RANGE_3                            :  1;  // Bits 3:3
    UINT32 INIT_RANGE_4                            :  1;  // Bits 4:4
    UINT32 INIT_RANGE_5                            :  1;  // Bits 5:5
    UINT32 INIT_RANGE_6                            :  1;  // Bits 6:6
    UINT32 INIT_RANGE_7                            :  1;  // Bits 7:7
    UINT32 RSVD                                    :  23;  // Bits 30:8
    UINT32 INIT_ALL_RANGES                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_MEMORY_INIT_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 DATA_TX_CNT                             :  25;  // Bits 24:0
    UINT32 RSVD                                    :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_P1_DATA_TX_CNT_STRUCT;
typedef union {
  struct {
    UINT32 DATA_RX_CNT                             :  25;  // Bits 24:0
    UINT32 RSVD                                    :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_P1_DATA_RX_CNT_STRUCT;

typedef MC0_IBECC_P1_DATA_TX_CNT_STRUCT MC0_IBECC_P0_DATA_TX_CNT_STRUCT;

typedef MC0_IBECC_P1_DATA_RX_CNT_STRUCT MC0_IBECC_P0_DATA_RX_CNT_STRUCT;
typedef union {
  struct {
    UINT32 HASH_ENABLED                            :  1;  // Bits 0:0
    UINT32 HASH_LSB                                :  3;  // Bits 3:1
    UINT32 RESERVED0                               :  2;  // Bits 5:4
    UINT32 HASH_MASK                               :  14;  // Bits 19:6
    UINT32 RESERVED1                               :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_ADDR_HASH_STRUCT;
typedef union {
  struct {
    UINT32 IP_VERSION                              :  8;  // Bits 7:0
    UINT32 IP_DERIVATIVE                           :  4;  // Bits 11:8
    UINT32 IP_SEGMENT                              :  4;  // Bits 15:12
    UINT32 IP_SUBVERSION                           :  8;  // Bits 23:16
    UINT32                                         :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_STRUCT;
typedef union {
  struct {
    UINT32 HOST_ID                                 :  8;  // Bits 7:0
    UINT32 DTL                                     :  1;  // Bits 8:8
    UINT32 RSL                                     :  1;  // Bits 9:9
    UINT32 RSVD                                    :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_CFI_HOST_STRUCT;
typedef union {
  struct {
    UINT32 DDR_TYPE                                :  2;  // Bits 1:0
    UINT32 ECC                                     :  1;  // Bits 2:2
    UINT32                                         :  5;  // Bits 7:3
    UINT32 CH_0_SIZE                               :  8;  // Bits 15:8
    UINT32 CH_1_SIZE                               :  8;  // Bits 23:16
    UINT32                                         :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MAD_INTER_CHANNEL_STRUCT;
typedef union {
  struct {
    UINT32 SUBCH_0_DENSITY                         :  4;  // Bits 3:0
    UINT32                                         :  2;  // Bits 5:4
    UINT32 SUBCH_0_WIDTH                           :  1;  // Bits 6:6
    UINT32 SUBCH_0_RANKS                           :  1;  // Bits 7:7
    UINT32 SUBCH_1_DENSITY                         :  4;  // Bits 11:8
    UINT32                                         :  2;  // Bits 13:12
    UINT32 SUBCH_1_WIDTH                           :  1;  // Bits 14:14
    UINT32 SUBCH_1_RANKS                           :  1;  // Bits 15:15
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MAD_INTRA_CHANNEL_0_STRUCT;

typedef MC0_MAD_INTRA_CHANNEL_0_STRUCT MC0_MAD_INTRA_CHANNEL_1_STRUCT;
typedef union {
  struct {
    UINT32 eim                                     :  1;  // Bits 0:0
    UINT32 ebh                                     :  1;  // Bits 1:1
    UINT32                                         :  2;  // Bits 3:2
    UINT32 bg0hash                                 :  3;  // Bits 6:4
    UINT32                                         :  1;  // Bits 7:7
    UINT32 bg1hash                                 :  3;  // Bits 10:8
    UINT32                                         :  1;  // Bits 11:11
    UINT32 bg2hash                                 :  3;  // Bits 14:12
    UINT32                                         :  17;  // Bits 31:15
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MAD_DRAM_HASH_0_STRUCT;

typedef MC0_MAD_DRAM_HASH_0_STRUCT MC0_MAD_DRAM_HASH_1_STRUCT;
typedef union {
  struct {
    UINT32 Spare_RW                                :  16;  // Bits 15:0
    UINT32 cbit_RGBSync_accelerate_x16             :  1;  // Bits 16:16
    UINT32 cbit_ddrpl_enable_mains_glbdrv          :  1;  // Bits 17:17
    UINT32 spare_RW_V                              :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MCDECS_MISC_STRUCT;
typedef union {
  struct {
    UINT32 increase_rcomp                          :  1;  // Bits 0:0
    UINT32 invert_dclk_clone                       :  1;  // Bits 1:1
    UINT32                                         :  1;  // Bits 2:2
    UINT32 freeze_pwm_counters                     :  1;  // Bits 3:3
    UINT32 dis_single_ch_sr                        :  1;  // Bits 4:4
    UINT32 dis_other_ch_stolen_ref                 :  1;  // Bits 5:5
    UINT32 oppsr_pending_wr_clkgate_dis_mcmain     :  1;  // Bits 6:6
    UINT32 block_rsp_credit_return                 :  1;  // Bits 7:7
    UINT32 block_data_credit_return                :  1;  // Bits 8:8
    UINT32 dis_clk_gate                            :  1;  // Bits 9:9
    UINT32 freeze_AFD_on_RTB                       :  1;  // Bits 10:10
    UINT32 iosfsb_keep_ISM_active                  :  1;  // Bits 11:11
    UINT32 ignoreRefBetweenSRX2SRE                 :  1;  // Bits 12:12
    UINT32 dis_cfi_spec_req_early_valid            :  1;  // Bits 13:13
    UINT32                                         :  4;  // Bits 17:14
    UINT32 dis_cmi_ep_power_flows                  :  1;  // Bits 18:18
    UINT32 dis_iosf_sb_error_opcodes               :  1;  // Bits 19:19
    UINT32 dis_iosf_sb_error_bar                   :  1;  // Bits 20:20
    UINT32 dis_iosf_sb_error_range                 :  1;  // Bits 21:21
    UINT32 dis_iosf_sb_error_sai                   :  1;  // Bits 22:22
    UINT32                                         :  1;  // Bits 23:23
    UINT32 dis_cfi_spec_rsp_cpl_early_valid        :  1;  // Bits 24:24
    UINT32 force_sb_ep_clk_req                     :  1;  // Bits 25:25
    UINT32 dis_cfi_wr_rsp                          :  1;  // Bits 26:26
    UINT32 dis_iosf_sb_clk_gate                    :  1;  // Bits 27:27
    UINT32 dis_glbdrv_clk_gate                     :  1;  // Bits 28:28
    UINT32 dis_reg_clk_gate                        :  1;  // Bits 29:29
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MCDECS_CBIT_STRUCT;
typedef union {
  struct {
    UINT32                                         :  6;  // Bits 5:0
    UINT32 HASH_MASK                               :  14;  // Bits 19:6
    UINT32                                         :  4;  // Bits 23:20
    UINT32 HASH_LSB_MASK_BIT                       :  3;  // Bits 26:24
    UINT32                                         :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MAD_CHANNEL_HASH_STRUCT;
typedef union {
  struct {
    UINT32                                         :  6;  // Bits 5:0
    UINT32 SUBCH_HASH_MASK                         :  14;  // Bits 19:6
    UINT32                                         :  4;  // Bits 23:20
    UINT32 SUBCH_HASH_LSB_MASK_BIT                 :  3;  // Bits 26:24
    UINT32                                         :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MAD_SUB_CHANNEL_HASH_STRUCT;
typedef union {
  struct {
    UINT32 CMI_Source_ID0                          :  8;  // Bits 7:0
    UINT32 CMI_Source_ID1                          :  8;  // Bits 15:8
    UINT32 CMI_Source_ID2                          :  8;  // Bits 23:16
    UINT32 CMI_Source_ID3                          :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32                                         :  3;  // Bits 2:0
    UINT32 refresh_enable                          :  1;  // Bits 3:3
    UINT32 init_complete                           :  4;  // Bits 7:4
    UINT32 sr_state                                :  1;  // Bits 8:8
    UINT32 pure_srx                                :  1;  // Bits 9:9
    UINT32                                         :  12;  // Bits 21:10
    UINT32 dclk_enable                             :  1;  // Bits 22:22
    UINT32                                         :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MC_INIT_STATE_G_STRUCT;
typedef union {
  struct {
    UINT32 IP_VERSION                              :  8;  // Bits 7:0
    UINT32 IP_DERIVATIVE                           :  4;  // Bits 11:8
    UINT32 IP_SEGMENT                              :  8;  // Bits 19:12
    UINT32                                         :  4;  // Bits 23:20
    UINT32 IP_SUBVERSION                           :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MC_IP_VERSION_STRUCT;
typedef union {
  struct {
    UINT32 IP_MODEL                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MC_RTL_VERSION_STRUCT;
typedef union {
  struct {
    UINT32 Idle_timer                              :  16;  // Bits 15:0
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PM_OPP_SREF_IDLE_TIMER_STRUCT;
typedef union {
  struct {
    UINT32 countLow                                :  32;  // Bits 31:0
    UINT32 countHigh                               :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_PWM_TOTAL_REQCOUNT_STRUCT;

typedef MC0_PWM_TOTAL_REQCOUNT_STRUCT MC0_PWM_PROGRAMMABLE_REQCOUNT_0_STRUCT;

typedef MC0_PWM_TOTAL_REQCOUNT_STRUCT MC0_PWM_PROGRAMMABLE_REQCOUNT_1_STRUCT;

typedef MC0_PWM_TOTAL_REQCOUNT_STRUCT MC0_PWM_RDCAS_COUNT_STRUCT;
typedef union {
  struct {
    UINT32 ALLOW_OPP_SR                            :  1;  // Bits 0:0
    UINT32                                         :  2;  // Bits 2:1
    UINT32 pm_opp_sr_policy                        :  2;  // Bits 4:3
    UINT32                                         :  11;  // Bits 15:5
    UINT32 min_duration                            :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PM_OPP_SREF_ENABLE_STRUCT;

typedef MC0_PWM_TOTAL_REQCOUNT_STRUCT MC0_READ_OCCUPANCY_COUNT_STRUCT;
typedef union {
  struct {
    UINT32 stall_until_drain                       :  1;  // Bits 0:0
    UINT32 stall_input                             :  1;  // Bits 1:1
    UINT32                                         :  2;  // Bits 3:2
    UINT32 mc_drained                              :  1;  // Bits 4:4
    UINT32                                         :  7;  // Bits 11:5
    UINT32 stall_state                             :  1;  // Bits 12:12
    UINT32                                         :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_STALL_DRAIN_STRUCT;
typedef union {
  struct {
    UINT32 NonVC1Threshold                         :  4;  // Bits 3:0
    UINT32 VC1RdThreshold                          :  4;  // Bits 7:4
    UINT32 FixedRateEn                             :  1;  // Bits 8:8
    UINT32 HIGH_PRIO_LIM                           :  3;  // Bits 11:9
    UINT32 LOW_PRIO_LIM                            :  3;  // Bits 14:12
    UINT32 spare                                   :  8;  // Bits 22:15
    UINT32                                         :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IPC_MC_ARB_STRUCT;

typedef MC0_IPC_MC_ARB_STRUCT MC0_IPC_MC_DEC_ARB_STRUCT;
typedef union {
  struct {
    UINT32 RPQ_count                               :  6;  // Bits 5:0
    UINT32                                         :  2;  // Bits 7:6
    UINT32 WPQ_count                               :  7;  // Bits 14:8
    UINT32                                         :  1;  // Bits 15:15
    UINT32 IPQ_count                               :  5;  // Bits 20:16
    UINT32 WPQ_MinSlotsToReq                       :  4;  // Bits 24:21
    UINT32 IPQ_MinSlotsToReq                       :  3;  // Bits 27:25
    UINT32 RPQ_MinSlotsToReq                       :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_QUEUE_CREDIT_C_STRUCT;
typedef union {
  struct {
    UINT32 AddressLow                              :  32;  // Bits 31:0
    UINT32 AddressHigh                             :  1;  // Bits 32:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_ECC_INJ_ADDR_COMPARE_STRUCT;

typedef MC0_PWM_TOTAL_REQCOUNT_STRUCT MC0_PWM_WRCAS_COUNT_STRUCT;

typedef MC0_PWM_TOTAL_REQCOUNT_STRUCT MC0_PWM_COMMAND_COUNT_STRUCT;
typedef union {
  struct {
    UINT32                                         :  20;  // Bits 19:0
    UINT32 TOLUD                                   :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_TOLUD_STRUCT;

typedef MC0_PWM_TOTAL_REQCOUNT_STRUCT MC0_PWM_ACT_COUNT_STRUCT;
typedef union {
  struct {
    UINT32 sys_addrLow                             :  32;  // Bits 31:0
    UINT32 sys_addrHigh                            :  1;  // Bits 32:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_STRUCT;

typedef MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_STRUCT MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_1_STRUCT;

typedef MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_STRUCT MC0_GDXC_DDR_SYS_ADD_FILTER_MATCH_0_STRUCT;

typedef MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_STRUCT MC0_GDXC_DDR_SYS_ADD_FILTER_MATCH_1_STRUCT;

typedef MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_STRUCT MC0_GDXC_DDR_SYS_ADD_TRIGGER_MASK_STRUCT;

typedef MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_STRUCT MC0_GDXC_DDR_SYS_ADD_TRIGGER_MATCH_STRUCT;
typedef union {
  struct {
    UINT32 Isoch_time_window                       :  17;  // Bits 16:0
    UINT32 Write_starvation_window                 :  11;  // Bits 27:17
    UINT32                                         :  4;  // Bits 31:28
    UINT32 Read_starvation_window                  :  11;  // Bits 42:32
    UINT32 VC0_read_count                          :  9;  // Bits 51:43
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_SC_QOS_STRUCT;
typedef union {
  struct {
    UINT32 GLB_GRACE_CNT                           :  8;  // Bits 7:0
    UINT32 GLB_DRV_GATE_DIS                        :  1;  // Bits 8:8
    UINT32 qclk_global_driver_override_to_dclk     :  1;  // Bits 9:9
    UINT32                                         :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_STRUCT;
typedef union {
  struct {
    UINT32 delay_PM_Ack_cycles                     :  10;  // Bits 9:0
    UINT32 R1_Ack_reset_bgf_run                    :  1;  // Bits 10:10
    UINT32 SAGV_Ack_reset_bgf_run                  :  1;  // Bits 11:11
    UINT32 init_complete_reset_bgf_run             :  1;  // Bits 12:12
    UINT32 Override_DDRPLDrainDone_Cbit            :  1;  // Bits 13:13
    UINT32 en_lpmode_exit_any_rd_early_wakeup      :  1;  // Bits 14:14
    UINT32 PM_REQ_Status_Clear_Select              :  1;  // Bits 15:15
    UINT32 PM_REQ_R0_received                      :  1;  // Bits 16:16
    UINT32 PM_REQ_R0_SAGV_received                 :  1;  // Bits 17:17
    UINT32 PM_REQ_R1_received                      :  1;  // Bits 18:18
    UINT32 PM_REQ_R1_deep_sr_received              :  1;  // Bits 19:19
    UINT32 PM_REQ_UnBlock_received                 :  1;  // Bits 20:20
    UINT32 PM_REQ_Acked                            :  1;  // Bits 21:21
    UINT32 MC_State_blocked_R0                     :  1;  // Bits 22:22
    UINT32 MC_State_blocked_R1                     :  1;  // Bits 23:23
    UINT32 PM_REQ_VRCG_Enabled_received            :  1;  // Bits 24:24
    UINT32 PM_REQ_VRCG_Disabled_received           :  1;  // Bits 25:25
    UINT32 PM_REQ_OppSR_cmd_0_received             :  1;  // Bits 26:26
    UINT32 PM_REQ_OppSR_cmd_1_received             :  1;  // Bits 27:27
    UINT32 PM_REQ_OppSR_cmd_2_received             :  1;  // Bits 28:28
    UINT32 en_pdx_early_fabric_wakeup              :  1;  // Bits 29:29
    UINT32 en_srx_early_fabric_wakeup              :  1;  // Bits 30:30
    UINT32 en_pdx_any_rd_early_wakeup              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PM_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 CLOCK_SPINE_GRACE_CNT                   :  8;  // Bits 7:0
    UINT32 CLOCK_SPINE_GATE_DIS                    :  1;  // Bits 8:8
    UINT32                                         :  23;  // Bits 31:9
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CLOCK_SPINE_GATE_CFG_STRUCT;
typedef union {
  struct {
    UINT32 CountLow                                :  32;  // Bits 31:0
    UINT32 CountHigh                               :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_PWM_COUNTERS_DURATION_STRUCT;
typedef union {
  struct {
    UINT32 cmi_req_stall_enable                    :  1;  // Bits 0:0
    UINT32 cmi_req_stall_phase                     :  1;  // Bits 1:1
    UINT32 block_scheduler                         :  4;  // Bits 5:2
    UINT32                                         :  2;  // Bits 7:6
    UINT32 dis_other_mc_stolen_ref                 :  1;  // Bits 8:8
    UINT32                                         :  3;  // Bits 11:9
    UINT32 init_complete_override_en               :  4;  // Bits 15:12
    UINT32 init_complete_override_value            :  4;  // Bits 19:16
    UINT32 prevent_sr_on_vc1_high_prio             :  1;  // Bits 20:20
    UINT32 use_initcomplete_ch0_only               :  1;  // Bits 21:21
    UINT32                                         :  1;  // Bits 22:22
    UINT32 Allow_RH_Debt_in_SR                     :  1;  // Bits 23:23
    UINT32 delay_qsync                             :  2;  // Bits 25:24
    UINT32 hvm_iso_override                        :  1;  // Bits 26:26
    UINT32                                         :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MCDECS_SECOND_CBIT_STRUCT;
typedef union {
  struct {
    UINT32 AddressLow                              :  32;  // Bits 31:0
    UINT32 AddressHigh                             :  1;  // Bits 32:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_ECC_INJ_ADDR_MASK_STRUCT;
typedef union {
  struct {
    UINT32 RW_Isoch_time_window                    :  17;  // Bits 16:0
    UINT32 RW_Write_starvation_window              :  11;  // Bits 27:17
    UINT32                                         :  4;  // Bits 31:28
    UINT32 RW_Read_starvation_window               :  11;  // Bits 42:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_SC_QOS2_STRUCT;
typedef union {
  struct {
    UINT32 Yellow_Decay_x128                       :  9;  // Bits 8:0
    UINT32 Yellow_Threshold                        :  10;  // Bits 18:9
    UINT32                                         :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_SC_QOS3_STRUCT;
typedef union {
  struct {
    UINT32 normalmode                              :  1;  // Bits 0:0
    UINT32 Force_SR_State                          :  2;  // Bits 2:1
    UINT32 SR_is_forced                            :  1;  // Bits 3:3
    UINT32                                         :  28;  // Bits 31:4
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_NORMALMODE_CFG_STRUCT;
typedef union {
  struct {
    UINT32                                         :  2;  // Bits 1:0
    UINT32 reset_cpgc                              :  1;  // Bits 2:2
    UINT32                                         :  21;  // Bits 23:3
    UINT32 cpgc_active                             :  1;  // Bits 24:24
    UINT32                                         :  3;  // Bits 27:25
    UINT32 cpgc_ecc_byte                           :  3;  // Bits 30:28
    UINT32 cpgc_ecc_enable                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MC_CPGC_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 in_order_ingress                        :  1;  // Bits 0:0
    UINT32 lock_on_active_cpgc_cmi_ism             :  1;  // Bits 1:1
    UINT32                                         :  1;  // Bits 2:2
    UINT32 delay_cpl_data                          :  1;  // Bits 3:3
    UINT32 delay_cpl_info                          :  1;  // Bits 4:4
    UINT32 halfcachelinemode                       :  1;  // Bits 5:5
    UINT32 cmd_req_noninterleave                   :  1;  // Bits 6:6
    UINT32                                         :  25;  // Bits 31:7
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MC_CPGC_CONTROL_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 ERR_ADDRESSLow                          :  32;  // Bits 31:0
    UINT32 ERR_ADDRESSHigh                         :  7;  // Bits 38:32
    UINT32 RESERVED                                :  20;  // Bits 58:39
    UINT32 ERR_SRC                                 :  1;  // Bits 59:59
    UINT32 ERR_TYPE                                :  2;  // Bits 61:60
    UINT32 ERR_STS_OVERFLOW                        :  1;  // Bits 62:62
    UINT32 ERR_STS                                 :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_PARITYERRLOG_STRUCT;
typedef union {
  struct {
    UINT32 DATA_ERR_EN                             :  8;  // Bits 7:0
    UINT32 REQ_ADDR_ERR_EN                         :  1;  // Bits 8:8
    UINT32 RWD_HEADER_ERR_EN                       :  2;  // Bits 10:9
    UINT32 RSVD                                    :  5;  // Bits 15:11
    UINT32 ERR_INJ_MASK                            :  6;  // Bits 21:16
    UINT32 RSVD2                                   :  9;  // Bits 30:22
    UINT32 DATA_ERR_INJ_SRC                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PARITY_ERR_INJ_STRUCT;
typedef union {
  struct {
    UINT32 REQ_ADDR_PARITY_EN                      :  1;  // Bits 0:0
    UINT32 RWD_HEADER_PARITY_EN                    :  1;  // Bits 1:1
    UINT32 WDATA_PARITY_EN                         :  2;  // Bits 3:2
    UINT32 RDATA_PARITY_EN                         :  1;  // Bits 4:4
    UINT32 RDCPL_HEADER_PARITY_EN                  :  1;  // Bits 5:5
    UINT32 RSVD_0                                  :  2;  // Bits 7:6
    UINT32 DIS_PARITY_PCIE_ERR                     :  1;  // Bits 8:8
    UINT32 DIS_PARITY_LOG                          :  1;  // Bits 9:9
    UINT32 RSVD_1                                  :  21;  // Bits 30:10
    UINT32 PARITY_EN                               :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PARITY_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 Hash_enabled                            :  1;  // Bits 0:0
    UINT32 Hash_LSB                                :  3;  // Bits 3:1
    UINT32                                         :  16;  // Bits 19:4
    UINT32 Zone1_start                             :  10;  // Bits 29:20
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MAD_MC_HASH_STRUCT;
typedef union {
  struct {
    UINT32 frz                                     :  1;  // Bits 0:0
    UINT32                                         :  7;  // Bits 7:1
    UINT32 reset_ctrl                              :  1;  // Bits 8:8
    UINT32 reset_ctrs                              :  1;  // Bits 9:9
    UINT32                                         :  22;  // Bits 31:10
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_PMON_UNIT_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 ctr_ov                                  :  5;  // Bits 4:0
    UINT32                                         :  27;  // Bits 31:5
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_PMON_UNIT_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 ev_sel                                  :  8;  // Bits 7:0
    UINT32 ch_mask                                 :  4;  // Bits 11:8
    UINT32                                         :  5;  // Bits 16:12
    UINT32 rst                                     :  1;  // Bits 17:17
    UINT32 edge_det                                :  1;  // Bits 18:18
    UINT32                                         :  1;  // Bits 19:19
    UINT32 frz_on_ov                               :  1;  // Bits 20:20
    UINT32                                         :  2;  // Bits 22:21
    UINT32 invert                                  :  1;  // Bits 23:23
    UINT32 thresh                                  :  8;  // Bits 31:24
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_PMON_COUNTER_CONTROL_0_STRUCT;

typedef MC0_PMON_COUNTER_CONTROL_0_STRUCT MC0_PMON_COUNTER_CONTROL_1_STRUCT;

typedef MC0_PMON_COUNTER_CONTROL_0_STRUCT MC0_PMON_COUNTER_CONTROL_2_STRUCT;

typedef MC0_PMON_COUNTER_CONTROL_0_STRUCT MC0_PMON_COUNTER_CONTROL_3_STRUCT;

typedef MC0_PMON_COUNTER_CONTROL_0_STRUCT MC0_PMON_COUNTER_CONTROL_4_STRUCT;
typedef union {
  struct {
    UINT32 event_countLow                          :  32;  // Bits 31:0
    UINT32 event_countHigh                         :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_PMON_COUNTER_DATA_0_STRUCT;

typedef MC0_PMON_COUNTER_DATA_0_STRUCT MC0_PMON_COUNTER_DATA_1_STRUCT;

typedef MC0_PMON_COUNTER_DATA_0_STRUCT MC0_PMON_COUNTER_DATA_2_STRUCT;

typedef MC0_PMON_COUNTER_DATA_0_STRUCT MC0_PMON_COUNTER_DATA_3_STRUCT;

typedef MC0_PMON_COUNTER_DATA_0_STRUCT MC0_PMON_COUNTER_DATA_4_STRUCT;
typedef union {
  struct {
    UINT32 EnableOSTelemetry                       :  1;  // Bits 0:0
    UINT32 enable_idle_ostelemetry                 :  1;  // Bits 1:1
    UINT32                                         :  30;  // Bits 31:2
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_OS_TELEMETRY_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 read_delay                              :  12;  // Bits 11:0
    UINT32 read_req                                :  6;  // Bits 17:12
    UINT32 write_req                               :  6;  // Bits 23:18
    UINT32 LPMode4_EN                              :  2;  // Bits 25:24
    UINT32 delay_lp_vc1                            :  1;  // Bits 26:26
    UINT32                                         :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PM_OPP_SR_POLICY_0_STRUCT;

typedef MC0_PM_OPP_SR_POLICY_0_STRUCT MC0_PM_OPP_SR_POLICY_1_STRUCT;
typedef union {
  struct {
    UINT32                                         :  4;  // Bits 3:0
    UINT32 dis_cfi_clk_gate                        :  1;  // Bits 4:4
    UINT32                                         :  1;  // Bits 5:5
    UINT32 cfi_rx_lock_on_connect                  :  1;  // Bits 6:6
    UINT32                                         :  2;  // Bits 8:7
    UINT32 cfi_tx_lock_on_connect                  :  1;  // Bits 9:9
    UINT32 cfi_rx_sagv_block_en                    :  1;  // Bits 10:10
    UINT32 cfi_tx_sagv_block_en                    :  1;  // Bits 11:11
    UINT32 cfi_sagv_block_delay                    :  4;  // Bits 15:12
    UINT32                                         :  11;  // Bits 26:16
    UINT32 mc_cfi_link_peer_clkack_bypass          :  1;  // Bits 27:27
    UINT32 mc_rsp_peer_clkack_bypass               :  1;  // Bits 28:28
    UINT32 mc_req_credits_peer_clkack_bypass       :  1;  // Bits 29:29
    UINT32 mc_rwd_credits_peer_clkack_bypass       :  1;  // Bits 30:30
    UINT32                                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MC_CFI_FABRIC_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 Cclk_is_Dclk                            :  1;  // Bits 0:0
    UINT32 dis_VC1_cpl_interleave                  :  1;  // Bits 1:1
    UINT32 dis_nonVC1_cpl_interleave               :  1;  // Bits 2:2
    UINT32 dis_all_cpl_interleave                  :  1;  // Bits 3:3
    UINT32                                         :  28;  // Bits 31:4
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_MC_CFI_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 RSP                                     :  1;  // Bits 0:0
    UINT32 Force_drain_R0_message                  :  1;  // Bits 1:1
    UINT32 Force_drain_R1_message                  :  1;  // Bits 2:2
    UINT32 Force_drain_R0_SAGV_message             :  1;  // Bits 3:3
    UINT32 Force_unblock_message                   :  1;  // Bits 4:4
    UINT32 Force_vrcg_enable_message               :  1;  // Bits 5:5
    UINT32 Force_vrcg_disable_message              :  1;  // Bits 6:6
    UINT32 Force_OppSR_cmd_message                 :  1;  // Bits 7:7
    UINT32 OppSR_cmd_select                        :  2;  // Bits 9:8
    UINT32                                         :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PM_REQ_FORCE_DFX_STRUCT;
typedef union {
  struct {
    UINT32 DVP_block_req_delay                     :  9;  // Bits 8:0
    UINT32 Override_CFI_RX_Req_trace_packet        :  1;  // Bits 9:9
    UINT32 Override_CFI_RX_Data_trace_packet       :  1;  // Bits 10:10
    UINT32                                         :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_DDRPL_CFG2_DTF_STRUCT;
typedef union {
  struct {
    UINT32 VC1_Read_starvation_en                  :  1;  // Bits 0:0
    UINT32 Write_starvation_in_Isoc_en             :  1;  // Bits 1:1
    UINT32 Read_starvation_in_Isoch_en             :  1;  // Bits 2:2
    UINT32 VC0_counter_disable                     :  1;  // Bits 3:3
    UINT32 Force_MCVC1Demote                       :  1;  // Bits 4:4
    UINT32 Disable_MCVC1Demote                     :  1;  // Bits 5:5
    UINT32 MC_Ignore_VC1Demote                     :  1;  // Bits 6:6
    UINT32 Ignore_RGBSync                          :  1;  // Bits 7:7
    UINT32 Force_MC_WPriority                      :  1;  // Bits 8:8
    UINT32 Disable_MC_WPriority                    :  1;  // Bits 9:9
    UINT32 allow_cross_vc_blocking                 :  1;  // Bits 10:10
    UINT32 VC1_block_VC0                           :  1;  // Bits 11:11
    UINT32 VC0_block_VC1                           :  1;  // Bits 12:12
    UINT32 Delay_VC1_on_read_starvation            :  1;  // Bits 13:13
    UINT32                                         :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_SC_QOS_UARCH_STRUCT;
typedef union {
  struct {
    UINT32                                         :  4;  // Bits 3:0
    UINT32 MCHTrace                                :  1;  // Bits 4:4
    UINT32 SCHTrace                                :  1;  // Bits 5:5
    UINT32                                         :  4;  // Bits 9:6
    UINT32 Switch_64b_Data_Trace                   :  1;  // Bits 10:10
    UINT32 DVP_Never_Block_Req                     :  1;  // Bits 11:11
    UINT32 DVP_Block_Req_on_PMA_clk_stop           :  1;  // Bits 12:12
    UINT32 DSO_BlkReq_GVInProgress_DIS             :  1;  // Bits 13:13
    UINT32 DTF_Survive_Mode                        :  1;  // Bits 14:14
    UINT32 CFI_OBS_clk_resume_cbit                 :  1;  // Bits 15:15
    UINT32                                         :  1;  // Bits 16:16
    UINT32 Enable_ACC_Trace                        :  1;  // Bits 17:17
    UINT32 Enable_ReadData_Trace                   :  1;  // Bits 18:18
    UINT32 Enable_WriteData_Trace                  :  1;  // Bits 19:19
    UINT32 DDRPL_Activate                          :  1;  // Bits 20:20
    UINT32 DDRPL_GLB_DRV_GATE_DIS                  :  1;  // Bits 21:21
    UINT32 CFI_OBS_BLOCK_ON_SAGV                   :  1;  // Bits 22:22
    UINT32                                         :  6;  // Bits 28:23
    UINT32 Spare_RW                                :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_DDRPL_CFG_DTF_STRUCT;
typedef union {
  struct {
    UINT32                                         :  16;  // Bits 15:0
    UINT32 Read_Data_UserDefined_Bits              :  8;  // Bits 23:16
    UINT32 Write_Data_UserDefined_Bits             :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_DDRPL_DEBUG_DTF_STRUCT;
typedef union {
  struct {
    UINT32 Visa_Qclk_Lane_1_out                    :  8;  // Bits 7:0
    UINT32 Visa_Qclk_Lane_0_out                    :  8;  // Bits 15:8
    UINT32 Visa_Dclk_Lane_1_out                    :  8;  // Bits 23:16
    UINT32 Visa_Dclk_Lane_0_out                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_DDRPL_VISA_LANES_STRUCT;
typedef union {
  struct {
    UINT32 VisaLane0_Data_Stage_Dclk_tree          :  2;  // Bits 1:0
    UINT32 VisaLane1_Data_Stage_Dclk_tree          :  2;  // Bits 3:2
    UINT32 VisaLane0_Data_Stage_Qclk_tree          :  2;  // Bits 5:4
    UINT32 VisaLane1_Data_Stage_Qclk_tree          :  2;  // Bits 7:6
    UINT32                                         :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_DDRPL_VISA_CFG_DTF_STRUCT;
typedef union {
  struct {
    UINT32 Data_Filter_Rank_EN                     :  1;  // Bits 0:0
    UINT32 Data_Filter_Group_EN                    :  1;  // Bits 1:1
    UINT32 Data_Filter_Bank_EN                     :  1;  // Bits 2:2
    UINT32 Data_Filter_Row_EN                      :  1;  // Bits 3:3
    UINT32 Data_Filter_Column_EN                   :  1;  // Bits 4:4
    UINT32 Column_Value                            :  8;  // Bits 12:5
    UINT32                                         :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_DDRPL_DRAMADDRESS_FILTER_STRUCT;
typedef union {
  struct {
    UINT32 Rank_Value                              :  2;  // Bits 1:0
    UINT32 Group_Value                             :  2;  // Bits 3:2
    UINT32 Bank_Value                              :  3;  // Bits 6:4
    UINT32 Row_Value                               :  18;  // Bits 24:7
    UINT32                                         :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_DDRPL_DRAMADDRESS_FILTER2_STRUCT;

#pragma pack(pop)
#endif
