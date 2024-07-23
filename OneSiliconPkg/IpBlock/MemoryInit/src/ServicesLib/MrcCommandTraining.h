/** @file
  Command training definitions.

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

///
/// Include files
///
#ifndef _MrcCommandTraining_h_
#define _MrcCommandTraining_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcCommon.h"
#include "MrcLpddr5Registers.h"

#define ECT_CLK_START (0)
#define ECT_CLK_STOP  (128)

#define ECT_CLK_STEP  (2)

#define ECT_CLK_LOOPS (ECT_CLK_STOP / ECT_CLK_STEP)

#define ECT_DQS_START    (-32)
#define ECT_DQS_STOP     (32)
#define ECT_DQS_STEP     (8)
#define ECT_MIN_WIDTH    (16)
#define PI_HIGH_LP5_CLK  (128)
#define PI_LOW_LP5_CLK   (-128)
#define PI_HIGH_LP5_CACS (64)
#define PI_LOW_LP5_CACS  (-64)
#define MRC_CMD_CENTER_NUM_POINTS (128)

#define MRC_SKIP_VREF    TRUE
#define MRC_NO_SKIP_VREF FALSE
#define MAX_DELAY_STEP_IN_CKD  (24)

#define MRC_120PERCENT_TOLERANCE   (120)
#define MRC_80PERCENT_TOLERANCE    (80)

typedef enum {
  MrcCaTrainingDisabled,
  MrcCaTrainingEnabled
} MrcCaTrainingKnob;

typedef enum {
  MrcCaParityDisabled,
  MrcCaParityEnabled
} MrcCaParityKnob;

typedef
UINT8
(*MRC_CHANNEL_ERROR_STATUS_FUNC) (
  IN  MrcParameters *MrcData,
  IN  UINT8          McChBitMask
  );

typedef enum {
  MrcCpsLone0,
  MrcCpsLone1,
  MrcCpsVa,
  MrcCpsMax
} MrcCaParityStress;

typedef enum {
  CaTrainingMode41,     ///< Enter CA training mode using MRW41
  CaTrainingMode48,     ///< Enter CA training mode using MRW48
  CaTrainingMode42      ///< Exit CA training mode using MRW42
} MrcCaTrainingMode;

#pragma pack(push, 1)
typedef struct _CCC_LINE {
  UINT32 CaHigh;
  UINT32 CaLow;
  UINT8  ChipSelect;
} CCC_LINE;

typedef enum {
  CsPiTableSelect,
  CaPiTableSelect
} CCC_TABLE_TYPE;

#pragma pack(pop)

/**
  This function performs early command training.
  Center CTL-CLK timing to allow subsequent steps to work

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus -  mrcSuccess if it succeeded
**/
extern
MRC_IRAM1_FUNCTION
MrcStatus
MrcEarlyCommandTraining (
  IN MrcParameters *const MrcData
  );

/**
  This function performs Late command training.
  Center CMD/CTL-CLK timing using complex patterns.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it's a success return mrcSuccess
**/
extern
MRC_IRAM1_FUNCTION
MrcStatus
MrcLateCommandTraining (
  IN MrcParameters *const MrcData
  );

/**
  Perform Command Voltage Centering.

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus - if it succeded returns mrcSuccess
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcCmdVoltageCentering (
  IN OUT MrcParameters *const MrcData
  );

/**
  Finds the largest passing region and centers Command/Control/Clock inside the passing region.

  If Clock iteration is selected, the passing region will allow wrapping around the edges and center
  in the combined region.  This is because Clock is a continuios toggling signal.

  @param[in] MrcData         - Include all MRC global data.
  @param[in] Iteration       - Determines which PI to shift.
  @param[in] Ranks           - Valid Rank bit mask
  @param[in] GroupMask       - which LPDDR groups to work on for CMD/CLK/CKE; not used for DDR3
  @param[in] IsSkipPrint     - BOOLEAN control to enable/disable priting
  @param[in] IsSkipVref      - BOOLEAN control to enable/disable 2 additional testing loops at +/- Vref Offsets.

  @retval Nothing
**/
MRC_IRAM1_FUNCTION
MrcStatus
CmdTimingCentering (
  IN MrcParameters *const MrcData,
  IN UINT8                Iteration,
  IN UINT8                Ranks,
  IN UINT8                GroupMask,
  IN BOOLEAN              IsSkipPrint,
  IN BOOLEAN              IsSkipVref
  );

