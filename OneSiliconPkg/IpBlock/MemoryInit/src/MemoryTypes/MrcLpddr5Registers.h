/** @file
  Definition of LPDDR5 Registers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _MRC_LPDDR5_REGISTERS_H_
#define _MRC_LPDDR5_REGISTERS_H_

/// Enumerations and Defines
#define LPDDR5_VREF_FIELD_MAX (127)

typedef enum {
  Lp5LmX16,
  Lp5LmByteMode,
  Lp5LmMax
} LPDDR5_LATENCY_MODE;

typedef enum {
  Lp5CkmDifferential,
  Lp5CkmSingleEnd,
  Lp5CkmMax
} LPDDR5_CK_MODE;

/**
  This list is for DVFSC disabled, x16/x8 DRAM Set B Latency
**/
typedef enum {
  Lp5Wl4t1B2,
  Lp5Wl4t1B3,
  Lp5Wl4t1B4,
  Lp5Wl4t1B5,
  Lp5Wl4t1B7,
  Lp5Wl4t1B8,
  Lp5Wl4t1B9,
  Lp5Wl4t1B11,
  Lp5Wl4t1B12,
  Lp5Wl4t1B14,
  Lp5Wl4t1B15,
  Lp5Wl4t1B16,
  Lp5Wl4t1B19,
  Lp5Wl4t1B22,
  Lp5Wl4t1B24,
  Lp5Wl4t1B26,
  Lp5Wl4t1BMax
} LPDDR5_WR_LATENCY_4t1_B;

/**
  This list is for DVFSC disabled, x16, DBI Disabled
**/
typedef enum {
  Lp5RlRbtp4t1X16_3,
  Lp5RlRbtp4t1X16_4,
  Lp5RlRbtp4t1X16_5,
  Lp5RlRbtp4t1X16_6,
  Lp5RlRbtp4t1X16_8,
  Lp5RlRbtp4t1X16_9,
  Lp5RlRbtp4t1X16_10,
  Lp5RlRbtp4t1X16_12,
  Lp5RlRbtp4t1X16_13,
  Lp5RlRbtp4t1X16_15,
  Lp5RlRbtp4t1X16_16,
  Lp5RlRbtp4t1X16_17,
  Lp5RlRbtp4t1X16_20,
  Lp5RlRbtp4t1X16_23,
  Lp5RlRbtp4t1X16_25,
  Lp5RlRbtp4t1X16_28,
  Lp5RlRbtp4t1X16Max
} LPDDR5_RL_RBTP_4t1_Set0;

/**
  This list is for DVFSC disabled, x8, DBI Disabled or x16, DBI Enabled
**/
typedef enum {
  Lp5RlRbtp4t1X8_3,
  Lp5RlRbtp4t1X8_4,
  Lp5RlRbtp4t1X8_5,
  Lp5RlRbtp4t1X8_7,
  Lp5RlRbtp4t1X8_8,
  Lp5RlRbtp4t1X8_10,
  Lp5RlRbtp4t1X8_11,
  Lp5RlRbtp4t1X8_13,
  Lp5RlRbtp4t1X8_14,
  Lp5RlRbtp4t1X8_16,
  Lp5RlRbtp4t1X8_17,
  Lp5RlRbtp4t1X8_18,
  Lp5RlRbtp4t1X8_22,
  Lp5RlRbtp4t1X8_25,
  Lp5RlRbtp4t1X8_28,
  Lp5RlRbtp4t1X8_30,
  Lp5RlRbtp4t1X8Max
} LPDDR5_RL_RBTP_4t1_Set1;

