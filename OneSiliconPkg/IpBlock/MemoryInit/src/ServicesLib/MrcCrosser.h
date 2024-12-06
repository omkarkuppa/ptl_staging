/** @file
    This file contains all the crosser training algorithm definitions.

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
#ifndef _MrcCrosser_h_
#define _MrcCrosser_h_

//
// Include files
//
#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcHalRegisterAccess.h"

#ifdef XTENSA
#include <CGreenMrcCommonTypes.h>
#else
#include <CBlueMrcCommonTypes.h>
#endif
#include "CMrcInterfaceGlobalTypes.h"


// Functional DCC defines
#define FUNC_DCC_CLK_WCK_COMP_CYCLES  (3)
#define FUNC_DCC_CLK_WCK_LARGE_CHANGE_CHECK  (FUNC_DCC_CLK_WCK_COMP_CYCLES - 1)
#define FUNC_DCC_CLK_WCK_SWEEP_RANGE  (10)
#define FUNC_DCC_DDR5_DQS_COMP_CYCLES  (4)
#define FUNC_DCC_DDR5_DQS_LARGE_CHANGE_CHECK  (FUNC_DCC_DDR5_DQS_COMP_CYCLES - 2)
#define FUNC_DCC_DDR5_DQS_SWEEP_RANGE  (15)

// Max Bytes storage needed
#define POWER_TRAINING_MAX_BYTES  (MAX_BYTE_IN_DDR5_CHANNEL * MAX_DDR5_CHANNEL)
#define MAX_BITS_FOR_OFFSET_TRAINING     (MAX_BITS) ///< for SOT offset training done for 8 bits

#define SOT_SELECT_RX                    (2)            ///  Amount of TrainingIdx cases for SenseAmpOffset, for the Rise and Fall
#define RX_RISE                          (0)            ///< Training Case Idx for Rise
#define RX_FALL                          (1)            ///< Training Case Idx for Fall

// Re-centering loop count
#define RE_CENTER_LOOP_COUNT (10)

// Re-centering loop count
#define CCC_RE_CENTER_LOOP_COUNT (10)

/// Power optimization re-centering step size
#define T_RECENTERING_STEP_SIZE (2)
#define V_RECENTERING_STEP_SIZE (3)

/// Power optimization final centering step size
#define V_FINAL_STEP_SIZE (2)

/// UPM/PWR increment value if margins are at or below the retrain limit.
#define MRC_UPM_PWR_INC_VAL (80)

/// Vref step size for the following: DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
#define MRC_COMP_VREF_STEP_SIZE   (191)

/// SenseAmplifier Wake-Up Time in pS
#define MRC_SENSE_AMP_WAKEUP_TIME (2000)

// Number of samples per lane for Sense Amp Offset Training
#define MRC_SENSE_AMP_SAMPLES_CNT (7)

/// SOT Spacing Macro
///   GroupIdx | Spaces | Trained
///   ---------|--------|-------------
///      0     |   9    | 8 DQs, 1 DQS
///      1     |   8    | 8 DQs
#define MRC_SOT_BIT_SPACE(x) (((x) == 0) ? ("         ") : ("        "))

/// We need to make one of the bits in the combined Scomp value the PC bit value. It can be any bit as long as we always use the same one everywhere, and don't overwrite any bits used to store the SComp values.
/// The combined Scomp value is split back into individual PC and Scomp values before being written to registers using the get/set methods.
#define SCOMP_PC_STORAGE_BIT_OFFSET (4)

#define DQ_TCO_COMP_STEP  (6)
#define CLK_TCO_CODE_STEP (5)
#define WCK_TCO_CODE_STEP (5)
#define VDDQ_STEP (10)
#define WR_DS_STEP (2)
#define WR_DS_STEP_COARSE (9)
#define WR_TXEQ_STEP (3)
#define CCC_DS_STEP (3)
#define CCC_SCOMP_STEP (3)
#define CCC_TXEQ_STEP (3)
#define RXDQ_ODT_STEP (2)
#define RXDQS_ODT_STEP (3)
#define DFE_TAP_1_2_STEP_COARSE (5)
#define DFE_TAP_3_4_STEP_COARSE (2)
#define DFE_TAP_1_2_STEP (3)
#define DFE_TAP_3_4_STEP (1)
#define DRAM_DFE_STEP_FINE   (1)
#define DRAM_DFE_STEP_MEDIUM (3)
#define DRAM_DFE_STEP_COARSE (5)
#define RXEQ_STEP (2)
#define RX_XTALK_STEP (1)
#define LP5_WCK_DCA_STEP (1)
#define TXDQ_DCC_STEP (1)
#define LP5_WCK_DCC_STEP (1)
#define DDR5_DQS_DCC_STEP (1)
#define DDR5_CLK_DCC_STEP (1)
#define RX_DFE_NUM (16)

#define MAX_SI_TRAINING_SWEEP_RANGE_BIT_PER_RANK_ARRAY (22) // More than this, and the XTensa will run out of memory during a per-bit sweep if the array is per-rank
#define MAX_SI_TRAINING_SWEEP_RANGE_BIT_PER_CHANNEL_ARRAY (MAX_RANK_IN_CHANNEL * MAX_SI_TRAINING_SWEEP_RANGE_BIT_PER_RANK_ARRAY) // More than this, and the XTensa will run out of memory during a per-bit sweep
#define MAX_SI_TRAINING_SWEEP_RANGE_BYTE_PER_RANK_ARRAY (MAX_SI_TRAINING_SWEEP_RANGE_BIT_PER_RANK_ARRAY * MAX_BITS) // More than this, and the XTensa will run out of memory during a per-byte sweep if the array is per-rank
#define MAX_SI_TRAINING_SWEEP_RANGE_BYTE_PER_CHANNEL_ARRAY (MAX_SI_TRAINING_SWEEP_RANGE_BIT_PER_CHANNEL_ARRAY * MAX_BITS) // More than this, and the XTensa will run out of memory during a per-byte sweep
#define MAX_PRINT_MARGIN_PARAMS (4) // Max prams SI training print function can handle

// Slew rate constants
#define SLEW_RATE_ENABLED (0)
#define CYCLE_LOCK (1)

// Number of read-write command margin failures.
#define NUM_RW_CMD_MARGIN_FAILURES (3)

// CCC margins are twice as large.
#define MAX_CCC_POSSIBLE_TIME (MAX_POSSIBLE_TIME * 2)

// 8 values can be set in each decap register
#define MAX_DECAP_VALUES (8)

// Max CCC TxEq limit
#define CCC_TXEQ_MAX (DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_CaTxEq_MAX)

// Min LPDDR/DDR CCC TxEq limits
#define LP_CCC_TXEQ_LOW_LIMIT (8)
#define DDR5_CCC_TXEQ_LOW_LIMIT (5)

// Number to divide the CCC DS min comp code by when calculating min CCC TxEq limits
#define CCC_TXEQ_COMP_DIVISOR (5)

// The max number of comps/comp vrefs we ever optimize at once
#define MAX_COMPS_OPTIMIZED (3)

// The max number of comp offsets we ever optimize at once
#define MAX_COMP_OFFSETS_OPTIMIZED (4)

// The array index in which we store the name of the optimized comp vref during comp optimization
#define OPTIMIZED_COMP_VREF_INDEX (0)

// The max number of comps we ever check for saturation at once
#define MAX_COMPS_CHECKED (3)

// The number of comp codes we reserve to prevent saturation during training
#define RESERVED_COMP_CODES (3)

// Right now, we can only run 1D and 2D optimizations
#define MAX_OPT_PARAM_LENGTH (2)

// Scale for linear normalized power
#define LINEAR_NORMALIZED_POWER_SCALE (1000)

// Margins are multiplied by this when returned from the margining functions
#define MARGIN_MULTIPLIER (10)

// test loopcount used to calculate UPMs
#define UPM_LOOPCOUNT (17)

// test loopcount used to calculate UPMs in Early TCO training
#define WEAK_LOOPCOUNT (8)

// Extra Comps for DqTargetR[0:1]
#define DQ_TARGET_R_INITIAL       (4)
#define DQ_TARGET_R_SUBSEQUENT    (2)

// Max buses checked (Rx/Tx/CCC)
#define MAX_MARGIN_CHECK (3)

// Number of ticks over which the margin rise/fall was seen
#define STRESS_SLOPE_DENOMINATOR (8)

// Speed delta over which 1 tick of eye width UPM rise/fall was seen, make this larger than reality to ensure top speed is always worst case.
#define SPEED_SLOPE_DENOMINATOR (1330) // Power_Training - Calibrate this

// Margin 2 ticks beyond the UPM limit in retraining
#define MARGIN_LIMIT_GUARDBAND (2)

/// Power optimization CCC loop count
#define CCC_OPT_PARAM_LOOP_COUNT (10)

/// Power optimization loop count
#define OPT_PARAM_LOOP_COUNT (12)

/// Power optimization cmd timing margining step size
#define CMD_T_MARGIN_STEP_SIZE (3)

// 2 places of decimal accuracy
#define TWO_DECIMAL_PLACES   (100)

// Max Bits storage needed
#define POWER_TRAINING_MAX_BITS  (POWER_TRAINING_MAX_BYTES * MAX_BITS)

extern const char *GlobalCompOffsetStr[];
extern const char *CompGlobalOffsetParamStr[];

///
/// Enumerations and Structs
///

// Up and Down
typedef enum {
  CompUp,
  CompDn,
  MaxCompDirections
} CompDirection;

typedef enum {
  ChessOdd,
  ChessEven,
  FullGrid,
  Random,
  CustomSR2,
  Reversed1D,
  DecapSweep
} TGRidModes;

typedef enum {
  RdOdtUpDn,
  WrDSUpDn,
  WrDSCmdUpDn,
  WrDSCtlUpDn,
  WrDSClkUpDn,
  SCompCmd,
  SCompCtl,
  MaxCompGlobalOffsetParam
} CompGlobalOffsetParam;

typedef enum {
  TcoCompAlgoEarlyPerByte,
  TcoCompAlgoEarlyPerBit,
  TcoCompAlgoPerByte,
  TcoCompAlgoPerBit,
  PerChannelPerByte,
  PerChannelPerBit,
  PerChannel,
  PerRankPerByte,
  PerRankPerBit,
  PerRank,
  PerSystem
} AlgoMode;

typedef enum {
  SotVrefSearchStage,
  SotVocSearchStage
} SenseAmpTrainingLsFeedbackStage; // LocalStub feedback stages

#define MAX_OPT_POINTS        (64)
#define MAX_OPT_POINTS_BIT    (16)
#define MAX_GRID_DIM          (2)
#define MAX_TRADEOFF_TYPES    (MAX_MARGINS_TRADEOFF + 1)
#define MAX_MARGINS_TRADEOFF  (4)

//
// Maximum number of tests in the test list of TrainDDROptParamCliff() routine
//
#define MAX_TESTS_OPT_PARAM_CLIFF (6)

#pragma pack (push, 1)
typedef struct {
  UINT16 EW;
} OptResult;

typedef struct {
  UINT16    Best;
  INT8      GuardBand;
  UINT8     Scale[MAX_TRADEOFF_TYPES];
  UINT32    MaxPost[MAX_TRADEOFF_TYPES];
  UINT32    MinPost[MAX_TRADEOFF_TYPES];
  UINT64    MaxR;
  UINT64    Result[MAX_OPT_POINTS];
  OptResult Margins[MAX_TRADEOFF_TYPES][MAX_OPT_POINTS];
} OptResultsPerByteBit;

typedef struct {
  INT16     Offset[MAX_CONTROLLER][POWER_TRAINING_MAX_BITS];
} OptOffsetChByteBit;

typedef struct {
  INT64               OrigRxTap0;
  INT64               OrigRxTap1;
  INT64               OrigRxTap2;
  INT64               OrigRxTap3;
  INT64               RxDqVrefOrig;
  INT64               DataOdtModeOrig;
  INT64               DisableOdtStaticOrig;
  INT64               DrvStatLegEnOrig;
  INT64               RcompOdtDnOrig;
  INT64               RcompOdtUpOrig;
  INT64               RcompDrvDnOrig;
  INT64               RcompDrvUpOrig;
  INT64               InternalClockOnOrig;
  INT64               DqsRFTrainingModeValue;
  UINT32              VcmTargetRatio;
  UINT32              VrefEdge[SOT_SELECT_RX][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS_FOR_OFFSET_TRAINING]; // serves as an output for the Vref search results. Used mainly for UTing
  INT32               VocEdge[SOT_SELECT_RX][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS_FOR_OFFSET_TRAINING]; // serves as an output for the Voc search results
} SenseAmpTrainingStateConfig;

#pragma pack (pop)

typedef struct {
  INT64  DqsOffsetNUI;
} SenseAmpTrainingOverride;

//***********************************************
//************** New EV Training ****************
//***********************************************

#define LPF3          (3)
#define LPF5          (5)
#define TOTAL_CURVES  (6)
#define TcoNum        (32)
#define TX_EQ_NUM     (32)
#define DFE_NUM_DDR5  (DDR5_DIMM_DFE_TAP1_RANGE + 11)
#define DFE_NUM_LP5   (LPDDR5_DIMM_DFE_TAP_RANGE + 1)

typedef struct {
  UINT16  OptLastParams[2];
  UINT16  Score;
} TrainingData;


/**
  This function implements Sense Amp Offset training.
  The algorithm sweeps Per-Bit Vref Offset and parks the SenseAmpOffset at the "last 1" setting before transitions to 0.

  Algorithm will account for speckling.
  Flag Error if:
    (LastOne-FirstZero) > 8
    Samp Offset -16 is Zero
    Samp Offset +15 is One

  @param[in,out] MrcData - Include all MRC global data.

  @retval MrcStatus - if it succeeded return mrcSuccess
**/
MRC_IRAM1_FUNCTION
extern
MrcStatus
MrcSenseAmpOffsetTraining (
  IN OUT MrcParameters *const MrcData
  );

