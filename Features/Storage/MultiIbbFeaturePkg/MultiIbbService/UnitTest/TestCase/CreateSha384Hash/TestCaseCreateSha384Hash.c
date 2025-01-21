/** @file
  Unit test case of the CreateSha384Hash function.

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

#include "TestCaseCreateSha384Hash.h"
#include "PrivateMock.h"

/**
  Unit Test case for CreateSha384Hash function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED             - Pass the test case.
  @retval UNIT_TEST_ERROR_TEST_FAILED  - Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCreateSha384Hash (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  //
  // Verify returned hash.
  //
  UINT8   Expected[] = {
            0x7D, 0xB1, 0x7E, 0x4E, 0x7C, 0xF5, 0x75, 0x65,
            0x0D, 0xF8, 0xF5, 0x03, 0xF3, 0x7B, 0x76, 0x46,
            0x03, 0xB9, 0xED, 0x5F, 0x36, 0xAC, 0x8D, 0x7E,
            0x46, 0xA0, 0xC4, 0x76, 0x5E, 0x15, 0xE4, 0x87,
            0x8F, 0x70, 0x60, 0xCE, 0xE5, 0x77, 0x13, 0xFB,
            0xC9, 0xDE, 0x56, 0x70, 0x9C, 0x5D, 0xC4, 0xB2
          };

  UT_ASSERT_MEM_EQUAL (gReturnResult, Expected, sizeof (Expected));

  return UNIT_TEST_PASSED;
}
