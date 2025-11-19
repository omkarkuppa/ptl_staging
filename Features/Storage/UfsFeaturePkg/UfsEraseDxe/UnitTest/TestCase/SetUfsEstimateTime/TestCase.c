/**@file
    Unit test cases of SetUfsEstimateTime.

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

#include "SetUfsEstimateTimeMock.h"

/**
  Unit test for SetUfsEstimateTime.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSetUfsEstimateTime (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UFS_ERASE_PRIVATE_DATA    *Private;

  Private  = AllocateZeroPool (sizeof (UFS_ERASE_PRIVATE_DATA));

  PreSetUfsEstimateTime_BlockErase (Context);
  SetUfsEstimateTime (Private);
  UT_ASSERT_EQUAL (Private->EstimateTime[UfsBlockErase-1], 3);  // UFS_SANITIZE_TIME_UNMAP
  UT_ASSERT_EQUAL (Private->EstimateTime[UfsClear-1], 0);
  CleanUpSetUfsEstimateTime (Context);

  PreSetUfsEstimateTime_128MB (Context);
  SetUfsEstimateTime (Private);
  if ((mBlockIo != NULL) && (mBlockIo->Media != NULL)) {
    UT_ASSERT_EQUAL (Private->EstimateTime[UfsClear-1], 0x200000001); // Min is 1 Second / 128MB, Max is 2 Second / 128MB
  } else {
    UT_ASSERT_EQUAL (Private->EstimateTime[UfsClear-1], 0);
  }
  CleanUpSetUfsEstimateTime (Context);

  PreSetUfsEstimateTime_256MB (Context);
  SetUfsEstimateTime (Private);
  if ((mBlockIo != NULL) && (mBlockIo->Media != NULL)) {
    UT_ASSERT_EQUAL (Private->EstimateTime[UfsClear-1], 0x400000002); // Min is 1 Second / 128MB, Max is 2 Second / 128MB
  } else {
    UT_ASSERT_EQUAL (Private->EstimateTime[UfsClear-1], 0);
  }
  CleanUpSetUfsEstimateTime (Context);

  if (Private != NULL) {
    FreePool (Private);
  }
  return UNIT_TEST_PASSED;
}
