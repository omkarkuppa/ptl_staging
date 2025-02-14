/** @file
  This file contains functions to detect and record DRAM Failing addresses via
  Advanced Memory Test.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#ifndef _MrcAmt_h_
#define _MrcAmt_h_

#include "MrcCpgcApi.h"
#include "Cpgc20.h"
#include "Cpgc20Patterns.h"

//
// Hooks to test error handling functionality
//
#define POISON_ROW_FAIL_LIST                              0

// Hook for function to display row failure list
#define AMT_PRINT_ROW_FAIL_LIST   0

#define LOCAL_STUB_PPR_RESOURCE_ALWAYS_AVAILABLE  0

#define AMT_USE_XMARCH_ALT 1
#define AMT_USE_YMARCH_SHORT_ALT 1

// Advanced Mem Test types
// Should agree with PPR test type bits defined in CMrcExtTypes.h
typedef enum {
  AdvMtWcMats8 = 0,         // Worst case MATS8 test; default test type
  AdvMtDataRet,             // Data retention test
  AdvMtXMarch,
  AdvMtXMarchG,
  AdvMtYMarchShort,
  AdvMtYMarchLong,
  AdvMtMmrw,
  AdvMtNumMemTests
} MRC_ADVANCED_MEM_TEST_TYPE;

typedef union {
  struct {
    UINT8 PprEnabled    : 1;
    UINT8 AmtEnabled    : 1;
    UINT8 Reserved      : 6;
  } Bits;
  UINT8 Data;
} AMT_PPR_ENABLE;

// Definitions for MMRW test type
#define NUM_AMT_MMRW_TESTS 3
#define NUM_AMT_MMRW_PATTERNS_PER_TEST 4
typedef enum {
  AmtMmrwTestPattern1 = 0x778501,
  AmtMmrwTestPattern2 = 0x778502,
  AmtMmrwTestPattern3 = 0x778503
} MRC_AMT_MMRW_TEST_PATTERN_TYPE;

// Mem test type selection and overrides
// PprTestType setup option encoding -- Bit 0: WCMATS8 test, Bit 1: Data Retention test, Bit 2: X March test, Bit 3: X March G test, Bit 4: Y March Short test, Bit 5: Y March Long test
#define PPR_TEST_TYPE_OVERRIDE                            (0x0)

// Definition used with AmtCheckTestResults
// cpgcErrorStatus parameter
//
// LP5:
// cpgcErrDat0S           - Channel data lane [15:0] UI error status.
// cpgcErrEccS            - Ecc lane [7:0] UI error status.
// cpgcErrRow             - Row address that error happens on
// cpgcErrBank            - Bank address that error happens on
// cpgcErrRank            - Rank address that error happens on
// overflow               - True if more than two errors were found on a given bank (LP5 has 16 banks max)
//
// DDR5:
// cpgcErrDat0S           - Channel data lane [31:0] UI error status.
// cpgcErrEccS            - Ecc lane [7:0] UI error status.
// cpgcErrRow             - Row address that error happens on
// cpgcErrBank            - [4:0]   Bank address that error happens on
// cpgcErrRank            - [2:0]   Rank address that error happens on
// overflow               - True if more than one error was found on a given bank (DDR5 has 32 banks max)

typedef struct {
  UINT32    cpgcErrDat0S;
  UINT32    cpgcErrEccS;
  UINT32    cpgcErrRow;
  UINT32    cpgcErrBank;
  UINT32    cpgcErrRank;
  BOOLEAN   overflow;
  UINT8     device;
} CPGC_ERROR_STATUS_AMT;

#define MAX_PATTERN_DEPTH CPGC_20_NUM_DPAT_EXTBUF
typedef struct {
  // Data pattern / test type
  UINT8                       CmdPat; // Command pattern (algorithm instruction) to be used on next test (PatWr, PatRd, PatRdWr)
  UINT8                       SubSeqDataInv[2]; // Max subsequences we use per pattern # is 2
  UINT64                      DataPattern[MAX_PATTERN_DEPTH];
  UINT8                       DataPatternDepth; // Number of DataPattern indices (buffers/ExtBuf) the current pattern uses
  BOOLEAN                     IsUseInvtPat; // Whether to invert the current data pattern
  UINT8                       UiShl; // Pattern Rotation per UI
  UINT8                       NumCL; // Number of cacheline transactions per algorithm instruction
  UINT8                       Direction;
  UINT8                       PatternNumber;
  MRC_ADVANCED_MEM_TEST_TYPE  TestType;
  BOOLEAN                     FromRowTestPpr;
    // Status reporting and data metrics
  MrcStatus                   TestStatus;   // mrcFail if RowFailRange is full
  UINT32                      CurTestTime;  // Accumulated time over all patterns for the current memory test (resets after disposition flow)
  UINT32                      TotalTestTime;  // Accumulated time over all AMT runs
  // DIMM parameters
  UINT8                       ColumnBits[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8                       RowBits[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8                       BankBits[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32                      BaseBits; // Number of low order bank group and bank address bits that are not included in the BG interleave
  // Test location tracking info
  UINT32                      Rank;
  UINT32                      Bank;
  UINT32                      TestRowAddr[MAX_CONTROLLER][MAX_IP_CHANNEL];  // The starting address for the next CPGC/MPTU test
  UINT32                      TestRowSize[MAX_CONTROLLER][MAX_IP_CHANNEL];  // The range that will be tested in the next CPGC/MPTU test
#if POISON_ROW_FAIL_LIST
  BOOLEAN                     HasRowFailListBeenPoisonedOnThisTest;
#endif
} PPR_AMT_PARAMETER_DATA;

/**
  This function will convert the CPGC Bank address into the DRAM Bank Address and Bank Group bits based on DDR technology

  @param[in]  MrcData     - Global MRC data structure
  @param[in]  LogicalBank - CPGC Bank Number
  @param[out] BankAddress - Pointer to BankAddress value
  @param[out] BankGroup   - Pointer to BankGroup value
**/
VOID
MrcConvertCpgcBanktoBankAddress (
  IN  MrcParameters *const MrcData,
  IN  UINT32               LogicalBank,
  OUT UINT8                *BankAddress,
  OUT UINT8                *BankGroup
  );

