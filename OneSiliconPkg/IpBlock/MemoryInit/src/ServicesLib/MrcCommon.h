/** @file
  This file include all the MRC common data.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _MrcCommon_h_
#define _MrcCommon_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "CMcAddress.h"
#include "MrcDebugPrint.h"
#include "MrcDdrIoDefines.h"

/// Defines and types.

/// Convert rank to dimm number
#ifndef RANK_TO_DIMM_NUMBER
#define RANK_TO_DIMM_NUMBER(Rank) ((Rank) / MAX_RANK_IN_DIMM)
#endif

/// Convert DIMM number to a rank bitmask, assuming 2 ranks per DIMM
#ifndef DIMM_TO_RANK_MASK
#define DIMM_TO_RANK_MASK(Dimm) (3 << (2 * (Dimm)))
#endif

/// Convert DIMM number to a rank bitmask, with either 1 or 2 ranks per DIMM
#ifndef DIMM_TO_VARIABLE_RANK_MASK
#define DIMM_TO_VARIABLE_RANK_MASK(Dimm, RankMask) ((RankMask) << (2 * (Dimm)))
#endif

/// Bit operation commands
#ifndef MRC_MASK
#define MRC_MASK(offset, width) (((1 << width) - 1) << (offset))
#endif

#ifndef MRC_MASK_NOT
#define MRC_MASK_NOT(offset, width) (~(MRC_MASK (offset, width)))
#endif

#ifndef MRC_CR_UPDATE
#define MRC_CR_UPDATE(Register, Offset, Width, Value) \
        (((Register) & MRC_MASK_NOT (Offset, Width)) | ((Value) << (Offset)))
#endif

#ifndef MRC_CR_DUMP
#define MRC_CR_DUMP(Register, offset, Width)\
        (((Register) & (MRC_MASK(offset, Width))) >> offset)
#endif

/// Self Refresh State Defines
#define MRC_SR_ENTER  (1)
#define MRC_SR_EXIT   (0)

/// Early Read/Write Time Centering 2D Traffic Modes
#define ERWTC2D_NORMAL (0)
#define ERTC2D_MPR    (1)

/// Cache line size in chunks
#define CACHE_LINE_SIZE (8)

/// Number of cache lines in WDB
#define WDB_NUM_CACHE_LINES (64)

/// tCAENT = 250ns
/// tCAENT is same for LP5
#define MRC_LP_tCAENT_NS (250)

#define OFFSET_CALC4(a, b, W, c, X, d, Y, e, Z) ((a) + (((b) - (a)) * (W)) + (((c) - (a)) * (X)) + (((d) - (a)) * (Y)) + (((e) - (a)) * (Z)))
#define OFFSET_CALC3(a, b, X, c, Y, d, Z)       ((a) + (((b) - (a)) * (X)) + (((c) - (a)) * (Y)) + (((d) - (a)) * (Z)))
#define OFFSET_CALC_MC_CH(MC0, MC1, MC, CH1, CH) (((MC0) + (((MC1) - (MC0)) * (MC)) + (((CH1) - (MC0)) * (CH))))
#define OFFSET_CALC_CH(CH0, CH1, CH) (((CH0) + (((CH1) - (CH0)) * (CH))))
#define OFFSET_CALC_INDEX(INDEX0, INDEX1, INDEX) OFFSET_CALC_CH(INDEX0, INDEX1, INDEX)

#define INC_OFFSET_CALC_MC_CH(MC0, MC1, MC, CH1, CH) (((((MC1) - (MC0)) * (MC)) + (((CH1) - (MC0)) * (CH))))
#define INC_OFFSET_CALC_CH(CH0, CH1, CH) (((((CH1) - (CH0)) * (CH))))
#define INC_OFFSET_CALC_INDEX(INDEX0, INDEX1, INDEX) INC_OFFSET_CALC_CH(INDEX0, INDEX1, INDEX)

#define IS_MC_SUB_CH(IsLpddr, Channel) ((IsLpddr) && ((Channel) % 2))
#define LP_IP_CH(IsLpddr, Channel) ((IsLpddr) ? (Channel) / 2 : (Channel))
#define DDR5_IP_CH(IsDdr5, Channel) ((IsDdr5) ? (Channel) * 2 : (Channel))

// Margins are multiplied by this when returned from the margining functions
#define MARGIN_MULTIPLIER (10)

// Margins are multiplied by this when returned from the margining functions
#define MARGIN_MULTIPLIER (10)

// RcvEnaX margin param step size in MRC
#define RCVENAX_STEPSIZE (4)
#define DEFAULT_STEPSIZE (1)

#define RDVC2D_BIT_MARGIN_ADJUST 16

#define MRC_1D_ERROR_LEN        ((206 * 2) + 1)

#define MAX_BITS_TRAINFEEDBACK  (MAX_BITS + 2)    ///< DQ[7:0], [8]: DBI, 9: DQS

// Data train feedback mask
#define FEEDBACKMASK            (0xFF)
#define FEEDBACKMASK_ECC_UPPER  (0xF0)
#define FEEDBACKMASK_ECC        (0x0F)

// Basic Test Engine DQ pattern (10100101)
#define BASIC_STATIC_PATTERN (0xA5)

// Configuration defines for MrcCaParityTeConfig
#define MRC_CA_PARITY_TEST_START_ASSERT_CYCLES  (17)
#define MRC_CA_PARITY_ASSERT_CYCLES_DDR5        (1)
#define MRC_CA_PARITY_INVERSE_CYCLES_DDR5       (63)
#define MRC_CA_PARITY_LMN_PERIOD_DDR5           (MRC_CA_PARITY_ASSERT_CYCLES_DDR5 + MRC_CA_PARITY_INVERSE_CYCLES_DDR5)
#define MRC_CA_PARITY_ASSERT_CYCLES_LPDDR5      (2)
#define MRC_CA_PARITY_INVERSE_CYCLES_LPDDR5     (47)
#define MRC_CA_PARITY_START_DELAY_DDR5_LP5G2    (1)

#define MRC_SQRT_DEC_PT_DIV (100)

#define MAX_OPT_POINTS_EARLY_LINEAR_CENTERING (255)

// Program data rotation Defines
#define ENABLE_DQ_LANE_MASK 0xFFFFFFFFFFFFFFFFULL
#define DISABLE_DQ_LANE_MASK 0
#define ENABLE_ECC_LANE_MASK 0xFF
#define DISABLE_ECC_LANE_MASK 0
#define BASIC_NON_MPR_VA_VICTIM_16     0
#define BASIC_NON_MPR_VA_AGGRESSOR_16  0xFFFF
#define MRC_NUM_MUX_SEEDS   (3)

typedef enum {
  NtDimmR0,
  NtDimmR1,
  TargetDimmNtRank,
  MaxNtOdt,
} NtOdt;

typedef enum {
  dDIMM0 = 0,
  dDIMM1
} MrcDimmType;

typedef enum {
  ctChannel0 = 0,
  ctChannel1,
  ctChannel2,
  ctChannel3
} MrcChannelType;

typedef enum {
  cCONTROLLER0 = 0,
  cCONTROLLER1
} MrcControllerType;

typedef enum {
  rRank0    = 0,
  rRank1,
  rRank2,
  rRank3
} MrcRank;

typedef enum {
  ssOne     = 0,
  ssTwo,
  ssThree,
  ssFour
} TSubSequencesNumber;

/// Define ECC mode.
typedef enum {
  emNoEcc,
  emEccIoActive,
  emEccLogicActive,
  emBothActive
} TEccModes;

/// Raw card list
typedef enum {
  rcA,
  rcB,
  rcC,
  rcD,
  rcE,
  rcF,
  rcG,
  rcH,
} TRawCard;

/// Reut Addressing Parameters
typedef enum {
  MrcReutFieldRank,
  MrcReutFieldBank,
  MrcReutFieldRow,
  MrcReutFieldCol,
  MrcReutFieldMax   ///< This must be the last entry in the enum.
} MrcReutField;

typedef enum {
  MrcPatSrcStatic,                ///< Pattern Buffer
  MrcPatSrcDynamic,               ///< LFSR
  MrcPatSrcAllZeroes,             ///< DC zero
  MrcPatSourceMax
} MRC_PAT_SRC_TYPE;

typedef struct {
  UINT32 Start;
  UINT32 End;
} MRC_RANGE;

typedef struct {
  UINT8 Channel;
  UINT8 Rank;
} MRC_CKD_GROUP;
typedef enum {
  BasicVA = 0,                    ///< Use 2 LFSR VicAggressor pattern with rotation of 10 bits
  CADB,                           ///< Do CADB on command/address bus and LMN VA on DQ (power supply noise)
  TurnAround,
  LMNVa,                          ///< Use (LMN aggressor + LFSR Victim) with rotation of 10 bits
  RdRdTA,                         ///< Run 2 tests, one with Trdrd=4 and one with Trdrd=5
  RdRdTA_All,                     ///< Run 8 tests, Covering tRDRD_sr 4,5,6,7 and tRDRD_dr = Min,+1,+2,+3
  StaticPattern,                  ///< We are running a test that uses a static pattern.
  GaloisMprVa,                    ///< Use 2 LFSR VicAgressor pattern using Galois
  CaParity,                       ///< IO Test using CA Mode of the DRAM receiving feedback on DQ.
} MrcDqPat;

typedef enum {
  NSOE = 0,                       ///< Never Stop On Any Error
  NTHSOE,                         ///< Stop on the Nth Any Lane Error
  ABGSOE,                         ///< Stop on All Byte Groups Error
  ALSOE                           ///< Stop on All Lanes Error
} MRC_TEST_STOP_TYPE;

typedef enum {
  MrcGearAuto = 0,
  MrcGear1 = 1,
  MrcGear2 = 2,
  MrcGearMax
}MRC_GEAR_TYPE;

typedef enum {
  MrcNoPendingSmbusRequest = 0,
  MrcSmbusWriteRequest,
  MrcSmbusReadRequest
} MRC_SMBUS_REQUEST_TYPE;

typedef enum {
  MrcSmbusRequestStatusPass = 0,
  MrcSmbusRequestStatusFail = 1
} MRC_SMBUS_REQUEST_STATUS;

typedef enum {
  GetTimingRead = 0,
  GetTimingWrite
} TIMING_GET_TYPE;

/// REUT CmdPattern
#define PatWrRd                             0
#define PatWr                               1
#define PatRd                               2
#define PatRdWrTA                           3
#define PatWrRdTA                           4
#define PatWrEndless                        5
#define PatRdEndless                        6
#define PatWrRdEndless                      7
#define PatWrScrub                          8
#define PatRdWr                             13
#define PatWrUp                             14     // MemTest, MEMTEL ^(wD)
#define PatWrDown                           15     // MemTest, MEMTEL v(wD)
#define PatWrUpInvt                         16     // MemTest, MEMTEL ^(wI)
#define PatWrDownInvt                       17     // MemTest, MEMTEL v(wI)
#define PatRdUp                             18     // MemTest, MEMTEL ^(rD)
#define PatRdDown                           19     // MemTest, MEMTEL v(rD)
#define PatRdUpInvt                         20     // MemTest, MEMTEL ^(rI)
#define PatRdDownInvt                       21     // MemTest, MEMTEL v(rI)
#define PatRdWrUp                           22     // MemTest, MEMTEL ^(rD,wI)
#define PatRdWrDown                         23     // MemTest, MEMTEL v(rD,wI)
#define PatRdWrUpInvt                       24     // MemTest, MEMTEL ^(rI,wD)
#define PatRdWrDownInvt                     25     // MemTest, MEMTEL v(rI,wD)
#define PatRdWrUpAlt                        26     // MemTest, MEMTEL ^(rD,wD)
#define PatRdWrDownAlt                      27     // MemTest, MEMTEL v(rD,wD)
#define PatRdWrUpInvtAlt                    28     // MemTest, MEMTEL ^(rI,wI)
#define PatRdWrDownInvtAlt                  29     // MemTest, MEMTEL v(rI,wI)

/// CPGC LFSR victim / aggressor masks
#define BASIC_VA_VICTIM_16     0x0101
#define BASIC_VA_AGGRESSOR_16  0xFEFE

/// Rx/Tx Bit Center Range
#define BitCenterMin    0
#define BitCenterMax    63

/// Struct definitions
typedef struct {
  INT64 CtlGrpPi[MAX_CONTROLLER][MAX_CHANNEL][MRC_CTL_GRP_PER_CH_MAX];
  INT64 CmdGrpPi[MAX_CONTROLLER][MAX_CHANNEL][MAX_COMMAND_GROUPS];
  INT64 CsPerPin[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL];
} MRC_CMD_CTL_MARGIN_SR;

/// Margin params
/**
 1D Margin Types:
  RcvEn:         Shifts just RcvEn.  Only side effect is it may eat into read dq-dqs for first bit of burst.
  RdT:           Shifts read DQS timing, changing where DQ is sampled.
  WrT:           Shifts write DQ timing, margining DQ-DQS timing.
  WrDqsT:        Shifts write DQS timing, margining both DQ-DQS and DQS-CLK timing.
  RdV:           Shifts read Vref voltage for DQ only.
  WrV:           Shifts write Vref voltage for DQ only.
  WrLevel:       Shifts write DQ and DQS timing, margining only DQS-CLK timing.
  RdVBit:        Shifts read DQ per bit voltage.
  RcvEnaX:       Shifts RcvEn in multiples of 4PI.
  WrTUnMatched:  Shifts Write DQ as an offset from Write DQS+tDQS2DQ / DQS + tWCK2CK
  CmdTBit:       Shifts Command per CA bit timing.

  param = {0:RcvEn, 1:RdT, 2:WrT, 3: WrDqsT, 4:RdV, 5:WrV, 6:WrLevel,
           7:RdVBit, 8:RdVBit1, 9:RcvEnaX, 10:WrTUnMatched, 11:CmdT, 12:CmdV,
           13:CtlV, 14:RdTN, 15:RdTP, 16:CmdTBit, 17:RdTDbi, 18:WrTDbi,
           19:RdVDbi, 20:WrTBit, 21:RdTBit}
**/
typedef enum {
  RcvEna = 0,
  RdT,
  WrT,
  WrDqsT,
  RdV,
  WrV,
  WrLevel,
  RdVBit,
  RdVBit1,
  RcvEnaX,
  WrTUnMatched, // 10
  CmdT,
  CmdV,
  CtlV,
  RdTN,
  RdTP,
  CmdTBit,
  RdTDbi,
  WrTDbi,
  RdVDbi,
  WrTBit,
  RdTBit,
  MarginTypeMax
} MrcMarginTypes;

/*
 * TOptParamOffset:
 *  MrcOptParamMappingTable's lines keep the
 *  same sequence like TOptParamOffset's member
 *  definition sequence.
 *  When adding or deleting an OptParam,
 *  MrcOptParamMappingTable must be updated too.
 */
typedef enum {
  OptWrDS = 0,
  OptVddq,
  OptTxDqTco,
  OptTxEq,
  OptRdDqOdt,
  OptRdDqsOdt,
  OptRxEq,
  OptDimmOdtWr,
  OptDimmOdtNom,
  OptDimmOdtNomWr,
  OptDimmOdtNomRd,
  OptDimmOdtPark,
  OptDimmOdtParkDqs,
  OptDimmOdtCA,
  OptDimmOdtCaStrap,
  OptDimmOdtCccGroupA,
  OptDimmOdtCAGroupA,
  OptDimmOdtCSGroupA,
  OptDimmOdtCccGroupB,
  OptDimmOdtCAGroupB,
  OptDimmOdtCSGroupB,
  OptDimmRon,
  OptDimmRonUp,
  OptDimmRonDn,
  OptDimmNTOdt,
  OptLpddr5DimmDFETapBoth,
  OptLpddr5DimmDFETapLower,
  OptLpddr5DimmDFETapUpper,
  OptDdr5DimmDFETap1,
  OptDdr5DimmDFETap2,
  OptDdr5DimmDFETap3,
  OptDdr5DimmDFETap4,
  OptCCCDS,
  OptCmdDS,
  OptCtlDS,
  OptDimmSocOdt,
  OptCCCTxEq,
  OptCCCSComp,
  OptCmdSComp,
  OptCtlSComp,
  OptRxVrefVddqDecap,
  OptDFETap0,
  OptDFETap1,
  OptDFETap2,
  OptDFETap3,
  OptRxCap,
  OptRxRes,
  OptClkTco,
  OptWckTco,
  OptDccDdr5Clk,
  OptDccDdr5Dqs,
  OptDccLpddr5Wck,
  OptDccDataDqTargetR0,
  OptDccDataDqTargetR1,
  OptLp5WckDcaUpper,
  OptLp5WckDcaLower,
  OptLp5WckDcaBoth,
  OptDdr5RxXTalk,
  OptDqOdtCompOffset,
  OptDqsOdtCompOffset,
  OptDqDrvCompOffset,
  OptCmdRCompDrvOffset,
  OptCmdSCompOffset,
  OptCtlRCompDrvOffset,
  OptCtlSCompOffset,
  OptLp5ReadDcaUpper,
  OptLp5ReadDcaLower,
  OptLp5ReadDcaBoth,
  OptLpddr5EmphasisUpBoth,
  OptLpddr5EmphasisDnBoth,
  OptLpddr5EmphasisUpLB,
  OptLpddr5EmphasisDnLB,
  OptLpddr5EmphasisUpUB,
  OptLpddr5EmphasisDnUB,
  OptDefault,
  // Following enums are not used by Power Training, hence they must be at the end of this enum and are not used in MrcOptParamMappingTable
  Ddr5OdtlOnWr,
  Ddr5OdtlOffWr,
  Ddr5OdtlOnWrNT,
  Ddr5OdtlOffWrNT,
  Ddr5OdtlOnRdNT,
  Ddr5OdtlOffRdNT,
  OptDimmNTOdtEn,
  OptDimmDcaQclk,
  OptDimmDcaIBclk,
  OptDimmDcaQBclk,
  OptMax
} TOptParamOffset;

typedef enum {
  drrd2rd = 0,
  ddrd2rd,
  drwr2wr,
  ddwr2wr,
  drrd2wr,
  ddrd2wr,
  drwr2rd,
  ddwr2rd,
  rdodtd,
  wrodtd,
  mcodts,
  mcodtd,
  rtl
} TOptParamTAT;

