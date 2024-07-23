/** @file
  Definition of DDR5 Registers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
  DDR5 JEDEC Spec
**/

#ifndef _MRC_DDR5_REGISTERS_H_
#define _MRC_DDR5_REGISTERS_H_

#include "CMrcTypes.h"

/// Enumerations and Defines
/**
  DDR5 MR0 Read Latency Values
**/
typedef enum {
  Ddr5Rl_22,
  Ddr5Rl_24,
  Ddr5Rl_26,
  Ddr5Rl_28,
  Ddr5Rl_30,
  Ddr5Rl_32,
  Ddr5Rl_34,
  Ddr5Rl_36,
  Ddr5Rl_38,
  Ddr5Rl_40,
  Ddr5Rl_42,
  Ddr5Rl_44,
  Ddr5Rl_46,
  Ddr5Rl_48,
  Ddr5Rl_50,
  Ddr5Rl_52,
  Ddr5Rl_54,
  Ddr5Rl_56,
  Ddr5Rl_58,
  Ddr5Rl_60,
  Ddr5Rl_62,
  Ddr5Rl_64,
  Ddr5Rl_66,
  Ddr5Rl_68,
  Ddr5Rl_70,
  Ddr5Rl_72,
  Ddr5Rl_74,
  Ddr5Rl_76,
  Ddr5Rl_78,
  Ddr5Rl_80,
  Ddr5Rl_82,
  Ddr5Rl_84,
  Ddr5Rl_86,
  Ddr5Rl_88,
  Ddr5Rl_90,
  Ddr5RlMax
} DDR5_MR0_RL;


/**
  DDR5 MR6 Write Recovery Time Values
**/
typedef enum {
  Ddr5Wr_48,
  Ddr5Wr_54,
  Ddr5Wr_60,
  Ddr5Wr_66,
  Ddr5Wr_72,
  Ddr5Wr_78,
  Ddr5Wr_84,
  Ddr5Wr_90,
  Ddr5Wr_96,
  Ddr5Wr_102,
  Ddr5Wr_108,
  Ddr5Wr_114,
  Ddr5Wr_120,
  Ddr5Wr_126,
  Ddr5Wr_132,
  Ddr5WrMax
} DDR5_MR6_WR;


/**
  DDR5 MR6 tRTP
**/
typedef enum {
  Ddr5Trtp_12,
  Ddr5Trtp_14,
  Ddr5Trtp_15,
  Ddr5Trtp_17,
  Ddr5Trtp_18,
  Ddr5Trtp_20,
  Ddr5Trtp_21,
  Ddr5Trtp_23,
  Ddr5Trtp_24,
  Ddr5Trtp_26,
  Ddr5Trtp_27,
  Ddr5Trtp_28,
  Ddr5Trtp_30,
  Ddr5Trtp_32,
  Ddr5Trtp_33,
  Ddr5TrtpMax
} DDR5_MR6_TRTP;


/**
  DDR5 MR8 Read Preamble Settings
**/
typedef enum {
  Ddr5tRPRE_1tCK_10,
  Ddr5tRPRE_2tCK_0010,
  Ddr5tRPRE_2tCK_1110, // (DDR4 Style)
  Ddr5tRPRE_3tCK_000010,
  Ddr5tRPRE_4tCK_00001010
} DDR5_MR8_tRPRE;

/**
  DDR5 MR8 Write Preamble Settings
**/
typedef enum {
  Ddr5tWPRE_Reserved,
  Ddr5tWPRE_2tCK_0010,
  Ddr5tWPRE_3tCK_000010,
  Ddr5tWPRE_4tCK_00001010
} DDR5_MR8_tWPRE;

/**
  DDR5 MR8 Write Postamble Settings
**/
typedef enum {
  Ddr5tWPOST_0pt5tCK_0,
  Ddr5tWPOST_1pt5tCK_000
} DDR5_MR8_tWPOST;

