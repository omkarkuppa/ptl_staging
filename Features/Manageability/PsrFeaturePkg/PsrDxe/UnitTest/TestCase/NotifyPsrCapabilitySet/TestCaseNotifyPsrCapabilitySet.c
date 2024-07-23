/** @file
  Unit test case of the NotifyPsrCapabilitySet function.

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

#include "TestCaseNotifyPsrCapabilitySet.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <MeBiosPayloadHob.h>
#include <Protocol/PsrProtocol.h>

/**
  Unit Test case for NotifyPsrCapabilitySet function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestNotifyPsrCapabilitySet (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT32  PcdValue;

  mMbpHob = AllocateZeroPool (sizeof (ME_BIOS_PAYLOAD_HOB));
  if (mMbpHob == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }

  // Case 1: If MeBiosPayload.PsrCapabilities is equal the value got from
  //         VpdPcdPsrSupportedCapabilities, NotifyPsrCapabilitySet() return
  //         EFI_SUCCESS without sending HeciSendPsrCapabilitySet() to update
  //         PSR capability.
  mMbpHob->MeBiosPayload.Psr.PsrCapabilities = PSR_CAPABILITY_MASK;
  PcdValue = PSR_CAPABILITY_MASK;
  expect_value (LibPcdGetEx32, TokenNumber, _PCD_TOKEN_VpdPcdPsrSupportedCapabilities);
  will_return (LibPcdGetEx32, PcdValue);
  UT_ASSERT_STATUS_EQUAL (NotifyPsrCapabilitySet (), EFI_SUCCESS);

  // Case 2: If MeBiosPayload.PsrCapabilities and the value got from
  //         VpdPcdPsrSupportedCapabilities are not equal,
  //         NotifyPsrCapabilitySet() should send call HeciSendPsrCapabilitySet()
  //         to update PSR capability and return Status got from HECI CMD.
  mMbpHob->MeBiosPayload.Psr.PsrCapabilities = PSR_CAPABILITY_MASK;
  PcdValue = 0x1F;
  expect_value (LibPcdGetEx32, TokenNumber, _PCD_TOKEN_VpdPcdPsrSupportedCapabilities);
  will_return (LibPcdGetEx32, PcdValue);

  expect_value (HeciSendPsrCapabilitySet, CapabilityMask, PcdValue & PSR_CAPABILITY_MASK);
  will_return (HeciSendPsrCapabilitySet, EFI_SUCCESS);

  UT_ASSERT_STATUS_EQUAL (NotifyPsrCapabilitySet (), EFI_SUCCESS);

  return UNIT_TEST_PASSED;
}