typedef enum {
  WriteOdt,
  ReadOdt,
  MaxOdtParamType
} OdtParamType;

#define RXF_SELECT_RC_100  (6)
#define RXF_SELECT_RC_133  (4)
#define RXF_SELECT_MIN     (0)
#define RXF_SELECT_MAX     (4)
#define RXF_SELECT_MAX_ULT (2)

/// Struct Definitions
#pragma pack (push, 1)
typedef union {
  struct {
    UINT32 Enable_2x_Refresh                       :  1;  // Bits 0:0
    UINT32 LPDDR_Min_MR4                           :  3;  // Bits 1:3
    UINT32                                         :  27; // Bits 4:30
    UINT32 Lock_Bit                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MrcMailbox2xRefresh;

typedef struct {
  UINT16 Start[MrcReutFieldMax];   ///< (4, uint16)    // Rank, Bank, Row, Col
  UINT16 Stop[MrcReutFieldMax];    ///< (4, uint16)    // Rank, Bank, Row, Col
  UINT8  Order[MrcReutFieldMax];   ///< [4, uint8)     // Rank, Bank, Row, Col
  UINT32 IncRate[MrcReutFieldMax]; ///< (4, unit32)    // Rank, Bank, Row, Col
  UINT16 IncVal[MrcReutFieldMax];  ///< (4, unit16)    // Rank, Bank, Row, Col
} MRC_REUTAddress;

/// Address Increment Order
typedef enum {
  ROW_COL_2_BANK_2_RANK,
  BANK_2_ROW_COL_2_RANK,
  BANK_2_COL_ROW_2_RANK,
  ROW_COL_2_RANK_2_BANK,
  RANK_2_ROW_COL_2_BANK,
  BANK_2_RANK_2_ROW_COL,
  RANK_2_BANK_2_ROW_COL
} MRC_ADDRESS_INCREMENT_ORDER;

/// Address Direction
typedef enum {
  FAST_Y,
  DIAGONAL,
  FAST_X,
  DIAGONAL2,
  INVERSE_FAST_Y,
  INVERSE_DIAGONAL,
  INVERSE_FAST_X,
  INVERSE_DIAGONAL2
} MRC_ADDRESS_DIRECTION;

typedef struct {
  MRC_ADDRESS_INCREMENT_ORDER  AddrIncOrder;
  MRC_ADDRESS_DIRECTION        AddrDirection;
  UINT8                        LastValidAddr; // Number > Max Address Instructions means no Last set
  UINT32                       RowStart;
  UINT16                       ColStart;
  UINT8                        RowSizeBits;
  UINT8                        ColSizeBits;
  UINT8                        BankSize;
} MRC_ADDRESS;

typedef struct {
  UINT32 ChunkMask;   // Which chunk (UI) to check
  UINT64 DataMask;    // Which bit to check
  UINT8  EccMask;     // Which ECC bit to check
} MRC_ERROR_MASKS;

typedef enum {
  ByteGroupErrStatus = 0,
  ChunkErrStatus,
  RankErrStatus,
  NthErrStatus,
  NthErrOverflow,
  AlertErrStatus,
  WdbRdChunkNumStatus,
  BitGroupErrStatus,
  EccLaneErrStatus,
  ChannelErrStatus,
  ErrStatusTypeMax
} MRC_ERR_STATUS_TYPE;

typedef enum {
  ErrCounterCtlPerLane = 0,    ///< Indicates Counter Status will count errors for a particular lane
  ErrCounterCtlPerByte,        ///< Indicates Counter Status will count errors for a particular byte group
  ErrCounterCtlPerNibble,      ///< Indicates Counter Status will count errors for a particular nibble
  ErrCounterCtlPerUI,          ///< Indicates Counter Status will count errors for a particular UI
  ErrCounterCtlAllLanes,       ///< Indicates Counter Status will count errors for all lanes
  RasterRepoStatus,            ///< Indicates raster repo status. This field is used to read raster repo ecc content registers which stores
                               ///< even and odd UI errors for all lanes
  ErrCounterCtlMax
} MRC_ERR_COUNTER_CTL_TYPE;

typedef struct {
  UINT8                    CounterPointer;  // Specifies in register which counter to setup. Each Channel has MAX_ERR_COUNTERS_IN_CHANNEL counters.
  MRC_ERR_COUNTER_CTL_TYPE Type;            // Specifies which type of error counter read will be executed.
} MRC_ERROR_COUNTERS;

typedef struct {
  MRC_PAT_SRC_TYPE PatSource;       ///< Defines the source of patterns: Static, Dynamic, All Zeroes (DC)
  UINT8  StaticPattern;          /// <Defined static pattern value
  UINT16 IncScale;
  UINT16 IncRate;                   ///< How quickly the pattern changes: WDB walks through cachelines, or PG rotates.  This is a linear count.
  UINT8  Start;                     ///< Starting pointer
  UINT8  Stop;                      ///< Stopping pointer
  UINT8  DQPat;                     ///< The Pattern type for the test.  See MrcDqPat.
  BOOLEAN EnableXor;                ///< Enables Xor'ing of pattern sources if available.
} MRC_PATTERN_CTL;

typedef struct {
  UINT8 BankGroup;
  UINT8 Bank;
} MRC_BG_BANK_PAIR; ///< Logical struct of BankGroup and Banks.

typedef struct {
  UINT8 TimingType;
  INT8  TimingMargin;
  UINT8 VoltageType;
  INT8  VoltageMargin;
} MarginCheckPoint;

#pragma pack (pop)

/// Structure to store Bit sweep results while we do linear search
typedef struct {
  INT16             CurrentPassingStartBit[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
  INT16             CurrentPassingEndBit[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
  INT16             LargestPassingStartBit[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
  INT16             LargestPassingEndBit[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
  INT16             CenterBit[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
} SweepResultsBit;

/// Structure to store Byte sweep results while we do linear search
typedef struct {
  INT16             LargestPassingStartByte[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  INT16             LargestPassingEndByte[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  INT16             CenterByte[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
} SweepResultsByte;

/// Structure to store Channel sweep resuls while we do linear search
typedef struct {
  INT16             LargestPassingStartChannel[MAX_CONTROLLER][MAX_CHANNEL];
  INT16             LargestPassingEndChannel[MAX_CONTROLLER][MAX_CHANNEL];
  INT16             CenterChannel[MAX_CONTROLLER][MAX_CHANNEL];
} SweepResultsChannel;

/// Structure to store the final Dqs and Dq Offsets
typedef struct {
  INT64             DqsNByteOffset[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  INT64             DqsPByteOffset[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  INT64             BitOffset[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
  INT64             BitDeskewOffset[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
} FinalRTCOffsets;

/// Structure to store the final Dqs and Dq Offsets
typedef struct {
  INT64             ByteDelay[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  INT64             BitDelay[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
} FinalWTCOffsets;



/// Structure to store which Bit has the worst centering margin
typedef struct {
  INT16             Width[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT8             BitLocation[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
} Worst1DPerBitMargin;


typedef enum {
  ChScope,
  ChRankScope,
  ByteScope,
  BitScope,
  BitScopeDqs
} LinearSweepScope;

#define NO_PRINT            0
#define PRINT_GENERAL       MRC_BIT0
#define PRINT_EYECHART      MRC_BIT1
#define PRINT_MARGINS       MRC_BIT2
#define PRINT_FINAL_VALUES  MRC_BIT3
#define PRINT_ALL           0xFF

#if MRC_POWER_TRAINING_DEBUG == SUPPORT
  #define POWER_TRAINING_1D_CENTERING_MASK PRINT_ALL
#else
  #define POWER_TRAINING_1D_CENTERING_MASK NO_PRINT
#endif

#define PRINT_CHECK(x) (x ? MSG_LEVEL_NOTE : MSG_LEVEL_NONE)

typedef union {
  struct {
    UINT8 PrintGeneral : 1;
    UINT8 PrintEyeChart : 1;
    UINT8 PrintMargins : 1;
    UINT8 PrintFinalValues : 1;
  } Bits;
  UINT8 Data;
} LINEAR_1D_SWEEP_PRINT_LEVEL;

typedef enum {
  BitMarginLevel = 0,
  ByteMarginLevel,
  ChannelMarginLevel,
} MRC_MARGIN_LEVEL;

// Save Data structure for DDR5 CS\CA training IO init
typedef struct {
  INT64 BiasPMCtrl;
  INT64 DefDrvEnLow;
  INT64 ForceRxOnAmpOrDqs;
  INT64 ForceRxOnDqsMux;
  INT64 ForceRxOnDq;
  INT64 InternalClocksOn;
  INT64 EnPhaseGating;
  INT64 DqsRFTrainingModeSave;
} DDR5_CA_TRAIN_IO_INIT_SAVE;

typedef struct {
  INT64 ForceOdtOn;
  INT64 ForceRxOnAmpOrDqs;
  INT64 ForceRxOnDqsMux;
  INT64 ForceRxOnDq;
  INT64 EnPhaseGating;
  INT64 DqsRFTrainingModeSave;
} LPDDR5_CA_TRAIN_IO_SAVE;

typedef union {
  LPDDR5_CA_TRAIN_IO_SAVE Lpddr5IoSave;
  DDR5_CA_TRAIN_IO_INIT_SAVE Ddr5IoSave;
  INT64 CsPulseCntSave;
} CA_PARITY_IO_SAVE;

typedef enum {
  LoopBackRdTDqsP,
  LoopBackRdTDqsN,
  LoopBackWriteTiming,
  LoopBackReadVoltage,
  LoopBackTxDqs,
  LoopBackRcvEn,
  LoopBackTasksMax
} LoopBackTasks;

typedef struct {
  UINT64  Signature;
  UINT16  LoopbackResult[LoopBackTasksMax][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
} LOOPBACK_RESULT;

///
/// External Global constants
///
#ifdef MRC_DEBUG_PRINT
extern const char CcdString[];
extern const char RcvEnDelayString[];
extern const char WrVDelayString[];
extern const char RdVDelayString[];
extern const char DqsDelayString[];
extern const char CmdVDelayString[];
extern const char TxDqDelayString[];
#endif

extern MRC_BG_BANK_PAIR Ddr5RdMprBankL2p[2];
extern MRC_BG_BANK_PAIR B2BL2p[MAX_CPGC_B2B_BANKS];
extern MRC_BG_BANK_PAIR RecEnDdr5G4L2p[MAX_CPGC_RECEN_BG_BANKS];
extern MRC_BG_BANK_PAIR BurstIdleBurstDdr5G4L2p[MAX_CPGC_RECEN_BG_BANKS];
extern MRC_BG_BANK_PAIR Ddr5_8Gbx16BankMapB2B[MAX_DDR5_8Gb_x16_BANKS];
extern MRC_BG_BANK_PAIR Ddr5_16Gbx16BankMapB2B[MAX_DDR5_16Gb_x16_BANKS];
extern MRC_BG_BANK_PAIR Ddr5_8Gbx8BankMapB2B[MAX_DDR5_8Gb_x8_BANKS];
extern MRC_BG_BANK_PAIR Ddr5_16Gbx8BankMapB2B[MAX_DDR5_16Gb_x8_BANKS];

///
/// MRC common functions
///

/**
  This function is used to calculate Vref or VSwing of a generic voltage divider.
  ReceiverOdt must be non-Zero, or exceptions occurs.
  VSS, the voltage level the PullDown is attached to, is assumed to be 0.

  @param[in]  MrcData         - Pointer to global data structure.
  @param[in]  DriverPullUp    - Used to calculate Vhigh.  Value in Ohms.
  @param[in]  DriverPullDown  - Used to calculate Vlow.  Value in Ohms.
  @param[in]  ReceiverOdt     - Value in Ohms.
  @param[in]  Vdd             - Voltage level PullUp is tied to.  Must be the same unit size as Vtermination.
  @param[in]  Vtermination    - Voltage level ReceiverOdt is tied to.  Must be the same unit size as Vdd.
  @param[in]  IsVref          - Boolean to select Vref or Vswing calculation.

  @retval - Vref in units of Vdd/Vterm
**/
UINT32
MrcCalcGenericVrefOrSwing (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                DriverPullUp,
  IN  UINT32                DriverPullDown,
  IN  UINT32                ReceiverOdt,
  IN  UINT32                Vdd,
  IN  UINT32                Vtermination,
  IN  BOOLEAN               IsVref
  );

/**
  This function is used to gather current system parameters to pass to the function,
  which calculates the ideal Read/Write/Command Vref.  It will return the result to the caller.

  @param[in]  MrcData         - Pointer to global data structure.
  @param[in]  DriverPullUp    - Used to calculate Vhigh.  Value in Ohms.
  @param[in]  DriverPullDown  - Used to calculate Vlow.  Value in Ohms.
  @param[in]  ReceiverOdt     - Value in Ohms.
  @param[in]  VrefType        - MrcMarginTypes: WrV, CmdV, RdV.
  @param[in]  Print           - Boolean switch to print the results and parameters.

  @retval - Vref in units of mV
**/
UINT32
MrcCalcIdealVref (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                DriverPullUp,
  IN  UINT32                DriverPullDown,
  IN  UINT32                ReceiverOdt,
  IN  MrcMarginTypes        VrefType,
  IN  BOOLEAN               Print
  );

/**
  This function will program the ideal Rx Vref to all the bytes in the Channel.
  Channel here denotes a 64-bit bus.  It will not program Bytes that aren't present,
  in the case of Sub-Channels.

  It has two phases:
    Calculate Vref based on parameters passed to the function.
    Enocde the Vref calculated into the CPU register format and program the register.

  Two parameters are determined inside the function stack:
    Vdd,
    Vtermination

  @param[in]  MrcData         - Pointer to MRC global data.
  @param[in]  Controller      - Memory Controller Number within the processor (0-based).
  @param[in]  Channel         - Channel to program.
  @param[in]  DriverPullUp    - Used to calculate Vhigh.  Value in Ohms.
  @param[in]  DriverPullDown  - Used to calculate Vlow.  Value in Ohms.
  @param[in]  ReceiverOdt     - Value in Ohms.
  @param[in]  PrintMsg        - Boolean switch to enable debug printing.

  @retval none
**/
VOID
MrcSetIdealRxVref (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                DriverPullUp,
  IN  UINT32                DriverPullDown,
  IN  UINT32                ReceiverOdt,
  IN BOOLEAN                PrintMsg
  );

/**
  Set default CPU Read Vref for LPDDR

  @param[in, out] MrcData      - Include all MRC global data.
  @param[in]      PhyInit      - TRUE if the function is called from PHY init.
  @param[in]      Print        - Print results.

  @retval none
**/
void
MrcSetDefaultRxVref (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN              PhyInit,
  IN     BOOLEAN              Print
  );

/**
  Return the rank mask if the rank exists in the Controller/Channel.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to work on.
  @param[in] Channel    - Channel to work on.
  @param[in] Rank       - Rank to check.

  @retval Bit mask of Rank requested if the Rank exists in the system.
**/
MRC_IRAM0_FUNCTION
UINT8
MrcRankExist (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Rank
  );

/**
  This function checks if the requested byte in the channel exists.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller  to check.
  @param[in] Channel    - Channel to check.
  @param[in] Byte       - Byte to check.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
MRC_IRAM0_FUNCTION
BOOLEAN
MrcByteExist (
  IN  MrcParameters *const  MrcData,
  IN  const UINT32          Controller,
  IN  const UINT32          Channel,
  IN  const UINT32          Byte
  );

/**
Return the number of ranks in specific dimm.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to work on.
  @param[in] Channel    - Channel to work on.
  @param[in] Dimm       - Dimm in channel to return.

@retval The number of ranks in the dimm.
**/
MRC_IRAM0_FUNCTION
UINT8
MrcGetRankInDimm (
  IN MrcParameters *const MrcData,
  IN const UINT8          Controller,
  IN const UINT8          Channel,
  IN const UINT8          Dimm
  );

/**
  Returns whether Controller is or is not present.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to test.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
extern
MRC_IRAM0_FUNCTION
BOOLEAN
MrcControllerExist (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller
  );

/**
  Returns whether Controller HW exists even though it may not be populated.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to test.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
extern
MRC_IRAM0_FUNCTION
BOOLEAN
MrcGetHwControllerExists (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller
  );

/**
  Returns whether Channel HW exists even though it may not be populated.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to test.
  @param[in] Channel    - Channel to test.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
extern
MRC_IRAM0_FUNCTION
BOOLEAN
MrcGetHwChannelExists (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  );

/**
  Returns whether Channel is or is not present.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller of the channel.
  @param[in] Channel - Channel to test.

  @retval TRUE  - if there is at least one enabled DIMM in the channel.
  @retval FALSE - if there are no enabled DIMMs in the channel.
**/
extern
MRC_IRAM0_FUNCTION
BOOLEAN
MrcChannelExist (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  );

/**
  Check whether a given MPTU is present.

  @param[in] MrcData  - Pointer to MRC global data.
  @param[in] Mptu     - MPTU to test.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
MRC_IRAM0_FUNCTION
BOOLEAN
MrcMptuExist (
  IN MrcParameters *const MrcData,
  IN const UINT32         Mptu
  );

/**
  Determine if the partition exists with the current channel population.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  PartType     - The partition type to look up.
  @param[in]  PartInstance - The partition instance to look up.
  @param[in]  PartChannel  - The partition channel to look up. Only used by the Data
                             Partitions as there are two "bytes" in 1 Data Instance.

  @return whether partition exists or not.
**/
BOOLEAN
MrcGetPartitionExists (
  IN     MrcParameters      *MrcData,
  IN     PARTITION_TYPE     PartType,
  IN     UINT32             PartInstance,
  IN     UINT32             PartChannel
  );

/**
  Determine if the hardware for a partition exists within memory subsytem even though channel may not be populated.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  PartType     - The partition type to look up.
  @param[in]  PartInstance - The partition instance to look up.
  @param[in]  PartChannel  - The partition channel to look up. Only used by the Data
                             Partitions as there are two "bytes" in 1 Data Instance.

  @return whether partition exists or not.
**/
BOOLEAN
MrcGetHwPartitionExists (
  IN     MrcParameters      *MrcData,
  IN     PARTITION_TYPE     PartType,
  IN     UINT32             PartInstance,
  IN     UINT32             PartChannel
  );

/**
  Determine if PHY Channel exists even though channel may not be populated.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  PhyChannel   - PHY channel number.

  @return whether PHY Channel exists or not.
**/
BOOLEAN
MrcGetHwPhyChannelExists (
  IN     MrcParameters      *MrcData,
  IN     UINT32             PhyChannel
  );

/**
  This function disable channel parameters.
  After this function the MRC don't use with the channel.

  @param[in] MrcData           - Include all MRC global data.
  @param[in] ChannelToDisable  - Channel to disable.
  @param[in] SkipDimmCapacity  - Switch to skip setting the DimmCapacity to 0 for the dimms in the channel disabled.

  @retval Nothing
**/
extern
void
MrcChannelDisable (
  IN MrcParameters *const MrcData,
  IN const UINT8          ChannelToDisable,
  IN const UINT8          SkipDimmCapacity
  );

/**
  This function returns the Error status results of the specified Error Type.

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  Controller  - Controller number (0-based)
  @param[in]  Channel     - the channel to work on
  @param[in]  Param       - Specifies which type of error status read will be executed.
  @param[out] Buffer      - Pointer to buffer which register values will be read into.
                              Error status bits will be returned starting with bit zero. Logical shifting will not be handled by this function.

  @retval Returns mrcWrongInputParameter if Param value is not supported by this function, otherwise mrcSuccess.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcGetErrorStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32           const  Controller,
  IN  UINT32           const  Channel,
  IN  MRC_ERR_STATUS_TYPE     Param,
  OUT INT64           *const  Buffer
  );

/**
  This function returns the Error Counter and Raster Repo status.

  @param[in]  MrcData       - Include all MRC global data.
  @param[in]  Controller    - Desired Controller.
  @param[in]  Channel       - Desired Channel.
  @param[in]  Byte          - Desired Byte.
  @param[in]  ErrControl    - Specifies the CounterControl setup. Valid ErrControl values:
                              ErrCounterCtlPerLane, ErrCounterCtlPerByte, or ErrCounterCtlPerChunk.
  @param[in]  EvenOddUIErr  - Selects which Raster Repo register to be read to get Even/Odd UI error status for all lanes.
  @param[in]  Status        - Pointer to buffer where counter status will be held.

  @retval Nothing.
**/
MRC_IRAM0_FUNCTION
void
MrcGetErrCounterStatus (
  IN  MrcParameters   *const    MrcData,
  IN  UINT32                    Controller,
  IN  UINT32                    Channel,
  IN  UINT8                     Byte,
  IN  MRC_ERR_COUNTER_CTL_TYPE  ErrControl,
  IN  UINT8                     EvenOddUIErr,
  OUT UINT64          *const    Status
);

/**
  This function gathers the Regular (Non - CA Parity) status for the channels in the McChBitMask, and returns
  an error status in the McCh bitmask format, where 1 is a pass and 0 is a fail.

  @param[in]      MrcData       - Pointer to MRC global data.
  @param[in]      McChBitMask   - Valid Channel bit mask

  @return UINT8 - Bitmask of failing channels.
**/
UINT8
MrcGetAllChannelsErrorStatus (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask
  );

/**
  This function completes setting up the Generic MRS FSM configuration to enable SAGV during normal operation.

  @param[in] MrcData  - Pointer to global MRC data.
  @param[in] Print    - Boolean control for debug print messages.

  @retval MrcStatus - mrcSuccess, otherwise an error status.
**/
MrcStatus
MrcFinalizeMrSeq (
  IN  MrcParameters *const MrcData,
  IN  BOOLEAN              Print
  );

/**
  Convert the given frequency and reference clock to a clock ratio.

  @param[in] MrcData   - Pointer to MRC global data.
  @param[in] Frequency - The memory frequency.

  @retval Returns the memory clock ratio.
**/
extern
MrcClockRatio
MrcFrequencyToRatio (
  IN MrcParameters *const  MrcData,
  IN const MrcFrequency    Frequency
  );

/**
  Convert the given ratio and reference clocks to a memory frequency.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] Ratio   - The memory ratio.

  @retval Returns the memory frequency.
**/
extern
MrcFrequency
MrcRatioToFrequency (
  IN MrcParameters *const  MrcData,
  IN const MrcClockRatio   Ratio
  );

/**
  Convert the given ratio and reference clocks to a memory clock.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] Ratio  - The memory ratio.

  @retval Returns the memory clock in femtoseconds.
**/
extern
UINT32
MrcRatioToClock (
  IN MrcParameters *const  MrcData,
  IN const MrcClockRatio   Ratio
);

/**
  This function determines the number of PI ticks required to cover the delay requested.
  This function will ceiling the result such that the PI tick delay may be greater
  than the requested time.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  TimeFs  - The delay in Femtoseconds to convert to PI ticks.

  @retval UINT32 - The number of PI ticks to reach the delay requested.
**/
UINT32
MrcFemtoSec2PiTick (
  IN  MrcParameters *const  MrcData,
  IN  const UINT32          TimeFs
  );

/**
  This function return the DIMM number according to the rank number.

  @param[in] Rank - The requested rank.

  @retval The DIMM number.
**/
extern
UINT8
MrcGetDimmFromRank (
  IN const UINT8 Rank
  );

/**
  This function sets the memory frequency.

  @param[in] MrcData           - Include all MRC global data.
  @param[in] LpJedecFreqSwitch - Boolean indicator that the current frequency update is for the LPDDR frequency switch flow.
  @param[in] DebugPrint        - Enable/disable debug printing

  @retval mrcSuccess on success, mrcFrequencyError on error.
**/
MrcStatus
McFrequencySet (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              LpJedecFreqSwitch,
  IN BOOLEAN              DebugPrint
  );

/**
  Returns the interpolated margin to a fixed # of errors
  FirstPass is 10x the first passing margin (with no errors) (10x used for int math)

  @param[in]      FirstPass     - 10x the first pass margin (w/no errors) (10x used for int match)
  @param[in]      ErrLog0       - Errors at vrefpass/10
  @param[in]      ErrLog1       - Errors at vrefpass/10+1
  @param[in]      ErrLogTarget  - Error target for the reported interpolation margin
  @param[in, out] BerStats      - Used to track interpolation vs extrapolation or if the slope is non-monotonic.
                                  NOTE: target would be Interpolation only

  @retval Interpolated/Extrapolated margin with the scale increased by 10.
**/
UINT32
MrcInterpolateMargin (
  IN     UINT32  FirstPass,
  IN     UINT32  ErrLog0,
  IN     UINT32  ErrLog1,
  IN     UINT32  ErrLogTarget,
  IN OUT UINT32  *BerStats
  );

/**
  This function swaps a subfield, within a 32 bit integer value with the specified value.

  @param[in] CurrentValue - 32 bit input value.
  @param[in] NewValue     - 32 bit New value.
  @param[in] Start        - Subfield start bit.
  @param[in] Length       - Subfield length in bits/

  @retval The updated 32 bit value.
**/
extern
UINT32
MrcBitSwap (
  IN UINT32    CurrentValue,
  IN const UINT32 NewValue,
  IN const UINT8  Start,
  IN const UINT8  Length
  );

/**
  This function returns the maximim Rx margin for a given Channel, Rank(s), byte and bits.
  RankMask is assumed to either be 0xFF or a RankMask with only 1 bit set.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Param      - Test parameter.
  @param[in] Controller - Memory Controller.
  @param[in] Channel    - Channel to calculate max Rx margin.
  @param[in] RankMask   - RankMask indicating a single rank or 0xFF causes all ranks to be considered.
  @param[in] Byte       - Byte to check.
  @param[in] Bit        - Bit to check, Optional Param. If bit = MRC_IGNORE_ARG_8 then the composite bit values are used to calculate the byte margins.
  @param[in] Sign       - Sign of the margins (0 - negative/min, 1 - positive/max).
  @param[in] MaxMargin  - Current max margin value.

  @retval The max Rx margin, either MaxMargin or value from stored margins.
**/
UINT8
MrcCalcMaxRxMargin (
  IN MrcParameters  *const MrcData,
  IN UINT8                 Param,
  IN const UINT8           Controller,
  IN const UINT8           Channel,
  IN const UINT8           RankMask,
  IN const UINT8           Byte,
  IN const UINT8           Bit, OPTIONAL
  IN const UINT8           Sign,
  IN UINT16                MaxMargin
  );

/**
  This function returns the maximim (Tx or Cmd) Vref margin for a given Channel.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to calculate max Tx margin.
  @param[in] Channel    - Channel to calculate max Tx margin.
  @param[in] RankMask   - Bit mask of ranks to consider.
  @param[in] Byte       - Zero based byte number.
  @param[in] Param      - Parameter of Vref to use
  @param[in] Sign       - Sign of the margins (0 - negative/min, 1 - positive/max).
  @param[in] MaxMargin  - Current max margin value.
  @param[in] Pda        - Use PDA or not.

  @retval The max Vref margin, either MaxMargin or value from stored margins.
**/
extern
UINT8
MrcCalcMaxVrefMargin (
  IN MrcParameters  *const MrcData,
  IN const UINT8           Controller,
  IN const UINT8           Channel,
  IN const UINT8           RankMask,
  IN const UINT8           Byte,
  IN const UINT8           Param,
  IN const UINT8           Sign,
  IN UINT8                 MaxMargin,
  IN BOOLEAN               Pda
  );

/**
  This function determines if a bit lane[0-12] (0-7 for the byte and 0-12 for CaBit) has seen a pass and a fail in each byte/CaBits for all Mc/channels populated.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] McChBitMask - Bit mask of channels to consider.
  @param[in] MaxBytes    - Max Bytes to consider
  @param[in] OnePass     - Array of Bit masks marking DQ lanes has had a passing value.
  @param[in] OneFail     - Array of Bit masks marking DQ lanes has had a failing value.

  @retval The AND result of each Channel/byte for OnePass and OneFail.
**/
UINT16
MrcAndBytes (
  IN MrcParameters* const MrcData,
  IN const UINT8          McChBitMask,
  IN const UINT8          MaxBytes,
  IN UINT16               OnePass[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM],
  IN UINT16               OneFail[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM]
);

/**
  This function Finds the margin for all channels/all bits. The margin sweep is a parameterized
  Assume REUT test has already been fully setup to run
  This will unscale the results such that future tests start at the correct point
  Uses ChangeMargin function to handle a variety cases (Timing, Voltage, Fan, etc.)

  @param[in]     MrcData      - Include all MRC global data.
  @param[in]     McChBitMask  - MC Channel BIT mask for MC Channel(s) to work on
  @param[in]     Rank         - Rank to work on
  @param[in,out] marginbit    - used as the return data ( real margin measurement, not multiplied by MARGIN_MULTIPLIER)
                                marginbit[mc,ch,byte,bit,sign] = abs(Margin)
                               Note: If param == RdTBit/RdVBit/WrVBit, marginbit is also the starting point
  @param[in,out] marginCaBit  - used as the return data ( real margin measurement, not multiplied by MARGIN_MULTIPLIER)
                                marginCaBit[mc,ch,CaBit,sign] = abs(Margin)
                                Note: If param == CmdTBit, marginCaBit is also the starting point OPTIONAL
  @param[in,out] marginbyte   - provides the starting point on a per byte basis (multiplied by MARGIN_MULTIPLIER)
  @param[in]     param        - defines the margin type
  @param[in]     MaxMargin    - Default Maximum margin
  @param[in]     MsgPrint     - Show debug prints

  @retval mrcSuccess if successful, otherwise it returns an error status.
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcGetMarginBit (
  IN     MrcParameters *const MrcData,
  IN     UINT8                McChBitMask,
  IN     UINT8                Rank,
  IN OUT UINT16               marginbit[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS][MAX_EDGES], OPTIONAL
  IN OUT UINT16               marginCaBit[MAX_CONTROLLER][MAX_CHANNEL][MAX_DDR5_CMD_PINS][MAX_EDGES], OPTIONAL
  IN OUT UINT16               marginbyte[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN     UINT8                param,
  IN     UINT8                MaxMargin,
  IN     BOOLEAN              MsgPrint
  );

/**
  Assume REUT test has already been fully setup to run
  Finds the margin for all channels/all bytes
  The margin sweep is parameterized
  Uses ChangeMargin function to handle a variety of cases (Timing, Voltage, Fan, etc.)
  margin is used as the starting point for the search (10x the actual margin)
  Interior: Search inside marginch limits, enabling multiple calls with different setups
  To reduce repeatibility noise, the returned margins is actually a BER extrapolation

  @param[in]     MrcData      - The global MrcData
  @param[in,out] margin       - Data structure with the latest margin results
  @param[in,out] marginBit    - Data structure with the latest margin results at bit level
  @param[in]     McChBitmask  - Bit mask of present MC channels
  @param[in]     RankMaskIn   - Rank Mask to change margins for
  @param[in]     RankRx       - Ranks for Rx margin
  @param[in]     param        - parameter to get margins for
  @param[in]     EnBER        - Enable BER extrapolation calculations
  @param[in]     MaxMargin    - Max Margin allowed for the parameter
  @param[in]     LevelOfMargin- Granularity of Margin: Channel/Byte/Bit
  @param[in,out] BERStats     - Bit Error Rate Stats.

  @retval mrcSuccess if successful, otherwise returns an error status.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcGetBERMargin (
  IN     MrcParameters * const MrcData,
  IN OUT UINT16          margin[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN OUT UINT16          marginBit[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS][MAX_EDGES], OPTIONAL
  IN     UINT8           McChBitMask,
  IN     UINT8           RankMaskIn,
  IN     UINT8           RankRx,
  IN     UINT8           param,
  IN     UINT8           EnBER,
  IN     UINT8           MaxMargin,
  IN     MRC_MARGIN_LEVEL LevelOfMargin,
  IN OUT UINT32          *BERStats
  );
/**
  Check whether there is errors at Point RdT/RdV or WrT/WrV

  @param[in]      MrcData     - Include all MRC global data.
  @param[in]      McChBitmask - Bit mask of present MC channels
  @param[in]      RankMask    - Bit mask of Ranks to change margins for
  @param[in]      MarginPoint - Margin Point to test
  @param[in]      ParamType   - MrcMarginTypes: WrV, WrT, RdT, RdV.

  @retval MrcStatus - mrcSuccess if point successful pass, otherwise returns an error status.
**/
MrcStatus
MrcDataPointTest (
  IN     MrcParameters     *MrcData,
  IN     UINT8             McChBitmask,
  IN     UINT8             RankMask,
  IN     MarginCheckPoint  *MarginPoint,
  IN     MrcMarginTypes    ParamType
  );

/**
  Check whether there is errors at Point CmdT/CmdV

  @param[in]      MrcData     - Include all MRC global data.
  @param[in]      McChBitmask - Bit mask of present MC channels
  @param[in]      RankMask    - Bit mask of Ranks to change margins for
  @param[in]      MarginPoint - CMD Margin Point to test
  @param[in]      ParamType   - MrcMarginTypes: CmdT, CmdV.

  @retval MrcStatus - mrcSuccess if point successful pass, otherwise returns an error status.
**/
MrcStatus
MrcCmdPointTest (
  IN     MrcParameters    *MrcData,
  IN     UINT8            McChBitMask,
  IN     UINT8            RankMask,
  IN     MarginCheckPoint *MarginPoint,
  IN     MrcMarginTypes   ParamType
  );

/**
  This function shifts a 32 bit int either positive or negative

  @param[in] Value       - Input value to be shifted
  @param[in] ShiftAmount - Number of bits places to be shifted.

  @retval 0 if ShiftAmount exceeds +/- 31.  Otherwise the updated 32 bit value.
**/
extern
UINT32
MrcBitShift (
  IN const UINT32 Value,
  IN const INT8  ShiftAmount
  );

/**
  This function Sign extends OldMSB to NewMSB Bits (Eg: Bit 6 to Bit 7)

  @param[in] CurrentValue - Input value to be shifted
  @param[in] OldMSB       - The original most significant Bit
  @param[in] NewMSB       - The new most significant bit.

  @retval The updated 8 bit value.
**/
extern
UINT16
MrcSE (
  IN UINT16     CurrentValue,
  IN const UINT16  OldMSB,
  IN const UINT16  NewMSB
  );

/**
  This function calculates the Log base 2 of the value to a maximum of Bits

  @param[in] MrcData - Include all MRC global data
  @param[in] Value - Input value

  @retval Returns the log base 2 of input value
**/
extern
UINT8
MrcLog2 (
  IN MrcParameters *MrcData,
  IN const UINT32 Value
  );

/**
  This function calculates the Log base 2 of the value to a maximum of Bits

  @param[in] MrcData - Include all MRC global data
  @param[in] Value - Input value

  @retval Returns the log base 2 of input value + 1 if result is not round number.
**/
UINT8
MrcLog2Round (
  IN MrcParameters *MrcData,
  IN const UINT32 Value
  );

/**
  This function calculates the Log base 2 of the value to a maximum of Bits

  @param[in] MrcData - Include all MRC global data
  @param[in] Value - Input value

  @retval Returns the round down log base 2 of input value.
**/
UINT8
MrcLog2RoundDown (
  IN MrcParameters *MrcData,
  IN const UINT32 Value
  );

/**
  This function search for item in a list and return index + 1 or 0 in not found

  @param[in]  Param       - Param to search for
  @param[in]  ParamList   - list of Params
  @param[in]  ParamLen    - Params length
  @param[in]  Offset      - list of the corresponded Offsets to work on
  @param[out] Override    - override indicator
  @param[out] LocalOffset - localoffset

  @retval Returns result
**/
UINT8
MrcSearchList (
  IN           UINT8    Param,
  IN     const UINT8   *ParamList,
  IN     const UINT8    ParamLen,
  IN     const INT8    *Offset,
  OUT          BOOLEAN *Override,
  OUT          INT8    *LocalOffset
  );

/**
  ***** Has Buffer Overflow for 68-71, 544-575, 4352-4607, ... ****
  This function calculates the Log base 8 of the Input parameter using integers

  @param[in] Value - Input value

  @retval Returns 10x the log base 8 of input Value
**/
extern
UINT32
MrcLog8 (
  IN UINT32 Value
  );

/**
  This function Sorts Arr from largest to smallest

  @param[in,out] Arr     - Array to be sorted
  @param[in]     Channel - Channel to sort.
  @param[in]     lenArr  - Length of the array

  @retval Nothing
**/
extern
void
MrcBsortPerChannel (
  IN OUT UINT32   Arr[MAX_CHANNEL][4],
  IN     const UINT8 Channel,
  IN     const UINT8 lenArr
  );

/**
  This function Sorts Arr from largest to smallest

  @param[in,out] Arr    - Array to be sort
  @param[in]     lenArr - Lenght of the array

  @retval Nothing
**/
extern
void
MrcBsort (
  IN OUT UINT32 *const Arr,
  IN     const UINT8   lenArr
  );

/**
  This function checks if a specific MR is treated as PDA according to the technology.

  @param[in] MrcData           - Pointer to MRC global data.
  @param[in] Controller        - Controller to work on.
  @param[in] Channel           - channel to work on.
  @param[in] MrAddress         - MR Address to check
  @param[in] IsSagvConfig      - TRUE if Programming MRS FSM for SAGV Transistion.
                                  If TRUE, Force DIMM DFE MRs to enable PDA

  @retval TRUE if the specific MR is PDA otherwise FALSE
**/
BOOLEAN
MrcMrIsPda (
  IN    MrcParameters      *MrcData,
  IN    UINT32             Controller,
  IN    UINT32             Channel,
  IN    MrcModeRegister    MrAddress,
  IN    BOOLEAN            IsSagvConfig
  );

/**
  This function converts from the MR address to the index needed to access this MR
  inside the storage array in MrcData.

  @param[in]      MrcData   - Pointer to global MRC Data.
  @param[in,out]  MrAddress - Pointer to MR address to translate.

  @retval UINT8 - The index to use in the MR array in MrcData or 0xFF if unsupported.
**/
UINT8
MrcMrAddrToIndex (
  IN      MrcParameters   *MrcData,
  IN OUT  MrcModeRegister *MrAddress
  );

/**
  This function calculates the Natural Log of the Input parameter using integers

  @param[in] Input - 100 times a number to get the Natural log from.
                      Max Input Number is 40,000 (without 100x)

  @retval 100 times the actual result. Accurate within +/- 2
**/
extern
UINT32
MrcNaturalLog (
  IN UINT32 Input
  );

/**
  This function calculates the number of bits set to 1 in a 32-bit value.

  @param[in] Input - The value to work on.

  @retval The number of bits set to 1 in Input.
**/
extern
UINT8
MrcCountBitsEqOne (
  IN UINT32 Input
  );

/**
  Returns the bit position of the lowest bit set in a 32-bit value.

  This function computes the bit position of the lowest bit set in the 32-bit
  value specified by Operand. If Operand is zero, then -1 is returned.
  Otherwise, a value between 0 and 31 is returned.

  @param  Operand The 32-bit operand to evaluate.

  @retval 0..31  The lowest bit set in Operand was found.
  @retval -1     Operand is zero.

**/
INT32
MrcLowBitSet32 (
  IN      UINT32  Operand
  );

/**
  This function calculates e to the power of of the Input parameter using integers.

  @param[in] Input - 100 times a number to elevate e to.

  @retval 100 times the actual result. Accurate within +/- 2.
**/
extern
UINT32
Mrceexp (
  IN UINT32 Input
  );

/**
  This function approximates the Square Root in integer math.  It scales the number by 100
  such that the ones and tens digit are the first two decimal points of the calculation.

  The algorithm implemented is the Babylonian Method for calculating Square Roots.  The
  approximate of the square root is found when the Remainder and the Result of Radicand / X
  are equal.  The algorithm converges to this by starting with an initial guess of X, and
  averaging the result and remainder of (Radicand / X) and using that average as the next X.

  The largest Radicand that can be calculated is 429496 as the function uses UINT32.

  @param[in] Radicand - Value to calculate the Square Root of.

  @retval UINT32 - Approximate Square Root
**/
extern
UINT32
MrcSqrt (
  IN UINT32 Radicand
  );

/**
  This function calculates the integer approximate for the mean and standard deviation
  from the list of values.

  @param[in] MrcData        - Pointer to global MRC data.
  @param[in] ValueList      - Input list for calculation
  @param[in] ListLen        - Length of input list
  @param[out,optional] Mean - Pointer to store Mean.

  @retval UINT32 - Integer approximate standard deviation.
**/
extern
UINT32
MrcStd (
  IN MrcParameters   *MrcData,
  IN const INT32     ValueList[],
  IN UINT8           ListLen,
  OUT OPTIONAL INT32 *Mean
  );

/**
  This function writes a 32 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.
  @param[in] Value   - Value to write.

  @retval The register value.
**/
MRC_IRAM0_FUNCTION
UINT32
MrcWriteCrMulticast (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const UINT32  Value
  );

/**
  This function writes a 64 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.
  @param[in] Value   - Value to write.

  @retval The register value.
**/
UINT64
MrcWriteCR64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const UINT64  Value
  );

/**
  This function writes a 32 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.
  @param[in] Value   - Value to write.

  @retval The register value.
**/
MRC_IRAM0_FUNCTION
UINT32
MrcWriteCR (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const UINT32  Value
  );

/**
  This function writes a 8 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.
  @param[in] Value   - The value to write.

  @retval The register value.
**/
extern
UINT8
MrcWriteCR8 (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const UINT8   Value
  );

/**
  This function reads a 64 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.

  @retval Value read from the register.
**/
UINT64
MrcReadCR64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset
  );

/**
  This function reads a 32 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.

  @retval Value read from the register
**/
MRC_IRAM0_FUNCTION
UINT32
MrcReadCR (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset
  );

/**
  Wait for at least the given number of nanoseconds.

  @param[in] MrcData   - Include all MRC global data.
  @param[in] DelayNs   - time to wait in [ns], up to 2^32 [ns] = 4.29 seconds

  @retval Nothing
**/
MRC_IRAM0_FUNCTION
void
MrcWait (
  IN MrcParameters *const MrcData,
  IN UINT32               DelayNs
  );

/**
  Wait for at least the given number of DRAM clocks.

  Utilizes MrcData->Outputs.MemoryClock as the clock period.

  @param[in] MrcData   - Include all MRC global data.
  @param[in] DelayTck  - time to wait in clocks, up to 2^32 [ns] = 4.29 seconds

  @retval Nothing
**/
extern
void
MrcWaitClk (
  IN MrcParameters *const MrcData,
  IN UINT32               DelayTck
  );

/**
  Fill AddressArray with address related settings data per channel

  @param[in]  MrcData             - Include all MRC global data.
  @param[in]  AddressArray    - 2D Array of Structure that stores address related settings per channel.
  @param[in]  Address         - Structure that stores address related settings.

  @retval Nothing
**/
MRC_IRAM0_FUNCTION
void
FillAddressArray (
  IN  MrcParameters *const    MrcData,
  IN  MRC_ADDRESS             AddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN  MRC_ADDRESS *const      Address
  );

/**
  Divide TestLength by Number of Cache lines per loop to define how many loops do we need

  @param[in]  NumCL                - Number of Cache lines per loop
  @param[in]  TestLengthExp        - Exponential Value of the total test length

  @retval Number of loops needed to perform the test (LoopCount)
**/
MRC_IRAM0_FUNCTION
UINT32
MrcCalculateLoopCount (
  IN const UINT32                 NumCL,
  IN const UINT8                  TestLengthExp
  );

/**
  Programs REUT to run on the selected physical ranks.

  @param[in] MrcData           - Pointer to MRC global data.
  @param[in] Controller        - Controller to enable.
  @param[in] ch                - Channel to enable.
  @param[in] RankBitMask       - Bit mask of ranks to enable.
  @param[in] SkipRankL2P       - Boolean control over skipping Logical 2 Physical programming for Rank.

  @retval Bit mask of channel enabled if rank in the channel exists.
**/
extern
MRC_IRAM0_FUNCTION
UINT8
SelectReutRanks (
  IN MrcParameters *const MrcData,
  IN const UINT8          Controller,
  IN const UINT8          ch,
  IN UINT8                RankBitMask,
  IN BOOLEAN              SkipRankL2P
  );

/**
  Programs REUT to run on the selected physical ranks for all channels in the system.

  @param[in] MrcData           - Pointer to MRC global data.
  @param[in] RankBitMask       - Bit mask of ranks to enable.
  @param[in] SkipRankL2P       - Boolean control over skipping Logical 2 Physical programming for Rank.

  @retval Bit mask of all channel in the system for the selected rank.
**/
extern
MRC_IRAM0_FUNCTION
UINT8
SelectReutRanksAll (
  IN MrcParameters *const MrcData,
  IN UINT8                RankBitMask,
  IN BOOLEAN              SkipRankL2P
  );

/**
  Returns the index into the array MarginResult in the MrcOutput structure.

  @param[in] ParamV - Margin parameter

  @retval One of the following values: LastRxV(0), LastRxT (1), LastTxV(2), LastTxT (3), LastRcvEna (4),
                                       LastWrLevel (5), LastCmdT (6), LastCmdV (7)
**/
extern
MRC_IRAM0_FUNCTION
UINT8
GetMarginResultType (
  IN const UINT8  ParamV
  );


/**
  Update the calculated Margin into Host structure.

  @param[in,out] MrcData           - Pointer to MRC global data.
  @param[in]     ParamV - Margin parameter
  @param[in]     Controller
  @param[in]     Channel
  @param[in]     Rank

  @retval Updated Margin variables in Host struct
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
UpdateMarginHost (
  IN OUT MrcParameters *const MrcData,
  IN const UINT8 ParamV,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN UINT8 Rank
);

/**
  This function Reads MrcData structure and finds the minimum last recorded margin for param.
  Searches across all bytes and ranks in RankMask.

  @param[in]     MrcData         - Include all MRC global data.
  @param[in,out] MarginResult    - Data structure with the latest margin results.
  @param[in]     Param           - Defines the margin type
  @param[in]     RankMask        - Condenses down the results from multiple ranks
  @param[in]     ResultRank      - This rank will keep the results from multiple ranks
  @param[in]     EnhancedChMode  - This parameter controls the margin search and location if we support x32 or x64 channels.

  @retval mrcWrongInputParameter if a bad Param is passed in, otherwise mrcSuccess.
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
GetMarginCh (
  IN     MrcParameters *const MrcData,
  IN OUT UINT16               MarginResult[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN     const UINT8          Param,
  IN     const UINT8          RankMask,
  IN     const UINT8          ResultRank
  );

/**
  This function Reads MrcData structure and finds the minimum last recorded margin for param
  Searches across all bytes and ranks in RankMask

  @param[in]     MrcData          - Include all MRC global data.
  @param[in,out] MarginResult0    - Data structure with the latest margin results type 0.
  @param[in,out] MarginResult1    - Data structure with the latest margin results type 1.
  @param[in,out] MarginResult2    - Data structure with the latest margin results type 2.
  @param[in]     Param            - Defines the margin type
  @param[in]     RankMask         - Desired RankMask that should be scanned.

  @retval mrcWrongInputParameter if a bad Param is passed in, otherwise mrcSuccess.
**/
MRC_IRAM0_FUNCTION
MrcStatus
GetPdaMargins (
  IN     MrcParameters *const MrcData,
  IN OUT UINT16               MarginResult0[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN OUT UINT16               MarginResult1[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN OUT UINT16               MarginResult2[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS][MAX_EDGES],
  IN     const UINT8          Param,
  IN     const UINT8          RankMask
  );

/**
  Use this function to retrieve the last margin results from MrcData

  @param[in]     MrcData         - Include all MRC global data.
  @param[in,out] MarginResult    - Data structure with the latest margin results.
  @param[in]     Param           - Defines the margin type
  @param[in]     RankIn          - Which rank of the host structure you want the result returned on
  @param[in]     Ranks           - Condenses down the results from multiple ranks

  @retval MarginResult structure has been updated if MrcStatus returns mrcSuccess.
  @retval Otherwise, mrcWrongInputParameter is returned if an incorrect Param is passed in.
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
GetMarginByte (
  IN     MrcParameters *const MrcData,
  IN OUT UINT16               MarginResult[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN     const UINT8          Param,
  IN     const UINT8          RankIn,
  IN     const UINT8          Ranks
  );

/**
  This function is used by most margin search functions to change te underlying margin parameter.
  This function allows single search function to be used for different types of margins with minimal impact.
  It provides multiple different parameters, including 2D parameters like Read or Write FAN.
  It can work in either MultiCast or single register mode.

  @param[in,out] MrcData       - Include all MRC global data.
  @param[in]     param         - Includes parameter(s) to change including two dimensional.
  @param[in]     value0        - Selected value to program margin param to
  @param[in]     value1        - Selected value to program margin param to in 2D mode (FAN mode)
  @param[in]     EnMultiCast   - To enable Multicast (broadcast) or single register mode
  @param[in]     Controller    - Desired Memory Controller
  @param[in]     Channel       - Desired Channel
  @param[in]     RankMaskIn    - Desired Rank mask that changes should be applied to
  @param[in]     byte          - Desired byte offset register
  @param[in]     UpdateMrcData - Used to decide if MRC host must be updated
  @param[in]     SkipWait      - Used to skip wait until all channel are done

  @retval MrcStatus -  if succeeded, return mrcSuccess
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
ChangeMargin (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          param,
  IN     const INT32          value0,
  IN     const INT32          value1,
  IN     const UINT8          EnMultiCast,
  IN     const UINT8          Controller,
  IN     const UINT8          Channel,
  IN     const UINT8          RankMaskIn,
  IN     const UINT16         byte,
  IN     const UINT8          UpdateMrcData,
  IN     const UINT8          SkipWait
  );

/**
  This function maps the Change Margin param to the Get Set group
  This function only supports params: RcvEna, RdT, WrT, WrLevel, WrDqsT, RcvEnaX, WrTUnMatched.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] param    - Change margin parameter

  return GSM_GT - The Get Set group that maps to the input parameter
**/
GSM_GT
MrcGetChangeMarginGroup (
  IN  MrcParameters* const MrcData,
  IN  const UINT8          param
  );

/**
  This function routes to the appropriate largest pass 1D centering function based on param type

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     param          - {0:RcvEn, 1:RdT, 2:WrT, 3: WrDqsT, 4:RdV, 5:WrV, 6:WrLevel,
                                   7:RdVBit, 8:RdVBit1, 9:RcvEnaX, 10:WrTUnMatched, 11:CmdT,
                                   12:CmdV, 13:RdTN, 14:RdTP, 15:RdTDbi, 16:WrTDbi, 17:RdVDbi}
  @param[in]     StepSize       - Step size
  @param[in]     loopcount      - loop count
  @param[in]     MsgPrintMask   - Show debug prints bit mask
  @param[in]     EarlyCentering - Execute as early centering routine

  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
LargestPass1DCentering (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          param,
  IN     const UINT8          StepSize,
  IN     const UINT8          loopcount,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              EarlyCentering
  );

/**
  This function saves or restores all CmdGrpPi/CtlGrpPi/CsPerBitCcc which are populated in the system.

  @param[in]     MrcData       - Pointer to global MRC data.
  @param[in,out] Buffer        - Pointer to buffer used for Save/Restore.
  @Param[in]     SaveOrRestore - MrcSaveOrRestore emum determining the behavior of the method.
**/
VOID
MrcCmdCtlPiSaveRestore (
  IN     MrcParameters         *const MrcData,
  IN OUT MRC_CMD_CTL_MARGIN_SR        *Buffer,
  IN     MrcSaveOrRestore             SaveOrRestore
  );

/**
  This procedure is meant to handle Write timing centering, places strobe in the middle of the data eye,
  using a very robust, linear search algorithm.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     StepSize       - Step size
  @param[in]     LoopCount      - loop count
  @param[in]     MsgPrintMask   - Serial debug output message enable.
  @param[in]     EarlyCentering - Execute as early centering routine

  @retval        MrcStatus      -  If succeeded, return mrcSuccess
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
WriteTimeByteCentering1D (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          StepSize,
  IN     const UINT8          LoopCount,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              EarlyCentering,
  IN     UINT8                RankBitMask
  );

/**
  This procedure is meant to handle command voltage centering, places  CmdV in the middle of the eye,
  using a very robust, linear search algorithm.

  @param[in,out] MrcData          - Include all MRC global data.
  @param[in]     StepSize         - Step size
  @param[in]     MsgPrintMask     - Serial debug output message enable mask
  @param[in]     IsEarlyCentering - Execute as early centering routine
  @param[out]    VrefOffsets      - Per-rank offsets from the low CAC Vref edge

  @retval        MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
CmdVCentering1D (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          StepSize,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              IsEarlyCentering,
  OUT    UINT8                VrefOffsets[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL]
  );

/**
  This procedure is meant to handle Write Voltage centering, places WrV in the middle of the eye,
  using a very robust, linear search algorithm.

  @param[in,out] MrcData          - Include all MRC global data.
  @param[in]     StepSize         - Step size
  @param[in]     LoopCount        - loop count
  @param[in]     MsgPrintMask     - Serial debug output message enable mask
  @param[in]     IsEarlyCentering - Execute as early centering routine

  @retval        MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
WrVCentering1D (
  IN OUT MrcParameters* const MrcData,
  IN     const UINT8          StepSize,
  IN     const UINT8          LoopCount,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              IsEarlyCentering
  );

/**
  This procedure is meant to handle basic timing centering, places strobe in the middle of the data eye,
  for read DQ/DQS using a very robust, linear search algorithm.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     param          - {1:RdT, 34:RdTN, 35:RdTP}
                                  ONLY RdT, RdTN and RdTP are allowed in this function
  @param[in]     StepSize       - Step size
  @param[in]     loopcount      - loop count
  @param[in]     MsgPrintMask   - Show debug prints bit mask
  @param[in]     EarlyCentering - Execute as early centering routine

  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
RdTimingCentering1D (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          param,
  IN     const UINT8          StepSize,
  IN     const UINT8          loopcount,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              EarlyCentering
  );

/**
  RdVCentering1D () is meant to handle Read Voltage centering per bit or per byte, places RdV in the middle of the eye for each DQ or each byte,
  using a very robust, linear search algorithm.

  @param[in,out] MrcData          - Include all MRC global data.
  @param[in]     StepSize         - Step size.
  @param[in]     LoopCount        - Loop count.
  @param[in]     MsgPrintMask     - Serial debug output message enable mask.
  @param[in]     IsEarlyCentering - Execute as early centering routine.

  @retval        MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
RdVCentering1D (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          StepSize,
  IN     const UINT8          LoopCount,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              IsEarlyCentering
  );

/**
  This procedure is meant to handle write unmatched time centering, places strobe in the middle of the data eye,
  using a very robust, linear search algorithm.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     StepSize       - Step size
  @param[in]     loopcount      - loop count
  @param[in]     MsgPrintMask   - Show debug prints bit mask
  @param[in]     EarlyCentering - Execute as early centering routine

  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
WrTUnmatchedTimeCentering1D (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          StepSize,
  IN     const UINT8          loopcount,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              EarlyCentering
  );

/**
  This procedure is meant to handle much more complex centering that will use a 2D algorithm to optimize asymetical
  eyes for both timing and voltage margin.

  @param[in,out] MrcData         - Include all MRC global data.
  @param[in,out] MarginResult    - Margin data from centering
  @param[in]     McChBitMaskIn   - MC Channel bit mask.
  @param[in]     param           - {0:RcvEn, 1:RdT, 2:WrT, 3: WrDqsT, 4:RdV, 5:WrV, 6:WrLevel,
                                    7:RdVBit, 8:RdVBit1, 9:RcvEnaX, 10:WrTUnMatched, 11:CmdT,
                                    12:CmdV, 13:RdTN, 14:RdTP, 15:RdTDbi, 16:WrTDbi, 17:RdVDbi}
                                    ONLY RdT and WrT are allowed in this function
  @param[in]     EnRxDutyCycleIn - Phase to center.
  @param[in]     LoopCount       - loop count
  @param[in]     En2D            - Option to only run center at nominal Vref point

  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
extern
MrcStatus
DataTimeCentering2D (
  IN OUT MrcParameters *const MrcData,
  IN OUT UINT16               MarginResult[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN     const UINT8          McChBitMaskIn,
  IN     const UINT8          Param,
  IN     const UINT8          EnRxDutyCycleIn,
  IN     const UINT8          LoopCount,
  IN     const UINT8          En2D
  );

/**
  This procedure is meant to centering Ca Per Lane with 1Pass/1Fail
  @param[in,out] MrcData         - Include all MRC global data.
  @param[in,out] MarginResult    - Margin data from centering
  @param[in]     McChBitMaskIn   - MC Channel bit mask.
  @param[in]     ResetPerBit     - Option to Reset PerBit Deskew to middle value before byte training
  @param[in]     LoopCount       - loop count
  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
CccTimeCenteringPerLane (
  IN OUT MrcParameters* const MrcData,
  IN OUT UINT16               MarginResult[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN     const UINT8          McChBitMaskIn,
  IN     const UINT8          ResetPerBit,
  IN     const UINT8          LoopCount
  );

/**
  Subfunction of 2D Timing Centering
  Measures paramV margin across ch/bytes and updates the EH/VrefScale variables

  @param[in]     MrcData     - Include all MRC global data.
  @param[in]     McChBitMask - MC Channel Bit mak for which test should be setup for.
  @param[in]     rank        - Defines rank to used for MrcData
  @param[in]     ParamV      - Margin parameter
  @param[in]     BMap        - Byte mapping to configure error counter control register
  @param[in]     PdaMode     - TRUE if PDA is used for WrV
  @param[in,out] EH          - Structure that stores start, stop and increment details for address
  @param[in,out] VrefScale   - Parameter to be updated
  @param[in,out] BERStats    - Bit Error Rate Stats.

  @retval mrcSuccess if successful, otherwise the function returns an error status.
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
DQTimeCenterEH (
  IN     MrcParameters * const MrcData,
  IN     const UINT8           McChBitMask,
  IN     const UINT8           rank,
  IN     const UINT8           ParamV,
  IN     UINT8 * const         BMap,
  IN     const BOOLEAN         PdaMode,
  IN OUT UINT16                EH[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM],
  IN OUT UINT16                VrefScale[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN OUT UINT32 * const        BERStats
  );

#ifdef MRC_DEBUG_PRINT
/**
  This step prints out the key training parameters.
  This will allow tracking of current training settings across multiple steps

  @param[in]     MrcData                - Include all MRC global data.
  @param[in]     PrintMrcDoneParamTable - extra list of all group types that is printed after MRC is Done
  @param[in]     DoneNumberOfElements   - Number Of Elements in PrintMrcDoneParamTable
  @param[in]     PrintTableBase         - basic list of all group types that is printed after Comp Offset optimization or after MRC is Done
  @param[in]     BaseNumberOfElements   - Number Of Elements in PrintTableBase
  @param[in]     PrintParamTable        - list of all group types that is printed after a specific training step is Done
  @param[in]     TaskNumberOfElements   - Number Of Elements in PrintParamTable
  @param[in]     PostCode               - Parameter to be updated

  @retval Nothing
**/
void
MrcTrainedStateTrace (
  IN MrcParameters *const  MrcData,
  IN const GSM_GT          *PrintMrcDoneParamTable,
  IN UINT32                DoneNumberOfElements,
  IN const GSM_GT          *PrintTableBase,
  IN UINT32                BaseNumberOfElements,
  IN const GSM_GT          *PrintParamTable,
  IN UINT32                TaskNumberOfElements,
  IN MrcPostCode           PostCode
  );
#endif //MRC_DEBUG_PRINT

/**
  Update the CA/DQ Vref value

  @param[in,out] MrcData             - Include all MRC global data.
  @param[in]     McChannelMask       - Select the Channels to update Vref for.
  @param[in]     RankMask            - Selecting which Rank to talk to WrV and CmdV
  @param[in]     DeviceMask          - Selecting which Devices to talk to (only valid for adjusting VrefDQ)
  @param[in]     VrefType            - Determines the Vref to change: WrV or CmdV only.
  @param[in]     Offset              - Vref offset value.
  @param[in]     UpdateMrcData       - Used to decide if Mrc host must be updated.
  @param[in]     PDAMode             - Selecting to use MRH or old method for MRS (only valid for adjusting VrefDQ)
  @param[in]     SkipWait            - Determines if we will wait for vref to settle after writing to register
  @param[in]     IsCachedOffsetParam - Determines if the parameter is an offset (relative to cache) or absolute value. Relevant only for internval Vref.

  @retval Nothing
**/
MRC_IRAM0_FUNCTION
void
MrcUpdateVref (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               McChannelMask,
  IN     UINT8                RankMask,
  IN     UINT16               DeviceMask,
  IN     UINT8                VrefType,
  IN     INT32                Offset,
  IN     BOOLEAN              UpdateMrcData,
  IN     BOOLEAN              PDAMode,
  IN     BOOLEAN              SkipWait,
  IN     BOOLEAN              IsCachedOffsetParam
  );

/**
  This function is used to move CMD/CTL/CLK/CKE PIs during training

  @param[in,out] MrcData    - Include all MRC global data.
  @param[in]     Controller - Controller to shift PI.
  @param[in]     Channel    - Channel to shift PI.
  @param[in]     Iteration  - Determines which PI to shift:
                              MrcIterationClock = 0
                              MrcIterationCmd   = 1
                              MrcIterationCtl   = 2
  @param[in]     RankMask   - Ranks to work on
  @param[in]     GroupMask  - Which groups to work on for CLK/CMD/CTL. See MrcGetCmdGroupMax()
  @param[in]     NewValue   - value to shift in case of CLK Iteration, New value for all other cases
  @param[in]     UpdateHost - Determines if MrcData structure is updated

  @retval mrcSuccess if successful, otherwise the function returns an error if the register setting was out of range for MrcIterationCmdCtl.
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
ShiftPIforCmdTraining (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel,
  IN     const UINT8          Iteration,
  IN     const UINT8          RankMask,
  IN     const UINT8          GroupMask,
  IN     INT32                NewValue,
  IN     const UINT8          UpdateHost
  );

/**
  Shifts RcvEn, WriteLevel and WriteDQS timing for all bytes
  Usually used when moving the clocks on a channel

  @param[in,out] MrcData    - Include all MRC global data.
  @param[in]     Controller - Controller to update
  @param[in]     Channel    - Channel to update
  @param[in]     RankMask   - Ranks to update
  @param[in]     ByteMask   - Bytes to update
  @param[in]     Offset     - value to shift
  @param[in]     UpdateHost - Determines if MrcData structure is updated

  @retval Nothing
**/
extern
MRC_IRAM0_FUNCTION
void
ShiftDQPIs (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel,
  IN     const UINT8          RankMask,
  IN     const UINT32         ByteMask,
  IN     const INT16          Offset,
  IN     const UINT8          UpdateHost
  );

#ifdef MRC_DEBUG_PRINT
/**
  Debug output of MarginResults for specific ResultType

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] ResultType - Margin Result Type.  MAX_RESULT_TYPE prints all parameters.

  @retval Nothing
**/
void
MrcDisplayMarginResultArray (
  IN MrcParameters *MrcData,
  IN UINT8         ResultType
  );
#endif // MRC_DEBUG_PRINT

/**
  Setup for Memory Check like Alias Checking and PDA mapping

  @param[in, out] MrcData           - Pointer to MRC global data.
  @param[in, out] ReutAddress       - Pointer to ReutAddress that will be programmed to REUT engine
  @param[in] Rank                   - Rank to setup
  @param[in] NumOfCachelines        - # of Cachelines to write and read to.  0 is a special value that will write the number
                                      of Cachelines that is required to determine Row/Column Aliasing.
  @param[in, out] ReutUninitialized - Pointer of whether ReutAddress Pointer needs initializing or not.

  @retval Nothing
void
MrcMemoryCheckSetup (
  IN OUT MrcParameters *const MrcData,
  IN OUT MRC_REUTAddress      *ReutAddress,
  IN UINT8                    Rank,
  IN UINT8                    NumOfCachelines,
  IN OUT BOOLEAN              *ReutUninitialized
  );
**/

/**
  Returns the effective Dimm Write ODT.
  Consider the dram tech, Rtt configuration (Wr, Nom, Park) and Normal Odt logic.
  Note : We still have the assumption of same Rtt's for all ranks in a same dimm.

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      Controller    - Controller to setup
  @param[in]      Channel       - Channel to setup
  @param[in]      Rank          - Rank to setup
  @param[in]      OdtType       - 0:write 1:read
  @param[in]      GetFromTable  - Get the Values from Odt tables

  @retval Value in Ohms
**/
extern
UINT32
MrcGetEffDimmWriteOdt (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               Controller,
  IN     UINT8                Channel,
  IN     UINT8                Rank,
  IN     UINT8                OdtType,
  IN     BOOLEAN              GetFromTable
  );

/**
  Generic routine to perform Linear Centering in different 2D points.
  Re-uses LargestPass1DCentering routine as the linear centering routine.
  Optimize Per channel in the 2D param space.
  Run-Time - about 20nsec for 6 vref points and loopcout of 10

  @param[in,out] MrcData             - Include all MRC global data.
  @param[in]     ChBitMask           - Channels to test.
  @param[in]     RankBitMask         - Ranks to test.
  @param[in]     Param               - Param to center with linear search - use LargestPass1DCentering (RdT,WrT,WrV,RdV,RcvX)
  @param[in]     Param2D             - The 2D Param to Offset - Use change margin routine
  @param[in]     Points2DMin         - Minimum value of second dimension points to sweep.
  @param[in]     Points2DMax         - Maxmimum value of second dimension points to sweep.
  @param[in]     ParamStepSize       - Step size of second dimension points to sweep.
  @param[in]     LoopCount           - Loop count
  @param[in]     TestType            - 0: normal reads, 1: Read MPR
  @param[in]     RunEyeSizeFailCheck - Fail for eyes that are too small.
  @param[in]     Prints              - Debug prints enable/disable

  @retval MrcStatus - mrcSuccess if passed, otherwise an error status.
**/
MrcStatus
EarlyLinearCentering2D (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                McChBitMask,
  IN     UINT8                RankBitMask,
  IN     UINT8                Param,
  IN     UINT8                Param2D,
  IN     INT16                Points2DMin,
  IN     INT16                Points2DMax,
  IN     UINT8                ParamStepSize,
  IN     UINT8                LoopCount,
  IN     UINT8                TestType,
  IN     BOOLEAN              RunEyeSizeFailCheck,
  IN     BOOLEAN              Prints
  );

/**
  Get the maximal possible offset for a given Param (e.g., WrV, RdV) and DDR technology.

  @param[in]      MrcData  - Include all MRC global data.
  @param[in]      Param    - Parameter to get the max possible offset for.

  @retval UINT32 variable the maximal possible offset.
**/
UINT8
GetVrefOffsetLimits (
  IN  MrcParameters *const MrcData,
  IN  UINT8                param
  );

/**
  This function checks to see if the margin parameter is a rank based parameter.  These tend to be
  things which are controlled on the DRAM Mode Register side.

  @param[in]  Param   - The parameter to check.  Must be from MrcMarginTypes.

  @retval BOOLEAN - TRUE if it is a rank based margin parameter.  Otherwise FALSE.
**/
BOOLEAN
IsPerRankMarginParam (
  IN UINT8                param
  );

/**
  This function will return the rank used to store the results based on the Rank Mask passed in.
  Results will be stored in the first Rank existing in the RankMask.

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  RankMask - Bit mask of Ranks being margined.

  @retval mrcSuccess
**/
extern
UINT8
GetRankToStoreResults (
  IN      MrcParameters *const MrcData,
  IN      UINT16               RankMask
  );

/**
  Relax RDRD same rank turnarounds on all channels.
  Used during early write training steps to avoid stressing reads.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Relax   - TRUE: relax the TAT values, FALSE: restore original values.
  @param[in] RdRdsg  - This parameter gets assigned inside the function before we relax the rd-rd timing.
  @param[in] RdRddg  - This parameter gets assigned inside the function before we relax the rd-rd timing.
**/
void
MrcRelaxReadToReadSameRank (
  IN  MrcParameters *const MrcData,
  IN BOOLEAN               Relax,
  IN OUT INT64      *const RdRdsg,
  IN OUT INT64      *const RdRddg
  );

/**
  Relax WRWR same rank turnarounds on all channels.
  Used during Jedec Write Leveling to leave enough space between writes during training

  @param[in] MrcData - Include all MRC global data.
**/
void
MrcRelaxWriteToWriteSameRank (
  IN  MrcParameters *const MrcData
  );

/**
  Convert # of femto seconds to # of tCK

  @param[in] MrcData        - Include all MRC global data.
  @param[in] TimeInFemto    - Time to convert to tCK.

  @retval The # of tCK.
**/
UINT32
MrcFemtoTimeToTCK (
  IN  MrcParameters *const MrcData,
  IN  UINT32               TimeInFemto
  );

/**
  This function configures the terminating ranks to the programmed FSP (FSP-OP = 1) to enable
  their CA ODT termination for the non-terminating ranks.  This is done across all LPDDR5 channels.

  @param[in]  MrcData - Pointer to the MRC global data.
  @param[in]  Enable  - Boolean parameter to enable(TRUE) or disable(FALSE) CA termination.

  @retval MrcStatus - mrcSuccess otherwise an error status.
**/
MrcStatus
MrcLpddrSetCbtCaOdtEn (
  IN  MrcParameters *const  MrcData,
  IN  BOOLEAN               Enable
  );

/**
  Convert the input timing value from picoseconds to nCK (number of clocks).
  Returns 0 if tCKmin is 0 to avoid divide by 0 error.

    @param[in] TimingInPs - Timing value in units of picoseconds.
    @param[in] tCKmin     - The memory clock period in units of femtoseconds.

    @return The input timing value in units of nCK
**/
UINT32
PicoSecondsToClocks (
  IN UINT32 TimingInPs,
  IN UINT32 tCKmin
  );

/**
  Determines which is the first populated Rank of each channel

   @param[in]  MrcData   - Pointer to the MRC global data.
   @param[out] FirstRank - Structure to hold which rank is first populated of each channel
**/
void
GetFirstRank (
  IN  MrcParameters *const MrcData,
  OUT UINT8         FirstRank[MAX_CONTROLLER][MAX_CHANNEL]
  );

/**
  Determines which is the last populated Rank of each channel

   @param[in]  MrcData   - Pointer to the MRC global data.
   @param[out] LastRank  - Array to hold which rank is last populated of each channel
**/
void
GetLastRank (
  IN  MrcParameters *const MrcData,
  OUT UINT8         LastRank[MAX_CONTROLLER][MAX_CHANNEL]
  );

/**
  Map CPU DQ Pins to DRAM DQ Pins for bit swizzling
  Main flow:
  Repeat for each of the 8 bits per DQ byte (total 8 iterations for both channels, for rank0 only):
  - Put DRAM into MPC RD DQ Calibration mode.
  - For Ddr5, set MR26 and MR27 to 0, and set the current DQ bit in both MR28 and MR29.
  This will result in a pattern of 0's on all but one bits per byte.
  - Issue a sequence of READ commands.
  - Locate the single DQ in each byte in DataTrainFeedback (using SenseAmp Mode).
  - Report error if more than one DQ pin toggles.
  - Report error if no active DQ pin found.
  Update the DQ mapping data structure.

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess if succeeded
**/
MrcStatus
MrcMapDqDqsSwizzle (
  IN MrcParameters *const MrcData
  );

/**
  LP DQS (Byte) Swizzle Training

  Iterate through DRAM bytes and use CPGC to identify the CPU to DRAM DQS Mapping
  This training should only be used for LP5

  The following is a breakdown of the algorithm code flow:
  1. Initialze DRAM DQ patterns to 0x0
  2. Initialze DRAM DQ invert patterns to 0x01 & 0xFE
  3. Issue CPGC RDRD test and read DatatrainFeedback for every Byte
  4. If number of 0's in Datatrainfeedback equals:
  1 : Indicates lower byte
  7 : Indicates upper byte
  5. Map out ChannelIn->DqsMapCpu2Dram with byte knowledge found above

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess if succeeded
**/
MrcStatus
LpByteSwizzleTraining (
  IN MrcParameters *const MrcData
  );

/**
  DDR5 x16 DQS (Byte) Swizzle Training

  Iterate through DRAM bytes using PDA and CPGC to identify the CPU to DRAM DQS Mapping
  This training should only be used for DDR5 x16 devices (x8 does not need additional swizzle training)

  The following is a breakdown of the algorithm code flow:
  1. Initialze DRAM DQ patterns (MR26 & MR27) to 0x0, and MR25 and MR30 to 0x0
  2. While each CpuByte is not accounted for iterate through each device
  a. PDA Select for the current device
  b. Initialze DRAM DQ invert patterns (MR28 & MR29) to 0x01 & 0xFE
  c. Issue CPGC RDRD test and read DatatrainFeedback for every Byte
  d. If number of 0's in Datatrainfeedback equals:
  0 : Byte not used
  1 : Indicates an lower byte
  7 : Indicates an upper byte
  e. Map out ChannelIn->DqsMapCpu2Dram with byte knowledge found above

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess if succeeded
**/
MrcStatus
Ddr5ByteSwizzleTraining (
  IN OUT MrcParameters *const   MrcData
  );

/**
  Setup Lp5 to prepare for MTG RdRd test
  This function also cleans up Lp5 registers after MTG RdRd test is complete.
  Input parameter Enable determines if we want to setup or clean up registers.

  @param[in] MrcData     - The MRC global data.
  @param[in] Enabled        -  Determines if we are enabling or disabling McMprConfig

  @retval void
**/
VOID
SetupReadMprConfig (
  IN MrcParameters *const MrcData,
  IN BOOLEAN Enable
  );

/**
  Update DqMapCpu2Dram array

  @param[in] MrcData     - The MRC global data.
  @param[in] Controller  - the controller to work on
  @param[in] Channel     - the channel to work on
  @param[in] Rank        - the Rank to work on
  @param[in] Feedback    - array of DATATRAINFEEDBACK values for all 8 bytes
  @param[in] Bit         - The DQ bit that should be set in each DRAM byte

  @retval mrcSuccess if succeeded
**/
MrcStatus
FillCA2DQMapResult (
  IN OUT MrcParameters *const   MrcData,
  IN const UINT32               Controller,
  IN const UINT32               Channel,
  IN const UINT32               Rank,
  IN const UINT8                Feedback[8],
  IN const UINT8                Bit
  );

/**
  Map CPU DQ Pins to DRAM DQ Pins for bit swizzling
  This training should only be used for DDR5 and LP5

  Main flow:
  Repeat for each of the 8 bits per DQ byte (total 8 iterations for both channels, for rank0 only):
  - Put DRAM into MPC RD DQ Calibration mode.
  - For Lp5, set MR32 and MR40 to 0, and set the current DQ bit in both MR15 and MR20.
  - For Ddr5, set MR26 and MR27 to 0, and set the current DQ bit in both MR28 and MR29.
  This will result in a pattern of 0's on all but one bits per byte.
  - Issue a sequence of READ commands.
  - Locate the single DQ in each byte in DataTrainFeedback (using SenseAmp Mode).
  - Report error if more than one DQ pin toggles.
  - Report error if no active DQ pin found.
  Update the DQ mapping data structure.

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess if succeeded
  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess if succeeded
**/
MrcStatus
BitSwizzleTriaining (
  IN OUT MrcParameters *const   MrcData
  );

/**
  Print DqMapCpu2Dram and DqsMapCpu2Dram array

  @param[in] MrcData     - The MRC global data.

  @retval void
**/
void
PrintDqDqsTable (
  IN OUT MrcParameters *const   MrcData
  );

/**
  Get the current SaGvPoint from the input MrcData

  @param[in] MrcData  - Pointer to global MRC data.

  @return The current SaGvPoint
**/
MrcSaGvPoint
MrcGetSaGvPoint (
  IN  MrcParameters *const MrcData
  );

/**
  This function looks up the CPU to DRAM Byte swizzle and then determines if this CPU byte will get
  CA Bus Training mode feedback from the DRAM.

  @param[in] MrcData    - Pointer to the MRC global data.
  @param[in] Controller - 0-based index instance to select.
  @param[in] Channel    - 0-based index instance to select.
  @param[in] Byte       - 0-based index instance to select.

  @retval BOOLEAN - TRUE if the instance gets DRAM feedback, otherwise FALSE.
**/
MRC_IRAM0_FUNCTION
BOOLEAN
MrcHasCbtFeedback (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Byte
  );

/**
  This function will return the value of the CA Parity Sampler in the DDRIO for the requested Controller, Channel, and Byte.
  The HW implementation on what parity level is sampled for the counter is configured in the DDRIO before running the test
  and reading the feedback interface.
  Controller, Channel, and Byte are in the logical (System level) definition of the DDR interface.

  @param[in] MrcData    - Pointer to the MRC global data.
  @param[in] Controller - 0-based index instance to select.
  @param[in] Channel    - 0-based index instance to select.
  @param[in] Byte       - 0-based index instance to select.

  @retval UINT8 - Count value between 0:255
**/
MRC_IRAM0_FUNCTION
UINT16
MrcDdrIoGetCaParityFeedback (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Byte
);
/**
  This function calculates center and final margins based on the sweep results

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     MinWidth       - Minimal eye width
  @param[in,out] SweepResult    - Structure to store liner search results
  @param[in,out] WorstMargin    - Structure to store which Bit has the worst centering margin
  @param[in]     StepSize       - Used to determine the correct checks for composite eye creation.
  @param[in]     Controller     - Controller to work with
  @param[in]     Channel        - Channel to work with
  @param[in]     Rank           - Rank to work with
  @param[in]     Byte           - Byte to work with
  @param[in]     Bit            - Bit to work with
  @param[in]     Scope          - Lowest Scope we want to print (Channel, Byte, Bit)
  @param[in]     DebugLevel     - Debug level

  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcGetMarginsAndCenter (
  IN OUT MrcParameters        *const  MrcData,
  IN     UINT8                        MinWidth,
  IN OUT VOID                 *const  SweepResult,
  IN OUT Worst1DPerBitMargin  *const  WorstMargin,
  IN     INT16                        StepSize,
  IN     UINT8                        Controller,
  IN     UINT8                        Channel,
  IN     UINT8                        Rank,
  IN     UINT8                        Byte,
  IN     UINT8                        Bit,
  IN     LinearSweepScope             Scope,
  IN     MrcDebugMsgLevel             DebugLevel
  );

/**
  This procedure prints header for Training Steps depending on Scope level
  Input NumTests must be 1 or greater. If NumTests is 0, function will error out.

  @param[in] MrcData        - Include all MRC global data.
  @param[in] DebugLevel     - DebugLevel
  @param[in] McChBitMask    - Determine which Channels to print
  @param[in] Rank           - Current operating rank
  @param[in] Scope          - Lowest Scope we want to print (Channel, Byte, Bit)
  @param[in] NumSpaces      - Number of spaces between the lowest scope.  Valid values are 0:19.
  @param[in] NumTests       - Defines how many consecutive headers to print out. Used for DDR5 x16 Readmpr. Default should be 1.
  @param[in] ErrCount       - Print Error Count Header

  @retval MrcStatus - mrcWrongInputParameter: NumSpaces was invalid
                    - mrcSuccess
*/
MRC_IRAM0_FUNCTION
MrcStatus
MrcPrintHeaderTestScope (
  IN OUT MrcParameters *const MrcData,
  IN     MrcDebugMsgLevel     DebugLevel,
  IN     UINT8                McChBitMask,
  IN     UINT8                Rank,
  IN     LinearSweepScope     Scope,
  IN     UINT8                NumSpaces,
  IN     UINT8                NumTests,
  IN     BOOLEAN              ErrCount
  );

/**
  Top level function which calls the function which  prints header for Training Steps
  Assumes number of tests is 1

  @param[in] MrcData        - Include all MRC global data.
  @param[in] DebugLevel     - DebugLevel
  @param[in] McChBitMask    - Determine which Channels to print
  @param[in] Rank           - Current operating rank
  @param[in] Scope          - Lowest Scope we want to print (Channel, Byte, Bit)
  @param[in] NumSpaces      - Number of spaces between the lowest scope.  Valid values are 0:19.
  @param[in] ErrCount       - Print Error Count Header

  @retval MrcStatus - mrcWrongInputParameter: NumSpaces was invalid
                    - mrcSuccess
*/
MRC_IRAM0_FUNCTION
MrcStatus
MrcPrintHeader (
  IN OUT MrcParameters *const MrcData,
  IN     MrcDebugMsgLevel     DebugLevel,
  IN     UINT8                McChBitMask,
  IN     UINT8                Rank,
  IN     LinearSweepScope     Scope,
  IN     UINT8                NumSpaces,
  IN     BOOLEAN              ErrCount
  );

/**
  This function determines the correct Rank Index to pass to RunIOTest based on
  IoTestType.
    IoTestType == CaParity
      Convert RankMask to an index in the case of CaParity.
      Return an error if more than 1 bit is set in RankMask.
    IoTestType != CaParity
      Returns MRC_IGNORE_ARG_8.

  @param[in]  MrcData    - Pointer to global MRC data.
  @param[in]  IoTestType - Test type from MrcDqPat.
  @param[in]  RankMask   - BitMask of Rank/Ranks under test
  @param[out] RankIndex  - Output buffer for storing the rank index calculated.

  @retval MrcStatus - mrcWrongInputParameter if RankIndex is NULL or RankMask has more than 1 bit set.
                    - mrcSuccess otherwise.
**/
MrcStatus
MrcCheckRankMaskPerDqPat (
  IN  MrcParameters *MrcData,
  IN  UINT8         IoTestType,
  IN  UINT8         RankMask,
  OUT UINT8         *RankIndex
  );

/**
  This function does linear search across the given sweep range

  @param[in,out] MrcData         - Include all MRC global data.
  @param[in]     param           - Pavameter for search. Algo is pretty generic except Reset for CmdV
  @param[in]     Step            - Step size
  @param[in]     SweepStart      - starting position for sweep
  @param[in]     SweepStop       - ending position for sweep
  @param[in]     McChBitMask     - Channel bit mask
  @param[in]     Rank            - Rank to test
  @param[in,out] SweepResultsBit - Structure to store liner search results on the bit level
  @param[in]     DebugLevel      - Debug level

  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcLinear1DSweep (
  IN OUT MrcParameters *const     MrcData,
  IN     const UINT8              param,
  IN     const INT8               Step,
  IN     const INT16              SweepStart,
  IN     const INT16              SweepStop,
  IN     UINT8                    McChBitMask,
  IN     UINT8                    Rank,
  IN OUT SweepResultsBit  *const  SweepResult,
  IN     MrcDebugMsgLevel         DebugLevel
  );

/**
  This procedure returns the minimal required eye height for a given training step.
  This information is subsequently used differently for making decisions, e.g., MRCFailure, PDA writing, etc.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     EarlyCentering - Is Step an Early Training step.

  return MinWidth UINT8
**/
MRC_IRAM0_FUNCTION
UINT8
MrcGetMinRequiredEyeWidth (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              EarlyCentering
  );

/**
  This function calculates out the final TxDqDelay and TxDqBitDelay for the per bit write timing centering

  @param[in] MrcData             - Include all MRC global data.
  @param[in,out] SweepResult     - Structure to store liner search results on the Bit level
  @param[in,out] FinalOffsets    - Structure to store final offsets for TxDqDelay and TxDqBitDelay
  @param[in] Rank                - Rank to work with
  @param[in] PrintLevel          - Print Level

  @retval MrcStatus   - If it succeeded, returns mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcCalculateWTCPBOffsets (
  IN    MrcParameters      *const MrcData,
  IN    SweepResultsBit    *const SweepResult,
  OUT   FinalWTCOffsets    *const FinalOffsets,
  IN    UINT8               Rank,
  IN    UINT8               PrintLevel
  );

/**
  This function calculates out the final RxDqsPDelay, RxDqsNDelay, RxDqsBitOffset, RxDqsBitDelay for the per bit read timing centering

  @param[in] MrcData             - Include all MRC global data.
  @param[in,out] SweepResultRdTP - Structure to store liner search results for RdTP on the Bit level
  @param[in,out] SweepResultRdTN - Structure to store liner search results for RdTN on the Bit level
  @param[in,out] FinalOffsets    - Structure to store final offsets for RxDqsPDelay, RxDqsNDelay, RxDqsBitOffset and RxDqsBitDelay
  @param[in] Rank                - Rank to work with
  @param[in] PrintLevel          - Print Level

  @retval MrcStatus   - If it succeeded, returns mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcCalculateRTCPBOffsets (
  IN    MrcParameters          *const MrcData,
  IN    SweepResultsBit        *const SweepResultRdTP,
  IN    SweepResultsBit        *const SweepResultRdTN,
  OUT   FinalRTCOffsets        *const FinalOffsets,
  IN    UINT8                   Rank,
  IN    UINT8                   PrintLevel
  );

/**
  Get all the data for all Ranks all Devs for specific controller, channel and Mr
  it calls to the specific function according to technology

  Important:
  The *NumMrData can different depending on the MR

  @param[in]     MrcData      - Pointer to global MRC data.
  @param[in]     Controller   - Controller to work on.
  @param[in]     Channel      - channel to work on.
  @param[in]     MrAddress    - MR Address
  @param[in,out] MrPdaData    - Array of Data that will be filled
  @param[in,out] NumMrData    - pointer that will contain # of data that filled in the array.

  @retval MrcStatus - mrcSuccess, otherwise an error status.
**/
MrcStatus
MrFillPdaMrsData (
  IN      MrcParameters   *MrcData,
  IN      UINT32          Controller,
  IN      UINT32          Channel,
  IN      MrcModeRegister MrAddress,
  IN OUT  UINT8           MrPdaData[MAX_PDA_MR_IN_CHANNEL],
  IN OUT  UINT8           *NumMrData
  );

/**
  Determines the Array Index of the leftmost Min Value and/or Max Value stored in an array.

  @param[in]  Array      - Pointer to Start of Array
  @param[in]  ArraySize  - Size of Array
  @param[out] MinIndex   - Pointer to return Index of the leftmost Min Value in Array
  @param[out] MaxIndex   - Pointer to return Index of the leftmost Max Value in Array

**/
void
MrcMinMaxIndexArray (
  IN  const INT32   *Array,
  IN  UINT8         ArraySize,
  OUT UINT32        *MinIndex,
  OUT UINT32        *MaxIndex
  );

/**
  This function finds the worst case eye per device

  @param[in,out] MrcData     - Include all MRC global data.
  @param[in]     Controller  - Controller to work with
  @param[in]     Channel     - Channel to work with
  @param[in]     Rank        - Rank to work with
  @param[in]     Dimm        - DIMM index used to get Device Width and Bus Width
  @param[in]     StepSize    - Used to determine the correct checks for composite eye creation.
  @param[in,out] SweepResult - Structure to store liner search results on the Byte level
  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
VOID
MrcGetPerDevice1DEye (
  IN OUT MrcParameters *const     MrcData,
  IN     UINT8                    Controller,
  IN     UINT8                    Channel,
  IN     UINT8                    Rank,
  IN     UINT8                    Dimm,
  IN     INT16                    StepSize,
  IN OUT SweepResultsByte  *const SweepResult
  );


/**
  This procedure is meant to handle Write Voltage centering, places WrV in the middle of the eye,
  using a very robust, linear search algorithm.

  @param[in,out] MrcData          - Include all MRC global data.
  @param[in]     StepSize         - Step size
  @param[in]     LoopCount        - loop count
  @param[in]     MsgPrintMask     - Serial debug output message enable mask
  @param[in]     IsEarlyCentering - Execute as early centering routine

  @retval        MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
WrVCentering1D (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          StepSize,
  IN     const UINT8          LoopCount,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              IsEarlyCentering
  );

/**
  This function returns Min/Max Value of RcvEn in QClk accross populated ranks.

  @param[in]  MrcData     - Pointer to global data structure.
  @param[in]  Controller  - Memory Controller Number within the processor (0-based).
  @param[in]  Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]  Strobe      - Dqs data group within the rank (0-based).
  @param[out] MinVal      - Return pointer for Minimum RcvEn Value.
  @param[out] MaxVal      - Return pointer for Maximum RcvEn Value.

  @retval Nothing
**/
void
MrcGetRecEnDelay (
  IN  MrcParameters *const MrcData,
  IN  UINT32         const Controller,
  IN  UINT32         const Channel,
  IN  UINT32         const Byte,
  OUT UINT32        *const MinVal,
  OUT UINT32        *const MaxVal
  );

/**
  This function will do a monotonic check on a variable length array of INT32's.  To be monotonic,
  slope between subsequent points must all have the same direction or 0.  A slope of 0 means two
  consecutive points are equalivent.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] ArrayList   - Pointer to a list of INT32's to preform the check upon.
  @param[in] ArrayLength - Number of entries in ArrayList.

  @retval mrcSuccess - The array is monotonic
          mrcFail - The array is not monotonic
          mrcWrongInputParameter - ArrayList is NULL or ArrayLength is less than 3
**/
MrcStatus
MrcMonotonicCheck (
  IN  MrcParameters *const MrcData,
  IN  INT32                *ArrayList,
  IN  UINT8                ArrayLength
  );

/**
  This function returns the timing value for Read/Write Preamble and Postamble.

  @param[in]  MrcData      - Pointer to global data structure.
  @param[in]  TimingType   - GetTimingRead for Read Preamble/Postable GetTimingWrite for Write Preamble/Postable
  @param[out] PreambleT    - Return pointer for Preamble Toggle time.
  @param[out] PreambleLow  - Return pointer for Preamble low time.
  @param[out] Postamble    - Return pointer for Postamble time.

  @retval MrcStatus - mrcSuccess if successful or an error status
**/
MrcStatus
MrcGetPrePostamble (
  IN  MrcParameters   *const MrcData,
  IN  TIMING_GET_TYPE  const TimingType,
  OUT UINT8           *const PreambleT,
  OUT UINT8           *const PreambleLow,
  OUT UINT8           *const Postamble
  );

/**
  This function maps a given Ch/Byte to a physical DataShared partition

  @param[in]  MrcData  - The global host structure
  @param[in]  Channel  - The channel to calculate
  @param[in]  Byte     - The byte to calculate
  @param[out] PartByte - The physical byte the input logical channel / byte maps to

  @retval Partition
**/
UINT8
MrcCalcDataPartition (
  IN  MrcParameters *const MrcData,
  IN  UINT8                Channel,
  IN  UINT8                Byte,
  OUT UINT8                *PartByte OPTIONAL
  );

/**
  This procedure finds the byte composite eye based on the incoming bit eye data

  @param[in]  MrcData          - Include all MRC global data.
  @param[in]  SweepResultsBit  - Structure to store liner search results on the Bit level
  @param[out] SweepResultsByte - Structure to store composite eye results on the Byte level
  @param[in]  McChBitMask      - Channel bit mask
  @param[in]  StepSize         - Used to determine if we are walking in a positive or negative direction
**/
VOID
MrcGetByteCompositeEye (
  IN  MrcParameters    *const MrcData,
  IN  SweepResultsBit  *const SweepResultBit,
  OUT SweepResultsByte *const SweepResultByte,
  IN  UINT8                   McChBitMask,
  IN  INT16                   StepSize
  );

/**
  This procedure finds the channel composite eye based on the incoming byte eye data

  @param[in]  MrcData             - Include all MRC global data.
  @param[in]  SweepResultsByte    - Structure to store liner search results on the Byte level
  @param[out] SweepResultsChannel - Structure to store composite eye results on the Channel level
  @param[in]  McChBitMask         - Channel bit mask
  @param[in]  StepSize            - Used to determine if we are walking in a positive or negative direction
**/
VOID
MrcGetChannelCompositeEye (
  IN  MrcParameters        *const MrcData,
  IN  SweepResultsByte     *const SweepResultByte,
  OUT SweepResultsChannel  *const SweepResultChannel,
  IN  UINT8                       McChBitMask,
  IN  INT16                       StepSize
  );

/**
  This function determines if the 1D sweep should continue.
    Step is positive:
      Current <= Stop
    Step is negative:
      Current >= Stop
  @param[in] Current
  @param[in] Stop
  @param[in] Step
**/
BOOLEAN
MrcLinear1DSweepIsNotDone (
  IN INT16 Current,
  IN INT16 Stop,
  IN INT8  Step
  );

/**
  This function maps a given Ch to a physical CCCShared partition

  @param[in]  MrcData - The global host structure
  @param[in]  Channel - The channel to calculate

  @retval Partition
**/
UINT8
MrcCalcCccPartition (
  IN  MrcParameters *const MrcData,
  IN  UINT8                Channel
  );

/**
  Switch LPDDR to high operating frequency:
   - Set FSP_OP = 1
   - Lock MC PLL at high frequency

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] DebugPrint - Enable/disable debug printing

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcLpddrSwitchToHigh (
  IN  MrcParameters *const  MrcData,
  IN BOOLEAN                DebugPrint
  );

/**
  Read the registers for the given Param into CR cache.
  This needs to be done before using ChangeMargin with certain parameters which use WriteOffsetUncached / ForceWriteOffsetUncached.

  @param[in]  MrcData - Global MRC data
  @param[in]  Param   - Margin parameter, see MrcMarginTypes

  @retval None
**/
VOID
MrcReadParamIntoCache (
  IN  MrcParameters *const MrcData,
  IN  UINT32         const Param
  );

/**
This function returns the CPU instance mapped to the requested DRAM instance.
This function will either return the Byte mapping level or Bit mapping level based on the parameters.

@param[in]     MrcData    - Pointer to MRC global data.
@param[in]     Controller - 0-based index selecting the instance to decode.
@param[in]     Channel    - 0-based index selecting the instance to decode.
@param[in]     Rank       - 0-based index selecting the instance to decode.
@param[in,out] Byte       - Pointer to 0-based index selecting the instance to decode.
@param[in,out] Bit        - NULL causes the function to return the Byte mapping.
Otherwise, 0-based index selecting the instance to decode.
**/
MrcStatus
MrcGetDramMapping (
  IN     MrcParameters *const MrcData,
  IN     UINT32               Controller,
  IN     UINT32               Channel,
  IN     UINT32               Rank,
  IN OUT UINT32               *Byte,
  IN OUT UINT32               *Bit   OPTIONAL
);

/**
  This function configures the settings in the Test Engine which do not change when running a Victim/Aggressor
  pattern walking through all CA pins as the victim for Command Bus Training Mode.

  @param[in] MrcData - Pointer to the MRC global data.
  @param[in] Rank    - 0-based index instance to configure.

  @retval MrcStatus - Status of subfunction calls.
                      mrcSuccess if no issues.
                      mrcWrongInputParameter if unsupported DDR type.
**/
MrcStatus
MrcCaParityTeConfig (
  IN     MrcParameters *const MrcData,
  IN     UINT32               Rank
  );

/**
  Reset DFI clock via MPTU ForceCkStop, to have it synced with PHY clocks.
  Do it on all MPTU's, and without a stepping check.

  @param[in] MrcData - Include all MRC global data.
**/
void
MrcResetDfiClkAll (
  IN MrcParameters *const MrcData
  );

/**
  This function determines the Non Target Rank values, including the Non Target Dimm Rank 0 and 1 and
  Target Dimm Non Target Rank.

  @param[in]      Rank               - Rank to check
  @param[in]      NtRankSelect       - Holds the Non Target Rank values

  @retval None
**/
VOID
GetNtRankSelection (
  IN  UINT32 Rank,
  OUT UINT8  NtRankSelect[MaxNtOdt]
  );

/**
  Set the following bits at first SAGV point before sending PM0 message:
   DDRPHY_MISC_PMA_SAUG_CR_PMCFG.PChannelEn     = 1 (0 for TC)
   PMA_MCMISCSSAUG_CR_PMMESSAGE.SkipRestoreCR   = 1
   PMA_MCMISCSSAUG_CR_PMMESSAGE.SkipRetentionCR = 1
  Set the following bit right after PLL is locked
   SAXG_Enable
   and poll for SAXG_Ready (enable case) / SAXGPwrGood (disable case)

  Set the above bits to the opposite values after sending PM14 for last SAGV point only (except PChannelEn and SkipRestoreCR).
  If SAGV is disabled then the bits are set and cleared at their respective locations within the same run.

  @param[in] MrcData      - Include all MRC global data.
  @param[in] IsPrePllLock - Defines which bits to enable based on when the function is called i.e. before or after PM0 message.
  @param[in] IsSet        - Decides whether we set or clear the bits accessed in this branch

  @retval mrcSuccess    - SAXG_Ready was set successfully / SAXGPwrGood was cleared successfully
  @retval mrcDeviceBusy - Timed out waiting for the SAXG_Ready / SAXGPwrGood
**/
MrcStatus
MrcPmCfgCrAccess (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsPrePllLock,
  IN BOOLEAN              IsSet
  );

/**
  Set Near End (NE) and Far End (FE) SRAM Power Gate setting

  @param[in] MrcData      - Include all MRC global data.
  @param[in] PGEnable     - Enable/Disable power gating

  @retval None
**/
VOID
MrcPowerGateUcssSrams (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              PGEnable
  );

/**
  This function will return the CA Parity status for the given byte.
  This function assumes Odd parity indicates failure.  If our counter sampled any Odd Parity feedback on DQ,
  it will be non-zero and will be marked as a FAIL.

  @param[in] MrcData    - Pointer to the MRC global data.
  @param[in] Controller - 0-based index instance to select.
  @param[in] Channel    - 0-based index instance to select.
  @param[in] Byte       - 0-based index instance to select.

  @retval UINT8 - 1 if FAIL, 0 if PASS.
**/
MRC_IRAM0_FUNCTION
UINT8
MrcCaParityByteErrorStatus (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Byte
  );

/**
  This function will return the CA Parity status for the entire channel.  All Bytes must pass for the channel to pass.
  The CA Parity status for the byte is provide by MrcCaParityByteErrorStatus().

  This function will handle DRAM specific behaviors for CA Training Mode for x8/x16 devices.

  @param[in] MrcData    - Pointer to the MRC global data.
  @param[in] Controller - 0-based index instance to select.
  @param[in] Channel    - 0-based index instance to select.

  @retval UINT8 - 1 if FAIL, 0 if PASS.
**/
MRC_IRAM0_FUNCTION
UINT8
MrcCaParityChannelErrorStatus (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel
  );

  /**
  This function returns the error count from DDRIO (CaParity) or Test Engine (MTG) register (!CaParity).
  This function will assert if called when ((Outputs->DQPat == CaPariy) && (LevelOfMargin != ChannelMarginLevel))

  @param[in] MrcData       - Include all MRC global data.
  @param[in] Controller    - Memory Controller to work on
  @param[in] Channel       - MC Channel to work on
  @param[in] Byte          - Byte to work on (OPTIONAL)
  @param[in] Bit           - Bit to work on (OPTIONAL)
  @param[in] LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)

  @retval The Error Status based on Test Engine (MTG) register
**/
MRC_IRAM0_FUNCTION
UINT64
GetBERGetErrors (
  IN     MrcParameters *  const MrcData,
  IN     UINT32           Controller,
  IN     UINT32           Channel,
  IN     UINT32           Byte, OPTIONAL
  IN     UINT32           Bit,  OPTIONAL
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

  /**
  This function returns the MaxByte and MaxBit settings based on LevelOfMargin.  Not always applicable for all loops in GetBER.

  @param[in]     MrcData       - Include all MRC global data.
  @param[out]    MaxByte       - Loop Max for Byte
  @param[out]    MaxBit        - Loop Max for Bit
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)
**/
MRC_IRAM0_FUNCTION
VOID
GetBERLoopMax (
  IN     MrcParameters *  const MrcData,
  OUT    UINT32  *MaxByte,
  OUT    UINT32  *MaxBit,
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

  /**
  This function returns the DoneMask based on LevelOfMargin.

  @param[in]     MrcData       - Include all MRC global data.
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)

  @retval DoneMask associated with TwoPass/TwoFail pointers.
**/
MRC_IRAM0_FUNCTION
UINT16
GetBERDoneMask (
  IN     MrcParameters *  const MrcData,
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

  /**
  This function determines the value of the next test point on the byte level and updates MinTested and MaxTested as appropriate.

  @param[in]     MrcData       - Include all MRC global data.
  @param[in,out] TwoPass       - Pointer to whether 2 contiguous passing points have been found for this location
  @param[in,out] TwoFail       - Pointer to whether 2 contiguous failing points have been found for this location
  @param[in,out] Margin        - Pointer to the value of the next test point for this location
  @param[in]     DoneMask      - The bitmask used in association with TwoPass/TwoFail pointers
  @param[in,out] MaxTested     - Pointer to the maximum value tested for this location
  @param[in,out] MinTested     - Pointer to the minimum value tested for this location
  @param[in]     IsPassing     - If point is passing or not

  @retval TRUE if this location is finished (found all 4 points) otherwise FALSE.
**/
MRC_IRAM0_FUNCTION
BOOLEAN
GetBERUpdateByteBitMargin (
  IN     MrcParameters *  const MrcData,
  IN OUT UINT16           *TwoPass,
  IN OUT UINT16           *TwoFail,
  IN OUT UINT16           *Margin,
  IN     UINT16           DoneMask,
  IN OUT UINT8            *MaxTested,
  IN OUT UINT8            *MinTested,
  IN     BOOLEAN          IsPassing
  );

  /**
  This function determines the value of the next test point on the channel level and updates MinTested and MaxTested as appropriate.

  @param[in]     MrcData       - Include all MRC global data.
  @param[in,out] TwoPass       - Pointer to whether 2 contiguous passing points have been found for this location
  @param[in,out] TwoFail       - Pointer to whether 2 contiguous failing points have been found for this location
  @param[in,out] Margin        - Pointer to the value of the next test point for this location
  @param[in,out] MaxTested     - Pointer to the maximum value tested for this location
  @param[in,out] MinTested     - Pointer to the minimum value tested for this location
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)

  @retval TRUE if this location is finished (found all 4 points) otherwise FALSE.
**/
MRC_IRAM0_FUNCTION
BOOLEAN
GetBERUpdateChannelMargin (
  IN     MrcParameters *  const MrcData,
  IN OUT UINT16           *TwoPass,
  IN OUT UINT16           *TwoFail,
  IN OUT UINT16           *Margin,
  IN OUT UINT8            *MaxTested,
  IN OUT UINT8            *MinTested,
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

  /**
  This function updates the ByteMask and ByteIndex based on LevelOfMargin

  @param[in]     MrcData       - Include all MRC global data.
  @param[in]     Byte          - Byte being worked on (Optional)
  @param[in,out] ByteMask      - Pointer to the Byte Mask for all LevelOfMargin
  @param[in,out] ByteIndex     - Pointer to the Byte Index for BitMarginLevel, Others are 0
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)
**/
MRC_IRAM0_FUNCTION
VOID
GetBERByteMaskAndIndex (
  IN     MrcParameters *  const MrcData,
  IN     UINT32           Byte,     OPTIONAL
  IN OUT UINT16           *ByteMask,
  IN OUT UINT32           *ByteIndex,
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

  /**
  This function Finds the max margin for the specific Controller/Channel and Byte (if ByteMarginLevel or BitMarginLevel)

  @param[in]     MrcData       - Include all MRC global data.
  @param[in]     Controller    - Memory Controller to work on
  @param[in]     Channel       - MC Channel to work on
  @param[in]     Byte          - Byte to work on (used only if LevelOfMargin is ByteMarginLevel or BitMarginLevel)
  @param[in]     RankRx        - Rank index.  0xFF causes all ranks to be considered.
  @param[in]     Sign          - Sign of the margins (0 - negative/min, 1 - positive/max).
  @param[in]     Param         - Parameter to find max margin for
  @param[in]     MaxMargin     - Current max margin value.
  @param[in]     DimmVrefParam - Whether is a DIMM Vref Param
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)

  @retval MaxMargin capable
**/
MRC_IRAM0_FUNCTION
UINT8
GetBERFindByteMax (
  IN     MrcParameters *  const MrcData,
  IN     UINT32           Controller,
  IN     UINT32           Channel,
  IN     UINT32           Byte,
  IN     UINT8            RankRx,
  IN     UINT8            Sign,
  IN     UINT8            Param,
  IN     UINT8            MaxMargin,
  IN     BOOLEAN          DimmVrefParam,
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

  /**
  This function determines if min/max saturation has occurred and mitigates if occurs.

  @param[in]     MrcData       - Include all MRC global data.
  @param[in,out] Errors        - Pointer to store the log8 of ErrCount for the 4 points
  @param[in]     Controller    - Memory Controller being worked on
  @param[in]     Channel       - MC Channel being worked on
  @param[in]     Byte          - Byte being worked on
  @param[in]     Sign          - Sign of the margins (0 - negative/min, 1 - positive/max).
  @param[in,out] TwoPass       - Pointer to whether 2 contiguous passing points have been found for this location
  @param[in,out] TwoFail       - Pointer to whether 2 contiguous failing points have been found for this location
  @param[in]     DoneMask      - The bitmask used in association with TwoPass/TwoFail pointers
  @param[in,out] ByteMax       - Pointer to the maximum value allowed for this location
  @param[in,out] LastPass      - Pointer to the last pass point for this location
  @param[in]     Margin        - Value of the current test point for this location of Byte/Bit
  @param[in]     Param         - Parameter being evaulated
  @param[in]     RankRx        - Rank index.  0xFF causes all ranks to be considered.
  @param[in]     MaxMargin     - The initial maximum value allowed for this location
  @param[in]     DimmVrefParam - Whether is a DIMM Vref Param
  @param[in]     IsPassing     - If point is passing or not
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)
**/
MRC_IRAM0_FUNCTION
VOID
GetBERSaturationChecks (
  IN     MrcParameters *  const MrcData,
  IN OUT UINT32           *Errors,
  IN     UINT32           Controller,
  IN     UINT32           Channel,
  IN     UINT32           Byte,
  IN     UINT32           Sign,
  IN OUT UINT16           *TwoPass,
  IN OUT UINT16           *TwoFail,
  IN     UINT16           DoneMask,
  IN OUT UINT8            *ByteMax,
  IN OUT UINT8            *LastPass,
  IN     UINT8            Margin,
  IN     UINT8            Param,
  IN     UINT8            RankRx,
  IN     UINT8            MaxMargin,
  IN     BOOLEAN          DimmVrefParam,
  IN     BOOLEAN          IsPassing,
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

  /**
  This function updates the MinTested/MaxTested/InitValue array based on the margin array (divide by 10)

  @param[in]     MrcData       - Include all MRC global data.
  @param[in]     Controller    - Memory Controller to work on
  @param[in]     Channel       - MC Channel to work on
  @param[in]     Rank          - Rank to work on
  @param[in]     Byte          - Byte to work on
  @param[in,out] margin        - Provides the starting point on a per byte basis (still 10x)
  @param[in,out] marginBit     - Starting point on a per bit basis
  @param[in,out] MaxTested     - Array storing the maximum tested (starting from margin array)
  @param[in,out] MinTested     - Array storing the minimum tested (starting from margin array)
  @param[in]     Sign          - Sign of the margins (0 - negative/min, 1 - positive/max).
  @param[in]     Param         - Parameter to get margins from margin array
  @param[in]     ByteMax       - Current max margin value.
  @param[in]     PerCh         - Whether this Param is Per Channel
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)
**/
MRC_IRAM0_FUNCTION
VOID
GetBERSetupMarginArray (
  IN     MrcParameters *  const MrcData,
  IN     UINT32           Controller,
  IN     UINT32           Channel,
  IN     UINT32           Rank,
  IN     UINT32           Byte,
  IN OUT UINT16           margin[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN OUT UINT16           marginBit[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS][MAX_EDGES],
  IN OUT UINT8            MaxTested[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS],
  IN OUT UINT8            MinTested[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS],
  IN     UINT8            Sign,
  IN     UINT8            Param,
  IN     UINT8            ByteMax,
  IN     BOOLEAN          PerCh,
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

/**
  This function determines if a DDR5 device has been iterated upon based on the incoming DeviceMask.
  This function takes into account byte swizzle and will appropriately detect the current device based on byte swizzle.
  If the incoming device is not part of the device mask, the function will set the device mask and return FALSE indicating
  that the device was not previously in the DeviceMask.
  If the incoming device is part of the device mask, the function will return TRUE.

  @param[in]      MrcData    - Include all MRC global data.
  @param[in]      Controller - Current Controller
  @param[in]      Channel    - Current Channel
  @param[in]      Rank       - Current Rank
  @param[in]      Byte       - Current Byte
  @param[in,out]  DeviceMask - Hold the DeviceMask which will be programmed if the current device is not already part of DeviceMask
  @param[out]     Device     - Returns the device in which the incoming byte is part of
  @param[in]      DebugPrint - Enable/disable debug printing

  @retval TRUE if current Device is already part of DeviceMask
  @retval TRUE if current Device is not already part of DeviceMask
**/
BOOLEAN
HasDeviceBeenSeenDdr5 (
  IN     MrcParameters      *MrcData,
  IN     UINT32             Controller,
  IN     UINT32             Channel,
  IN     UINT32             Rank,
  IN     UINT32             Byte,
  IN OUT UINT8              *DeviceMask,
  OUT    UINT8              *Device,
  IN     BOOLEAN            DebugPrint
  );

/**
  This function determines if multicasting should be used based on input parameters.
  This function returns through parameters the start and stop range of Socket, Controller, Channel, Rank, Strobe, Lane based on if the input paramters suggest multicating should be used
  The *Range input paramters are optional since not all cases may need to use them all. If the *Range paramters are NULL, than they will not be programmed in the function
  If the 'Group" parameter is not needed, input GsmGtMax

  @param[in]      MrcData          - Pointer to global data structure.
  @param[in]      Socket           - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller       - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel          - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank             - Rank number within a channel (0-based).
  @param[in]      Strobe           - Dqs data group within the rank (0-based).
  @param[in]      Lane             - Lane index within the GSM_GT group (0-based).
  @param[out]     SocketRange      - Start/Stop sweep range for Socket
  @param[out]     ControllerRange  - Start/Stop sweep range for Controller
  @param[out]     ChannelRange     - Start/Stop sweep range for Channel
  @param[out]     StrobeRange      - Start/Stop sweep range for Strobe
  @param[out]     LaneRange        - Start/Stop sweep range for Lane

  @param[out] MulticastAccess - TRUE if multicasting is desired
**/
MRC_IRAM0_FUNCTION
BOOLEAN
MrcDetectMulticast (
  IN      MrcParameters *const  MrcData,
  IN      GSM_GT         const  Group,
  IN      UINT32                Socket,
  IN      UINT32                Controller,
  IN      UINT32                Channel,
  IN      UINT32                Rank,
  IN      UINT32                Strobe,
  IN      UINT32                Lane,
  OUT     MRC_RANGE             *SocketRange,       OPTIONAL
  OUT     MRC_RANGE             *ControllerRange,   OPTIONAL
  OUT     MRC_RANGE             *ChannelRange,      OPTIONAL
  OUT     MRC_RANGE             *RankRange,         OPTIONAL
  OUT     MRC_RANGE             *StrobeRange,       OPTIONAL
  OUT     MRC_RANGE             *LaneRange          OPTIONAL
  );

  /**
  Sweep all MC/Channel/Byte to determine if all 4 points have been found in TwoPass/TwoFail arrays

  @param[in]     MrcData       - The global MrcData
  @param[in,out] TwoPass       - Array of whether 2 contiguous passing points have been found
  @param[in,out] TwoFail       - Array of whether 2 contiguous failing points have been found
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)

  @retval TRUE if all MC/Channel/Byte found all 4 points otherwise FALSE.
**/
MRC_IRAM0_FUNCTION
BOOLEAN
MrcGetBERTestingDoneCheck (
  IN     MrcParameters *  const MrcData,
  IN OUT UINT16           TwoPass[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM],
  IN OUT UINT16           TwoFail[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM],
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );

  /**
  This function determines whether the test point is passing or failing and whether it is the 1st 2 contiguous points within passing or failing.
  Errors array will be updated to whether based on the test point falling within which of these 4 points (2 points just in the passing and 2 points just in the failing).
  Keeps track of whether the specific location (MC/Ch/Byte) has found TwoPass or TwoFail.

  @param[in]     MrcData       - Include all MRC global data.
  @param[in]     ErrCount      - Error status for the current location
  @param[in,out] Errors        - Pointer to the log8 of ErrCount for the 4 points
  @param[in]     Controller    - Memory Controller being worked on
  @param[in]     Channel       - MC Channel being worked on
  @param[in]     Byte          - Byte being worked on
  @param[in,out] TwoPass       - Pointer to whether 2 contiguous passing points have been found for this location
  @param[in,out] TwoFail       - Pointer to whether 2 contiguous failing points have been found for this location
  @param[in]     DoneMask      - The bitmask used in association with TwoPass/TwoFail pointers
  @param[in,out] LastPass      - Pointer to the last pass point for this location
  @param[in,out] MaxTested     - Pointer to the maximum value tested for this location
  @param[in,out] MinTested     - Pointer to the minimum value tested for this location
  @param[in]     Margin        - Value of the current test point for this location of Byte/Bit
  @param[in]     IsPassing     - If point is passing or not
  @param[in]     LevelOfMargin - The margin granularity (ChannelMarginLevel/ByteMarginLevel/BitMarginLevel)

  @retval TRUE if this location is finished (found all 4 points) otherwise FALSE.
**/
MRC_IRAM0_FUNCTION
BOOLEAN
GetBERCollectResults (
  IN     MrcParameters *  const MrcData,
  IN     UINT64           ErrCount,
  IN OUT UINT32           *Errors,
  IN     UINT32           Controller,
  IN     UINT32           Channel,
  IN     UINT32           Byte,
  IN OUT UINT16           *TwoPass,
  IN OUT UINT16           *TwoFail,
  IN     UINT16           DoneMask,
  IN OUT UINT8            *LastPass,
  IN OUT UINT8            *MaxTested,
  IN OUT UINT8            *MinTested,
  IN     UINT8            Margin,
  IN     BOOLEAN          IsPassing,
  IN     MRC_MARGIN_LEVEL LevelOfMargin
  );
/**
  Override Read to Read turnaround timings to support back to back reads.
  This function supports 2 modes:
    1. IsLpMrrOverride = 0: Ensures B2B traffic in case tCCD_S/tCCD_L is relaxed (e.g. in OC scenarios Input TAT overrides)
    2. IsLpMrrOverride = 1: For LP5 only, confiure MC to propery handle BL16 MRR data
  This function should be utilized at the minimum during Read Leveling, ReadMPR, and Swizzle detection
  This function also restores the TAT values CPGC RdRd test is complete.
  IsLpMrrOverride should be ignored for non-LP5 configs

  @param[in]      MrcData         - The MRC global data.
  @param[in,out]  RDRDsgPrevData  - Copy of RdRd delay for same bank group
  @param[in,out]  RDRDdgPrevData  - Copy of RdRd delay for different bank group
  @param[in]      Enabled         - Determines if we are enabling or disabling McMprConfig
  @param[in]      IsLpMrrOverride - TRUE if LP TAT values should be overridden to support BL16 MRR data
**/
VOID
ReadB2BTATOverride (
  IN MrcParameters *const MrcData,
  IN OUT UINT8  *RDRDsgPrevData,
  IN OUT UINT8  *RDRDdgPrevData,
  IN BOOLEAN     Enable,
  IN BOOLEAN     IsLpMrrOverride
  );

/**
  This function checks to make sure a timing parameters sweep range falls within certain limits.
  For the case of RdT, RdTP, RdTN, the sweep range needs to fit within the hardware limits.

  @param[in]  MrcData    - Pointer to global MRC data.
  @param[in]  param      - Current Timing Paramter to check limit for
  @param[in]  Controller - Current Controller
  @param[in]  Channel    - Current Channel
  @param[in]  Rank       - Current Rank
  @param[in]  Byte       - Current Byte
  @param[in]  Bit        - Current Bit OPTIONAL
  @param[in out]  Limits - Structure to hold the start and stop sweep limits
  @param[in]  SweepStart - Initial sweep starting location
  @param[in]  SweepStop  - Initial sweep stopping location
**/
VOID
DetermineSweepLimits (
  IN OUT MrcParameters* const MrcData,
  IN     const UINT8          param,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                Rank,
  IN     UINT8                Byte,
  IN     UINT8                Bit, OPTIONAL
  IN OUT INT16                Limits[MAX_EDGES],
  IN     INT16                SweepStart,
  IN     INT16                SweepStop
);

/**
  This Function configures DDR5 CKD Control Word Registers.

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval mrcSuccess - CKD Control words properly configured.
**/
MrcStatus
MrcDdr5CkdConfig (
  IN MrcParameters *const MrcData
  );

/**
  Obtain CKD Smbus Address through SPD Smbus Address.

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcFail on failure, otherwise mrcSuccess.
**/
MrcStatus
MrcSetupCkdAddress (
  MrcParameters    *const MrcData
  );

/**
  This function gets the current CKD Control Word value from the CkdBuffer.

  @param[in] MrcData    - Pointer to global data.
  @param[in] CkdDimm    - CKD Dimm Number.
  @param[in] Offset     - Register offset.

  @retval The current CKD Control Word value, either from the CkdBuffer or from hardware if CkdBuffer doesn't exist.
**/
UINT8
MrcCkdBufferRead (
  IN MrcParameters *const MrcData,
  IN const UINT8   CkdDimm,
  IN const UINT32  Offset
  );

/**
  This function sets the current CKD Control Word value and updates corresponding CKD Buffer.

  @param[in] MrcData  - Pointer to global data.
  @param[in] Mode     - Bit-field flags controlling Get/Set.
  @param[in] CkdDimm  - CKD Dimm Number.
  @param[in] Offset   - Register offset.
  @param[in] Data     - New Control Word value.

  @retval mrcSuccess - If Write was successful
  @retval mrcWrongInputParameter - Offset is outside CKD register space.
**/
MrcStatus
MrcCkdBufferWrite (
  IN MrcParameters *const MrcData,
  IN const UINT32  Mode,
  IN const UINT8   CkdDimm,
  IN const UINT32  Offset,
  IN const UINT8   Data
  );

/**
  Obtain CKD Output Pin number and CKD Dimm number based on MC/Channel/Rank

  @param[in]  MrcData     - Pointer to global data.
  @param[in]  Controller  - Memory Controller Number.
  @param[in]  Channel     - Channel Number.
  @param[in]  Rank        - Rank Number.
  @param[out] CkdDimm     - CKD Dimm Number.
  @param[out] CkdPin      - CKD Pin Number.
**/
VOID
MrcCalcCkdDimmPin (
  IN MrcParameters      *const MrcData,
  IN     UINT32          const Controller,
  IN     UINT32          const Channel,
  IN     UINT32          const Rank,
  OUT    UINT32          *const CkdDimm, OPTIONAL
  OUT    UINT32          *const CkdPin   OPTIONAL
  );

/**
  Check CKD Clock:
  - no more than two instances of the clock requested
  - must be in the same CKD DIMM
  @param[in] MrcData - Pointer to MRC global data.
  @retval mrcFail on failure, otherwise mrcSuccess.
**/
MrcStatus
MrcCkdCheckValidInstance (
  IN MrcParameters* const MrcData
  );

/**
  Obtain Physical MC/Channel/Rank based on based on PhyClockToCkdDimm

  @param[in]  MrcData     - Pointer to global data.
  @param[in]  Controller  - Memory Controller Number.
  @param[in]  Channel     - Channel Number.
  @param[in]  Rank        - Rank Number.
  @param[out] PhyController - Physical Controller that the CLK resides in
  @param[out] PhyChannel    - Physical Channel that the CLK resides in
  @param[out] PhyChannel    - Physical Rank that the CLK resides in

  @retval IsCkdSupported
**/
BOOLEAN
MrcGetDdr5ClkIndex (
  IN MrcParameters      *const MrcData,
  IN     UINT32          const Controller,
  IN     UINT32          const Channel,
  IN     UINT32          const Rank,
  OUT    UINT32         *const PhyController,
  OUT    UINT32         *const PhyChannel,
  OUT    UINT32         *const PhyRank
  );

/**
  This function returns the tADCmin/max based on frequency.

  @param[in]  Frequency  - The memory frequency.
  @param[out] tADCmin    - Min limit for tADC based on freq
  @param[out] tADCmax    - Max limit for tADC based on freq
**/
VOID
MrcGetDdr5Tadc (
  IN const  MrcFrequency  Frequency,
  OUT       UINT8         *tADCmin,
  OUT       UINT8         *tADCmax
  );

/**
  ECC byte only has 4 bits DQ
  This function check if the bit is:
      - lower nibble (bit 3:0) for MC0, MC1
  @param[in]      Controller  - Current MC
  @param[in]      Byte        - Current Byte
  @param[in]      Bit         - Current Bit
  @retval BOOLEAN - TRUE if current bit is within the nibble of ECC byte, non-ECC byte, or DQS bit
**/
BOOLEAN
MrcBitExist (
  IN    UINT32         Controller,
  IN    UINT32         Byte,
  IN    UINT32         Bit
  );

/**
  This function return the ErrStatus Mask based on byte
  ECC bytes only has 4 bit, so return 0x0F, else return 0xFF
  @param[in]  Byte - Byte to get ErrStatusMask
**/
UINT8
MrcErrStatusMask (
  UINT32 Byte
  );

/**
  This function return the BitMask based on byte
  ECC byte only has 4 bit, so return bitmask of lower nibble
  @param[in]  Byte - Byte to get BitMask
  @param[in]  Bit  - Bit to get BitMask
**/
UINT16
MrcGetBitMask (
  UINT32 Byte,
  UINT32 Bit
  );

/**
  This function return if bit is Start Bit within the byte
  return TRUE if is bit 0 for MC0, MC1
  @param[in]  Controller - Controller to get StartBit
  @param[in]  Byte       - Byte to get StartBit
  @param[in]  Bit        - Bit to check with StartBit
**/
BOOLEAN
MrcIsStartBit (
  UINT32 Controller,
  UINT32 Byte,
  UINT32 Bit
  );

/**
  This function return the ECC DataTrainFeedback Mask based on MC
  ECC byte uses lower nibble for MCO, MC1
  @param[in]  Controller - Controller to get FeedbackMask
**/
UINT32
MrcEccFeedbackMask (
  UINT32 Controller
  );

/**
Check if Dram Emphasis is supported by LPDDR5

@param[in] MrcData - Include all MRC global data.

@retval MrcStatus -  if it succeeds return mrcSuccess

**/
BOOLEAN
MrcIsDramEmphasisSupported(
  IN MrcParameters* const MrcData
);

/**
  This function returns the scope of the group type

  @param[in] MrcData - Include all MRC global data.
  @param[in] Param   - group type

  @retval GROUP_SCOPE_TYPE
**/
GROUP_SCOPE_TYPE
MrcGetScope (
  IN MrcParameters* MrcData,
  IN GSM_GT Param
  );

/**
  This function sets the bit-mask FailingChannelBitMask based on inputs Controller, Channel and Dimm value
  @param[in out]  MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
**/
VOID
MrcSetFailingChannelBitMask (
  IN OUT  MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Dimm
  );

/**
  This function sets the bit of InternalClocksOn in DataControl0 struct.

  @param[in out] DataControl0  - Pointer to DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT.
  @param[in]     Enable        - Bit value.

  @retval mrcSuccess - If Write was successful
**/
MrcStatus
MrcSetInternalClocksOn (
  IN OUT DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT *DataControl0,
  IN BOOLEAN Enable
  );

/**
  If there is a value match with any of the elements in the array, then the matched array index is returned.

  @param[in]  Value     - Value to be matched.
  @param[in]  Array     - Input array.
  @param[in]  ArraySize - Size of Array.

  @retval Array index if there is a value match else returns -1.
**/
INT8
MrcFindIndex (
  IN UINT16 Value,
  IN UINT16 Array[],
  IN UINT8 ArraySize
  );

/**
  In a given 1D array, finds the maximum value and returns it.

  @param[in]  Array - Input array.
  @param[in]  ArraySize - Size of Array.

  @retval Returns the maximum value from the input array.
**/
INT8
MrcFindMaxVal (
  IN INT8  Array[],
  IN UINT8 ArraySize
  );

/**
  Linear 1D algorithm for a given byte-level param.
  The requested param is centered, and bit-level margins are reported.
  This routine is used by HVM DQ Loopback.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     Param          - Params supported are RcvEna and WrDqsT
  @param[in]     StepSize       - Step size
  @param[in]     LoopCount      - loop count
  @param[in]     MsgPrintMask   - Serial debug output message enable.
  @param[in]     EarlyCentering - Execute as early centering routine
  @param[in]     RankBitMask    - Ranks to run on

  @retval        MrcStatus -  If succeeded, return mrcSuccess
**/
MrcStatus
LoopbackByteCentering1D (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          Param,
  IN     const UINT8          StepSize,
  IN     const UINT8          LoopCount,
  IN     UINT8                MsgPrintMask,
  IN     BOOLEAN              EarlyCentering,
  IN     UINT8                RankBitMask
  );

/**
  Report DQ loopback margin results into a DRAM0 array

  @param[in]  MrcData      - Include all MRC global data
  @param[in]  Task         - Loopback task to report
  @param[in]  PerBitResult - Per-bit margin results to report
**/
VOID
GetLoopbackTestMarginsResults (
  IN MrcParameters *const MrcData,
  IN LoopBackTasks        Task,
  IN SweepResultsBit     *PerBitResult
  );

/**
  Display MR value from the host struct
  @param[in] MrcData - Include all MRC global data.
  @param[in] MrAddr  - MR Address.
**/
extern
VOID
DisplayMRContentFromHost (
  IN MrcParameters *const MrcData,
  IN MrcModeRegister      MrAddr
  );

  /**
  Early Centering for RxDqsVoc
  @param[in,out] MrcData   - Include all MRC global data.
  @retval        MrcStatus -  If succeeded, return mrcSuccess
**/
MrcStatus
MrcEarlyRxDqsVocCentering1D (
  IN OUT MrcParameters *const MrcData
  );

/**
  Centering for RxDqsVoc
  @param[in,out] MrcData   - Include all MRC global data.
  @retval        MrcStatus -  If succeeded, return mrcSuccess
**/
MrcStatus
MrcRxDqsVocCentering1D (
  IN OUT MrcParameters *const MrcData
  );

#endif //_MrcCommon_h_