/**
  This function will convert the DRAM Bank Address and Bank Group bits into the CPGC Bank address based on DDR technology

  @param[in]  MrcData     - Global MRC data structure
  @param[in]  BankAddress - Pointer to BankAddress value
  @param[in]  BankGroup   - Pointer to BankGroup value
  @param[out] LogicalBank - CPGC Bank Number
**/
VOID
MrcConvertBankAddresstoCpgcBank (
  IN  MrcParameters *const MrcData,
  IN  UINT32               BankAddress,
  IN  UINT32               BankGroup,
  OUT UINT32               *LogicalBank
  );

/**
  This function programs the specified 64-bit Pattern provided, into CPGC ExtBuf registers.
  Data Pattern is shifted across each UI, if non-zero

  @param[in]  MrcData                 - Global MRC data structure
  @param[in]  McChBitMask             - Memory Controller Channel Bit mask to read status for.
  @param[in]  PatternQW               - Array of 64-bit Data Pattern to write
  @param[in]  PatternDepth            - Length of PatternQW in number of UIs
  @param[in]  UiShl                   - Bit-shift value per UI
  @param[in]  ErrInjMask16            - Bitmask of DQ lanes to inject error

  @retval   mrcSuccess / mrcFail
**/
MrcStatus
MrcProgramMATSPattern (
  IN  MrcParameters *const MrcData,
  IN  UINT32               McChBitMask,
  IN  UINT64               PatternQW[],
  IN  UINT8                PatternDepth,
  IN  UINT8                UiShl,
  IN  UINT16               ErrInjMask16
  );

