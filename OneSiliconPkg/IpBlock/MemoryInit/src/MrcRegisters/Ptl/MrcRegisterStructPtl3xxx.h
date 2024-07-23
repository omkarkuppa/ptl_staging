#ifndef __MrcRegisterStructPtl3xxx_h__
#define __MrcRegisterStructPtl3xxx_h__

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


typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRCCC_SHARED0_CR_DDRCRVCCCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRCCC_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRCCC_SHARED0_CR_DDRCRVCCCLKCOMPDATACCC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRCCC_SHARED0_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRCCC_SHARED0_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRCCC_SHARED0_CR_DDRCRDLLOFFSETCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 CtlVoltageSelect                        :  1;  // Bits 0:0
    UINT32 ClkVoltageSelect                        :  1;  // Bits 1:1
    UINT32 CaVoltageSelect                         :  1;  // Bits 2:2
    UINT32 TxCANmosOnly                            :  1;  // Bits 3:3
    UINT32 TxCTLNmosOnly                           :  1;  // Bits 4:4
    UINT32 TxCLKNmosOnly                           :  1;  // Bits 5:5
    UINT32 DdrCaSlwDlyBypass                       :  1;  // Bits 6:6
    UINT32 DdrCtlSlwDlyBypass                      :  1;  // Bits 7:7
    UINT32 CkUseCtlComp                            :  1;  // Bits 8:8
    UINT32 CsUseCaComp                             :  1;  // Bits 9:9
    UINT32 CccStaticLegCtrl                        :  1;  // Bits 10:10
    UINT32 PhaseDrvPiPMEn                          :  2;  // Bits 12:11
    UINT32 PiSyncDivider                           :  2;  // Bits 14:13
    UINT32 PIClkDuration                           :  3;  // Bits 17:15
    UINT32 GearMode                                :  1;  // Bits 18:18
    UINT32 SEClk                                   :  1;  // Bits 19:19
    UINT32 RunRefPIFSM                             :  1;  // Bits 20:20
    UINT32 PreDrvOn                                :  1;  // Bits 21:21
    UINT32 CAUsePi1forPi0                          :  1;  // Bits 22:22
    UINT32 CSUsePi1forPi0                          :  1;  // Bits 23:23
    UINT32 XoverMinDelayCtl                        :  2;  // Bits 25:24
    UINT32 Spare                                   :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SHARED0_CR_DDRCRVOLTAGEUSEDCLKCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 ClkGateDisable                          :  1;  // Bits 0:0
    UINT32 DefDrvEnLow                             :  2;  // Bits 2:1
    UINT32 RTO                                     :  1;  // Bits 3:3
    UINT32 IntCkOn                                 :  1;  // Bits 4:4
    UINT32 CkeIdlePiGateDisable                    :  1;  // Bits 5:5
    UINT32 CaValidPiGateDisable                    :  1;  // Bits 6:6
    UINT32 CaTxEq                                  :  5;  // Bits 11:7
    UINT32 CtlTxEq                                 :  5;  // Bits 16:12
    UINT32 IOBufAct                                :  1;  // Bits 17:17
    UINT32 CAParkIdle                              :  1;  // Bits 18:18
    UINT32 Ddr5Pll                                 :  1;  // Bits 19:19
    UINT32 Ddr5PllLock                             :  2;  // Bits 21:20
    UINT32 Ddr5PllCntDnOvrdEn                      :  1;  // Bits 22:22
    UINT32 Ddr5PllCntDnOvrdVal                     :  1;  // Bits 23:23
    UINT32 spare                                   :  7;  // Bits 30:24
    UINT32 BlockTrainRst                           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRCCC_SHARED0_CR_DCCCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRCCC_SHARED0_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRCCC_SHARED0_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRCCC_SHARED0_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRCCC_SHARED0_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SHARED0_CR_SPARE_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRCCC_SHARED0_CR_DDRCRTXDLLSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 ISenseLVRBonusIn                        :  2;  // Bits 1:0
    UINT32 ISenseLVRBonusOut                       :  2;  // Bits 3:2
    UINT32 thermsensoren                           :  1;  // Bits 4:4
    UINT32 anaviewcrossbaren0                      :  1;  // Bits 5:5
    UINT32 anaviewcrossbaren1                      :  1;  // Bits 6:6
    UINT32 vsxhibypassen_onsag                     :  1;  // Bits 7:7
    UINT32 picodemsbovrd                           :  3;  // Bits 10:8
    UINT32 phenovrd                                :  1;  // Bits 11:11
    UINT32 vioglvr_cmn_marginmode                  :  1;  // Bits 12:12
    UINT32 Spare                                   :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SHARED0_CR_DDR_PMBONUS0_STRUCT;
