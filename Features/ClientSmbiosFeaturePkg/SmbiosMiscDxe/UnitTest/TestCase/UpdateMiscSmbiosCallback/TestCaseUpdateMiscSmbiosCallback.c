/** @file
  Unit test case of the UpdateMiscSmbiosCallback function.

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
#include "TestCaseUpdateMiscSmbiosCallback.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>
#include <Guid/Smbios.h>
#include <SmbiosMiscDxeDefault.h>
#include <PrivateMock.h>

/**
  Unit Test case for UpdateMiscSmbiosCallback function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestUpdateMiscSmbiosCallback (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN  Index;
  UINTN  Instance;
  EFI_SMBIOS_TABLE_HEADER  *DefaultHeader;

  //
  // Case 1: If PcdSmbiosPlatformInfo is NULL, nothing to update.
  //
  expect_value (LibPcdGet64, TokenNumber, _PCD_TOKEN_PcdSmbiosPlatformInfo);
  will_return (LibPcdGet64, 0);

  will_return (MockBootServiceCloseEvent, EFI_SUCCESS);

  UpdateMiscSmbiosCallback (NULL, NULL);

  UT_ASSERT_EQUAL (gSmbiosTableIndex, 0);

  //
  // Case 2: If PcdSmbiosPlatformInfo is not NULL and PcdSmbiosPlatformInfoSize is not 0,
  //         update smbios records.
  //
  expect_value (LibPcdGet64, TokenNumber, _PCD_TOKEN_PcdSmbiosPlatformInfo);
  will_return (LibPcdGet64, (UINTN) &mDefaultSmbiosPlatformInfo);
  expect_value (LibPcdGet64, TokenNumber, _PCD_TOKEN_PcdSmbiosPlatformInfoSize);
  will_return (LibPcdGet64, DEFAULT_SMBIOS_PLATFORM_INFO_SIZE);

  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiSmbiosProtocolGuid);
  will_return (MockBootServiceLocateProtocol, gSmbiosProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_not_value_count (MockSmbiosAdd, SmbiosHandle, NULL, -1);
  will_return_always (MockSmbiosAdd, EFI_SUCCESS);

  will_return (MockBootServiceCloseEvent, EFI_SUCCESS);

  UpdateMiscSmbiosCallback (NULL, NULL);

  Instance = 0;

  for (Index = 0; Index < DEFAULT_SMBIOS_PLATFORM_INFO_SIZE; Index++) {
    if (mDefaultSmbiosPlatformInfo[Index].NoOfInstances > 0) {
      DefaultHeader = (EFI_SMBIOS_TABLE_HEADER *) mDefaultSmbiosPlatformInfo[Index].Info;
      UT_ASSERT_EQUAL (gSmbiosTable[Instance]->Type, DefaultHeader->Type);
      Instance = Instance + 1;
    }
  }

  return UNIT_TEST_PASSED;
}
