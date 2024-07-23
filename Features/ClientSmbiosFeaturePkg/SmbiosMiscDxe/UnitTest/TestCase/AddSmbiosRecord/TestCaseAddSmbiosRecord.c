/** @file
  Unit test case of the AddSmbiosRecord function.

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
#include "TestCaseAddSmbiosRecord.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <PrivateMock.h>
#include "SmbiosMisc.h"

/**
  Unit Test case for AddSmbiosRecord function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddSmbiosRecord (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS         Status;
  EFI_SMBIOS_HANDLE  SmbiosHandle;
  UINT8              *SmbiosRecord;

  //
  // Take Smbios Type 2 for testing, and two extra zero are used to mark the end of the record.
  //
  SmbiosRecord = AllocateZeroPool (sizeof (SMBIOS_TABLE_TYPE2) + 2);
  if (SmbiosRecord == NULL) {
    DEBUG ((DEBUG_ERROR, "UnitTestAddSmbiosRecord: Failed to allocate memory for SmbiosRecord\n"));
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  CopyMem (SmbiosRecord, mDefaultSmbiosPlatformInfo[BaseBoardManufacturer].Info, sizeof (SMBIOS_TABLE_TYPE2));

  expect_not_value (MockSmbiosAdd, SmbiosHandle, NULL);
  will_return (MockSmbiosAdd, EFI_SUCCESS);

  Status = AddSmbiosRecord (gSmbiosProtocol, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  UT_ASSERT_EQUAL (gSmbiosTable[0]->Type, EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION);
  UT_ASSERT_MEM_EQUAL (
    (SMBIOS_TABLE_TYPE2 *) gSmbiosTable[0],
    (SMBIOS_TABLE_TYPE2 *) mDefaultSmbiosPlatformInfo[BaseBoardManufacturer].Info,
    sizeof (SMBIOS_TABLE_TYPE2)
    );
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  return UNIT_TEST_PASSED;
}
