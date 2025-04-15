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

#include "CMrcExtTypes.h"

//
// Hooks to test error handling functionality
//
#define POISON_ROW_FAIL_LIST                              0

#define LOCAL_STUB_PPR_RESOURCE_ALWAYS_AVAILABLE  0

#define MAX_PATTERN_DEPTH 16 // CPGC_20_NUM_DPAT_EXTBUF

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
  PprTestTypeOffset           TestType;
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

#endif // _MrcAmtPprInterface_h_