/**
  Override NUI for SOT Vref Search

  @param[in]      MrcData  - Include all MRC global data.
  @param[in]      Enable   - Enable or disable the override
  @param[in, out] Override - Save/restore struct
**/
VOID
SenseAmpOffsetOverrideNui (
  IN MrcParameters           *const MrcData,
  IN BOOLEAN                        Enable,
  IN OUT SenseAmpTrainingOverride   *Override
  );

/**
  Use RxDqVrefOffsetR for VocFall

  @param[in] MrcData  - Include all MRC global data.
**/
VOID
SenseAmpOffsetUseVocRiseForFall (
  IN MrcParameters *const MrcData
  );

/**
  Program Voc with the average of VocRise and VocFall

  @param[in]      MrcData     - Include all MRC global data.
  @param[in]      Controller  - Controller to work on
  @param[in]      Channel     - Channel to work on
  @param[in]      Byte        - Byte to work on
  @param[in]      Bit         - Bit to work on
  @param[in]      Rank        - Rank to work on
  @param[in, out] VocRise     - VocRise value, will be updated with the average if needed
  @param[in, out] VocFall     - VocFall value, will be updated with the average if needed
**/
VOID
SenseAmpOffsetSetVocAverage (
  IN MrcParameters *const MrcData,
  IN UINT8                Controller,
  IN UINT8                Channel,
  IN UINT8                Byte,
  IN UINT8                Bit,
  IN UINT8                Rank,
  IN OUT INT64            *VocRise,
  IN OUT INT64            *VocFall
  );

