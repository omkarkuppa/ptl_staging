/**@file

    TestCase of GetStrLength function.

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

#include "TestCaseGetStrLength.h"


/**
  Testcase for GetStrLength function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseGetStrLength (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  CHAR8  GetStrLengthTestString[] = "UnitTest";


  if (GetStrLength (GetStrLengthTestString) == 9) {
    DEBUG ((DEBUG_ERROR, "[GetStrLength] Testing result is pass.\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "GetStrLength funciton Fail.\n"));
    UT_ASSERT_FALSE (TRUE);
  }

  return UNIT_TEST_PASSED;
}
