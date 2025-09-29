/** @file
  This file contains functions to find and track DRAM Failing addresses for the
  Post Package Repair feature.

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
#ifndef _MrcPpr_h_
#define _MrcPpr_h_

#include "CMrcTypes.h"
#include "CMrcInterface.h"
#include "MrcCommon.h"
#include "MrcAmtPprInterface.h"

#define PPR_MAX_DETECTED_ERRORS       (0xF000)

typedef enum PPR_STATUS {
  pprSuccess = 0,
  pprFail = 1,
} PPR_STATUS;

typedef struct {
  INT64 SaveVal0;
  INT64 SaveVal1;
  INT64 SaveVal2;
} PPR_MC_SETUP_SAVE;

/**
  Enter Post Package Repair (PPR) to attempt to repair detected failed row.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] Controller  - Controller for detected fail row
  @param[in] Channel     - Channel for detected fail row
  @param[in] Rank        - Rank for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] Row         - Row for detected fail row
  @param[in] ByteMask    - Byte mask to repair for fail row

  @retval MrcStatus
**/
MrcStatus
Ddr5PostPackageRepair (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               BankGroup,
  IN UINT32               BankAddress,
  IN UINT32               Row,
  IN UINT16               ByteMask
  );

/**
  Enter Post Package Repair (PPR) to attempt to repair detected failed row.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] Controller  - Controller for detected fail row
  @param[in] Channel     - Channel for detected fail row
  @param[in] Rank        - Rank for detected fail row
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] Row         - Row for detected fail row

  @retval MrcStatus
**/
MrcStatus
LpDdr5PostPackageRepair (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               BankGroup,
  IN UINT32               BankAddress,
  IN UINT32               Row
  );

/**
  This function enables and runs Post Package Repair on detected fail rows.

  @param[in] MrcData     - Pointer to global MRC data.
  @param[in] Controller  - 0-based index to controller
  @param[in] Channel     - 0-based index to channel
  @param[in] Rank        - Physical rank index inside the dimm
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] Row         - Row for detected fail row
  @param[in] ByteMask    - Byte mask to repair for fail row

  @retval MrcStatus - mrcSuccess if the DelayType is supported.
**/
MrcStatus
MrcPostPackageRepair (
  IN  MrcParameters *const MrcData,
  IN  UINT32         Controller,
  IN  UINT32         Channel,
  IN  UINT32         Rank,
  IN  UINT32         BankGroup,
  IN  UINT32         BankAddress,
  IN  UINT32         Row,
  IN  UINT16         ByteMask
  );

/**
  Get status whether PPR resource is available

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
  @param[in] Rank           - Rank number
  @param[in] DeviceByte     - Byte number used to identify DRAM
  @param[in] BankGroup      - BankGroup number
  @param[in] BankAddress    - BankAddress number

  @retval TRUE if PPR resource available; FALSE if not available
**/
BOOLEAN
GetPprResourceAvailable (
  MrcParameters   *const    MrcData,
  UINT32                    Controller,
  UINT32                    Channel,
  UINT32                    Rank,
  UINT16                    DeviceByte,
  UINT8                     BankGroup,
  UINT8                     BankAddress
  );


/**
  Get status for all PPR resources and store number of resources available
  per Controller/Channel/Rank/Dimm into Outputs->PprAvailableResources.

  @param[in] MrcData        - Global MRC data structure
**/
VOID
GetAllPprResources (
  MrcParameters   *const    MrcData
  );

/**
  Configures the data pattern that will be written and read for the next memory test.

  @param[in] MrcData                    - Global MRC data structure
  @param[in] PprAmtData                 - PPR and AMT data structure
**/
VOID
AmtSetDataPattern (
  MrcParameters *const            MrcData,
  PPR_AMT_PARAMETER_DATA  *const  PprAmtData
  );

/**
  Returns the number of low order bank group and bank address bits that are not included in the BG interleave

  @param[in] MrcData                - Pointer to MrcData

  @retval BaseBits                  - Number of bank bits for software bank loop
**/
UINT32
GetBaseBits (
  IN MrcParameters *const MrcData
  );

/**
  Cleans up registers and values set during PPR memory tests.

  @param[in] MrcData            - Global MRC data structure
  @param[in] PprAmtData         - PPR and AMT data structure
  @param[in] McChBitMask        - Bit masks of MC/Channels to clean settings.
**/
VOID
PprCleanup (
  IN MrcParameters *const           MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          McChBitMask
  );

/**
  Check row failure if repairs are required

  @param[in] FailRangePtr         - Pointer to Row Fail
  @param[in] BankGroup            - Bank group

  @retval status - TRUE/FALSE
**/
BOOLEAN
IsRowFailureRepairRequired (
  ROW_FAIL_RANGE  *const FailRangePtr,
  UINT8                  BankGroup
  );

