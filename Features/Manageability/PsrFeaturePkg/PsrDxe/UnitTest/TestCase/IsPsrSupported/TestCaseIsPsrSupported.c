/** @file
  Unit test case of the IsPsrSupported function.

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

#include "TestCaseIsPsrSupported.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <MeBiosPayloadHob.h>

/**
  Unit Test case for IsPsrSupported function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestIsPsrSupported (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  // Case 1: If mMbpHob is NULL, IsPsrSupported () should return false.
  mMbpHob = NULL;
  UT_ASSERT_FALSE (IsPsrSupported ());

  mMbpHob = AllocateZeroPool (sizeof (ME_BIOS_PAYLOAD_HOB));
  if (mMbpHob == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }

  // Case 2: If mMbpHob is not NULL and MeBiosPayload.PSR is 1, IsPsrSupported()
  //         should return TRUE.
  mMbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.PSR = 1;
  UT_ASSERT_TRUE (IsPsrSupported ());

  // Case 3: If mMbpHob is not NULL and MeBiosPayload.PSR is 0, IsPsrSupported()
  //         should return FALSE.
  mMbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.PSR = 0;
  UT_ASSERT_FALSE (IsPsrSupported ());

  FreePool (mMbpHob);

  return UNIT_TEST_PASSED;
}
