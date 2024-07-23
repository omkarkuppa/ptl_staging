/**@file
    Unit test cases of SetNvmeEstimateTime.

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

#include "SetNvmeEstimateTimeMock.h"

/**
  Unit test for SetNvmeEstimateTime.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSetNvmeEstimateTime (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  NVME_ERASE_BLOCK_PRIVATE    *Private;

  Private   = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));

  PreSetNvmeEstimateTime_SecureErase (Context);
  SetNvmeEstimateTime (Private, mIdentifyData);
  UT_ASSERT_EQUAL (Private->EstimateTime[NvmeBlockErase-1], 10);      // NVME_SANITIZE_TIME_FORMAT
  UT_ASSERT_EQUAL (Private->EstimateTime[NvmeCryptoErase-1], 10);
  UT_ASSERT_EQUAL (Private->EstimateTime[NvmeClear-1], 0);
  CleanUpSetNvmeEstimateTime (Context);

  PreSetNvmeEstimateTime_256MB (Context);
  SetNvmeEstimateTime (Private, mIdentifyData);
  if ((mBlockIo != NULL) && (mBlockIo->Media != NULL)) {
    UT_ASSERT_EQUAL (Private->EstimateTime[NvmeClear-1], 0xb400000014); // Min 20 seconds per 4GB, Max 180 seconds per 4GB
  } else {
    UT_ASSERT_EQUAL (Private->EstimateTime[NvmeClear-1], 0);
  }
  CleanUpSetNvmeEstimateTime (Context);

  PreSetNvmeEstimateTime_64GB (Context);
  SetNvmeEstimateTime (Private, mIdentifyData);
  if ((mBlockIo != NULL) && (mBlockIo->Media != NULL)) {
    UT_ASSERT_EQUAL (Private->EstimateTime[NvmeClear-1], 0xb4000000140);
  } else {
    UT_ASSERT_EQUAL (Private->EstimateTime[NvmeClear-1], 0);
  }
  CleanUpSetNvmeEstimateTime (Context);

  return UNIT_TEST_PASSED;
}
