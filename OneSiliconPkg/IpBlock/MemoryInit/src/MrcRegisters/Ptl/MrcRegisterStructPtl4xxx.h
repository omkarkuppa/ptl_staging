#ifndef __MrcRegisterStructPtl4xxx_h__
#define __MrcRegisterStructPtl4xxx_h__

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
    UINT32 FuseDistDoneOvrd                        :  1;  // Bits 0:0
    UINT32 saxgoffcheckovrd                        :  1;  // Bits 1:1
    UINT32 InitCompleteOvrd                        :  1;  // Bits 2:2
    UINT32 InitCompleteOvrdVal                     :  4;  // Bits 6:3
    UINT32 CompDistDoneOvrd                        :  1;  // Bits 7:7
    UINT32 VttAckOvrd                              :  1;  // Bits 8:8
    UINT32 VccClkAckOvrd                           :  1;  // Bits 9:9
    UINT32 VccDDQAckOvrd                           :  1;  // Bits 10:10
    UINT32 DD2GAckOvrd                             :  1;  // Bits 11:11
    UINT32 IOGAckOvrd                              :  1;  // Bits 12:12
    UINT32 SAGAckOvrd                              :  1;  // Bits 13:13
    UINT32 BlockAbpReq                             :  1;  // Bits 14:14
    UINT32 IOSFClkGateDis                          :  1;  // Bits 15:15
    UINT32 CRIClkGateDis                           :  1;  // Bits 16:16
    UINT32 TrainRstOnInit                          :  1;  // Bits 17:17
    UINT32 OtherCRWait                             :  1;  // Bits 18:18
    UINT32 VccClkFFCRWait                          :  1;  // Bits 19:19
    UINT32 VccLVRCRWait                            :  1;  // Bits 20:20
    UINT32 InitCompleteWait                        :  1;  // Bits 21:21
    UINT32 ForceLPMode                             :  2;  // Bits 23:22
    UINT32 blockcrrestore                          :  1;  // Bits 24:24
    UINT32 mrc_workaround                          :  1;  // Bits 25:25
    UINT32 ungateinitcompletebypm15                :  1;  // Bits 26:26
    UINT32 EnPeriodicComp                          :  4;  // Bits 30:27
    UINT32 DisCstateExitComp                       :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMOVRD_STRUCT;
