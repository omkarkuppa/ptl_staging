/** @file
  Unit test case of the AddSmbiosT14Callback function.

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
#include "TestCaseAddSmbiosT14Callback.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SmBios.h>
#include <Guid/SmBios.h>
#include <PrivateMock.h>
#include "SmbiosMisc.h"

/**
  Unit Test case for AddSmbiosT14Callback function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddSmbiosT14Callback (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_SMBIOS_TABLE_HEADER  *SmbiosTableHeader;
  EFI_SMBIOS_TABLE_HEADER  *SmbiosTableHeaderHandleReservedBegin;

  SmbiosTableHeader = AllocateZeroPool (sizeof (EFI_SMBIOS_TABLE_HEADER));
  if (SmbiosTableHeader == NULL) {
    DEBUG ((DEBUG_ERROR, "UnitTestAddSmbiosT14Callback: Failed to allocate memory for EFI_SMBIOS_TABLE_HEADER\n"));
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  SmbiosTableHeader->Type   = 0xDD;
  SmbiosTableHeader->Length = sizeof (EFI_SMBIOS_TABLE_HEADER);
  SmbiosTableHeader->Handle = SMBIOS_HANDLE_PI_RESERVED;

  SmbiosTableHeaderHandleReservedBegin = AllocateZeroPool (sizeof (EFI_SMBIOS_TABLE_HEADER));
  if (SmbiosTableHeaderHandleReservedBegin == NULL) {
    DEBUG ((DEBUG_ERROR, "UnitTestAddSmbiosT14Callback: Failed to allocate memory for EFI_SMBIOS_TABLE_HEADER\n"));
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  SmbiosTableHeaderHandleReservedBegin->Type   = 0xDD;
  SmbiosTableHeaderHandleReservedBegin->Length = sizeof (EFI_SMBIOS_TABLE_HEADER);
  SmbiosTableHeaderHandleReservedBegin->Handle = SMBIOS_HANDLE_RESERVED_BEGIN;

  //
  // Case 1: Successful add records to the SMBIOS table
  //
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiSmbiosProtocolGuid);
  will_return (MockBootServiceLocateProtocol, gSmbiosProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_not_value (MockSmbiosGetNext, SmbiosHandle, NULL);
  will_return (MockSmbiosGetNext, SmbiosTableHeader->Handle);
  will_return (MockSmbiosGetNext, SmbiosTableHeader);
  will_return (MockSmbiosGetNext, EFI_SUCCESS);

  expect_not_value (MockSmbiosGetNext, SmbiosHandle, NULL);
  will_return (MockSmbiosGetNext, SmbiosTableHeaderHandleReservedBegin->Handle);
  will_return (MockSmbiosGetNext, SmbiosTableHeaderHandleReservedBegin);
  will_return (MockSmbiosGetNext, EFI_SUCCESS);

  expect_not_value (MockSmbiosGetNext, SmbiosHandle, NULL);
  will_return (MockSmbiosGetNext, NULL);
  will_return (MockSmbiosGetNext, NULL);
  will_return (MockSmbiosGetNext, EFI_OUT_OF_RESOURCES);

  expect_not_value (MockSmbiosAdd, SmbiosHandle, NULL);
  will_return (MockSmbiosAdd, EFI_SUCCESS);
  will_return (MockBootServiceCloseEvent, EFI_SUCCESS);

  AddSmbiosT14Callback (NULL, NULL);

  UT_ASSERT_EQUAL (gSmbiosTable[0]->Type, EFI_SMBIOS_TYPE_GROUP_ASSOCIATIONS);
  UT_ASSERT_EQUAL (gSmbiosTable[0]->Handle, SMBIOS_HANDLE_PI_RESERVED);
  UT_ASSERT_EQUAL (gSmbiosTable[0]->Length, (UINT8) (sizeof (SMBIOS_TABLE_TYPE14) + sizeof (GROUP_STRUCT)));
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE14 *) gSmbiosTable[0])->GroupName, 1);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE14 *) gSmbiosTable[0])->Group[0].ItemType, 0xDD);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE14 *) gSmbiosTable[0])->Group[0].ItemHandle, SMBIOS_HANDLE_PI_RESERVED);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE14 *) gSmbiosTable[0])->Group[1].ItemType, 0xDD);
  UT_ASSERT_EQUAL (((SMBIOS_TABLE_TYPE14 *) gSmbiosTable[0])->Group[1].ItemHandle, SMBIOS_HANDLE_RESERVED_BEGIN);
  //
  // Check the extra zero which marks the end of the record.
  //
  UT_ASSERT_EQUAL (((UINT8 *) gSmbiosTable[0])[sizeof (SMBIOS_TABLE_TYPE14) + (UINT64) sizeof (T14_FVI_STRING) + sizeof (GROUP_STRUCT)], 0);

  //
  // Case 2: If smbios type 14 table is already in SMBIOS records, no action taken.
  //
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiSmbiosProtocolGuid);
  will_return (MockBootServiceLocateProtocol, gSmbiosProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_not_value (MockSmbiosGetNext, SmbiosHandle, NULL);
  will_return (MockSmbiosGetNext, gSmbiosTable[0]->Handle);
  will_return (MockSmbiosGetNext, gSmbiosTable[0]);
  will_return (MockSmbiosGetNext, EFI_SUCCESS);

  AddSmbiosT14Callback (NULL, NULL);

  //
  // gSmbiosTableIndex remains 1 as only previous added table in case 1.
  //
  UT_ASSERT_EQUAL (gSmbiosTableIndex, 1);

  return UNIT_TEST_PASSED;
}
