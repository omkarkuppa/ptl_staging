/**@file
    Unit test cases of UpdateResult.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "UpdateResultMock.h"

/**
  Unit test for UpdateResult.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestUpdateResult (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  SATA_ERASE_BLOCK_PRIVATE    *Private;

  Private = AllocateZeroPool (sizeof (SATA_ERASE_BLOCK_PRIVATE));
  Private->Info = AllocateZeroPool (sizeof (SATA_ERASE_INFO));

  UpdateResult (Private, SataBlockErase, EFI_SUCCESS);

  UT_ASSERT_NOT_NULL (Private->Info);
  UT_ASSERT_EQUAL (Private->Info->Method, SataBlockErase);
  UT_ASSERT_EQUAL (Private->Info->Result, EFI_SUCCESS);

  FreePool(Private->Info);
  FreePool(Private);
  return UNIT_TEST_PASSED;
}
