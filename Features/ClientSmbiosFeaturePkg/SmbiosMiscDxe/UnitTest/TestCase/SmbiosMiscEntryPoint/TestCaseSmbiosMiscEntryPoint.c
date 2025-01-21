/** @file
  Unit test case of the SmbiosMiscEntryPoint function.

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
#include "TestCaseSmbiosMiscEntryPoint.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>
#include <SmbiosMiscDxeDefault.h>
#include <PrivateMock.h>

/**
  Unit Test case for SmbiosMiscEntryPoint function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSmbiosMiscEntryPoint (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN  Index;
  UINTN  Instance;
  EFI_SMBIOS_TABLE_HEADER  *DefaultHeader;
  EFI_STATUS               Status;

  //
  // Case 1: If PcdSmbiosPlatformInfoSize is 0, would not update smbios info.
  //
  expect_value (LibPcdGet64, TokenNumber, _PCD_TOKEN_PcdSmbiosPlatformInfo);
  will_return (LibPcdGet64, (UINTN) &mDefaultSmbiosPlatformInfo);
  expect_value (LibPcdGet64, TokenNumber, _PCD_TOKEN_PcdSmbiosPlatformInfoSize);
  will_return (LibPcdGet64, 0);

  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);
  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);
  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);

  Status = SmbiosMiscEntryPoint (NULL, NULL);

  UT_ASSERT_STATUS_EQUAL (Status, EFI_SUCCESS);

  //
  // Case 2: If PcdSmbiosPlatformInfo is 0, would not update smbios info and create
  //         event with UpdateMiscSmbiosCallback().
  //
  expect_value (LibPcdGet64, TokenNumber, _PCD_TOKEN_PcdSmbiosPlatformInfo);
  will_return (LibPcdGet64, 0);
  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);
  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);
  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);
  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);

  Status = SmbiosMiscEntryPoint (NULL, NULL);

  UT_ASSERT_STATUS_EQUAL (Status, EFI_SUCCESS);

  //
  // Case 3: successful get data from PcdSmbiosPlatformInfo and add smbios records
  //         to table.
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

  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);
  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);
  expect_not_value_count (EfiCreateEventReadyToBootEx, ReadyToBootEvent, NULL, 1);
  will_return (EfiCreateEventReadyToBootEx, EFI_SUCCESS);

  Status = SmbiosMiscEntryPoint (NULL, NULL);

  UT_ASSERT_STATUS_EQUAL (Status, EFI_SUCCESS);

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