/**
  Check row failure list and PPR resource list to determine if repairs are required

  @param[in] MrcData              - Global MRC data structure
  @param[in] BaseBits             - Number of bank bits in SW loop

  @retval status - TRUE/FALSE
**/
BOOLEAN
IsPprRepairRequired (
  MrcParameters  *const   MrcData,
  UINT8                   BaseBits
  );

/**
  Checks row failure list for any failures, and checks whether PPR resource exists for any failures.
  If PPR resources exist, runs disposition flow for each failure.

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     PprAmtData          - PPR and AMT data structure
  @param [in,out] *RepairDone         - Flag to indicate the repair was done
**/
VOID
PprDispositionFailRange (
  IN     MrcParameters *const       MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN OUT UINT8                      *RepairDone
  );

/**
  Executes PPR flow on correctable failures in the row failure list

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     PprAmtData          - PPR and AMT data structure
  @param [in]     McChBitMask         - Memory Controller Channel Bit mask to update
  @param [in,out] *RepairDone         - Flag to indicate the repair was done
**/
VOID
DispositionFailRangesWithPprFlow (
  IN     MrcParameters      *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN     UINT32                     McChBitMask,
  IN OUT UINT8                      *RepairDone
  );

/**
  Checks if a Post Package Repair succeded and updates internal outputs structure based on the results.

  @param[in] MrcData     - Global MRC data structure
  @param[in] PprAmtData  - PPR and AMT data structure
  @param[in] McChBitMask - Memory Controller Channel Bit mask to read results for.
  @param[in] Bank        - Current bank address
  @param[in] Row         - Row address to run test

  @retval True when PPR succeded.
  @retval False when PPR failed.
**/
BOOLEAN
MrcPostPprAnalysis (
  IN MrcParameters  *const          MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT32                         Controller,
  IN UINT32                         Channel,
  IN UINT32                         Bank,
  IN UINT32                         Row
  );

/**
  Retrieves the Post Package Repair (PPR) channel bit mask for a specified controller and channel.

  @param[in]  MrcData     - Pointer to the MRC global data structure
  @param[in]  Controller  - The controller index
  @param[in]  Channel     - The channel index

  @returns The PPR channel bit mask for the specified controller and channel.
**/
UINT8
MrcGetPprChBitMask (
  IN MrcParameters  *const          MrcData,
  IN UINT32                         Controller,
  IN UINT32                         Channel
  );

/**
  Check to see if Retry is required after a PPR repair

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  RepairDone  - Indicator whether PPR repair was done
  @param[in]  RetryCount  - Count of retry attempts

  @retval TRUE if Retry is required; FALSE otherwise
**/
BOOLEAN
IsAmtRetryRequiredAfterRepair (
  IN MrcParameters *const MrcData,
  IN UINT8                RepairDone,
  IN UINT32               RetryCount
  );

/**

  Inject error for given Rank/Controller/Channel

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  PprAmtData    - PPR and AMT data structure
  @param[in]  Controller    - 0-based index to controller
  @param[in]  Channel       - 0-based index to channel
  @param[in]  Bank          - bank address number
  @param[in]  Row           - row address
  @param[in]  TestSize      - number of rows to test
  @param[in]  ErrInjMask16  - Bitmask of DQ lanes to inject error

  @retval MrcStatus - mrcSuccess/mrcFail
  **/
MrcStatus
InjectMemtestError (
  IN MrcParameters *const           MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT32                         Controller,
  IN UINT32                         Channel,
  IN UINT32                         Bank,
  IN UINT32                         Row,
  IN UINT32                         TestSize,
  IN UINT16                         ErrInjMask16
  );

/**
  Wrapper function for the Advanced Memory test algorithm that tracks time and splits read/write patterns.

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
  @param[in] CmdPat         - Type of sequence PatWr, PatRd, or PatRdWr
  @param[in] IsUseInvtPat   - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] Direction      - Sequential address direction AMT_ADDR_DIR_UP, AMT_ADDR_DIR_DOWN
  @param[in] PatternNumber  - Which pattern in the MATS8 sequence is this test

  @retval None
**/
VOID
MrcRunPprDetection (
  IN MrcParameters *const           MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          CmdPat,
  IN BOOLEAN                        IsUseInvtPat,
  IN UINT8                          Direction,
  IN UINT8                          PatternNumber
  );

/**
  Pause refreshes during Data Retention memory test

  @param[in] MrcData - Global MRC data structure
**/
VOID
AdvMemTestPauseDataRet (
  IN MrcParameters* const MrcData
);

