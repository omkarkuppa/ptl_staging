/**@file
    Unit test cases of SataDataCryptoErase.

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

#include "SataDataCryptoEraseMock.h"

/**
  Unit test for SataDataCryptoErase.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSataDataCryptoErase (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;

  Status = SataDataCryptoErase (NULL, 1, 0, NULL, 0);

  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  return UNIT_TEST_PASSED;
}
