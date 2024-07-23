/**@file

    Testcase of the FindPmaxTable function.

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

#include "TestCaseFindPmaxTable.h"

extern  EFI_ACPI_DESCRIPTION_HEADER *gPmaxTable;
/**
  Unit Test case for FindPmaxTable function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseFindPmaxTable(
  IN UNIT_TEST_CONTEXT  Context
  )
{
    UINT8* Buffer;

  if (gPmaxTable->OemTableId == SIGNATURE_64 ('P', 'm', 'a', 'x', '_', 'D', 'e', 'v')) {
    DEBUG ((DEBUG_ERROR, "Pmax ACPI Table found.\n"));
    Buffer = (UINT8*) &gPmaxTable->Signature;
    DEBUG ((DEBUG_ERROR, "Table Signature : %c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3]
      ));
    DEBUG ((DEBUG_ERROR, "Table Length : 0x%X\n", gPmaxTable->Length));
    DEBUG ((DEBUG_ERROR, "Table Revision : 0x%X\n", gPmaxTable->Revision));
    DEBUG ((DEBUG_ERROR, "Table Checksum : 0x%X\n", gPmaxTable->Checksum));
    Buffer = (UINT8*) &gPmaxTable->OemId;
    DEBUG ((DEBUG_ERROR, "Table OemId : %c%c%c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3],
      Buffer[4],
      Buffer[5]
      ));
    Buffer = (UINT8*) &gPmaxTable->OemTableId;
    DEBUG ((DEBUG_ERROR, "Table OemTableId : %c%c%c%c%c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3],
      Buffer[4],
      Buffer[5],
      Buffer[6],
      Buffer[7]
      ));
    DEBUG ((DEBUG_ERROR, "Table OemRevision : 0x%X\n", gPmaxTable->OemRevision));
    DEBUG ((DEBUG_ERROR, "Table CreatorId : 0x%X\n", gPmaxTable->CreatorId));
    DEBUG ((DEBUG_ERROR, "Table CreatorRevision : 0x%X\n", gPmaxTable->CreatorRevision));
  } else {
    DEBUG ((DEBUG_ERROR, "Pmax ACPI Table isn't find.\n"));
    UT_ASSERT_FALSE (TRUE);
  }

  DEBUG ((DEBUG_ERROR, "[FindPmaxTable] Testing result is pass.\n"));
  return UNIT_TEST_PASSED;
}

/**
  Unit Test case for TestCaseGetHandlesWithFvProtocol function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseGetHandlesWithFvProtocol(
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;
  VOID        *ExpectedHandleBuffer;
  UINTN       NumberOfHandles;
  EFI_HANDLE  *HandleBuffer;

  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;

  ExpectedHandleBuffer = AllocateZeroPool (0x1000);
  UT_ASSERT_NOT_NULL (ExpectedHandleBuffer);

  expect_value (MockBootServiceLocateHandleBuffer, SearchType, ByProtocol);
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_value (MockBootServiceLocateHandleBuffer, SearchKey, NULL);
  expect_any (MockBootServiceLocateHandleBuffer, NumberHandles);
  expect_any (MockBootServiceLocateHandleBuffer, Buffer);
  will_return (MockBootServiceLocateHandleBuffer, 0xFF);
  will_return (MockBootServiceLocateHandleBuffer, ExpectedHandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  Status = GetHandlesWithFvProtocol (&NumberOfHandles, &HandleBuffer);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (NumberOfHandles, 0xFF);
  UT_ASSERT_EQUAL (HandleBuffer, ExpectedHandleBuffer);

  FreePool (ExpectedHandleBuffer);

  return UNIT_TEST_PASSED;
}

/**
  Unit Test case for TestCaseGetPmaxFirmwareVolume function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseGetPmaxFirmwareVolume(
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN                         NumberOfHandles;
  EFI_HANDLE                    *HandleBuffer;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FvVol;

  NumberOfHandles = 1;
  HandleBuffer = AllocateZeroPool (sizeof (EFI_HANDLE));

  gFwVol->ReadFile    = MockFwVol2ReadFile;

  gBS->HandleProtocol = MockBootServiceHandleProtocol;

  //
  // Mock function for gBS->HandleProtocol
  //
  expect_any (MockBootServiceHandleProtocol, UserHandle);
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_any (MockBootServiceHandleProtocol, Interface);
  will_return (MockBootServiceHandleProtocol, gFwVol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  //
  // Mock function for FwVol->ReadFile
  //
  expect_value (MockFwVol2ReadFile, This, gFwVol);
  expect_value (MockFwVol2ReadFile, NameGuid, &gPmaxAcpiTableStorageGuid);
  expect_any (MockFwVol2ReadFile, Buffer);
  expect_any (MockFwVol2ReadFile, BufferSize);
  expect_any (MockFwVol2ReadFile, FoundType);
  expect_any (MockFwVol2ReadFile, FileAttributes);
  expect_any (MockFwVol2ReadFile, AuthenticationStatus);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  FvVol = GetPmaxFirmwareVolume (NumberOfHandles, NULL);
  UT_ASSERT_EQUAL (FvVol, NULL);

  FvVol = GetPmaxFirmwareVolume (NumberOfHandles, HandleBuffer);
  UT_ASSERT_NOT_NULL (FvVol);

  return UNIT_TEST_PASSED;
}

/**
  Unit Test case for TestCaseReadPmaxAcpiFile function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseReadPmaxAcpiFile(
  IN UNIT_TEST_CONTEXT  Context
  )
{
EFI_STATUS  Status;
UINTN                       PmaxBufferSize;
EFI_ACPI_DESCRIPTION_HEADER   *PmaxTable;
EFI_ACPI_DESCRIPTION_HEADER   *zeroPmaxTable;

will_return (MockPmaxAcpiTableFile, "Build/PowerManagement/PmaxFeaturePkg/HostTest/NOOPT_VS2019/X64/PowerManagement/PmaxFeaturePkg/PmaxDxe/PmaxDxe/OUTPUT/PmaxDev.aml");
Status = MockPmaxAcpiTableFile ((UINT8**) &gPmaxTable, &PmaxBufferSize);
UT_ASSERT_NOT_EFI_ERROR (Status);

gFwVol->ReadSection = MockFwVol2ReadSection;

Status = ReadPmaxAcpiFile (NULL, &PmaxTable);
UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);

zeroPmaxTable = AllocateZeroPool (sizeof (EFI_ACPI_DESCRIPTION_HEADER));

expect_value (MockFwVol2ReadSection, This, gFwVol);
expect_value (MockFwVol2ReadSection, NameGuid, &gPmaxAcpiTableStorageGuid);
expect_any (MockFwVol2ReadSection, SectionType);
expect_any (MockFwVol2ReadSection, SectionInstance);
expect_any (MockFwVol2ReadSection, Buffer);
expect_any (MockFwVol2ReadSection, BufferSize);
expect_any (MockFwVol2ReadSection, AuthenticationStatus);
will_return (MockFwVol2ReadSection, zeroPmaxTable);
will_return (MockFwVol2ReadSection, PmaxBufferSize);
will_return (MockFwVol2ReadSection, EFI_SUCCESS);


//
// Mock function for FwVol->ReadSection
//
expect_value (MockFwVol2ReadSection, This, gFwVol);
expect_value (MockFwVol2ReadSection, NameGuid, &gPmaxAcpiTableStorageGuid);
expect_any (MockFwVol2ReadSection, SectionType);
expect_any (MockFwVol2ReadSection, SectionInstance);
expect_any (MockFwVol2ReadSection, Buffer);
expect_any (MockFwVol2ReadSection, BufferSize);
expect_any (MockFwVol2ReadSection, AuthenticationStatus);
will_return (MockFwVol2ReadSection, gPmaxTable);
will_return (MockFwVol2ReadSection, PmaxBufferSize);
will_return (MockFwVol2ReadSection, EFI_SUCCESS);

Status = ReadPmaxAcpiFile (gFwVol,&PmaxTable);
UT_ASSERT_EQUAL (gPmaxTable, PmaxTable);
UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
UT_ASSERT_EQUAL(PmaxTable->OemTableId,SIGNATURE_64 ('P', 'm', 'a', 'x', '_', 'D', 'e', 'v'));
return UNIT_TEST_PASSED;
}

