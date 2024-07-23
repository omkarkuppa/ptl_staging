/** @file
  This file include IoTest methods.

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

#ifndef MRC_IO_TEST_H_
#define MRC_IO_TEST_H_

#include "CMrcTypes.h"
#include "MrcCommon.h"

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
SetupIOTest (
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
  This function sets up a basic victim-aggressor test for the given channel mask and per Outputs->McChBitMask.
  McChBitMask is initialized in MrcPretraining based on population, and used throughout internal CPGC structure.
  Modify McChBitMask to specify which MC/CH to program

  @param[in,out] MrcData       - Pointer to MRC global data.
  @param[in]     McChBitMask   - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in]     LC            - Exponential number of loops to run the test.
  @param[in]     SOE           - Error handling switch for test.
  @param[in]     Spread        - Stopping point of the pattern.
  @param[in]     CmdPat        - Command Pattern
  @param[in]     NumCLOverride - Override for NumCL (Non-zero causes override)
**/
void
SetupIOTestBasicVA (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          McChBitMask,
  IN     const UINT8          LC,
  IN     const UINT8          SOE,
  IN     const UINT8          Spread,
  IN     const UINT8          CmdPat,
  IN     const UINT8          Wait,
  IN     const UINT16         NumCLOverride
  );

/**
  Runs one or more REUT tests (based on TestType).

  @param[in] MrcData      - Include all MRC global data.
  @param[in] McChBitMask  - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] CaParityRank - Optional parameter only used when DQPat is CaParity and passed to CA Parity IO runner.
  @param[in] DQPat        - The Pattern type for the test.  See MrcDqPat.
  @param[in] ClearErrors  - Decision to clear or not errors.
  @param[in] IsCmdVC      - Determine if command voltage training is being executed.

  @retval McChBitMask Returns ch errors
**/
UINT8
RunIOTest (
  IN MrcParameters *const MrcData,
  IN const UINT8          McChBitMask,
  IN const UINT8          CaParityRank, OPTIONAL
  IN const UINT8          DQPat,
  IN const UINT8          ClearErrors,
  IN BOOLEAN              IsCmdVC
  );

#endif //MRC_IO_TEST_H_