/**
  This list is for DVFSC disabled, x8, DBI Enabled
**/
typedef enum {
  Lp5RlRbtp4t1Set2_3,
  Lp5RlRbtp4t1Set2_4,
  Lp5RlRbtp4t1Set2_6,
  Lp5RlRbtp4t1Set2_7,
  Lp5RlRbtp4t1Set2_9,
  Lp5RlRbtp4t1Set2_10,
  Lp5RlRbtp4t1Set2_12,
  Lp5RlRbtp4t1Set2_14,
  Lp5RlRbtp4t1Set2_15,
  Lp5RlRbtp4t1Set2_17,
  Lp5RlRbtp4t1Set2_19,
  Lp5RlRbtp4t1Set2_20,
  Lp5RlRbtp4t1Set2_24,
  Lp5RlRbtp4t1Set2_26,
  Lp5RlRbtp4t1Set2_29,
  Lp5RlRbtp4t1Set2_33,
  Lp5RlRbtp4t1Set2Max
} LPDDR5_RL_RBTP_4t1_Set2;

/**
  This enumeration maps out the values for CBT Mode Enable
**/
typedef enum {
  Lp5CbtModeNormalMode,
  Lp5CbtModeEnFsp0,
  Lp5CbtModeEnFsp1,
  Lp5CbtModeEnFsp2,
  Lp5CbtModeMax
} LPDDR5_CBT_MODE_ENUM;

/**
  This enumeration maps out the values for FSP-WR
**/
typedef enum {
  Lp5FspWr0,
  Lp5FspWr1,
  Lp5FspWr2,
  Lp5FspWrMax
} LPDDR5_FSP_WR_ENUM;

typedef enum {
  Lp5CbtModePhaseRising,
  Lp5CbtModePhaseFalling,
  Lp5CbtModePhaseMax
} LPDDR5_CBT_MODE_PHASE;

/**
  LPDDR5x MR8 Type OP[1:0]
**/
typedef enum {
  Lp5Mr8TypeLp5x8533 = 1,
  Lp5Mr8TypeLp5x9600 = 2,
  Lp5Mr8TypeLp5x10667 = 3,
  Lp5Mr8TypeLp5xMax
} LPDDR5_MR8_TYPE;

/**
  LPDDR5 MR10 RDQS Postamble Length
**/
typedef enum {
  Lp5tRPST_0p5_tWck,  // 0.5 tWCK
  Lp5tRPST_2p5_tWck,  // 2.5 tWCK
  Lp5tRPST_4p5_tWck   // 4.5 tWCK
} LPDDR5_MR10_RDQS_PST_LENGTH;

/**
  LPDDR5 MR10 RD Preamble Length
**/
typedef enum {
  Lp5tRPRE_4S_0T_00000000,
  Lp5tRPRE_2S_2T_00001010,
  Lp5tRPRE_0S_4T_10101010,
  Lp5tRPRE_2S_4T_000010101010
} LPDDR5_MR10_tRPRE;