/**
  This function looks at the margin values stored in the global data structure and checks
  WrT, WrV, RdT, and RdV to see if they are above the minimum margin required.

  @param[in, out] MrcData - MRC global data.

  @retval mrcSuccess if margins are acceptable.
  @retval Otherwise, mrcRetrain.
**/
MrcStatus
MrcRetrainMarginCheck (
  IN OUT MrcParameters *const MrcData
  );

/**
  Check Margin Limits

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus -  mrcSuccess if succeded
**/
MrcStatus
MrcMarginLimitCheck (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function calculates the average Rx ODT impedance for the given channel (in ohms).

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  Controller    - 0-based index to controller
  @param[in]  Channel       - 0-based index to legacy channel (x64 bit)
  @param[out] ImpedenceOhms - Pointer to impedance return value

  @retval mrcSuccess Successfully calculated impedance
**/
MrcStatus
CalcRxDqOdtAverageByteImpedance(
  IN  MrcParameters *const MrcData,
  IN  UINT8                Controller,
  IN  UINT8                Channel,
  OUT UINT16        *const ImpedenceOhms
  );

/**
  Print DIMM ODT values per DIMM.

  @param[in] MrcData  - Include all MRC global data.

  @retval none
**/
extern
void
MrcPrintDimmOdtValues (
  IN MrcParameters *const MrcData
  );

/**
  This function implements Dimm Ron training.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus -  if it succeeds return mrcSuccess
**/
extern
MrcStatus
MrcDimmRonTraining (
  IN MrcParameters *const MrcData
  );

/**
  DIMM DFE training for DDR5 / LPDDR5

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus -  if it succeeds return mrcSuccess
**/
MrcStatus
MrcDimmDFETraining (
  IN MrcParameters *const MrcData
  );

/**
  DIMM DFE training for LPDDR5 / DDR5

@param[in] MrcData  - Include all MRC global data.
@param[in] Param    - Dfe Tap by memory type
@param[in] Optimize   Add Convolution

@retval MrcStatus - if it succeed return mrcSuccess
**/
MrcStatus
DimmDFETraining (
  IN MrcParameters* const MrcData,
  IN UINT8          Param,
  IN BOOLEAN        Optimize
  );

/**
  This function wrap DimmODTCATraining routine.

  @param[in] MrcData  - Include all MRC global data.
  @retval MrcStatus - if it succeed return mrcSuccess
**/
extern
MrcStatus
MrcDimmOdtCaTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function is the DimmODTTraining routine.

  @param[in] MrcData     - Include all MRC global data.
  @retval MrcStatus      - if it succeed return mrcSuccess
**/
extern
MrcStatus
MrcDimmOdtTraining(
  IN MrcParameters *const MrcData
  );

/**
  This function is the 2D Dram ODT vs NT ODT Training routine.

  @param[in] MrcData  - Include all MRC global data.

  @retval MrcStatus - if it succeed return mrcSuccess
**/
MrcStatus
MrcDimmOdtNTTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function wrap DimmODTParkNomTraining routine.

  @param[in] MrcData  - Include all MRC global data.

  @retval MrcStatus - if it succeed return mrcSuccess
**/
extern
MrcStatus
MrcDimmOdtParkNomTraining(
  IN MrcParameters *const MrcData
  );

/**
  This function implements Read Equalization training.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MRC_IRAM1_FUNCTION
extern
MrcStatus
MrcReadEqTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function implements Read DQ ODT training.
  Optimize Read ODT strength for performance & power

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus -  mrcSuccess.
**/
MrcStatus
MrcReadODTTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function implements Read DQS ODT training.
  Optimize Read ODT strength for performance & power

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus -  mrcSuccess.
**/
MrcStatus
MrcReadDqsODTTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function implements Write (Transmitter) Equalization training.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MrcStatus
MrcWriteEqDsTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function implements the general training algorithm for DDR and IO parameters
  that impact margin and power.

  This function can train for power or for margin, and the function determines the mode as follows:
  PowerTraining: (NumTests <= MAX_TRADEOFF_TYPES) && (Scale[NumTests] != 0)
  else MarginTraining.

  The Parameters that are supported:
  [0: WrDS, 1: RdODT, 2: SComp, 3: TComp, 3: TxEq, 4: RxEq, 5: RxBias, 6: DimmOdt, 7: DimmOdtWr]

  @param[in,out] MrcData           - Include all MRC global data.
  @param[in,out] BestOff           - Structure containg the best offest and margins (values) for th Opt param.
  @param[in]     McChannelMask     - MCs and Channels to train
  @param[in]     RankMask          - Condenses down the results from multiple ranks
  @param[in]     OptParam          - Defines the OptParam Offsets (e.g. OptRdDqOdt, OptSComp..etc)
  @param[in]     OptParamLen       - Defines the size of OptParam[].
  @param[in]     GridMode          - Selects the way to sweep the params
  @param[in]     TestList          - List of margin params that will be tested (up to 4)
  @param[in]     NumTests          - The length of TestList[].
  @param[in]     TestScale         - List of the relative importance between the 4 tests
  @param[in]     UPMOptimize       - Optimize in FindOptimalTradeOff only for UPM limit for selected params, so if they pass UPM they do not affect the score.
  @param[in]     SweepStart        - Start point of sweeping the Comp values. If it equals NULL,
                                     query MrcOptParamMappingTable or Dimm parameter values.
  @param[in]     SweepStop         - Stop point of sweeping the Comp values. If it equals NULL,
                                     query MrcOptParamMappingTable or Dimm parameter values.
  @param[in]     LoopCount         - The number of loops to run in IO tests.
  @param[in]     CPGCAllRanks      - Run CPGC on all the specified ranks at once. Else, one rank at a time.
  @param[in]     NoPrint           - Switch to disable printing.
  @param[in]     SkipOptUpdate     - Switch to train but not update Opt settings. Not really supported any longer
  @param[in]     GuardBand         - Signed offset to apply to the Opt param best value.
  @param[in]     PatType           - Type of pattern the will be applied for optimization, trying to keep MrcDqPat definitions. If not specified, LFSR VA is used. Allowed values: [StaticPattern (Simple Pattern)]
  @param[in]     Recenter          - Types of re-centering you want done after the training is finished.
  @param[in]     RecenterLength    - length of Recenter array.
  @param[in]     SaveMargin        - The array for saving margins in for Byte. Can be declared to be only as large as needed to save memory.  Optional, mutually exclusive with SaveMarginBit
  @param[in]     SaveMarginBit     - The array for saving margins in for Bit. Can be declared to be only as large as needed to save memory.   Optional, mutually exclusive with SaveMargin

  @retval Nothing
**/
void
TrainDDROptParam (
  IN OUT MrcParameters *const MrcData,
  IN OUT OptOffsetChByteBit   *BestOff,
  IN     UINT8                McChannelMask,
  IN     UINT8                RankMask,
  IN     const UINT8          OptParam[],
  IN     UINT8                OptParamLen,
  IN     UINT8                GridMode,
  IN     const UINT8          *TestList,
  IN     UINT8                NumTests,
  IN     UINT8                *TestScale,
  IN     UINT8                UPMOptimize[MAX_TRADEOFF_TYPES],
  IN     const INT8           SweepStart[],
  IN     const INT8           SweepStop[],
  IN     UINT8                LoopCount,
  IN     BOOLEAN              CPGCAllRanks,
  IN     BOOLEAN              NoPrint,
  IN     BOOLEAN              SkipOptUpdate, // Not really supported any longer
  IN     INT8                 GuardBand,
  IN     UINT8                PatType,
  IN     const UINT8          *Recenter,
  IN     UINT8                RecenterLength,
  IN OUT UINT16               SaveMargin[][MAX_OPT_POINTS][MAX_CONTROLLER][POWER_TRAINING_MAX_BYTES],  OPTIONAL
  IN OUT UINT8                SaveMarginBit[][MAX_OPT_POINTS_BIT][MAX_CONTROLLER][POWER_TRAINING_MAX_BITS]  OPTIONAL
  );

/**
  This function implements Read ODT training.
  Optimize Read ODT strength for performance & power

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it succeded return mrcSuccess
**/
extern
MrcStatus
MrcReadODTTraining (
  IN MrcParameters *const MrcData
  );

/**
  Updates a given ch/Rank/byte/bit combination with a new value for OptParam
  @param[in,out] MrcData         - Include all MRC global data.
  @param[in]     Controller      - Controller index to work on.
  @param[in]     Channel         - Channel index to work on.
  @param[in]     Ranks           - Condenses down the results from multiple ranks
  @param[in]     Byte            - Byte index to work on.
  @param[in]     Bit             - Bit index to work on.
  @param[in]     OptParam        - Defines the OptParam Offsets.
  @param[in]     Off             - Offset
  @param[in]     UpdateHost      - Decides if MrcData has to be updated. Should always use TRUE for now,
                                   since many multi-D sweeps are setting params within the same registers.

  @retval Nothing
**/
VOID
UpdateOptParamOffset (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          Controller,
  IN     const UINT8          Channel,
  IN           UINT8          Ranks,
  IN     const UINT8          Byte,
  IN     const UINT8          Bit,
  IN           UINT8          OptParam,
  IN           INT16          Off,
  IN     const UINT8          UpdateHost
  );

/**
  Slightly penalize any Asymmetry in margin

  @param[in] NegEdge - Negative edge of the margin
  @param[in] PosEdge - Positive edge of the margin

  @retval p2p - Width/Height reduced by the asymmetric difference in margin.
**/
extern
UINT16
EffectiveMargin (
  IN const UINT16 NegEdge,
  IN const UINT16 PosEdge
  );

/**
  This function does a running average on Margins in two dimentional fashion.

  @param[in,out] Margins - Margins to average in a 1D array.
  @param[in]     MLen    - Determines the Y-Dimension lengths
  @param[in]     XDim    - Determines the X-Dimension lengths
  @param[in]     XMin    - Used to skip the first elements in the Margin when averaging.
  @param[in]     CScale  - Used to place more weight on the center point.

  @retval Nothing
**/
extern
void
RunningAverage2D (
  IN OUT UINT16    Margins[MAX_OPT_POINTS],
  IN     const UINT8  MLen,
  IN     const UINT8  XDim,
  IN     const UINT8  XMin,
  IN     const UINT8  CScale
  );

/**
  This function does a running average on Margins in two dimentional fashion.

  @param[in,out] Margins - Margins to average
  @param[in]     Test    - Selects the Margins to average
  @param[in]     MLen    - Determines the Y-Dimension lengths
  @param[in]     XDim    - Determines the X-Dimension lengths
  @param[in]     XMin    - Used to skip the first elements in the Margin when averaging.
  @param[in]     CScale  - Used to place more weight on the center point.

  @retval Nothing
**/
void
Fill2DAverage (
  IN     MrcParameters      *const MrcData,
  IN OUT UINT16    Margins[][MAX_OPT_POINTS],
  IN     const UINT8  Test,
  IN     const UINT8  MLen,
  IN     const UINT8  XDim,
  IN     const UINT8  XMin,
  IN     const UINT8  CScale
  );

/**
  This function takes in 2D array of Margins: MarginType / Parameter Index.
  This index to the array represents some arbitrary parameter value that we are optimizing.
  The function will look for up to MAX_TRADEOFF_TYPES entries to optimize on.
  OptResultByte will store the results of the optimization, and various other data.

  In addition to optimizing for margin, this function can also optimize for power.
  GoodPowerLimit is an array that sets level where power is more important than margin.
    i.e. Any points where ((Margin[0]>GoodPowerLimit[0]) & (Margin[1]>GoodPowerLimit[1]) & ... )
  To avoid overflow, this function will automatic scale margins to fit in UINT64

  @param[in]     MrcData          - The global MRC data structure.
  @param[in,out] OptResults       - Structure containing the optimized results.
  @param[in]     InputMargins     - Margins we are optimizing
  @param[in]     MarginsLength    - The length of InputMargins
  @param[in]     LenMargin        - The length of InputMargins we are optimizing (0 - LenMargin -1).
  @param[in]     Scale            - Controls the relative importance on Margins[0] vs. [1] ...
                                      ex: To make Margins[0] twice as important, set Scale = [1, 2, 2, 2, 2].
                                      Since the search optimizes the lowest margin, increasing [1:4] makes 0 more important.
                                      This function can be used to optimize only Margin[0] by setting Scale = [1, 0, 0, 0, 0].
  @param[in]     UPMOptimize      - Optimize only for UPM limit for selected params, so if they pass UPM they do not affect the score.
  @param[in]     EnSq             - Enables the square root term in the optimization functions to make the tradeoff steeper.
  @param[in]     AveN             - The number of points used for the averaging filter.
  @param[in]     IncEnds          - Controls if the endpoints are to be included.
  @param[in]     ScaleM           - Controls the scaling of the middle point in 1-D average filter.
  @param[in]     GoodPowerLimit   - The power limit above which we only trade-off for power and not margin.
  @param[in]     OptimizationMode - 0:    Returns first good margin limit point.
                                    1-4:  Return the first index that meets GoodPowerLimit and lowest power.
                                            OptimizationMode is power column index.
                                    5-99: Return the index that meets GoodPowerLimit and >= % of the Max Optimization result.
                                    >100: Returns the highest Optimization Result.
  @param[in]     GuardBand        - Signed offest to check if margin drop is acceptable.  Save good guardband
                                    in OptResultByte.

  @retval Nothing.
**/
extern
void
FindOptimalTradeOff (
  IN     MrcParameters      *const  MrcData,
  IN OUT OptResultsPerByteBit       *OptResults,
  IN     const UINT16               *InputMargins,
  IN     UINT8                      MarginsLength,
  IN     UINT8                      LenMargin,
  IN     const UINT8                Scale[MAX_TRADEOFF_TYPES],
  IN     const UINT8                UPMOptimize[MAX_TRADEOFF_TYPES],
  IN     UINT8                      EnSq,
  IN     UINT8                      AveN,
  IN     UINT8                      IncEnds,
  IN     UINT8                      ScaleM,
  IN     const UINT16               GoodPowerLimit[MAX_TRADEOFF_TYPES],
  IN     UINT8                      OptimizationMode,
  IN     INT8                       GuardBand
  );

/**
  General purpose function to optimize an abritray value, OptParam (see list above)
    OptParam is generally some timing number that impacts performance or power
    Expects that as OptParam gets smaller*, margins are flat until we hit a cliff
    This procedure defines a cliff as a reducution of 4 ticks in eye height/width
    * In the case of mcodts, higher values are actually worst
    To stress out the timing, xxDDR_CLK is shifted by +/- 15 PI ticks

  @param[in] MrcData         - Include all MRC global data.
  @param[in] OptParam        - Supports Turnaround Timings and ODT Start / Duration
  @param[in] TestList        - List of margin param to check to make sure timing are okay.
  @param[in] NumTests        - The size of TestList
  @param[in] Start           - Start point for this turn around time setting.
  @param[in] Stop            - Stop point for this turnaround time setting.
                                 Note that the Start/Stop values are the real values, not the encoded value
  @param[in] LoopCount       - Length of a given test (per rank)
  @param[in] Update          - Update the CRs and host structure with ideal values
  @param[in] MarginByte      - Byte level margins
  @param[in] ClkShifts       - Array of PI clocks to be shifted
  @param[in] NumR2RPhases    - Number of PI clock phases
  @param[in] rank            - rank to work on
  @param[in] RankMask        - RankMask to be optimized
  @param[in] GuardBand       - GuardBand to be added to last pass value (to be a bit conservative).

  @retval MrcStatus      - If it succeeds return mrcSuccess
**/
extern
MrcStatus
TrainDDROptParamCliff (
  IN MrcParameters *const MrcData,
  IN UINT8       OptParam,
  IN const UINT8 TestList[],
  IN UINT8       NumTests,
  IN INT8        Start,
  IN INT8        Stop,
  IN UINT8       LoopCount,
  IN UINT8       Update,
  IN UINT16      MarginByte[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN const INT8  *ClkShifts,
  IN UINT8       NumR2RPhases,
  IN UINT8       rank,
  IN UINT8       RankMask,
  IN UINT8       GuardBand
  );

/**
  Updates the value for following OptParamCliff variables:
  drrd2rd=0, ddrd2rd=1, drwr2wr=2, ddwr2wr=3, drrd2wr=4, ddrd2wr=5, drwr2rd=6, ddwr2rd=7,
  rdodtd=8, wrodtd=9, mcodts=10, mcodtd=11, rtl=12}

  @param[in,out] MrcData    - Include all MRC global data.
  @param[in]     Controller - Controller to update the specified parameter.
  @param[in]     Channel    - Channel to update the specified parameter.
  @param[in]     Byte       - Byte to update the specified parameter.
  @param[in]     OptParam   - Parameter to update.
  @param[in]     Off        - Value to offset the current setting.
  @param[in]     UpdateHost - Switch to update the host structure with the new value.
  @param[in]     SkipPrint  - Switch to skip debug prints.
  @param[in]     RankMask   - Bit mask of Ranks to update.

  @retval Nothing
**/
extern
void
UpdateTAParamOffset (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel,
  IN     const UINT8          Byte,
  IN     const UINT8          OptParam,
  IN     const INT8           Off,
  IN     const UINT8          UpdateHost,
  IN     const UINT8          SkipPrint,
  IN     const UINT8          RankMask
  );

/**
  This function applies an offset to the global compensation logic.
  Reruns Compensation and returns the new comp value

  @param[in,out] MrcData         - Include all MRC global data.
  @param[in]     Param           - Parameter defining the desired global compensation logic
  @param[in]     Direction       - Direction of the Parameter
  @param[in]     Offset          - Value to apply
  @param[in]     UpdateHost      - Decides if MrcData has to be updated

  @retval mrcSuccess Successfully computed new compensation value
**/
extern
MrcStatus
UpdateCompGlobalOffset (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          Param,
  IN     const UINT8          Direction,
  IN     const INT32          Offset,
  IN     const BOOLEAN        UpdateHost
  );

/**
  Programs Delay/Duration for the SenseAmp and MCODT based on RcvEn timing
  Provide GuardBand > 0 if needed to be more conservative in timing
  Main goal is to optimize power

  @param[in,out] MrcData   - Include all MRC global data.

  @retval Nothing
**/
extern
void
MrcUpdateSampOdtTiming(
  IN OUT MrcParameters *const MrcData
  );

/**
  Turns off unused portions of the DLL phase

  @param[in,out] MrcData - Include all MRC global data.

  @retval Nothing
**/
extern
void
DLLSegmentDisable(
  IN OUT MrcParameters * const MrcData
  );

/**
  This function Shifts the CMD timing.
  NOTE: ONLY one, ResetDDR or SelfRefresh can be set inside this function

  @param[in,out] MrcData     - Include all MRC global data.
  @param[in]     Ranks       - Parameter defining the desired global compensation logic
  @param[in]     offset      - per channel Value to shift picode for
  @param[in]     ResetDDR    - Do we reset DDR?
  @param[in]     SelfRefresh - Do we perform Self refresh?
  @param[in]     UpdateHost  - Determines if MrcData has to be updated

  @retval MrcStatus       - If it succeeds return mrcSuccess
**/
extern
MrcStatus
ShiftCh2Ch (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          Ranks,
  IN     INT16                offset[MAX_CONTROLLER][MAX_CHANNEL],
  IN     UINT8                ResetDDR,
  IN     const UINT8          SelfRefresh,
  IN     const UINT8          UpdateHost
  );

#ifdef MRC_DEBUG_PRINT
/**
  This function prints the Optimize margin result table

  @param[in] MrcData     - MRC data structure
  @param[in] ChMask      - Channels to print
  @param[in] ResultArray - Array with saved margin results
  @param[in] ResultArrayBit - Array with saved margin results for bits
  @param[in] TestNum     - Test index
  @param[in] OffsetsNum  - number of offsets
  @param[in] MidPoint    - Zero point
  @param[in] OptParam    - Used to convert to the Opt param string for printing
  @param[in] OptParam2   - Used to convert to the Opt param2 string for printing
  @param[in] Param       - Margin type to be printed.
  @param[in] PowerLimits - Power limits to print.
  @param[in] noPrint     - Used to skip printing.

  @retval Nothing
**/
extern
void
PrintResultTableByte4by24(
  IN MrcParameters   *MrcData,
  IN UINT8           McChMask,
  IN UINT16          ResultArray[][MAX_OPT_POINTS][MAX_CONTROLLER][POWER_TRAINING_MAX_BYTES],
  IN UINT8           ResultArrayBit[][MAX_OPT_POINTS_BIT][MAX_CONTROLLER][POWER_TRAINING_MAX_BITS],
  IN UINT16          TestNum,
  IN UINT8           OffsetsNum,
  IN UINT8           MidPoint,
  IN UINT8           OptParam,
  IN UINT8           OptParam2,
  IN UINT8           Param,
  IN UINT16          *PowerLimits,
  IN BOOLEAN         noPrint
  );

/**
  This function prints the Optimize margin result table

  @param[in] MrcData            - MRC data structure
  @param[in] Controller         - Controller to print
  @param[in] Channel            - Channel to print
  @param[in] Byte               - Byte to print
  @param[in] calcResultSummary  - Array with saved margin results
  @param[in] BestOff            - Pointer to the selected offsets
  @param[in] Param              - Margin type to print.
  @param[in] OffsetsNum         - number of offsets
  @param[in] Start              - Start offsets
  @param[in] Stop               - End offsets
  @param[in] OptParam           - List of optimization parameters
  @param[in] OptParamLen        - Number of optimization parameters
  @param[in] PowerLimits        - Power limits to print.
  @param[in] Dim                - Dimension
  @param[in] TestNum            - Test index
  @param[in] NumTests           - Number of tests
  @param[in] MaxBit             - Maximum number of Bits
  @param[in] Bit                - Bit being targeted
  @param[in] noPrint            - Used to skip printing.

  @retval Nothing
**/
void
Print2DResultTableChByte (
  IN MrcParameters      *MrcData,
  IN UINT8              Controller,
  IN UINT8              Channel,
  IN UINT8              Byte,
  IN OptResultsPerByteBit *calcResultSummary,
  IN OptOffsetChByteBit   *BestOff,
  IN MrcMarginTypes     Param,
  IN UINT8              OffsetsNum,
  IN INT8               *Start,
  IN INT8               *Stop,
  IN const UINT8        *OptParam,
  IN const UINT8        *OptParamLen,
  IN UINT16             *PowerLimits,
  IN UINT8              Dim,
  IN UINT16             TestNum,
  IN UINT8              NumTests,
  IN UINT8              MaxBit,
  IN UINT8              Bit,
  IN BOOLEAN            noPrint
  );
#endif // MRC_DEBUG_PRINT

/**
  This function returns the UPM or PWR limit value for the specified parameter

  @param[in] MrcData   - Pointer to MRC global data.
  @param[in] Param     - Margin type
  @param[in] LoopCount - CPGC loopcount
  @param[in] LimitType - Type of limit: UpmLimit or PowerLimit

  @retval Returns the UPM or PWR limit
**/
UINT16
MrcGetUpmPwrLimit (
  IN MrcParameters *const  MrcData,
  IN UINT8                 Param,
  IN INT16                 LoopCount,
  IN MRC_MARGIN_LIMIT_TYPE LimitType
  );

/**
  This function returns various data for the specified OptParam

  @param[in]  MrcData   - Pointer to MRC global data.
  @param[in]  Param     - Opt Param type
  @param[out] StepSize  - Step Size for power training
  @param[out] Start     - Start Offset
  @param[out] Stop      - Stop Offset
  @param[out] DimmVals  - Dimm Value Array pointer if it's Dimm OptParam

  @retval OptParamMapping * when finding the parameters. Otherwise, return NULL when errors happen
**/
const OptParamMapping *
OptParamGetMappingData (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 Param,
  OUT UINT8                 *StepSize,
  OUT INT16                 *Start,
  OUT INT16                 *Stop,
  OUT UINT16                **DimmVals
  );

/**
  This function returns the Start/Stop limits value for the specified parameter

  @param[in] MrcData   - Pointer to MRC global data.
  @param[in] Param     - Opt Param type
  @param[in] LimitType - Type of limit

  @retval Returns the Start or Stop limit
**/
INT8
OptParamLimitValue (
  IN MrcParameters *const MrcData,
  IN UINT8                Param,
  IN OptLimitType         LimitType
  );

/**
  This function will adjust the requested Limit Type of the margin parameter by the signed offset passed in.

  @param[in]  MrcData   - MRC global data.
  @param[in]  Param     - Margin parameter type to adjust.
  @param[in]  Offset    - The adjustment value.

  @retval UINT16 - The new value of Param[MRC_MARGIN_LIMIT_TYPE]
**/
UINT16
MrcUpdateUpmPwrLimits (
  IN OUT MrcParameters * const  MrcData,
  IN UINT8                      Param,
  IN INT16                      Offset
  );

/**
  Calculate Power for the selected Opt param using linear approximation

  @param[in] MrcData     - Include all MRC global data.
  @param[in] Scale       - Multiplication factor for margins and power to preserve decimal accuracy (usually 10 to preserve 1 decimal place)
  @param[in] Params      - Params that power should be estimated for
  @param[in] ParamValues - Param values that power should be estimated for
  @param[in] ParamsCount - Number of params that power should be estimated for

  @retval mrcSuccess
**/
MrcStatus
CalcSysPower(
  IN MrcParameters   *MrcData,
  IN UINT8           Scale,
  IN const UINT8     Params[],
  IN INT8            ParamValues[],
  IN UINT8           ParamsCount
  );

/**
  Normalizes the Power values to the Margins passed in Points2Calc.
  Assumes that power numbers are represented as lowest number is the lowest power,
  and inverts the scale such that highest number is the lowest power.  This is done
  before normalizing to margins.

  @param[in]     MrcData       - Include all MRC global data.
  @param[in]     Points2calc   - Data normalize power.
  @param[in]     ArrayLength   - Array length of Points2calc.
  @param[in]     LenMargin     - The length of inputMargins we are optimizing (0 - LenMargin -1).
  @param[in]     TestListSize  - Size of TestList/Scale

  @retval Nothing
**/
extern
void
NormalizePowerToMargins (
  IN     MrcParameters   *MrcData,
  IN     void            *Points2calc,
  IN     UINT8           ArrayLength,
  IN     UINT8           LenMargin,
  IN     UINT8           TestListSize
  );

/**
  This function prints out the Margin eye diagram for ParamT/ParamV.


  @param[in] MrcData      - Include all MRC global data.
  @param[in] Ranks        - Bit mask of Ranks to margin.
  @param[in] ParamT       - Time parameter to margin.
  @param[in] ParamV       - Voltage parameter to margin.
  @param[in] CmdIteration - Whether doing CLK/CMD/CTL (only used with ParamT == CmdT and ParamV == CmdV)
  @param[in] CmdGroupMask - Determines which CmdGrp to use (only used with CmdIteration == MrcIterationCmd)
  @param[in] Start        - Starting point for margining.
  @param[in] Stop         - Stopping point for margining.
  @param[in] Repeats      - Number of times to repeat the test to average out any noise.

  @retval Nothing
**/
extern
void
EyeMargin (
  IN MrcParameters *const MrcData,
  IN UINT8                Ranks,
  IN UINT8                ParamT,
  IN UINT8                ParamV,
  IN UINT8                CmdIteration,
  IN UINT8                CmdGroupMask,
  IN INT8                 Start,
  IN INT8                 Stop,
  IN UINT16               SearchLimits,
  IN UINT8                LoopCount,
  IN UINT8                Repeats
  );

/**
  This function optimize the digital offsets by reducing the digital
  offset and apply the difference to the global one.

  @param[in] MrcData          - Include all MRC global data.
  @param[in, out] RcompTarget - Array of Rcomp Resistors
  @param[in] Param            - Parameter defining the desired digital compensation offset. Valid parameters: WrDSUpDn, WrDSCmdUpDn, RdOdtUpDn

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
OptimizeCompOffset (
  IN MrcParameters *const MrcData,
  IN OUT UINT16           RcompTarget[MAX_RCOMP_TARGETS],
  IN const UINT8          Param
  );

/**
  This function optimize the digital offsets by reducing the digital
  offset and apply the difference to the global one for Scomp.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Param      - Parameter defining the desired digital compensation offset.

  @retval MrcStatus - mrcSuccess
**/
extern
MrcStatus
OptimizeSCompOffset (
  IN MrcParameters *const MrcData,
  IN const UINT8          Param
  );

/**
  This step performs Comp Offset optimization on the param list defined in this function.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess if all Comps was able to be optimized otherwise mrcFail.
**/
extern
MrcStatus
MrcOptimizeComp (
  IN MrcParameters *const MrcData
  );

/**
  This function calculates the percent of power saving from the power optimization steps and
  updates the proper registers in the PCU.  To get the correct base line for this calculation,
  this routing needs to run first time early in the training in order to update the MrcStruct
  with the base line.  After the power training steps, it will run again to get the actual
  percent of power saving.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - mrcSuccess

**/
extern
MrcStatus
MrcPowerSavingMeter (
  IN MrcParameters *const MrcData
  );

/**
  This function reads the selected comp code.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] OptParam - Parameter to read the relevant comp code.
  @param[in] UpDown   - 0 : up only. 1 : down only.

  @retval The selected comp code (or -1 if the comp is unused and thus invalid in this configuration)
**/
INT8
GetCompCode (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                OptParam,
  IN     BOOLEAN              UpDown
  );

/**
@todo
**/
BOOLEAN
GetParamsXYZ (
  IN     MrcParameters *const MrcData,
  IN OUT INT8*                ParamOff,
  IN     const UINT8          OptParamLen,
  IN     const UINT8          GridMode,
  IN     const UINT8          Index,
  IN     const INT8           *Start,
  IN     const UINT8          *ParamOffLen
  );

/**
  This function is used to train the Clk TCO Comp offset.

  @param[in] MrcData - Pointer to global MRC data.

  @retval - mrcSuccess
**/
MrcStatus
MrcClkTcoCompTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function is used to train the Wck TCO Comp offset.

  @param[in] MrcData - Pointer to global MRC data.

  @retval - mrcSuccess
**/
MrcStatus
MrcWckTcoCompTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function is used to train CMD/CTL Drive Strength and Equalization 2D.

  @param[in] MrcData - Pointer to global MRC data.

  @retval mrcSuccess - if it succeed return mrcSuccess
**/
MrcStatus
MrcCmdEqDsTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function is used to train CMD/CTL/CLK Slew Rate.

  @param[in] MrcData - Pointer to global MRC data.

  @retval mrcSuccess - if it succeed return mrcSuccess
**/
MrcStatus
MrcCmdSlewRate (
  IN MrcParameters *const MrcData
  );

/**
  This function is used to train RxVref Decap.

  @param[in] MrcData - Pointer to global MRC data.

  @retval mrcSuccess - if it succeed return mrcSuccess
**/
MrcStatus
MrcRxVrefDecapTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function does Functional Duty Cycle Training for DDR5 CLK

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DCC converges otherwise mrcFail.
**/
MrcStatus
MrcDccDdr5Clk (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function does Functional Duty Cycle Training for DATA DQ

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
MrcDccDataDq (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function does Functional Duty Cycle Training for DDR5 DQS

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DCC converges otherwise mrcFail.
**/
MrcStatus
MrcDccDdr5Dqs (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function does Functional Duty Cycle Training for LPDDR5 WCK

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DCC converges otherwise mrcFail.
**/
MrcStatus
MrcDccLpddr5Wck (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function does DRAM Duty Cycle Correction for LP5 WCK DCA

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DCC converges otherwise mrcFail.
**/
MrcStatus
MrcDccLp5WckDca (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function implements Read DCA for LP5
  @param[in] MrcData - Include all MRC global data.
  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MrcStatus
MrcDccLp5ReadDca(
  IN MrcParameters* const MrcData
  );

/**
  This function does Rx Cross-Talk Cancellation
  This should only be run on DDR5

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess - if it succeed return mrcSuccess
**/
MrcStatus
MrcDdr5RxXTalkCancellation (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function Calls for LP5 WrDS training

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MrcStatus
WriteDsTraining(
  IN MrcParameters* const MrcData
);

/**
  This function implements Read Equalization training.

  @param[in] MrcData - Include all MRC global data.
  @param[in] DfeTap  - DFE Tap : 0, 1, 2, 3

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MRC_IRAM1_FUNCTION
MrcStatus
MrcReadDfe (
  IN MrcParameters* const MrcData,
  IN GSM_GT         const Tap
  );
/**
This function is the Pre-Emphasis training routine for LP5x.

@param[in] MrcData  - Include all MRC global data.

@retval MrcStatus - if it succeed return mrcSuccess
**/
MRC_IRAM1_FUNCTION
MrcStatus
MrcLp5EmphasisTraining (
  IN MrcParameters* const MrcData
  );

/**
  This function implements CompOffset per byte training

  @param[in] MrcData - Include all MRC global data.
  @param[in] OptParam - OptRdDqOdt, OptWrDS, etc.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MRC_IRAM1_FUNCTION
MrcStatus
MrcCompOffsetTraining(
  IN MrcParameters* const MrcData,
  IN TOptParamOffset          const OptParam
  );


/**
  This function implements Read Equalization (CTLE) training.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Optimize  - Add Convolution.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MrcStatus
MrcReadDqCTLETraining(
  IN MrcParameters* const MrcData,
  IN BOOLEAN             Optimize
  );


/**
  This function is CTLE - Cap vs Res training routine.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Optimize  - Add Convolution.

  @retval MrcStatus - if it succeed return mrcSuccess
**/
MrcStatus
MrcCtleCapResTraining (
  IN MrcParameters* const MrcData,
  IN BOOLEAN             Optimize
  );

/**
  This function calls for CTLE training.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MrcStatus
MrcReadCTLETraining(
  IN MrcParameters* const MrcData
  );

/**
  This function is used to train the TxDq TCO Comp offset.

  @param[in] MrcData - Pointer to global MRC data.

  @retval - mrcSuccess
**/
MrcStatus
MrcTxDqTcoCompTraining (
  IN MrcParameters* const MrcData
  );

/**
  This function provides TxDq TCO Comp offset Training algorithm.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] AlgoMode  - run training in Early/Late Mode and Bit/Byte mode Early training will be done with low LC

  @retval MrcStatus - if it succeed return mrcSuccess
**/
MrcStatus
MrcTxDqTcoCompTrainingAlgo (
  IN MrcParameters* const MrcData,
  AlgoMode                AlgoMode
  );

/**
  This function is used to train the TxDq TCO Comp offset.

  @param[in] MrcData - Pointer to global MRC data.

  @retval - mrcSuccess
**/
MrcStatus
MrcTxDqTcoCompTrainingEarly (
  IN MrcParameters* const MrcData
  );

/**
  This function implements Write (Transmitter) Equalization training.

  @param[in] MrcData - Include all MRC global data.

**/
MrcStatus
WriteEqTraining(
  IN MrcParameters* const MrcData
  );

/**
  This function implements the  DimmODTTraining for LPDDR5.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MrcStatus
DimmOdtTrainingLP5(
  IN MrcParameters* const MrcData
  );

/**
  This function implements the  DimmODTTraining for DDR5.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MrcStatus
DimmOdtTrainingDDR5(
  IN MrcParameters* const MrcData
  );

/**
  This function implements Lpddr5 DimmRx Offset Calibration training.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it succeeds return mrcSuccess
**/
MrcStatus
MrcDimmRxOffsetCalibration(
  IN MrcParameters* const MrcData
  );
/**
  This function implements RttPark 1D only training for Ddr5

  @param[in] MrcData - Include all MRC global data.
  @param[in] OptParam - RttPark, DQS_Rtt_Park or any similar.

**/
MrcStatus
Ddr5RttPark1D (
  IN MrcParameters* const MrcData,
  IN UINT8          OptParam
  );

/**
  This function implements RttWr vs NT_Rtt only training for Ddr5

  @param[in] MrcData - Include all MRC global data.

**/
MrcStatus
Ddr5RttWr_RttNT_1dpc (
  IN MrcParameters* const MrcData
  );

/**
  This function approximates a non linear heaviside function using 5 linear (Y = Ax + B) curves.
  UPM stands for Units Per Milion
  The curves are defined by 3 parameters:
    A    - The curve's slope
    B    - Intercept with y axis.
    XMax - The maximal x value to which curve parameters apply. i.e., for x < x1 a = a1, b = b1.
  It should select a balanced wr/rd operating point with respect to UPM

  @param[in]  MrcData          - MRC host structure
  @param[in]  PostMargin       - Margin after training
  @param[in]  GoodPwrLimitPost - UPM limits for margins

  @retval  linear approximation function output
**/
UINT64
UPMFilter(
  IN MrcParameters* const MrcData,
  IN UINT32               PostMargin,
  IN UINT16               GoodPwrLimitPost
  );

/**
  This function provides Matrix Convolution calculation
  Get an Matrix, perform convolution using Convolution matrix and update result matrix

  @param[in]  InputArray  - Input array
  @param[out] OutputArray - Output array
  @param[in]  MatrixRow   - Number of rows in input/output arrays
  @param[in]  MatrixCol   - Number of columns in input/output arrays
  @param[in]  ColForSeek  - Number of columns that are actually used
  @param[in]  lpfarr      - Low Pass Filter array
  @param[in]  Size        - LPF array size
**/
VOID
MrcConvolution2D (
  UINT16* InputArray,
  UINT16* OutputArray,
  UINT16  MatrixRow,
  UINT16  MatrixCol,
  UINT16  ColForSeek,
  UINT8*  lpfarr,
  UINT8   Size
  );

/**
  This procedure is meant to handle basic per bit timing centering, places strobe in the middle of the data eye,
  for both read and write DQ/DQS using a very robust, linear search algorithm.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     StepSize       - Step size
  @param[in]     loopcount      - loop count
  @param[in]     MsgPrint       - Show debug prints
  @param[in]     EarlyCentering - Execute as early centering routine

  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
MrcStatus
WriteTimingPerBit1DCentering(
  IN OUT MrcParameters* const MrcData,
  IN     const UINT8          StepSize,
  IN     const UINT8          LoopCount,
  IN     UINT8                MsgPrintMsk,
  IN     BOOLEAN              EarlyCentering
);

/**
  1) Store current MRC settings pre-test for later restoration.
  2) Pad setup for SenseAmpOffset. Calculates Common Mode Percentages and configures comps.

  @param[in,out] MrcData - Include all MRC global data.
  @param[out] Config - Include MRC settings senseamp changes and Odt Mode.

  @retval N/A
**/
VOID
SenseAmpOffsetTrainingSetup(
  IN OUT MrcParameters* const MrcData,
  IN OUT SenseAmpTrainingStateConfig* Config
);

/**
  Restore previous MRC settings to pre SenseAmpOffset Training
  and clean up.

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Config - Include MRC settings senseamp changes and Odt Mode.

  @retval N/A
**/
VOID
SenseAmpOffsetTrainingTeardown(
  IN OUT  MrcParameters* const MrcData,
  IN     SenseAmpTrainingStateConfig* Config
);

/**
  Search for the Vref voltage per lane where the 0 to 1 transition for both Rx
  are equidistant from the center.

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Config - Include MRC settings senseamp changes and Odt Mode.

  @retval mrcSuccess if successful calibration else mrcFail
**/
MrcStatus
SenseAmpOffsetVrefSearch(
  IN OUT MrcParameters* const MrcData,
  IN     SenseAmpTrainingStateConfig* Config
);

/**
  Search for the offset code per lane for the 0 to 1 transition for both Rx

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Config - Include MRC settings senseamp changes and Odt Mode.

  @retval mrcSuccess if successful calibration else mrcFail
**/
MrcStatus
SenseAmpOffsetVocSearch(
  IN OUT MrcParameters* const MrcData,
  IN     SenseAmpTrainingStateConfig* Config
);

/**
  Set the RxVref code for all lanes for the given Controller/Channel/Byte.
  If MultiCast is enabled, the RxVref code will be casted to all Controller/Channel/Byte.

  @param[in,out] MrcData  - Include all MRC global data.
  @param[in] Controller   - Controller to update
  @param[in] Channel      - Channel to update
  @param[in] Channel      - Byte to update
  @param[in] LaneValue    - Array containing the Vref code for each lane
  @param[in] MultiCast    - Bool flag to enable/disable multicasting


  @retval N/A
**/
VOID
SenseAmpOffsetSetRxVrefCodeAllLanes(
  IN OUT MrcParameters* const MrcData,
  IN     UINT8    Controller,
  IN     UINT8    Channel,
  IN     UINT8    Byte,
  IN     UINT32   LaneValue[MAX_BITS_FOR_OFFSET_TRAINING],
  IN     BOOLEAN  MultiCast
);

/**
  Set the RxVoc (offset) codes for all lanes for the given Controller/Channel/Byte.
  If MultiCast is enabled, the RxVoc code will be casted to all Controller/Channel/Byte.

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Controller   - Controller to update
  @param[in] Channel      - Channel to update
  @param[in] Channel      - Byte to update
  @param[in] LaneValue    - 2D Array containing the Voc code for both Rx per lane
  @param[in] MultiCast    - Bool flag to enable/disable multicasting

  @retval N/A
**/
VOID
SenseAmpOffsetSetRxVocCodeAllLanes(
  IN OUT MrcParameters* const MrcData,
  IN     UINT8    Controller,
  IN     UINT8    Channel,
  IN     UINT8    Rank,
  IN     UINT8    Byte,
  IN     INT32    LaneValues[MAX_BITS_FOR_OFFSET_TRAINING][SOT_SELECT_RX],
  IN     BOOLEAN  MultiCast
);

/**
  Set the RxVoc codes for a specific lane.
  The offset code will be set for both receivers per lane.

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Controller   - Controller to update
  @param[in] Channel      - Channel to update
  @param[in] Channel      - Byte to update
  @param[in] VocCodes     - Voc codes to set for the two Rx
  @param[in] MultiCast    - Bool flag to enable/disable multicasting

  @retval N/A
**/
VOID
SenseAmpOffsetSetRxVocCodeLane(
  IN OUT MrcParameters* const MrcData,
  IN     UINT8    Controller,
  IN     UINT8    Channel,
  IN     UINT8    Rank,
  IN     UINT8    Byte,
  IN     UINT8    Bit,
  IN     INT32    VocCodes[SOT_SELECT_RX],
  IN     BOOLEAN  MultiCast
);

/**
  Collect all samples from the DataTrainFeedback registers.

  @param[in,out] MrcData    - Include all MRC global data.
  @param[in,out] LaneResult - Pointer to the data structure that holds the DataTrainFeedback values for both Rx on a byte level

  @retval N/A
**/
VOID
SenseAmpOffsetGetRxOffsetResults(
  IN OUT MrcParameters* const MrcData,
  IN OUT UINT32   LaneResult[SOT_SELECT_RX][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM]
);

/**
  Run comp if needed based on OptParam or if ForceComp is TRUE.

  @param[in] MrcData   - MRC global data.
  @param[in] OptParam  - Param type
  @param[in] ForceComp - Force the comp to run regardless of the param type

  @retval BOOLEAN - Whether a comp was performed or not
**/
BOOLEAN
ForceSystemRComp(
  IN MrcParameters* const MrcData,
  IN UINT8                OptParam,
  IN BOOLEAN              ForceComp
);

#endif // _MrcCrosser_h_

