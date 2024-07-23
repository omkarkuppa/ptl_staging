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

#include "MrcCpgcApi.h"
#include "MrcAmt.h"

#define PPR_MAX_DETECTED_ERRORS       (0xF000)

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
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] Row         - Row for detected fail row
  @param[in] BankMode    - Bank/Bank Group mode for dimm

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
  @param [in]     BankMapping         - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers.
  @param [in,out] *RepairDone         - Flag to indicate the repair was done

  @retval status - MRC_STATUS_SUCCESS/MRC_STATUS_FAILURE
**/
MrcStatus
PprDispositionFailRange (
  IN     MrcParameters *const    MrcData,
  IN     MRC_BG_BANK_PAIR        BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  IN OUT UINT8                   *RepairDone
  );

/**
  Executes PPR flow on correctable failures in the row failure list

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     McChBitMask         - Memory Controller Channel Bit mask to update
  @param [in]     Rank                - Rank number
  @param [in]     BaseBits            - Number of BaseBits per technology
  @param [in]     BankMapping         - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers.
  @param [in,out] *RepairDone         - Flag to indicate the repair was done

  @retval status - MRC_STATUS_SUCCESS/MRC_STATUS_FAILURE

**/
MrcStatus
DispositionFailRangesWithPprFlow (
  IN     MrcParameters* const    MrcData,
  IN     UINT32                  McChBitMask,
  IN     UINT32                  Rank,
  IN     UINT8                   BaseBits,
  IN     MRC_BG_BANK_PAIR        BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  IN OUT UINT8*                  RepairDone
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
  @param[in]  Rank          - Physical rank index inside the dimm
  @param[in]  Controller    - 0-based index to controller
  @param[in]  Channel       - 0-based index to channel
  @param[in]  Bank          - bank address number
  @param[in]  Row           - row address
  @param[in]  TestSize      - number of rows to test
  @param[in]  Pattern       - Array of 64-bit test pattern
  @param[in]  PatternDepth  - Length of PatternQW in number of UIs
  @param[in]  ErrInjMask16  - Bitmask of DQ lanes to inject error
  @param[in]  Direction     - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  @param[in]  SeqDataInv[2] - Enables pattern inversion per subsequence
  @param[in]  IsUseInvtPat  - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in]  UiShl         - Enables pattern rotation between all UIs

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
InjectMemtestError (
  IN MrcParameters* const MrcData,
  IN UINT32         Rank,
  IN UINT32         Controller,
  IN UINT32         Channel,
  IN UINT32         Bank,
  IN UINT32         Row,
  IN UINT32         TestSize,
  IN UINT64         Pattern[],
  IN UINT8          PatternDepth,
  IN UINT16         ErrInjMask16,
  IN UINT8          Direction,
  IN UINT8          SeqDataInv[2],
  IN BOOLEAN        IsUseInvtPat,
  IN UINT8          UiShl,
  IN MRC_ADVANCED_MEM_TEST_TYPE TestType
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
  Returns the number of low order bank group and bank address bits that are not included in the BG interleave

  @param[in] MrcData                - Pointer to MrcData

  @retval BaseBits                  - Number of bank bits for software bank loop
**/
UINT32
GetBaseBits (
  IN MrcParameters *const MrcData
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
  Wrapper function for the Advanced Memory test algorithm that tracks time and splits read/write patterns.

  @param[in] MrcData        - Global MRC data structure
  @param[in] CmdPat         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] SeqDataInv     - Specifies whether data pattern should be inverted per subsequence
  @param[in] Pattern        - Array of 64-bit Data Pattern for the test
  @param[in] PatternDepth   - Length of PatternQW in number of UIs
  @param[in] IsUseInvtPat   - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] UiShl          - Bit-shift value per UI
  @param[in] NumCL          - Number of cachelines to use in WDB
  @param[in] Direction      - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DOWN
  @param[in] Status         - mrcSuccess / mrcFail
  @param[in] TotalTime      - Accumulated time over all AMT runs for the current data pattern
  @param[in] PatternNumber  - Which pattern in the MATS8 sequence is this test
  @param[in] TestType       - Memory test type that is currently being run

  @retval None
**/
VOID
MrcRunPprDetection (
  IN MrcParameters*             const MrcData,
  IN UINT8                      CmdPat,
  IN UINT8                      SeqDataInv[2], // MT_MAX_SUBSEQ = 2
  IN UINT64                     Pattern[],
  IN UINT8                      PatternDepth,
  IN BOOLEAN                    IsUseInvtPat,
  IN UINT8                      UiShl, // Pattern Rotation per UI
  IN UINT8                      NumCL,
  IN UINT8                      Direction,
  IN MrcStatus                  *Status,
  IN UINT32                     *TotalTime,
  IN UINT8                      PatternNumber,
  IN MRC_ADVANCED_MEM_TEST_TYPE TestType
  );

/**
  Pause condition for data retention Advanced Memory Test

  @param[in] MrcData - Global MRC data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
AdvMemTestPauseDataRet (
  IN MrcParameters* const MrcData
);

/**
  Runs Advanced Memory Tests based on the MATS + algorithm

  @param[in] MrcData        - Global MRC data structure
  @param[in] BankMapping    - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers
  @param[in] InvertedPassEn - Whether to run MATS+ patterns 4-6, which are the same as patterns 1-3 but with inverted data
  @param[in] TotalTestTime  - Accumulated time over all AMT runs

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMATSN (
  IN MrcParameters  *const MrcData,
  MRC_BG_BANK_PAIR  BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  BOOLEAN           InvertedPassEn,
  UINT32            *TotalTestTime
);

/**
  Runs Advanced Memory Test targeting Data retention between refresh

  @param[in] MrcData        - Global MRC data structure
  @param[in] BankMapping    - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers
  @param[in] TotalTestTime  - Accumulated time over all AMT runs

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestDataRetention (
  IN MrcParameters  *const MrcData,
  MRC_BG_BANK_PAIR  BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  UINT32            *TotalTestTime
);

/**
  Runs Advanced Memory Test targeting Data retention between refresh

  @param[in] MrcData        - Global MRC data structure
  @param[in] BankMapping    - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers
  @param[in] TotalTestTime  - Accumulated time over all AMT runs
  @param[in] TestType       - Memory test type that is currently being run

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMarch (
  IN MrcParameters              *const MrcData,
  MRC_BG_BANK_PAIR              BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  UINT32                        *TotalTestTime,
  IN MRC_ADVANCED_MEM_TEST_TYPE TestType
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

#endif // _MrcPpr_h_
