/**@file
    Unit test cases of SetSataEstimateTime.

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

#include "SetSataEstimateTimeMock.h"

/**
  Unit test for SetSataEstimateTime.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSetSataEstimateTime (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  SATA_ERASE_BLOCK_PRIVATE    *Private;

  Private   = AllocateZeroPool (sizeof (SATA_ERASE_BLOCK_PRIVATE));

  PreSetSataEstimateTime_SecureErase (Context);
  SetSataEstimateTime (Private, mIdentifyData);
  UT_ASSERT_EQUAL (Private->EstimateTime[SataBlockErase-1], 2 * 60);     // time_for_security_erase_unit * 2 * 60
  UT_ASSERT_EQUAL (Private->EstimateTime[SataClear-1], 0);
  CleanUpSetSataEstimateTime (Context);

  PreSetSataEstimateTime_256MB (Context);
  SetSataEstimateTime (Private, mIdentifyData);
  if ((mBlockIo != NULL) && (mBlockIo->Media != NULL)) {
    UT_ASSERT_EQUAL (Private->EstimateTime[SataClear-1], 0x3C0000000F);  // Min 15 Second per 4GB, Max 60 Second per 4GB
  } else {
    UT_ASSERT_EQUAL (Private->EstimateTime[SataClear-1], 0);
  }
  CleanUpSetSataEstimateTime (Context);

  PreSetSataEstimateTime_64GB (Context);
  SetSataEstimateTime (Private, mIdentifyData);
  if ((mBlockIo != NULL) && (mBlockIo->Media != NULL)) {
    UT_ASSERT_EQUAL (Private->EstimateTime[SataClear-1], 0x3C0000000F0);
  } else {
    UT_ASSERT_EQUAL (Private->EstimateTime[SataClear-1], 0);
  }
  CleanUpSetSataEstimateTime (Context);
  return UNIT_TEST_PASSED;
}