/**
  Shift the CLK/CTL Timing
  Shift the CMD Timing
  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to update.
  @param[in] Channel    - Channel to shift
  @param[in] Offset     - Offset to shift by
  @retval Nothing
**/
extern
MRC_IRAM0_FUNCTION
void
ShiftChannelTiming (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN INT32                Offset
  );

/**
  Early CA / CS training for LPDDR.
  Main flow:
  1. Run Early 2D CS/CMD training
  2. Run CA vs. CLK training.
  3. Program DESWIZZLE_HIGH/LOW registers.
  4. Set DRAM to FSP-High for all Ranks; lock PLL to High Frequency.

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess if succeeded
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
EarlyCommandTrainingLpddr (
  IN MrcParameters *const MrcData
  );

/**
  Sweep right and left from the current point to find the margins.

  @param[in, out] MrcData             - Include all MRC global data.
  @param[in]      Iteration           - Determines which PI to shift
  @param[in]      McChBitMask         - Valid Channel bit mask flat across all controllers.
  @param[in]      RankMask            - Valid Rank bit mask
  @param[in]      GroupMask           - Valid Group bit mask
  @param[in]      PiStep              - Step size
  @param[in]      MaxMargin           - Max Margin allowed for the parameter. NULL if MaxMarginOverride is FALSE
  @param[in]      IsMaxMarginOverride - TRUE if want to override defualt max margin values
  @param[in]      DebugPrint          - Print debug messages or not

  @retval MrcStatus -  If it succeeds return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
CmdLinearFindEdgesLpddr (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Iteration,
  IN     UINT8                McChBitMask,
  IN     UINT8                RankMask,
  IN     UINT8                GroupMask,
  IN     INT16                PiStep,
  IN     UINT8                MaxMargin,
  IN     BOOLEAN              IsMaxMarginOverride,
  IN     BOOLEAN              DebugPrint
  );

/**
  Enter / exit LPDDR5 CA training mode:
  If ENABLE:
    - Send MRW to MR13 OP[0] = 1, Wait tMRD
    - Force CKE low
    - Switch PLL to High frequency
  Else // DISABLE
    - Switch PLL to Low frequency
    - Force CKE high, Wait tFC
    - Send MRW to MR13 OP[0] = 0.

  @param[in] MrcData      - The MRC global data.
  @param[in] ChBitMask    - Channels to work on.
  @param[in] Rank         - Ranks to work on.
  @param[in] Enable       - Enable or disable CBT.
  @param[in] CbtPhase     - CbtPhase for MR13 register in LP5

  @retval mrcSuccess if succeeded
**/
MRC_IRAM0_FUNCTION
MrcStatus
LpddrCommandTrainingMode (
  IN MrcParameters *const  MrcData,
  IN UINT8                 McChBitMask,
  IN UINT32                Rank,
  IN UINT8                 Enable,
  IN UINT8                 CbtPhase
  );

/**
  This function handles configuring the DDRIO for LPDDR5 CA training mode.
  This function will also program CaParity alongside Ca Training mode if IsCaParity is true
  When entering the training mode, it will save the current settings to the provided SaveBuffer.
  When exiting the training mode, it will restore the IO configuration from the provided SaveBuffer.
  The IO is setup to consider Even Parity as the passing response.

  @param[in]      MrcData             - Pointer to MRC global data.
  @param[in]      IsCaTrainingEnabled - TRUE enters the CA Training Mode, FALSE exits the CA Training Mode
  @param[in]      IsCaParity          - TRUE programs CaParity alongside Ca Training, FALSE ignores programming CaParity
  @param[in,out]  SaveBuffer          - Pointer for save/restore of the configuration.
**/
MRC_IRAM0_FUNCTION
MrcStatus
LpddrCaTrainingInitIo (
  IN  MrcParameters *const        MrcData,
  IN  BOOLEAN                     IsCaTrainingEnabled,
  IN  BOOLEAN                     IsCaParityEnabled,
  IN OUT LPDDR5_CA_TRAIN_IO_SAVE  *SaveBuffer
  );

