/**@file
    Unit test cases of GetUfsEstimateTime.

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

#include "GetUfsEstimateTimeMock.h"

/**
  Unit test for GetUfsEstimateTime.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetUfsEstimateTime (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UFS_ERASE_PRIVATE_DATA    *Private;
  UINT64                    Time;

  Private   = AllocateZeroPool (sizeof (UFS_ERASE_PRIVATE_DATA));
  Private->Signature = UFS_ERASE_SIGNATURE;
  Private->EstimateTime[UfsBlockErase - 1] = 6;

  Time = GetUfsEstimateTime (&Private->EraseBlock, UfsBlockErase);
  UT_ASSERT_EQUAL (Time, 6);

  Time = GetUfsEstimateTime (&Private->EraseBlock, UfsClear);
  UT_ASSERT_EQUAL (Time, 0);

  Time = GetUfsEstimateTime (&Private->EraseBlock, UFS_SANITIZE_COUNT + 1);
  UT_ASSERT_EQUAL (Time, 0);

  return UNIT_TEST_PASSED;
}