/**
  Runs Advanced Memory Tests based on the MATS + algorithm

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
  @param[in] InvertedPassEn - Whether to run MATS+ patterns 4-6, which are the same as patterns 1-3 but with inverted data

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMATSN (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN BOOLEAN                        InvertedPassEn
);

/**
  Runs Advanced Memory Test targeting Data retention between refresh

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestDataRetention (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
);

/**
  Runs Advanced Memory Test march pattern

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMarch (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
  );

/**
  Runs Advanced Memory Test march pattern with different data pattern and test flow

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMarchAlt (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
  );

/**
  Runs Advanced Memory Test write/read test using multiple types of alternating data patterns

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMmrw (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
  );

/**
  Top-level PPR function that determines which memory tests should be run.
  Executes Post Package Repair flow to repair failures found during memory test.

  @param[in]  MrcData      - Pointer to global MRC data.

  @retval MrcStatus - mrcSuccess if the entire memory test/PPR flow executes, otherwise mrcFail if it is cut short
**/
MrcStatus
MrcPostPackageRepairEnable (
  IN MrcParameters *const MrcData
  );

/**
  Set up the controller for Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the current state of the controller.

  @retval None
**/
void
MrcDdr5PprControllerSetup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  );


/**
  Cleanup the MC configuration after Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the controller state prior to PPR.

  @retval None
**/
void
MrcDdr5PprControllerTeardown (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  );

/**
  Set up the controller for Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the current state of the controller.

  @retval None
**/
void
MrcLpddr5PprControllerSetup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  );

/**
  Cleanup the MC configuration after Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the controller state prior to PPR.

  @retval None
**/
void
MrcLpddr5PprControllerTeardown (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  );

/**
  Runs MBIST

  @param[in]  MrcData           - Global MRC data structure
  @param[in]  Rank              - Rank number
  @param[in]  MbistMcChBitMask  - Memory Controller Channel Bit mask for MBIST rerun status
  @param[in, out] MbistMpprStatus   - Array to store MBIST/mPPR Transparency status

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMbist (
  IN  MrcParameters             *const MrcData,
  IN  UINT32                    Rank,
  IN  UINT32                    MbistMcChBitMask,
  IN OUT UINT8                  MbistMpprStatus[MAX_CONTROLLER][MAX_CHANNEL][MAX_BYTE_IN_DDR5_CHANNEL]
  );

/**
  Runs mPPR

  @param[in]  MrcData               - Global MRC data structure
  @param[in]  Rank                  - Rank number
  @param[in]  MpprtMcChBitMask      - Memory Controller Channel Bit mask for mPPR rerun status
  @param[in, out] MbistMpprStatus   - Array to store MBIST/mPPR Transparency status

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
RunMppr (
  IN     MrcParameters             *const MrcData,
  IN     UINT32                    Rank,
  IN     UINT32                    MpprtMcChBitMask,
  IN OUT UINT8                     MbistMpprStatus[MAX_CONTROLLER][MAX_CHANNEL][MAX_BYTE_IN_DDR5_CHANNEL]
  );

/**
  Initial function to execute MBIST/mPPR

  @param[in]  MrcData           - Global MRC data structure

  @retval MrcStatus - mrcSuccess/mrcFail

**/
MrcStatus
RunMbistMppr (
  IN  MrcParameters             *const MrcData
  );

/**
  This is a test function for direct testing of the Post Package Repair sequence.
  @param[in] MrcData     - Pointer to global MRC data.

  @return MrcStatus - mrcSuccess if no errors, otherwise return error status.
**/
MrcStatus
MrcPostPackageRepairTest (
  IN  MrcParameters *const MrcData
  );

/**
  Get status whether PPR is enabled or disabled based on supported usecases.

  @param[in] MrcData global MRC data structure.

  @retval TRUE if PPR is enabled and a supported usecase is detected.
  @retval FALSE if PPR is disabled.
**/
BOOLEAN
MrcIsPprEnabled (
  IN MrcParameters *const MrcData
  );

/**
  Checks if Targeted PPR is requested.

  @param[in] MrcData global MRC data structure.

  @returns TRUE if Targeted PPR is requested, otherwise FALSE.
**/
BOOLEAN
MrcIsTargetedPprRequested (
  IN MrcParameters *const MrcData
  );

/**
  Run Targeted PPR if requested.
  @param[in] MrcData pointer to global MRC data.
  @param[in] PprAmtData pointer to PPR and AMT data structure.

  @returns MrcStatus
**/
MrcStatus
MrcRunPprTargeted (
  IN MrcParameters *const MrcData
  );

/**
  Enable or disable refreshes in the test engine

  @param[in]  MrcData           - Pointer to global MRC data.
  @param[in]  RefreshEnabled    - Whether refreshes should be enabled.
**/
VOID
MrcPprSetRefresh (
  IN  MrcParameters *const MrcData,
  IN  BOOLEAN              RefreshEnabled
  );

#endif // _MrcPpr_h_