typedef union {
  struct {
    UINT32 DD2PwrGood                              :  1;  // Bits 0:0
    UINT32 SAPwrGood                               :  1;  // Bits 1:1
    UINT32 Ispid_PwrGood_rstb                      :  1;  // Bits 2:2
    UINT32 FusePull                                :  1;  // Bits 3:3
    UINT32 PmaIPReady                              :  1;  // Bits 4:4
    UINT32 SAGpwrGood                              :  1;  // Bits 5:5
    UINT32 DD2GPwrGood                             :  1;  // Bits 6:6
    UINT32 IOGPwrGood                              :  1;  // Bits 7:7
    UINT32 CLKPwrGood                              :  1;  // Bits 8:8
    UINT32 DDQPwrGood                              :  1;  // Bits 9:9
    UINT32 VTTPwrGood                              :  1;  // Bits 10:10
    UINT32 QClkLocked                              :  1;  // Bits 11:11
    UINT32 AuxClkLocked                            :  1;  // Bits 12:12
    UINT32 SAFusePush                              :  1;  // Bits 13:13
    UINT32 DLLLock                                 :  1;  // Bits 14:14
    UINT32 CompDone                                :  1;  // Bits 15:15
    UINT32 CompDist                                :  1;  // Bits 16:16
    UINT32 CompUpdate                              :  1;  // Bits 17:17
    UINT32 VttOffset                               :  1;  // Bits 18:18
    UINT32 DCC                                     :  1;  // Bits 19:19
    UINT32 WeakLockCal                             :  1;  // Bits 20:20
    UINT32 ReadCDR                                 :  1;  // Bits 21:21
    UINT32 AllCompDone                             :  1;  // Bits 22:22
    UINT32 ReadReTraining                          :  1;  // Bits 23:23
    UINT32 WriteReTraining                         :  1;  // Bits 24:24
    UINT32 InitComplete                            :  4;  // Bits 28:25
    UINT32 PMAck                                   :  1;  // Bits 29:29
    UINT32 TXDLLLock                               :  1;  // Bits 30:30
    UINT32 CLKFbPwrGood                            :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_STRUCT;
typedef union {
  struct {
    UINT32 PMMessage                               :  4;  // Bits 3:0
    UINT32 WorkPointSel                            :  2;  // Bits 5:4
    UINT32 Spares                                  :  25;  // Bits 30:6
    UINT32 RunBusy                                 :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_STRUCT;
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
} DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_STRUCT;

typedef DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_STRUCT DDRPHY_MISC_SAUG_CR_DDR_PHY_VDD2_POWER_STRUCT;
typedef union {
  struct {
    UINT32 VccF0                                   :  9;  // Bits 8:0
    UINT32 VccF1                                   :  9;  // Bits 17:9
    UINT32 VccF2                                   :  9;  // Bits 26:18
    UINT32 EnOverClocking                          :  1;  // Bits 27:27
    UINT32 Spare                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_STRUCT;
typedef union {
  struct {
    UINT32 CurrentState                            :  4;  // Bits 3:0
    UINT32 CurrentWP                               :  2;  // Bits 5:4
    UINT32 FusaVoltageSense                        :  1;  // Bits 6:6
    UINT32 FusaPLLCheck                            :  1;  // Bits 7:7
    UINT32 FusaAuxCheck                            :  1;  // Bits 8:8
    UINT32 FusaDLLCheck                            :  1;  // Bits 9:9
    UINT32 FusaDCCCheck                            :  1;  // Bits 10:10
    UINT32 FusaRetrainCheck                        :  1;  // Bits 11:11
    UINT32 SaveRestoreDoneCheck                    :  1;  // Bits 12:12
    UINT32 PwrFsmOvrdVal                           :  4;  // Bits 16:13
    UINT32 PwrFsmOvrdEn                            :  1;  // Bits 17:17
    UINT32 PmAckOvrd                               :  1;  // Bits 18:18
    UINT32 CRI_Fuse_Force_Repull                   :  1;  // Bits 19:19
    UINT32 OvrdCompBlock                           :  1;  // Bits 20:20
    UINT32 ForceFuseDist                           :  1;  // Bits 21:21
    UINT32 FusaTXDLLCheck                          :  1;  // Bits 22:22
    UINT32 SAXGPwrGood                             :  1;  // Bits 23:23
    UINT32 VssHiPwrGood                            :  1;  // Bits 24:24
    UINT32 DistgPwrGood                            :  1;  // Bits 25:25
    UINT32 DD2PGEnableAck                          :  1;  // Bits 26:26
    UINT32 SAPGEnableAck                           :  1;  // Bits 27:27
    UINT32 IOGPGEnableAck                          :  1;  // Bits 28:28
    UINT32 DDQGPGEnableAck                         :  1;  // Bits 29:29
    UINT32 LVRPwrGoodOnSA                          :  1;  // Bits 30:30
    UINT32 PllLockExt                              :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_STRUCT;
typedef union {
  struct {
    UINT32 WorkPointUpdateOvrd                     :  1;  // Bits 0:0
    UINT32 WorkPointUpdateOvrdVal                  :  1;  // Bits 1:1
    UINT32 AuxWorkPointUpdateOvrd                  :  1;  // Bits 2:2
    UINT32 AuxWorkPointUpdateOvrdVal               :  1;  // Bits 3:3
    UINT32 NoWaitCompBlock                         :  1;  // Bits 4:4
    UINT32 DisDvfsExitComp                         :  1;  // Bits 5:5
    UINT32 Iosf_ClkGate_Dis                        :  1;  // Bits 6:6
    UINT32 CRI_ClkGate_Dis                         :  1;  // Bits 7:7
    UINT32 SkipDDRIPReady                          :  1;  // Bits 8:8
    UINT32 SkipSRIPReady                           :  1;  // Bits 9:9
    UINT32 CRI_FusePull_Bypass                     :  1;  // Bits 10:10
    UINT32 SelPllBypassClkOvrd                     :  1;  // Bits 11:11
    UINT32 FusePullDoneOvrd                        :  1;  // Bits 12:12
    UINT32 IP_Ready_Ovrd_Val                       :  1;  // Bits 13:13
    UINT32 IP_Ready_Ovrd_En                        :  1;  // Bits 14:14
    UINT32 DelayCompOnPMExit                       :  1;  // Bits 15:15
    UINT32 DisCompAbort                            :  1;  // Bits 16:16
    UINT32 dvp_blockreqen                          :  1;  // Bits 17:17
    UINT32 dvp_forcetsen                           :  1;  // Bits 18:18
    UINT32 dvp_ignoreack                           :  1;  // Bits 19:19
    UINT32 dvp_blockreqen_pkgc_only                :  1;  // Bits 20:20
    UINT32 dvp_forcetsen_dvfs_only                 :  1;  // Bits 21:21
    UINT32 DdrSwitch2DeepSleepOvrd                 :  1;  // Bits 22:22
    UINT32 DdrSwitch2DeepSleepOvrdVal              :  1;  // Bits 23:23
    UINT32 SelPllBypassClkOvrdVal                  :  1;  // Bits 24:24
    UINT32 DvfsRptCntDelay                         :  2;  // Bits 26:25
    UINT32 PllRptCntDelay                          :  2;  // Bits 28:27
    UINT32 EnDynRestoreCR                          :  1;  // Bits 29:29
    UINT32 upma_sbclk_gate_disable                 :  1;  // Bits 30:30
    UINT32 pmsb_sbclk_gate_disable                 :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_STRUCT;
typedef union {
  struct {
    UINT32 sramtrim_wpulse                         :  2;  // Bits 1:0
    UINT32 sramtrim_wa_disable                     :  1;  // Bits 2:2
    UINT32 sramtrim_ra                             :  2;  // Bits 4:3
    UINT32 sramtrim_wa                             :  3;  // Bits 7:5
    UINT32 sramtrim_mce                            :  1;  // Bits 8:8
    UINT32 sramtrim_stbyp                          :  1;  // Bits 9:9
    UINT32 sramtrim_rmce                           :  4;  // Bits 13:10
    UINT32 sramtrim_wmce                           :  2;  // Bits 15:14
    UINT32 sramtrim_sbc                            :  2;  // Bits 17:16
    UINT32 rftrim_mce                              :  1;  // Bits 18:18
    UINT32 rftrim_stbyp                            :  1;  // Bits 19:19
    UINT32 rftrim_rmce                             :  2;  // Bits 21:20
    UINT32 rftrim_wmce                             :  2;  // Bits 23:22
    UINT32 rftrim_sbc                              :  2;  // Bits 25:24
    UINT32 Spare                                   :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_STRUCT;
typedef union {
  struct {
    UINT32 Count                                   :  30;  // Bits 29:0
    UINT32 CountEn                                 :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_DDRPERFMON_STRUCT;
typedef union {
  struct {
    UINT32 Version                                 :  8;  // Bits 7:0
    UINT32 Derivative                              :  4;  // Bits 11:8
    UINT32 Segment                                 :  4;  // Bits 15:12
    UINT32 Subversion                              :  8;  // Bits 23:16
    UINT32 rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_IPVERSION_STRUCT;
typedef union {
  struct {
    UINT32 IP_Model                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_RTL_VERSION_STRUCT;
typedef union {
  struct {
    UINT32 SRDebugEn                               :  1;  // Bits 0:0
    UINT32 ForceStartSave                          :  1;  // Bits 1:1
    UINT32 ForceStartRestore                       :  1;  // Bits 2:2
    UINT32 ROMGRP0                                 :  4;  // Bits 6:3
    UINT32 ROMGRP1                                 :  4;  // Bits 10:7
    UINT32 ROMGRP2                                 :  4;  // Bits 14:11
    UINT32 ROMGRP3                                 :  4;  // Bits 18:15
    UINT32 DisableSRMiscClkGate                    :  1;  // Bits 19:19
    UINT32 Spare                                   :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_SR_OVRD_STRUCT;
typedef union {
  struct {
    UINT32 VccSAG_forceon_deassert_dly             :  2;  // Bits 1:0
    UINT32 VccSAG_pwrgood_deassert_dly             :  2;  // Bits 3:2
    UINT32 VccSAG_enableack_assert_dly             :  2;  // Bits 5:4
    UINT32 VccSAG_pgctrlfsm_clkgate_disable        :  1;  // Bits 6:6
    UINT32 VccDD2G_forceon_deassert_dly            :  2;  // Bits 8:7
    UINT32 VccDD2G_pwrgood_deassert_dly            :  2;  // Bits 10:9
    UINT32 VccDD2G_enableack_assert_dly            :  2;  // Bits 12:11
    UINT32 VccDD2G_pgctrlfsm_clkgate_disable       :  1;  // Bits 13:13
    UINT32 VccDDQG_forceon_deassert_dly            :  2;  // Bits 15:14
    UINT32 VccDDQG_pwrgood_deassert_dly            :  2;  // Bits 17:16
    UINT32 VccDDQG_enableack_assert_dly            :  2;  // Bits 19:18
    UINT32 VccDDQG_pgctrlfsm_clkgate_disable       :  1;  // Bits 20:20
    UINT32 VccIOG_forceon_deassert_dly             :  2;  // Bits 22:21
    UINT32 VccIOG_pwrgood_deassert_dly             :  2;  // Bits 24:23
    UINT32 VccIOG_enableack_assert_dly             :  2;  // Bits 26:25
    UINT32 VccIOG_pgctrlfsm_clkgate_disable        :  1;  // Bits 27:27
    UINT32 SAG_PwrGoodEarlyOvrdVal                 :  1;  // Bits 28:28
    UINT32 Spare                                   :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PG_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 VccSAG_prampictl                        :  4;  // Bits 3:0
    UINT32 VccDD2G_prampictl                       :  4;  // Bits 7:4
    UINT32 VccDDQG_prampictl                       :  4;  // Bits 11:8
    UINT32 DD2G_PG_DacCode                         :  5;  // Bits 16:12
    UINT32 reserved                                :  1;  // Bits 17:17
    UINT32 VccSAG_PG_Bonus                         :  2;  // Bits 19:18
    UINT32 VccDD2G_PG_Bonus                        :  2;  // Bits 21:20
    UINT32 VccDDQG_PG_Bonus                        :  2;  // Bits 23:22
    UINT32 VccSAXG_clkgateen_assert_dly            :  2;  // Bits 25:24
    UINT32 VccSAXG_reset_deassert_dly              :  2;  // Bits 27:26
    UINT32 VccSAXG_reset_assert_dly                :  2;  // Bits 29:28
    UINT32 VccSAXG_clkgateen_deassert_dly          :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PG_CTRL1_STRUCT;
typedef union {
  struct {
    UINT32 sbclk_gate_disable                      :  1;  // Bits 0:0
    UINT32 qclk_gate_disable                       :  1;  // Bits 1:1
    UINT32 DistG_RampDnDelay                       :  2;  // Bits 3:2
    UINT32 DistG_PwrGoodDeassertDelay              :  2;  // Bits 5:4
    UINT32 SagPwrGoodEarlyDly                      :  2;  // Bits 7:6
    UINT32 DistG_LVREnableDelay                    :  2;  // Bits 9:8
    UINT32 DistG_LVRRelockDelay                    :  2;  // Bits 11:10
    UINT32 LVRPwrGoodFSMCntDelay                   :  2;  // Bits 13:12
    UINT32 DistGResetDeassertDelay                 :  2;  // Bits 15:14
    UINT32 DistGResetAssertDelay                   :  2;  // Bits 17:16
    UINT32 DistGLVRResetOvrdVal                    :  1;  // Bits 18:18
    UINT32 phy_content_start                       :  1;  // Bits 19:19
    UINT32 phy_content_done                        :  1;  // Bits 20:20
    UINT32 rptch_sbclk_spine_clkgate_dis           :  1;  // Bits 21:21
    UINT32 DDQG_PG_DacCode                         :  5;  // Bits 26:22
    UINT32 SAG_PG_DacCode                          :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PG_CTRL2_STRUCT;
typedef union {
  struct {
    UINT32 visa_cmd_request                        :  1;  // Bits 0:0
    UINT32 visa_write_addr_offset                  :  5;  // Bits 5:1
    UINT32 visa_local_mode                         :  1;  // Bits 6:6
    UINT32 visa_write_addr_id                      :  9;  // Bits 15:7
    UINT32 visa_cmd                                :  2;  // Bits 17:16
    UINT32 visa_vrc_range                          :  2;  // Bits 19:18
    UINT32 sbr_local_vrc_mode                      :  1;  // Bits 20:20
    UINT32 sr_local_vrc_mode                       :  1;  // Bits 21:21
    UINT32 Spare                                   :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_VISA_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 ddrphy_visa_write_data                  :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_STRUCT;
typedef union {
  struct {
    UINT32 ddrphy_VISA_READ_DATA                   :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_STRUCT;
typedef union {
  struct {
    UINT32 DvfsPrepOverPChannel                    :  1;  // Bits 0:0
    UINT32 PChannelAlwaysAccept                    :  1;  // Bits 1:1
    UINT32 DfiInitComplete4BitResolution           :  1;  // Bits 2:2
    UINT32 DfiInitBlockInitComplete                :  1;  // Bits 3:3
    UINT32 DvfsOverDfiIntPropDelay                 :  2;  // Bits 5:4
    UINT32 LpMode4IntPropDelay                     :  2;  // Bits 7:6
    UINT32 LPM4DisPhCheck                          :  1;  // Bits 8:8
    UINT32 LPM4DisTrainReset                       :  1;  // Bits 9:9
    UINT32 LPM4DisPowerDownFsm                     :  1;  // Bits 10:10
    UINT32 PChPAcceptOvrd                          :  1;  // Bits 11:11
    UINT32 PChPAcceptOvrdVal                       :  1;  // Bits 12:12
    UINT32 PChPDenyOvrd                            :  1;  // Bits 13:13
    UINT32 PChPDenyOvrdVal                         :  1;  // Bits 14:14
    UINT32 LPM4FsmStateOvrd                        :  1;  // Bits 15:15
    UINT32 LPM4FsmStateOvrdVal                     :  4;  // Bits 19:16
    UINT32 LPM4DisLvrPwrDn                         :  1;  // Bits 20:20
    UINT32 LPM4DisSAGPwrDn                         :  1;  // Bits 21:21
    UINT32 LPM4DisDDQGPwrDn                        :  1;  // Bits 22:22
    UINT32 LPM4DisIOGPwrDn                         :  1;  // Bits 23:23
    UINT32 LPM4SAGThenLVRPwrUp                     :  1;  // Bits 24:24
    UINT32 LPM4DisCompUpdate                       :  1;  // Bits 25:25
    UINT32 PMSAGThenLVRPwrUp                       :  1;  // Bits 26:26
    UINT32 LPM4FsmClkGateDis                       :  1;  // Bits 27:27
    UINT32 LPM4ForcePMExit                         :  1;  // Bits 28:28
    UINT32 LPM4DisableIOSFClkReq                   :  1;  // Bits 29:29
    UINT32 LPM4DisVssHILvrPwrDn                    :  1;  // Bits 30:30
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 pmstc_crrst                             :  1;  // Bits 0:0
    UINT32 pmstc_rst                               :  1;  // Bits 1:1
    UINT32 pmstc_forceclken                        :  1;  // Bits 2:2
    UINT32 pmstc_clkgateen                         :  1;  // Bits 3:3
    UINT32 ucss_reset                              :  1;  // Bits 4:4
    UINT32 rsvd                                    :  11;  // Bits 15:5
    UINT32 xt_ne_sram_pg_en                        :  1;  // Bits 16:16
    UINT32 xt_fe_sram_pg_en                        :  1;  // Bits 17:17
    UINT32 xt_fe_side_clken                        :  1;  // Bits 18:18
    UINT32 mptu_crirst                             :  1;  // Bits 19:19
    UINT32 dunclk_div2ordiv4b                      :  1;  // Bits 20:20
    UINT32 dunclk_rst                              :  1;  // Bits 21:21
    UINT32 write_retrain_done                      :  1;  // Bits 22:22
    UINT32 read_retrain_done                       :  1;  // Bits 23:23
    UINT32 initcomplete_cmd                        :  1;  // Bits 24:24
    UINT32 initcomplete_mrr                        :  1;  // Bits 25:25
    UINT32 initcomplete_rdr                        :  1;  // Bits 26:26
    UINT32 initcomplete_wrr                        :  1;  // Bits 27:27
    UINT32 mptu_clken                              :  1;  // Bits 28:28
    UINT32 dfi_mux_sel                             :  1;  // Bits 29:29
    UINT32 mptu_en                                 :  1;  // Bits 30:30
    UINT32 mptu_rst                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 reset_abort                             :  1;  // Bits 0:0
    UINT32 DistGLvrCtrlFsm                         :  11;  // Bits 11:1
    UINT32 LPM4FsmState                            :  4;  // Bits 15:12
    UINT32 PgSAXGFsmState                          :  4;  // Bits 19:16
    UINT32 PgSAGFsmState                           :  3;  // Bits 22:20
    UINT32 PgDD2GFsmState                          :  3;  // Bits 25:23
    UINT32 PgIOGFsmState                           :  3;  // Bits 28:26
    UINT32 PgDDQGFsmState                          :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PMCTRL_STRUCT;
typedef union {
  struct {
    UINT32 DistG_LVRDisableDelay                   :  2;  // Bits 1:0
    UINT32 DistG_PwrGoodDeassertDelay              :  2;  // Bits 3:2
    UINT32 DistG_LVREnableDelay                    :  2;  // Bits 5:4
    UINT32 DistG_LVRRelockDelay                    :  2;  // Bits 7:6
    UINT32 SAG_PwrgoodEarly_override               :  1;  // Bits 8:8
    UINT32 PG_PwrgoodEarly_override                :  1;  // Bits 9:9
    UINT32 SAXG_Enable                             :  1;  // Bits 10:10
    UINT32 VccSAXG_pgctrlfsm_clkgate_disable       :  1;  // Bits 11:11
    UINT32 VccSAXG_enableack_assert_dly            :  2;  // Bits 13:12
    UINT32 VccSAXG_pwrgood_deassert_dly            :  2;  // Bits 15:14
    UINT32 VccSAXG_forceon_deassert_dly            :  2;  // Bits 17:16
    UINT32 VccSAXG_prampictl                       :  4;  // Bits 21:18
    UINT32 VccSAXG_PG_Bonus                        :  2;  // Bits 23:22
    UINT32 VccSAXG_BisrBypass                      :  1;  // Bits 24:24
    UINT32 SkipSAXGOffChkDuringRstAbrt             :  1;  // Bits 25:25
    UINT32 SAXG_Ready                              :  1;  // Bits 26:26
    UINT32 VccSAXG_DisDuringRstAbort               :  1;  // Bits 27:27
    UINT32 BISRHandshakeStartDelay                 :  2;  // Bits 29:28
    UINT32 bypass_srsram_bisr                      :  1;  // Bits 30:30
    UINT32 saxg_reset_abort_cbit                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PG_CTRL3_STRUCT;
typedef union {
  struct {
    UINT32 SAG_PGOvrdModeEn                        :  1;  // Bits 0:0
    UINT32 SAG_PGEnableOvrd                        :  1;  // Bits 1:1
    UINT32 SAG_PowerUpOvrdVal                      :  1;  // Bits 2:2
    UINT32 SAG_ForceOnOvrdVal                      :  1;  // Bits 3:3
    UINT32 SAG_PwrGoodOvrdVal                      :  1;  // Bits 4:4
    UINT32 SAXG_PGOvrdModeEn                       :  1;  // Bits 5:5
    UINT32 SAXG_PGEnableOvrd                       :  1;  // Bits 6:6
    UINT32 SAXG_PowerUpOvrdVal                     :  1;  // Bits 7:7
    UINT32 SAXG_ForceOnOvrdVal                     :  1;  // Bits 8:8
    UINT32 SAXG_PwrGoodOvrdVal                     :  1;  // Bits 9:9
    UINT32 SAXG_ResetOvrdVal                       :  1;  // Bits 10:10
    UINT32 SAXG_ClkEnOvrdVal                       :  1;  // Bits 11:11
    UINT32 DD2G_PGOvrdModeEn                       :  1;  // Bits 12:12
    UINT32 DD2G_PGEnableOvrd                       :  1;  // Bits 13:13
    UINT32 DD2G_PowerUpOvrdVal                     :  1;  // Bits 14:14
    UINT32 DD2G_ForceOnOvrdVal                     :  1;  // Bits 15:15
    UINT32 DD2G_PwrGoodOvrdVal                     :  1;  // Bits 16:16
    UINT32 IOG_PGOvrdModeEn                        :  1;  // Bits 17:17
    UINT32 IOG_PGEnableOvrd                        :  1;  // Bits 18:18
    UINT32 IOG_PowerUpOvrdVal                      :  1;  // Bits 19:19
    UINT32 IOG_ForceOnOvrdVal                      :  1;  // Bits 20:20
    UINT32 IOG_PwrGoodOvrdVal                      :  1;  // Bits 21:21
    UINT32 DDQG_PGOvrdModeEn                       :  1;  // Bits 22:22
    UINT32 DDQG_PGEnableOvrd                       :  1;  // Bits 23:23
    UINT32 DDQG_PowerUpOvrdVal                     :  1;  // Bits 24:24
    UINT32 DDQG_ForceOnOvrdVal                     :  1;  // Bits 25:25
    UINT32 DDQG_PwrGoodOvrdVal                     :  1;  // Bits 26:26
    UINT32 SAG_PGEnAckOvrdVal                      :  1;  // Bits 27:27
    UINT32 SAXG_PGEnAckOvrdVal                     :  1;  // Bits 28:28
    UINT32 DD2G_PGEnAckOvrdVal                     :  1;  // Bits 29:29
    UINT32 IOG_PGEnAckOvrdVal                      :  1;  // Bits 30:30
    UINT32 DDQG_PGEnAckOvrdVal                     :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_STRUCT;
typedef union {
  struct {
    UINT32 LVROvrdModeEn                           :  1;  // Bits 0:0
    UINT32 LVREnableOvrdVal                        :  1;  // Bits 1:1
    UINT32 LVRPwrGoodOvrdVal                       :  1;  // Bits 2:2
    UINT32 LVROffOvrdVal                           :  1;  // Bits 3:3
    UINT32 DistG_LVROvrdModeEn                     :  1;  // Bits 4:4
    UINT32 DistG_LVREnableOvrd                     :  1;  // Bits 5:5
    UINT32 DistG_PwrGoodOvrd                       :  1;  // Bits 6:6
    UINT32 DistG_LVROffOvrd                        :  1;  // Bits 7:7
    UINT32 VccCLKFBPwrGoodOvrd                     :  1;  // Bits 8:8
    UINT32 RetentionCtrlOvrd                       :  1;  // Bits 9:9
    UINT32 RetentionCtrlOvrdVal                    :  1;  // Bits 10:10
    UINT32 RetentionCrRestoreOvrd                  :  1;  // Bits 11:11
    UINT32 RetentionCrRestoreOvrdVal               :  1;  // Bits 12:12
    UINT32 ddrphy_clken_qclk_ovrd                  :  1;  // Bits 13:13
    UINT32 ddrphy_clken_qclk_ovrdval               :  1;  // Bits 14:14
    UINT32 ddrphy_reset_ovrd                       :  1;  // Bits 15:15
    UINT32 ddrphy_reset_ovrdval                    :  1;  // Bits 16:16
    UINT32 ddrphy_cr_reset_ovrd                    :  1;  // Bits 17:17
    UINT32 ddrphy_cr_reset_ovrdval                 :  1;  // Bits 18:18
    UINT32 ddrphy_pllen_ovrd                       :  1;  // Bits 19:19
    UINT32 ddrphy_pllen_ovrdval                    :  1;  // Bits 20:20
    UINT32 ddrphy_plllock_ovrd                     :  1;  // Bits 21:21
    UINT32 ddrphy_plllock_ovrdval                  :  1;  // Bits 22:22
    UINT32 PllOnOffOvrd                            :  1;  // Bits 23:23
    UINT32 PllOnOvrdVal                            :  1;  // Bits 24:24
    UINT32 PllOffOvrdVal                           :  1;  // Bits 25:25
    UINT32 PllUnLockRstAllCompDone                 :  1;  // Bits 26:26
    UINT32 ddrphy_sr_safe_mode_rst_b               :  1;  // Bits 27:27
    UINT32 RetentionSRSbClkDisOvrd                 :  1;  // Bits 28:28
    UINT32 RetentionSRSbClkDisOvrdVal              :  1;  // Bits 29:29
    UINT32 RetentionCriClkEnOvrd                   :  1;  // Bits 30:30
    UINT32 RetentionCriClkEnOvrdVal                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_STRUCT;
typedef union {
  struct {
    UINT32 crforcelpmode4                          :  1;  // Bits 0:0
    UINT32 plloffmode                              :  1;  // Bits 1:1
    UINT32 dislpmode4clkgate                       :  1;  // Bits 2:2
    UINT32 lpmode4exittimer                        :  10;  // Bits 12:3
    UINT32 scr_twaitsrx                            :  6;  // Bits 18:13
    UINT32 scr_twaitsrxdone                        :  6;  // Bits 24:19
    UINT32 scr_tlpmode4endone                      :  4;  // Bits 28:25
    UINT32 enable_lpmode4                          :  1;  // Bits 29:29
    UINT32 qualifypmlp4fsm                         :  1;  // Bits 30:30
    UINT32 qualifylpmode4en                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 PChPmStateOvrd                          :  1;  // Bits 0:0
    UINT32 PChPmStateOvrdVal                       :  3;  // Bits 3:1
    UINT32 PChIfStateOvrd                          :  1;  // Bits 4:4
    UINT32 PChIfStateOvrdVal                       :  3;  // Bits 7:5
    UINT32 DvfsOverDfiStateOvrd                    :  1;  // Bits 8:8
    UINT32 DvfsOverDfiStateOvrdVal                 :  3;  // Bits 11:9
    UINT32 PChPActiveOvrd                          :  1;  // Bits 12:12
    UINT32 PChPActiveOvrdVal                       :  8;  // Bits 20:13
    UINT32 ddrphy_cr_reset_saug_qclk_ovrd          :  1;  // Bits 21:21
    UINT32 ddrphy_cr_reset_saug_qclk_ovrdval       :  1;  // Bits 22:22
    UINT32 RetentionClkEnOvrd                      :  1;  // Bits 23:23
    UINT32 InitCompleteExtOvrd                     :  2;  // Bits 25:24
    UINT32 InitCompleteIntMcmiscsOvrd              :  2;  // Bits 27:26
    UINT32 InitCompleteIntRptchOvrd                :  2;  // Bits 29:28
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_STRUCT;
typedef union {
  struct {
    UINT32 PChannelEn                              :  1;  // Bits 0:0
    UINT32 SkipRestoreCR                           :  1;  // Bits 1:1
    UINT32 SkipRetentionCR                         :  1;  // Bits 2:2
    UINT32 SkipPMAck                               :  1;  // Bits 3:3
    UINT32 Spares                                  :  28;  // Bits 31:4
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_PMA_SAUG_CR_PMCFG_STRUCT;
typedef union {
  struct {
    UINT32 DistG_RampUpDelay                       :  2;  // Bits 1:0
    UINT32 DistG_LVRDisableDelay                   :  2;  // Bits 3:2
    UINT32 VccIOGPGRampRate                        :  2;  // Bits 5:4
    UINT32 VccIOGRampDn                            :  3;  // Bits 8:6
    UINT32 VccIOGPwrGoodDelay                      :  2;  // Bits 10:9
    UINT32 VccIOGRampUp                            :  2;  // Bits 12:11
    UINT32 VccDDQGPGRampRate                       :  2;  // Bits 14:13
    UINT32 VccDDQGRampDn                           :  3;  // Bits 17:15
    UINT32 VccDDQGPwrGoodDelay                     :  2;  // Bits 19:18
    UINT32 VccDDQGRampUp                           :  2;  // Bits 21:20
    UINT32 VccSAXGRampUp                           :  3;  // Bits 24:22
    UINT32 VccSAXGPwrGoodDelay                     :  2;  // Bits 26:25
    UINT32 VccSAXGRampDn                           :  3;  // Bits 29:27
    UINT32 PGRampRate_SAXG                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PG_CTRL4_STRUCT;
typedef union {
  struct {
    UINT32 DdrPChIfState                           :  3;  // Bits 2:0
    UINT32 DdrPChPmState                           :  3;  // Bits 5:3
    UINT32 DvfsOverDfiFsmState                     :  3;  // Bits 8:6
    UINT32 CompCtrlFsmState                        :  3;  // Bits 11:9
    UINT32 AuxPllFsmState                          :  3;  // Bits 14:12
    UINT32 InitCompleteFsmState                    :  3;  // Bits 17:15
    UINT32 PllCtrlFsmState                         :  5;  // Bits 22:18
    UINT32 DvfsFsmState                            :  4;  // Bits 26:23
    UINT32 LvrPwrGoodFsmState                      :  3;  // Bits 29:27
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_STRUCT;
typedef union {
  struct {
    UINT32 LVRRampDn                               :  3;  // Bits 2:0
    UINT32 Vdd2GRampUp                             :  2;  // Bits 4:3
    UINT32 Vdd2GPwrGoodDelay                       :  2;  // Bits 6:5
    UINT32 Vdd2GRampDn                             :  3;  // Bits 9:7
    UINT32 VccSAGRampUp                            :  3;  // Bits 12:10
    UINT32 VccSAGPwrGoodDelay                      :  2;  // Bits 14:13
    UINT32 VccSAGRampDn                            :  3;  // Bits 17:15
    UINT32 PGRampRate                              :  2;  // Bits 19:18
    UINT32 ReportFuSaErrors                        :  1;  // Bits 20:20
    UINT32 PGRampRate_SAG                          :  2;  // Bits 22:21
    UINT32 Spare                                   :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PG_STRUCT;
typedef union {
  struct {
    UINT32 InitCompleteSetOvrd                     :  1;  // Bits 0:0
    UINT32 PhCheckFsmDoneOvrd                      :  1;  // Bits 1:1
    UINT32 DccAckOvrd                              :  1;  // Bits 2:2
    UINT32 DllLockOvrd                             :  1;  // Bits 3:3
    UINT32 DllCompCompleteOvrd                     :  1;  // Bits 4:4
    UINT32 DqsDlyCompCompleteOvrd                  :  1;  // Bits 5:5
    UINT32 TrainResetDoneOvrd                      :  1;  // Bits 6:6
    UINT32 TxDllLockOvrd                           :  1;  // Bits 7:7
    UINT32 InitCompleteFsmStateOvrd                :  1;  // Bits 8:8
    UINT32 InitCompleteFsmStateOvrdVal             :  3;  // Bits 11:9
    UINT32 SkewCntDelay                            :  3;  // Bits 14:12
    UINT32 PropCntDelay                            :  3;  // Bits 17:15
    UINT32 CriClkEnPropCntDelay                    :  3;  // Bits 20:18
    UINT32 RetentionCtrlRptCntDelay                :  2;  // Bits 22:21
    UINT32 UseLPM4PllOpt                           :  1;  // Bits 23:23
    UINT32 UseLPM4PllOptForPkgc                    :  1;  // Bits 24:24
    UINT32 pgddr5mode                              :  1;  // Bits 25:25
    UINT32 IOGLVRWorkPointUpdateOvrdEn             :  1;  // Bits 26:26
    UINT32 IOGLVRWorkPointUpdateOvrdVal            :  1;  // Bits 27:27
    UINT32 DccAck_fsm_Ovrd                         :  1;  // Bits 28:28
    UINT32 PerfCountEn                             :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_STRUCT;
typedef union {
  struct {
    UINT32 SAXG_PG_DacCode                         :  5;  // Bits 4:0
    UINT32 PeriodicCompTimerCnt                    :  8;  // Bits 12:5
    UINT32 Spare                                   :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PG_CTRL5_STRUCT;
typedef union {
  struct {
    UINT32 IOG_LVRDisableDelay                     :  2;  // Bits 1:0
    UINT32 IOG_PwrGoodDeassertDelay                :  2;  // Bits 3:2
    UINT32 IOG_LVREnableDelay                      :  2;  // Bits 5:4
    UINT32 IOG_LVRRelockDelay                      :  2;  // Bits 7:6
    UINT32 IOG_RampUpDelay                         :  2;  // Bits 9:8
    UINT32 IOG_RampDnDelay                         :  2;  // Bits 11:10
    UINT32 IOG_ResetDeassertDelay                  :  2;  // Bits 13:12
    UINT32 IOG_ResetAssertDelay                    :  2;  // Bits 15:14
    UINT32 IOG_LVRResetOvrdVal                     :  1;  // Bits 16:16
    UINT32 IOG_LVROvrdModeEn                       :  1;  // Bits 17:17
    UINT32 IOG_LVREnableOvrd                       :  1;  // Bits 18:18
    UINT32 IOG_PwrGoodOvrd                         :  1;  // Bits 19:19
    UINT32 IOG_LVROffOvrd                          :  1;  // Bits 20:20
    UINT32 IOG_LVRVrefEnOvrd                       :  1;  // Bits 21:21
    UINT32 IOG_DisableGV_Switch                    :  1;  // Bits 22:22
    UINT32 IOG_LVRPwrGoodOnWhileRelockOvrd         :  1;  // Bits 23:23
    UINT32 IOG_LVRVrefEnOvrdEn                     :  1;  // Bits 24:24
    UINT32 IOG_LVRWPUpdateOvrd                     :  1;  // Bits 25:25
    UINT32 IOG_LVRWPUpdateOvrdEn                   :  1;  // Bits 26:26
    UINT32 IOG_LVRRelockDoneOvrd                   :  1;  // Bits 27:27
    UINT32 IOG_GV_LVROvrdModeEn                    :  1;  // Bits 28:28
    UINT32 Spare                                   :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_LVR_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 VccClk_LVRDisableDelay                  :  2;  // Bits 1:0
    UINT32 VccClk_PwrGoodDeassertDelay             :  2;  // Bits 3:2
    UINT32 VccClk_LVREnableDelay                   :  2;  // Bits 5:4
    UINT32 VccClk_LVRRelockDelay                   :  2;  // Bits 7:6
    UINT32 VccClk_RampUpDelay                      :  2;  // Bits 9:8
    UINT32 VccClk_RampDnDelay                      :  2;  // Bits 11:10
    UINT32 VccClk_ResetDeassertDelay               :  2;  // Bits 13:12
    UINT32 VccClk_ResetAssertDelay                 :  2;  // Bits 15:14
    UINT32 VccClk_LVRResetOvrdVal                  :  1;  // Bits 16:16
    UINT32 VccClk_LVROvrdModeEn                    :  1;  // Bits 17:17
    UINT32 VccClk_LVREnableOvrd                    :  1;  // Bits 18:18
    UINT32 VccClk_PwrGoodOvrd                      :  1;  // Bits 19:19
    UINT32 VccClk_LVROffOvrd                       :  1;  // Bits 20:20
    UINT32 VccClk_LVRVrefEnOvrd                    :  1;  // Bits 21:21
    UINT32 VccClk_DisableGV_Switch                 :  1;  // Bits 22:22
    UINT32 VccClk_LVRPwrGoodOnWhileRelockOvrd      :  1;  // Bits 23:23
    UINT32 VccClk_LVRVrefEnOvrdEn                  :  1;  // Bits 24:24
    UINT32 VccClk_LVRWPUpdateOvrd                  :  1;  // Bits 25:25
    UINT32 VccClk_LVRWPUpdateOvrdEn                :  1;  // Bits 26:26
    UINT32 VccClk_LVRRelockDoneOvrd                :  1;  // Bits 27:27
    UINT32 Spare                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_LVR_CTRL1_STRUCT;
typedef union {
  struct {
    UINT32 anaeleprobeen                           :  1;  // Bits 0:0
    UINT32 digeleprobeen                           :  1;  // Bits 1:1
    UINT32 i_probemuxsel                           :  3;  // Bits 4:2
    UINT32 anaeleprobeenshare                      :  1;  // Bits 5:5
    UINT32 digeleprobeenshare                      :  1;  // Bits 6:6
    UINT32 i_probemuxselShare                      :  3;  // Bits 9:7
    UINT32 Ddr_reg_clken_ovrd                      :  1;  // Bits 10:10
    UINT32 directviewmuxsel_port0                  :  7;  // Bits 17:11
    UINT32 directviewmuxsel_port1                  :  7;  // Bits 24:18
    UINT32 directviewmergemuxsel_port0             :  3;  // Bits 27:25
    UINT32 directviewmergemuxsel_port1             :  3;  // Bits 30:28
    UINT32 digviewdirectsel                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCSSAUG_SB_DEBUG_STRUCT;
typedef union {
  struct {
    UINT32 DisSbClkGatingDdrCCC0                   :  1;  // Bits 0:0
    UINT32 DisSbClkGatingDdrCCC1                   :  1;  // Bits 1:1
    UINT32 DisSbClkGatingDdrCCC2                   :  1;  // Bits 2:2
    UINT32 DisSbClkGatingDdrCCC3                   :  1;  // Bits 3:3
    UINT32 DisSbClkGatingDdrCOMP                   :  1;  // Bits 4:4
    UINT32 DisSbClkGatingDdrDATA0                  :  1;  // Bits 5:5
    UINT32 DisSbClkGatingDdrDATA1                  :  1;  // Bits 6:6
    UINT32 DisSbClkGatingDdrDATA2                  :  1;  // Bits 7:7
    UINT32 DisSbClkGatingDdrDATA3                  :  1;  // Bits 8:8
    UINT32 DisSbClkGatingDdrDATA4                  :  1;  // Bits 9:9
    UINT32 DisSbClkGatingDdrDATA5                  :  1;  // Bits 10:10
    UINT32 DisSbClkGatingDdrDATA6                  :  1;  // Bits 11:11
    UINT32 DisSbClkGatingDdrDATA7                  :  1;  // Bits 12:12
    UINT32 DisSbClkGatingDdrMCMISCS                :  1;  // Bits 13:13
    UINT32 DisSbClkGatingDdrPGCOMBO0               :  1;  // Bits 14:14
    UINT32 DisSbClkGatingDdrPGCOMBO1               :  1;  // Bits 15:15
    UINT32 DisSbClkGatingDdrPGCOMBO1B              :  1;  // Bits 16:16
    UINT32 DisSbClkGatingDdrPGCOMBO2               :  1;  // Bits 17:17
    UINT32 DisSbClkGatingDdrPGCOMBO3               :  1;  // Bits 18:18
    UINT32 DisSbClkGatingDdrPGCOMBO4               :  1;  // Bits 19:19
    UINT32 DisSbClkGatingDdrPGCOMBO5               :  1;  // Bits 20:20
    UINT32 DisSbClkGatingDdrPGCOMBO6               :  1;  // Bits 21:21
    UINT32 DisSbClkGatingDdrPGCOMBO7               :  1;  // Bits 22:22
    UINT32 DisSbClkGatingDdrPGCOMBO7B              :  1;  // Bits 23:23
    UINT32 DisSbClkGatingDdrPGCOMBO8               :  1;  // Bits 24:24
    UINT32 DisSbClkGatingDdrPGCOMBO9               :  1;  // Bits 25:25
    UINT32 DisSbClkGatingDdrPGTERM0                :  1;  // Bits 26:26
    UINT32 DisSbClkGatingDdrPGTERM1                :  1;  // Bits 27:27
    UINT32 DisSbClkGatingDdrRPTCH_P                :  1;  // Bits 28:28
    UINT32 DisSbClkGatingDdrRPTCH_S                :  1;  // Bits 29:29
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCSSAUG_SBCLK_GATING_STRUCT;
typedef union {
  struct {
    UINT32 LPM4RetCtrlOvrdEn                       :  1;  // Bits 0:0
    UINT32 LPM4RetRestoreOvrdEn                    :  1;  // Bits 1:1
    UINT32 LPM4RetSRSBClkDisOvrdEn                 :  1;  // Bits 2:2
    UINT32 LPM4RetCriClkEnOvrdEn                   :  1;  // Bits 3:3
    UINT32 LPM4CrResetOvrdEn                       :  1;  // Bits 4:4
    UINT32 LPM4ClkEnOvrdEn                         :  1;  // Bits 5:5
    UINT32 LPM4RetCtrlOvrdVal                      :  1;  // Bits 6:6
    UINT32 LPM4RetRestoreOvrdVal                   :  1;  // Bits 7:7
    UINT32 LPM4RetSRSBClkDisOvrdVal                :  1;  // Bits 8:8
    UINT32 LPM4RetCriClkEnOvrdVal                  :  1;  // Bits 9:9
    UINT32 LPM4CrResetOvrdVal                      :  1;  // Bits 10:10
    UINT32 LPM4ClkEnOvrdVal                        :  1;  // Bits 11:11
    UINT32 CBitBlockInitHandShakeDurCompUpdate     :  1;  // Bits 12:12
    UINT32 DistG_LVRVrefEnOvrdEn                   :  1;  // Bits 13:13
    UINT32 DistG_LVRWPUpdateOvrd                   :  1;  // Bits 14:14
    UINT32 DistG_LVRWPUpdateOvrdEn                 :  1;  // Bits 15:15
    UINT32 DistG_LVRRelockDoneOvrd                 :  1;  // Bits 16:16
    UINT32 DistG_LVRRelockDoneOvrdEn               :  1;  // Bits 17:17
    UINT32 IOG_LVRRelockDoneOvrdEn                 :  1;  // Bits 18:18
    UINT32 VccClk_LVRRelockDoneOvrdEn              :  1;  // Bits 19:19
    UINT32 DistG_LVRPwrGoodOnWhileRelockOvrd       :  1;  // Bits 20:20
    UINT32 DistG_LVRVrefEnOvrd                     :  1;  // Bits 21:21
    UINT32 PLLEnLow                                :  4;  // Bits 25:22
    UINT32 PllBypassenwaitForceonOvrd              :  1;  // Bits 26:26
    UINT32 CstateExitPllenwaitSagpwrgoodOvrd       :  1;  // Bits 27:27
    UINT32 Spare                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_STRUCT;
typedef union {
  struct {
    UINT32 GraceCountDdrCCC                        :  5;  // Bits 4:0
    UINT32 GraceCountDdrDATA                       :  5;  // Bits 9:5
    UINT32 GraceCountDdrCOMP                       :  5;  // Bits 14:10
    UINT32 GraceCountDdrMCMISCS                    :  5;  // Bits 19:15
    UINT32 GraceCountDdrRPTCH                      :  5;  // Bits 24:20
    UINT32 GraceCountDdrPGCOMBO                    :  5;  // Bits 29:25
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCMISCSSAUG_SBCLK_GATING1_STRUCT;
typedef union {
  struct {
    UINT32 VssHi_LVREnableDelay                    :  2;  // Bits 1:0
    UINT32 VssHi_RampUpDelay                       :  2;  // Bits 3:2
    UINT32 VssHi_PwrGoodAssertDelay                :  2;  // Bits 5:4
    UINT32 VssHi_ReadyDeassertDelay                :  2;  // Bits 7:6
    UINT32 VssHi_LVRDisableDelay                   :  2;  // Bits 9:8
    UINT32 VssHi_RampDnDelay                       :  2;  // Bits 11:10
    UINT32 VssHi_LVROvrdModeEn                     :  1;  // Bits 12:12
    UINT32 VssHi_LVREnableOvrd                     :  1;  // Bits 13:13
    UINT32 VssHi_PwrGoodOvrd                       :  1;  // Bits 14:14
    UINT32 VssHi_LVROffOvrd                        :  1;  // Bits 15:15
    UINT32 VssHi_LVRReadyOvrd                      :  1;  // Bits 16:16
    UINT32 Spare                                   :  15;  // Bits 31:17
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_LVR_CTRL2_STRUCT;
typedef union {
  struct {
    UINT32 EnMptuLpmodeExitCounter                 :  1;  // Bits 0:0
    UINT32 LpMode0p5or2ExitCount                   :  6;  // Bits 6:1
    UINT32 StepSizeLpMode0p5or2                    :  2;  // Bits 8:7
    UINT32 LpMode3or4ExitCount                     :  6;  // Bits 14:9
    UINT32 StepSizeLpMode3or4                      :  2;  // Bits 16:15
    UINT32 reserved                                :  15;  // Bits 31:17
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 WaitiInt                                :  1;  // Bits 0:0
    UINT32 EnGVRestoreBypass                       :  1;  // Bits 1:1
    UINT32 rsvd0                                   :  14;  // Bits 15:2
    UINT32 GVRestoreBypass                         :  1;  // Bits 16:16
    UINT32 rsvd1                                   :  15;  // Bits 31:17
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 VccClkLVRCtrlFsm                        :  11;  // Bits 10:0
    UINT32 IOGLVRCtrlFsm                           :  11;  // Bits 21:11
    UINT32 VssHiLVRCtrlFsm                         :  8;  // Bits 29:22
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_STRUCT;
typedef union {
  struct {
    UINT32 IOGGVLVRCtrlFsm                         :  11;  // Bits 10:0
    UINT32 Spare                                   :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_STRUCT;
typedef union {
  struct {
    UINT32 clklvr_selvdd2_ladder                   :  1;  // Bits 0:0
    UINT32 clklvr_vrefsel                          :  8;  // Bits 8:1
    UINT32 clklvr_gainctrl                         :  2;  // Bits 10:9
    UINT32 clklvr_rxdllb0vrefsel                   :  8;  // Bits 18:11
    UINT32 clklvr_rxdllb1vrefsel                   :  8;  // Bits 26:19
    UINT32 clklvr_bonus_in_GV                      :  2;  // Bits 28:27
    UINT32 reserved                                :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP3_STRUCT;
typedef union {
  struct {
    UINT32 RunBusy                                 :  1;  // Bits 0:0
    UINT32 SelLVR                                  :  2;  // Bits 2:1
    UINT32 Delay                                   :  2;  // Bits 4:3
    UINT32 StatusChop0                             :  5;  // Bits 9:5
    UINT32 StatusChop1                             :  5;  // Bits 14:10
    INT32  OffsetIOG                               :  5;  // Bits 19:15
    INT32  OffsetClk                               :  5;  // Bits 24:20
    UINT32 reserved                                :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT;
typedef union {
  struct {
    UINT32 bw0                                     :  4;  // Bits 3:0
    UINT32 bw1                                     :  4;  // Bits 7:4
    UINT32 bw2                                     :  4;  // Bits 11:8
    UINT32 bw3                                     :  4;  // Bits 15:12
    UINT32 bw4                                     :  4;  // Bits 19:16
    UINT32 bw5                                     :  4;  // Bits 23:20
    UINT32 spare                                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT;
typedef union {
  struct {
    UINT32 bw6                                     :  4;  // Bits 3:0
    UINT32 bw7                                     :  4;  // Bits 7:4
    UINT32 bw8                                     :  4;  // Bits 11:8
    UINT32 cb0                                     :  2;  // Bits 13:12
    UINT32 cb1                                     :  2;  // Bits 15:14
    UINT32 cb2                                     :  2;  // Bits 17:16
    UINT32 cb3                                     :  2;  // Bits 19:18
    UINT32 cb4                                     :  2;  // Bits 21:20
    UINT32 cb5                                     :  2;  // Bits 23:22
    UINT32 cb6                                     :  2;  // Bits 25:24
    UINT32 cb7                                     :  2;  // Bits 27:26
    UINT32 cb8                                     :  2;  // Bits 29:28
    UINT32 spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM0_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM0_CR_DDRCRTXDLLCBTUNE1_STRUCT;
typedef union {
  struct {
    UINT32 ioglvr_channel_ovrd_en                  :  2;  // Bits 1:0
    UINT32 ioglvr_channel_ovrd_val                 :  2;  // Bits 3:2
    UINT32 DDQGPG0BonusOut                         :  2;  // Bits 5:4
    UINT32 DDQGPG1BonusOut                         :  2;  // Bits 7:6
    UINT32 PwrGoodDistParBonusIn                   :  2;  // Bits 9:8
    UINT32 PwrGoodDistParBonusOut                  :  2;  // Bits 11:10
    UINT32 EnIOGDDQGLpMode4Mask                    :  1;  // Bits 12:12
    UINT32 VccDD2GPGBonusOut                       :  2;  // Bits 14:13
    UINT32 DDQGDisable                             :  1;  // Bits 15:15
    UINT32 CLK_LVRVrefEn_MaskforLp4                :  1;  // Bits 16:16
    UINT32 CLK_LVREn_MaskforLp4                    :  1;  // Bits 17:17
    UINT32 IOG_LVRVrefEn_MaskforLp4                :  1;  // Bits 18:18
    UINT32 IOG_LVREn_MaskforLp4                    :  1;  // Bits 19:19
    UINT32 clklvr_txdlldisable_ovrd                :  1;  // Bits 20:20
    UINT32 clklvr_rxdllb0disable_ovrd              :  1;  // Bits 21:21
    UINT32 clklvr_rxdllb1disable_ovrd              :  1;  // Bits 22:22
    UINT32 ioglvr_disable_ovrd                     :  1;  // Bits 23:23
    UINT32 reserved                                :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 ioglvr_selvdd2_ladder                   :  1;  // Bits 0:0
    UINT32 ioglvr_vrefsel                          :  8;  // Bits 8:1
    UINT32 ioglvr_gainctrl                         :  2;  // Bits 10:9
    UINT32 ioglvr_ffreadcode                       :  6;  // Bits 16:11
    UINT32 ioglvr_bonus_in                         :  2;  // Bits 18:17
    UINT32 reserved                                :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM0_SB_DEBUG_STRUCT;
typedef union {
  struct {
    UINT32 vsshilvr_vrefsel                        :  3;  // Bits 2:0
    UINT32 vsshilvrbypassen                        :  1;  // Bits 3:3
    UINT32 vsshi_LVREn_MaskforLp4                  :  1;  // Bits 4:4
    UINT32 vsshi_LVRReady_MaskforLp4               :  1;  // Bits 5:5
    UINT32 vsshilvr_bonus_in                       :  2;  // Bits 7:6
    UINT32 vsshilvr_bonus_out                      :  2;  // Bits 9:8
    UINT32 clklvr_bonus_in_nonGV                   :  4;  // Bits 13:10
    UINT32 clklvr_bonus_out                        :  6;  // Bits 19:14
    UINT32 ioglvr_bonus_out                        :  2;  // Bits 21:20
    UINT32 reserved                                :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM0_CR_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 viewmuxsel_port0                        :  8;  // Bits 7:0
    UINT32 viewmuxsel_port1                        :  8;  // Bits 15:8
    UINT32 visamuxsel                              :  8;  // Bits 23:16
    UINT32 Spare                                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRDATA_SBMEM1_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM1_CR_DDRCRDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM1_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM1_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM1_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRDATA_SBMEM1_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM1_SB_DEBUG_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRDATA_SBMEM1_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM1_CR_SPARE_STRUCT;

typedef DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT DDRDATA_SBMEM1_SB_DEBUG_1_0_0_MCHBAR_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM2_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM2_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM2_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM2_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRDATA_SBMEM2_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM2_CR_DDRCRDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM2_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM2_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM2_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRDATA_SBMEM2_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM2_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM2_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM2_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM2_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM2_SB_DEBUG_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRDATA_SBMEM2_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM2_CR_SPARE_STRUCT;

typedef DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT DDRDATA_SBMEM2_SB_DEBUG_1_0_0_MCHBAR_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM3_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM3_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM3_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM3_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRDATA_SBMEM3_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM3_CR_DDRCRDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM3_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM3_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM3_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRDATA_SBMEM3_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM3_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM3_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM3_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM3_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM3_SB_DEBUG_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRDATA_SBMEM3_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM3_CR_SPARE_STRUCT;

typedef DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT DDRDATA_SBMEM3_SB_DEBUG_1_0_0_MCHBAR_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM4_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM4_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM4_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM4_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRDATA_SBMEM4_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM4_CR_DDRCRDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM4_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM4_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM4_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRDATA_SBMEM4_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM4_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM4_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM4_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM4_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM4_SB_DEBUG_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRDATA_SBMEM4_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM4_CR_SPARE_STRUCT;

typedef DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT DDRDATA_SBMEM4_SB_DEBUG_1_0_0_MCHBAR_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM5_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM5_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM5_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM5_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRDATA_SBMEM5_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM5_CR_DDRCRDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM5_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM5_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM5_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRDATA_SBMEM5_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM5_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM5_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM5_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM5_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM5_SB_DEBUG_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRDATA_SBMEM5_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM5_CR_SPARE_STRUCT;

typedef DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT DDRDATA_SBMEM5_SB_DEBUG_1_0_0_MCHBAR_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM6_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM6_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM6_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM6_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRDATA_SBMEM6_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM6_CR_DDRCRDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM6_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM6_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM6_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRDATA_SBMEM6_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM6_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM6_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM6_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM6_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM6_SB_DEBUG_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRDATA_SBMEM6_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM6_CR_SPARE_STRUCT;

typedef DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT DDRDATA_SBMEM6_SB_DEBUG_1_0_0_MCHBAR_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM7_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM7_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM7_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM7_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRDATA_SBMEM7_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM7_CR_DDRCRDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM7_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM7_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM7_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRDATA_SBMEM7_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM7_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM7_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM7_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM7_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM7_SB_DEBUG_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRDATA_SBMEM7_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM7_CR_SPARE_STRUCT;

typedef DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT DDRDATA_SBMEM7_SB_DEBUG_1_0_0_MCHBAR_STRUCT;
typedef union {
  struct {
    UINT32 clklvr_selvdd2_ladder                   :  1;  // Bits 0:0
    UINT32 clklvr_vrefsel                          :  8;  // Bits 8:1
    UINT32 clklvr_gainctrl                         :  2;  // Bits 10:9
    UINT32 clklvr_bonus_in                         :  2;  // Bits 12:11
    UINT32 reserved                                :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRCCC_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRCCC_SBMEM0_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRCCC_SBMEM0_CR_DDRCRTXDLLCBTUNE1_STRUCT;
typedef union {
  struct {
    UINT32 tCKCSH                                  :  3;  // Bits 2:0
    UINT32 tCSH                                    :  3;  // Bits 5:3
    UINT32 tCSLortXP                               :  4;  // Bits 9:6
    UINT32 tSRX2CKSTOP                             :  6;  // Bits 15:10
    UINT32 TimerCount                              :  3;  // Bits 18:16
    UINT32 RANKEN                                  :  4;  // Bits 22:19
    UINT32 LPDDR5DCH0                              :  1;  // Bits 23:23
    UINT32 LPDDR5DCH1                              :  1;  // Bits 24:24
    UINT32 ENSRXFSM                                :  1;  // Bits 25:25
    UINT32 DISSRXFSMCLKGATE                        :  1;  // Bits 26:26
    UINT32 DISSHADOWBLOCK                          :  1;  // Bits 27:27
    UINT32 DISLPMODE4CLKGATE                       :  1;  // Bits 28:28
    UINT32 FORCECADRVEN                            :  1;  // Bits 29:29
    UINT32 FORCECLKDRVEN                           :  1;  // Bits 30:30
    UINT32 ENCADRVENSRX                            :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 clklvr_bonus_out                        :  2;  // Bits 1:0
    UINT32 ioglvr_bonus_out                        :  2;  // Bits 3:2
    UINT32 ioglvr_channel_ovrd_en                  :  2;  // Bits 5:4
    UINT32 ioglvr_channel_ovrd_val                 :  2;  // Bits 7:6
    UINT32 DDQGPG0BonusOut                         :  2;  // Bits 9:8
    UINT32 DDQGPG1BonusOut                         :  2;  // Bits 11:10
    UINT32 PwrGoodDistParBonusIn                   :  2;  // Bits 13:12
    UINT32 PwrGoodDistParBonusOut                  :  2;  // Bits 15:14
    UINT32 EnIOGDDQGLpMode4Mask                    :  1;  // Bits 16:16
    UINT32 VccDD2GPGBonusOut                       :  2;  // Bits 18:17
    UINT32 DDQGDisable                             :  1;  // Bits 19:19
    UINT32 CLK_LVRVrefEn_MaskforLp4                :  1;  // Bits 20:20
    UINT32 CLK_LVREn_MaskforLp4                    :  1;  // Bits 21:21
    UINT32 IOG_LVRVrefEn_MaskforLp4                :  1;  // Bits 22:22
    UINT32 IOG_LVREn_MaskforLp4                    :  1;  // Bits 23:23
    UINT32 ioglvr_disable_ovrd                     :  1;  // Bits 24:24
    UINT32 clklvr_disable_ovrd                     :  1;  // Bits 25:25
    UINT32 reserved                                :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SBMEM0_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRCCC_SBMEM0_SB_DEBUG_STRUCT;
typedef union {
  struct {
    UINT32 vsshilvr_vrefsel                        :  3;  // Bits 2:0
    UINT32 vsshilvrbypassen                        :  1;  // Bits 3:3
    UINT32 vsshi_LVREn_MaskforLp4                  :  1;  // Bits 4:4
    UINT32 vsshi_LVRReady_MaskforLp4               :  1;  // Bits 5:5
    UINT32 vsshilvr_bonus_in                       :  2;  // Bits 7:6
    UINT32 vsshilvr_bonus_out                      :  2;  // Bits 9:8
    UINT32 reserved                                :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRCCC_SBMEM0_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SBMEM0_CR_SPARE_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRCCC_SBMEM1_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRCCC_SBMEM1_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRCCC_SBMEM1_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_STRUCT DDRCCC_SBMEM1_CR_DDRCRLPMODE4CTRL0_STRUCT;

typedef DDRCCC_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRCCC_SBMEM1_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRCCC_SBMEM1_SB_DEBUG_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRCCC_SBMEM1_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SBMEM1_CR_SPARE_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM2_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM2_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM2_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM2_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRCCC_SBMEM2_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRCCC_SBMEM2_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRCCC_SBMEM2_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_STRUCT DDRCCC_SBMEM2_CR_DDRCRLPMODE4CTRL0_STRUCT;

typedef DDRCCC_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRCCC_SBMEM2_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM2_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM2_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM2_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM2_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRCCC_SBMEM2_SB_DEBUG_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRCCC_SBMEM2_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SBMEM2_CR_SPARE_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM3_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM3_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM3_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM3_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRCCC_SBMEM3_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRCCC_SBMEM3_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRCCC_SBMEM3_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_STRUCT DDRCCC_SBMEM3_CR_DDRCRLPMODE4CTRL0_STRUCT;

typedef DDRCCC_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRCCC_SBMEM3_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM3_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM3_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM3_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM3_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRCCC_SBMEM3_SB_DEBUG_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRCCC_SBMEM3_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SBMEM3_CR_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 tltermcomp                              :  5;  // Bits 4:0
    UINT32 tltermstaticen                          :  1;  // Bits 5:5
    UINT32 tltermovrden                            :  1;  // Bits 6:6
    UINT32 TLClken2TLTermEn                        :  6;  // Bits 12:7
    UINT32 TLTermEn2TLClken                        :  6;  // Bits 18:13
    UINT32 LVRPwrGoodQualTLTermEnonLP4             :  1;  // Bits 19:19
    UINT32 LVRPwrGoodQualTLTermEn                  :  1;  // Bits 20:20
    UINT32 reserved                                :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT;

typedef DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP1_STRUCT;

typedef DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP2_STRUCT;

typedef DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP3_STRUCT;
typedef union {
  struct {
    UINT32 PwrGoodPODBonusIn                       :  2;  // Bits 1:0
    UINT32 PwrGoodPODBonusOut                      :  2;  // Bits 3:2
    UINT32 MEMGPGBonusOut                          :  3;  // Bits 6:4
    UINT32 DD2GPGBonusOut                          :  2;  // Bits 8:7
    UINT32 PGTermBonusIn                           :  2;  // Bits 10:9
    UINT32 Ddr_reg_clken_ovrd_ddrpg                :  6;  // Bits 16:11
    UINT32 Spare                                   :  15;  // Bits 31:17
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRVCCCLK_SBMEM0_SB_DEBUG_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRVCCCLK_SBMEM0_CR_SPARE_STRUCT;

typedef DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP0_STRUCT;

typedef DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP1_STRUCT;

typedef DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP2_STRUCT;

typedef DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP3_STRUCT;

typedef DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_STRUCT DDRVCCCLK_SBMEM1_CR_PMMISC_CTRL_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRVCCCLK_SBMEM1_SB_DEBUG_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRVCCCLK_SBMEM1_CR_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 DD2GPGBonusOut_PG5                      :  2;  // Bits 1:0
    UINT32 reserved                                :  30;  // Bits 31:2
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 Target                                  :  9;  // Bits 8:0
    UINT32 RemoteSense                             :  1;  // Bits 9:9
    UINT32 OCDivider                               :  1;  // Bits 10:10
    UINT32 Bypass                                  :  1;  // Bits 11:11
    UINT32 Disable                                 :  1;  // Bits 12:12
    UINT32 BiasAdj                                 :  4;  // Bits 16:13
    UINT32 AdaptiveZeroAdj1                        :  4;  // Bits 20:17
    UINT32 AdaptiveZeroAdj2                        :  5;  // Bits 25:21
    UINT32 LockTimer                               :  2;  // Bits 27:26
    UINT32 PMBiasAdj                               :  1;  // Bits 28:28
    UINT32 LvrPM                                   :  2;  // Bits 30:29
    UINT32 PwrGoodFB                               :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP1_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP2_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP3_STRUCT;
typedef union {
  struct {
    UINT32 Target                                  :  9;  // Bits 8:0
    UINT32 RemoteSense                             :  1;  // Bits 9:9
    UINT32 OCDivider                               :  1;  // Bits 10:10
    UINT32 Bypass                                  :  1;  // Bits 11:11
    UINT32 Disable                                 :  1;  // Bits 12:12
    UINT32 BiasAdj                                 :  4;  // Bits 16:13
    UINT32 AdaptiveZeroAdj1                        :  4;  // Bits 20:17
    UINT32 AdaptiveZeroAdj2                        :  5;  // Bits 25:21
    UINT32 LockTimer                               :  2;  // Bits 27:26
    UINT32 PMBiasAdj                               :  1;  // Bits 28:28
    UINT32 LvrPM                                   :  2;  // Bits 30:29
    UINT32 PwrGoodFB                               :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP1_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP2_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP3_STRUCT;
typedef union {
  struct {
    UINT32 RunBusy                                 :  1;  // Bits 0:0
    UINT32 SelLVR                                  :  2;  // Bits 2:1
    UINT32 Delay                                   :  2;  // Bits 4:3
    UINT32 StatusChop0                             :  5;  // Bits 9:5
    UINT32 StatusChop1                             :  5;  // Bits 14:10
    INT32  OffsetIOG                               :  5;  // Bits 19:15
    INT32  OffsetIOGGV                             :  5;  // Bits 24:20
    INT32  OffsetDist                              :  5;  // Bits 29:25
    UINT32 Spare                                   :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_STRUCT;
typedef union {
  struct {
    UINT32 QClkRatio                               :  9;  // Bits 8:0
    UINT32 AuxClkRatio                             :  9;  // Bits 17:9
    UINT32 Gear4                                   :  1;  // Bits 18:18
    UINT32 DISTGLVRSelVdd2Ladder                   :  1;  // Bits 19:19
    UINT32 DISTGLVRVrefSel                         :  8;  // Bits 27:20
    UINT32 VddqCompVrefSel                         :  3;  // Bits 30:28
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT1_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT2_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT3_STRUCT;
typedef union {
  struct {
    UINT32 ColdLockTime                            :  10;  // Bits 9:0
    UINT32 LockSource                              :  2;  // Bits 11:10
    UINT32 PLLLockSel                              :  2;  // Bits 13:12
    UINT32 IntPLLLockSel                           :  2;  // Bits 15:14
    UINT32 StatusPLLLockSel                        :  2;  // Bits 17:16
    UINT32 LockCountF                              :  2;  // Bits 19:18
    UINT32 LockCountP_M                            :  2;  // Bits 21:20
    UINT32 LockWindow                              :  4;  // Bits 25:22
    UINT32 LockPhyST                               :  1;  // Bits 26:26
    UINT32 FusaFlagUnlock                          :  1;  // Bits 27:27
    UINT32 PLLEnLow                                :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_STRUCT;
typedef union {
  struct {
    UINT32 DeskewEn                                :  1;  // Bits 0:0
    UINT32 OffsetCalByp                            :  1;  // Bits 1:1
    UINT32 OffsetCalCnt                            :  3;  // Bits 4:2
    UINT32 OffsetCalCntEarly                       :  3;  // Bits 7:5
    UINT32 OffsetCalEn                             :  1;  // Bits 8:8
    UINT32 OffsetCalWr                             :  1;  // Bits 9:9
    UINT32 PLLReserved0                            :  14;  // Bits 23:10
    UINT32 PLLReserved1                            :  2;  // Bits 25:24
    UINT32 AltRefClk                               :  1;  // Bits 26:26
    UINT32 Spare                                   :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_STRUCT;

typedef DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEAUXCLK_STRUCT;
typedef union {
  struct {
    UINT32 PhClkPLLEn                              :  1;  // Bits 0:0
    UINT32 PhClkLock                               :  1;  // Bits 1:1
    UINT32 PhClkFilterLock                         :  1;  // Bits 2:2
    UINT32 PhClkLateLock                           :  1;  // Bits 3:3
    UINT32 PhClkStickyLock                         :  1;  // Bits 4:4
    UINT32 PhClkLockFallCount                      :  5;  // Bits 9:5
    UINT32 PhClkOffsetCalLock_spare                :  1;  // Bits 10:10
    UINT32 PhClkLateCalLock_spare                  :  1;  // Bits 11:11
    UINT32 PhClkStickyCalLock_spare                :  1;  // Bits 12:12
    UINT32 PhClkCalLockFallCount_spare             :  3;  // Bits 15:13
    UINT32 PhClkFinalLock                          :  1;  // Bits 16:16
    UINT32 AuxClkPLLEn                             :  1;  // Bits 17:17
    UINT32 AuxClkLock                              :  1;  // Bits 18:18
    UINT32 AuxClkLateLock                          :  1;  // Bits 19:19
    UINT32 AuxClkLockFallCount                     :  5;  // Bits 24:20
    UINT32 AuxClkFinalLock                         :  1;  // Bits 25:25
    UINT32 TLLock_spare                            :  1;  // Bits 26:26
    UINT32 TLLateLock_spare                        :  1;  // Bits 27:27
    UINT32 TLStickyLock_spare                      :  1;  // Bits 28:28
    UINT32 TLLockFallCount_spare                   :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 OvrdPhClock                             :  1;  // Bits 0:0
    UINT32 OvrdPhClkPLLEn                          :  1;  // Bits 1:1
    UINT32 PHClkDTestEn                            :  1;  // Bits 2:2
    UINT32 PhClkForceBypass                        :  1;  // Bits 3:3
    UINT32 PHClkOffsetFastCal                      :  1;  // Bits 4:4
    UINT32 OvrdAuxClock                            :  1;  // Bits 5:5
    UINT32 OvrdAuxPLLEn                            :  1;  // Bits 6:6
    UINT32 AuxDTestEn                              :  1;  // Bits 7:7
    UINT32 AuxForceBypass                          :  1;  // Bits 8:8
    UINT32 AuxOffsetFastCal                        :  1;  // Bits 9:9
    UINT32 PhClkPLLFeedback                        :  2;  // Bits 11:10
    UINT32 TLDelay                                 :  4;  // Bits 15:12
    UINT32 TLFsmStart                              :  2;  // Bits 17:16
    UINT32 TLFsmEn                                 :  2;  // Bits 19:18
    UINT32 TLFsmCount                              :  2;  // Bits 21:20
    UINT32 TLRstDelay                              :  1;  // Bits 22:22
    UINT32 Spare                                   :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_STRUCT;
typedef union {
  struct {
    UINT32 OffsetCal2_spare                        :  12;  // Bits 11:0
    UINT32 OffsetCal3_spare                        :  12;  // Bits 23:12
    UINT32 rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_STRUCT;
typedef union {
  struct {
    UINT32 OffsetCal0_spare                        :  12;  // Bits 11:0
    UINT32 OffsetCal1_spare                        :  12;  // Bits 23:12
    UINT32 rsvd                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_STRUCT;
typedef union {
  struct {
    UINT32 OffsetCal2_spare                        :  12;  // Bits 11:0
    UINT32 OffsetCal3_spare                        :  12;  // Bits 23:12
    INT32  QClkDCC3                                :  5;  // Bits 28:24
    UINT32 rsvd                                    :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_STRUCT;
typedef union {
  struct {
    UINT32 OffsetCal0_spare                        :  12;  // Bits 11:0
    UINT32 OffsetCal1_spare                        :  12;  // Bits 23:12
    INT32  QClkDCC2                                :  5;  // Bits 28:24
    UINT32 rsvd                                    :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_STRUCT;
typedef union {
  struct {
    UINT32 OffsetCal2_spare                        :  12;  // Bits 11:0
    UINT32 OffsetCal3_spare                        :  12;  // Bits 23:12
    INT32  QClkDCC1                                :  5;  // Bits 28:24
    UINT32 rsvd                                    :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_STRUCT;
typedef union {
  struct {
    UINT32 OffsetCal0_spare                        :  12;  // Bits 11:0
    UINT32 OffsetCal1_spare                        :  12;  // Bits 23:12
    INT32  QClkDCC0                                :  5;  // Bits 28:24
    UINT32 rsvd                                    :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_STRUCT;
typedef union {
  struct {
    UINT32 BGGndRefBypass                          :  1;  // Bits 0:0
    UINT32 BGBiasTrim                              :  3;  // Bits 3:1
    INT32  UGB0Offset                              :  5;  // Bits 8:4
    INT32  UGB1Offset                              :  5;  // Bits 13:9
    UINT32 UGBRampUpDelay                          :  4;  // Bits 17:14
    UINT32 VccIOSlopeOC                            :  4;  // Bits 21:18
    UINT32 VccCLKSlopeOC                           :  4;  // Bits 25:22
    UINT32 Spare                                   :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_STRUCT;
typedef union {
  struct {
    UINT32 PHClkLockTime                           :  11;  // Bits 10:0
    UINT32 AuxClkLockTime                          :  11;  // Bits 21:11
    UINT32 PHClkPhaseEarly                         :  1;  // Bits 22:22
    UINT32 PHClkPhaseLate                          :  1;  // Bits 23:23
    UINT32 AuxClkPhaseEarly                        :  1;  // Bits 24:24
    UINT32 AuxClkPhaseLate                         :  1;  // Bits 25:25
    UINT32 FusaPhPllResetStatus                    :  1;  // Bits 26:26
    UINT32 FusaAuxPllResetStatus                   :  1;  // Bits 27:27
    UINT32 AuxClkOffsetCalLock                     :  1;  // Bits 28:28
    UINT32 AuxClkLateCalLock                       :  1;  // Bits 29:29
    UINT32 AuxClkStickyCalLock                     :  1;  // Bits 30:30
    UINT32 Spare                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_STRUCT;
typedef union {
  struct {
    UINT32 EnDrvNClk                               :  5;  // Bits 4:0
    UINT32 EnDrvNClkb                              :  5;  // Bits 9:5
    UINT32 EnDrvSClk                               :  5;  // Bits 14:10
    UINT32 EnDrvSClkb                              :  5;  // Bits 19:15
    UINT32 LjpllUsync12In                          :  5;  // Bits 24:20
    UINT32 LjpllUsync1Out                          :  5;  // Bits 29:25
    UINT32 Ljpll_clk_bypass_sel                    :  1;  // Bits 30:30
    UINT32 Auxpll_clk_bypass_sel                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVSSHI_STRUCT;
typedef union {
  struct {
    UINT32 ioglvr_bonus_out                        :  2;  // Bits 1:0
    UINT32 ioglvr40_bonus_out                      :  2;  // Bits 3:2
    UINT32 ioglvr_channel_ovrd_en                  :  2;  // Bits 5:4
    UINT32 ioglvr_channel_ovrd_val                 :  2;  // Bits 7:6
    UINT32 PwrGoodDistParBonusIn                   :  2;  // Bits 9:8
    UINT32 PwrGoodDistParBonusOut                  :  2;  // Bits 11:10
    UINT32 EnIOGDDQGLpMode4Mask                    :  1;  // Bits 12:12
    UINT32 DistLvrBonusIn                          :  2;  // Bits 14:13
    UINT32 DistLvrBonusOut                         :  2;  // Bits 16:15
    UINT32 BGVrefBonusIn                           :  2;  // Bits 18:17
    UINT32 BGVref0BonusOut                         :  2;  // Bits 20:19
    UINT32 IOG_LVRVrefEn_MaskforLp4                :  1;  // Bits 21:21
    UINT32 IOG_LVREn_MaskforLp4                    :  1;  // Bits 22:22
    UINT32 ioglvr40_channel_ovrd_en                :  2;  // Bits 24:23
    UINT32 ioglvr40_channel_ovrd_val               :  2;  // Bits 26:25
    UINT32 ioglvr_disable_ovrd                     :  1;  // Bits 27:27
    UINT32 Spare                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_STRUCT;
typedef union {
  struct {
    UINT32 disable_ringosc_h                       :  1;  // Bits 0:0
    UINT32 dcc_enable                              :  1;  // Bits 1:1
    UINT32 pwrup_clk_sel                           :  2;  // Bits 3:2
    UINT32 bypass_clk_sel                          :  1;  // Bits 4:4
    UINT32 ratio_scaling_factor                    :  3;  // Bits 7:5
    UINT32 fdbk_src_sel                            :  2;  // Bits 9:8
    UINT32 dcc_src_sel                             :  2;  // Bits 11:10
    UINT32 pll_refclk_valid                        :  1;  // Bits 12:12
    UINT32 finallocksel_int                        :  1;  // Bits 13:13
    UINT32 finallocksel_ext                        :  1;  // Bits 14:14
    UINT32 clkout3_en                              :  1;  // Bits 15:15
    UINT32 backuppll_qclk_lock                     :  1;  // Bits 16:16
    UINT32 early_lock_selection                    :  1;  // Bits 17:17
    UINT32 keep_regulator_on                       :  1;  // Bits 18:18
    UINT32 spare                                   :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_STRUCT;
typedef union {
  struct {
    UINT32 disable_ringosc_h                       :  1;  // Bits 0:0
    UINT32 dcc_enable                              :  1;  // Bits 1:1
    UINT32 pwrup_clk_sel                           :  2;  // Bits 3:2
    UINT32 bypass_clk_sel                          :  1;  // Bits 4:4
    UINT32 ratio_scaling_factor                    :  3;  // Bits 7:5
    UINT32 fdbk_src_sel                            :  2;  // Bits 9:8
    UINT32 dcc_src_sel                             :  2;  // Bits 11:10
    UINT32 pll_refclk_valid                        :  1;  // Bits 12:12
    UINT32 finallocksel_int                        :  1;  // Bits 13:13
    UINT32 finallocksel_ext                        :  1;  // Bits 14:14
    UINT32 clkout3_en                              :  1;  // Bits 15:15
    UINT32 backuppll_auxclk_lock                   :  1;  // Bits 16:16
    UINT32 keep_regulator_on                       :  1;  // Bits 17:17
    UINT32 auxclkgear                              :  1;  // Bits 18:18
    UINT32 auxpll_lock_ovrd                        :  1;  // Bits 19:19
    UINT32 auxpll_lock_ovrd_en                     :  1;  // Bits 20:20
    UINT32 spare                                   :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRPHY_DDRCOMP_SBMEM_SB_DEBUG_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRPHY_DDRCOMP_SBMEM_CR_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 CMD0                                    :  4;  // Bits 3:0
    UINT32 CMD1                                    :  4;  // Bits 7:4
    UINT32 CMD2                                    :  4;  // Bits 11:8
    UINT32 CMD3                                    :  4;  // Bits 15:12
    UINT32 CMD4                                    :  4;  // Bits 19:16
    UINT32 CMD5                                    :  4;  // Bits 23:20
    UINT32 CMD6                                    :  4;  // Bits 27:24
    UINT32 CMD7                                    :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_PCMD_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 CMD0                                    :  4;  // Bits 3:0
    UINT32 CMD1                                    :  4;  // Bits 7:4
    UINT32 CMD2                                    :  4;  // Bits 11:8
    UINT32 CMD3                                    :  4;  // Bits 15:12
    UINT32 CMD4                                    :  4;  // Bits 19:16
    UINT32 CMD5                                    :  4;  // Bits 23:20
    UINT32 CMD6                                    :  4;  // Bits 27:24
    UINT32 CMD7                                    :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_PCMD_CTRL1_STRUCT;
typedef union {
  struct {
    UINT32 TotalCount                              :  16;  // Bits 15:0
    UINT32 CONTINUOUS_MODE                         :  1;  // Bits 16:16
    UINT32 RUN_MODE                                :  3;  // Bits 19:17
    UINT32 LFSREn                                  :  1;  // Bits 20:20
    UINT32 spare                                   :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_PCMDPATTERN_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 LFSRStartValue0                         :  12;  // Bits 11:0
    UINT32 LFSRSel0                                :  2;  // Bits 13:12
    UINT32 LFSRStartValue1                         :  12;  // Bits 25:14
    UINT32 LFSRSel1                                :  2;  // Bits 27:26
    UINT32 spare                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_PCMDPATTERN_CTRL1_STRUCT;
typedef union {
  struct {
    UINT32 LFSRStartValue2                         :  12;  // Bits 11:0
    UINT32 LFSRSel2                                :  2;  // Bits 13:12
    UINT32 LFSRStartValue3                         :  12;  // Bits 25:14
    UINT32 LFSRSel3                                :  2;  // Bits 27:26
    UINT32 spare                                   :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_PCMDPATTERN_CTRL2_STRUCT;
typedef union {
  struct {
    UINT32 RUN                                     :  1;  // Bits 0:0
    UINT32 spare                                   :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_RUN_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 MASTERCTRL_SINGLE_STEP_EN               :  1;  // Bits 0:0
    UINT32 MASTERCTRL_SINGLE_STEP_ADV              :  1;  // Bits 1:1
    UINT32 CSTATE_SINGLE_STEP_EN                   :  1;  // Bits 2:2
    UINT32 CSTATE_SINGLE_STEP_ADV                  :  1;  // Bits 3:3
    UINT32 SAGV_SINGLE_STEP_EN                     :  1;  // Bits 4:4
    UINT32 SAGV_SINGLE_STEP_ADV                    :  1;  // Bits 5:5
    UINT32 XCTRL_SINGLE_STEP_EN                    :  1;  // Bits 6:6
    UINT32 XCTRL_SINGLE_STEP_ADV                   :  1;  // Bits 7:7
    UINT32 UPCTRL_SINGLE_STEP_EN                   :  1;  // Bits 8:8
    UINT32 UPCTRL_SINGLE_STEP_ADV                  :  1;  // Bits 9:9
    UINT32 LPCTRL_SINGLE_STEP_EN                   :  1;  // Bits 10:10
    UINT32 LPCTRL_SINGLE_STEP_ADV                  :  1;  // Bits 11:11
    UINT32 FSMState                                :  6;  // Bits 17:12
    UINT32 FSMSel                                  :  3;  // Bits 20:18
    UINT32 dislpctrlasynclp3                       :  1;  // Bits 21:21
    UINT32 dislpctrlasynclp4                       :  1;  // Bits 22:22
    UINT32 RSVD                                    :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_DEBUG_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 BreakPoint0                             :  24;  // Bits 23:0
    UINT32 BreakPoint0_En                          :  1;  // Bits 24:24
    UINT32 BreakPoint0_Hit                         :  1;  // Bits 25:25
    UINT32 BREAKPOINT_ADV                          :  1;  // Bits 26:26
    UINT32 RSVD                                    :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_DEBUG_CTRL1_STRUCT;
typedef union {
  struct {
    UINT32 BreakPoint1                             :  24;  // Bits 23:0
    UINT32 BreakPoint1_En                          :  1;  // Bits 24:24
    UINT32 BreakPoint1_Hit                         :  1;  // Bits 25:25
    UINT32 RSVD                                    :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_DEBUG_CTRL2_STRUCT;
typedef union {
  struct {
    UINT32 RunCount                                :  16;  // Bits 15:0
    UINT32 RunCountSel                             :  4;  // Bits 19:16
    UINT32 StartGVPoint                            :  2;  // Bits 21:20
    UINT32 CurrentGVPoint                          :  2;  // Bits 23:22
    UINT32 CurrentTest                             :  4;  // Bits 27:24
    UINT32 PrevTest                                :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_MASTERCTRL_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 FailCount                               :  12;  // Bits 11:0
    UINT32 FailCountSel                            :  4;  // Bits 15:12
    UINT32 FailStopMode                            :  2;  // Bits 17:16
    UINT32 FailStopModeThreshold                   :  6;  // Bits 23:18
    UINT32 FailStopModeSel                         :  4;  // Bits 27:24
    UINT32 FirstFailTest                           :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_FAILMODE_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 TimerCount                              :  5;  // Bits 4:0
    UINT32 PstateOvrdVal                           :  3;  // Bits 7:5
    UINT32 PstateOvrdEn                            :  1;  // Bits 8:8
    UINT32 PreqOvrdVal                             :  1;  // Bits 9:9
    UINT32 PreqOvrdEn                              :  1;  // Bits 10:10
    UINT32 AckOvrdEn                               :  1;  // Bits 11:11
    UINT32 AckOvrdVal                              :  1;  // Bits 12:12
    UINT32 ErrOvrdEn                               :  1;  // Bits 13:13
    UINT32 ErrOvrdVal                              :  1;  // Bits 14:14
    UINT32 ManualCycle                             :  1;  // Bits 15:15
    UINT32 PAccept                                 :  1;  // Bits 16:16
    UINT32 PDeny                                   :  1;  // Bits 17:17
    UINT32 PActive                                 :  8;  // Bits 25:18
    UINT32 PReq                                    :  1;  // Bits 26:26
    UINT32 PState                                  :  3;  // Bits 29:27
    UINT32 Ack                                     :  1;  // Bits 30:30
    UINT32 Err                                     :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_UPCONTROLLER_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 TimerCount                              :  5;  // Bits 4:0
    UINT32 xtfuncovrdval                           :  6;  // Bits 10:5
    UINT32 xtfuncovrden                            :  1;  // Bits 11:11
    UINT32 xtreqovrdval                            :  1;  // Bits 12:12
    UINT32 xtreqovrden                             :  1;  // Bits 13:13
    UINT32 xtackovrdval                            :  1;  // Bits 14:14
    UINT32 xtackovrden                             :  1;  // Bits 15:15
    UINT32 xtreq                                   :  1;  // Bits 16:16
    UINT32 xtfunc                                  :  6;  // Bits 22:17
    UINT32 xtack                                   :  1;  // Bits 23:23
    UINT32 xtcack                                  :  1;  // Bits 24:24
    UINT32 dramstate                               :  2;  // Bits 26:25
    UINT32 reserved                                :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_XTCONTROLLER_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 TimerCount                              :  5;  // Bits 4:0
    UINT32 PkgCExitTimerCount                      :  5;  // Bits 9:5
    UINT32 ExternalTriggerEn                       :  1;  // Bits 10:10
    UINT32 ExternalTrigger                         :  1;  // Bits 11:11
    UINT32 SkipMemtest0                            :  1;  // Bits 12:12
    UINT32 SkipMemtest1                            :  1;  // Bits 13:13
    UINT32 MemBlockCheck                           :  1;  // Bits 14:14
    UINT32 EnRandomTraffic                         :  1;  // Bits 15:15
    UINT32 ManualC3Test                            :  1;  // Bits 16:16
    UINT32 ManualC7Test                            :  1;  // Bits 17:17
    UINT32 ManualPrepAbort                         :  1;  // Bits 18:18
    UINT32 ManualSRAbort                           :  1;  // Bits 19:19
    UINT32 PKGCCounter                             :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_PKGC_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 TimerCount                              :  5;  // Bits 4:0
    UINT32 Reserved                                :  4;  // Bits 8:5
    UINT32 SkipMemtest0                            :  1;  // Bits 9:9
    UINT32 SkipMemtest1                            :  1;  // Bits 10:10
    UINT32 MemBlockCheck                           :  1;  // Bits 11:11
    UINT32 EnRandomTraffic                         :  1;  // Bits 12:12
    UINT32 ManualGVSwitchPoint                     :  2;  // Bits 14:13
    UINT32 ManualGVSwitch                          :  1;  // Bits 15:15
    UINT32 SAGVCounter                             :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_SAGV_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 TimerCount                              :  5;  // Bits 4:0
    UINT32 LPExitTimerCount                        :  8;  // Bits 12:5
    UINT32 ExternalTriggerEn                       :  1;  // Bits 13:13
    UINT32 ExternalTrigger                         :  1;  // Bits 14:14
    UINT32 SkipMemtest0                            :  1;  // Bits 15:15
    UINT32 SkipMemtest1                            :  1;  // Bits 16:16
    UINT32 MemBlockCheck                           :  1;  // Bits 17:17
    UINT32 ManualLP3Test                           :  1;  // Bits 18:18
    UINT32 ManualLP4Test                           :  1;  // Bits 19:19
    UINT32 LPModeCounter                           :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_LPMODE_CTRL0_STRUCT;
typedef union {
  struct {
    UINT32 MemTestPass                             :  1;  // Bits 0:0
    UINT32 MemTestFail                             :  1;  // Bits 1:1
    UINT32 MemTestCode                             :  6;  // Bits 7:2
    UINT32 PkgCPass                                :  1;  // Bits 8:8
    UINT32 PkgCFail                                :  1;  // Bits 9:9
    UINT32 PkgCFailCode                            :  6;  // Bits 15:10
    UINT32 SAGVPass                                :  1;  // Bits 16:16
    UINT32 SAGVFail                                :  1;  // Bits 17:17
    UINT32 SAGVFailCode                            :  6;  // Bits 23:18
    UINT32 LPModePass                              :  1;  // Bits 24:24
    UINT32 LPModeFail                              :  1;  // Bits 25:25
    UINT32 LPModeFailCode                          :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_TESTSTATUS_STRUCT;
typedef union {
  struct {
    UINT32 pchannel_mux_sel                        :  1;  // Bits 0:0
    UINT32 reserved                                :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDRPMSTC_MISC_CTRL0_STRUCT;



#pragma pack(pop)
#endif /* __MrcRegisterStructPtl4xxx_h__ */