/**
  This function handles configuring the DDRIO for CA Parity or previous operating state.

  @param[in]      MrcData     - Pointer to the MRC global data.
  @param[in]      McChBitMask - Channels to configure.  MC1 begins at bit MrcOutput.MaxChannels.
  @param[in]      IsEnabled   - Switch to control CBT mode state in the DRAM, IO Config State, and usage of SaveBuffer.
  @param[in, out] SaveBuffer  - Buffer for saving/restoring settings that are changed by the training config.

  @retval MrcStatus - mrcUnsupportedTechnology if the function does not have a configuration for the specifc DDR type.
                    - Error status from subfunction.
                    - mrcSuccess otherwise.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcCaParityVaIoMode (
  IN     MrcParameters *const MrcData,
  IN     UINT8                McChBitMask,
  IN     BOOLEAN              IsEnabled,
  IN OUT CA_PARITY_IO_SAVE    *SaveBuffer
  );

/**
  Early CS Training for DDR5

  Trains for optimal CtlGrpPi and DRAM VrefCS.

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess if succeeded
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcDdr5EarlyCsTraining (
  IN MrcParameters *const MrcData
  );

/**
  Early CA Training for DDR5

  Trains for optimal CmdGrpPi and DRAM VrefCA.

  @param[in] MrcData      - The MRC global data.
  @param[in] PiToCkdScale - Storage array for PiToCkdScale (Granularity of 100 multiple) for all DIMMs.

  @retval mrcSuccess if succeeded
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcDdr5EarlyCaTraining (
  IN MrcParameters *const MrcData,
  IN UINT32  PiToCkdScale[MAX_CONTROLLER][MAX_DDR5_CHANNEL][MAX_DIMMS_IN_CHANNEL]
  );

/**
  Early Command Training for DDR5

  Executes DDR5 CS and CA training.

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess if succeeded
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
EarlyCommandTrainingDdr5 (
  IN MrcParameters *const MrcData
  );

/**
  Issue VREFCS command using MRH (Mode Register Handler).

  @param[in] MrcData       - Include all MRC global data.
  @param[in] Controller    - the controller to work on
  @param[in] Channel       - The channel to work on
  @param[in] Rank          - The rank to work on
  @param[in] VrefCsOffset  - VREFCs offset
  @param[in] UpdateMrcData - Cache the results or not
  @param[in] DebugPrint    - When TRUE, will print debugging information
**/
MRC_IRAM1_FUNCTION
VOID
MrcDdr5SetVrefCsCachedOffset (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN INT32                VrefCsOffset,
  IN INT32                UpdateMrcData,
  IN BOOLEAN              DebugPrint
  );

/**
  This function switches Channel Error method based on the TestPattern of the point test.

  @param[in] MrcData     - Pointer to global MRC data.
  @param[in] McChBitMask - Channel bit mask across all memory controllers.
  @param[in] TestPattern - Used to select which method to get the channel error status: MrcDqPat.

  @retval UINT8 - Bit mask of error status per Channel across all memory controllers.
**/
MRC_IRAM1_FUNCTION
UINT8
MrcMcChErrorStatus (
  IN  MrcParameters *MrcData,
  IN  UINT8         McChBitMask,
  IN  UINT8         TestPattern
  );

/**
  This function calculate High and Low sweep borders for a given paramter.
  This function is used for LPDDR5 only

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[in]  Value     - Current trained value.
  @param[out] PiHigh    - Return pointer for High limit.
  @param[out] PiLow     - Return pointer for Low limit.
**/
MRC_IRAM1_FUNCTION
VOID
MrcUpdateRange (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT         const Group,
  IN  INT64                Value,
  OUT INT16                *PiLow,
  OUT INT16                *PiHigh
  );

/**
  This function calculate global High and Low borders for sweep range based in limits for WCK, CLK, RecEn and TxDq.
  This function is used for LPDDR5 only

  @param[in]  MrcData   - Pointer to global data.
  @param[out] PiHigh    - Return pointer for High limit.
  @param[out] PiLow     - Return pointer for Low limit.
  @param[in]  Print     - Flag to enable debug print.
**/
MRC_IRAM1_FUNCTION
VOID
CalculateMaxSweepRange (
  IN  MrcParameters *const MrcData,
  OUT INT16                PiLow[MAX_CONTROLLER][MAX_CHANNEL],
  OUT INT16                PiHigh[MAX_CONTROLLER][MAX_CHANNEL],
  IN  BOOLEAN              Print
  );
