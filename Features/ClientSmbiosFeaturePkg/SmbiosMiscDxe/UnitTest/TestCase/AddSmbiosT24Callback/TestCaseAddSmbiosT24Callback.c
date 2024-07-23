/** @file
  Unit test case of the AddSmbiosT24Callback function.

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
#include "TestCaseAddSmbiosT24Callback.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <PrivateMock.h>

/**
  Check the result of test case with AddSmbiosT24Callback.

  @param[in]  Context                          The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED                 The result is correct.
  @retval     UNIT_TEST_ERROR_TEST_FAILED      The result is wrong.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddSmbiosT24Callback (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  //
  // Case 1: If no Password Status variable exists, Set feature to not implemented.
  //         If PcdPasswordStatusAdminName is 0xFF, HardwareSecuritySettings should be 0xAA
  //
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiSmbiosProtocolGuid);
  will_return (MockBootServiceLocateProtocol, gSmbiosProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdPasswordStatusAdminName);
  will_return (LibPcdGet8, 0xFF);

  expect_not_value (MockSmbiosAdd, SmbiosHandle, NULL);
  will_return (MockSmbiosAdd, EFI_SUCCESS);
  will_return (MockBootServiceCloseEvent, EFI_SUCCESS);

  AddSmbiosT24Callback (NULL, NULL);

  UT_ASSERT_EQUAL (gSmbiosTable[0]->Type, EFI_SMBIOS_TYPE_HARDWARE_SECURITY);
  UT_ASSERT_EQUAL (gSmbiosTable[0]->Handle, SMBIOS_HANDLE_PI_RESERVED);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE24 *) gSmbiosTable[0])->HardwareSecuritySettings, 0xAA);

  //
  // Case 2: If PcdPasswordStatusAdminName and PcdPasswordStatusUserName are 0x3,
  //         HardwareSecuritySettings should be 0xBE.
  //
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiSmbiosProtocolGuid);
  will_return (MockBootServiceLocateProtocol, gSmbiosProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdPasswordStatusAdminName);
  will_return (LibPcdGet8, 0x3);
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdPasswordStatusUserName);
  will_return (LibPcdGet8, 0x3);
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdPasswordStatusAdminName);
  will_return (LibPcdGet8, 0x3);

  expect_not_value (MockSmbiosAdd, SmbiosHandle, NULL);
  will_return (MockSmbiosAdd, EFI_SUCCESS);
  will_return (MockBootServiceCloseEvent, EFI_SUCCESS);

  AddSmbiosT24Callback (NULL, NULL);

  UT_ASSERT_EQUAL (gSmbiosTable[1]->Type, EFI_SMBIOS_TYPE_HARDWARE_SECURITY);
  UT_ASSERT_EQUAL (gSmbiosTable[1]->Handle, SMBIOS_HANDLE_PI_RESERVED);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE24 *) gSmbiosTable[1])->HardwareSecuritySettings, 0xBE);

  return UNIT_TEST_PASSED;
}
