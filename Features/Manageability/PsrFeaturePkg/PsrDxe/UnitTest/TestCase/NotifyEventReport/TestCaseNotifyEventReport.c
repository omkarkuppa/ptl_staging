/** @file
  Unit test case of the NofityEventReport function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include "TestCaseNotifyEventReport.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <MeBiosPayloadHob.h>
#include <Protocol/PsrProtocol.h>
#include "PlatformServiceRecord.h"

/**
  Unit Test EFI_NOT_READY case for NofityEventReport function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestNotifyEventReport (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT32 PcdValue;
  mMbpHob = AllocateZeroPool (sizeof (ME_BIOS_PAYLOAD_HOB));
  if (mMbpHob == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }

  // Case 1: If MeBiosPayload.PsrLogState is 1 and VpdPcdPsrSupportedCapabilities
  //         is 0x2F, NotifyEventReport should call HeciEventReport() if input
  //         event ID is FW_RECOVERY_EVENT_ID and return Status got from
  //         HeciEventReport().
  mMbpHob->MeBiosPayload.Psr.PsrData.PsrLogState = 1;
  PcdValue = 0x2F;
  expect_value (LibPcdGetEx32, TokenNumber, _PCD_TOKEN_VpdPcdPsrSupportedCapabilities);
  will_return (LibPcdGetEx32, PcdValue);

  expect_value (HeciEventReport, EventId, FW_RECOVERY_EVENT_ID);
  expect_value (HeciEventReport, EventData, 0);
  will_return (HeciEventReport, EFI_SUCCESS);

  UT_ASSERT_EQUAL (NotifyEventReport (NULL, FW_RECOVERY_EVENT_ID, 0), EFI_SUCCESS);

  // Case 2: If MeBiosPayload.PsrLogState is 1 and VpdPcdPsrSupportedCapabilities
  //         is 0x4F, NotifyEventReport should call HeciEventReport() if input
  //         event ID is FW_UPDATE_EVENT_ID and return Status got from
  //         HeciEventReport().
  PcdValue = 0x4F;
  expect_value (LibPcdGetEx32, TokenNumber, _PCD_TOKEN_VpdPcdPsrSupportedCapabilities);
  will_return (LibPcdGetEx32, PcdValue);

  expect_value (HeciEventReport, EventId, FW_UPDATE_EVENT_ID);
  expect_value (HeciEventReport, EventData, 0);
  will_return (HeciEventReport, EFI_SUCCESS);

  UT_ASSERT_EQUAL (NotifyEventReport (NULL, FW_UPDATE_EVENT_ID, 0), EFI_SUCCESS);

  // Case 3: If MeBiosPayload.PsrLogState is 1 and VpdPcdPsrSupportedCapabilities
  //         is 0x8F, NotifyEventReport should call HeciEventReport() if input
  //         event ID is SYSTEM_HANG_EVENT_ID and return Status got from
  //         HeciEventReport().
  PcdValue = 0x8F;
  expect_value (LibPcdGetEx32, TokenNumber, _PCD_TOKEN_VpdPcdPsrSupportedCapabilities);
  will_return (LibPcdGetEx32, PcdValue);

  expect_value (HeciEventReport, EventId, SYSTEM_HANG_EVENT_ID);
  expect_value (HeciEventReport, EventData, 0);
  will_return (HeciEventReport, EFI_SUCCESS);

  UT_ASSERT_EQUAL (NotifyEventReport (NULL, SYSTEM_HANG_EVENT_ID, 0), EFI_SUCCESS);

  // Case 4: If MeBiosPayload.PsrLogState is 1 and VpdPcdPsrSupportedCapabilities
  //         is 0x11F, NotifyEventReport should call HeciEventReport() if input
  //         event ID is POWER_DROP_EVENT_ID and return Status got from
  //         HeciEventReport().
  PcdValue = 0x11F;
  expect_value (LibPcdGetEx32, TokenNumber, _PCD_TOKEN_VpdPcdPsrSupportedCapabilities);
  will_return (LibPcdGetEx32, PcdValue);

  expect_value (HeciEventReport, EventId, POWER_DROP_EVENT_ID);
  expect_value (HeciEventReport, EventData, 0);
  will_return (HeciEventReport, EFI_SUCCESS);

  UT_ASSERT_EQUAL (NotifyEventReport (NULL, POWER_DROP_EVENT_ID, 0), EFI_SUCCESS);

  // Case 5: If MeBiosPayload.PsrLogState is 0, NotifyEventReport should return
  //         EFI_NOT_READY.
  mMbpHob->MeBiosPayload.Psr.PsrData.PsrLogState = 0;
  PcdValue = 0x2F;
  expect_value (LibPcdGetEx32, TokenNumber, _PCD_TOKEN_VpdPcdPsrSupportedCapabilities);
  will_return (LibPcdGetEx32, PcdValue);

  UT_ASSERT_EQUAL (NotifyEventReport (NULL, FW_RECOVERY_EVENT_ID, 0), EFI_NOT_READY);

  // Case 6: If MeBiosPayload.PsrLogState is 1 and VpdPcdPsrSupportedCapabilities
  //         is 0x1F, NotifyEventReport should return EFI_UNSUPPORTED as the config of
  //         PSR capability is not supported OEM events.
  mMbpHob->MeBiosPayload.Psr.PsrData.PsrLogState = 1;
  PcdValue = 0x1F;
  expect_value (LibPcdGetEx32, TokenNumber, _PCD_TOKEN_VpdPcdPsrSupportedCapabilities);
  will_return (LibPcdGetEx32, PcdValue);

  UT_ASSERT_EQUAL (NotifyEventReport (NULL, FW_RECOVERY_EVENT_ID, 0), EFI_UNSUPPORTED);

  FreePool (mMbpHob);

  return UNIT_TEST_PASSED;
}