/**
  Use a linear search to find the edges between Low and High
    if WrapAround = 0: Look for largest passing region between low and high
    if WrapAround = 1: Look for largest passing region, including wrapping from high to low

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      Iteration     - Determines which PI to shift
  @param[in]      McChBitMask   - Valid Channel bit mask
  @param[in]      Rank          - 0-based index of rank to sweep.
  @param[in]      GroupMask     - which LPDDR groups to work on for CMD/CLK/CKE; not used for DDR3
  @param[in]      Low           - Low limit
  @param[in]      High          - High limit
  @param[in]      Step          - Step size
  @param[in]      WrapAllowed   - Determines the search region
  @param[out]     ChResults     - Pointer to data structure which stores largest passing start/end.
  @param[in]      SkipPrint     - Switch to enable or disable debug printing

  @retval MrcStatus -  If it succeeds return mrcSuccess
**/
MRC_IRAM1_FUNCTION
MrcStatus
MrcCccLinear1DSweep (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Iteration,
  IN     UINT8                McChBitMask,
  IN     UINT32               Rank,
  IN     UINT8                GroupMask,
  IN     INT16                Low,
  IN     UINT16               High,
  IN     UINT8                Step,
  IN     UINT8                WrapAllowed,
  OUT    SweepResultsChannel *ChResults,
  IN     BOOLEAN              SkipPrint
  );

/**
  This function returns the bit error status based on the DDRIO CA Parity Feedback.
  As feedback is only at byte level, all bits are either passing or failing.

  @param[in]  MrcData        - Pointer to the MRC global data.
  @param[in]  Controller     - Controller instance of request.
  @param[in]  Channel        - Channel instance of request.
  @param[out] BitErrorBuffer - Buffer to return bit error status of each byte.  Expected to be MAX 5 entries long in case of DDR5 + ecc.
**/
MRC_IRAM1_FUNCTION
VOID
MrcCaParityMcChBitErrorStatus (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  OUT INT64                *BitErrorBuffer
  );

/**
  This function gathers the CA Parity status for the channels in the McChBitMask, and returns
  an error status in the McCh bitmask format, where 1 is a pass and 0 is a fail.

  @param MrcData     - Pointer to MRC global data.
  @param McChBitMask - Bitmask of channels across all controllers.

  @return UINT8 - Bitmask of failing channels.
**/
MRC_IRAM1_FUNCTION
UINT8
MrcCaParityMcChErrorStatus (
  IN MrcParameters *const MrcData,
  IN UINT8                McChBitMask
  );

/**
  This function will check if we are LPDDR5 and switch to low frequency for safe PI programming
  before issuing CBT command, and return to the high frequency.  For DDR5, this function just
  issues the CBT command.

  If either of the subfunction flow fails, this function will stop the flow and return the error
  status as this means some FSM failed and is not recoverable.

   @param[in] MrcData     - Pointer to MRC global data.
   @param[in] McChBitMask - Channels to configure.  MC1 begins at bit MrcOutput.MaxChannels.
   @param[in] Rank        - 0-based index instance to configure.
   @param[in] CbtPhase    - Lp5CbtModePhaseRising / Lp5CbtModePhaseFalling.
   @param[in] IsEnabled   - Selects entering or exiting CBT mode.

   @retval MrcStatus - First error message from subfunctions.
**/
MRC_IRAM1_FUNCTION
MrcStatus
MrcLpSafeCaParityCbtEnter (
  IN MrcParameters *const MrcData,
  IN UINT8                McChBitMask,
  IN UINT32               Rank,
  IN LPDDR5_CBT_MODE_PHASE CbtPhase,
  IN BOOLEAN              IsEnabled
  );

/**
  This function returns a bit mask of all channels in which the rank exists.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] Rank    - 0-based index to check.

  @return UINT8 - Bit mask of populated channels across all controllers
**/
MRC_IRAM1_FUNCTION
UINT8
MrcChMaskForRank (
  IN MrcParameters *const MrcData,
  IN const UINT32         Rank
  );

