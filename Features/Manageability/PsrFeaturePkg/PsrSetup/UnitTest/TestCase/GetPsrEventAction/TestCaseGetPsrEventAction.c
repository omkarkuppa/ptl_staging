/** @file
  Unit test case of the GetPsrEventAction function.

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

#include "TestCaseGetPsrEventAction.h"
#include "PrivateMock.h"

/**
  Unit Test case for GetPsrEventAction function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetPsrEventAction (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  //
  // Verify returned result.
  // Expected Result: L"Clear BIOS NVM Variables                          ".
  //
  UT_ASSERT_MEM_EQUAL (gReturnResult, L"Clear BIOS NVM Variables                          ", sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE);

  return UNIT_TEST_PASSED;
}