/**
  This function programs a specific pattern for the MMRW test into CPGC ExtBuf registers.

  @param[in]  MrcData                 - Global MRC data structure
  @param[in]  McChBitMask             - Memory Controller Channel Bit mask to read status for.
  @param[in]  PatternQW               - Array of 64-bit Data Pattern to write
  @param[in]  ErrInjMask16            - Bitmask of DQ lanes to inject error

  @retval   mrcSuccess / mrcFail
**/
MrcStatus
MrcProgramDSPattern (
  IN  MrcParameters *const MrcData,
  IN  UINT32               McChBitMask,
  IN  UINT64               PatternQW[],
  IN  UINT16               ErrInjMask16
  );

/**
  Cpgc command pattern setup up for memory test

  @param[in] MrcData        - Pointer to MRC global data.
  @param[in] RwMode         - Cpgc read and write mode
  @param[in] UseSubSeq1     - Cpgc subseq1 usage
  @param[in] Direction      - Address direction during memory test
  @param[in] SeqDataInv     - Enables pattern inversion per subsequence
  @param[in] IsUseInvtPat   - Info to indicate whether or not patternQW is inverted by comparing original pattern

  @retval n/a
**/
VOID
MrcConfigurePprCmdPat (
  IN MrcParameters  *const MrcData,
  IN UINT8                 RwMode,
  IN BOOLEAN               UseSubSeq1,
  IN UINT8                 Direction,
  IN UINT8                 SeqDataInv[2],
  IN BOOLEAN               IsUseInvtPat
  );

/**
  This function programs the Raster Repo Configuration register based on input values.

  @param[in]  MrcData         - Pointer to MRC global data.
  @param[in]  McChBitMask     - Bitmask of participating controllers / channels
  @param[in]  Mode3Banks      - Specifies which Bank mode to use in Raster Repo Mode 3 usage.
  @param[in]  UpperBanks      - Indicates the base Bank to store when using Raster Repo Mode 3
  @param[in]  StopOnRaster    - If TRUE, the test will stop after status condition is met in Raster Repo
  @param[in]  RasterRepoClear - If set, Raster Repo entries will be Reset (this bit auto-clears)
  @param[in]  RasterRepoMode  - Indicates the mode of operation for Raster Repo
  @param[in]  Mode3Max        - In Raster Repo Mode 3, sets maximum number of failing address before an additional failing address will set fail_excess for the respective Bank.
**/
void
Cpgc20RasterRepoConfig (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT8         *const  Mode3Banks,
  IN  UINT8         *const  Mode3UpperBanks,
  IN  BOOLEAN       *const  StopOnRaster,
  IN  BOOLEAN       *const  RasterRepoClear,
  IN  UINT8         *const  RasterRepoMode,
  IN  UINT8         *const  Mode3Max
  );

/**
  This function Reads the Repo Content registers, and returns those values in an input array.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  Controller   - Controller to work on
  @param[in]  Channel      - Channel to work on
  @param[out] RepoStatus   - Contains the data from Repo Content registers, for 8 normal entries and 2 ECC entries

  @retval nothing
**/
void
Cpgc20ReadRasterRepoContent (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  OUT UINT64_STRUCT         RepoStatus[CPGC20_MAX_RASTER_REPO_CONTENT]
  );

/**
  This function reads the Raster Repo Status register and parses the individual fields into the input pointers.

  @param[in]   MrcData        - Pointer to MRC global data.
  @param[in]   Controller     - Controller to work on
  @param[in]   Channel        - Channel to work on
  @param[out]  BitmapValid    - Optional pointer, if provided it will be updated with indication of valid bits in Repo Status
  @param[out]  SummaryValid   - Optional pointer, if provided it will be updated with indication that test summary in Raster Repo is valid
  @param[out]  FailExcessAll  - Optional pointer, if provided it will be updated with indication of any bank that observed a number of failures that exceeded MAX_BANKS setting
  @param[out]  FailAnyAll     - Optional pointer, if provided it will be updated with indication if any bank failed
  @param[out]  NumErrLogged   - Optional pointer, if provided it will be updated with number of errors logged
  @param[out]  RasterRepoFull - Optional pointer, if provided it will be updated with indication of RasterRepo being full

  @retval nothing
**/
void
Cpgc20ReadRasterRepoStatus (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 Controller,
  IN  UINT8                 Channel,
  OUT UINT8         *const  BitmapValid,
  OUT UINT8         *const  SummaryValid,
  OUT UINT8         *const  FailExcessAll,
  OUT UINT8         *const  FailAnyAll,
  OUT UINT8         *const  NumErrLogged,
  OUT UINT8         *const  RasterRepoFull
  );