/**
  DDR5 MR10, MR11, and MR12 VREF Calibration Settings
**/
typedef enum {
  Ddr5Vref_97p5,
  Ddr5Vref_97p0,
  Ddr5Vref_96p5,
  Ddr5Vref_96p0,
  Ddr5Vref_95p5,
  Ddr5Vref_95p0,
  Ddr5Vref_94p5,
  Ddr5Vref_94p0,
  Ddr5Vref_93p5,
  Ddr5Vref_93p0,
  Ddr5Vref_92p5,
  Ddr5Vref_92p0,
  Ddr5Vref_91p5,
  Ddr5Vref_91p0,
  Ddr5Vref_90p5,
  Ddr5Vref_90p0,
  Ddr5Vref_89p5,
  Ddr5Vref_89p0,
  Ddr5Vref_88p5,
  Ddr5Vref_88p0,
  Ddr5Vref_87p5,
  Ddr5Vref_87p0,
  Ddr5Vref_86p5,
  Ddr5Vref_86p0,
  Ddr5Vref_85p5,
  Ddr5Vref_85p0,
  Ddr5Vref_84p5,
  Ddr5Vref_84p0,
  Ddr5Vref_83p5,
  Ddr5Vref_83p0,
  Ddr5Vref_82p5,
  Ddr5Vref_82p0,
  Ddr5Vref_81p5,
  Ddr5Vref_81p0,
  Ddr5Vref_80p5,
  Ddr5Vref_80p0,
  Ddr5Vref_79p5,
  Ddr5Vref_79p0,
  Ddr5Vref_78p5,
  Ddr5Vref_78p0,
  Ddr5Vref_77p5,
  Ddr5Vref_77p0,
  Ddr5Vref_76p5,
  Ddr5Vref_76p0,
  Ddr5Vref_75p5,
  Ddr5Vref_75p0,
  Ddr5Vref_74p5,
  Ddr5Vref_74p0,
  Ddr5Vref_73p5,
  Ddr5Vref_73p0,
  Ddr5Vref_72p5,
  Ddr5Vref_72p0,
  Ddr5Vref_71p5,
  Ddr5Vref_71p0,
  Ddr5Vref_70p5,
  Ddr5Vref_70p0,
  Ddr5Vref_69p5,
  Ddr5Vref_69p0,
  Ddr5Vref_68p5,
  Ddr5Vref_68p0,
  Ddr5Vref_67p5,
  Ddr5Vref_67p0,
  Ddr5Vref_66p5,
  Ddr5Vref_66p0,
  Ddr5Vref_65p5,
  Ddr5Vref_65p0,
  Ddr5Vref_64p5,
  Ddr5Vref_64p0,
  Ddr5Vref_63p5,
  Ddr5Vref_63p0,
  Ddr5Vref_62p5,
  Ddr5Vref_62p0,
  Ddr5Vref_61p5,
  Ddr5Vref_61p0,
  Ddr5Vref_60p5,
  Ddr5Vref_60p0,
  Ddr5Vref_59p5,
  Ddr5Vref_59p0,
  Ddr5Vref_58p5,
  Ddr5Vref_58p0,
  Ddr5Vref_57p5,
  Ddr5Vref_57p0,
  Ddr5Vref_56p5,
  Ddr5Vref_56p0,
  Ddr5Vref_55p5,
  Ddr5Vref_55p0,
  Ddr5Vref_54p5,
  Ddr5Vref_54p0,
  Ddr5Vref_53p5,
  Ddr5Vref_53p0,
  Ddr5Vref_52p5,
  Ddr5Vref_52p0,
  Ddr5Vref_51p5,
  Ddr5Vref_51p0,
  Ddr5Vref_50p5,
  Ddr5Vref_50p0,
  Ddr5Vref_49p5,
  Ddr5Vref_49p0,
  Ddr5Vref_48p5,
  Ddr5Vref_48p0,
  Ddr5Vref_47p5,
  Ddr5Vref_47p0,
  Ddr5Vref_46p5,
  Ddr5Vref_46p0,
  Ddr5Vref_45p5,
  Ddr5Vref_45p0,
  Ddr5Vref_44p5,
  Ddr5Vref_44p0,
  Ddr5Vref_43p5,
  Ddr5Vref_43p0,
  Ddr5Vref_42p5,
  Ddr5Vref_42p0,
  Ddr5Vref_41p5,
  Ddr5Vref_41p0,
  Ddr5Vref_40p5,
  Ddr5Vref_40p0,
  Ddr5Vref_39p5,
  Ddr5Vref_39p0,
  Ddr5Vref_38p5,
  Ddr5Vref_38p0,
  Ddr5Vref_37p5,
  Ddr5Vref_37p0,
  Ddr5Vref_36p5,
  Ddr5Vref_36p0,
  Ddr5Vref_35p5,
  Ddr5Vref_35p0
} DDR5_MR10_VREF;

