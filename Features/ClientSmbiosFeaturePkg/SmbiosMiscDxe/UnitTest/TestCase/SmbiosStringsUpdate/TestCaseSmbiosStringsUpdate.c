/** @file
  Unit test case of the SmbiosStringsUpdate function.

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
#include "TestCaseSmbiosStringsUpdate.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <PrivateMock.h>

/**
  Unit Test case for SmbiosStringsUpdate function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSmbiosStringsUpdate (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  CHAR8   *StrStart;
  UINTN   StringNumber;
  UINT8   *SmbiosRecord;
  CHAR16  *SmbiosTableStrings[1];

  StringNumber = 1;
  SmbiosRecord = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH);
  StrStart     = (CHAR8 *) (SmbiosRecord);
  SmbiosTableStrings[0] = UNIT_TEST_SMBIOS_STRING;

  SmbiosStringsUpdate ((CHAR16 **) SmbiosTableStrings, (UINT8 *) StrStart, StringNumber, SMBIOS_STRING_MAX_LENGTH);

  UT_ASSERT_MEM_EQUAL (StrStart, "StringForSmbiosUnitTest", sizeof ("StringForSmbiosUnitTest"));

  FreePool (SmbiosRecord);

  return UNIT_TEST_PASSED;
}