/**
  This function configures the CPGC Data Control register.

  @param[in]  MrcData                   - Pointer to MRC global data.
  @param[in]  DataSelectRotationRepeats - Number of times to repeat a Data_Instruction with Data_Select_Rotation_Enable
  @param[in]  SplitBackground           - Background mode.

  @retval Nothing.
**/
void
Cpgc20SetDataControl (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                DataSelectRotationRepeats,
  IN  UINT32                SplitBackground
  );

/**
  This function configures the CPGC Base Address Control register based on TestType.

  @param[in] MrcData        - Pointer to MRC global data.
  @param[in] TestType       - Memory test type that is currently being run

  @retval Nothing.
**/
void
AmtSetBaseAddressControl (
  IN  MrcParameters *const      MrcData,
  IN MRC_ADVANCED_MEM_TEST_TYPE TestType
  );

/**
  Get a pointer to Row fail range entry at the given list index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list

  @retval Pointer to Row fail range entry
**/
ROW_FAIL_RANGE *
AmtGetRowFailRange (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailIndex
);

/**
  Sets values for the fail range entry at the given index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list
  @param[in] NewAddr        - ROW_ADDR to set
  @param[in] NewBgMask      - new BankGroupMask
  @param[in] Overflow       - Whether too many errors were found on this bank
  @param[in] RowOverride    - if non-negative, value gets written to RowAddr after NewAddr is written

  @retval None
**/
VOID
AmtSetRowFailRange (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailIndex,
  ROW_ADDR                NewAddr,
  UINT32                  NewBgMask,
  BOOLEAN                 Overflow,
  INT32                   RowOverride
);

/**
  Sets values for the fail range entry at the given index with separate parameters for each ROW_ADDR field

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list
  @param[in] Row            - Row address of the fail
  @param[in] BankPair       - Bank address of the fail
  @param[in] Rank           - Rank address of the fail
  @param[in] UpperBgMask    - Not used for DDR5
  @param[in] Valid          - Whether this entry is a valid failure (should be 1)
  @param[in] NewBgMask      - new BankGroupMask
  @param[in] Overflow       - Whether too many errors were found on this bank

  @retval None
**/
VOID
AmtSetRowFailRangeAddr (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailIndex,
  UINT32                  Row,
  UINT32                  BankPair,
  UINT32                  Rank,
  UINT32                  UpperBgMask,
  UINT32                  Valid,
  UINT32                  NewBgMask,
  BOOLEAN                 Overflow
  );

/**
  Get the number of entries in the Row fail list for given controller/channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number

  @retval Number of entries in the Row fail list
**/
UINT32
AmtGetRowFailListSize (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel
);

/**
  Get the current index in the Row fail list for given controller/channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number

  @retval Current row fail range index
**/
UINT32
AmtGetRowFailIndex (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel
);

/**
  Set the current index in the Row fail list for given controller/channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list

  @retval   None
**/
VOID
AmtSetRowFailIndex (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailIndex
);

/**
  Set the number of entries in the Row fail list for given controller/channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel
  @param[in] FailMax        - Number of entries in Row fail list

  @retval   None
**/
VOID
AmtSetRowFailListSize (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailMax
);

/**
  Creates a full DQ failure per technology.

  @param[in] MrcData        - Global MRC data structure
  @param[in,out] DqMask     - Bitmask of DQ errors

**/
VOID
AmtCreateDqFailure (
  IN     MrcParameters* const   MrcData,
  IN OUT UINT32                 DqMask[DQ_MASK_INDEX_MAX]
  );