/**
  DDR5 MR32 and MR33 Values for
  CK ODT, CS ODT, and CA ODT
**/
typedef enum {
  CkCsCaOdt_RTT_OFF,
  CkCsCaOdt_RZQ_0p5_480,
  CkCsCaOdt_RZQ_1_240,
  CkCsCaOdt_RZQ_2_120,
  CkCsCaOdt_RZQ_3_80,
  CkCsCaOdt_RZQ_4_60,
  CkCsCaOdt_RZQ_Reserved,
  CkCsCaOdt_RZQ_6_40,
} DDR5_MR32_CKCSCA_ODT;

/**
  DDR5 MR33, MR34 and MR35 Values for
  DQS_RTT_PARK, RTT_PARK, RTT_WR,
  RTT_NOM_WR, and RTT_NOM_RD
**/
typedef enum {
  Rtt_RTT_OFF,
  Rtt_RZQ_240,
  Rtt_RZQ_2_120,
  Rtt_RZQ_3_80,
  Rtt_RZQ_4_60,
  Rtt_RZQ_5_48,
  Rtt_RZQ_6_40,
  Rtt_RZQ_7_34,
} DDR5_MR33_RTT_PARK;

/**
  DDR5 MR37 ODTLon_WR_Offset
  DDR5 MR38 ODTLon_WR_NT_Offset
**/
typedef enum {
  OdtlOnWrOffsetReserved,
  OdtlOnWrOffsetMinus4,
  OdtlOnWrOffsetMinus3,
  OdtlOnWrOffsetMinus2,
  OdtlOnWrOffsetMinus1,
  OdtlOnWrOffset0,
  OdtlOnWrOffsetPlus1,
  OdtlOnWrOffsetPlus2,
} DDR5_MR37_ODTLON_WR_OFFSET;

/**
  DDR5 MR37 ODTLoff_WR_Offset
  DDR5 MR38 ODTLoff_WR_NT_Offset
**/
typedef enum {
  OdtlOffWrOffsetReserved,
  OdtlOffWrOffsetPlus4,
  OdtlOffWrOffsetPlus3,
  OdtlOffWrOffsetPlus2,
  OdtlOffWrOffsetPlus1,
  OdtlOffWrOffset0,
  OdtlOffWrOffsetMinus1,
  OdtlOffWrOffsetMinus2,
} DDR5_MR37_ODTLOFF_WR_OFFSET;

/**
  DDR5 MR39 ODTLon_RD_NT_Offset
**/
typedef enum {
  OdtlOnRdNtOffsetReserved0,
  OdtlOnRdNtOffsetReserved1,
  OdtlOnRdNtOffsetMinus3,
  OdtlOnRdNtOffsetMinus2,
  OdtlOnRdNtOffsetMinus1,
  OdtlOnRdNtOffset0,
  OdtlOnRdNtOffsetPlus1,
  OdtlOnRdNtOffsetReserved2,
} DDR5_MR39_ODTLON_RD_NT_OFFSET;

/**
  DDR5 MR39 ODTLoff_RD_NT_Offset
**/
typedef enum {
  OdtlOffRdNtOffsetReserved0,
  OdtlOffRdNtOffsetReserved1,
  OdtlOffRdNtOffsetPlus3,
  OdtlOffRdNtOffsetPlus2,
  OdtlOffRdNtOffsetPlus1,
  OdtlOffRdNtOffset0,
  OdtlOffRdNtOffsetMinus1,
  OdtlOffRdNtOffsetReserved2,
} DDR5_MR39_ODTLOFF_RD_NT_OFFSET;

