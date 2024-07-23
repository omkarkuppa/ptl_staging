/**@file
    Unit test cases of GetNvmeEstimateTime.

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

#include "GetNvmeEstimateTimeMock.h"

/**
  Unit test for GetNvmeEstimateTime.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetNvmeEstimateTime (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  NVME_ERASE_BLOCK_PRIVATE    *Private;
  UINT64                      Time;

  Private   = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));
  Private->Signature = NVME_ERASE_BLOCK_SIGNATURE;
  Private->EstimateTime[NvmeBlockErase - 1] = 10;

  Time = GetNvmeEstimateTime (&Private->EraseBlock, NvmeBlockErase);
  UT_ASSERT_EQUAL (Time, 10);

  Time = GetNvmeEstimateTime (&Private->EraseBlock, NvmeClear);
  UT_ASSERT_EQUAL (Time, 0);

  Time = GetNvmeEstimateTime (&Private->EraseBlock, NVME_SANITIZE_COUNT + 1);
  UT_ASSERT_EQUAL (Time, 0);

  return UNIT_TEST_PASSED;
}
