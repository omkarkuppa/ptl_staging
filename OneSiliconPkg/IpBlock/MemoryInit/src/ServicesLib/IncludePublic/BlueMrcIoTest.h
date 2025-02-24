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
  This function sets up a WR-only test for the given MC channel mask, with all-zero data.
  This is used for ECC scrubbing or memory clear.

  @param[in,out] MrcData     - Pointer to MRC global data.
  @param[in]     CPGCAddressArray - Memory region size to scrub.
  @param[in]     McChbitMask - Bit masks of MC channels to enable for the test.
  @param[in]     CmdPat      - read/write.
**/
VOID
SetupIOTestScrub (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_ADDRESS          CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN     const UINT8          McChBitMask,
  IN     const UINT8          CmdPat
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