/// Structure Definitions
#pragma pack(push, 1)
typedef union {
  struct {
    UINT8 BurstLength                         : 2;  // Bits 1:0
    UINT8 CasLatency                          : 6;  // Bits 7:2
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_0_TYPE;

typedef union {
  struct {
    UINT8 PdaEnumerateId                      : 4;  // Bits 3:0
    UINT8 PdaSelectId                         : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_1_TYPE;

typedef union {
  struct {
    UINT8 ReadPreambleTraining                : 1;  // Bits 0:0
    UINT8 WriteLevelingTraining               : 1;  // Bits 1:1
    UINT8 Mode1n                              : 1;  // Bits 2:2
    UINT8 MaxPowerSavingsMode                 : 1;  // Bits 3:3
    UINT8 CsAssertionDuration                 : 1;  // Bits 4:4
    UINT8 Device15Mpsm                        : 1;  // Bits 5:5
    UINT8                                     : 1;  // Bits 6:6
    UINT8 InternalWriteTiming                 : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_2_TYPE;

typedef union {
  struct {
    UINT8 WriteLevelingInternalCycleLowerByte : 4;  // Bits 3:0
    UINT8 WriteLevelingInternalCycleUpperByte : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_3_TYPE;

typedef union {
  struct {
    UINT8 RefreshRate                         : 3;  // Bits 2:0
    UINT8 RefreshIntervalRateIndicator        : 1;  // Bits 3:3
    UINT8 RefreshTrfcMode                     : 1;  // Bits 4:4
    UINT8                                     : 2;  // Bits 6:5
    UINT8 Tuf                                 : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_4_TYPE;

typedef union {
  struct {
    UINT8 DataOutputDisable                   : 1;  // Bits 0:0
    UINT8 PullUpOutputDriverImpedance         : 2;  // Bits 2:1
    UINT8 PodtmSupport                        : 1;  // Bits 3:3
    UINT8 TdqsEnable                          : 1;  // Bits 4:4
    UINT8 DmEnable                            : 1;  // Bits 5:5
    UINT8 PullDownOutputDriverImpedance       : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_5_TYPE;

typedef union {
  struct {
    UINT8 WriteRecoveryTime                   : 4;  // Bits 3:0
    UINT8 tRTP                                : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_6_TYPE;

typedef union {
  struct {
    UINT8 WriteLevelingInternalCycleHalfStepLowerByte : 1;  // Bits 0:0
    UINT8 WriteLevelingInternalCycleHalfStepUpperByte : 1;  // Bits 1:1
    UINT8                                             : 6;  // Bits 7:2
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_7_TYPE;

typedef union {
  struct {
    UINT8 ReadPreambleSettings                : 3;  // Bits 2:0
    UINT8 WritePreambleSettings               : 2;  // Bits 4:3
    UINT8                                     : 1;  // Bits 5:5
    UINT8 ReadPostambleSettings               : 1;  // Bits 6:6
    UINT8 WritePostambleSettings              : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_8_TYPE;

typedef union {
  struct {
    UINT8 IntVrefDqMon                        : 1;  // Bits 0:0
    UINT8 IntVrefCaMon                        : 1;  // Bits 1:1
    UINT8                                     : 5;  // Bits 6:2
    UINT8 Tm                                  : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_9_TYPE;

typedef union {
  struct {
    UINT8 VrefDqCalibrationValue              : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_10_TYPE;

typedef union {
  struct {
    UINT8 VrefCaCalibrationValue              : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_11_TYPE;

typedef union {
  struct {
    UINT8 VrefCsCalibrationValue              : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_12_TYPE;

typedef union {
  struct {
    UINT8 tCCD_L_tDLLK                        : 4;  // Bits 3:0
    UINT8 CS_GEAR_DOWN_ENABLE                 : 1;  // Bits 4:4
    UINT8                                     : 3;  // Bits 7:5
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_13_TYPE;

typedef union {
  struct {
    UINT8 Cid0                                : 1;  // Bits 0:0
    UINT8 Cid1                                : 1;  // Bits 1:1
    UINT8 Cid2                                : 1;  // Bits 2:2
    UINT8 Cid3                                : 1;  // Bits 3:3
    UINT8                                     : 1;  // Bits 4:4
    UINT8 RowMode_CodeWordMode                : 1;  // Bits 5:5
    UINT8 ResetEcsCounter                     : 1;  // Bits 6:6
    UINT8 EcsMode                             : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_14_TYPE;

typedef union {
  struct {
    UINT8 EcsErrorThresholdCount              : 3;  // Bits 2:0
    UINT8                                     : 5;  // Bits 7:3
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_15_TYPE;

typedef union {
  struct {
    UINT8 MaxRowErrorAddresLsb                : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_16_TYPE;

typedef union {
  struct {
    UINT8 MaxRowErrorAddresMsb                : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_17_TYPE;

typedef union {
  struct {
    UINT8 MaxRowErrorAddresBgBaR              : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_18_TYPE;

typedef union {
  struct {
    UINT8 MaxRowErrorCount                    : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_19_TYPE;

typedef union {
  struct {
    UINT8 ErrorCount                          : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_20_TYPE;

// MR21 is reserved

typedef union {
  struct {
    UINT8 MbistStatus                         : 3;  // Bits 2:0
    UINT8 RxCtleSupport                       : 1;  // Bits 3:3
    UINT8 RxCaCtleCtrl                        : 2;  // Bits 5:4
    UINT8 RxCsCtleCtrl                        : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_22_TYPE;

typedef union {
  struct {
    UINT8 Hppr                                : 1;  // Bits 0:0
    UINT8 Sppr                                : 2;  // Bits 2:1
    UINT8 Mppr                                : 1;  // Bits 3:3
    UINT8 Mbist                               : 1;  // Bits 4:4
    UINT8                                     : 3;  // Bits 7:5
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_23_TYPE;

// MR24 is not used

typedef union {
  struct {
    UINT8 ReadTrainingPatternFormat           : 1;  // Bits 0:0
    UINT8 Lsfr0PatternOption                  : 1;  // Bits 1:1
    UINT8 Lfsr1PatternOption                  : 1;  // Bits 2:2
    UINT8 ContinuousBurstMode                 : 1;  // Bits 3:3
    UINT8                                     : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_25_TYPE;

typedef union {
  struct {
    UINT8 ReadTrainingPatternData0_Lfsr0Seed  : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_26_TYPE;

typedef union {
  struct {
    UINT8 ReadTrainingPatternData1_Lfsr1Seed  : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_27_TYPE;

typedef union {
  struct {
    UINT8 ReadTrainingPatternInvertDqL        : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_28_TYPE;

typedef union {
  struct {
    UINT8 ReadTrainingPatternInvertDqU        : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_29_TYPE;

typedef union {
  struct {
    UINT8 LfsrAssignmentDq0                   : 1;  // Bits 0:0
    UINT8 LfsrAssignmentDq1                   : 1;  // Bits 1:1
    UINT8 LfsrAssignmentDq2                   : 1;  // Bits 2:2
    UINT8 LfsrAssignmentDq3                   : 1;  // Bits 3:3
    UINT8 LfsrAssignmentDq4                   : 1;  // Bits 4:4
    UINT8 LfsrAssignmentDq5                   : 1;  // Bits 5:5
    UINT8 LfsrAssignmentDq6                   : 1;  // Bits 6:6
    UINT8 LfsrAssignmentDq7                   : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_30_TYPE;

typedef union {
  struct {
    UINT8 ReadTrainingPatternAddress          : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_31_TYPE;

typedef union {
  struct {
    UINT8 CkOdt                               : 3;  // Bits 2:0
    UINT8 CsOdt                               : 3;  // Bits 5:3
    UINT8 CaOdtStrapValue                     : 1;  // Bits 6:6
    UINT8                                     : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_32_TYPE;

typedef union {
  struct {
    UINT8 CaOdt                               : 3;  // Bits 2:0
    UINT8 DqsRttPark                          : 3;  // Bits 5:3
    UINT8                                     : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_33_TYPE;

typedef union {
  struct {
    UINT8 RttPark                             : 3;  // Bits 2:0
    UINT8 RttWr                               : 3;  // Bits 5:3
    UINT8                                     : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_34_TYPE;

typedef union {
  struct {
    UINT8 RttNomWr                            : 3;  // Bits 2:0
    UINT8 RttNomRd                            : 3;  // Bits 5:3
    UINT8                                     : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_35_TYPE;

typedef union {
  struct {
    UINT8 RttLoopback                         : 3;  // Bits 2:0
    UINT8                                     : 5;  // Bits 7:3
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_36_TYPE;

typedef union {
  struct {
    UINT8 OdtlOnWrOffset                      : 3;  // Bits 2:0
    UINT8 OdtlOffWrOffset                     : 3;  // Bits 5:3
    UINT8                                     : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_37_TYPE;

typedef union {
  struct {
    UINT8 OdtlOnWrNtOffset                    : 3;  // Bits 2:0
    UINT8 OdtlOffWrNtOffset                   : 3;  // Bits 5:3
    UINT8                                     : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_38_TYPE;

typedef union {
  struct {
    UINT8 OdtlOnRdNtOffset                    : 3;  // Bits 2:0
    UINT8 OdtlOffRdNtOffset                   : 3;  // Bits 5:3
    UINT8                                     : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_39_TYPE;

typedef union {
  struct {
    UINT8 ReadDqsOffsetTiming                 : 3;  // Bits 2:0
    UINT8                                     : 5;  // Bits 7:3
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_40_TYPE;

// MR41 is reserved

typedef union {
  struct {
    UINT8 DcaTypesSupported                   : 2;  // Bits 1:0
    UINT8 DcaTrainingAssistMode               : 2;  // Bits 3:2
    UINT8                                     : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_42_TYPE;

typedef union {
  struct {
    UINT8 DcaForQclkIn4PhaseClk               : 3;  // Bits 2:0
    UINT8 DcaForQclkSignBit                   : 1;  // Bits 3:3
    UINT8 DcaForIbclkIn4PhaseClk              : 3;  // Bits 6:4
    UINT8 DcaForIclkSignBit                   : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_43_TYPE;

typedef union {
  struct {
    UINT8 DcaForQbclkIn4PhaseClk              : 3;  // Bits 2:0
    UINT8 DcaForQbclkSignBit                  : 1;  // Bits 3:3
    UINT8                                     : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_44_TYPE;

typedef union {
  struct {
    UINT8 DqsIntervalTimerRunTime             : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_45_TYPE;

typedef union {
  struct {
    UINT8 DqsOscillatorCountLsb               : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_46_TYPE;

typedef union {
  struct {
    UINT8 DqsOscillatorCountMsb               : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_47_TYPE;

typedef union {
  struct {
    UINT8 WritePatternMode                    : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_48_TYPE;

// MR49 Reserved

typedef union {
  struct {
    UINT8 ReadCrcEnable                       : 1;  // Bits 0:0
    UINT8 WriteCrcEnableLowerNibble           : 1;  // Bits 1:1
    UINT8 WriteCrcEnableUpperNibble           : 1;  // Bits 2:2
    UINT8 WriteCrcErrorStatus                 : 1;  // Bits 3:3
    UINT8 WriteCrcAutoDisableEnable           : 1;  // Bits 4:4
    UINT8 WriteCrcAutoDisableStatus           : 1;  // Bits 5:5
    UINT8                                     : 1;  // Bits 6:6
    UINT8                                     : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_50_TYPE;

typedef union {
  struct {
    UINT8 WriteCrcAutoDisableThreshold        : 7;  // Bits 6:0
    UINT8                                     : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_51_TYPE;

typedef union {
  struct {
    UINT8 WriteCrcAutoDisableWindow           : 7;  // Bits 6:0
    UINT8                                     : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_52_TYPE;

typedef union {
  struct {
    UINT8 LoopbackOutputSelect                : 5;  // Bits 4:0
    UINT8 LoopbackSelectPhase                 : 2;  // Bits 6:5
    UINT8 LoopbackOutputMode                  : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_53_TYPE;

typedef union {
  struct {
    UINT8 HpprResourceBg0Bank0                : 1;  // Bits 0:0
    UINT8 HpprResourceBg0Bank1                : 1;  // Bits 1:1
    UINT8 HpprResourceBg0Bank2                : 1;  // Bits 2:2
    UINT8 HpprResourceBg0Bank3                : 1;  // Bits 3:3
    UINT8 HpprResourceBg1Bank0                : 1;  // Bits 4:4
    UINT8 HpprResourceBg1Bank1                : 1;  // Bits 5:5
    UINT8 HpprResourceBg1Bank2                : 1;  // Bits 6:6
    UINT8 HpprResourceBg1Bank3                : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_54_TYPE;

typedef union {
  struct {
    UINT8 HpprResourceBg2Bank0                : 1;  // Bits 0:0
    UINT8 HpprResourceBg2Bank1                : 1;  // Bits 1:1
    UINT8 HpprResourceBg2Bank2                : 1;  // Bits 2:2
    UINT8 HpprResourceBg2Bank3                : 1;  // Bits 3:3
    UINT8 HpprResourceBg3Bank0                : 1;  // Bits 4:4
    UINT8 HpprResourceBg3Bank1                : 1;  // Bits 5:5
    UINT8 HpprResourceBg3Bank2                : 1;  // Bits 6:6
    UINT8 HpprResourceBg3Bank3                : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_55_TYPE;

typedef union {
  struct {
    UINT8 HpprResourceBg4Bank0                : 1;  // Bits 0:0
    UINT8 HpprResourceBg4Bank1                : 1;  // Bits 1:1
    UINT8 HpprResourceBg4Bank2                : 1;  // Bits 2:2
    UINT8 HpprResourceBg4Bank3                : 1;  // Bits 3:3
    UINT8 HpprResourceBg5Bank0                : 1;  // Bits 4:4
    UINT8 HpprResourceBg5Bank1                : 1;  // Bits 5:5
    UINT8 HpprResourceBg5Bank2                : 1;  // Bits 6:6
    UINT8 HpprResourceBg5Bank3                : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_56_TYPE;

typedef union {
  struct {
    UINT8 HpprResourceBg6Bank0                : 1;  // Bits 0:0
    UINT8 HpprResourceBg6Bank1                : 1;  // Bits 1:1
    UINT8 HpprResourceBg6Bank2                : 1;  // Bits 2:2
    UINT8 HpprResourceBg6Bank3                : 1;  // Bits 3:3
    UINT8 HpprResourceBg7Bank0                : 1;  // Bits 4:4
    UINT8 HpprResourceBg7Bank1                : 1;  // Bits 5:5
    UINT8 HpprResourceBg7Bank2                : 1;  // Bits 6:6
    UINT8 HpprResourceBg7Bank3                : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_57_TYPE;

typedef union {
  struct {
    UINT8 RfmRequired                         : 1;  // Bits 0:0
    UINT8 Raaimt                              : 4;  // Bits 4:1
    UINT8 Raammt                              : 3;  // Bits 7:5
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_58_TYPE;

typedef union {
  struct {
    UINT8 DRFE                                : 1;  // Bits 0:0
    UINT8 BRC                                 : 2;  // Bits 2:1
    UINT8 BRCSupport                          : 1;  // Bits 3:3
    UINT8 ARFM                                : 2;  // Bits 5:4
    UINT8 RfmRaaCounter                       : 2;  // Bits 7:6
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_59_TYPE;

// MR60 Reserved

typedef union {
  struct {
    UINT8 PackageOutputDriverTestMode         : 5;  // Bits 4:0
    UINT8                                     : 3;  // Bits 7:5
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_61_TYPE;

// MR62 is vendor specific

typedef union {
  struct {
    UINT8 DramScratchPad                      : 8;  // Bits 7:0
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_63_TYPE;

// MR64 to MR102 Reserved

typedef union {
  struct {
    UINT8 DqsltDcaForQclk                     : 2;  // Bits 1:0
    UINT8                                     : 1;  // Bits 2:2
    UINT8 DqsltQclkSign                       : 1;  // Bits 3:3
    UINT8 DqsltDcaForIbclk                    : 2;  // Bits 5:4
    UINT8                                     : 1;  // Bits 6:6
    UINT8 DqsltIbclkSign                      : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_103_TYPE;

typedef union {
  struct {
    UINT8 DqsltDcaForQbclk                    : 2;  // Bits 1:0
    UINT8                                     : 1;  // Bits 2:2
    UINT8 DqsltQbclkSign                      : 1;  // Bits 3:3
    UINT8                                     : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_104_TYPE;

typedef union {
  struct {
    UINT8 DqslcDcaForQclk                     : 2;  // Bits 1:0
    UINT8                                     : 1;  // Bits 2:2
    UINT8 DqslcQclkSign                       : 1;  // Bits 3:3
    UINT8 DqslcDcaForIbclk                    : 2;  // Bits 5:4
    UINT8                                     : 1;  // Bits 6:6
    UINT8 DqslcIbclkSign                      : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_105_TYPE;

typedef union {
  struct {
    UINT8 DqslcDcaForQbclk                    : 2;  // Bits 1:0
    UINT8                                     : 1;  // Bits 2:2
    UINT8 DqslcQbclkSign                      : 1;  // Bits 3:3
    UINT8                                     : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_106_TYPE;

typedef union {
  struct {
    UINT8 DqsutDcaForQclk                     : 2;  // Bits 1:0
    UINT8                                     : 1;  // Bits 2:2
    UINT8 DqsutQclkSign                       : 1;  // Bits 3:3
    UINT8 DqsutDcaForIbclk                    : 2;  // Bits 5:4
    UINT8                                     : 1;  // Bits 6:6
    UINT8 DqsutIbclkSign                      : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_107_TYPE;

typedef union {
  struct {
    UINT8 DqsutDcaForQbclk                    : 2;  // Bits 1:0
    UINT8                                     : 1;  // Bits 2:2
    UINT8 DqsutQbclkSign                      : 1;  // Bits 3:3
    UINT8                                     : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_108_TYPE;

typedef union {
  struct {
    UINT8 DqsucDcaForQclk                     : 2;  // Bits 1:0
    UINT8                                     : 1;  // Bits 2:2
    UINT8 DqsucQclkSign                       : 1;  // Bits 3:3
    UINT8 DqsucDcaForIbclk                    : 2;  // Bits 5:4
    UINT8                                     : 1;  // Bits 6:6
    UINT8 DqsucIbclkSign                      : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_109_TYPE;

typedef union {
  struct {
    UINT8 DqsucDcaForQbclk                    : 2;  // Bits 1:0
    UINT8                                     : 1;  // Bits 2:2
    UINT8 DqsucQbclkSign                      : 1;  // Bits 3:3
    UINT8                                     : 4;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_110_TYPE;

typedef union {
  struct {
    UINT8 GlobalDfeGainEnable                 : 1;  // Bits 0:0
    UINT8 GlobalDfeTap1Enable                 : 1;  // Bits 1:1
    UINT8 GlobalDfeTap2Enable                 : 1;  // Bits 2:2
    UINT8 GlobalDfeTap3Enable                 : 1;  // Bits 3:3
    UINT8 GlobalDfeTap4Enable                 : 1;  // Bits 4:4
    UINT8                                     : 3;  // Bits 7:5
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_111_TYPE;

typedef union {
  struct {
    UINT8 DfeGainBias                         : 3;  // Bits 2:0
    UINT8 GainBiasSignBit                     : 1;  // Bits 3:3
    UINT8                                     : 3;  // Bits 7:4
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_112_TYPE;

typedef union {
  struct {
    UINT8 DfeTap1Bias                         : 6;  // Bits 5:0
    UINT8 DfeTap1BiasSignBit                  : 1;  // Bits 6:6
    UINT8 DfeTap1Enable                       : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_129_TYPE;

typedef union {
  struct {
    UINT8 DfeTap2Bias                         : 6;  // Bits 5:0
    UINT8 DfeTap2BiasSignBit                  : 1;  // Bits 6:6
    UINT8 DfeTap2Enable                       : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_130_TYPE;

typedef union {
  struct {
    UINT8 DfeTap3Bias                         : 6;  // Bits 5:0
    UINT8 DfeTap3BiasSignBit                  : 1;  // Bits 6:6
    UINT8 DfeTap3Enable                       : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_131_TYPE;

typedef union {
  struct {
    UINT8 DfeTap4Bias                         : 6;  // Bits 5:0
    UINT8 DfeTap4BiasSignBit                  : 1;  // Bits 6:6
    UINT8 DfeTap4Enable                       : 1;  // Bits 7:7
  } Bits;
  UINT8 Data8;
} DDR5_MODE_REGISTER_132_TYPE;

// MR118 to MR255 not used

#pragma pack(pop)
#endif // _MRC_DDR5_REGISTERS_H_

