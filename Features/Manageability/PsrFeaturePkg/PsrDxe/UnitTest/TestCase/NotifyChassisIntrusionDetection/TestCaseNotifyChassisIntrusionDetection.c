/** @file
  Unit test case of the NotifyChassisIntrusionDetection function.

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

#include "TestCaseNotifyChassisIntrusionDetection.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <MeBiosPayloadHob.h>
#include <Protocol/PsrProtocol.h>
#include "PlatformServiceRecord.h"

/**
  Unit Test case for NotifyChassisIntrusionDetection function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestNotifyChassisIntrusionDetection (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  mMbpHob = AllocateZeroPool (sizeof (ME_BIOS_PAYLOAD_HOB));
  if (mMbpHob == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  // Case 1: If MeBiosPayload.ChassisIntrusionEvent is 0,
  //         NotifyChassisIntrusionDetection should return EFI_NOT_READY.
  mMbpHob->MeBiosPayload.Psr.PsrData.ChassisIntrusionEvent = 0;
  UT_ASSERT_EQUAL (NotifyChassisIntrusionDetection (NULL), EFI_NOT_READY);

  // Case 2: If MeBiosPayload.ChassisIntrusionEvent is 1,
  //         NotifyChassisIntrusionDetection should return EFI_SUCCESS.
  mMbpHob->MeBiosPayload.Psr.PsrData.ChassisIntrusionEvent = 1;
  will_return (HeciSendChassisIntrDetEvent, EFI_SUCCESS);
  UT_ASSERT_EQUAL (NotifyChassisIntrusionDetection (NULL), EFI_SUCCESS);

  FreePool (mMbpHob);

  return UNIT_TEST_PASSED;
}
