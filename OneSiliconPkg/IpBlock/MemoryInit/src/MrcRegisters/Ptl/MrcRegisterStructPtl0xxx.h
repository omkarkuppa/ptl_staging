#ifndef __MrcRegisterStructPtl0xxx_h__
#define __MrcRegisterStructPtl0xxx_h__

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
    UINT32 rsvd                                    :  6;  // Bits 5:0
    INT32  RxDQPerBitDeskewOffset                  :  4;  // Bits 9:6
    UINT32 RxDQPerBitDeskew                        :  6;  // Bits 15:10
    INT32  RxDQVrefOffsetR                         :  5;  // Bits 20:16
    INT32  RxDQVrefOffsetF                         :  5;  // Bits 25:21
    UINT32 TxDQPerBitDeskew                        :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE7_STRUCT;
typedef union {
  struct {
    UINT32 rsvd                                    :  7;  // Bits 6:0
    INT32  RxDQSVrefOffset                         :  5;  // Bits 11:7
    UINT32 Rsvd1                                   :  4;  // Bits 15:12
    INT32  DqsTcoCode                              :  6;  // Bits 21:16
    INT32  DqsPrNfTcoDelay                         :  5;  // Bits 26:22
    INT32  DqsPfNrTcoDelay                         :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA0CH0_CR_DDRDATADQSRANK1_STRUCT;
typedef union {
  struct {
    UINT32 TxDqDelay                               :  11;  // Bits 10:0
    UINT32 TxDqsDelay                              :  11;  // Bits 21:11
    INT32  DqTxEqualization                        :  6;  // Bits 27:22
    UINT32 dqstxequalization                       :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA0CH0_CR_TXCONTROL0RANK1_STRUCT;
typedef union {
  struct {
    INT32  RcvEnOffset                             :  8;  // Bits 7:0
    INT32  RxDqsOffset                             :  7;  // Bits 14:8
    INT32  TxDqOffset                              :  8;  // Bits 22:15
    INT32  TxDqsOffset                             :  8;  // Bits 30:23
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT;
typedef union {
  struct {
    UINT32 BertMode                                :  1;  // Bits 0:0
    UINT32 GearMode                                :  1;  // Bits 1:1
    UINT32 RxSALTailCtrl                           :  2;  // Bits 3:2
    UINT32 wrpreamble                              :  3;  // Bits 6:4
    UINT32 BlockSdlWithRcvEn                       :  1;  // Bits 7:7
    UINT32 RcvEnExtFF                              :  2;  // Bits 9:8
    UINT32 RxVrefVddqProgMFC                       :  3;  // Bits 12:10
    UINT32 RxVrefVttProgMFC                        :  3;  // Bits 15:13
    UINT32 RxBurstLenBy2Minus1                     :  4;  // Bits 19:16
    UINT32 EnDqsNRcvEn                             :  1;  // Bits 20:20
    UINT32 DisableDqsOdtStatic                     :  1;  // Bits 21:21
    UINT32 RxClkStgNum                             :  6;  // Bits 27:22
    UINT32 ForceRcvEnOn                            :  1;  // Bits 28:28
    UINT32 RxClkDlySel                             :  2;  // Bits 30:29
    UINT32 RxSALSpeed                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT;
typedef union {
  struct {
    UINT32 RankEn                                  :  2;  // Bits 1:0
    UINT32 LaneEn                                  :  10;  // Bits 11:2
    UINT32 RankMap                                 :  2;  // Bits 13:12
    UINT32 DccSamples                              :  5;  // Bits 18:14
    UINT32 SaveFullDcc                             :  1;  // Bits 19:19
    UINT32 RORank                                  :  3;  // Bits 22:20
    UINT32 EnRankOvrd                              :  1;  // Bits 23:23
    UINT32 MeasPoint                               :  2;  // Bits 25:24
    UINT32 SkipCRWrite                             :  1;  // Bits 26:26
    UINT32 ForceAck                                :  1;  // Bits 27:27
    UINT32 LargeChangeEnable                       :  1;  // Bits 28:28
    UINT32 LargeChangeReset                        :  1;  // Bits 29:29
    INT32  TargetBitDBI                            :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCFSMCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 DccStepSize                             :  5;  // Bits 4:0
    INT32  DqTargetR0                              :  4;  // Bits 8:5
    INT32  DqTargetR1                              :  4;  // Bits 12:9
    INT32  DqsTargetR0                             :  4;  // Bits 16:13
    INT32  DqsTargetR1                             :  4;  // Bits 20:17
    UINT32 TrackExtremeLane                        :  4;  // Bits 24:21
    UINT32 TrackExtremeMax                         :  1;  // Bits 25:25
    UINT32 TrackExtremeRank                        :  1;  // Bits 26:26
    UINT32 LargeChange                             :  3;  // Bits 29:27
    UINT32 WrLUTDq0                                :  1;  // Bits 30:30
    UINT32 rsvd                                    :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCCALCCONTROL_STRUCT;
typedef union {
  struct {
    INT32  TargetBit0                              :  4;  // Bits 3:0
    INT32  TargetBit1                              :  4;  // Bits 7:4
    INT32  TargetBit2                              :  4;  // Bits 11:8
    INT32  TargetBit3                              :  4;  // Bits 15:12
    INT32  TargetBit4                              :  4;  // Bits 19:16
    INT32  TargetBit5                              :  4;  // Bits 23:20
    INT32  TargetBit6                              :  4;  // Bits 27:24
    INT32  TargetBit7                              :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCLANETARGET_STRUCT;
typedef union {
  struct {
    UINT32 R2RDelayForDqsOffset                    :  3;  // Bits 2:0
    UINT32 R2RDelayForDqOffset                     :  3;  // Bits 5:3
    UINT32 R2RDelayForRxPi                         :  3;  // Bits 8:6
    UINT32 R2RDelayForRcvEn                        :  3;  // Bits 11:9
    UINT32 R2RDQSDelayForEq                        :  3;  // Bits 14:12
    UINT32 R2RDQDelayForEq                         :  3;  // Bits 17:15
    UINT32 R2RDQSDelayForPi                        :  3;  // Bits 20:18
    UINT32 R2RDQDelayForPi                         :  3;  // Bits 23:21
    UINT32 RxRankMuxDelay                          :  4;  // Bits 27:24
    UINT32 TxRankMuxDelay                          :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT;
typedef union {
  struct {
    UINT32 DLLPHClkSel                             :  2;  // Bits 1:0
    UINT32 DllDacCodeFreeze                        :  1;  // Bits 2:2
    UINT32 LockCtr                                 :  3;  // Bits 5:3
    UINT32 LockSel                                 :  1;  // Bits 6:6
    UINT32 VctlInit                                :  11;  // Bits 17:7
    UINT32 DllPwrDownOvrd                          :  1;  // Bits 18:18
    UINT32 DllDcdTargetSel                         :  1;  // Bits 19:19
    UINT32 DllVcdlDccCodeEn                        :  1;  // Bits 20:20
    INT32  DllVcdlDccCode                          :  4;  // Bits 24:21
    UINT32 FastLockCtr                             :  3;  // Bits 27:25
    UINT32 EnTurboOnStartUp                        :  1;  // Bits 28:28
    UINT32 EnWL                                    :  1;  // Bits 29:29
    UINT32 DLLSegmentDisable                       :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT;
typedef union {
  struct {
    INT32  Fine0                                   :  4;  // Bits 3:0
    INT32  Fine1                                   :  4;  // Bits 7:4
    INT32  Fine2                                   :  4;  // Bits 11:8
    INT32  Fine3                                   :  4;  // Bits 15:12
    INT32  Fine4                                   :  4;  // Bits 19:16
    INT32  Fine5                                   :  4;  // Bits 23:20
    INT32  Fine6                                   :  4;  // Bits 27:24
    INT32  Fine7                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCPILUT0_STRUCT;
typedef union {
  struct {
    UINT32 RefPi                                   :  8;  // Bits 7:0
    UINT32 RefPiOffset                             :  8;  // Bits 15:8
    UINT32 RefPi4Xover                             :  9;  // Bits 24:16
    UINT32 RefPi4XoverOffset                       :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_REFPICONTROL_STRUCT;
typedef union {
  struct {
    UINT32 spare                                   :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 Rsvd                                    :  15;  // Bits 14:0
    UINT32 sparebits                               :  1;  // Bits 15:15
    UINT32 DQPbiasCtl                              :  2;  // Bits 17:16
    UINT32 DQNbiasCtl                              :  2;  // Bits 19:18
    UINT32 DQSPbiasCtl                             :  2;  // Bits 21:20
    UINT32 DQSNbiasCtl                             :  2;  // Bits 23:22
    UINT32 DQSBonus                                :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 dqsmdll_sdlmode                         :  1;  // Bits 0:0
    UINT32 dqsmdll_selphclkdll                     :  1;  // Bits 1:1
    UINT32 dqsmdll_terdcdselrefclk                 :  1;  // Bits 2:2
    INT32  dqsmdll_tlclk_dlldcccode                :  5;  // Bits 7:3
    UINT32 SelWCKDQSPi                             :  2;  // Bits 9:8
    UINT32 rxfifodcccode_sag                       :  2;  // Bits 11:10
    UINT32 phen_ovrd_onsag                         :  1;  // Bits 12:12
    UINT32 DqIdleModeDrvenHigh                     :  1;  // Bits 13:13
    UINT32 DqIdleModeDrvenLow                      :  1;  // Bits 14:14
    UINT32 RxDqsDelaySatPN                         :  8;  // Bits 22:15
    UINT32 RxDdrMode                               :  1;  // Bits 23:23
    UINT32 Rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;
typedef union {
  struct {
    UINT32 i_terdcdselovrval                       :  2;  // Bits 1:0
    UINT32 i_terdcdselovren                        :  1;  // Bits 2:2
    UINT32 i_terclkandselovrval                    :  1;  // Bits 3:3
    UINT32 i_srzdcdpatterninv                      :  1;  // Bits 4:4
    UINT32 i_terdcden                              :  1;  // Bits 5:5
    UINT32 i_clkwckbuf_selterclkandovrval          :  1;  // Bits 6:6
    UINT32 i_ddrdcd_quatdcden_onsag                :  1;  // Bits 7:7
    UINT32 rsvd                                    :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_TERDCCCONTROL0_STRUCT;
typedef union {
  struct {
    UINT32 dq0_xtalksel                            :  3;  // Bits 2:0
    UINT32 dq1_xtalksel                            :  3;  // Bits 5:3
    UINT32 dq2_xtalksel                            :  3;  // Bits 8:6
    UINT32 dq3_xtalksel                            :  3;  // Bits 11:9
    UINT32 dq4_xtalksel                            :  3;  // Bits 14:12
    UINT32 dq5_xtalksel                            :  3;  // Bits 17:15
    UINT32 dq6_xtalksel                            :  3;  // Bits 20:18
    UINT32 dq7_xtalksel                            :  3;  // Bits 23:21
    UINT32 xtalken                                 :  1;  // Bits 24:24
    UINT32 spare                                   :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT;
typedef union {
  struct {
    UINT32 DQ0XtalkCap0                            :  3;  // Bits 2:0
    UINT32 DQ0XtalkCap1                            :  3;  // Bits 5:3
    UINT32 DQ0XtalkGain0                           :  2;  // Bits 7:6
    UINT32 DQ0XtalkGain1                           :  2;  // Bits 9:8
    UINT32 DQ1XtalkCap0                            :  3;  // Bits 12:10
    UINT32 DQ1XtalkCap1                            :  3;  // Bits 15:13
    UINT32 DQ1XtalkGain0                           :  2;  // Bits 17:16
    UINT32 DQ1XtalkGain1                           :  2;  // Bits 19:18
    UINT32 DQ2XtalkCap0                            :  3;  // Bits 22:20
    UINT32 DQ2XtalkCap1                            :  3;  // Bits 25:23
    UINT32 DQ2XtalkGain0                           :  2;  // Bits 27:26
    UINT32 DQ2XtalkGain1                           :  2;  // Bits 29:28
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT;
typedef union {
  struct {
    UINT32 DQ3XtalkCap0                            :  3;  // Bits 2:0
    UINT32 DQ3XtalkCap1                            :  3;  // Bits 5:3
    UINT32 DQ3XtalkGain0                           :  2;  // Bits 7:6
    UINT32 DQ3XtalkGain1                           :  2;  // Bits 9:8
    UINT32 DQ4XtalkCap0                            :  3;  // Bits 12:10
    UINT32 DQ4XtalkCap1                            :  3;  // Bits 15:13
    UINT32 DQ4XtalkGain0                           :  2;  // Bits 17:16
    UINT32 DQ4XtalkGain1                           :  2;  // Bits 19:18
    UINT32 DQ5XtalkCap0                            :  3;  // Bits 22:20
    UINT32 DQ5XtalkCap1                            :  3;  // Bits 25:23
    UINT32 DQ5XtalkGain0                           :  2;  // Bits 27:26
    UINT32 DQ5XtalkGain1                           :  2;  // Bits 29:28
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT;
typedef union {
  struct {
    UINT32 DQ6XtalkCap0                            :  3;  // Bits 2:0
    UINT32 DQ6XtalkCap1                            :  3;  // Bits 5:3
    UINT32 DQ6XtalkGain0                           :  2;  // Bits 7:6
    UINT32 DQ6XtalkGain1                           :  2;  // Bits 9:8
    UINT32 DQ7XtalkCap0                            :  3;  // Bits 12:10
    UINT32 DQ7XtalkCap1                            :  3;  // Bits 15:13
    UINT32 DQ7XtalkGain0                           :  2;  // Bits 17:16
    UINT32 DQ7XtalkGain1                           :  2;  // Bits 19:18
    UINT32 DQ8XtalkCap0                            :  3;  // Bits 22:20
    UINT32 DQ8XtalkCap1                            :  3;  // Bits 25:23
    UINT32 DQ8XtalkGain0                           :  2;  // Bits 27:26
    UINT32 DQ8XtalkGain1                           :  2;  // Bits 29:28
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT;
typedef union {
  struct {
    INT32  TxDqDCCOffsetR0dq0                      :  6;  // Bits 5:0
    INT32  TxDqDCCOffsetR0dq1                      :  6;  // Bits 11:6
    INT32  TxDqDCCOffsetR0dq2                      :  6;  // Bits 17:12
    INT32  TxDqDCCOffsetR0dq3                      :  6;  // Bits 23:18
    INT32  TxDqDCCOffsetR0dq4                      :  6;  // Bits 29:24
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;
typedef union {
  struct {
    INT32  TxDqDCCOffsetR0dq5                      :  6;  // Bits 5:0
    INT32  TxDqDCCOffsetR0dq6                      :  6;  // Bits 11:6
    INT32  TxDqDCCOffsetR0dq7                      :  6;  // Bits 17:12
    INT32  TxDqDCCOffsetR1dq0                      :  6;  // Bits 23:18
    INT32  TxDqDCCOffsetR1dq1                      :  6;  // Bits 29:24
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;
typedef union {
  struct {
    INT32  TxDqDCCOffsetR1dq2                      :  6;  // Bits 5:0
    INT32  TxDqDCCOffsetR1dq3                      :  6;  // Bits 11:6
    INT32  TxDqDCCOffsetR1dq4                      :  6;  // Bits 17:12
    INT32  TxDqDCCOffsetR1dq5                      :  6;  // Bits 23:18
    INT32  TxDqDCCOffsetR1dq6                      :  6;  // Bits 29:24
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;
typedef union {
  struct {
    INT32  TxDqDCCOffsetR1dq7                      :  6;  // Bits 5:0
    INT32  TxDqDCCOffsetR0dqs                      :  7;  // Bits 12:6
    INT32  TxDqDCCOffsetR1dqs                      :  7;  // Bits 19:13
    INT32  TxDqDCCOffsetR0dbi                      :  6;  // Bits 25:20
    INT32  TxDqDCCOffsetR1dbi                      :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;
typedef union {
  struct {
    UINT32 spare                                   : 32;  // Bits 31:0
  } Bits;
UINT32 Data;
UINT16 Data16[2];
UINT8  Data8[4];
} DATA0CH0_CR_SPARE1_STRUCT ;
typedef union {
  struct {
    INT32  FFCodeRxPiOffset                        :  5;  // Bits 4:0
    INT32  FFCodeRxPhEnOffset                      :  5;  // Bits 9:5
    INT32  FFCodeRxLockOffset                      :  5;  // Bits 14:10
    INT32  FFCodeRxWLOffset                        :  5;  // Bits 19:15
    UINT32 rxdll_ffcode_compupdt_ovrd_en           :  1;  // Bits 20:20
    UINT32 rxdll_ffcode_compupdt_ovrd_val          :  1;  // Bits 21:21
    INT32  RloadDqsOffset                          :  5;  // Bits 26:22
    UINT32 rsvd                                    :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT;
typedef union {
  struct {
    UINT32 InitPiCode                              :  10;  // Bits 9:0
    UINT32 ROCount                                 :  16;  // Bits 25:10
    UINT32 Spare                                   :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA0CH0_CR_DDRCRWRRETRAININITRANK0_STRUCT;
typedef union {
  struct {
    UINT32 InitPiCode                              :  11;  // Bits 10:0
    UINT32 ROCount                                 :  16;  // Bits 26:11
    UINT32 Spare                                   :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA0CH0_CR_DDRCRRDRETRAININITRANK0_STRUCT;
typedef union {
  struct {
    UINT32 CtleEn                                  :  1;  // Bits 0:0
    UINT32 CtleCap                                 :  2;  // Bits 2:1
    UINT32 CtleRes                                 :  2;  // Bits 4:3
    UINT32 CtleEq                                  :  3;  // Bits 7:5
    UINT32 CtleRcmn                                :  2;  // Bits 9:8
    UINT32 CtleDcLoad                              :  2;  // Bits 11:10
    UINT32 CtleDcCmn                               :  2;  // Bits 13:12
    UINT32 CtleTailCtl_dq0                         :  2;  // Bits 15:14
    UINT32 CtleTailCtl_dq1                         :  2;  // Bits 17:16
    UINT32 CtleTailCtl_dq2                         :  2;  // Bits 19:18
    UINT32 CtleTailCtl_dq3                         :  2;  // Bits 21:20
    UINT32 CtleTailCtl_dq4                         :  2;  // Bits 23:22
    UINT32 CtleTailCtl_dq5                         :  2;  // Bits 25:24
    UINT32 CtleTailCtl_dq6                         :  2;  // Bits 27:26
    UINT32 CtleTailCtl_dq7                         :  2;  // Bits 29:28
    UINT32 CtleTailCtl_dbi                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRRXCTLE_STRUCT;
typedef union {
  struct {
    UINT32 RxRcvEnPi                               :  11;  // Bits 10:0
    UINT32 RxDqsPDelayPi                           :  7;  // Bits 17:11
    UINT32 RxDqsNDelayPi                           :  7;  // Bits 24:18
    UINT32 Spare                                   :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA0CH0_CR_RXCONTROL0RANK1_STRUCT;
typedef union {
  struct {
    UINT32 CTLECap                                 :  2;  // Bits 1:0
    UINT32 CTLERes                                 :  2;  // Bits 3:2
    UINT32 CTLEEq                                  :  3;  // Bits 6:4
    UINT32 CTLERcmn                                :  2;  // Bits 8:7
    UINT32 CTLEDcLoad                              :  2;  // Bits 10:9
    UINT32 CTLEDcCmn                               :  2;  // Bits 12:11
    UINT32 CTLETailCtl                             :  2;  // Bits 14:13
    UINT32 CTLENBiasCtl                            :  2;  // Bits 16:15
    UINT32 CTLEPBiasCasCtl                         :  2;  // Bits 18:17
    UINT32 Rsvd                                    :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT;
typedef union {
  struct {
    INT32  DqDrvCompOffset                         :  6;  // Bits 5:0
    INT32  DqOdtCompOffset                         :  6;  // Bits 11:6
    UINT32 Dq_rcompcode_compupdt_ovrd_val          :  1;  // Bits 12:12
    UINT32 Dq_rcompcode_compupdt_ovrd_en           :  1;  // Bits 13:13
    UINT32 Dqs_rcompcode_compupdt_ovrd_val         :  1;  // Bits 14:14
    UINT32 Dqs_rcompcode_compupdt_ovrd_en          :  1;  // Bits 15:15
    UINT32 Wck_rcompcode_compupdt_ovrd_val         :  1;  // Bits 16:16
    UINT32 Wck_rcompcode_compupdt_ovrd_en          :  1;  // Bits 17:17
    UINT32 dqstx_eqdlycomp                         :  4;  // Bits 21:18
    UINT32 dqtx_eqdelaycomp                        :  6;  // Bits 27:22
    UINT32 Rsvd                                    :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT;
typedef union {
  struct {
    UINT32 tx_clock_on_with_txanalogen             :  1;  // Bits 0:0
    UINT32 local_gate_d0tx                         :  1;  // Bits 1:1
    UINT32 InternalClocksOn                        :  1;  // Bits 2:2
    UINT32 TxPiOn                                  :  1;  // Bits 3:3
    UINT32 TxOn                                    :  1;  // Bits 4:4
    UINT32 TxDisable                               :  1;  // Bits 5:5
    UINT32 EnDqOdtParkMode                         :  1;  // Bits 6:6
    UINT32 EnDQODTParkHi                           :  1;  // Bits 7:7
    UINT32 EnDqsOdtParkMode                        :  2;  // Bits 9:8
    UINT32 BiasPMCtrl                              :  2;  // Bits 11:10
    UINT32 VrefPMCtrl                              :  1;  // Bits 12:12
    UINT32 RxDisable                               :  1;  // Bits 13:13
    UINT32 ForceRxOn                               :  1;  // Bits 14:14
    UINT32 RxMode                                  :  1;  // Bits 15:15
    UINT32 DqsNParkLowVoh                          :  1;  // Bits 16:16
    UINT32 OdtForceQDrvEn                          :  1;  // Bits 17:17
    UINT32 OdtSampOff                              :  1;  // Bits 18:18
    UINT32 DisableOdtStatic                        :  1;  // Bits 19:19
    UINT32 DdrCRForceODTOn                         :  1;  // Bits 20:20
    UINT32 OdtMode                                 :  2;  // Bits 22:21
    UINT32 RxPwrMuxSelVdd2                         :  1;  // Bits 23:23
    UINT32 RunRefPIFSM                             :  1;  // Bits 24:24
    UINT32 PreDrvOn                                :  1;  // Bits 25:25
    UINT32 DisRxSalOffWhenDrvEnHi                  :  1;  // Bits 26:26
    UINT32 dficmddecode_en                         :  1;  // Bits 27:27
    UINT32 gvblock_blockretrainload_dis            :  1;  // Bits 28:28
    UINT32 dqidlemodedrven                         :  1;  // Bits 29:29
    UINT32 dqdrvstatlegen                          :  1;  // Bits 30:30
    UINT32 dqsidlemodedrven                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT;
typedef union {
  struct {
    UINT32 DisableTxDqs                            :  1;  // Bits 0:0
    UINT32 TxDqsNmosOnly                           :  1;  // Bits 1:1
    UINT32 dqssecterfinalmuxsel                    :  1;  // Bits 2:2
    UINT32 DBIenable                               :  2;  // Bits 4:3
    UINT32 DBIMode                                 :  1;  // Bits 5:5
    UINT32 DqsOdtDelay                             :  5;  // Bits 10:6
    UINT32 DqsOdtDuration                          :  3;  // Bits 13:11
    UINT32 DqOdtDelay                              :  5;  // Bits 18:14
    UINT32 DqOdtDuration                           :  4;  // Bits 22:19
    UINT32 SenseAmpDelay                           :  5;  // Bits 27:23
    UINT32 SenseAmpDuration                        :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 RxXtalk                                 :  1;  // Bits 0:0
    UINT32 DataInvertNibble                        :  2;  // Bits 2:1
    UINT32 RxPiOn                                  :  1;  // Bits 3:3
    UINT32 TxDqNmosOnly                            :  1;  // Bits 4:4
    UINT32 BiasRloadVref                           :  3;  // Bits 7:5
    UINT32 BiasIrefAdj                             :  4;  // Bits 11:8
    UINT32 RXVcmResSel                             :  2;  // Bits 13:12
    UINT32 RXDqsTailCtl                            :  2;  // Bits 15:14
    UINT32 ODTSingleSegEn                          :  1;  // Bits 16:16
    INT32 DqsOdtCompOffset                         :  5;  // Bits 21:17 // Manual edit - 2's complement field
    UINT32 VrefPwrMuxSelVdd2                       :  1;  // Bits 22:22
    UINT32 RxVref                                  :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT;
typedef union {
  struct {
    INT32  RxEqRank0Tap0                           :  6;  // Bits 5:0
    INT32  RxEqRank0Tap1                           :  6;  // Bits 11:6
    INT32  RxEqRank0Tap2                           :  5;  // Bits 16:12
    INT32  RxEqRank0Tap3                           :  5;  // Bits 21:17
    UINT32 dqpadmux_selvddq                        :  2;  // Bits 23:22
    UINT32 Rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT;
typedef union {
  struct {
    INT32  RxEqRank1Tap0                           :  6;  // Bits 5:0
    INT32  RxEqRank1Tap1                           :  6;  // Bits 11:6
    INT32  RxEqRank1Tap2                           :  5;  // Bits 16:12
    INT32  RxEqRank1Tap3                           :  5;  // Bits 21:17
    UINT32 Rsvd                                    :  2;  // Bits 23:22
    UINT32 DqsEqCap                                :  2;  // Bits 25:24
    UINT32 DqsEqRes                                :  2;  // Bits 27:26
    UINT32 DqsEqLegs                               :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRRXEQTAP_STRUCT;
typedef union {
  struct {
    UINT32 DdrDqOvrdModeEn                         :  11;  // Bits 10:0
    UINT32 DdrDqOvrdData                           :  11;  // Bits 21:11
    UINT32 DdrCRNumOfPulses                        :  2;  // Bits 23:22
    UINT32 DdrCRMaskCntPulseNumStart               :  4;  // Bits 27:24
    UINT32 DdrCRWrPostamble                        :  1;  // Bits 28:28
    UINT32 Spare                                   :  2;  // Bits 30:29
    UINT32 ddrdqsovrdtoggle                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT;
typedef union {
  struct {
    INT32  DqTcoCompBit0                           :  6;  // Bits 5:0
    INT32  DqTcoCompBit1                           :  6;  // Bits 11:6
    INT32  DqTcoCompBit2                           :  6;  // Bits 17:12
    INT32  DqTcoCompBit3                           :  6;  // Bits 23:18
    INT32  DqTcoCompBit4                           :  6;  // Bits 29:24
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT;
typedef union {
  struct {
    INT32  DqTcoCompBit5                           :  6;  // Bits 5:0
    INT32  DqTcoCompBit6                           :  6;  // Bits 11:6
    INT32  DqTcoCompBit7                           :  6;  // Bits 17:12
    INT32  DMITcoComp                              :  6;  // Bits 23:18
    UINT32 IOLBCycles                              :  4;  // Bits 27:24
    UINT32 dqmmskipcount                           :  2;  // Bits 29:28
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 InitTrain                               :  1;  // Bits 0:0
    UINT32 Duration                                :  10;  // Bits 10:1
    UINT32 ResetStatus                             :  1;  // Bits 11:11
    UINT32 LargeChangeDelta                        :  3;  // Bits 14:12
    UINT32 updonlargechange                        :  1;  // Bits 15:15
    UINT32 StopOnLargeChange                       :  1;  // Bits 16:16
    UINT32 R0Status                                :  2;  // Bits 18:17
    UINT32 R1Status                                :  2;  // Bits 20:19
    UINT32 R2Status                                :  2;  // Bits 22:21
    UINT32 R3Status                                :  2;  // Bits 24:23
    UINT32 FSMStatus                               :  4;  // Bits 28:25
    UINT32 FSMRank                                 :  2;  // Bits 30:29
    UINT32 LargeChangeTrig                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA0CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 DataRetrain_DQ0                         :  3;  // Bits 2:0
    UINT32 DataRetrain_DQ1                         :  3;  // Bits 5:3
    UINT32 DataRetrain_DQ2                         :  3;  // Bits 8:6
    UINT32 DataRetrain_DQ3                         :  3;  // Bits 11:9
    UINT32 DataRetrain_DQ4                         :  3;  // Bits 14:12
    UINT32 DataRetrain_DQ5                         :  3;  // Bits 17:15
    UINT32 DataRetrain_DQ6                         :  3;  // Bits 20:18
    UINT32 DataRetrain_DQ7                         :  3;  // Bits 23:21
    UINT32 DataRetrain_ByteSel                     :  1;  // Bits 24:24
    UINT32 DataRetrainEn                           :  2;  // Bits 26:25
    UINT32 SerialMRROrder                          :  1;  // Bits 27:27
    UINT32 DataInvertNibble                        :  2;  // Bits 29:28
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;
typedef union {
  struct {
    INT32  DeltaPiCodeR0                           :  7;  // Bits 6:0
    INT32  DeltaPiCodeR1                           :  7;  // Bits 13:7
    INT32  DeltaPiCodeR2                           :  7;  // Bits 20:14
    INT32  DeltaPiCodeR3                           :  7;  // Bits 27:21
    UINT32 Spare                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT;
typedef union {
  struct {
    INT32  DeltaPiCodeR0                           :  8;  // Bits 7:0
    INT32  DeltaPiCodeR1                           :  8;  // Bits 15:8
    INT32  DeltaPiCodeR2                           :  8;  // Bits 23:16
    INT32  DeltaPiCodeR3                           :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT;
typedef union {
  struct {
    UINT32 RankOvrd                                :  1;  // Bits 0:0
    UINT32 RankValue                               :  2;  // Bits 2:1
    UINT32 RxAmpOffsetEn                           :  1;  // Bits 3:3
    UINT32 RxTrainingMode                          :  1;  // Bits 4:4
    UINT32 WLTrainingMode                          :  2;  // Bits 6:5
    UINT32 RLTrainingMode                          :  1;  // Bits 7:7
    UINT32 SenseampTrainingMode                    :  1;  // Bits 8:8
    UINT32 CATrainingMode                          :  1;  // Bits 9:9
    UINT32 CAParityOdd                             :  1;  // Bits 10:10
    UINT32 caparitytrain                           :  1;  // Bits 11:11
    UINT32 LPDDR5                                  :  1;  // Bits 12:12
    UINT32 R2RTrainingMode                         :  4;  // Bits 16:13
    UINT32 DqsRFTrainingMode                       :  1;  // Bits 17:17
    UINT32 En16UIMask                              :  1;  // Bits 18:18
    UINT32 Ddr5_CSTrainingMode                     :  1;  // Bits 19:19
    UINT32 Spare                                   :  2;  // Bits 21:20
    UINT32 DataTrainFeedback                       :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT;
typedef union {
  struct {
    UINT32 LaneDone                                :  10;  // Bits 9:0
    UINT32 RankDone                                :  2;  // Bits 11:10
    UINT32 CalcState                               :  3;  // Bits 14:12
    UINT32 CalcRank                                :  2;  // Bits 16:15
    UINT32 CalcLane                                :  4;  // Bits 20:17
    UINT32 UpdateReady                             :  1;  // Bits 21:21
    UINT32 UpdateDone                              :  1;  // Bits 22:22
    UINT32 LargeChange                             :  1;  // Bits 23:23
    UINT32 DCCReady                                :  1;  // Bits 24:24
    UINT32 rsvd                                    :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCFSMSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 Lane0Result                             :  9;  // Bits 8:0
    UINT32 Lane1Result                             :  9;  // Bits 17:9
    UINT32 Lane2Result                             :  9;  // Bits 26:18
    UINT32 rsvd                                    :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCLANESTATUS0_STRUCT;
typedef union {
  struct {
    UINT32 Lane3Result                             :  9;  // Bits 8:0
    UINT32 Lane4Result                             :  9;  // Bits 17:9
    UINT32 Lane5Result                             :  9;  // Bits 26:18
    UINT32 rsvd                                    :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCLANESTATUS1_STRUCT;
typedef union {
  struct {
    UINT32 Lane6Result                             :  9;  // Bits 8:0
    UINT32 Lane7Result                             :  9;  // Bits 17:9
    UINT32 Lane8Result                             :  9;  // Bits 26:18
    UINT32 rsvd                                    :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCLANESTATUS2_STRUCT;
typedef union {
  struct {
    UINT32 Lane9Result                             :  9;  // Bits 8:0
    UINT32 ExtremeOffset                           :  7;  // Bits 15:9
    UINT32 ExtremeCount                            :  9;  // Bits 24:16
    UINT32 rsvd                                    :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DCCLANESTATUS3_STRUCT;
typedef union {
  struct {
    UINT32 RunTest                                 :  1;  // Bits 0:0
    UINT32 Param                                   :  3;  // Bits 3:1
    INT32  MinVal                                  :  9;  // Bits 12:4
    INT32  MaxVal                                  :  9;  // Bits 21:13
    UINT32 MinWidth                                :  6;  // Bits 27:22
    UINT32 CalcCenter                              :  1;  // Bits 28:28
    UINT32 BurstIdleBurst                          :  1;  // Bits 29:29
    UINT32 LaneResult                              :  1;  // Bits 30:30
    UINT32 EnDQSDrvPark                            :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT;
typedef union {
  struct {
    UINT32 result                                  :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA0CH0_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;
typedef union {
  struct {
    UINT32 DLLReset                                :  1;  // Bits 0:0
    UINT32 DLLReady                                :  1;  // Bits 1:1
    UINT32 CompCompleteOnce                        :  1;  // Bits 2:2
    UINT32 Startup                                 :  1;  // Bits 3:3
    UINT32 DetrmLockStat                           :  1;  // Bits 4:4
    UINT32 FuncLockStat                            :  1;  // Bits 5:5
    UINT32 StickyUnlock                            :  1;  // Bits 6:6
    UINT32 LongLock                                :  1;  // Bits 7:7
    UINT32 WeakLockDone                            :  1;  // Bits 8:8
    UINT32 CompComplete                            :  1;  // Bits 9:9
    UINT32 WeakLock                                :  1;  // Bits 10:10
    UINT32 PDVctlWayTooLow                         :  1;  // Bits 11:11
    UINT32 PDVctlWayTooHi                          :  1;  // Bits 12:12
    UINT32 DllDacCode                              :  11;  // Bits 23:13
    UINT32 Spare                                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 EnCnt                                   :  1;  // Bits 0:0
    UINT32 Duration                                :  5;  // Bits 5:1
    UINT32 SelCnt                                  :  3;  // Bits 8:6
    UINT32 WeightPhPi0                             :  4;  // Bits 12:9
    UINT32 WeightPhPi1                             :  3;  // Bits 15:13
    UINT32 WeightPhPi2                             :  3;  // Bits 18:16
    UINT32 WeightPi0                               :  4;  // Bits 22:19
    UINT32 WeightPi1                               :  2;  // Bits 24:23
    UINT32 WeightPi2                               :  2;  // Bits 26:25
    UINT32 WeightRxDrv                             :  3;  // Bits 29:27
    UINT32 CntStart                                :  1;  // Bits 30:30
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;
typedef union {
  struct {
    UINT32 Count                                   :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;
typedef union {
  struct {
    UINT32 RxDqsDelayP                             :  8;  // Bits 7:0
    UINT32 DqsPTargetNUI                           :  1;  // Bits 8:8
    UINT32 DqsPOffsetNUI                           :  2;  // Bits 10:9
    UINT32 RxDqsDelayN                             :  8;  // Bits 18:11
    UINT32 DqsNTargetNUI                           :  1;  // Bits 19:19
    UINT32 DqsNOffsetNUI                           :  2;  // Bits 21:20
    UINT32 DqsTargetNUI                            :  1;  // Bits 22:22
    UINT32 DqsOffsetNUI                            :  2;  // Bits 24:23
    UINT32 SinStepBinEn                            :  1;  // Bits 25:25
    UINT32 SinStepResumeBin                        :  1;  // Bits 26:26
    UINT32 Spare                                   :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT;
typedef union {
  struct {
    UINT32 ADC_frz                                 :  1;  // Bits 0:0
    UINT32 ADC_en                                  :  1;  // Bits 1:1
    UINT32 ADCstartcount                           :  2;  // Bits 3:2
    UINT32 ADCClkDiv                               :  2;  // Bits 5:4
    UINT32 ADCChopEn                               :  1;  // Bits 6:6
    UINT32 ADCDone                                 :  1;  // Bits 7:7
    UINT32 CountStatus                             :  10;  // Bits 17:8
    UINT32 Spare                                   :  5;  // Bits 22:18
    INT32  VrefOffset                              :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRADC_STRUCT;
typedef union {
  struct {
    UINT32 Param                                   :  2;  // Bits 1:0
    UINT32 MinVal                                  :  8;  // Bits 9:2
    UINT32 MaxVal                                  :  9;  // Bits 18:10
    UINT32 MinWidth                                :  6;  // Bits 24:19
    UINT32 TestCycle                               :  3;  // Bits 27:25
    UINT32 ForceWrongPhase                         :  1;  // Bits 28:28
    UINT32 WriteValParam                           :  1;  // Bits 29:29
    UINT32 SaveModN                                :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH0_CR_DDRDATADQRANK1LANE8_STRUCT;
typedef union {
  struct {
    UINT32 RxVref0                                 :  10;  // Bits 9:0
    UINT32 RxVref1                                 :  10;  // Bits 19:10
    UINT32 RxVref2                                 :  10;  // Bits 29:20
    UINT32 Rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT;
typedef union {
  struct {
    UINT32 RxVref3                                 :  10;  // Bits 9:0
    UINT32 RxVref4                                 :  10;  // Bits 19:10
    UINT32 RxVref5                                 :  10;  // Bits 29:20
    UINT32 Rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT;
typedef union {
  struct {
    UINT32 RxVref6                                 :  10;  // Bits 9:0
    UINT32 RxVref7                                 :  10;  // Bits 19:10
    UINT32 RxVrefDBI                               :  10;  // Bits 29:20
    UINT32 Rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA0CH1_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA0CH1_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA0CH1_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA0CH1_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA0CH1_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA0CH1_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA0CH1_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA0CH1_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA0CH1_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA0CH1_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA0CH1_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA0CH1_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA0CH1_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA0CH1_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA0CH1_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA0CH1_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA0CH1_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA0CH1_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA0CH1_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA0CH1_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA0CH1_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA0CH1_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA0CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA0CH1_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA0CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA0CH1_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA0CH1_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA0CH1_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA0CH1_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA0CH1_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA0CH1_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA0CH1_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA0CH1_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA0CH1_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA0CH1_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA0CH1_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA0CH1_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA0CH1_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA0CH1_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA0CH1_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA0CH1_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA0CH1_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA0CH1_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA0CH1_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA0CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA0CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA0CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA0CH1_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA0CH1_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA0CH1_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA0CH1_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA0CH1_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA0CH1_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA0CH1_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA0CH1_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA0CH1_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA0CH1_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA0CH1_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA0CH1_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA0CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA0CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA0CH1_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA0CH1_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA0CH1_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA0CH1_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA0CH1_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA0CH1_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA0CH1_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA1CH0_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA1CH0_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA1CH0_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA1CH0_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA1CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA1CH0_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA1CH0_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA1CH0_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA1CH0_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA1CH0_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA1CH0_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA1CH0_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA1CH0_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA1CH0_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA1CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA1CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA1CH0_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA1CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA1CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA1CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA1CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA1CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA1CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA1CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA1CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA1CH0_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA1CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA1CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA1CH0_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA1CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA1CH0_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA1CH0_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA1CH0_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA1CH0_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA1CH0_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA1CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA1CH0_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA1CH0_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA1CH0_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA1CH0_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA1CH0_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA1CH0_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA1CH0_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA1CH0_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA1CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA1CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA1CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA1CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA1CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA1CH0_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA1CH0_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA1CH0_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA1CH0_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA1CH0_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA1CH0_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA1CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA1CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA1CH0_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA1CH0_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA1CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA1CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA1CH0_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA1CH0_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA1CH0_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH0_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA1CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA1CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA1CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA1CH1_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA1CH1_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA1CH1_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA1CH1_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA1CH1_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA1CH1_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA1CH1_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA1CH1_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA1CH1_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA1CH1_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA1CH1_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA1CH1_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA1CH1_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA1CH1_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA1CH1_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA1CH1_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA1CH1_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA1CH1_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA1CH1_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA1CH1_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA1CH1_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA1CH1_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA1CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA1CH1_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA1CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA1CH1_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA1CH1_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA1CH1_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA1CH1_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA1CH1_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA1CH1_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA1CH1_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA1CH1_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA1CH1_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA1CH1_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA1CH1_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA1CH1_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA1CH1_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA1CH1_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA1CH1_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA1CH1_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA1CH1_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA1CH1_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA1CH1_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA1CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA1CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA1CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA1CH1_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA1CH1_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA1CH1_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA1CH1_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA1CH1_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA1CH1_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA1CH1_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA1CH1_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA1CH1_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA1CH1_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA1CH1_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA1CH1_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA1CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA1CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA1CH1_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA1CH1_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA1CH1_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA1CH1_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA1CH1_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA1CH1_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA1CH1_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA2CH0_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA2CH0_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA2CH0_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA2CH0_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA2CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA2CH0_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA2CH0_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA2CH0_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA2CH0_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA2CH0_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA2CH0_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA2CH0_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA2CH0_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA2CH0_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA2CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA2CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA2CH0_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA2CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA2CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA2CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA2CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA2CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA2CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA2CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA2CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA2CH0_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA2CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA2CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA2CH0_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA2CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA2CH0_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA2CH0_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA2CH0_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA2CH0_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA2CH0_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA2CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA2CH0_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA2CH0_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA2CH0_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA2CH0_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA2CH0_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA2CH0_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA2CH0_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA2CH0_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA2CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA2CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA2CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA2CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA2CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA2CH0_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA2CH0_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA2CH0_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA2CH0_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA2CH0_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA2CH0_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA2CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA2CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA2CH0_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA2CH0_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA2CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA2CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA2CH0_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA2CH0_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA2CH0_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH0_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA2CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA2CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA2CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA2CH1_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA2CH1_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA2CH1_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA2CH1_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA2CH1_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA2CH1_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA2CH1_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA2CH1_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA2CH1_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA2CH1_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA2CH1_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA2CH1_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA2CH1_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA2CH1_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA2CH1_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA2CH1_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA2CH1_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA2CH1_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA2CH1_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA2CH1_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA2CH1_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA2CH1_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA2CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA2CH1_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA2CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA2CH1_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA2CH1_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA2CH1_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA2CH1_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA2CH1_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA2CH1_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA2CH1_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA2CH1_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA2CH1_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA2CH1_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA2CH1_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA2CH1_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA2CH1_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA2CH1_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA2CH1_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA2CH1_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA2CH1_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA2CH1_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA2CH1_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA2CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA2CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA2CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA2CH1_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA2CH1_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA2CH1_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA2CH1_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA2CH1_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA2CH1_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA2CH1_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA2CH1_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA2CH1_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA2CH1_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA2CH1_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA2CH1_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA2CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA2CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA2CH1_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA2CH1_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA2CH1_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA2CH1_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA2CH1_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA2CH1_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA2CH1_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA3CH0_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA3CH0_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA3CH0_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA3CH0_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA3CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA3CH0_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA3CH0_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA3CH0_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA3CH0_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA3CH0_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA3CH0_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA3CH0_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA3CH0_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA3CH0_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA3CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA3CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA3CH0_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA3CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA3CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA3CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA3CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA3CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA3CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA3CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA3CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA3CH0_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA3CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA3CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA3CH0_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA3CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA3CH0_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA3CH0_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA3CH0_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA3CH0_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA3CH0_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA3CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA3CH0_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA3CH0_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA3CH0_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA3CH0_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA3CH0_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA3CH0_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA3CH0_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA3CH0_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA3CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA3CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA3CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA3CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA3CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA3CH0_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA3CH0_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA3CH0_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA3CH0_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA3CH0_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA3CH0_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA3CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA3CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA3CH0_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA3CH0_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA3CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA3CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA3CH0_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA3CH0_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA3CH0_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH0_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA3CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA3CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA3CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA3CH1_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA3CH1_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA3CH1_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA3CH1_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA3CH1_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA3CH1_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA3CH1_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA3CH1_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA3CH1_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA3CH1_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA3CH1_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA3CH1_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA3CH1_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA3CH1_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA3CH1_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA3CH1_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA3CH1_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA3CH1_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA3CH1_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA3CH1_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA3CH1_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA3CH1_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA3CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA3CH1_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA3CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA3CH1_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA3CH1_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA3CH1_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA3CH1_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA3CH1_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA3CH1_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA3CH1_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA3CH1_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA3CH1_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA3CH1_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA3CH1_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA3CH1_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA3CH1_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA3CH1_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA3CH1_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA3CH1_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA3CH1_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA3CH1_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA3CH1_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA3CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA3CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA3CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA3CH1_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA3CH1_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA3CH1_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA3CH1_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA3CH1_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA3CH1_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA3CH1_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA3CH1_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA3CH1_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA3CH1_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA3CH1_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA3CH1_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA3CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA3CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA3CH1_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA3CH1_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA3CH1_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA3CH1_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA3CH1_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA3CH1_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA3CH1_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA4CH0_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA4CH0_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA4CH0_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA4CH0_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA4CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA4CH0_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA4CH0_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA4CH0_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA4CH0_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA4CH0_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA4CH0_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA4CH0_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA4CH0_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA4CH0_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA4CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA4CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA4CH0_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA4CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA4CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA4CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA4CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA4CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA4CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA4CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA4CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA4CH0_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA4CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA4CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA4CH0_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA4CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA4CH0_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA4CH0_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA4CH0_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA4CH0_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA4CH0_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA4CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA4CH0_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA4CH0_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA4CH0_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA4CH0_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA4CH0_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA4CH0_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA4CH0_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA4CH0_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA4CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA4CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA4CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA4CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA4CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA4CH0_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA4CH0_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA4CH0_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA4CH0_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA4CH0_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA4CH0_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA4CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA4CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA4CH0_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA4CH0_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA4CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA4CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA4CH0_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA4CH0_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA4CH0_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH0_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA4CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA4CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA4CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA4CH1_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA4CH1_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA4CH1_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA4CH1_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA4CH1_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA4CH1_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA4CH1_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA4CH1_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA4CH1_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA4CH1_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA4CH1_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA4CH1_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA4CH1_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA4CH1_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA4CH1_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA4CH1_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA4CH1_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA4CH1_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA4CH1_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA4CH1_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA4CH1_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA4CH1_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA4CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA4CH1_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA4CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA4CH1_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA4CH1_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA4CH1_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA4CH1_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA4CH1_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA4CH1_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA4CH1_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA4CH1_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA4CH1_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA4CH1_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA4CH1_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA4CH1_CR_DDRCRDATACONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL1_STRUCT DATA4CH1_CR_DDRCRDATACONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL5_STRUCT DATA4CH1_CR_DDRCRDATACONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP01_STRUCT DATA4CH1_CR_DDRCRRXEQTAP01_STRUCT;

typedef DATA0CH0_CR_DDRCRRXEQTAP_STRUCT DATA4CH1_CR_DDRCRRXEQTAP_STRUCT;

typedef DATA0CH0_CR_DDRCRCMDBUSTRAIN_STRUCT DATA4CH1_CR_DDRCRCMDBUSTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL0_STRUCT DATA4CH1_CR_DDRCRDATATCOCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDATATCOCONTROL1_STRUCT DATA4CH1_CR_DDRCRDATATCOCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA4CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_STRUCT DATA4CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT DATA4CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_STRUCT DATA4CH1_CR_DDRCRWRRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_STRUCT DATA4CH1_CR_DDRCRRDRETRAINDELTARANK_STRUCT;

typedef DATA0CH0_CR_DATATRAINFEEDBACK_STRUCT DATA4CH1_CR_DATATRAINFEEDBACK_STRUCT;

typedef DATA0CH0_CR_DCCFSMSTATUS_STRUCT DATA4CH1_CR_DCCFSMSTATUS_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS0_STRUCT DATA4CH1_CR_DCCLANESTATUS0_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS1_STRUCT DATA4CH1_CR_DCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS2_STRUCT DATA4CH1_CR_DCCLANESTATUS2_STRUCT;

typedef DATA0CH0_CR_DCCLANESTATUS3_STRUCT DATA4CH1_CR_DCCLANESTATUS3_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODECONTROL0_STRUCT DATA4CH1_CR_DDRCRMARGINMODECONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA4CH1_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DATA4CH1_CR_DDRCRMARGINMODEDEBUGLSB0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DATA4CH1_CR_DDRCRDLLSTATUS_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT DATA4CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DATA4CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRCOMPDQSDELAY_STRUCT DATA4CH1_CR_DDRCRCOMPDQSDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DATA4CH1_CR_DDRCRADC_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT DATA4CH1_CR_REFPIFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK0LANE8_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA4CH1_CR_DDRDATADQRANK1LANE8_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT012_STRUCT DATA4CH1_CR_DDRCRDATAVREFPERBIT012_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT345_STRUCT DATA4CH1_CR_DDRCRDATAVREFPERBIT345_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT DATA4CH1_CR_DDRCRDATAVREFPERBIT67DBI_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK0LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK0LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK0LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK0LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK0LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK0LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK0LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK0LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK1LANE0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK1LANE1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK1LANE2_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK1LANE3_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK1LANE4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK1LANE5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK1LANE6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQRANK0LANE0_STRUCT DATA5CH0_CR_DDRDATADQRANK1LANE7_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA5CH0_CR_DDRDATADQSRANK0_STRUCT;

typedef DATA0CH0_CR_DDRDATADQSRANK0_STRUCT DATA5CH0_CR_DDRDATADQSRANK1_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA5CH0_CR_TXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_TXCONTROL0RANK0_STRUCT DATA5CH0_CR_TXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DATA5CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL2_STRUCT DATA5CH0_CR_DDRCRDATACONTROL2_STRUCT;

typedef DATA0CH0_CR_DCCFSMCONTROL_STRUCT DATA5CH0_CR_DCCFSMCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCCALCCONTROL_STRUCT DATA5CH0_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_DCCLANETARGET_STRUCT DATA5CH0_CR_DCCLANETARGET_STRUCT;

typedef DATA0CH0_CR_DDRCRRANKMUXDELAY_STRUCT DATA5CH0_CR_DDRCRRANKMUXDELAY_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT DATA5CH0_CR_DDRCRDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DCCPILUT0_STRUCT DATA5CH0_CR_DCCPILUT0_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT DATA5CH0_CR_REFPICONTROL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DATA5CH0_CR_SPARE_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT DATA5CH0_CR_DDRCRXTALKBIASCONTROL1_STRUCT;

typedef DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT DATA5CH0_CR_DDRCRDQSWCKDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT DATA5CH0_CR_TERDCCCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT DATA5CH0_CR_DDRCRXTALKBIASCONTROL2_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT DATA5CH0_CR_DDRCRXTALKBIASCONTROL3_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT DATA5CH0_CR_DDRCRXTALKBIASCONTROL4_STRUCT;

typedef DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT DATA5CH0_CR_DDRCRXTALKBIASCONTROL5_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT DATA5CH0_CR_DDRDATADQDCCRANK0LANE0TO4_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT DATA5CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT DATA5CH0_CR_DDRDATADQDCCRANK1LANE2TO6_STRUCT;

typedef DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT DATA5CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_STRUCT;

typedef DATA0CH0_CR_SPARE1_STRUCT DATA5CH0_CR_SPARE1_STRUCT;

typedef DATA0CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT DATA5CH0_CR_VCCCLKCOMPRXOFFSET_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA5CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRWRRETRAININITRANK1_STRUCT DATA5CH0_CR_DDRCRWRRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA5CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT;

typedef DATA0CH0_CR_DDRCRRDRETRAININITRANK1_STRUCT DATA5CH0_CR_DDRCRRDRETRAININITRANK0_STRUCT;

typedef DATA0CH0_CR_DDRCRRXCTLE_STRUCT DATA5CH0_CR_DDRCRRXCTLE_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA5CH0_CR_RXCONTROL0RANK0_STRUCT;

typedef DATA0CH0_CR_RXCONTROL0RANK0_STRUCT DATA5CH0_CR_RXCONTROL0RANK1_STRUCT;

typedef DATA0CH0_CR_DDRDQREPLICACTLE_STRUCT DATA5CH0_CR_DDRDQREPLICACTLE_STRUCT;

typedef DATA0CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT DATA5CH0_CR_DDRCRDATAOFFSETCOMP_STRUCT;

typedef DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT DATA5CH0_CR_DDRCRDATACONTROL0_STRUCT;



#pragma pack(pop)
#endif /* __MrcRegisterStructPtl0xxx_h__ */







