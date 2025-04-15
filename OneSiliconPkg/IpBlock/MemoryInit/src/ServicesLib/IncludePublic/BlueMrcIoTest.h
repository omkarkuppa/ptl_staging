/** @file
   This file include IoTest methods.

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

@par Specification
**/

#ifndef BLUE_MRC_IO_TEST_H_
#define BLUE_MRC_IO_TEST_H_

#include "MrcIoTest.h" // for definitions common to blue and green
#include "Cpgc20Patterns.h"
#include "MrcIoTest.h"

/**
  Programs all the key registers to define a CPGC test as per input mask and Outputs->McChBitMask.
  McChBitMask is initialized in MrcPretraining based on population, and used throughout internal CPGC structure.
  Modify McChBitMask to specify which MC/CH to program
  This function shall be used in Blue MRC CPGC-related calls
  
  @param[in] MrcData          - Include all MRC global data.
  @param[in] McChBitMask      - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] CmdPat           - 0: PatWrRd (Standard Write/Read Loopback),
                                1: PatWr (Write Only),
                                2: PatRd (Read Only),
                                3: PatRdWrTA (ReadWrite Turnarounds),
                                4: PatWrRdTA (WriteRead Turnarounds),
  @param[in] NumCL            - Number of Cache lines transactions per algorithm instruction within 1 BlockRepeat.
                                Non-zero Inputs->NumCL will override this.
  @param[in] LcExp            - Loop Count exponent. Non-zero Inputs->LoopCount will override this.
  @param[in] AddressArray     - 2D Array of Structure that stores address related settings
  @param[in] SOE              - [0: Never Stop, 1: Stop on Any Lane, 2: Stop on All Byte, 3: Stop on All Lane]
  @param[in] PatCtlPtr        - Structure that stores start, Stop, IncRate and Dqpat for pattern.
  @param[in] SubSeqWait       - # of Dclks to stall at the end of a sub-sequence
  @param[in] CapNotPowerOf2   - Whether non-power of 2 capacity found per MC/CH
**/
extern
VOID
SetupIOTestCpgc (
  IN MrcParameters *const         MrcData,
  IN const UINT8                  McChBitMask,
  IN const UINT8                  CmdPat,
  IN       UINT32                 NumCL,
  IN       UINT8                  LcExp,
  IN       MRC_ADDRESS            AddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN const MRC_TEST_STOP_TYPE     SOE,
  IN       MRC_PATTERN_CTL *const PatCtlPtr,
  IN       UINT16                 SubSeqWait,
  IN       BOOLEAN                CapNotPowerOf2[MAX_CONTROLLER][MAX_CHANNEL] OPTIONAL
  );
  
/**
  This function sets up a WR-only test for the given MC channel mask, with all-zero data.
  This is used for ECC scrubbing or memory clear.

  @param[in,out] MrcData          - Pointer to MRC global data.
  @param[in]     CPGCAddressArray - Memory region size to scrub.
  @param[in]     McChbitMask      - Bit masks of MC channels to enable for the test.
  @param[in]     CmdPat           - read/write.
  @param[in]     CapNotPowerOf2   - Whether non-power of 2 capacity found per MC/CH.
**/
VOID
SetupIOTestScrubCpgc (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_ADDRESS          CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN     const UINT8          McChBitMask,
  IN     const UINT8          CmdPat,
  IN     BOOLEAN              CapNotPowerOf2[MAX_CONTROLLER][MAX_CHANNEL] OPTIONAL
  );

/**
  Calls RunIoTest with Rank fixed as MRC_IGNORE_ARG_8.

  @param[in] MrcData      - Include all MRC global data.
  @param[in] McChBitMask  - Memory Controller Bit mask for which test should be setup for.
  @param[in] DQPat        - The Pattern type for the test.  See MrcDqPat.
  @param[in] ClearErrors  - Decision to clear or not errors.

  @retval Returns ch errors
**/
UINT8
RunIoTestNoRank (
  IN MrcParameters *const MrcData,
  IN const UINT8          McChBitMask,
  IN const UINT8          DQPat,
  IN const UINT8          ClearErrors
  );

#endif //BLUE_MRC_IO_TEST_H_