/**
  Reads and returns approximate DIMM temperature via MR4 Refresh Multiplier.

  @param[in] MrcData      - Global MRC data structure
  @param[in] Controller   - Target Controller
  @param[in] Channel      - Target Channel
  @param[in] Rank         - Target Rank

  @retval   DeviceTemp    - MR4 Refresh Multiplier value.

**/
UINT8
MrcReadDeviceTemperature (
  IN MrcParameters* const   MrcData,
  IN UINT32                 Controller,
  IN UINT32                 Channel,
  IN UINT32                 Rank
  );

/**
  Prints an approximate DIMM temperature based on MR4 Refresh Multplier value.

  @param[in] MrcData      - Global MRC data structure
  @param[in] DeviceTemp   - MR4 Refresh Multiplier value.

**/
VOID
MrcPrintDeviceTemperature (
  IN MrcParameters* const   MrcData,
  IN UINT8                  DeviceTemp
  );

/**
  Compare most significant channel address bits excluding row bits

  @param CurAddr          - First address to compare
  @param RowAddr          - Second address to compare

  @retval   TRUE for match, otherwise FALSE
**/
BOOLEAN
AmtCurrentAddrMatch (
  ROW_ADDR CurAddr,
  ROW_ADDR RowAddr
  );

/**
  Display the row failure list

  @param[in] MrcData            - Global MRC data structure
  @param[in] Controller         - Controller number
  @param[in] Channel            - Channel number

  @retval   None
**/
#if AMT_PRINT_ROW_FAIL_LIST
VOID
AmtDisplayRowFailureList (
  IN MrcParameters   *const   MrcData,
  IN UINT8                    Controller,
  IN UINT8                    Channel
  );
#endif

/**
  Shifts row fail list to right by number of entries starting at given index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to compare
  @param[in] Channel        - Channel number to compare
  @param[in] Index          - List index to begin the shift operation
  @param[in] NumEntries     - Number of entries to shift

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtShiftFailRangeListRight (
  MrcParameters *const MrcData,
  UINT8                Controller,
  UINT8                Channel,
  UINT32               Index,
  UINT32               NumEntries
  );

/**
  Shifts row fail list left by number of entries starting at given index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to compare
  @param[in] Channel        - Channel number to compare
  @param[in] Index          - List index to begin the shift operation
  @param[in] NumEntries     - Number of entries to shift

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtShiftFailRangeListLeft (
  MrcParameters *const MrcData,
  UINT8                Controller,
  UINT8                Channel,
  UINT32               Index,
  UINT32               NumEntries
  );

/**
  Remove an entry from the Row Fail Range list

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
  @param[in] FailIndex      - List index to be removed from the list

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtRemoveRowFailRange (
  MrcParameters *const MrcData,
  UINT8                Controller,
  UINT8                Channel,
  UINT32               FailIndex
  );

/**
  Sets values for the fail range entry at the given index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list
  @param[in] NewFail        - FailRange for the new fail to add
  @param[in] SetIndex       - Whether to update the MrcOutputs->FailIndex pointer to the newly created failure
  @param[in,out] Status     - mrcSuccess / mrcFail

  @retval Whether the operation was successful
**/
UINT8
AmtInsertEntryIntoRowFailRange (
  IN     MrcParameters* const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT32               FailIndex,
  IN     ROW_FAIL_RANGE       *NewFail,
  IN     BOOLEAN              SetIndex,
  IN OUT MrcStatus            *Status
  );

/**
  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
  @param[in] NewFail        - New failure information

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtUpdateRowFailList (
  MrcParameters *const MrcData,
  UINT8                Controller,
  UINT8                Channel,
  ROW_FAIL_RANGE       NewFail
  );

/**
  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param[in]  MrcData                 - Global MRC data structure
  @param[in]  PprAmtData              - PPR and AMT data structure
  @param[in]  Controller              - Controller number of failure
  @param[in]  Channel                 - Channel number of failure
  @param[in]  CpgcErrorStatus         - the failure information
  @param[in]  DeviceTemp              - Dimm Temperature on initial failure

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtUpdateRowFailures (
  IN  MrcParameters *const          MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN  UINT8                         Controller,
  IN  UINT8                         Channel,
  IN  CPGC_ERROR_STATUS_AMT         CpgcErrorStatus,
  IN  UINT8                         DeviceTemp[MAX_CONTROLLER][MAX_CHANNEL]
  );

/**

  CPGC setup per rank for Advanced Memory test

  @param[in] MrcData                    - Global MRC data structure
  @param[in] PprAmtData                 - PPR and AMT data structure
  @param[in] McChBitMask                - Memory Controller Channel Bit mask to read results for.

  @retval Bit mask of channels enabled if rank exists

**/
UINT8
AdvancedMemTestRankSetupMATSRowRange (
  IN OUT MrcParameters  *const      MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          McChBitMask
  );