/// Structure Definitions
#pragma pack(push, 1)
/// Mode Register 0 is Read Only.
typedef union {
  struct {
    UINT8               : 1;  ///< Bits 0:0
    UINT8 LatencyMode   : 1;  ///< Bits 1:1
    UINT8               : 6;  ///< Bits 2:7
  } Bits;
  struct {
    UINT8 NTODT                  : 1;  ///< Bits 0:0
    UINT8 LatencyMode            : 1;  ///< Bits 1:1
    UINT8 EnhancedWCK            : 1;  ///< Bits 2:2
    UINT8 OptimizedRefreshMode   : 1;  ///< Bits 3:3
    UINT8 DMIBehaviorMode        : 1;  ///< Bits 4:4
    UINT8 UnifiedNTODT           : 1;  ///< Bits 5:5
    UINT8 PreEmphasis            : 1;  ///< Bits 6:6
    UINT8                        : 1;  ///< Bits 7:7
  } BitsLp5x;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_0_TYPE;

typedef union {
  struct {
    UINT8 CsOdtOpSupport    : 1;  ///< Bits 0:0
    UINT8 ArfmSupport       : 1;  ///< Bits 1:1 - Adaptive Refresh Management Support
    UINT8 DrfmSupport       : 1;  ///< Bits 2:2 - Directed Refresh Management Support
    UINT8 CkMode            : 1;  ///< Bits 3:3
    UINT8 WriteLatency      : 4;  ///< Bits 4:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_1_TYPE;

typedef union {
  struct {
    UINT8 RlAndRbtp       : 4;  ///< Bits 0:3
    UINT8 WriteRecovery   : 4;  ///< Bits 4:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_2_TYPE;

typedef union {
  struct {
    UINT8 PullDnDrvStr    : 3;  ///< Bits 0:2
    UINT8 BankBgOrg       : 2;  ///< Bits 3:4
    UINT8 WriteLatencySet : 1;  ///< Bits 5:5
    UINT8 DbiRdEn         : 1;  ///< Bits 6:6
    UINT8 DbiWrEn         : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_3_TYPE;

/// Mode Register 4 is Read Only
typedef union {
  struct {
    UINT8 RefreshRate     : 5;  ///< Bits 0:4
    UINT8 ZqUpdateFlag    : 1;  ///< Bits 5:5
    UINT8 ZqMaster        : 1;  ///< Bits 6:6
    UINT8 TempUpdateFlag  : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_4_TYPE;

/// Mode Register 5 is Read Only
typedef union {
  struct {
    UINT8 Manufacturer    : 8;  ///< Bits 0:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_5_TYPE;

/// Mode Register 8 is Read Only
typedef union {
  struct {
    UINT8 Type    : 2;  ///< Bits 0:1
    UINT8 Density : 4;  ///< Bits 2:5
    UINT8 IoWidth : 2;  ///< Bits 6:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_8_TYPE;

typedef union {
  struct {
    UINT8 RdqsPstMode : 1;  ///< Bits 0:0
    UINT8             : 1;  ///< Bits 1:1
    UINT8 WckPstLen   : 2;  ///< Bits 2:3
    UINT8 RdqsPreLen  : 2;  ///< Bits 4:5
    UINT8 RdqsPstLen  : 2;  ///< Bits 6:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_10_TYPE;

typedef union {
  struct {
    UINT8 DqOdt   : 3;  ///< Bits 0:2
    UINT8 NtOdtEn : 1;  ///< Bits 3:3
    UINT8 CaOdt   : 3;  ///< Bits 4:6
    UINT8 CsOdtOP : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_11_TYPE;

/// Mode Register 12 is Read/Write
typedef union {
  struct {
    UINT8 CaVref      : 7;  ///< Bits 0:6
    UINT8 VrefByteSel : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_12_TYPE;

typedef union {
  struct {
    UINT8 ThermalOffset : 2;  ///< Bits 0:1
    UINT8 VroMode       : 1;  ///< Bits 2:2 - Vref Output Mode
    UINT8               : 2;  ///< Bits 3:4
    UINT8 DmDisable     : 1;  ///< Bits 5:5 - Data Mask Disable
    UINT8 CbtMode       : 1;  ///< Bits 6:6
    UINT8 DualVdd2      : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_13_TYPE;

/// Mode Register is Read/Write
typedef union {
  struct {
    UINT8 DqVref  : 7;  ///< Bits 0:6
    UINT8 Vdlc    : 1;  ///< Bits 7:7 - Vref DQ Lower Byte Control
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_14_TYPE;

/// Mode Register is Read/Write
typedef union {
  struct {
    UINT8 DqVref            : 7;  ///< Bits 0:6
    UINT8 OffsetCalibration : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_15_TYPE;

/// Mode Register is Read/Write
typedef union {
  struct {
    UINT8 FspWr     : 2;  ///< Bits 0:1
    UINT8 FspOp     : 2;  ///< Bits 2:3
    UINT8 CbtEn     : 2;  ///< Bits 4:5
    UINT8 Vrcg      : 1;  ///< Bits 6:6
    UINT8 CbtPhase  : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_16_TYPE;

typedef union {
  struct {
    UINT8 SocOdt        : 3;  ///< Bits 0:2
    UINT8 CkOdtDis      : 1;  ///< Bits 3:3
    UINT8 CsOdtDis      : 1;  ///< Bits 4:4
    UINT8 CaOdtDis      : 1;  ///< Bits 5:5
    UINT8 x8OdtDis7to0  : 1;  ///< Bits 6:6 - x8_2ch Byte mode
    UINT8 x8OdtDis15to8 : 1;  ///< Bits 7:7 - x8_2ch Byte mode
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_17_TYPE;

typedef union {
  struct {
    UINT8 WckOdt        : 3;  ///< Bits 0:2
    UINT8 WckFreqMode   : 1;  ///< Bits 3:3
    UINT8 WckAlwaysOn   : 1;  ///< Bits 4:4
    UINT8 WckSync       : 1;  ///< Bits 5:5
    UINT8 Wck2CkLevel   : 1;  ///< Bits 6:6
    UINT8 WckToCkRatio  : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_18_TYPE;

typedef union {
  struct {
    UINT8 DVFSC        : 2;  ///< Bits 0:1 - VDD2 Dynamic Voltage and Frequency Scaling Core
    UINT8 DVFSQ        : 2;  ///< Bits 2:3 - VDDQ Dynamic Voltage and Frequency Scaling VDDQ
    UINT8 WCK2DQOSC    : 1;  ///< Bits 4:4
    UINT8 WCK2DQOSCSUP : 1;  ///< Bits 5:5
    UINT8 CsOdt        : 2;  ///< Bits 6:7 - CS ODT termination (LP5x Only)
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_19_TYPE;

typedef union {
  struct {
    UINT8 RdqsMode    : 2;  ///< Bits 0:1
    UINT8 WckMode     : 2;  ///< Bits 2:3
    UINT8             : 2;  ///< Bits 4:5
    UINT8 RdcDmiMode  : 1;  ///< Bits 6:6
    UINT8 RdcDqMode   : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_20_TYPE;

typedef union {
  struct {
    UINT8 Wdcfs : 1;  ///< Bits 0:0 - Read Only
    UINT8 Rdcfs : 1;  ///< Bits 1:1 - Read Only
    UINT8 Wxfs  : 1;  ///< Bits 2:2 - Read Only
    UINT8       : 1;  ///< Bits 3:3
    UINT8 Wdcfe : 1;  ///< Bits 4:4
    UINT8 Rdcfe : 1;  ///< Bits 5:5
    UINT8 Wxfe  : 1;  ///< Bits 6:6
    UINT8       : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_21_TYPE;

typedef union {
  struct {
    UINT8 Dfeql : 3;  ///< Bits 2:0 - DFE Quantity for Lower Byte
    UINT8 Rdcas : 1;  ///< Bits 3:3 - Read DCA support
    UINT8 Dfequ : 3;  ///< Bits 6:4 - DFE Quantity for Upper Byte
    UINT8 Dfes  : 1;  ///< Bits 7:7 - DFE support
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_24_TYPE;

typedef union {
  struct {
    UINT8                   : 4;  ///< Bits 0:3
    UINT8 CkPairOdt         : 1;  ///< Bits 4:4
    UINT8 CaInputOdt        : 1;  ///< Bits 5:5
    UINT8 Parc              : 1;  ///< Bits 6:6
    UINT8 OptimizedRefresh  : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_25_TYPE;

typedef union {
  struct {
    UINT8 DCMStartStop   : 1;  ///< Bits 0:0
    UINT8 DCMFlip        : 1;  ///< Bits 1:1
    UINT8 DCML0          : 1;  ///< Bits 2:2
    UINT8 DCML1          : 1;  ///< Bits 3:3
    UINT8 DCMU0          : 1;  ///< Bits 4:4
    UINT8 DCMU1          : 1;  ///< Bits 5:5
    UINT8 RDQSTFS        : 1;  ///< Bits 6:6
    UINT8 RDQSTFE        : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_26_TYPE;

typedef union {
  struct {
    UINT8 RFM         : 1;  ///< Bits 0:0
    UINT8 RAAIMT      : 5;  ///< Bits 1:5
    UINT8 RAAMULT     : 2;  ///< Bits 6:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_27_TYPE;

typedef union {
  struct {
    UINT8 ZqReset     : 1;  ///< Bits 0:0
    UINT8 ZqStop      : 1;  ///< Bits 1:1
    UINT8 ZqInterval  : 2;  ///< Bits 2:3
    UINT8             : 1;  ///< Bits 4:4
    UINT8 ZqMode      : 1;  ///< Bits 5:5
    UINT8             : 2;  ///< Bits 6:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_28_TYPE;

typedef union {
  struct {
    UINT8 PPRBank0      : 1;  ///< Bits 0:0
    UINT8 PPRBank1      : 1;  ///< Bits 1:1
    UINT8 PPRBank2      : 1;  ///< Bits 2:2
    UINT8 PPRBank3      : 1;  ///< Bits 3:3
    UINT8 PPRBank4      : 1;  ///< Bits 4:4
    UINT8 PPRBank5      : 1;  ///< Bits 5:5
    UINT8 PPRBank6      : 1;  ///< Bits 6:6
    UINT8 PPRBank7      : 1;  ///< Bits 7:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_29_TYPE;

typedef union {
  struct {
    UINT8 DcaLowByte    : 4;  ///< Bits 0:3
    UINT8 DcaUpperByte  : 4;  ///< Bits 4:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_30_TYPE;

typedef union {
  struct {
    UINT8 Wck2DqiInterval : 8;  ///< Bits 0:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_37_TYPE;

typedef union {
  struct {
    UINT8 Wck2DqoInterval : 8;  ///< Bits 0:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_40_TYPE;

typedef union {
  struct {
    UINT8 Pdfec             : 1;  ///< Bits 0:0 - Per-pin DFE Control
    UINT8 DvfscSupport      : 2;  ///< Bits 1:2 - DVFSC/E-DVFSC Support
    UINT8 EdvfscOdtSupport  : 1;  ///< Bits 3:3 - E-DVFSC ODT Option Support
    UINT8 Ppre              : 1;  ///< Bits 4:4 - PPR Enable / Disable
    UINT8 NtDqOdt           : 3;  ///< Bits 5:7 - NT DDQ ODT
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_41_TYPE;

typedef union {
  struct {
    UINT8 EnhRdqsTrainingMode : 1;  ///< Bits 0:0
    UINT8 RdqsToggleMode      : 1;  ///< Bits 1:1
    UINT8 FifoRdqsTrainMode   : 1;  ///< Bits 2:2
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_46_TYPE;

typedef union {
  struct {
    UINT8 RAADEC    : 2;  ///< Bits 0:1
    UINT8 RFMSB     : 2;  ///< Bits 2:3
    UINT8 RFMSBC    : 2;  ///< Bits 4:5
    UINT8 ARFMLevel : 2;  ///< Bits 6:7 - Adaptive Refresh Management Level
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_57_TYPE;

typedef union {
  struct {
    UINT8 PUEmphasisLower : 2;  ///< Bits 0:1
    UINT8 PDEmphasisLower : 2;  ///< Bits 2:3
    UINT8 PUEmphasisUpper : 2;  ///< Bits 4:5
    UINT8 PDEmphasisUpper : 2;  ///< Bits 6:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_58_TYPE;

typedef union {
  struct {
    UINT8 ReadDcaLowByte : 4;  ///< Bits 0:3
    UINT8 ReadDcaUpperByte : 4;  ///< Bits 4:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_69_TYPE;

typedef union {
  struct {
    UINT8 BrcSupported              : 1;  ///< Bits 0:0 - Blast Radius Config (BRC) Supported
    UINT8                           : 1;  ///< Bits 1:1
    UINT8 DrfmEnable                : 1;  ///< Bits 2:2 - Directed Refresh Management Enable
    UINT8 DrfmAutoPrechargeSampling : 1;  ///< Bits 3:3
    UINT8 BlastRadiusConfig         : 2;  ///< Bits 4:5 - BRC == (victim_rows - 2)
    UINT8                           : 2;  ///< Bits 6:7
  } Bits;
  UINT8 Data8;
} LPDDR5_MODE_REGISTER_75_TYPE;

#pragma pack(pop)
#endif // _MRC_LPDDR5_REGISTERS_H_

