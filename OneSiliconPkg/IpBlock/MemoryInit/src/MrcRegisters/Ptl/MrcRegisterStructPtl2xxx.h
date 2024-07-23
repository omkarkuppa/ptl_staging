#ifndef __MrcRegisterStructPtl2xxx_h__
#define __MrcRegisterStructPtl2xxx_h__

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


typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR3_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR3_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR3_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR3_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR4_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR4_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR4_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR4_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR5_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR5_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR5_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR5_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR6_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR6_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR6_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR6_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR7_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR7_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR7_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR7_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR8_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR8_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR8_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR8_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR9_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR9_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR9_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR9_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGTERMCR1_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGTERMCR1_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGTERMCR1_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGTERMCR1_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR1B_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR1B_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR1B_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR1B_CR_TLINETERMINATION_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPGCR7B_CR_LVRVOLTAGESENSOR_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPGCR7B_CR_LVRVOLTAGESENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPGCR7B_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_TLINETERMINATION_STRUCT DDRPGCR7B_CR_TLINETERMINATION_STRUCT;
typedef union {
  struct {
    UINT32 RcompOdtUp                              :  6;  // Bits 5:0
    UINT32 RcompOdtDown                            :  6;  // Bits 11:6
    UINT32 RcompDrvUp                              :  6;  // Bits 17:12
    UINT32 RcompDrvDown                            :  6;  // Bits 23:18
    UINT32 Rloadcomp                               :  5;  // Bits 28:24
    UINT32 Spare                                   :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT;
typedef union {
  struct {
    UINT32 Scomp                                   :  6;  // Bits 5:0
    UINT32 RcompDrvUp                              :  6;  // Bits 11:6
    UINT32 RcompDrvDown                            :  6;  // Bits 17:12
    UINT32 Rloadcomp                               :  5;  // Bits 22:18
    UINT32 Spare                                   :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT DDRPHY_DDRCOMP_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT DDRPHY_DDRCOMP_CR_DDRCRCLKCOMP_STRUCT;
typedef union {
  struct {
    UINT32 EnVttOdt                                :  1;  // Bits 0:0
    UINT32 DisableFallingPD                        :  1;  // Bits 1:1
    UINT32 sinstepadvbin                           :  1;  // Bits 2:2
    UINT32 sinstepbin                              :  1;  // Bits 3:3
    UINT32 rcompvrefenable_ovrd                    :  1;  // Bits 4:4
    UINT32 spare1                                  :  1;  // Bits 5:5
    UINT32 spare2                                  :  1;  // Bits 6:6
    UINT32 TxCaNmosOnly                            :  1;  // Bits 7:7
    UINT32 TxCtlNmosOnly                           :  1;  // Bits 8:8
    UINT32 TxClkNmosOnly                           :  1;  // Bits 9:9
    UINT32 cccstaticlegctl                         :  1;  // Bits 10:10
    UINT32 TxDqNmosOnly                            :  1;  // Bits 11:11
    UINT32 DqOdtVrefDn                             :  8;  // Bits 19:12
    UINT32 DqOdtVrefUp                             :  8;  // Bits 27:20
    UINT32 ForceLVROn                              :  1;  // Bits 28:28
    UINT32 Spare                                   :  2;  // Bits 30:29
    UINT32 RunRefPIFSM                             :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL0_STRUCT;
typedef union {
  struct {
    UINT32 cmdscompcells                           :  4;  // Bits 3:0
    UINT32 vddqcompvrefsel                         :  3;  // Bits 6:4
    UINT32 vddqcompen_ovrdval                      :  1;  // Bits 7:7
    UINT32 vddqcompen_ovrden                       :  1;  // Bits 8:8
    UINT32 dqsrloadenable_ovrdval                  :  1;  // Bits 9:9
    UINT32 DisableCompRotate                       :  1;  // Bits 10:10
    UINT32 GearMode                                :  1;  // Bits 11:11
    UINT32 CompClkOn                               :  1;  // Bits 12:12
    UINT32 RcompVrefVdd2                           :  1;  // Bits 13:13
    UINT32 rloadcompcodelive_ovrdval               :  6;  // Bits 19:14
    UINT32 rloadcompcodelive_ovrden                :  1;  // Bits 20:20
    UINT32 ddrrcomp_cccstaticlegctrl               :  1;  // Bits 21:21
    UINT32 rloadnumsegs                            :  4;  // Bits 25:22
    UINT32 ddrrcomp_dqsrloadenqnnnh                :  1;  // Bits 26:26
    UINT32 ddrrcomp_dqodtpupenqnnnh                :  1;  // Bits 27:27
    UINT32 lpddr5modeen                            :  1;  // Bits 28:28
    UINT32 dqodtpupenable_ovrdval                  :  1;  // Bits 29:29
    UINT32 SinStep                                 :  1;  // Bits 30:30
    UINT32 SinStepAdv                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL1_STRUCT;
typedef union {
  struct {
    UINT32 CmdDrvVrefUp                            :  8;  // Bits 7:0
    UINT32 CtlDrvVrefUp                            :  8;  // Bits 15:8
    UINT32 ClkDrvVrefUp                            :  8;  // Bits 23:16
    UINT32 DqDrvVrefUp                             :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL2_STRUCT;
typedef union {
  struct {
    UINT32 PanicVttDn0                             :  8;  // Bits 7:0
    UINT32 PanicVttDn1                             :  8;  // Bits 15:8
    UINT32 PanicVttUp0                             :  8;  // Bits 23:16
    UINT32 PanicVttUp1                             :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRDATACOMPVTT_STRUCT;
typedef union {
  struct {
    UINT32 CmdDrvVrefDn                            :  8;  // Bits 7:0
    UINT32 CtlDrvVrefDn                            :  8;  // Bits 15:8
    UINT32 ClkDrvVrefDn                            :  8;  // Bits 23:16
    UINT32 DqDrvVrefDn                             :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL3_STRUCT;
typedef union {
  struct {
    UINT32 LeakCodePhDrvG                          :  7;  // Bits 6:0
    UINT32 LeakCodePiCCC                           :  7;  // Bits 13:7
    UINT32 RcompDrvUpVdd2                          :  6;  // Bits 19:14
    UINT32 RcompDrvDownVdd2                        :  6;  // Bits 25:20
    UINT32 spare                                   :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPTEMP_STRUCT;
typedef union {
  struct {
    UINT32 ViewDrvUp                               :  6;  // Bits 5:0
    UINT32 ViewDrvDn                               :  6;  // Bits 11:6
    UINT32 Spare                                   :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCOMPDATA_STRUCT;
typedef union {
  struct {
    UINT32 CompInitDelay                           :  3;  // Bits 2:0
    UINT32 CompStageSwitch                         :  3;  // Bits 5:3
    UINT32 CompCodeSwitch                          :  3;  // Bits 8:6
    UINT32 CompSamples                             :  3;  // Bits 11:9
    UINT32 ODTSingleSegEn                          :  1;  // Bits 12:12
    UINT32 BiasVrefSel                             :  3;  // Bits 15:13
    UINT32 BiasIrefAdj                             :  4;  // Bits 19:16
    UINT32 RXVcmResSel                             :  2;  // Bits 21:20
    UINT32 RXTailCtl                               :  2;  // Bits 23:22
    UINT32 PhaseDrvPiPMEn                          :  2;  // Bits 25:24
    UINT32 DramReset                               :  1;  // Bits 26:26
    UINT32 DisableOdtStatic                        :  1;  // Bits 27:27
    UINT32 cccpidiv                                :  1;  // Bits 28:28
    UINT32 BwselCompClkOn                          :  1;  // Bits 29:29
    UINT32 EnableBwselComp                         :  1;  // Bits 30:30
    UINT32 spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL4_STRUCT;
typedef union {
  struct {
    UINT32 CompVTarget                             :  8;  // Bits 7:0
    UINT32 OCDivider                               :  1;  // Bits 8:8
    UINT32 CodeLeakSearch                          :  1;  // Bits 9:9
    UINT32 LocalVsxHiBypass                        :  1;  // Bits 10:10
    UINT32 PbiasCalibration                        :  3;  // Bits 13:11
    UINT32 LeakScale                               :  3;  // Bits 16:14
    UINT32 CbMix                                   :  2;  // Bits 18:17
    UINT32 CbMux                                   :  4;  // Bits 22:19
    UINT32 EnableOversamplePi                      :  1;  // Bits 23:23
    UINT32 DCABW                                   :  2;  // Bits 25:24
    UINT32 PICode                                  :  5;  // Bits 30:26
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCCLKTARGET_STRUCT;
typedef union {
  struct {
    UINT32 FFCodePhDrvG                            :  6;  // Bits 5:0
    UINT32 PanicVttDnRd                            :  7;  // Bits 12:6
    UINT32 Spare                                   :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCCLKPG_STRUCT;
typedef union {
  struct {
    UINT32 FFCodePiES                              :  6;  // Bits 5:0
    UINT32 FFCodePiRef                             :  6;  // Bits 11:6
    UINT32 FFCodeTxPhEn                            :  6;  // Bits 17:12
    UINT32 FFCodeTxLock                            :  6;  // Bits 23:18
    UINT32 FFCodeTxWL                              :  6;  // Bits 29:24
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCCLKCOMPDATACCC_STRUCT;
typedef union {
  struct {
    UINT32 FFCodeRxPi                              :  6;  // Bits 5:0
    UINT32 FFCodeRxPhEn                            :  6;  // Bits 11:6
    UINT32 FFCodeRxLock                            :  6;  // Bits 17:12
    UINT32 FFCodeRxWL                              :  6;  // Bits 23:18
    UINT32 rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCCLKCOMPDATA_STRUCT;
typedef union {
  struct {
    UINT32 FFCodePbiasCal                          :  6;  // Bits 5:0
    UINT32 rsvd                                    :  26;  // Bits 31:6
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCCLKCOMPTEMPCODE_STRUCT;
typedef union {
  struct {
    UINT32 FLLCalEnSrc                             :  1;  // Bits 0:0
    UINT32 FLLCalRatioSrc                          :  1;  // Bits 1:1
    UINT32 ForceClkReq                             :  1;  // Bits 2:2
    UINT32 spare1                                  :  2;  // Bits 4:3
    UINT32 FllWireRatio                            :  8;  // Bits 12:5
    UINT32 FllWireCalCode                          :  16;  // Bits 28:13
    UINT32 FllWireCodeVld                          :  1;  // Bits 29:29
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRFLLWIRED_STRUCT;
typedef union {
  struct {
    UINT32 CapCancelCodePhDrvG                     :  4;  // Bits 3:0
    UINT32 CapCancelCodePhDrvPi                    :  4;  // Bits 7:4
    UINT32 CapCancelCodeRxDrv                      :  4;  // Bits 11:8
    UINT32 CapCancelCodePiData                     :  4;  // Bits 15:12
    UINT32 CapCancelCodePiCCC                      :  4;  // Bits 19:16
    UINT32 CapCancelCodePiRead                     :  4;  // Bits 23:20
    UINT32 CapCancelCodeTxDll                      :  4;  // Bits 27:24
    UINT32 CapCancelCodeRxDll                      :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRVCCCLKCOUPLINGCAP_STRUCT;
typedef union {
  struct {
    UINT32 FFCodePhDrvGOffset                      :  5;  // Bits 4:0
    UINT32 FFCodePhDrvPiOffset                     :  5;  // Bits 9:5
    UINT32 FFCodeRxDrvOffset                       :  5;  // Bits 14:10
    UINT32 FFCodePiDataOffset                      :  5;  // Bits 19:15
    UINT32 FFCodePiCCCOffset                       :  5;  // Bits 24:20
    UINT32 FFCodePiReadOffset                      :  5;  // Bits 29:25
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRVCCCLKCOMPOFFSET_STRUCT;
typedef union {
  struct {
    INT32  Ph0Delay                                :  5;  // Bits 4:0
    INT32  Ph1Delay                                :  5;  // Bits 9:5
    INT32  Ph2Delay                                :  5;  // Bits 14:10
    INT32  Ph3Delay                                :  5;  // Bits 19:15
    UINT32 PhaseDrvGPMEn                           :  2;  // Bits 21:20
    UINT32 DccQClkgateDisable                      :  1;  // Bits 22:22
    UINT32 rsvd                                    :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER0_STRUCT;
typedef union {
  struct {
    INT32  Ph0DCC                                  :  6;  // Bits 5:0
    INT32  Ph1DCC                                  :  6;  // Bits 11:6
    INT32  Ph2DCC                                  :  6;  // Bits 17:12
    INT32  Ph3DCC                                  :  6;  // Bits 23:18
    INT32  QClkDCC                                 :  6;  // Bits 29:24
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT;
typedef union {
  struct {
    UINT32 LeakCodePhDrvPi                         :  7;  // Bits 6:0
    UINT32 LeakCodeRxDrv                           :  7;  // Bits 13:7
    UINT32 LeakCodePiData                          :  7;  // Bits 20:14
    UINT32 LeakCodePiRead                          :  7;  // Bits 27:21
    UINT32 spare                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRVCCCLKTEMP_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_STRUCT DDRPHY_DDRCOMP_CR_LVRVOLTAGESENSOR_STRUCT;
typedef union {
  struct {
    UINT32 QClkDelayVccCLK                         :  7;  // Bits 6:0
    UINT32 PHDelayVccSA                            :  7;  // Bits 13:7
    UINT32 rsvd                                    :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_WRONGPHASECONTROL_STRUCT;
typedef union {
  struct {
    UINT32 DqDrvU                                  :  1;  // Bits 0:0
    UINT32 DqDrvD                                  :  1;  // Bits 1:1
    UINT32 DqOdtU                                  :  1;  // Bits 2:2
    UINT32 DqOdtD                                  :  1;  // Bits 3:3
    UINT32 CmdDrvU                                 :  1;  // Bits 4:4
    UINT32 CmdDrvD                                 :  1;  // Bits 5:5
    UINT32 CtlDrvU                                 :  1;  // Bits 6:6
    UINT32 CtlDrvD                                 :  1;  // Bits 7:7
    UINT32 ClkDrvU                                 :  1;  // Bits 8:8
    UINT32 ClkDrvD                                 :  1;  // Bits 9:9
    UINT32 rsvd                                    :  2;  // Bits 11:10
    UINT32 rloadcomp                               :  1;  // Bits 12:12
    UINT32 cmdsr                                   :  1;  // Bits 13:13
    UINT32 ffenrxpi                                :  1;  // Bits 14:14
    UINT32 ffenrxphen                              :  1;  // Bits 15:15
    UINT32 ffenrxlock                              :  1;  // Bits 16:16
    UINT32 ffenrxwl                                :  1;  // Bits 17:17
    UINT32 ffenpies                                :  1;  // Bits 18:18
    UINT32 ffenpiref                               :  1;  // Bits 19:19
    UINT32 ffentxphen                              :  1;  // Bits 20:20
    UINT32 ffentxwl                                :  1;  // Bits 21:21
    UINT32 ffenpbiascal                            :  1;  // Bits 22:22
    UINT32 Spare                                   :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPOVR0_STRUCT;
typedef union {
  struct {
    UINT32 EqDelay                                 :  6;  // Bits 5:0
    UINT32 EqCoeff                                 :  5;  // Bits 10:6
    UINT32 TcoComp0                                :  6;  // Bits 16:11
    UINT32 TcoComp1                                :  6;  // Bits 22:17
    UINT32 WrEnViewDrv                             :  1;  // Bits 23:23
    UINT32 drven                                   :  1;  // Bits 24:24
    UINT32 drv2xsen                                :  1;  // Bits 25:25
    UINT32 rcompviewanaouten                       :  1;  // Bits 26:26
    UINT32 spare                                   :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRVIEWCTL_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPHY_DDRCOMP_CR_SPARE_STRUCT;

typedef DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_STRUCT DDRPHY_DDRCOMP_CR_LVRVOLTAGESENSORCOUNT_STRUCT;
typedef union {
  struct {
    UINT32 EnSensor                                :  1;  // Bits 0:0
    UINT32 EnCnt                                   :  1;  // Bits 1:1
    UINT32 Duration                                :  5;  // Bits 6:2
    UINT32 EnChop                                  :  1;  // Bits 7:7
    UINT32 CntStart                                :  2;  // Bits 9:8
    UINT32 SelLVR                                  :  3;  // Bits 12:10
    UINT32 AmpHyster                               :  4;  // Bits 16:13
    UINT32 Spare                                   :  15;  // Bits 31:17
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRPHY_DDRCOMP_CR_LVRCURRENTSENSORCOUNT_STRUCT;
typedef union {
  struct {
    UINT32 DCCStepSize                             :  5;  // Bits 4:0
    INT32  QClkTarget                              :  4;  // Bits 8:5
    INT32  PHClkTarget                             :  4;  // Bits 12:9
    UINT32 QClkEn                                  :  1;  // Bits 13:13
    UINT32 PHClkDutyCycleEn                        :  1;  // Bits 14:14
    UINT32 PHClkPhaseEn                            :  1;  // Bits 15:15
    UINT32 DCCSamples                              :  5;  // Bits 20:16
    UINT32 SaveFullDCC                             :  1;  // Bits 21:21
    UINT32 SkipCRWrite                             :  1;  // Bits 22:22
    UINT32 ForceAck                                :  1;  // Bits 23:23
    UINT32 LargeChange                             :  3;  // Bits 26:24
    UINT32 LargeChangeEnable                       :  1;  // Bits 27:27
    UINT32 LargeChangeReset                        :  1;  // Bits 28:28
    UINT32 rsvd                                    :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 Ph0Result                               :  9;  // Bits 8:0
    UINT32 Ph1Result                               :  9;  // Bits 17:9
    UINT32 Ph2Result                               :  9;  // Bits 26:18
    UINT32 rsvd                                    :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT;
typedef union {
  struct {
    UINT32 Ph3Result                               :  9;  // Bits 8:0
    UINT32 QclkResult                              :  9;  // Bits 17:9
    UINT32 LargeChangePh                           :  1;  // Bits 18:18
    UINT32 SampleDone                              :  1;  // Bits 19:19
    UINT32 UpdateDone                              :  1;  // Bits 20:20
    UINT32 LargeChangeDCC                          :  1;  // Bits 21:21
    UINT32 DCCReady                                :  1;  // Bits 22:22
    UINT32 rsvd                                    :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT DDRPHY_DDRCOMP_CR_DDRCRADC_STRUCT;
typedef union {
  struct {
    INT32  TxVctlStartUpOffset                     :  6;  // Bits 5:0
    INT32  RxVctlStartUpOffset                     :  6;  // Bits 11:6
    INT32  TxVctlFastRelockOffset                  :  6;  // Bits 17:12
    INT32  RxVctlFastRelockOffset                  :  6;  // Bits 23:18
    UINT32 Rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 compcodestage2switch                    :  3;  // Bits 2:0
    UINT32 compcodestage3switch                    :  3;  // Bits 5:3
    UINT32 compcodestage4switch                    :  3;  // Bits 8:6
    UINT32 compcodestage5switch                    :  3;  // Bits 11:9
    UINT32 binresetdelay                           :  3;  // Bits 14:12
    UINT32 compfinishdelay                         :  3;  // Bits 17:15
    UINT32 binfinishdelay                          :  3;  // Bits 20:18
    UINT32 reserved                                :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_FSMCODEDELAYCTRL_STRUCT;
typedef union {
  struct {
    UINT32 init_mrc                                :  1;  // Bits 0:0
    UINT32 reserved3                               :  1;  // Bits 1:1
    UINT32 forcecomp_init                          :  1;  // Bits 2:2
    UINT32 comp_complete                           :  1;  // Bits 3:3
    UINT32 reserved2                               :  1;  // Bits 4:4
    UINT32 drvcccup_polarinv                       :  1;  // Bits 5:5
    UINT32 drvcccdn_polarinv                       :  1;  // Bits 6:6
    UINT32 drvdqodtup_polarinv                     :  1;  // Bits 7:7
    UINT32 drvdqodtdn_polarinv                     :  1;  // Bits 8:8
    UINT32 scompcmdctl_polarinv                    :  1;  // Bits 9:9
    UINT32 vccclkff0_polarinv                      :  1;  // Bits 10:10
    UINT32 vccclkff1_polarinv                      :  1;  // Bits 11:11
    UINT32 FFCodeTxLockStatic                      :  6;  // Bits 17:12
    INT32  FFCodePbiasCal_offset                   :  6;  // Bits 23:18
    UINT32 rloaddqsvrefcode                        :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_COMPOVERRIDE_STRUCT;
typedef union {
  struct {
    UINT32 ccm_done                                :  1;  // Bits 0:0
    UINT32 ccm_result                              :  10;  // Bits 10:1
    UINT32 ccm_clk_sel                             :  4;  // Bits 14:11
    UINT32 spare1                                  :  1;  // Bits 15:15
    UINT32 ccm_bcnt_ival                           :  9;  // Bits 24:16
    UINT32 spare2                                  :  4;  // Bits 28:25
    UINT32 ccm_clkgate_dis                         :  1;  // Bits 29:29
    UINT32 ccm_reset_n                             :  1;  // Bits 30:30
    UINT32 ccm_start                               :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT;
typedef union {
  struct {
    UINT32 obstypesel_onview0                      :  3;  // Bits 2:0
    UINT32 obstypesel_onview1                      :  3;  // Bits 5:3
    UINT32 i_localforcedaccode                     :  5;  // Bits 10:6
    UINT32 i_localforcedacen                       :  1;  // Bits 11:11
    UINT32 i_anaviewoutench1                       :  1;  // Bits 12:12
    UINT32 i_anaviewoutench0                       :  1;  // Bits 13:13
    UINT32 Spare                                   :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VISA2VIEW_STRUCT;
typedef union {
  struct {
    UINT32 VSBonusIn                               :  2;  // Bits 1:0
    UINT32 VSBonusOut0                             :  2;  // Bits 3:2
    UINT32 VSBonusOut1                             :  2;  // Bits 5:4
    UINT32 ISenseLVRBonusIn                        :  2;  // Bits 7:6
    UINT32 ISenseLVRBonusOut                       :  2;  // Bits 9:8
    UINT32 phbufpllcbb_o_bonus                     :  4;  // Bits 13:10
    UINT32 phbufpllcbbtxdll_o_bonus                :  10;  // Bits 23:14
    UINT32 o_txdll_dcdsampfall                     :  1;  // Bits 24:24
    UINT32 o_txdll_dcdsamprise                     :  1;  // Bits 25:25
    UINT32 phbufpllcbb_i_bonus                     :  2;  // Bits 27:26
    UINT32 VS_sel_vdd2_ladder                      :  1;  // Bits 28:28
    UINT32 anaviewcrossbaren                       :  1;  // Bits 29:29
    UINT32 vsxhibypassen_onsag                     :  1;  // Bits 30:30
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_PM_MISCBONUS_STRUCT;
typedef union {
  struct {
    UINT32 rcompcbb_i_bonus                        :  4;  // Bits 3:0
    UINT32 rcompcbb_o_bonus                        :  4;  // Bits 7:4
    UINT32 viewanapad_i_bonus                      :  4;  // Bits 11:8
    UINT32 txresetana_i_bonus                      :  2;  // Bits 13:12
    UINT32 txresetana_o_bonus                      :  2;  // Bits 15:14
    UINT32 vccclkcompcbb_i_bonus                   :  8;  // Bits 23:16
    UINT32 vccclkcompcbb_o_bonus                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_CBB_BONUS_STRUCT;
typedef union {
  struct {
    UINT32 Cben_TxDllG4                            :  2;  // Bits 1:0
    UINT32 Cben_TxDllG2                            :  2;  // Bits 3:2
    UINT32 Cben_RxDllG2                            :  2;  // Bits 5:4
    UINT32 Bwsel_TxDllG4                           :  4;  // Bits 9:6
    UINT32 Bwsel_RxDllG2                           :  4;  // Bits 13:10
    UINT32 Bwsel_TxDllG2                           :  4;  // Bits 17:14
    UINT32 Spare                                   :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRBWSELCOMPDATACCC_STRUCT;
typedef union {
  struct {
    UINT32 FFCodeTxDllCCC                          :  6;  // Bits 5:0
    UINT32 Bwsel_TxDllCCC                          :  4;  // Bits 9:6
    UINT32 Spare                                   :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLCOMPCCC_STRUCT;
typedef union {
  struct {
    UINT32 LeakCodeTxDll                           :  7;  // Bits 6:0
    UINT32 LeakCodeRxDll                           :  7;  // Bits 13:7
    UINT32 LeakCodeTxDllCCC                        :  7;  // Bits 20:14
    UINT32 spare                                   :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRVCCDLLTEMP_STRUCT;
typedef union {
  struct {
    INT32  TxDllDccCode                            :  5;  // Bits 4:0
    UINT32 TxDllDccEn                              :  1;  // Bits 5:5
    UINT32 TxDllDccRangeSel                        :  2;  // Bits 7:6
    UINT32 DllDcdTargetSel                         :  1;  // Bits 8:8
    UINT32 DllDcdEn                                :  1;  // Bits 9:9
    UINT32 UpdatePulse                             :  1;  // Bits 10:10
    UINT32 DllVcdlDccCode                          :  4;  // Bits 14:11
    UINT32 DllVcdlDccCodeEn                        :  1;  // Bits 15:15
    UINT32 spare                                   :  1;  // Bits 16:16
    UINT32 phbufpllcbb_fastlock_sag                :  1;  // Bits 17:17
    UINT32 vccclkcomp_turbotrimonrelock_sag        :  2;  // Bits 19:18
    UINT32 phbufpllcbb_turbotrimonrelock_sag       :  2;  // Bits 21:20
    UINT32 Dllsegmentdisable                       :  2;  // Bits 23:22
    UINT32 Dllsegmentdisable_dvfs                  :  2;  // Bits 25:24
    UINT32 Spare0                                  :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLDCCCONTROL0_STRUCT;
typedef union {
  struct {
    UINT32 swcapreset_enb_ovrdval                  :  1;  // Bits 0:0
    UINT32 VccClkgCompVrefSel                      :  8;  // Bits 8:1
    UINT32 SelVdd2Ladder                           :  1;  // Bits 9:9
    UINT32 bwctrltxdll                             :  4;  // Bits 13:10
    UINT32 cbentxdll                               :  2;  // Bits 15:14
    UINT32 bwctrlrxdll                             :  4;  // Bits 19:16
    UINT32 cbenrxdll                               :  2;  // Bits 21:20
    UINT32 bwctrltxdllccc                          :  4;  // Bits 25:22
    UINT32 cbentxdllccc                            :  2;  // Bits 27:26
    UINT32 gracious_deassert                       :  1;  // Bits 28:28
    UINT32 clkcomp_en_ovrd                         :  1;  // Bits 29:29
    UINT32 clkcomp_en_ovrdval                      :  1;  // Bits 30:30
    UINT32 swcapreset_enb_ovrd                     :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 TxDLLVccDllFunClock                     :  1;  // Bits 0:0
    UINT32 RxDLLVccDllFunClock                     :  1;  // Bits 1:1
    UINT32 TxDLLVccDllDacCode                      :  11;  // Bits 12:2
    UINT32 RxDLLVccDllDacCode                      :  11;  // Bits 23:13
    UINT32 rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 txdll_turboonstartup                    :  1;  // Bits 0:0
    UINT32 rxdll_turboonstartup                    :  1;  // Bits 1:1
    UINT32 txdll_vctltrim                          :  2;  // Bits 3:2
    UINT32 rxdll_vctltrim                          :  2;  // Bits 5:4
    UINT32 txdll_endaccodeout                      :  1;  // Bits 6:6
    UINT32 rxdll_endaccodeout                      :  1;  // Bits 7:7
    UINT32 rxdll_daccode_freeze_ovrdval            :  1;  // Bits 8:8
    UINT32 txdll_daccode_freeze_ovrdval            :  1;  // Bits 9:9
    UINT32 rxdll_daccode_freeze_ovrden             :  1;  // Bits 10:10
    UINT32 txdll_daccode_freeze_ovrden             :  1;  // Bits 11:11
    UINT32 txdll_turboovrden                       :  1;  // Bits 12:12
    UINT32 rxdll_turboovrden                       :  1;  // Bits 13:13
    UINT32 txdll_turboovrdval                      :  1;  // Bits 14:14
    UINT32 rxdll_turboovrdval                      :  1;  // Bits 15:15
    UINT32 txdll_turbotrim                         :  2;  // Bits 17:16
    UINT32 rxdll_turbotrim                         :  2;  // Bits 19:18
    UINT32 txdll_turbotrimonrelock                 :  2;  // Bits 21:20
    UINT32 rxdll_turbotrimonrelock                 :  2;  // Bits 23:22
    UINT32 vccclkglvren_ovrden                     :  1;  // Bits 24:24
    UINT32 vccclkglvren_ovrdval                    :  1;  // Bits 25:25
    UINT32 vccclkglvrvrefen_ovrden                 :  1;  // Bits 26:26
    UINT32 vccclkglvrvrefen_ovrdval                :  1;  // Bits 27:27
    UINT32 txdll_fastlock                          :  1;  // Bits 28:28
    UINT32 rxdll_fastlock                          :  1;  // Bits 29:29
    UINT32 txdll_piclkdiv2en                       :  1;  // Bits 30:30
    UINT32 rsvd                                    :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL2_STRUCT;
typedef union {
  struct {
    UINT32 Tlvrsettle                              :  12;  // Bits 11:0
    UINT32 Tclkcompen2swcaprstb                    :  4;  // Bits 15:12
    UINT32 Tgvref2clkcompen                        :  4;  // Bits 19:16
    UINT32 Tqclk2gvrefen                           :  4;  // Bits 23:20
    UINT32 txdll_fastlock_val                      :  4;  // Bits 27:24
    UINT32 rxdll_fastlock_val                      :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL5_STRUCT;
typedef union {
  struct {
    UINT32 TxDllBonus                              :  10;  // Bits 9:0
    UINT32 PhBufBonus                              :  4;  // Bits 13:10
    UINT32 PhEnUpperBonus                          :  2;  // Bits 15:14
    UINT32 PhEnMiddleBonus                         :  2;  // Bits 17:16
    UINT32 PhEnLowerBonus                          :  2;  // Bits 19:18
    UINT32 RefPiBonus                              :  2;  // Bits 21:20
    UINT32 TxDllWLOvrdEn                           :  1;  // Bits 22:22
    UINT32 TxDllWLOvrdVal                          :  1;  // Bits 23:23
    UINT32 TxDllShutDownOvrdEn                     :  1;  // Bits 24:24
    UINT32 TxDllShutDownOvrdVal                    :  1;  // Bits 25:25
    UINT32 RxDllWLOvrdEn                           :  1;  // Bits 26:26
    UINT32 RxDllWLOvrdVal                          :  1;  // Bits 27:27
    UINT32 RxDllShutDownOvrdEn                     :  1;  // Bits 28:28
    UINT32 RxDllShutDownOvrdVal                    :  1;  // Bits 29:29
    UINT32 EnDacCodeOutOvrd                        :  1;  // Bits 30:30
    UINT32 IOBufActOvrd                            :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;
typedef union {
  struct {
    UINT32 bw_txdll                                :  4;  // Bits 3:0
    UINT32 bw_rxdll                                :  4;  // Bits 7:4
    UINT32 bw_txdll_ovrden                         :  1;  // Bits 8:8
    UINT32 bw_rxdll_ovrden                         :  1;  // Bits 9:9
    UINT32 spare0                                  :  2;  // Bits 11:10
    UINT32 cb_txdll                                :  2;  // Bits 13:12
    UINT32 cb_rxdll                                :  2;  // Bits 15:14
    UINT32 cb_txdll_ovrden                         :  1;  // Bits 16:16
    UINT32 cb_rxdll_ovrden                         :  1;  // Bits 17:17
    UINT32 bwsel_gear4_mode_sel                    :  1;  // Bits 18:18
    UINT32 rloaddqs_staticleg                      :  1;  // Bits 19:19
    UINT32 spare                                   :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT;
typedef union {
  struct {
    UINT32 thermsensoren                           :  1;  // Bits 0:0
    UINT32 gvrefselvdd2                            :  1;  // Bits 1:1
    UINT32 vccclkcomp_rxdllen_ovrden               :  1;  // Bits 2:2
    UINT32 vccclkcomp_rxdllen_ovrdval              :  1;  // Bits 3:3
    UINT32 ffentxwl_ovrden                         :  1;  // Bits 4:4
    UINT32 ffentxwl_ovrdval                        :  1;  // Bits 5:5
    UINT32 ffenrxwl_ovrden                         :  1;  // Bits 6:6
    UINT32 ffenrxwl_ovrdval                        :  1;  // Bits 7:7
    UINT32 ffenrxlock_ovrden                       :  1;  // Bits 8:8
    UINT32 ffenrxlock_ovrdval                      :  1;  // Bits 9:9
    UINT32 rsvd                                    :  1;  // Bits 10:10
    UINT32 PICodeph1                               :  4;  // Bits 14:11
    UINT32 rsvd0                                   :  8;  // Bits 22:15
    UINT32 bwsel_done                              :  1;  // Bits 23:23
    UINT32 bwsel_error                             :  1;  // Bits 24:24
    UINT32 previous_pass                           :  1;  // Bits 25:25
    UINT32 spare                                   :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL5_STRUCT;
typedef union {
  struct {
    UINT32 FastLockCtrTXDLL                        :  4;  // Bits 3:0
    UINT32 FastLockCtrRXDLL                        :  4;  // Bits 7:4
    UINT32 TxDllWeaklockExitDelay                  :  2;  // Bits 9:8
    UINT32 RxDllWeaklockExitDelay                  :  2;  // Bits 11:10
    UINT32 TxDllStartupCntSel                      :  4;  // Bits 15:12
    UINT32 RxDllStartupCntSel                      :  4;  // Bits 19:16
    UINT32 TxDllShutDownGraceCounter               :  4;  // Bits 23:20
    UINT32 Rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCOMPOVR0_STRUCT DDRPHY_DDRCOMP_CR_DDRCRDVFSCOMPOVR0_STRUCT;
typedef union {
  struct {
    UINT32 TxDLLShutDownToDACStoringDelay          :  4;  // Bits 3:0
    UINT32 RxDLLShutDownToDACStoringDelay          :  4;  // Bits 7:4
    UINT32 TxDllWeakLockExitDelay                  :  2;  // Bits 9:8
    UINT32 RxDllWeakLockExitDelay                  :  2;  // Bits 11:10
    UINT32 TxDllStartupCntSel                      :  4;  // Bits 15:12
    UINT32 RxDllStartupCntSel                      :  4;  // Bits 19:16
    UINT32 TxDllShutDownGraceCounter               :  4;  // Bits 23:20
    UINT32 RxDllShutDownGraceCounter               :  4;  // Bits 27:24
    UINT32 TxDllResetFlushPulse                    :  3;  // Bits 30:28
    UINT32 Rsvd                                    :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDLL_COMPCR_DLLTIMERCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 TxDllWL_ovrdval                         :  1;  // Bits 0:0
    UINT32 TxDllWL_ovrden                          :  1;  // Bits 1:1
    UINT32 WL_ovrden_txdlldig                      :  1;  // Bits 2:2
    UINT32 TxDllEnDacCodeOutOvrd                   :  1;  // Bits 3:3
    UINT32 TxDllLockCtr                            :  3;  // Bits 6:4
    UINT32 txdll_reset_ovrd_val                    :  1;  // Bits 7:7
    UINT32 txdll_reset_ovrd_en                     :  1;  // Bits 8:8
    UINT32 TxDllPwrDownOvrd                        :  1;  // Bits 9:9
    UINT32 ForceTxDllReset                         :  1;  // Bits 10:10
    UINT32 TxDllLockOvrd                           :  1;  // Bits 11:11
    UINT32 TxDllLockSel                            :  1;  // Bits 12:12
    UINT32 RxDllWL_ovrdval                         :  1;  // Bits 13:13
    UINT32 RxDllWL_ovrden                          :  1;  // Bits 14:14
    UINT32 WL_ovrden_rxdlldig                      :  1;  // Bits 15:15
    UINT32 RxDllEnDacCodeOutOvrd                   :  1;  // Bits 16:16
    UINT32 RxDllLockCtr                            :  3;  // Bits 19:17
    UINT32 rxdll_reset_ovrd_val                    :  1;  // Bits 20:20
    UINT32 rxdll_reset_ovrd_en                     :  1;  // Bits 21:21
    UINT32 RxDllPwrDownOvrd                        :  1;  // Bits 22:22
    UINT32 ForceRxDllReset                         :  1;  // Bits 23:23
    UINT32 RxDllLockOvrd                           :  1;  // Bits 24:24
    UINT32 RxDllLockSel                            :  1;  // Bits 25:25
    UINT32 rsvd                                    :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDLL_COMPCR_DLLTIMERCONTROL2_STRUCT;
typedef union {
  struct {
    UINT32 txdll_openloop                          :  1;  // Bits 0:0
    UINT32 rxdll_openloop                          :  1;  // Bits 1:1
    UINT32 forced_ckt_on                           :  1;  // Bits 2:2
    UINT32 lvr_on_ovrden                           :  1;  // Bits 3:3
    UINT32 lvr_on_ovrdval                          :  1;  // Bits 4:4
    UINT32 dll_lvr_clkgdisable                     :  1;  // Bits 5:5
    UINT32 forced_rxdll_on                         :  1;  // Bits 6:6
    UINT32 forced_txdll_on                         :  1;  // Bits 7:7
    UINT32 VccClkCompRatioFFLock_FFWL              :  2;  // Bits 9:8
    UINT32 RxDLL_Tlvrrstlo                         :  8;  // Bits 17:10
    UINT32 TxDLL_Tlvrrstlo                         :  8;  // Bits 25:18
    UINT32 funclock_avg_cntr                       :  1;  // Bits 26:26
    UINT32 vccclkglvrvccrsten1_ovrd                :  1;  // Bits 27:27
    UINT32 vccclkglvrvccrsten1_ovrdval             :  1;  // Bits 28:28
    UINT32 vccclkglvrvccrsten1_bwselval            :  1;  // Bits 29:29
    UINT32 vccclkglvrvccrsten0_ovrd                :  1;  // Bits 30:30
    UINT32 vccclkglvrvccrsten0_ovrdval             :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL3_STRUCT;
typedef union {
  struct {
    UINT32 funclock_target_samples                 :  8;  // Bits 7:0
    UINT32 avg_cntr_low                            :  12;  // Bits 19:8
    UINT32 avg_cntr_high                           :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL4_STRUCT;
typedef union {
  struct {
    UINT32 daccode_lo_thresh                       :  11;  // Bits 10:0
    UINT32 daccode_hi_thresh                       :  11;  // Bits 21:11
    UINT32 daccodefreeze_timer_val                 :  6;  // Bits 27:22
    UINT32 Enable_TxDllCRWr                        :  1;  // Bits 28:28
    UINT32 Enable_RxDllCRWr                        :  1;  // Bits 29:29
    UINT32 Enable_TxDllCCCCRWr                     :  1;  // Bits 30:30
    UINT32 SpareRW                                 :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_BWSELCOMPCONTROL0_STRUCT;
typedef union {
  struct {
    UINT32 daccode_targ_thresh                     :  11;  // Bits 10:0
    UINT32 DllShutDown_timer_val                   :  5;  // Bits 15:11
    UINT32 BwselStatvalue                          :  4;  // Bits 19:16
    UINT32 VccDllDacCode_TxDLL                     :  11;  // Bits 30:20
    UINT32 SpareRW                                 :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_BWSELCOMPCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 VccDllDacCode_RxDLL                     :  11;  // Bits 10:0
    UINT32 VccDllDacCode_TxDLLCCC                  :  11;  // Bits 21:11
    UINT32 BwselClkCompSel                         :  2;  // Bits 23:22
    UINT32 bwseldec_timer_val                      :  4;  // Bits 27:24
    UINT32 SpareRW                                 :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_BWSELCOMPCONTROL2_STRUCT;
typedef union {
  struct {
    UINT32 analogeleprobeen                        :  1;  // Bits 0:0
    UINT32 digitaleleprobeen                       :  1;  // Bits 1:1
    UINT32 lpddr5en                                :  1;  // Bits 2:2
    UINT32 spareRW3                                :  3;  // Bits 5:3
    UINT32 spareRW4                                :  4;  // Bits 9:6
    UINT32 spareRW5                                :  5;  // Bits 14:10
    UINT32 spareRW6                                :  6;  // Bits 20:15
    UINT32 dlllocktimer                            :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_COMPCTL6_STRUCT;
typedef union {
  struct {
    UINT32 spareRW1                                :  1;  // Bits 0:0
    UINT32 spareRW2                                :  2;  // Bits 2:1
    UINT32 spareRW3                                :  3;  // Bits 5:3
    UINT32 spareRW4                                :  4;  // Bits 9:6
    UINT32 spareRW5                                :  5;  // Bits 14:10
    UINT32 spareRW6                                :  6;  // Bits 20:15
    UINT32 spareRW7                                :  7;  // Bits 27:21
    UINT32 spareRW                                 :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_COMPBACKUP1_STRUCT;
typedef union {
  struct {
    UINT32 clkscompdiv2en                          :  1;  // Bits 0:0
    UINT32 scomp_disfallpd                         :  1;  // Bits 1:1
    UINT32 scomp_parktoggle_ovrd_en                :  1;  // Bits 2:2
    UINT32 scomp_parktoggle_ovrd_val               :  1;  // Bits 3:3
    UINT32 scomp_delaycells                        :  4;  // Bits 7:4
    UINT32 scomp_cycphsel                          :  1;  // Bits 8:8
    UINT32 scomp_bonus_in                          :  2;  // Bits 10:9
    UINT32 scomp_reset_delay                       :  3;  // Bits 13:11
    UINT32 rsvd                                    :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRSCOMP0_STRUCT;
typedef union {
  struct {
    UINT32 fallingphdetrawupdn                     :  1;  // Bits 0:0
    UINT32 risingphdetrawupdn                      :  1;  // Bits 1:1
    UINT32 scompdlyclkprepd                        :  1;  // Bits 2:2
    UINT32 scomp_bonus_out                         :  1;  // Bits 3:3
    UINT32 rsvd                                    :  28;  // Bits 31:4
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_DDRCRCOMPSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 pwrgoodvccclkglvrdly                    :  2;  // Bits 1:0
    UINT32 picode                                  :  3;  // Bits 4:2
    UINT32 rxpicode                                :  4;  // Bits 8:5
    UINT32 phbuf_bw                                :  2;  // Bits 10:9
    UINT32 pbiascalvrefsel                         :  2;  // Bits 12:11
    UINT32 lvrgainctrl                             :  2;  // Bits 14:13
    UINT32 vccclkcomp_cccafegear4en                :  1;  // Bits 15:15
    UINT32 vccclkcomp_gear4en                      :  1;  // Bits 16:16
    UINT32 vccclkglvrvrefen_ovrden                 :  1;  // Bits 17:17
    UINT32 vccclkglvrvrefen_ovrdval                :  1;  // Bits 18:18
    UINT32 DVFSModeEn_ovrden                       :  1;  // Bits 19:19
    UINT32 DVFSModeEn_ovrdval                      :  1;  // Bits 20:20
    UINT32 ffentxlock_ovrden                       :  1;  // Bits 21:21
    UINT32 ffentxlock_ovrdval                      :  1;  // Bits 22:22
    UINT32 vccclkcomp_txdllen_ovrden               :  1;  // Bits 23:23
    UINT32 vccclkcomp_txdllen_ovrdval              :  1;  // Bits 24:24
    UINT32 tlclk_tldrven                           :  2;  // Bits 26:25
    UINT32 reserved                                :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL6_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRDATA_SHARED0_CR_DCCCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 InputClkSelect                          :  2;  // Bits 1:0
    UINT32 ParkToggle                              :  1;  // Bits 2:2
    UINT32 EnPhaseGating                           :  1;  // Bits 3:3
    UINT32 Rsvd0                                   :  2;  // Bits 5:4
    UINT32 EnFuSaChecking                          :  1;  // Bits 6:6
    UINT32 LockOvrd                                :  1;  // Bits 7:7
    UINT32 OpenLoop                                :  1;  // Bits 8:8
    UINT32 ForceDLLReset                           :  1;  // Bits 9:9
    UINT32 EnDLLWithRcvEnPi                        :  1;  // Bits 10:10
    UINT32 EnablePiWhenOff                         :  1;  // Bits 11:11
    UINT32 RunSingleCal                            :  1;  // Bits 12:12
    UINT32 Rsvd1                                   :  2;  // Bits 14:13
    UINT32 FusaFlagUnlock                          :  1;  // Bits 15:15
    UINT32 LPMode3DLLWeakLockDisable               :  1;  // Bits 16:16
    UINT32 LPMode3DLLShutDownDisable               :  1;  // Bits 17:17
    UINT32 LPMode4DLLShutDownDisable               :  1;  // Bits 18:18
    UINT32 FastLock                                :  1;  // Bits 19:19
    UINT32 DisableFF                               :  1;  // Bits 20:20
    UINT32 VctrlCap                                :  2;  // Bits 22:21
    UINT32 Turboovrdval                            :  1;  // Bits 23:23
    UINT32 Turboovrden                             :  1;  // Bits 24:24
    UINT32 TurboTrim                               :  2;  // Bits 26:25
    UINT32 RxDllWLWithNotLPMode0                   :  1;  // Bits 27:27
    UINT32 Rsvd2                                   :  1;  // Bits 28:28
    UINT32 DllTurboTrimRelock                      :  2;  // Bits 30:29
    UINT32 DLLSortEn                               :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 DdrBscanEnable                          :  1;  // Bits 0:0
    UINT32 DataValue                               :  10;  // Bits 10:1
    UINT32 rsvd                                    :  1;  // Bits 11:11
    UINT32 RxVref                                  :  7;  // Bits 18:12
    UINT32 ForceVrefOn                             :  1;  // Bits 19:19
    UINT32 BiasVrefSel                             :  3;  // Bits 22:20
    UINT32 BiasIrefAdj                             :  4;  // Bits 26:23
    UINT32 RxVcmResSel                             :  2;  // Bits 28:27
    UINT32 RxTailCtl                               :  1;  // Bits 29:29
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRBSCANDATA_STRUCT;
typedef union {
  struct {
    INT32  Ph0Delay                                :  5;  // Bits 4:0
    INT32  Ph1Delay                                :  5;  // Bits 9:5
    INT32  Ph2Delay                                :  5;  // Bits 14:10
    INT32  Ph3Delay                                :  5;  // Bits 19:15
    UINT32 PhaseDrvGPMEn                           :  2;  // Bits 21:20
    UINT32 DccQClkgateDisable                      :  1;  // Bits 22:22
    UINT32 phbuf_cmn_phbuf_bw                      :  2;  // Bits 24:23
    UINT32 rsvd                                    :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRDATA_SHARED0_CR_PHCLOCKREPEATER1_STRUCT;
typedef union {
  struct {
    UINT32 FFCodePiES                              :  6;  // Bits 5:0
    UINT32 FFCodePiRef                             :  6;  // Bits 11:6
    UINT32 FFCodeTxPhEn                            :  6;  // Bits 17:12
    UINT32 FFCodeTxLock                            :  6;  // Bits 23:18
    UINT32 FFCodeTxWL                              :  6;  // Bits 29:24
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT;
typedef union {
  struct {
    UINT32 FFCodeRxPi                              :  6;  // Bits 5:0
    UINT32 FFCodeRxPhEn                            :  6;  // Bits 11:6
    UINT32 FFCodeRxLock                            :  6;  // Bits 17:12
    UINT32 FFCodeRxWL                              :  6;  // Bits 23:18
    UINT32 rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_STRUCT;
typedef union {
  struct {
    UINT32 ntl_clkgate_dis                         :  1;  // Bits 0:0
    UINT32 ntl_reset_n                             :  1;  // Bits 1:1
    UINT32 ntl_enable                              :  1;  // Bits 2:2
    UINT32 ntlctr_matchval                         :  13;  // Bits 15:3
    UINT32 spare                                   :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_NTLTEST_STRUCT;
typedef union {
  struct {
    UINT32 ntl_done                                :  1;  // Bits 0:0
    UINT32 ntl_ppfail_bl0                          :  8;  // Bits 8:1
    UINT32 rsvd                                    :  2;  // Bits 10:9
    UINT32 ntl_ppfail_bl1                          :  8;  // Bits 18:11
    UINT32 spare                                   :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_NTLRSLT_STRUCT;
typedef union {
  struct {
    UINT32 DriftLimitLocal                         :  4;  // Bits 3:0
    UINT32 DriftLimitGlobal                        :  8;  // Bits 11:4
    UINT32 TrainTargetOffsetUI                     :  2;  // Bits 13:12
    UINT32 RxDqsACCap                              :  2;  // Bits 15:14
    UINT32 RxDqsSlope                              :  2;  // Bits 17:16
    UINT32 UseDramDqs                              :  1;  // Bits 18:18
    UINT32 InitTrain                               :  1;  // Bits 19:19
    UINT32 Override                                :  1;  // Bits 20:20
    UINT32 RxDqsVref                               :  3;  // Bits 23:21
    UINT32 RxFullSpeed                             :  1;  // Bits 24:24
    UINT32 CompSamples                             :  2;  // Bits 26:25
    UINT32 DLLSamples                              :  2;  // Bits 28:27
    UINT32 DLLCodeSwitch                           :  2;  // Bits 30:29
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRDATA_SHARED0_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRDATA_SHARED0_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRDATA_SHARED0_CR_DDRCRDLLOFFSETCONTROL_STRUCT;
typedef union {
  struct {
    INT32  Ph4Delay                                :  5;  // Bits 4:0
    INT32  Ph5Delay                                :  5;  // Bits 9:5
    INT32  Ph6Delay                                :  5;  // Bits 14:10
    INT32  Ph7Delay                                :  5;  // Bits 19:15
    UINT32 Ph7Result                               :  9;  // Bits 28:20
    UINT32 rsvd                                    :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT;
typedef union {
  struct {
    INT32  Ph4DCC                                  :  6;  // Bits 5:0
    INT32  Ph5DCC                                  :  6;  // Bits 11:6
    INT32  Ph6DCC                                  :  6;  // Bits 17:12
    INT32  Ph7DCC                                  :  6;  // Bits 23:18
    INT32  TxDllDccCode                            :  6;  // Bits 29:24
    UINT32 chk_revert_extenden                     :  1;  // Bits 30:30
    UINT32 rsvd                                    :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT DDRDATA_SHARED0_CR_RCOMPDATA_STRUCT;
typedef union {
  struct {
    UINT32 Rsvd                                    :  5;  // Bits 4:0
    UINT32 TlClkDCABw                              :  4;  // Bits 8:5
    UINT32 rcvenffmuxsel_byte0                     :  3;  // Bits 11:9
    UINT32 rcvenffmuxsel_byte1                     :  3;  // Bits 14:12
    UINT32 RdEarlyDqs                              :  1;  // Bits 15:15
    UINT32 ShiftVttCompSpineGated                  :  2;  // Bits 17:16
    UINT32 UseDefaultRdPtrCalc                     :  1;  // Bits 18:18
    UINT32 RstNumPre                               :  3;  // Bits 21:19
    UINT32 RXTogglePreAmble                        :  2;  // Bits 23:22
    UINT32 RxSALDFEOvrdSel                         :  1;  // Bits 24:24
    UINT32 RcvEnWaveShape                          :  2;  // Bits 26:25
    UINT32 RxReadPointer                           :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_STRUCT;
typedef union {
  struct {
    UINT32 ClkVoltageSelect                        :  1;  // Bits 0:0
    UINT32 TxCLKNmosOnly                           :  1;  // Bits 1:1
    UINT32 ddrinitcompleteinternal_sel_chkbit      :  1;  // Bits 2:2
    UINT32 CccStaticLegCtrl                        :  1;  // Bits 3:3
    UINT32 DisWckPupDcc                            :  2;  // Bits 5:4
    UINT32 WCKDivEnPreamble                        :  1;  // Bits 6:6
    UINT32 WCKIncPreamble                          :  2;  // Bits 8:7
    UINT32 PiSyncDivider                           :  2;  // Bits 10:9
    UINT32 PIClkDuration                           :  3;  // Bits 13:11
    UINT32 GearMode                                :  1;  // Bits 14:14
    UINT32 SEClk                                   :  1;  // Bits 15:15
    UINT32 PhaseDrvPiPMEn                          :  2;  // Bits 17:16
    UINT32 IntCkOn                                 :  1;  // Bits 18:18
    UINT32 CkeIdlePiGateDisable                    :  1;  // Bits 19:19
    UINT32 IOBufAct                                :  1;  // Bits 20:20
    UINT32 PreDrvOn                                :  1;  // Bits 21:21
    UINT32 ClkGateDisable                          :  1;  // Bits 22:22
    UINT32 Spare                                   :  7;  // Bits 29:23
    UINT32 RunRefPIFSM                             :  1;  // Bits 30:30
    UINT32 BlockTrainRst                           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 PiCode                                  :  6;  // Bits 5:0
    INT32  TcoCode                                 :  6;  // Bits 11:6
    INT32  DccCode                                 :  7;  // Bits 18:12
    UINT32 PiDivEn                                 :  1;  // Bits 19:19
    UINT32 PiDivInc                                :  2;  // Bits 21:20
    UINT32 LaneEn                                  :  1;  // Bits 22:22
    UINT32 PiEn                                    :  1;  // Bits 23:23
    UINT32 rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED0_CR_DDRCRCCCCH1LANE0_STRUCT;
typedef union {
  struct {
    UINT32 spare1                                  :  9;  // Bits 8:0
    UINT32 CLKPICodeCh0                            :  9;  // Bits 17:9
    UINT32 CLKPICodeCh1                            :  9;  // Bits 26:18
    UINT32 ZeroRefPiLatency                        :  1;  // Bits 27:27
    UINT32 PiEnOvrd                                :  1;  // Bits 28:28
    UINT32 CCCMuxSelect                            :  2;  // Bits 30:29
    UINT32 spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRCCCPICODES_STRUCT;
typedef union {
  struct {
    INT32  PfNrCh0                                 :  5;  // Bits 4:0
    INT32  PrNfCh0                                 :  5;  // Bits 9:5
    INT32  PfNrCh1                                 :  5;  // Bits 14:10
    INT32  PrNfCh1                                 :  5;  // Bits 19:15
    UINT32 EnFeedbackCh0                           :  2;  // Bits 21:20
    UINT32 PfNrFeedbackCh0                         :  2;  // Bits 23:22
    UINT32 PrNfFeedbackCh0                         :  2;  // Bits 25:24
    UINT32 EnFeedbackCh1                           :  2;  // Bits 27:26
    UINT32 PfNrFeedbackCh1                         :  2;  // Bits 29:28
    UINT32 PrNfFeedbackCh1                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT;
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
    UINT32 rsvd                                    :  1;  // Bits 28:28
    UINT32 UpdateTcoComp                           :  1;  // Bits 29:29
    UINT32 rsvd1                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 DccStepSize                             :  5;  // Bits 4:0
    UINT32 CKTargetElse                            :  5;  // Bits 9:5
    INT32  WCKTargetCh1                            :  5;  // Bits 14:10
    INT32  WCKTargetCh0                            :  5;  // Bits 19:15
    UINT32 rsvd2                                   :  1;  // Bits 20:20
    UINT32 TrackExtremeLane                        :  4;  // Bits 24:21
    UINT32 TrackExtremeMax                         :  1;  // Bits 25:25
    UINT32 TrackExtremeRank                        :  1;  // Bits 26:26
    UINT32 LargeChange                             :  3;  // Bits 29:27
    UINT32 LargeChangeEnable                       :  1;  // Bits 30:30
    UINT32 LargeChangeReset                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_STRUCT;
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
} DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 ExtremeOffset                           :  7;  // Bits 6:0
    UINT32 ExtremeCount                            :  9;  // Bits 15:7
    UINT32 LaneResult                              :  9;  // Bits 24:16
    UINT32 rsvd                                    :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED0_CR_WCKDCCLANESTATUS1_STRUCT;
typedef union {
  struct {
    UINT32 spare1                                  :  6;  // Bits 5:0
    UINT32 RcompDrvUp                              :  6;  // Bits 11:6
    UINT32 RcompDrvDown                            :  6;  // Bits 17:12
    UINT32 Spare                                   :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRCLKCOMP_STRUCT;
typedef union {
  struct {
    INT32  ClkRcompDrvOffsetCh0                    :  6;  // Bits 5:0
    INT32  ClkRcompDrvOffsetCh1                    :  6;  // Bits 11:6
    INT32  DqsRcompDrvOffsetByte0                  :  6;  // Bits 17:12
    INT32  DqsRcompDrvOffsetByte1                  :  6;  // Bits 23:18
    UINT32 Spare                                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_STRUCT;
typedef union {
  struct {
    UINT32 RunTest                                 :  1;  // Bits 0:0
    UINT32 Param                                   :  3;  // Bits 3:1
    INT32  MinVal                                  :  7;  // Bits 10:4
    INT32  MaxVal                                  :  7;  // Bits 17:11
    UINT32 MinWidth                                :  6;  // Bits 23:18
    UINT32 CalcCenter                              :  1;  // Bits 24:24
    UINT32 IOLBCycles                              :  4;  // Bits 28:25
    UINT32 IOLB_CCC_Force_ODTEN                    :  1;  // Bits 29:29
    UINT32 rsvd                                    :  1;  // Bits 30:30
    UINT32 LaneResult                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED0_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED0_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRDATA_SHARED0_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRDATA_SHARED0_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 TxDllDccRangeSel                        :  2;  // Bits 1:0
    UINT32 DllForceEnPDFilter                      :  1;  // Bits 2:2
    UINT32 DllForceDisPDFilter                     :  1;  // Bits 3:3
    UINT32 BypMajVoteForPD                         :  1;  // Bits 4:4
    UINT32 Rsvd1                                   :  2;  // Bits 6:5
    UINT32 SelResetRefPiDivFlop                    :  1;  // Bits 7:7
    UINT32 TxDllResetFlushPulse                    :  3;  // Bits 10:8
    UINT32 TxDllStaticFF                           :  2;  // Bits 12:11
    UINT32 DLLShutDownToDACStoringDelay            :  4;  // Bits 16:13
    UINT32 TxDllPiEnBeforeResetFlush               :  3;  // Bits 19:17
    UINT32 PhaseDrvPiPMEn                          :  2;  // Bits 21:20
    UINT32 RxPiDisable                             :  1;  // Bits 22:22
    UINT32 TxPiDisable                             :  1;  // Bits 23:23
    UINT32 Rsvd                                    :  1;  // Bits 24:24
    UINT32 TxDllDccEn                              :  1;  // Bits 25:25
    UINT32 TxDLL_DccFsmEn                          :  1;  // Bits 26:26
    UINT32 txdlldcccode_extend_en                  :  1;  // Bits 27:27
    UINT32 RxDllLpMode2TxDllLpMode                 :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT;
typedef union {
  struct {
    UINT32 DLLPHClkSel                             :  2;  // Bits 1:0
    UINT32 DllDacCodeFreeze                        :  1;  // Bits 2:2
    UINT32 LockCtr                                 :  3;  // Bits 5:3
    UINT32 LockSel                                 :  1;  // Bits 6:6
    UINT32 VctlInit                                :  11;  // Bits 17:7
    UINT32 DllPwrDownOvrd                          :  1;  // Bits 18:18
    UINT32 Spare                                   :  6;  // Bits 24:19
    UINT32 FastLockCtr                             :  3;  // Bits 27:25
    UINT32 EnTurboOnStartUp                        :  1;  // Bits 28:28
    UINT32 EnWL                                    :  1;  // Bits 29:29
    UINT32 DLLSegmentDisable                       :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRDATA_SHARED0_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED0_CR_DFXCCMONBYTE0_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED0_CR_DFXCCMONBYTE1_STRUCT;
typedef union {
  struct {
    UINT32 ISenseLVRBonusIn                        :  2;  // Bits 1:0
    UINT32 ISenseLVRBonusOut                       :  2;  // Bits 3:2
    UINT32 thermsensoren                           :  1;  // Bits 4:4
    UINT32 anaviewcrossbaren0                      :  1;  // Bits 5:5
    UINT32 anaviewcrossbaren1                      :  1;  // Bits 6:6
    UINT32 vsxhibypassen_onsag                     :  1;  // Bits 7:7
    UINT32 Spare                                   :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDR_PMBONUS0_STRUCT;
typedef union {
  struct {
    UINT32 Cben_TxDllG4                            :  2;  // Bits 1:0
    UINT32 Cben_TxDllG2                            :  2;  // Bits 3:2
    UINT32 Cben_RxDllG2                            :  2;  // Bits 5:4
    UINT32 Bwsel_TxDllG4                           :  4;  // Bits 9:6
    UINT32 Bwsel_RxDllG2                           :  4;  // Bits 13:10
    UINT32 Bwsel_TxDllG2                           :  4;  // Bits 17:14
    UINT32 Spare                                   :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT;
typedef union {
  struct {
    INT32  FFCodePiESOffset                        :  5;  // Bits 4:0
    INT32  FFCodePiRefOffset                       :  5;  // Bits 9:5
    INT32  FFCodeTxPhEnOffset                      :  5;  // Bits 14:10
    INT32  FFCodeTxLockOffset                      :  5;  // Bits 19:15
    INT32  FFCodeTxWLOffset                        :  5;  // Bits 24:20
    UINT32 txdll_ffcode_compupdt_ovrd_en           :  1;  // Bits 25:25
    UINT32 txdll_ffcode_compupdt_ovrd_val          :  1;  // Bits 26:26
    UINT32 Spare                                   :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT DDRDATA_SHARED0_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;
typedef union {
  struct {
    UINT32 DQ0_bonus                               :  4;  // Bits 3:0
    UINT32 DQ1_bonus                               :  4;  // Bits 7:4
    UINT32 DQ2_bonus                               :  4;  // Bits 11:8
    UINT32 DQ3_bonus                               :  4;  // Bits 15:12
    UINT32 DQ4_bonus                               :  4;  // Bits 19:16
    UINT32 DQ5_bonus                               :  4;  // Bits 23:20
    UINT32 DQ6_bonus                               :  4;  // Bits 27:24
    UINT32 DQ7_bonus                               :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED0_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT;
typedef union {
  struct {
    UINT32 DQSByte0_bonus                          :  2;  // Bits 1:0
    INT32  RxDllDccCode_Byte0                      :  4;  // Bits 5:2
    UINT32 RxDllSourcePhClkSel_Byte0               :  1;  // Bits 6:6
    UINT32 RxDllTerDcdSelRefClk_Byte0              :  1;  // Bits 7:7
    UINT32 DQSByte1_bonus                          :  2;  // Bits 9:8
    INT32  RxDllDccCode_Byte1                      :  4;  // Bits 13:10
    UINT32 RxDllSourcePhClkSel_Byte1               :  1;  // Bits 14:14
    UINT32 RxDllTerDcdSelRefClk_Byte1              :  1;  // Bits 15:15
    UINT32 WCKByte0_bonus                          :  8;  // Bits 23:16
    UINT32 WCKByte1_bonus                          :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT;
typedef union {
  struct {
    UINT32 DbiByte0_bonus                          :  4;  // Bits 3:0
    UINT32 DbiByte1_bonus                          :  4;  // Bits 7:4
    UINT32 dqsbuf_ibonus                           :  14;  // Bits 21:8
    UINT32 dqsbuf_obonus                           :  6;  // Bits 27:22
    UINT32 Rsvd                                    :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_STRUCT;
typedef union {
  struct {
    UINT32 RefPi4XoverOffset_DQSByte0              :  7;  // Bits 6:0
    UINT32 RefPi4XoverOffset_DQSByte1              :  7;  // Bits 13:7
    UINT32 RefPi4XoverOffset_WCKByte0              :  7;  // Bits 20:14
    UINT32 RefPi4XoverOffset_WCKByte1              :  7;  // Bits 27:21
    UINT32 Rsvd                                    :  3;  // Bits 30:28
    UINT32 RefPi2Pi_full_sweep                     :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRDATA_SHARED0_CR_DDRCRDLLCBTUNEOVRD_STRUCT;
typedef union {
  struct {
    UINT32 dq0_dqsfifop_dlysel                     :  2;  // Bits 1:0
    UINT32 dq0_dqsfifon_dlysel                     :  2;  // Bits 3:2
    UINT32 dq1_dqsfifop_dlysel                     :  2;  // Bits 5:4
    UINT32 dq1_dqsfifon_dlysel                     :  2;  // Bits 7:6
    UINT32 dq2_dqsfifop_dlysel                     :  2;  // Bits 9:8
    UINT32 dq2_dqsfifon_dlysel                     :  2;  // Bits 11:10
    UINT32 dq3_dqsfifop_dlysel                     :  2;  // Bits 13:12
    UINT32 dq3_dqsfifon_dlysel                     :  2;  // Bits 15:14
    UINT32 dq4_dqsfifop_dlysel                     :  2;  // Bits 17:16
    UINT32 dq4_dqsfifon_dlysel                     :  2;  // Bits 19:18
    UINT32 dq5_dqsfifop_dlysel                     :  2;  // Bits 21:20
    UINT32 dq5_dqsfifon_dlysel                     :  2;  // Bits 23:22
    UINT32 dq6_dqsfifop_dlysel                     :  2;  // Bits 25:24
    UINT32 dq6_dqsfifon_dlysel                     :  2;  // Bits 27:26
    UINT32 dq7_dqsfifop_dlysel                     :  2;  // Bits 29:28
    UINT32 dq7_dqsfifon_dlysel                     :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 dqdbi_byte0_dqsfifop_dlysel             :  2;  // Bits 1:0
    UINT32 dqdbi_byte0_dqsfifon_dlysel             :  2;  // Bits 3:2
    UINT32 dqdbi_byte1_dqsfifop_dlysel             :  2;  // Bits 5:4
    UINT32 dqdbi_byte1_dqsfifon_dlysel             :  2;  // Bits 7:6
    UINT32 Rsvd                                    :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_STRUCT;
typedef union {
  struct {
    UINT32 MixerCode0Ph0                           :  3;  // Bits 2:0
    UINT32 MixerCode1Ph0                           :  3;  // Bits 5:3
    UINT32 MixerCode2Ph0                           :  3;  // Bits 8:6
    UINT32 MixerCode3Ph0                           :  3;  // Bits 11:9
    UINT32 MixerCode4Ph0                           :  3;  // Bits 14:12
    UINT32 MixerCode5Ph0                           :  3;  // Bits 17:15
    UINT32 MixerCode6Ph0                           :  3;  // Bits 20:18
    UINT32 MixerCode7Ph0                           :  3;  // Bits 23:21
    UINT32 MixerCode8Ph0                           :  3;  // Bits 26:24
    UINT32 MixerCode9Ph0                           :  3;  // Bits 29:27
    UINT32 DcaBw                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT;
typedef union {
  struct {
    UINT32 MixerCode10Ph0                          :  3;  // Bits 2:0
    UINT32 MixerCode11Ph0                          :  3;  // Bits 5:3
    UINT32 MixerCode12Ph0                          :  3;  // Bits 8:6
    UINT32 MixerCode13Ph0                          :  3;  // Bits 11:9
    UINT32 MixerCode14Ph0                          :  3;  // Bits 14:12
    UINT32 MixerCode15Ph0                          :  3;  // Bits 17:15
    UINT32 MixerCode0Ph1                           :  3;  // Bits 20:18
    UINT32 MixerCode1Ph1                           :  3;  // Bits 23:21
    UINT32 MixerCode2Ph1                           :  3;  // Bits 26:24
    UINT32 MixerCode3Ph1                           :  3;  // Bits 29:27
    UINT32 CbMix                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT;
typedef union {
  struct {
    UINT32 MixerCode4Ph1                           :  3;  // Bits 2:0
    UINT32 MixerCode5Ph1                           :  3;  // Bits 5:3
    UINT32 MixerCode6Ph1                           :  3;  // Bits 8:6
    UINT32 MixerCode7Ph1                           :  3;  // Bits 11:9
    UINT32 MixerCode8Ph1                           :  3;  // Bits 14:12
    UINT32 MixerCode9Ph1                           :  3;  // Bits 17:15
    UINT32 MixerCode10Ph1                          :  3;  // Bits 20:18
    UINT32 MixerCode11Ph1                          :  3;  // Bits 23:21
    UINT32 MixerCode12Ph1                          :  3;  // Bits 26:24
    UINT32 CbMux                                   :  4;  // Bits 30:27
    UINT32 Rsvd                                    :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT;
typedef union {
  struct {
    UINT32 MixerCode13Ph1                          :  3;  // Bits 2:0
    UINT32 MixerCode14Ph1                          :  3;  // Bits 5:3
    UINT32 MixerCode15Ph1                          :  3;  // Bits 8:6
    UINT32 Rsvd                                    :  23;  // Bits 31:9
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED0_DDRRXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED0_DDRRXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED0_DDRRXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED0_DDRRXDLL_CR_PICODELUT3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRDATA_SHARED0_CR_DLLTIMERCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 Ph4Result                               :  9;  // Bits 8:0
    UINT32 Ph5Result                               :  9;  // Bits 17:9
    UINT32 Ph6Result                               :  9;  // Bits 26:18
    UINT32 rsvd                                    :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT;
typedef union {
  struct {
    UINT32 WrongPhaseCal                           :  2;  // Bits 1:0
    UINT32 WaitWrongPhaseUpd                       :  2;  // Bits 3:2
    UINT32 WaitRefPiCalStart                       :  2;  // Bits 5:4
    UINT32 WaitRefPiCalEnd                         :  2;  // Bits 7:6
    UINT32 WrongPhaseCalDone                       :  1;  // Bits 8:8
    UINT32 WrongPhaseLowLimit                      :  8;  // Bits 16:9
    UINT32 WrongPhaseHighLimit                     :  8;  // Bits 24:17
    UINT32 WrongPhClkOvrd                          :  1;  // Bits 25:25
    UINT32 spare                                   :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRDATA_SHARED1_CR_DCCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED1_CR_DDRCRDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBSCANDATA_STRUCT DDRDATA_SHARED1_CR_DDRCRBSCANDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRDATA_SHARED1_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRDATA_SHARED1_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRDATA_SHARED1_CR_VCCCLKCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_STRUCT DDRDATA_SHARED1_CR_VCCCLKCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRDATA_SHARED1_CR_NTLTEST_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLRSLT_STRUCT DDRDATA_SHARED1_CR_NTLRSLT_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT DDRDATA_SHARED1_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRDATA_SHARED1_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRDATA_SHARED1_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRDATA_SHARED1_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRDATA_SHARED1_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRDATA_SHARED1_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT DDRDATA_SHARED1_CR_RCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_STRUCT DDRDATA_SHARED1_CR_DDRCRDATACONTROL6_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_STRUCT DDRDATA_SHARED1_CR_DDRCRWCKCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED1_CR_DDRCRCCCCH0LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED1_CR_DDRCRCCCCH1LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCPICODES_STRUCT DDRDATA_SHARED1_CR_DDRCRCCCPICODES_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT DDRDATA_SHARED1_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_STRUCT DDRDATA_SHARED1_CR_WCKDCCFSMCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_STRUCT DDRDATA_SHARED1_CR_WCKDCCCALCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_STRUCT DDRDATA_SHARED1_CR_WCKDCCFSMSTATUS_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED1_CR_WCKDCCLANESTATUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED1_CR_WCKDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMP_STRUCT DDRDATA_SHARED1_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_STRUCT DDRDATA_SHARED1_CR_DDRCRCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT DDRDATA_SHARED1_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED1_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED1_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRDATA_SHARED1_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRDATA_SHARED1_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED1_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRDATA_SHARED1_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRDATA_SHARED1_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRDATA_SHARED1_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED1_CR_DFXCCMONBYTE0_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED1_CR_DFXCCMONBYTE1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRDATA_SHARED1_CR_DDR_PMBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRDATA_SHARED1_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRDATA_SHARED1_CR_VCCCLKCOMPTXOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT DDRDATA_SHARED1_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED1_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED1_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT DDRDATA_SHARED1_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_STRUCT DDRDATA_SHARED1_CR_DDRCRCBBBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_STRUCT DDRDATA_SHARED1_CR_REFPIDQSWCKOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRDATA_SHARED1_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED1_CR_DDRCRRXFIFODLYCONTROL0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED1_CR_DDRCRRXFIFODLYCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_STRUCT DDRDATA_SHARED1_CR_DDRCRRXFIFODLYCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED1_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED1_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED1_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED1_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED1_DDRRXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED1_DDRRXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED1_DDRRXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED1_DDRRXDLL_CR_PICODELUT3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRDATA_SHARED1_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRDATA_SHARED1_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRDATA_SHARED1_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRDATA_SHARED2_CR_DCCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED2_CR_DDRCRDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBSCANDATA_STRUCT DDRDATA_SHARED2_CR_DDRCRBSCANDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRDATA_SHARED2_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRDATA_SHARED2_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRDATA_SHARED2_CR_VCCCLKCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_STRUCT DDRDATA_SHARED2_CR_VCCCLKCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRDATA_SHARED2_CR_NTLTEST_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLRSLT_STRUCT DDRDATA_SHARED2_CR_NTLRSLT_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT DDRDATA_SHARED2_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRDATA_SHARED2_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRDATA_SHARED2_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRDATA_SHARED2_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRDATA_SHARED2_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRDATA_SHARED2_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT DDRDATA_SHARED2_CR_RCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_STRUCT DDRDATA_SHARED2_CR_DDRCRDATACONTROL6_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_STRUCT DDRDATA_SHARED2_CR_DDRCRWCKCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED2_CR_DDRCRCCCCH0LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED2_CR_DDRCRCCCCH1LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCPICODES_STRUCT DDRDATA_SHARED2_CR_DDRCRCCCPICODES_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT DDRDATA_SHARED2_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_STRUCT DDRDATA_SHARED2_CR_WCKDCCFSMCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_STRUCT DDRDATA_SHARED2_CR_WCKDCCCALCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_STRUCT DDRDATA_SHARED2_CR_WCKDCCFSMSTATUS_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED2_CR_WCKDCCLANESTATUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED2_CR_WCKDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMP_STRUCT DDRDATA_SHARED2_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_STRUCT DDRDATA_SHARED2_CR_DDRCRCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT DDRDATA_SHARED2_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED2_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED2_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRDATA_SHARED2_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRDATA_SHARED2_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED2_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRDATA_SHARED2_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRDATA_SHARED2_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRDATA_SHARED2_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED2_CR_DFXCCMONBYTE0_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED2_CR_DFXCCMONBYTE1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRDATA_SHARED2_CR_DDR_PMBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRDATA_SHARED2_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRDATA_SHARED2_CR_VCCCLKCOMPTXOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT DDRDATA_SHARED2_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED2_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED2_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT DDRDATA_SHARED2_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_STRUCT DDRDATA_SHARED2_CR_DDRCRCBBBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_STRUCT DDRDATA_SHARED2_CR_REFPIDQSWCKOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRDATA_SHARED2_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED2_CR_DDRCRRXFIFODLYCONTROL0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED2_CR_DDRCRRXFIFODLYCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_STRUCT DDRDATA_SHARED2_CR_DDRCRRXFIFODLYCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED2_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED2_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED2_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED2_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED2_DDRRXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED2_DDRRXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED2_DDRRXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED2_DDRRXDLL_CR_PICODELUT3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRDATA_SHARED2_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRDATA_SHARED2_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRDATA_SHARED2_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRDATA_SHARED3_CR_DCCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED3_CR_DDRCRDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBSCANDATA_STRUCT DDRDATA_SHARED3_CR_DDRCRBSCANDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRDATA_SHARED3_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRDATA_SHARED3_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRDATA_SHARED3_CR_VCCCLKCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_STRUCT DDRDATA_SHARED3_CR_VCCCLKCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRDATA_SHARED3_CR_NTLTEST_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLRSLT_STRUCT DDRDATA_SHARED3_CR_NTLRSLT_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT DDRDATA_SHARED3_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRDATA_SHARED3_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRDATA_SHARED3_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRDATA_SHARED3_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRDATA_SHARED3_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRDATA_SHARED3_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT DDRDATA_SHARED3_CR_RCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_STRUCT DDRDATA_SHARED3_CR_DDRCRDATACONTROL6_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_STRUCT DDRDATA_SHARED3_CR_DDRCRWCKCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED3_CR_DDRCRCCCCH0LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED3_CR_DDRCRCCCCH1LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCPICODES_STRUCT DDRDATA_SHARED3_CR_DDRCRCCCPICODES_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT DDRDATA_SHARED3_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_STRUCT DDRDATA_SHARED3_CR_WCKDCCFSMCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_STRUCT DDRDATA_SHARED3_CR_WCKDCCCALCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_STRUCT DDRDATA_SHARED3_CR_WCKDCCFSMSTATUS_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED3_CR_WCKDCCLANESTATUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED3_CR_WCKDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMP_STRUCT DDRDATA_SHARED3_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_STRUCT DDRDATA_SHARED3_CR_DDRCRCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT DDRDATA_SHARED3_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED3_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED3_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRDATA_SHARED3_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRDATA_SHARED3_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED3_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRDATA_SHARED3_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRDATA_SHARED3_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRDATA_SHARED3_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED3_CR_DFXCCMONBYTE0_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED3_CR_DFXCCMONBYTE1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRDATA_SHARED3_CR_DDR_PMBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRDATA_SHARED3_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRDATA_SHARED3_CR_VCCCLKCOMPTXOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT DDRDATA_SHARED3_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED3_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED3_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT DDRDATA_SHARED3_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_STRUCT DDRDATA_SHARED3_CR_DDRCRCBBBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_STRUCT DDRDATA_SHARED3_CR_REFPIDQSWCKOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRDATA_SHARED3_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED3_CR_DDRCRRXFIFODLYCONTROL0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED3_CR_DDRCRRXFIFODLYCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_STRUCT DDRDATA_SHARED3_CR_DDRCRRXFIFODLYCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED3_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED3_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED3_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED3_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED3_DDRRXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED3_DDRRXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED3_DDRRXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED3_DDRRXDLL_CR_PICODELUT3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRDATA_SHARED3_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRDATA_SHARED3_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRDATA_SHARED3_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRDATA_SHARED4_CR_DCCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED4_CR_DDRCRDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBSCANDATA_STRUCT DDRDATA_SHARED4_CR_DDRCRBSCANDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRDATA_SHARED4_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRDATA_SHARED4_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRDATA_SHARED4_CR_VCCCLKCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_STRUCT DDRDATA_SHARED4_CR_VCCCLKCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRDATA_SHARED4_CR_NTLTEST_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLRSLT_STRUCT DDRDATA_SHARED4_CR_NTLRSLT_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT DDRDATA_SHARED4_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRDATA_SHARED4_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRDATA_SHARED4_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRDATA_SHARED4_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRDATA_SHARED4_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRDATA_SHARED4_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT DDRDATA_SHARED4_CR_RCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_STRUCT DDRDATA_SHARED4_CR_DDRCRDATACONTROL6_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_STRUCT DDRDATA_SHARED4_CR_DDRCRWCKCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED4_CR_DDRCRCCCCH0LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED4_CR_DDRCRCCCCH1LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCPICODES_STRUCT DDRDATA_SHARED4_CR_DDRCRCCCPICODES_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT DDRDATA_SHARED4_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_STRUCT DDRDATA_SHARED4_CR_WCKDCCFSMCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_STRUCT DDRDATA_SHARED4_CR_WCKDCCCALCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_STRUCT DDRDATA_SHARED4_CR_WCKDCCFSMSTATUS_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED4_CR_WCKDCCLANESTATUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED4_CR_WCKDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMP_STRUCT DDRDATA_SHARED4_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_STRUCT DDRDATA_SHARED4_CR_DDRCRCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT DDRDATA_SHARED4_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED4_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED4_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRDATA_SHARED4_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRDATA_SHARED4_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED4_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRDATA_SHARED4_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRDATA_SHARED4_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRDATA_SHARED4_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED4_CR_DFXCCMONBYTE0_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED4_CR_DFXCCMONBYTE1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRDATA_SHARED4_CR_DDR_PMBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRDATA_SHARED4_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRDATA_SHARED4_CR_VCCCLKCOMPTXOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT DDRDATA_SHARED4_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED4_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED4_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT DDRDATA_SHARED4_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_STRUCT DDRDATA_SHARED4_CR_DDRCRCBBBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_STRUCT DDRDATA_SHARED4_CR_REFPIDQSWCKOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRDATA_SHARED4_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED4_CR_DDRCRRXFIFODLYCONTROL0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED4_CR_DDRCRRXFIFODLYCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_STRUCT DDRDATA_SHARED4_CR_DDRCRRXFIFODLYCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED4_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED4_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED4_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED4_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED4_DDRRXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED4_DDRRXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED4_DDRRXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED4_DDRRXDLL_CR_PICODELUT3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRDATA_SHARED4_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRDATA_SHARED4_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRDATA_SHARED4_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRDATA_SHARED5_CR_DCCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED5_CR_DDRCRDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBSCANDATA_STRUCT DDRDATA_SHARED5_CR_DDRCRBSCANDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRDATA_SHARED5_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRDATA_SHARED5_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRDATA_SHARED5_CR_VCCCLKCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_STRUCT DDRDATA_SHARED5_CR_VCCCLKCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRDATA_SHARED5_CR_NTLTEST_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLRSLT_STRUCT DDRDATA_SHARED5_CR_NTLRSLT_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT DDRDATA_SHARED5_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRDATA_SHARED5_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRDATA_SHARED5_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRDATA_SHARED5_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRDATA_SHARED5_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRDATA_SHARED5_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT DDRDATA_SHARED5_CR_RCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_STRUCT DDRDATA_SHARED5_CR_DDRCRDATACONTROL6_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_STRUCT DDRDATA_SHARED5_CR_DDRCRWCKCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED5_CR_DDRCRCCCCH0LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED5_CR_DDRCRCCCCH1LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCPICODES_STRUCT DDRDATA_SHARED5_CR_DDRCRCCCPICODES_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT DDRDATA_SHARED5_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_STRUCT DDRDATA_SHARED5_CR_WCKDCCFSMCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_STRUCT DDRDATA_SHARED5_CR_WCKDCCCALCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_STRUCT DDRDATA_SHARED5_CR_WCKDCCFSMSTATUS_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED5_CR_WCKDCCLANESTATUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED5_CR_WCKDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMP_STRUCT DDRDATA_SHARED5_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_STRUCT DDRDATA_SHARED5_CR_DDRCRCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT DDRDATA_SHARED5_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED5_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED5_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRDATA_SHARED5_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRDATA_SHARED5_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED5_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRDATA_SHARED5_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRDATA_SHARED5_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRDATA_SHARED5_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED5_CR_DFXCCMONBYTE0_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED5_CR_DFXCCMONBYTE1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRDATA_SHARED5_CR_DDR_PMBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRDATA_SHARED5_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRDATA_SHARED5_CR_VCCCLKCOMPTXOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT DDRDATA_SHARED5_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED5_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED5_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT DDRDATA_SHARED5_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_STRUCT DDRDATA_SHARED5_CR_DDRCRCBBBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_STRUCT DDRDATA_SHARED5_CR_REFPIDQSWCKOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRDATA_SHARED5_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED5_CR_DDRCRRXFIFODLYCONTROL0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED5_CR_DDRCRRXFIFODLYCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_STRUCT DDRDATA_SHARED5_CR_DDRCRRXFIFODLYCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED5_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED5_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED5_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED5_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED5_DDRRXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED5_DDRRXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED5_DDRRXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED5_DDRRXDLL_CR_PICODELUT3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRDATA_SHARED5_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRDATA_SHARED5_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRDATA_SHARED5_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRDATA_SHARED6_CR_DCCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED6_CR_DDRCRDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBSCANDATA_STRUCT DDRDATA_SHARED6_CR_DDRCRBSCANDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRDATA_SHARED6_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRDATA_SHARED6_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRDATA_SHARED6_CR_VCCCLKCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_STRUCT DDRDATA_SHARED6_CR_VCCCLKCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRDATA_SHARED6_CR_NTLTEST_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLRSLT_STRUCT DDRDATA_SHARED6_CR_NTLRSLT_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT DDRDATA_SHARED6_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRDATA_SHARED6_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRDATA_SHARED6_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRDATA_SHARED6_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRDATA_SHARED6_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRDATA_SHARED6_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT DDRDATA_SHARED6_CR_RCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_STRUCT DDRDATA_SHARED6_CR_DDRCRDATACONTROL6_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_STRUCT DDRDATA_SHARED6_CR_DDRCRWCKCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED6_CR_DDRCRCCCCH0LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED6_CR_DDRCRCCCCH1LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCPICODES_STRUCT DDRDATA_SHARED6_CR_DDRCRCCCPICODES_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT DDRDATA_SHARED6_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_STRUCT DDRDATA_SHARED6_CR_WCKDCCFSMCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_STRUCT DDRDATA_SHARED6_CR_WCKDCCCALCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_STRUCT DDRDATA_SHARED6_CR_WCKDCCFSMSTATUS_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED6_CR_WCKDCCLANESTATUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED6_CR_WCKDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMP_STRUCT DDRDATA_SHARED6_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_STRUCT DDRDATA_SHARED6_CR_DDRCRCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT DDRDATA_SHARED6_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED6_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED6_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRDATA_SHARED6_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRDATA_SHARED6_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED6_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRDATA_SHARED6_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRDATA_SHARED6_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRDATA_SHARED6_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED6_CR_DFXCCMONBYTE0_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED6_CR_DFXCCMONBYTE1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRDATA_SHARED6_CR_DDR_PMBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRDATA_SHARED6_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRDATA_SHARED6_CR_VCCCLKCOMPTXOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT DDRDATA_SHARED6_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED6_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED6_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT DDRDATA_SHARED6_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_STRUCT DDRDATA_SHARED6_CR_DDRCRCBBBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_STRUCT DDRDATA_SHARED6_CR_REFPIDQSWCKOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRDATA_SHARED6_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED6_CR_DDRCRRXFIFODLYCONTROL0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED6_CR_DDRCRRXFIFODLYCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_STRUCT DDRDATA_SHARED6_CR_DDRCRRXFIFODLYCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED6_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED6_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED6_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED6_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED6_DDRRXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED6_DDRRXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED6_DDRRXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED6_DDRRXDLL_CR_PICODELUT3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRDATA_SHARED6_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRDATA_SHARED6_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRDATA_SHARED6_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRDATA_SHARED7_CR_DCCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED7_CR_DDRCRDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBSCANDATA_STRUCT DDRDATA_SHARED7_CR_DDRCRBSCANDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRDATA_SHARED7_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRDATA_SHARED7_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRDATA_SHARED7_CR_VCCCLKCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_STRUCT DDRDATA_SHARED7_CR_VCCCLKCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRDATA_SHARED7_CR_NTLTEST_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLRSLT_STRUCT DDRDATA_SHARED7_CR_NTLRSLT_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT DDRDATA_SHARED7_CR_DDRCRCOMPDQSDELAYCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRDATA_SHARED7_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRDATA_SHARED7_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRDATA_SHARED7_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRDATA_SHARED7_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRDATA_SHARED7_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_STRUCT DDRDATA_SHARED7_CR_RCOMPDATA_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_STRUCT DDRDATA_SHARED7_CR_DDRCRDATACONTROL6_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_STRUCT DDRDATA_SHARED7_CR_DDRCRWCKCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED7_CR_DDRCRCCCCH0LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_STRUCT DDRDATA_SHARED7_CR_DDRCRCCCCH1LANE0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCCCPICODES_STRUCT DDRDATA_SHARED7_CR_DDRCRCCCPICODES_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT DDRDATA_SHARED7_CR_DDRCRWCKPERBITDESKEWRISEFALL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_STRUCT DDRDATA_SHARED7_CR_WCKDCCFSMCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_STRUCT DDRDATA_SHARED7_CR_WCKDCCCALCCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_STRUCT DDRDATA_SHARED7_CR_WCKDCCFSMSTATUS_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED7_CR_WCKDCCLANESTATUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_STRUCT DDRDATA_SHARED7_CR_WCKDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMP_STRUCT DDRDATA_SHARED7_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_STRUCT DDRDATA_SHARED7_CR_DDRCRCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT DDRDATA_SHARED7_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED7_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT DDRDATA_SHARED7_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRDATA_SHARED7_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRDATA_SHARED7_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRDATA_SHARED7_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRDATA_SHARED7_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRDATA_SHARED7_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRDATA_SHARED7_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED7_CR_DFXCCMONBYTE0_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT DDRDATA_SHARED7_CR_DFXCCMONBYTE1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRDATA_SHARED7_CR_DDR_PMBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRDATA_SHARED7_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRDATA_SHARED7_CR_VCCCLKCOMPTXOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT DDRDATA_SHARED7_CR_DDRCRCLOCKINGBONUSOVRD0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED7_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT DDRDATA_SHARED7_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT DDRDATA_SHARED7_CR_DDRCRWCKDQSBUFFERBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_STRUCT DDRDATA_SHARED7_CR_DDRCRCBBBONUS0_STRUCT;

typedef DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_STRUCT DDRDATA_SHARED7_CR_REFPIDQSWCKOFFSET_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRDATA_SHARED7_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED7_CR_DDRCRRXFIFODLYCONTROL0_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_STRUCT DDRDATA_SHARED7_CR_DDRCRRXFIFODLYCONTROL1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_STRUCT DDRDATA_SHARED7_CR_DDRCRRXFIFODLYCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED7_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED7_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED7_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED7_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT DDRDATA_SHARED7_DDRRXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT DDRDATA_SHARED7_DDRRXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT DDRDATA_SHARED7_DDRRXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT DDRDATA_SHARED7_DDRRXDLL_CR_PICODELUT3_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRDATA_SHARED7_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRDATA_SHARED7_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRDATA_SHARED7_CR_DDRWRONGPHASE_CTRL_STRUCT;



#pragma pack(pop)
#endif /* __MrcRegisterStructPtl2xxx_h__ */