/**
  This function sets up a test for the given MC channel mask, with specified data pattern.

  @param[in,out] MrcData           - Pointer to MRC global data.
  @param[in]     PprAmtData        - PPR and AMT data structure
  @param[in]     McChBitMask       - Memory Controller Channel Bit mask to test.
  @param[in]     LoopCount         - CPGC sequence loop count

  @retval mrcSuccess
**/
MrcStatus
SetupIOTestRetention (
  IN OUT   MrcParameters *const     MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN       UINT8                    McChBitMask,
  IN       UINT8                    LoopCount
  );

/**
  Programs all the key registers to define a CPGC test as per input mask and Outputs->McChBitMask.
  McChBitMask is initialized in MrcPretraining based on population, and used throughout internal CPGC structure.
  Modify McChBitMask to specify which MC/CH to program

  @param[in] MrcData       - Include all MRC global data.
  @param[in] McChBitMask   - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] CmdPat        - 0: PatWrRd (Standard Write/Read Loopback),
                             1: PatWr (Write Only),
                             2: PatRd (Read Only),
                             3: PatRdWrTA (ReadWrite Turnarounds),
                             4: PatWrRdTA (WriteRead Turnarounds),
  @param[in] NumCL         - Number of Cache lines transactions per algorithm instruction within 1 BlockRepeat.
                             Non-zero Inputs->NumCL will override this.
  @param[in] LcExp         - Loop Count exponent. Non-zero Inputs->LoopCount will override this.
  @param[in] AddressArray  - 2D Array of Structure that stores address related settings
  @param[in] SOE           - [0: Never Stop, 1: Stop on Any Lane, 2: Stop on All Byte, 3: Stop on All Lane]
  @param[in] PatCtlPtr     - Structure that stores start, Stop, IncRate and Dqpat for pattern.
  @param[in] SubSeqWait    - # of Dclks to stall at the end of a sub-sequence
**/
extern
VOID
SetupIOTestAmt (
  IN MrcParameters *const         MrcData,
  IN const UINT8                  McChBitMask,
  IN const UINT8                  CmdPat,
  IN       UINT32                 NumCL,
  IN       UINT8                  LcExp,
  IN MRC_ADDRESS                  AddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN const MRC_TEST_STOP_TYPE     SOE,
  IN       MRC_PATTERN_CTL *const PatCtlPtr,
  IN       UINT16                 SubSeqWait
  );

/**
  This function sets up a WR-only test for the given MC channel mask, with all-zero data.
  Used for driving DQ low for Post Package Repair after passing repair row address.

  @param[in,out] MrcData     - Pointer to MRC global data.
  @param[in]     Controller  - Targeted controller
  @param[in]     Channel     - Targeted channel
  @param[in]     Row         - Targeted row
  @param[in]     BankGroup   - Targeted bank group
  @param[in]     BankAddress - Targeted bank address
  @param[in]     ByteMask    - Targeted Bytes for DQ toggle

  @retval MrcStatus          - mrcSuccess if no errors encountered
**/
MrcStatus
SetupIOTestPPR (
  IN OUT MrcParameters* const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel,
  IN     const UINT32         Row,
  IN     const UINT32         BankGroup,
  IN     const UINT32         BankAddress,
  IN     const UINT16         *ByteMask     OPTIONAL
  );

