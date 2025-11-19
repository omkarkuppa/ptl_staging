/** @file
  Unit test case of the MiscCommonSmbiosFunction function.

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
#include "TestCaseMiscCommonSmbiosFunction.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <SmbiosPlatformInfoTable.h>
#include <PrivateMock.h>
#include "SmbiosMisc.h"

/**
  Unit Test case for MiscCommonSmbiosFunction function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestMiscCommonSmbiosFunction (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS   Status;

  //
  // Case 1: success add smbios type 0 record.
  //
  expect_not_value (MockSmbiosAdd, SmbiosHandle, NULL);
  will_return (MockSmbiosAdd, EFI_SUCCESS);
  Status = MiscCommonSmbiosFunction (
             (VOID*) ((SMBIOS_PLATFORM_INFO *) (mDefaultSmbiosPlatformInfo)->Info),
             (VOID*) ((SMBIOS_PLATFORM_INFO *) (mDefaultSmbiosPlatformInfo)->Strings),
             mDefaultSmbiosPlatformInfo->StringCount,
             mDefaultSmbiosPlatformInfo->IsPoolAllocated,
             mDefaultSmbiosPlatformInfo->NoOfInstances,
             gSmbiosProtocol
             );
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (gSmbiosTable[0]->Type, ((EFI_SMBIOS_TABLE_HEADER *) mDefaultSmbiosPlatformInfo[0].Info)->Type);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE0 *) gSmbiosTable[0])->Vendor, 1);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE0 *) gSmbiosTable[0])->BiosVersion, 2);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE0 *) gSmbiosTable[0])->BiosReleaseDate, 3);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE0 *) gSmbiosTable[0])->BiosSize, 0);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE0 *) gSmbiosTable[0])->EmbeddedControllerFirmwareMajorRelease, 0xFF);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE0 *) gSmbiosTable[0])->EmbeddedControllerFirmwareMinorRelease, 0xFF);

  //
  // Case 2: return EFI_INVALID_PARAMETER when RecordData is NULL.
  //
  Status = MiscCommonSmbiosFunction (
             NULL,
             (VOID*) ((SMBIOS_PLATFORM_INFO *) (mDefaultSmbiosPlatformInfo)->Strings),
             mDefaultSmbiosPlatformInfo->StringCount,
             mDefaultSmbiosPlatformInfo->IsPoolAllocated,
             mDefaultSmbiosPlatformInfo->NoOfInstances,
             gSmbiosProtocol
             );
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Case 3: return EFI_INVALID_PARAMETER when String count is larger than MISC_MAX_SMBIOS_TABLE_STRINGS.
  //
  Status = MiscCommonSmbiosFunction (
             (VOID*) ((SMBIOS_PLATFORM_INFO *) (mDefaultSmbiosPlatformInfo)->Info),
             (VOID*) ((SMBIOS_PLATFORM_INFO *) (mDefaultSmbiosPlatformInfo)->Strings),
             MISC_MAX_SMBIOS_TABLE_STRINGS + 1,
             mDefaultSmbiosPlatformInfo->IsPoolAllocated,
             mDefaultSmbiosPlatformInfo->NoOfInstances,
             gSmbiosProtocol
             );
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  return UNIT_TEST_PASSED;
}
