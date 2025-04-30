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
#ifndef _MrcAmtPprInterface_h_
#define _MrcAmtPprInterface_h_

#include "MrcCommon.h"
#include "CMrcExtTypes.h"
#include "CMrcInterface.h"

//
// Hooks to test error handling functionality
//
#define POISON_ROW_FAIL_LIST                              0

#define LOCAL_STUB_PPR_RESOURCE_ALWAYS_AVAILABLE  0

#define AMT_ADDR_DIR_UP      (0)
#define AMT_ADDR_DIR_DOWN    (1)

#define AMT_ROW_RANGE_TEST_MAX_LOOPS  (120)



typedef enum {
  AmtInitialTest,
  AmtRemBankTest,
  AmtRem2ndOrderTest, // Columns in FAST_Y address order
  AmtRem3rdOrderTest, // Rows in FAST_Y address order
  AmtTestComplete,
  AmtYMarchLongRemEvenRows, // 2nd order test for Y March long, even rows only
  AmtYMarchLongRemOddRows   // 2nd order test for Y March long, odd rows only
} PPR_AMT_TEST_STATE;

#define MAX_PATTERN_DEPTH 16 // CPGC_20_NUM_DPAT_EXTBUF

// Definition used with AmtCheckTestResults
//
// errData                - Channel data lane [15:0] UI error status.
// errEcc                 - Ecc lane [7:0] UI error status.
// errRow                 - Row address that error happens on
// errCol                 - Column address that error happens on
// errBank                - Bank address that error happens on
// errRank                - Rank address that error happens on
// overflow               - True if more than two errors were found on a given bank (LP5 has 16 banks max, DDR5 has 32 banks max)
// device                 - DRAM device that error happens on
// deviceMask             - mask to track which DRAM devices have errors (used with stop-on-error logic)

typedef struct {
  UINT32    errData;
  UINT32    errEcc;
  UINT32    errRow;
  UINT32    errCol;
  UINT32    errBank;
  UINT32    errRank;
  BOOLEAN   overflow;
  UINT8     device;
} ERROR_STATUS_AMT;

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
  UINT8                       TestNumber;
  PprTestTypeOffset           TestType;
  BOOLEAN                     FromRowTestPpr;
  BOOLEAN                     UseStopOnError;
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
  // Stop on error specific tracking info
  ERROR_STATUS_AMT            LRA[MAX_CONTROLLER];        // Last read address, used with stop on error
  BOOLEAN                     ErrorFound[MAX_CONTROLLER]; // Whether error was found on last memory test
  PPR_AMT_TEST_STATE          AmtTestState[MAX_CONTROLLER];
  INT64                       ColStart[MAX_CONTROLLER][MAX_IP_CHANNEL];       // The starting column address for the next CPGC/MPTU test
  INT64                       ColEnd[MAX_CONTROLLER][MAX_IP_CHANNEL];         // The ending column address for the next CPGC/MPTU test
  INT64                       RowStart[MAX_CONTROLLER][MAX_IP_CHANNEL];       // The ending row address for the next CPGC/MPTU test
  INT64                       RowEnd[MAX_CONTROLLER][MAX_IP_CHANNEL];         // The ending row address for the next CPGC/MPTU test
  UINT32                      NumIgnoreReads[MAX_CONTROLLER][MAX_CHANNEL];    // Number of reads to ignore data
  // Error injection
  UINT16                      ErrInjMask;
  BOOLEAN                     LastWriteErrorInjection;    // Whether the last write injected an error

#if POISON_ROW_FAIL_LIST
  BOOLEAN                     HasRowFailListBeenPoisonedOnThisTest;
#endif
} PPR_AMT_PARAMETER_DATA;

#endif // _MrcAmtPprInterface_h_