typedef union {
  struct {
    UINT32 tPiEnAll2PiDis                          :  5;  // Bits 4:0
    UINT32 tPiDis2ResetDeassert                    :  5;  // Bits 9:5
    UINT32 tResetDeassert2PiEn                     :  5;  // Bits 14:10
    UINT32 tPiEn2IOBufAct                          :  5;  // Bits 19:15
    UINT32 tIOBufAct2Done                          :  5;  // Bits 24:20
    UINT32 pienovrden                              :  1;  // Bits 25:25
    UINT32 pienovrdval                             :  1;  // Bits 26:26
    UINT32 resetpiovrden                           :  1;  // Bits 27:27
    UINT32 resetpiovrdval                          :  1;  // Bits 28:28
    UINT32 iobufactovrden                          :  1;  // Bits 29:29
    UINT32 iobufactovrdval                         :  1;  // Bits 30:30
    UINT32 qualifydllwakeup                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SHARED0_CR_DDRCRCCCDLLWAKEUPCONTROLS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_VCCDLLCOMPCCC_STRUCT DDRCCC_SHARED0_CR_DDRCRVCCDLLCOMPCCC_STRUCT;
typedef union {
  struct {
    INT32  FFCodeTxDllCCCOffset                    :  5;  // Bits 4:0
    UINT32 Spare                                   :  27;  // Bits 31:5
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SHARED0_CR_DDRCRVCCDLLCOMPCCCOFFSET_STRUCT;
typedef union {
  struct {
    UINT32 phbufcbb_i_bonus                        :  4;  // Bits 3:0
    UINT32 phenableana_i_bonus                     :  2;  // Bits 5:4
    UINT32 txdlltopcbb_i_bonus                     :  10;  // Bits 15:6
    UINT32 weaklockovrden                          :  1;  // Bits 16:16
    UINT32 weaklockovrdval                         :  1;  // Bits 17:17
    UINT32 txdllpowerdownovrden                    :  1;  // Bits 18:18
    UINT32 txdllpowerdownovrdval                   :  1;  // Bits 19:19
    UINT32 channelnotpopqual                       :  1;  // Bits 20:20
    UINT32 enpowerdownmodelp4                      :  1;  // Bits 21:21
    UINT32 enweaklockonlp2                         :  1;  // Bits 22:22
    UINT32 enweaklockonlp3                         :  1;  // Bits 23:23
    UINT32 enweaklockonlp4                         :  1;  // Bits 24:24
    UINT32 EnDacCodeOutOvrd                        :  1;  // Bits 25:25
    UINT32 CCCPiEnDuration                         :  4;  // Bits 29:26
    UINT32 disdllqual                              :  1;  // Bits 30:30
    UINT32 cccgear4en                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SHARED0_CR_CBB_BONUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRCCC_SHARED0_CR_NTLTEST_STRUCT;
typedef union {
  struct {
    UINT32 ntl_done                                :  1;  // Bits 0:0
    UINT32 ntl_ppfail_g0                           :  9;  // Bits 9:1
    UINT32 ntl_ppfail_g1                           :  9;  // Bits 18:10
    UINT32 spare                                   :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SHARED0_CR_NTLRSLT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRCCC_SHARED0_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRCCC_SHARED0_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRCCC_SHARED0_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRCCC_SHARED0_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRCCC_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRCCC_SHARED0_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRCCC_SHARED1_CR_DDRCRVCCCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRCCC_SHARED1_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRCCC_SHARED1_CR_DDRCRVCCCLKCOMPDATACCC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRCCC_SHARED1_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRCCC_SHARED1_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRCCC_SHARED1_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRVOLTAGEUSEDCLKCONTROL_STRUCT DDRCCC_SHARED1_CR_DDRCRVOLTAGEUSEDCLKCONTROL_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_STRUCT DDRCCC_SHARED1_CR_DDRCRCCCPINCONTROLS_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRCCC_SHARED1_CR_DCCCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRCCC_SHARED1_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRCCC_SHARED1_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRCCC_SHARED1_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRCCC_SHARED1_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRCCC_SHARED1_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SHARED1_CR_SPARE_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRCCC_SHARED1_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRCCC_SHARED1_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRCCC_SHARED1_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRCCC_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRCCC_SHARED1_CR_DDR_PMBONUS0_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRCCCDLLWAKEUPCONTROLS_STRUCT DDRCCC_SHARED1_CR_DDRCRCCCDLLWAKEUPCONTROLS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_VCCDLLCOMPCCC_STRUCT DDRCCC_SHARED1_CR_DDRCRVCCDLLCOMPCCC_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRVCCDLLCOMPCCCOFFSET_STRUCT DDRCCC_SHARED1_CR_DDRCRVCCDLLCOMPCCCOFFSET_STRUCT;

typedef DDRCCC_SHARED0_CR_CBB_BONUS1_STRUCT DDRCCC_SHARED1_CR_CBB_BONUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRCCC_SHARED1_CR_NTLTEST_STRUCT;

typedef DDRCCC_SHARED0_CR_NTLRSLT_STRUCT DDRCCC_SHARED1_CR_NTLRSLT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRCCC_SHARED1_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRCCC_SHARED1_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRCCC_SHARED1_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRCCC_SHARED1_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRCCC_SHARED1_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRCCC_SHARED1_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRCCC_SHARED2_CR_DDRCRVCCCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRCCC_SHARED2_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRCCC_SHARED2_CR_DDRCRVCCCLKCOMPDATACCC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRCCC_SHARED2_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRCCC_SHARED2_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRCCC_SHARED2_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRVOLTAGEUSEDCLKCONTROL_STRUCT DDRCCC_SHARED2_CR_DDRCRVOLTAGEUSEDCLKCONTROL_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_STRUCT DDRCCC_SHARED2_CR_DDRCRCCCPINCONTROLS_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRCCC_SHARED2_CR_DCCCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRCCC_SHARED2_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRCCC_SHARED2_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRCCC_SHARED2_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRCCC_SHARED2_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRCCC_SHARED2_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SHARED2_CR_SPARE_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRCCC_SHARED2_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRCCC_SHARED2_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRCCC_SHARED2_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRCCC_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRCCC_SHARED2_CR_DDR_PMBONUS0_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRCCCDLLWAKEUPCONTROLS_STRUCT DDRCCC_SHARED2_CR_DDRCRCCCDLLWAKEUPCONTROLS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_VCCDLLCOMPCCC_STRUCT DDRCCC_SHARED2_CR_DDRCRVCCDLLCOMPCCC_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRVCCDLLCOMPCCCOFFSET_STRUCT DDRCCC_SHARED2_CR_DDRCRVCCDLLCOMPCCCOFFSET_STRUCT;

typedef DDRCCC_SHARED0_CR_CBB_BONUS1_STRUCT DDRCCC_SHARED2_CR_CBB_BONUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRCCC_SHARED2_CR_NTLTEST_STRUCT;

typedef DDRCCC_SHARED0_CR_NTLRSLT_STRUCT DDRCCC_SHARED2_CR_NTLRSLT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRCCC_SHARED2_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRCCC_SHARED2_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRCCC_SHARED2_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRCCC_SHARED2_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRCCC_SHARED2_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRCCC_SHARED2_CR_DLLTIMERCONTROL_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_STRUCT DDRCCC_SHARED3_CR_DDRCRVCCCLKCOMPOFFSET_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_STRUCT DDRCCC_SHARED3_CR_DDRCRBWSELCOMPDATACCC_STRUCT;

typedef DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_STRUCT DDRCCC_SHARED3_CR_DDRCRVCCCLKCOMPDATACCC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_STRUCT DDRCCC_SHARED3_CR_LVRCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT DDRCCC_SHARED3_CR_LVRCURRENTSENSORCOUNT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_STRUCT DDRCCC_SHARED3_CR_DDRCRDLLOFFSETCONTROL_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRVOLTAGEUSEDCLKCONTROL_STRUCT DDRCCC_SHARED3_CR_DDRCRVOLTAGEUSEDCLKCONTROL_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_STRUCT DDRCCC_SHARED3_CR_DDRCRCCCPINCONTROLS_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_STRUCT DDRCCC_SHARED3_CR_DCCCONTROL_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_STRUCT DDRCCC_SHARED3_CR_PHDCCLANESTATUS0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_STRUCT DDRCCC_SHARED3_CR_PHDCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_STRUCT DDRCCC_SHARED3_CR_PHCLOCKREPEATER0_STRUCT;

typedef DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_STRUCT DDRCCC_SHARED3_CR_PHCLOCKREPEATER1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_STRUCT DDRCCC_SHARED3_CR_DDRCRTXDLLCONTROL1_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SHARED3_CR_SPARE_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_STRUCT DDRCCC_SHARED3_CR_DDRCRTXDLLCONTROL2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT DDRCCC_SHARED3_CR_DDRCRTXDLLCONTROL0_STRUCT;

typedef DATA0CH0_CR_DDRCRDLLSTATUS_STRUCT DDRCCC_SHARED3_CR_DDRCRTXDLLSTATUS_STRUCT;

typedef DDRCCC_SHARED0_CR_DDR_PMBONUS0_STRUCT DDRCCC_SHARED3_CR_DDR_PMBONUS0_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRCCCDLLWAKEUPCONTROLS_STRUCT DDRCCC_SHARED3_CR_DDRCRCCCDLLWAKEUPCONTROLS_STRUCT;

typedef DDRPHY_DDRCOMP_CR_VCCDLLCOMPCCC_STRUCT DDRCCC_SHARED3_CR_DDRCRVCCDLLCOMPCCC_STRUCT;

typedef DDRCCC_SHARED0_CR_DDRCRVCCDLLCOMPCCCOFFSET_STRUCT DDRCCC_SHARED3_CR_DDRCRVCCDLLCOMPCCCOFFSET_STRUCT;

typedef DDRCCC_SHARED0_CR_CBB_BONUS1_STRUCT DDRCCC_SHARED3_CR_CBB_BONUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_NTLTEST_STRUCT DDRCCC_SHARED3_CR_NTLTEST_STRUCT;

typedef DDRCCC_SHARED0_CR_NTLRSLT_STRUCT DDRCCC_SHARED3_CR_NTLRSLT_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_STRUCT DDRCCC_SHARED3_CR_DDRCRDLLCBTUNEOVRD_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_STRUCT DDRCCC_SHARED3_CR_PHCLOCKREPEATER2_STRUCT;

typedef DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_STRUCT DDRCCC_SHARED3_CR_PHCLOCKREPEATER3_STRUCT;

typedef DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_STRUCT DDRCCC_SHARED3_CR_PHDCCLANESTATUS2_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_STRUCT DDRCCC_SHARED3_CR_DDRWRONGPHASE_CTRL_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_STRUCT DDRCCC_SHARED3_CR_DLLTIMERCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 ucss_sram_data                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_DATA_STRUCT;
typedef union {
  struct {
    UINT32 ucss_sram_addr                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_ADDR_STRUCT;
typedef union {
  struct {
    UINT32 fw_dnld_done                            :  1;  // Bits 0:0
    UINT32 fw_dnld_auto_incr_dis                   :  1;  // Bits 1:1
    UINT32 sram_active_arb_en                      :  1;  // Bits 2:2
    UINT32 spare                                   :  29;  // Bits 31:3
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_FW_DNLD_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 xt_soft_reset                           :  1;  // Bits 0:0
    UINT32 xt_runstall                             :  1;  // Bits 1:1
    UINT32 xt_debug_en                             :  1;  // Bits 2:2
    UINT32 xt_ocdhaltonrst                         :  1;  // Bits 3:3
    UINT32 xt_alt_vec_sel                          :  1;  // Bits 4:4
    UINT32 xt_breset_cnt                           :  4;  // Bits 8:5
    UINT32 xt_runstall_cnt                         :  4;  // Bits 12:9
    UINT32 xt_dreset_ovr                           :  1;  // Bits 13:13
    UINT32 xt_dreset_ovr_val                       :  1;  // Bits 14:14
    UINT32 xt_breset_ovr                           :  1;  // Bits 15:15
    UINT32 xt_breset_ovr_val                       :  1;  // Bits 16:16
    UINT32 xt_waiti_wake_intrpt                    :  1;  // Bits 17:17
    UINT32 xt_sram_fastsleep_en                    :  1;  // Bits 18:18
    UINT32 spare                                   :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 xt_pwaitmode                            :  1;  // Bits 0:0
    UINT32 xt_xocdmode                             :  1;  // Bits 1:1
    UINT32 xt_debugmode                            :  1;  // Bits 2:2
    UINT32 xt_pfatalerror                          :  1;  // Bits 3:3
    UINT32 xt_ecc_corr_err                         :  1;  // Bits 4:4
    UINT32 xt_halt                                 :  1;  // Bits 5:5
    UINT32 xt_double_exception_error               :  1;  // Bits 6:6
    UINT32 xt_prefbuf_ecc_corr_err                 :  1;  // Bits 7:7
    UINT32 xt_prefbuf_ecc_uncorr_err               :  1;  // Bits 8:8
    UINT32 xt_sram_pg_status                       :  1;  // Bits 9:9
    UINT32 spare                                   :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 xt_debug_pc                             :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_DEBUG_PC_STRUCT;
typedef union {
  struct {
    UINT32 pfaultinfo0                             :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_PFAULTINFO0_STRUCT;
typedef union {
  struct {
    UINT32 pfaultinfo1                             :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_PFAULTINFO1_STRUCT;
typedef union {
  struct {
    UINT32 ecc_fe_corr_err_count_clr               :  1;  // Bits 0:0
    UINT32 ecc_ne_corr_err_count_clr               :  1;  // Bits 1:1
    UINT32 ecc_uncorr_status_clr                   :  1;  // Bits 2:2
    UINT32 fesram_ecc_en                           :  1;  // Bits 3:3
    UINT32 nesram_ecc_en                           :  1;  // Bits 4:4
    UINT32 tracemem_ecc_en                         :  1;  // Bits 5:5
    UINT32 ecc_tracemem_corr_err_count_clr         :  1;  // Bits 6:6
    UINT32 ecc_tracemem_uncorr_status_clr          :  1;  // Bits 7:7
    UINT32 spare                                   :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 ne_iram0_ecc_corr_count                 :  8;  // Bits 7:0
    UINT32 ne_iram1_ecc_corr_count                 :  8;  // Bits 15:8
    UINT32 ne_dram0_ecc_corr_count                 :  8;  // Bits 23:16
    UINT32 ne_iram0_ecc_uncorr_err                 :  1;  // Bits 24:24
    UINT32 ne_iram1_ecc_uncorr_err                 :  1;  // Bits 25:25
    UINT32 ne_dram0_ecc_uncorr_err                 :  1;  // Bits 26:26
    UINT32 spare                                   :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_NE_ECC_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 fe_ecc_corr_count                       :  10;  // Bits 9:0
    UINT32 fe_ecc_uncorr_err                       :  1;  // Bits 10:10
    UINT32 spare                                   :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_FE_ECC_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 tracemem_ecc_corr_count                 :  8;  // Bits 7:0
    UINT32 tracemem_ecc_uncorr_err                 :  1;  // Bits 8:8
    UINT32 spare                                   :  23;  // Bits 31:9
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_TRACEMEM_ECC_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 direct_load_data                        :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_DATA_STRUCT;
typedef union {
  struct {
    UINT32 direct_load_start_addr                  :  18;  // Bits 17:0
    UINT32 Spare                                   :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_START_ADDR_STRUCT;
typedef union {
  struct {
    UINT32 direct_load_stop_addr                   :  18;  // Bits 17:0
    UINT32 direct_load_decrement                   :  1;  // Bits 18:18
    UINT32 start_direct_load                       :  1;  // Bits 19:19
    UINT32 cri_clkgate_dis_ucss                    :  1;  // Bits 20:20
    UINT32 iosf_clkgate_dis_ucss                   :  1;  // Bits 21:21
    UINT32 ne_start_direct_load                    :  1;  // Bits 22:22
    UINT32 skip_dram0_direct_load                  :  1;  // Bits 23:23
    UINT32 skip_iram0_direct_load                  :  1;  // Bits 24:24
    UINT32 skip_iram1_direct_load                  :  1;  // Bits 25:25
    UINT32 Spare                                   :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_STOP_ADDR_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH0CCC_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH0CCC_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH0CCC_CR_DDRCRCACOMP_STRUCT;
typedef union {
  struct {
    INT32  CtlScompOffset                          :  6;  // Bits 5:0
    INT32  CtlRcompDrvOffset                       :  6;  // Bits 11:6
    INT32  ClkRcompDrvOffset                       :  6;  // Bits 17:12
    INT32  CaScompOffset                           :  6;  // Bits 23:18
    INT32  CaRcompDrvOffset                        :  6;  // Bits 29:24
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT;
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
} CH0CCC_CR_DDRCRCCCLANE0_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH0CCC_CR_DDRCRCCCLANE1_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH0CCC_CR_DDRCRCCCLANE2_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH0CCC_CR_DDRCRCCCLANE3_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH0CCC_CR_DDRCRCCCLANE4_STRUCT;
typedef union {
  struct {
    UINT32 PiCode                                  :  6;  // Bits 5:0
    UINT32 rsvd1                                   :  13;  // Bits 18:6
    UINT32 PiDivEn                                 :  1;  // Bits 19:19
    UINT32 PiDivInc                                :  2;  // Bits 21:20
    UINT32 LaneEn                                  :  1;  // Bits 22:22
    UINT32 PiEn                                    :  1;  // Bits 23:23
    UINT32 rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DDRCRCCCLANE5_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH0CCC_CR_DDRCRCCCLANE6_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH0CCC_CR_DDRCRCCCLANE7_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH0CCC_CR_DDRCRCCCLANE8_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH0CCC_CR_DDRCRCCCLANE9_STRUCT;
typedef union {
  struct {
    UINT32 CAPICode                                :  9;  // Bits 8:0
    UINT32 CLKPICode                               :  9;  // Bits 17:9
    UINT32 CTLPICode                               :  9;  // Bits 26:18
    UINT32 ZeroRefPiLatency                        :  1;  // Bits 27:27
    UINT32 PiEnOvrd                                :  1;  // Bits 28:28
    UINT32 CCCMuxSelect                            :  2;  // Bits 30:29
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DDRCRCCCPICODES_STRUCT;
typedef union {
  struct {
    INT32  PfNr5                                   :  5;  // Bits 4:0
    INT32  PrNf5                                   :  5;  // Bits 9:5
    UINT32 spare0                                  :  5;  // Bits 14:10
    UINT32 spare                                   :  5;  // Bits 19:15
    UINT32 EnClkFeedback                           :  2;  // Bits 21:20
    UINT32 PfNrFeedback                            :  2;  // Bits 23:22
    UINT32 PrNfFeedback                            :  2;  // Bits 25:24
    UINT32 rsvd                                    :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT;
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
    UINT32 pidccen                                 :  1;  // Bits 28:28
    UINT32 UpdateTcoComp                           :  1;  // Bits 29:29
    UINT32 rsvd1                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DCCFSMCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 DccStepSize                             :  5;  // Bits 4:0
    UINT32 CKTargetElse                            :  5;  // Bits 9:5
    INT32  CKTarget5                               :  5;  // Bits 14:10
    UINT32 CKTarget67                              :  5;  // Bits 19:15
    UINT32 EnLane12                                :  1;  // Bits 20:20
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
} CH0CCC_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT CH0CCC_CR_REFPICONTROL_STRUCT;
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
} CH0CCC_CR_DDRCRBSCANDATA_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT CH0CCC_CR_REFPIFSMCONTROL_STRUCT;
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
} CH0CCC_CR_DCCFSMSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 Lane5Result                             :  9;  // Bits 8:0
    UINT32 Lane67Result                            :  9;  // Bits 17:9
    UINT32 Lane89Result                            :  9;  // Bits 26:18
    UINT32 rsvd                                    :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DCCLANESTATUS0_STRUCT;
typedef union {
  struct {
    UINT32 ExtremeOffset                           :  7;  // Bits 6:0
    UINT32 ExtremeCount                            :  9;  // Bits 15:7
    UINT32 Lane34Result                            :  9;  // Bits 24:16
    UINT32 rsvd                                    :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT CH0CCC_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH0CCC_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH0CCC_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT CH0CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT CH0CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT CH0CCC_CR_DDRCRADC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT CH0CCC_CR_DFXCCMON_STRUCT;
typedef union {
  struct {
    UINT32 lfsrseed                                :  8;  // Bits 7:0
    UINT32 skiplaneval                             :  10;  // Bits 17:8
    UINT32 skiplaneen                              :  10;  // Bits 27:18
    UINT32 sweepincr                               :  2;  // Bits 29:28
    UINT32 rsvd                                    :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 clk2xcbb_i_bonus                        :  8;  // Bits 7:0
    UINT32 cmd2xcbb_i_bonus                        :  8;  // Bits 15:8
    UINT32 cmdmisccbb_i_bonus                      :  4;  // Bits 19:16
    UINT32 refpidisclkgate                         :  1;  // Bits 20:20
    UINT32 spare                                   :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_CBB_BONUS0_STRUCT;
typedef union {
  struct {
    UINT32 spare                                   :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_BONUS0_STRUCT;
typedef union {
  struct {
    UINT32 RefPi4XoverOffset                       :  7;  // Bits 6:0
    UINT32 RefPi2Pi_full_sweep                     :  1;  // Bits 7:7
    UINT32 Spare                                   :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_REFPICLKOFFSET_STRUCT;
typedef union {
  struct {
    UINT32 CAFLYBY                                 :  3;  // Bits 2:0
    UINT32 CSFLYBY                                 :  3;  // Bits 5:3
    UINT32 CLKFLYBY                                :  3;  // Bits 8:6
    UINT32 CCCBUFFLYBY                             :  12;  // Bits 20:9
    UINT32 rsvd                                    :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_LPMODE4_FLYBY_STRUCT;
typedef union {
  struct {
    UINT32 cccbypdataen                            :  10;  // Bits 9:0
    UINT32 crenpibuf                               :  10;  // Bits 19:10
    UINT32 crenpiref                               :  1;  // Bits 20:20
    UINT32 dispiovrdonlp0p5                        :  1;  // Bits 21:21
    UINT32 spare                                   :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_CCC_OVRDCTL_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT CH0CCC_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT CH0CCC_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT CH0CCC_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT CH0CCC_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT CH0CCC_CR_TERDCCCONTROL0_STRUCT;
typedef union {
  struct {
    INT32  CSGearDownPiCodeRank0                   :  7;  // Bits 6:0
    INT32  CSGearDownPiCodeRank1                   :  7;  // Bits 13:7
    INT32  CSGearDownPiCodeRank2                   :  7;  // Bits 20:14
    INT32  CSGearDownPiCodeRank3                   :  7;  // Bits 27:21
    UINT32 EnCSGearDownOffset                      :  1;  // Bits 28:28
    UINT32 rsvd                                    :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_CS_GEARDOWN_STRUCT;
typedef union {
  struct {
    INT32  TcoCode                                 :  6;  // Bits 5:0
    INT32  DccCode                                 :  7;  // Bits 12:6
    UINT32 rsvd                                    :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT;
typedef union {
  struct {
    UINT32 Ca_rcompcode_compupdt_ovrd_en           :  1;  // Bits 0:0
    UINT32 Ca_rcompcode_compupdt_ovrd_val          :  1;  // Bits 1:1
    UINT32 Ctl_rcompcode_compupdt_ovrd_en          :  1;  // Bits 2:2
    UINT32 Ctl_rcompcode_compupdt_ovrd_val         :  1;  // Bits 3:3
    UINT32 Clk_rcompcode_compupdt_ovrd_en          :  1;  // Bits 4:4
    UINT32 Clk_rcompcode_compupdt_ovrd_val         :  1;  // Bits 5:5
    INT32  RloadDqsOffset                          :  5;  // Bits 10:6
    UINT32 rsvd                                    :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CH0CCC_CR_COMPUPDATEOVERRIDE_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT CH0CCC_CR_SPARE_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH1CCC_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH1CCC_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH1CCC_CR_DDRCRCACOMP_STRUCT;

typedef CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT CH1CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE0_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE1_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE2_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE3_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE4_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE5_STRUCT CH1CCC_CR_DDRCRCCCLANE5_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE6_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE7_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE8_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH1CCC_CR_DDRCRCCCLANE9_STRUCT;

typedef CH0CCC_CR_DDRCRCCCPICODES_STRUCT CH1CCC_CR_DDRCRCCCPICODES_STRUCT;

typedef CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT CH1CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT;

typedef CH0CCC_CR_DCCFSMCONTROL_STRUCT CH1CCC_CR_DCCFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCCALCCONTROL_STRUCT CH1CCC_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT CH1CCC_CR_REFPICONTROL_STRUCT;

typedef CH0CCC_CR_DDRCRBSCANDATA_STRUCT CH1CCC_CR_DDRCRBSCANDATA_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT CH1CCC_CR_REFPIFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCFSMSTATUS_STRUCT CH1CCC_CR_DCCFSMSTATUS_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS0_STRUCT CH1CCC_CR_DCCLANESTATUS0_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS1_STRUCT CH1CCC_CR_DCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT CH1CCC_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH1CCC_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH1CCC_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT CH1CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT CH1CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT CH1CCC_CR_DDRCRADC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT CH1CCC_CR_DFXCCMON_STRUCT;

typedef CH0CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT CH1CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT;

typedef CH0CCC_CR_CBB_BONUS0_STRUCT CH1CCC_CR_CBB_BONUS0_STRUCT;

typedef CH0CCC_CR_BONUS0_STRUCT CH1CCC_CR_BONUS0_STRUCT;

typedef CH0CCC_CR_REFPICLKOFFSET_STRUCT CH1CCC_CR_REFPICLKOFFSET_STRUCT;

typedef CH0CCC_CR_LPMODE4_FLYBY_STRUCT CH1CCC_CR_LPMODE4_FLYBY_STRUCT;

typedef CH0CCC_CR_CCC_OVRDCTL_STRUCT CH1CCC_CR_CCC_OVRDCTL_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT CH1CCC_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT CH1CCC_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT CH1CCC_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT CH1CCC_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT CH1CCC_CR_TERDCCCONTROL0_STRUCT;

typedef CH0CCC_CR_CS_GEARDOWN_STRUCT CH1CCC_CR_CS_GEARDOWN_STRUCT;

typedef CH0CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT CH1CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT;

typedef CH0CCC_CR_COMPUPDATEOVERRIDE_STRUCT CH1CCC_CR_COMPUPDATEOVERRIDE_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT CH1CCC_CR_SPARE_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH2CCC_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH2CCC_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH2CCC_CR_DDRCRCACOMP_STRUCT;

typedef CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT CH2CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE0_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE1_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE2_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE3_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE4_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE5_STRUCT CH2CCC_CR_DDRCRCCCLANE5_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE6_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE7_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE8_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH2CCC_CR_DDRCRCCCLANE9_STRUCT;

typedef CH0CCC_CR_DDRCRCCCPICODES_STRUCT CH2CCC_CR_DDRCRCCCPICODES_STRUCT;

typedef CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT CH2CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT;

typedef CH0CCC_CR_DCCFSMCONTROL_STRUCT CH2CCC_CR_DCCFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCCALCCONTROL_STRUCT CH2CCC_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT CH2CCC_CR_REFPICONTROL_STRUCT;

typedef CH0CCC_CR_DDRCRBSCANDATA_STRUCT CH2CCC_CR_DDRCRBSCANDATA_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT CH2CCC_CR_REFPIFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCFSMSTATUS_STRUCT CH2CCC_CR_DCCFSMSTATUS_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS0_STRUCT CH2CCC_CR_DCCLANESTATUS0_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS1_STRUCT CH2CCC_CR_DCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT CH2CCC_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH2CCC_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH2CCC_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT CH2CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT CH2CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT CH2CCC_CR_DDRCRADC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT CH2CCC_CR_DFXCCMON_STRUCT;

typedef CH0CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT CH2CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT;

typedef CH0CCC_CR_CBB_BONUS0_STRUCT CH2CCC_CR_CBB_BONUS0_STRUCT;

typedef CH0CCC_CR_BONUS0_STRUCT CH2CCC_CR_BONUS0_STRUCT;

typedef CH0CCC_CR_REFPICLKOFFSET_STRUCT CH2CCC_CR_REFPICLKOFFSET_STRUCT;

typedef CH0CCC_CR_LPMODE4_FLYBY_STRUCT CH2CCC_CR_LPMODE4_FLYBY_STRUCT;

typedef CH0CCC_CR_CCC_OVRDCTL_STRUCT CH2CCC_CR_CCC_OVRDCTL_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT CH2CCC_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT CH2CCC_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT CH2CCC_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT CH2CCC_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT CH2CCC_CR_TERDCCCONTROL0_STRUCT;

typedef CH0CCC_CR_CS_GEARDOWN_STRUCT CH2CCC_CR_CS_GEARDOWN_STRUCT;

typedef CH0CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT CH2CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT;

typedef CH0CCC_CR_COMPUPDATEOVERRIDE_STRUCT CH2CCC_CR_COMPUPDATEOVERRIDE_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT CH2CCC_CR_SPARE_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH3CCC_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH3CCC_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH3CCC_CR_DDRCRCACOMP_STRUCT;

typedef CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT CH3CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE0_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE1_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE2_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE3_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE4_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE5_STRUCT CH3CCC_CR_DDRCRCCCLANE5_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE6_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE7_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE8_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH3CCC_CR_DDRCRCCCLANE9_STRUCT;

typedef CH0CCC_CR_DDRCRCCCPICODES_STRUCT CH3CCC_CR_DDRCRCCCPICODES_STRUCT;

typedef CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT CH3CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT;

typedef CH0CCC_CR_DCCFSMCONTROL_STRUCT CH3CCC_CR_DCCFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCCALCCONTROL_STRUCT CH3CCC_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT CH3CCC_CR_REFPICONTROL_STRUCT;

typedef CH0CCC_CR_DDRCRBSCANDATA_STRUCT CH3CCC_CR_DDRCRBSCANDATA_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT CH3CCC_CR_REFPIFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCFSMSTATUS_STRUCT CH3CCC_CR_DCCFSMSTATUS_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS0_STRUCT CH3CCC_CR_DCCLANESTATUS0_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS1_STRUCT CH3CCC_CR_DCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT CH3CCC_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH3CCC_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH3CCC_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT CH3CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT CH3CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT CH3CCC_CR_DDRCRADC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT CH3CCC_CR_DFXCCMON_STRUCT;

typedef CH0CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT CH3CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT;

typedef CH0CCC_CR_CBB_BONUS0_STRUCT CH3CCC_CR_CBB_BONUS0_STRUCT;

typedef CH0CCC_CR_BONUS0_STRUCT CH3CCC_CR_BONUS0_STRUCT;

typedef CH0CCC_CR_REFPICLKOFFSET_STRUCT CH3CCC_CR_REFPICLKOFFSET_STRUCT;

typedef CH0CCC_CR_LPMODE4_FLYBY_STRUCT CH3CCC_CR_LPMODE4_FLYBY_STRUCT;

typedef CH0CCC_CR_CCC_OVRDCTL_STRUCT CH3CCC_CR_CCC_OVRDCTL_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT CH3CCC_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT CH3CCC_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT CH3CCC_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT CH3CCC_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT CH3CCC_CR_TERDCCCONTROL0_STRUCT;

typedef CH0CCC_CR_CS_GEARDOWN_STRUCT CH3CCC_CR_CS_GEARDOWN_STRUCT;

typedef CH0CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT CH3CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT;

typedef CH0CCC_CR_COMPUPDATEOVERRIDE_STRUCT CH3CCC_CR_COMPUPDATEOVERRIDE_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT CH3CCC_CR_SPARE_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH4CCC_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH4CCC_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH4CCC_CR_DDRCRCACOMP_STRUCT;

typedef CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT CH4CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE0_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE1_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE2_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE3_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE4_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE5_STRUCT CH4CCC_CR_DDRCRCCCLANE5_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE6_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE7_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE8_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH4CCC_CR_DDRCRCCCLANE9_STRUCT;

typedef CH0CCC_CR_DDRCRCCCPICODES_STRUCT CH4CCC_CR_DDRCRCCCPICODES_STRUCT;

typedef CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT CH4CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT;

typedef CH0CCC_CR_DCCFSMCONTROL_STRUCT CH4CCC_CR_DCCFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCCALCCONTROL_STRUCT CH4CCC_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT CH4CCC_CR_REFPICONTROL_STRUCT;

typedef CH0CCC_CR_DDRCRBSCANDATA_STRUCT CH4CCC_CR_DDRCRBSCANDATA_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT CH4CCC_CR_REFPIFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCFSMSTATUS_STRUCT CH4CCC_CR_DCCFSMSTATUS_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS0_STRUCT CH4CCC_CR_DCCLANESTATUS0_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS1_STRUCT CH4CCC_CR_DCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT CH4CCC_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH4CCC_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH4CCC_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT CH4CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT CH4CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT CH4CCC_CR_DDRCRADC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT CH4CCC_CR_DFXCCMON_STRUCT;

typedef CH0CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT CH4CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT;

typedef CH0CCC_CR_CBB_BONUS0_STRUCT CH4CCC_CR_CBB_BONUS0_STRUCT;

typedef CH0CCC_CR_BONUS0_STRUCT CH4CCC_CR_BONUS0_STRUCT;

typedef CH0CCC_CR_REFPICLKOFFSET_STRUCT CH4CCC_CR_REFPICLKOFFSET_STRUCT;

typedef CH0CCC_CR_LPMODE4_FLYBY_STRUCT CH4CCC_CR_LPMODE4_FLYBY_STRUCT;

typedef CH0CCC_CR_CCC_OVRDCTL_STRUCT CH4CCC_CR_CCC_OVRDCTL_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT CH4CCC_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT CH4CCC_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT CH4CCC_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT CH4CCC_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT CH4CCC_CR_TERDCCCONTROL0_STRUCT;

typedef CH0CCC_CR_CS_GEARDOWN_STRUCT CH4CCC_CR_CS_GEARDOWN_STRUCT;

typedef CH0CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT CH4CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT;

typedef CH0CCC_CR_COMPUPDATEOVERRIDE_STRUCT CH4CCC_CR_COMPUPDATEOVERRIDE_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT CH4CCC_CR_SPARE_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH5CCC_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH5CCC_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH5CCC_CR_DDRCRCACOMP_STRUCT;

typedef CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT CH5CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE0_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE1_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE2_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE3_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE4_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE5_STRUCT CH5CCC_CR_DDRCRCCCLANE5_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE6_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE7_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE8_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH5CCC_CR_DDRCRCCCLANE9_STRUCT;

typedef CH0CCC_CR_DDRCRCCCPICODES_STRUCT CH5CCC_CR_DDRCRCCCPICODES_STRUCT;

typedef CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT CH5CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT;

typedef CH0CCC_CR_DCCFSMCONTROL_STRUCT CH5CCC_CR_DCCFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCCALCCONTROL_STRUCT CH5CCC_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT CH5CCC_CR_REFPICONTROL_STRUCT;

typedef CH0CCC_CR_DDRCRBSCANDATA_STRUCT CH5CCC_CR_DDRCRBSCANDATA_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT CH5CCC_CR_REFPIFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCFSMSTATUS_STRUCT CH5CCC_CR_DCCFSMSTATUS_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS0_STRUCT CH5CCC_CR_DCCLANESTATUS0_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS1_STRUCT CH5CCC_CR_DCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT CH5CCC_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH5CCC_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH5CCC_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT CH5CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT CH5CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT CH5CCC_CR_DDRCRADC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT CH5CCC_CR_DFXCCMON_STRUCT;

typedef CH0CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT CH5CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT;

typedef CH0CCC_CR_CBB_BONUS0_STRUCT CH5CCC_CR_CBB_BONUS0_STRUCT;

typedef CH0CCC_CR_BONUS0_STRUCT CH5CCC_CR_BONUS0_STRUCT;

typedef CH0CCC_CR_REFPICLKOFFSET_STRUCT CH5CCC_CR_REFPICLKOFFSET_STRUCT;

typedef CH0CCC_CR_LPMODE4_FLYBY_STRUCT CH5CCC_CR_LPMODE4_FLYBY_STRUCT;

typedef CH0CCC_CR_CCC_OVRDCTL_STRUCT CH5CCC_CR_CCC_OVRDCTL_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT CH5CCC_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT CH5CCC_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT CH5CCC_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT CH5CCC_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT CH5CCC_CR_TERDCCCONTROL0_STRUCT;

typedef CH0CCC_CR_CS_GEARDOWN_STRUCT CH5CCC_CR_CS_GEARDOWN_STRUCT;

typedef CH0CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT CH5CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT;

typedef CH0CCC_CR_COMPUPDATEOVERRIDE_STRUCT CH5CCC_CR_COMPUPDATEOVERRIDE_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT CH5CCC_CR_SPARE_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH6CCC_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH6CCC_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH6CCC_CR_DDRCRCACOMP_STRUCT;

typedef CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT CH6CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE0_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE1_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE2_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE3_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE4_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE5_STRUCT CH6CCC_CR_DDRCRCCCLANE5_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE6_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE7_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE8_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH6CCC_CR_DDRCRCCCLANE9_STRUCT;

typedef CH0CCC_CR_DDRCRCCCPICODES_STRUCT CH6CCC_CR_DDRCRCCCPICODES_STRUCT;

typedef CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT CH6CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT;

typedef CH0CCC_CR_DCCFSMCONTROL_STRUCT CH6CCC_CR_DCCFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCCALCCONTROL_STRUCT CH6CCC_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT CH6CCC_CR_REFPICONTROL_STRUCT;

typedef CH0CCC_CR_DDRCRBSCANDATA_STRUCT CH6CCC_CR_DDRCRBSCANDATA_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT CH6CCC_CR_REFPIFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCFSMSTATUS_STRUCT CH6CCC_CR_DCCFSMSTATUS_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS0_STRUCT CH6CCC_CR_DCCLANESTATUS0_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS1_STRUCT CH6CCC_CR_DCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT CH6CCC_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH6CCC_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH6CCC_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT CH6CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT CH6CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT CH6CCC_CR_DDRCRADC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT CH6CCC_CR_DFXCCMON_STRUCT;

typedef CH0CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT CH6CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT;

typedef CH0CCC_CR_CBB_BONUS0_STRUCT CH6CCC_CR_CBB_BONUS0_STRUCT;

typedef CH0CCC_CR_BONUS0_STRUCT CH6CCC_CR_BONUS0_STRUCT;

typedef CH0CCC_CR_REFPICLKOFFSET_STRUCT CH6CCC_CR_REFPICLKOFFSET_STRUCT;

typedef CH0CCC_CR_LPMODE4_FLYBY_STRUCT CH6CCC_CR_LPMODE4_FLYBY_STRUCT;

typedef CH0CCC_CR_CCC_OVRDCTL_STRUCT CH6CCC_CR_CCC_OVRDCTL_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT CH6CCC_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT CH6CCC_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT CH6CCC_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT CH6CCC_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT CH6CCC_CR_TERDCCCONTROL0_STRUCT;

typedef CH0CCC_CR_CS_GEARDOWN_STRUCT CH6CCC_CR_CS_GEARDOWN_STRUCT;

typedef CH0CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT CH6CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT;

typedef CH0CCC_CR_COMPUPDATEOVERRIDE_STRUCT CH6CCC_CR_COMPUPDATEOVERRIDE_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT CH6CCC_CR_SPARE_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH7CCC_CR_DDRCRCLKCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH7CCC_CR_DDRCRCTLCOMP_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DDRCRCACOMP_STRUCT CH7CCC_CR_DDRCRCACOMP_STRUCT;

typedef CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT CH7CCC_CR_DDRCRCLKCTLCACOMPOFFSET_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE0_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE1_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE2_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE3_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE4_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE5_STRUCT CH7CCC_CR_DDRCRCCCLANE5_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE6_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE7_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE8_STRUCT;

typedef CH0CCC_CR_DDRCRCCCLANE0_STRUCT CH7CCC_CR_DDRCRCCCLANE9_STRUCT;

typedef CH0CCC_CR_DDRCRCCCPICODES_STRUCT CH7CCC_CR_DDRCRCCCPICODES_STRUCT;

typedef CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT CH7CCC_CR_DDRCRPERBITDESKEWRISEFALL_STRUCT;

typedef CH0CCC_CR_DCCFSMCONTROL_STRUCT CH7CCC_CR_DCCFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCCALCCONTROL_STRUCT CH7CCC_CR_DCCCALCCONTROL_STRUCT;

typedef DATA0CH0_CR_REFPICONTROL_STRUCT CH7CCC_CR_REFPICONTROL_STRUCT;

typedef CH0CCC_CR_DDRCRBSCANDATA_STRUCT CH7CCC_CR_DDRCRBSCANDATA_STRUCT;

typedef DATA0CH0_CR_REFPIFSMCONTROL_STRUCT CH7CCC_CR_REFPIFSMCONTROL_STRUCT;

typedef CH0CCC_CR_DCCFSMSTATUS_STRUCT CH7CCC_CR_DCCFSMSTATUS_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS0_STRUCT CH7CCC_CR_DCCLANESTATUS0_STRUCT;

typedef CH0CCC_CR_DCCLANESTATUS1_STRUCT CH7CCC_CR_DCCLANESTATUS1_STRUCT;

typedef DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_STRUCT CH7CCC_CR_DDRCRMARGINMODECONTROL_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH7CCC_CR_DDRCRMARGINMODEDEBUGMSB_STRUCT;

typedef DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_STRUCT CH7CCC_CR_DDRCRMARGINMODEDEBUGLSB_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT CH7CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_STRUCT;

typedef DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT CH7CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_STRUCT;

typedef DATA0CH0_CR_DDRCRADC_STRUCT CH7CCC_CR_DDRCRADC_STRUCT;

typedef DDRPHY_DDRCOMP_CR_DFXCCMON_STRUCT CH7CCC_CR_DFXCCMON_STRUCT;

typedef CH0CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT CH7CCC_CR_DDRCRMARGINMODECONTROL1_STRUCT;

typedef CH0CCC_CR_CBB_BONUS0_STRUCT CH7CCC_CR_CBB_BONUS0_STRUCT;

typedef CH0CCC_CR_BONUS0_STRUCT CH7CCC_CR_BONUS0_STRUCT;

typedef CH0CCC_CR_REFPICLKOFFSET_STRUCT CH7CCC_CR_REFPICLKOFFSET_STRUCT;

typedef CH0CCC_CR_LPMODE4_FLYBY_STRUCT CH7CCC_CR_LPMODE4_FLYBY_STRUCT;

typedef CH0CCC_CR_CCC_OVRDCTL_STRUCT CH7CCC_CR_CCC_OVRDCTL_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_STRUCT CH7CCC_DDRTXDLL_CR_PICODELUT0_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_STRUCT CH7CCC_DDRTXDLL_CR_PICODELUT1_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_STRUCT CH7CCC_DDRTXDLL_CR_PICODELUT2_STRUCT;

typedef DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_STRUCT CH7CCC_DDRTXDLL_CR_PICODELUT3_STRUCT;

typedef DATA0CH0_CR_TERDCCCONTROL0_STRUCT CH7CCC_CR_TERDCCCONTROL0_STRUCT;

typedef CH0CCC_CR_CS_GEARDOWN_STRUCT CH7CCC_CR_CS_GEARDOWN_STRUCT;

typedef CH0CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT CH7CCC_CR_DDRCRCCCDCCCLKLANE5_STRUCT;

typedef CH0CCC_CR_COMPUPDATEOVERRIDE_STRUCT CH7CCC_CR_COMPUPDATEOVERRIDE_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT CH7CCC_CR_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 ECCPresent                              :  1;  // Bits 0:0
    UINT32 dficmddecode_en                         :  1;  // Bits 1:1
    UINT32 gvblock_blockretrain_dis                :  1;  // Bits 2:2
    UINT32 DdrNoChInterleave                       :  1;  // Bits 3:3
    UINT32 GearMode                                :  1;  // Bits 4:4
    UINT32 ForceLPMode                             :  2;  // Bits 6:5
    UINT32 ClkGateDisable                          :  1;  // Bits 7:7
    UINT32 DataClkGateDisAtIdle                    :  1;  // Bits 8:8
    UINT32 lp5wck2ckratio                          :  1;  // Bits 9:9
    UINT32 LpDdr4Mode                              :  1;  // Bits 10:10
    UINT32 Channel_Not_Populated                   :  8;  // Bits 18:11
    UINT32 dis_iosf_sb_clk_gate                    :  1;  // Bits 19:19
    UINT32 ScramEn                                 :  1;  // Bits 20:20
    UINT32 Write0En                                :  1;  // Bits 21:21
    UINT32 BGMode                                  :  2;  // Bits 23:22
    UINT32 RdRetrainOvrd                           :  1;  // Bits 24:24
    UINT32 WrRetrainOvrd                           :  1;  // Bits 25:25
    UINT32 DDR5Mode                                :  1;  // Bits 26:26
    UINT32 DisableCCCPh1                           :  1;  // Bits 27:27
    UINT32 DDR52NMode                              :  1;  // Bits 28:28
    UINT32 DDR52dpc                                :  1;  // Bits 29:29
    UINT32 Lp5Mode                                 :  1;  // Bits 30:30
    UINT32 cmddelay                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRSCRAM_CR_DDRMISCCONTROL0_STRUCT;
typedef union {
  struct {
    UINT32 io_train_rst                            :  1;  // Bits 0:0
    UINT32 io_train_rst_quiet_time                 :  1;  // Bits 1:1
    UINT32 io_train_rst_duration                   :  4;  // Bits 5:2
    UINT32 dccclkgatedisable                       :  1;  // Bits 6:6
    UINT32 ca_mirrored                             :  8;  // Bits 14:7
    UINT32 txfifordenflopbyp                       :  1;  // Bits 15:15
    UINT32 Spare                                   :  13;  // Bits 28:16
    UINT32 cccchannelinterleave                    :  1;  // Bits 29:29
    UINT32 Ddr5Pll                                 :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRSCRAM_CR_DDRMISCCONTROL1_STRUCT;
typedef union {
  struct {
    UINT32 TxBurstLen                              :  5;  // Bits 4:0
    UINT32 RxBurstLen                              :  5;  // Bits 9:5
    UINT32 WckDrvEnExtension                       :  2;  // Bits 11:10
    UINT32 lpdeltadqstrainmode                     :  1;  // Bits 12:12
    UINT32 DataInvertNibble                        :  2;  // Bits 14:13
    UINT32 Cas2FSWCk                               :  7;  // Bits 21:15
    UINT32 TrainWCkSyncRatio                       :  2;  // Bits 23:22
    UINT32 TrainWCkEn                              :  1;  // Bits 24:24
    UINT32 XSyncDelay                              :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRSCRAM_CR_DDRMISCCONTROL2_STRUCT;
typedef union {
  struct {
    UINT32 rsvd                                    :  3;  // Bits 2:0
    UINT32 tcwl4txdqfifowren                       :  7;  // Bits 9:3
    UINT32 tcwl4txdqfiforden                       :  7;  // Bits 16:10
    UINT32 tx_analog_clk_gate_dis                  :  1;  // Bits 17:17
    UINT32 Spare                                   :  3;  // Bits 20:18
    UINT32 RptChDqTxClkOn                          :  1;  // Bits 21:21
    UINT32 RptChDqRxClkOn                          :  1;  // Bits 22:22
    UINT32 RptChRepClkOn                           :  1;  // Bits 23:23
    UINT32 TxAnlgEnGraceCnt                        :  7;  // Bits 30:24
    UINT32 Spare1                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_WRITECFGCH0_STRUCT;

typedef MCMISCS_WRITECFGCH0_STRUCT MCMISCS_WRITECFGCH1_STRUCT;

typedef MCMISCS_WRITECFGCH0_STRUCT MCMISCS_WRITECFGCH2_STRUCT;

typedef MCMISCS_WRITECFGCH0_STRUCT MCMISCS_WRITECFGCH3_STRUCT;

typedef MCMISCS_WRITECFGCH0_STRUCT MCMISCS_WRITECFGCH4_STRUCT;

typedef MCMISCS_WRITECFGCH0_STRUCT MCMISCS_WRITECFGCH5_STRUCT;

typedef MCMISCS_WRITECFGCH0_STRUCT MCMISCS_WRITECFGCH6_STRUCT;

typedef MCMISCS_WRITECFGCH0_STRUCT MCMISCS_WRITECFGCH7_STRUCT;
typedef union {
  struct {
    UINT32 rx_analogen_grace_cnt                   :  7;  // Bits 6:0
    UINT32 tCL4RcvEn                               :  7;  // Bits 13:7
    UINT32 tCL4RxDqFifoRdEn                        :  7;  // Bits 20:14
    UINT32 rx_analog_clk_gate_dis                  :  1;  // Bits 21:21
    UINT32 Spare                                   :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_READCFGCH0_STRUCT;

typedef MCMISCS_READCFGCH0_STRUCT MCMISCS_READCFGCH1_STRUCT;

typedef MCMISCS_READCFGCH0_STRUCT MCMISCS_READCFGCH2_STRUCT;

typedef MCMISCS_READCFGCH0_STRUCT MCMISCS_READCFGCH3_STRUCT;

typedef MCMISCS_READCFGCH0_STRUCT MCMISCS_READCFGCH4_STRUCT;

typedef MCMISCS_READCFGCH0_STRUCT MCMISCS_READCFGCH5_STRUCT;

typedef MCMISCS_READCFGCH0_STRUCT MCMISCS_READCFGCH6_STRUCT;

typedef MCMISCS_READCFGCH0_STRUCT MCMISCS_READCFGCH7_STRUCT;
typedef union {
  struct {
    UINT32 RxDqFifoRdEnRank0ChADel                 :  4;  // Bits 3:0
    UINT32 RxDqFifoRdEnRank1ChADel                 :  4;  // Bits 7:4
    UINT32 RxDqFifoRdEnRank2ChADel                 :  4;  // Bits 11:8
    UINT32 RxDqFifoRdEnRank3ChADel                 :  4;  // Bits 15:12
    UINT32 RxDqFifoRdEnRank0ChBDel                 :  4;  // Bits 19:16
    UINT32 RxDqFifoRdEnRank1ChBDel                 :  4;  // Bits 23:20
    UINT32 RxDqFifoRdEnRank2ChBDel                 :  4;  // Bits 27:24
    UINT32 RxDqFifoRdEnRank3ChBDel                 :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_RXDQFIFORDENCH01_STRUCT;

typedef MCMISCS_RXDQFIFORDENCH01_STRUCT MCMISCS_RXDQFIFORDENCH23_STRUCT;

typedef MCMISCS_RXDQFIFORDENCH01_STRUCT MCMISCS_RXDQFIFORDENCH45_STRUCT;

typedef MCMISCS_RXDQFIFORDENCH01_STRUCT MCMISCS_RXDQFIFORDENCH67_STRUCT;
typedef union {
  struct {
    UINT32 PhaseDrvGBIB                            :  2;  // Bits 1:0
    UINT32 ParkToggle                              :  1;  // Bits 2:2
    UINT32 EnQclkGating                            :  1;  // Bits 3:3
    UINT32 QClkCountMod                            :  1;  // Bits 4:4
    UINT32 QClkCountRef                            :  2;  // Bits 6:5
    UINT32 Phclk_GraceLimitEntry                   :  5;  // Bits 11:7
    UINT32 GraceLimitEntry                         :  5;  // Bits 16:12
    UINT32 EnRstRefPiEveryN                        :  4;  // Bits 20:17
    UINT32 spinegate_en_lpmode05                   :  1;  // Bits 21:21
    UINT32 spinegate_en_lpmode2                    :  1;  // Bits 22:22
    UINT32 spinegate_en_lpmode3                    :  1;  // Bits 23:23
    UINT32 spinegate_en_lpmode4                    :  1;  // Bits 24:24
    UINT32 phclkgate_en_lpmode05                   :  1;  // Bits 25:25
    UINT32 phclkgate_en_lpmode1                    :  1;  // Bits 26:26
    UINT32 phclkgate_en_lpmode2                    :  1;  // Bits 27:27
    UINT32 phclkgate_en_lpmode3                    :  1;  // Bits 28:28
    UINT32 phclkgate_en_lpmode4                    :  1;  // Bits 29:29
    UINT32 entlclkgating                           :  1;  // Bits 30:30
    UINT32 enphclkgatingWithKeepGatingOff          :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPLL_CR_TXCLOCKGLOBAL_STRUCT;
typedef union {
  struct {
    UINT32 cas2rdwck                               :  7;  // Bits 6:0
    UINT32 cas2wrwck                               :  7;  // Bits 13:7
    UINT32 tWckHalfRate                            :  3;  // Bits 16:14
    UINT32 tWckPre                                 :  5;  // Bits 21:17
    UINT32 TrainWCkPulse                           :  1;  // Bits 22:22
    UINT32 Spare                                   :  1;  // Bits 23:23
    UINT32 WCkDiffLowInIdle                        :  1;  // Bits 24:24
    UINT32 TrainWCkBL                              :  4;  // Bits 28:25
    UINT32 TrainWCkMask                            :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_DDRWCKCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 forcecomp                               :  1;  // Bits 0:0
    UINT32 ForceDVFSComp                           :  1;  // Bits 1:1
    UINT32 ForceCompDist                           :  1;  // Bits 2:2
    UINT32 ForceCompUpdate                         :  1;  // Bits 3:3
    UINT32 BQTGrant2BQTReqFall                     :  6;  // Bits 9:4
    UINT32 disable_lpm4exitcccbufupdatemask        :  1;  // Bits 10:10
    UINT32 disable_lpm4exitmcbusquietreqmask       :  1;  // Bits 11:11
    UINT32 skip_comp_dist                          :  9;  // Bits 20:12
    UINT32 Spare1                                  :  5;  // Bits 25:21
    UINT32 disable_Lpm4updatetoBqt2SrxFsmHandshake :  1;  // Bits 26:26
    UINT32 disable_PeriodicupdatetoBqt2SrxFsmHandshake :  1;  // Bits 27:27
    UINT32 disable_lpm4mcbusquietreqmask           :  1;  // Bits 28:28
    UINT32 mcmiscs_comp_clken                      :  1;  // Bits 29:29
    UINT32 spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_COMPCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 EnInitComplete                          :  1;  // Bits 0:0
    UINT32 Spare                                   :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRSCRAM_CR_DDRLASTCR_STRUCT;
typedef union {
  struct {
    UINT32 ReadpJ                                  :  8;  // Bits 7:0
    UINT32 WritepJ                                 :  8;  // Bits 15:8
    UINT32 IdlemW                                  :  8;  // Bits 23:16
    UINT32 CKEmW                                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMCMISC_CR_DDRVDD2POWER_STRUCT;
typedef union {
  struct {
    UINT32 ScramKeyCh0                             :  16;  // Bits 15:0
    UINT32 ScramKeyCh1                             :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRSCRAM_CR_DDRSCRAMBLECH10_STRUCT;

typedef DDRSCRAM_CR_DDRSCRAMBLECH10_STRUCT DDRSCRAM_CR_DDRSCRAMBLECH32_STRUCT;

typedef DDRSCRAM_CR_DDRSCRAMBLECH10_STRUCT DDRSCRAM_CR_DDRSCRAMBLECH54_STRUCT;

typedef DDRSCRAM_CR_DDRSCRAMBLECH10_STRUCT DDRSCRAM_CR_DDRSCRAMBLECH76_STRUCT;
typedef union {
  struct {
    UINT32 lp_ctrl_en                              :  4;  // Bits 3:0
    UINT32 dfi_lp0p5_wakeup                        :  4;  // Bits 7:4
    UINT32 dfi_lp2_wakeup                          :  6;  // Bits 13:8
    UINT32 dfi_lp3_wakeup                          :  6;  // Bits 19:14
    UINT32 dfi_lp4_wakeup                          :  6;  // Bits 25:20
    UINT32 dfi_lp_resp                             :  5;  // Bits 30:26
    UINT32 lpmode_qual_with_initcomplete           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DDRPHY_DFI_LPCTRL0_STRUCT;
typedef union {
  struct {
    UINT32 dfi_lpmode0p5_exit_lat                  :  7;  // Bits 6:0
    UINT32 dfi_lpmode2_exit_lat                    :  10;  // Bits 16:7
    UINT32 dfi_lpmode3_exit_lat                    :  15;  // Bits 31:17
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DDRPHY_DFI_LPCTRL1_STRUCT;
typedef union {
  struct {
    UINT32 dfi_lpmode4_exit_lat                    :  11;  // Bits 10:0
    UINT32 lpctrldisclkgate                        :  1;  // Bits 11:11
    UINT32 lpmode0p5_spinegate                     :  5;  // Bits 16:12
    UINT32 lpmode2_spinegate                       :  5;  // Bits 21:17
    UINT32 lpmode3_spinegate                       :  5;  // Bits 26:22
    UINT32 lpmode4_spinegate                       :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DDRPHY_DFI_LPCTRL2_STRUCT;
typedef union {
  struct {
    UINT32 ddrcrobsvisasel0_16to1                  :  4;  // Bits 3:0
    UINT32 ddrcrobsvisasel1_16to1                  :  4;  // Bits 7:4
    UINT32 ddrcrobsvisasel0_24to8                  :  2;  // Bits 9:8
    UINT32 ddrcrobsvisasel1_24to8                  :  2;  // Bits 11:10
    UINT32 spare                                   :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRSCRAM_CR_VISA2VIEW_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT MCMISCS_CR_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 RSVD_7_0                                :  8;  // Bits 7:0
    UINT32 AndMatch                                :  1;  // Bits 8:8
    UINT32 InvMatch                                :  1;  // Bits 9:9
    UINT32 TrigEn                                  :  1;  // Bits 10:10
    UINT32 RSVD_15_11                              :  5;  // Bits 15:11
    UINT32 BLBMask                                 :  8;  // Bits 23:16
    UINT32 BLBMatch                                :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0_STRUCT DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_1_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0_STRUCT DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_2_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0_STRUCT DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_3_STRUCT;
typedef union {
  struct {
    UINT32 SourceEnable                            :  1;  // Bits 0:0
    UINT32 DestID                                  :  1;  // Bits 1:1
    UINT32 RSVD_7_2                                :  6;  // Bits 7:2
    UINT32 OperationMode                           :  1;  // Bits 8:8
    UINT32 StartCapture                            :  1;  // Bits 9:9
    UINT32 StopCapture                             :  1;  // Bits 10:10
    UINT32 ForceIdle                               :  1;  // Bits 11:11
    UINT32 EnActiveStart                           :  1;  // Bits 12:12
    UINT32 EnActiveStop                            :  1;  // Bits 13:13
    UINT32 RSVD_17_14                              :  4;  // Bits 17:14
    UINT32 OvrdResourceUsage                       :  1;  // Bits 18:18
    UINT32 ClkAckOvrd                              :  1;  // Bits 19:19
    UINT32 StopOnFull                              :  1;  // Bits 20:20
    UINT32 EnTraceDuringBlockreq                   :  1;  // Bits 21:21
    UINT32 ClkOffDetThresh                         :  6;  // Bits 27:22
    UINT32 DoNotCaptureAtClockOn                   :  1;  // Bits 28:28
    UINT32 RSVD_31_29                              :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_SRC_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 MasterID                                :  8;  // Bits 7:0
    UINT32 ChannelID                               :  8;  // Bits 15:8
    UINT32 BufferState                             :  4;  // Bits 19:16
    UINT32 RSVD_31_20                              :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_SRC_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 CRDT_CTRL                               :  1;  // Bits 0:0
    UINT32 EOF_CTRL                                :  1;  // Bits 1:1
    UINT32 PATGEN_CTRL                             :  1;  // Bits 2:2
    UINT32 PATGEN_MODE                             :  3;  // Bits 5:3
    UINT32 DEST_ID                                 :  1;  // Bits 6:6
    UINT32 ClkGateOvrdEn                           :  1;  // Bits 7:7
    UINT32 ResourceOwnOvrd                         :  1;  // Bits 8:8
    UINT32 RSVD_31_9                               :  23;  // Bits 31:9
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_ENCDR_DTF_SRC_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 RSVD_4_0                                :  5;  // Bits 4:0
    UINT32 NUMPKTSTREAMS                           :  4;  // Bits 8:5
    UINT32 CLKOFFDETLOGIC                          :  1;  // Bits 9:9
    UINT32 NUMLATSTAGES                            :  3;  // Bits 12:10
    UINT32 FASTCNTRWIDTH                           :  4;  // Bits 16:13
    UINT32 TRIGGERMASKMATCHEN                      :  1;  // Bits 17:17
    UINT32 RSVD_20_18                              :  3;  // Bits 20:18
    UINT32 DVP_EXT_VER                             :  3;  // Bits 23:21
    UINT32 STANDALONE_TOP                          :  1;  // Bits 24:24
    UINT32 RSVD_28_25                              :  4;  // Bits 28:25
    UINT32 DVP_VER                                 :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_DVP_CAP_STRUCT;
typedef union {
  struct {
    UINT32 FunReset                                :  1;  // Bits 0:0
    UINT32 ContDataPktGen                          :  1;  // Bits 1:1
    UINT32 ClkOffCntrDis                           :  1;  // Bits 2:2
    UINT32 VISASnapshot                            :  1;  // Bits 3:3
    UINT32 FrameCntrMatch                          :  4;  // Bits 7:4
    UINT32 TrigData                                :  8;  // Bits 15:8
    UINT32 PartialBufEmptyEn                       :  1;  // Bits 16:16
    UINT32 AllPktzrOverflow                        :  1;  // Bits 17:17
    UINT32 PatgenChidMode                          :  1;  // Bits 18:18
    UINT32 RSVD_31_19                              :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_DVP_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 SrcClkActive                            :  8;  // Bits 7:0
    UINT32 ClkOffOnStat                            :  8;  // Bits 15:8
    UINT32 PktWrittenStat                          :  1;  // Bits 16:16
    UINT32 PktDrainStat                            :  1;  // Bits 17:17
    UINT32 DVPTrig                                 :  1;  // Bits 18:18
    UINT32 DTFActStat                              :  1;  // Bits 19:19
    UINT32 ARTTimeStampStat                        :  1;  // Bits 20:20
    UINT32 XtalClkStat                             :  1;  // Bits 21:21
    UINT32 DTFCreditAvail                          :  1;  // Bits 22:22
    UINT32 UCIFSMStat                              :  2;  // Bits 24:23
    UINT32 PeriodicTSStat                          :  1;  // Bits 25:25
    UINT32 RSVD_28_26                              :  3;  // Bits 28:26
    UINT32 AllValidTSClkStat                       :  1;  // Bits 29:29
    UINT32 LTAVal                                  :  1;  // Bits 30:30
    UINT32 RSVD_31_31                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_DVP_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 RSVD_1_0                                :  2;  // Bits 1:0
    UINT32 DataWidth                               :  2;  // Bits 3:2
    UINT32 ForceVal                                :  1;  // Bits 4:4
    UINT32 CapMode                                 :  1;  // Bits 5:5
    UINT32 StorQualEn                              :  1;  // Bits 6:6
    UINT32 ForceCaptureEn                          :  1;  // Bits 7:7
    UINT32 TriggerMode0                            :  1;  // Bits 8:8
    UINT32 TriggerMode1                            :  1;  // Bits 9:9
    UINT32 TriggerMode2                            :  1;  // Bits 10:10
    UINT32 TriggerMode3                            :  1;  // Bits 11:11
    UINT32 TrigLatAdjust                           :  3;  // Bits 14:12
    UINT32 RSVD_15_15                              :  1;  // Bits 15:15
    UINT32 CapMask                                 :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_PKTSTRM_CTRL_0_STRUCT;
typedef union {
  struct {
    UINT32 InWidth                                 :  2;  // Bits 1:0
    UINT32 PendClkOffStat                          :  1;  // Bits 2:2
    UINT32 RSVD_7_3                                :  5;  // Bits 7:3
    UINT32 StorageFSMState                         :  2;  // Bits 9:8
    UINT32 RSVD_10_10                              :  1;  // Bits 10:10
    UINT32 PktWritten                              :  1;  // Bits 11:11
    UINT32 ValidTSClkStat                          :  1;  // Bits 12:12
    UINT32 ValidTimeoutStat                        :  1;  // Bits 13:13
    UINT32 QLT3Stat                                :  1;  // Bits 14:14
    UINT32 QFullStat                               :  1;  // Bits 15:15
    UINT32 QEmptyStat                              :  1;  // Bits 16:16
    UINT32 StoreEnStat                             :  1;  // Bits 17:17
    UINT32 DataValidStat                           :  1;  // Bits 18:18
    UINT32 DataRdyStat                             :  1;  // Bits 19:19
    UINT32 FrmRdyStat                              :  1;  // Bits 20:20
    UINT32 NewDataFlag                             :  1;  // Bits 21:21
    UINT32 EventRecogOutStat                       :  8;  // Bits 29:22
    UINT32 ActiveInjStatus                         :  1;  // Bits 30:30
    UINT32 RSVD_31_31                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_PKTSTRM_STATUS_0_STRUCT;
typedef union {
  struct {
    UINT32 VISABitEn                               :  8;  // Bits 7:0
    UINT32 AndMatch                                :  1;  // Bits 8:8
    UINT32 InvMatch                                :  1;  // Bits 9:9
    UINT32 RSVD_15_10                              :  6;  // Bits 15:10
    UINT32 BLBMask                                 :  8;  // Bits 23:16
    UINT32 BLBMatch                                :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_0_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_0_STRUCT DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_1_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_0_STRUCT DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_2_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_0_STRUCT DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_3_STRUCT;
typedef union {
  struct {
    UINT32 VISACaptureVal                          :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_VISA_CAPTURE_0_STRUCT;
typedef union {
  struct {
    UINT32 DebugStatus                             :  26;  // Bits 25:0
    UINT32 DFDC2_2_CTRL                            :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRMISCS_CR_DVP_PKTZR_DTF_DVP_DEBUG_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0_STRUCT DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_0_0_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0_STRUCT DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_0_1_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0_STRUCT DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_0_2_STRUCT;

typedef DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0_STRUCT DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_0_3_STRUCT;
typedef union {
  struct {
    UINT32 pulse_stretcher_counter_bit0            :  1;  // Bits 0:0
    UINT32 pulse_stretcher_counter_bit1            :  1;  // Bits 1:1
    UINT32 pulse_stretcher_counter_bit2            :  1;  // Bits 2:2
    UINT32 pulse_stretcher_counter_bit3            :  1;  // Bits 3:3
    UINT32 pulse_stretcher_counter_bit4            :  1;  // Bits 4:4
    UINT32 pulse_stretcher_counter_bit5            :  1;  // Bits 5:5
    UINT32 pulse_stretcher_counter_bit6            :  1;  // Bits 6:6
    UINT32 pulse_stretcher_counter_bit7            :  1;  // Bits 7:7
    UINT32 pulse_stretcher_counter_bit8            :  1;  // Bits 8:8
    UINT32 pulse_stretcher_counter_bit9            :  1;  // Bits 9:9
    UINT32 dvp_fast_counter_0                      :  1;  // Bits 10:10
    UINT32 dvp_fast_counter_1                      :  1;  // Bits 11:11
    UINT32 dvp_survive_mode                        :  1;  // Bits 12:12
    UINT32 spare                                   :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_DVP_MISC_STRUCT;
typedef union {
  struct {
    UINT32 DDr5ScramSeedMode                       :  1;  // Bits 0:0
    UINT32 Rsvd                                    :  7;  // Bits 7:1
    UINT32 dummy_spare                             :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_DUMMY_STRUCT;
typedef union {
  struct {
    UINT32 scratch0                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_UCSS_SCRATCH0_STRUCT;
typedef union {
  struct {
    UINT32 scratch1                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_UCSS_SCRATCH1_STRUCT;
typedef union {
  struct {
    UINT32 scratch2                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_UCSS_SCRATCH2_STRUCT;
typedef union {
  struct {
    UINT32 scratch3                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_UCSS_SCRATCH3_STRUCT;
typedef union {
  struct {
    UINT32 scratch4                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_UCSS_SCRATCH4_STRUCT;
typedef union {
  struct {
    UINT32 scratch5                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_UCSS_SCRATCH5_STRUCT;
typedef union {
  struct {
    UINT32 scratch6                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_UCSS_SCRATCH6_STRUCT;
typedef union {
  struct {
    UINT32 scratch7                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCS_CR_UCSS_SCRATCH7_STRUCT;
typedef union {
  struct {
    UINT32 EnCSGearDown                            :  1;  // Bits 0:0
    UINT32 ForceCSGearDown                         :  1;  // Bits 1:1
    UINT32 EnSRDRAMTrack                           :  1;  // Bits 2:2
    UINT32 reserved0                               :  4;  // Bits 6:3
    UINT32 Wait2NCount                             :  5;  // Bits 11:7
    UINT32 Wait1NCount                             :  5;  // Bits 16:12
    UINT32 RankEn                                  :  8;  // Bits 24:17
    UINT32 CSGearDownReset                         :  1;  // Bits 25:25
    UINT32 reserved                                :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRSCRAM_CR_CS_GEARDOWN_STRUCT;
typedef union {
  struct {
    UINT32 TLTermEn2TLClken                        :  5;  // Bits 4:0
    UINT32 TLClken2TLTermEn                        :  5;  // Bits 9:5
    UINT32 TLTermEnOnLp2                           :  1;  // Bits 10:10
    UINT32 TLTermEnOnLp3                           :  1;  // Bits 11:11
    UINT32 TLTermEnOnLp4                           :  1;  // Bits 12:12
    UINT32 TLDLLDrvenOvrd                          :  1;  // Bits 13:13
    UINT32 TLClkEnOvrd                             :  1;  // Bits 14:14
    UINT32 TLTermEnOvrd                            :  1;  // Bits 15:15
    UINT32 rsvd                                    :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRSCRAM_CR_TLCLKGATING_STRUCT;



#pragma pack(pop)
#endif /* __MrcRegisterStructPtl3xxx_h__ */