/**
  This implements the Advanced Memory test algorithm, to run over the specified Controller/Channels.

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval status - mrcSuccess / mrcFail
**/
VOID
MrcAdvancedMemTest (
  IN MrcParameters            *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA   *const  PprAmtData
);

/**
  Checks the results of the preceding advanced memory test

  @param[in]  MrcData                 - Global MRC data structure
  @param[in]  PprAmtData              - PPR and AMT data structure
  @param[in]  McChBitMask             - Memory Controller Channel Bit mask to check results for.

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtCheckTestResults (
  IN  MrcParameters *const          MrcData,
  IN  PPR_AMT_PARAMETER_DATA *const PprAmtData,
  IN  UINT32                        McChBitMask
  );

/**
  Executes a step of Advanced Memory test on given row address and size, and logs results.

  @param[in] MrcData                - Global MRC data structure
  @param[in] PprAmtData             - PPR and AMT data structure
  @param[in] McChBitMask            - Memory Controller Channel Bit mask to read results for.
  @param[in] BaseRow                - Row address to start test
  @param[in] RangeSize              - Row range size to test

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtExecuteRowRangeTest (
  MrcParameters   *const        MrcData,
  IN PPR_AMT_PARAMETER_DATA     *const  PprAmtData,
  UINT8                         McChBitMask,
  UINT32                        BaseRow[MAX_CONTROLLER][MAX_IP_CHANNEL],
  UINT32                        RangeSize[MAX_CONTROLLER][MAX_IP_CHANNEL]
  );

/**
  Run write/read test on a row.

  @param[in] MrcData              - Global MRC data structure
  @param[in] PprAmtData           - PPR and AMT data structure
  @param[in] McChBitMask          - Memory Controller Channel Bit mask to read results for.
  @param[in] Row                  - Row address to run test

  @retval status - mrcSuccess if no row failures found, mrcFail if row failure found

  **/
MrcStatus
RowTestPprWorker (
  MrcParameters *const            MrcData,
  PPR_AMT_PARAMETER_DATA  *const  PprAmtData,
  UINT8                           McChBitMask,
  UINT32                          Row
  );

/**
  Executes a single-row write/read memory test with a simple data pattern.

  @param[in] MrcData              - Global MRC data structure
  @param[in] PprAmtData           - PPR and AMT data structure
  @param[in] McChBitMask          - Memory Controller Channel Bit mask to read results for.
  @param[in] Bank                 - Current bank address
  @param[in] Row                  - Row address to run test

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
RowTestPpr (
  IN MrcParameters  *const          MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          McChBitMask,
  IN UINT32                         CpgcBank,
  IN UINT32                         Row
  );

/**
  Manually add entries into the row failure list for debugging purposes

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval none
**/
#if POISON_ROW_FAIL_LIST
VOID
AmtPoisonRowFailList (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
);

/**
Loop through all controllers and channels to find and print the total row failures per controller, channel

@param[in]  MrcData      - Pointer to global MRC data.

**/
VOID
AmtPrintTotalRowFailures (
  MrcParameters *const    MrcData
  );
#endif // POISON_ROW_FAIL_LIST

/**
  This function sets the Pattern Generators' Alternate Dpat Buffer Control for use with AMT tests.
  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  Controller          - Desired Memory Controller.
  @param[in]  Channel             - Desired Channel.
  @param[in]  Start               - Lane to start from
  @param[in]  Stop                - Lane to wrap ("stop") on
  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
AmtSetDpatAltBufCtl (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 Controller,
  IN  UINT8                 Channel,
  IN  UINT8                 Start,
  IN  UINT8                 Stop
  );

/**
  This function programs Bank Logical to Physical mapping for single-bank tests and for LP5 16-bank mode.
  The register will be updated on all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  UseSingleBank     - Whether to program a single bank; if false, will program all banks in order without BG calculation
  @param[in]  CpgcBank          - Cpgc address of bank to work on, only valid if UseSingleBank = TRUE
**/
VOID
AmtUpdateL2PBankMappingWithoutBG (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Rank,
  IN  BOOLEAN               UseSingleBank,
  IN  UINT32                CpgcBank
  );

#endif // _MrcAmt_h_
