/** @file
  Unit test case of the SmbiosStrInit function.

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
#include "TestCaseSmbiosStrInit.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Smbios.h>
#include <PrivateMock.h>
#include "SmbiosMisc.h"

/**
  Unit Test case for SmbiosStrInit function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSmbiosStrInit (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;
  CHAR16      *SmbiosTableStrings[2];
  CHAR16      *StrBufferPtr;
  CHAR16      *StrBufferStart;
  CHAR16      *StringForTest;
  UINTN       StringNumber;
  UINTN       StrBuffSize;
  UINTN       StringLength;
  UINTN       TableIndex;

  StringForTest = UNIT_TEST_SMBIOS_STRING;

  //
  // Case 1: Successful initialize SMBIOS table strings.
  //
  StringNumber   = 0;
  StrBuffSize    = SMBIOS_STRING_MAX_LENGTH;
  StrBufferStart = AllocateZeroPool (StrBuffSize * sizeof (CHAR16));
  StrBufferPtr   = StrBufferStart;
  StringLength   = 0;

  for (TableIndex = 1; TableIndex <= 2; TableIndex++) {
    SmbiosTableStrings[TableIndex] = NULL;

    Status = SmbiosStrInit (&SmbiosTableStrings[TableIndex], StringForTest, &StrBufferPtr, &StringNumber, &StringLength, &StrBuffSize);
    UT_ASSERT_STATUS_EQUAL (Status, EFI_SUCCESS);
    UT_ASSERT_EQUAL (StrCmp (SmbiosTableStrings[TableIndex], StringForTest), 0);
    UT_ASSERT_EQUAL (StringNumber, TableIndex);
    UT_ASSERT_EQUAL (StringLength, TableIndex * StrLen (UNIT_TEST_SMBIOS_STRING));
    UT_ASSERT_EQUAL (StrBuffSize, SMBIOS_STRING_MAX_LENGTH - TableIndex * (StrLen (UNIT_TEST_SMBIOS_STRING) + 1));
  }

  FreePool (StrBufferStart);

  //
  // Case 2: Return EFI_INVALID_PARAMETER when Destination is not NULL.
  //
  StringNumber   = 0;
  StrBuffSize    = SMBIOS_STRING_MAX_LENGTH;
  StrBufferStart = AllocateZeroPool (StrBuffSize * sizeof (CHAR16));
  StrBufferPtr   = StrBufferStart;
  StringLength   = 0;

  Status = SmbiosStrInit (&SmbiosTableStrings[0], StringForTest, &StrBufferPtr, &StringNumber, &StringLength, &StrBuffSize);
  UT_ASSERT_STATUS_EQUAL (Status, EFI_INVALID_PARAMETER);
  FreePool (StrBufferStart);

  //
  // Case 3: Return EFI_INVALID_PARAMETER when StrBuffer is NULL.
  //
  StringNumber = 0;
  StrBuffSize  = SMBIOS_STRING_MAX_LENGTH;
  StrBufferPtr = NULL;
  StringLength = 0;
  SmbiosTableStrings[0] = NULL;

  Status = SmbiosStrInit (&SmbiosTableStrings[0], StringForTest, &StrBufferPtr, &StringNumber, &StringLength, &StrBuffSize);
  UT_ASSERT_STATUS_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Case 4: Return EFI_BUFFER_TOO_SMALL when StrBuffSize is 0.
  //
  StringNumber   = 0;
  StrBuffSize    = 0;
  StrBufferStart = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH * sizeof (CHAR16));
  StrBufferPtr   = StrBufferStart;
  StringLength   = 0;
  SmbiosTableStrings[0] = NULL;

  Status = SmbiosStrInit (&SmbiosTableStrings[0], StringForTest, &StrBufferPtr, &StringNumber, &StringLength, &StrBuffSize);
  UT_ASSERT_STATUS_EQUAL (Status, EFI_BUFFER_TOO_SMALL);
  FreePool (StrBufferStart);

  //
  // Case 5: Return EFI_BUFFER_TOO_SMALL when StrBuffSize is smaller than string length.
  //
  StringNumber   = 0;
  StrBuffSize    = StrLen (StringForTest) - 1;
  StrBufferStart = AllocateZeroPool (StrBuffSize * sizeof (CHAR16));
  StrBufferPtr   = StrBufferStart;
  StringLength   = 0;
  SmbiosTableStrings[0] = NULL;

  Status = SmbiosStrInit (&SmbiosTableStrings[0], StringForTest, &StrBufferPtr, &StringNumber, &StringLength, &StrBuffSize);
  UT_ASSERT_STATUS_EQUAL (Status, EFI_BUFFER_TOO_SMALL);
  FreePool (StrBufferStart);

  return UNIT_TEST_PASSED;
}