/**
  This function returns the composite eye based on all the ranks defined by RanksSwept.
  The result is returned in the entry of SweepResults defined by ResultRank.

  @param[in]     MrcData      - Pointer to MRC global data.
  @param[in,out] SweepResults - Array of SweepResults MAX_RANK_IN_CHANNEL long.
  @param[in]     RanksSwept   - Bitmask of ranks that are to be analyzed for the composite result.
  @param[in]     ResultRank   - Rank index to store the results in SweepResults[].

  @retval MrcStatus - mrcWrongInputParameter if SweepResults is NULL or if ResultRank >= MAX_RANK_IN_CHANNEL or if RanksSwept only has < 2 ranks set.
                    - mrcStatus otherwise.
**/
MRC_IRAM1_FUNCTION
MrcStatus
MrcRankCompositeEyeForChannel (
  IN     MrcParameters       *const MrcData,
  IN OUT SweepResultsChannel        SweepResults[MAX_RANK_IN_CHANNEL],
  IN     UINT8                      RanksSwept,
  IN     UINT8                      ResultRank
  );

/**
  This function executes the CA Parity point test.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] Rank        - Rank to run the point test on.
  @param[in] McChBitMask - Combined Memory Controller's Channel bit mask.
  @param[in] IsCmdVC     - Determine if command voltage training is being executed.

  @retval MrcStatus - mrcSuccess or returned error status from subfunctions.
**/
MRC_IRAM1_FUNCTION
MrcStatus
MrcCaParityTest (
  IN MrcParameters *const MrcData,
  IN UINT8                Rank,
  IN UINT8                McChBitMask,
  IN BOOLEAN              IsCmdVC
  );

/**
  This function tracks the CA or CS Pi codes which Pass / Fail based on given Pass criteria.
  This function tracks the number of consecutive passes found from CA or CS Pi code sweeps.
  This function returns TRUE if the total number of consecutive passes meets a given pass threshold.

  This function is used for LPDDR5 only

  @param[in] MrcData                  - Pointer to global data.
  @param[in] CccTable                 - Enum input to select if the CS or CA sweep table should be used in deremining consecutive passes
  @param[in] CurPiCode                - Current CCC Pi code value being checked for pass / fail.
  @param[in] PassThreshold            - This value determines the minimum needed consecutive passes in order to return TRUE
  @param[in out] PassingPiCodes       - Array which tracks the consecutive Passing pi code values
  @param[in out] NumConsecutivePasses - Pointer which tracks the total number of pi codes which are part of the consecutive pass
  @param[in] RankMask                 - Mask to track current Ranks for a given channel
  @param[in] PassRankMask             - Mask used to track passing ranks that will be compared to RankMask to determine if the current test is a pass or fail

  @retval BOOLEAN - TRUE if the total number of consecutive passes meets the PassThreshold
**/
MRC_IRAM1_FUNCTION
BOOLEAN
Lp5EctConsecutivePassCheck (
  IN MrcParameters* const   MrcData,
  IN     CCC_TABLE_TYPE     CccTable,
  IN     UINT32             CurPiCode,
  IN     UINT32             PassThreshold,
  IN OUT UINT32             PassingPiCodes[],
  IN OUT UINT8              *NumConsecutivePasses,
  IN     UINT8              RankMask,
  IN     UINT8              PassRankMask
  );

/**
  This function sets CA/CS patterns for all ECT/LCT/CMD voltage training using MPTU for both LP5/DDR5.

  @param[in, out] MrcData       - Include all MRC global data
  @param[in]      IsCatg        - Selects whether the patterns are sent using 0 : DRAM_CMD register or 1: CATG. Ignored for DDR5.
  @param[in]      IsEct         - This function is used for ECT/LCT/CMDV. But the patterns are same for LCT and CMDV.
                                  This parameter distinguishes between ECT (fixed) vs. LFSR patterns.
  @param[in]      Pattern       - Select either Pattern = 0 : 0x2A or  Pattern = 1 : 0x55.  Ignored for DDR5.
  @param[in]      Rank          - Rank at which the pattern will be used during Sel phase.
**/
MRC_IRAM1_FUNCTION
VOID
SetCmdTest (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsCatg,
  IN BOOLEAN              IsEct,
  IN UINT8                Pattern,
  IN UINT32               Rank
  );

#endif // _MrcCommandTraining_h_
