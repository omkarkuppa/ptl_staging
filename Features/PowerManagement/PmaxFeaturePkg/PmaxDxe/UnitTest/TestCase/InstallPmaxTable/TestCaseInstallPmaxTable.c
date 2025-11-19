/**@file

    TestCase of InstallPmaxTable function.

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

#include "TestCaseInstallPmaxTable.h"
#include <PmaxDevDef.h>

extern VPD_PMAX_DEV                  *gMockPmaxCfg;
extern EFI_ACPI_TABLE_PROTOCOL       *gAcpiTableProtocol;
extern EFI_ACPI_DESCRIPTION_HEADER   *gPmaxTable;

/**

  Unit Test case for InstallPmaxTable function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
TestCaseInstallPmaxTable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS          Status;
  VOID                *ExpectedHandleBuffer;
  UINTN               PmaxBufferSize;
  UINTN               NumberOfHandles;

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  UT_ASSERT_NOT_NULL (gBS);

  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  gBS->HandleProtocol = MockBootServiceHandleProtocol;
  gBS->LocateProtocol = MockBootServiceLocateProtocol;

  ExpectedHandleBuffer = AllocateZeroPool (0x1000);
  UT_ASSERT_NOT_NULL (ExpectedHandleBuffer);
  NumberOfHandles = 1;

  expect_value (MockBootServiceLocateHandleBuffer, SearchType, ByProtocol);
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_value (MockBootServiceLocateHandleBuffer, SearchKey, NULL);
  expect_any (MockBootServiceLocateHandleBuffer, NumberHandles);
  expect_any (MockBootServiceLocateHandleBuffer, Buffer);
  will_return (MockBootServiceLocateHandleBuffer, NULL);
  will_return (MockBootServiceLocateHandleBuffer, NULL);
  will_return (MockBootServiceLocateHandleBuffer, EFI_NOT_FOUND);

  Status = InstallPmaxTable (NULL,NULL);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);

  expect_value (MockBootServiceLocateHandleBuffer, SearchType, ByProtocol);
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_value (MockBootServiceLocateHandleBuffer, SearchKey, NULL);
  expect_any (MockBootServiceLocateHandleBuffer, NumberHandles);
  expect_any (MockBootServiceLocateHandleBuffer, Buffer);
  will_return (MockBootServiceLocateHandleBuffer, 0xFF);
  will_return (MockBootServiceLocateHandleBuffer, ExpectedHandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  expect_any (MockBootServiceHandleProtocol, UserHandle);
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_any (MockBootServiceHandleProtocol, Interface);
  will_return (MockBootServiceHandleProtocol, gFwVol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  expect_value (MockFwVol2ReadFile, This, gFwVol);
  expect_value (MockFwVol2ReadFile, NameGuid, &gPmaxAcpiTableStorageGuid);
  expect_any (MockFwVol2ReadFile, Buffer);
  expect_any (MockFwVol2ReadFile, BufferSize);
  expect_any (MockFwVol2ReadFile, FoundType);
  expect_any (MockFwVol2ReadFile, FileAttributes);
  expect_any (MockFwVol2ReadFile, AuthenticationStatus);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  will_return (MockPmaxAcpiTableFile, "Build/PowerManagement/PmaxFeaturePkg/HostTest/NOOPT_VS2019/X64/PowerManagement/PmaxFeaturePkg/PmaxDxe/PmaxDxe/OUTPUT/PmaxDev.aml");
  Status = MockPmaxAcpiTableFile ((UINT8**) &gPmaxTable, &PmaxBufferSize);

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

  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gPmaxFeaturePkgTokenSpaceGuid_VpdPcdPmaxDevices);
  will_return (LibPcdGetExPtr, NULL);

  Status = InstallPmaxTable (NULL,NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  ExpectedHandleBuffer = AllocateZeroPool (0x100);
  UT_ASSERT_NOT_NULL (ExpectedHandleBuffer);

  expect_value (MockBootServiceLocateHandleBuffer, SearchType, ByProtocol);
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_value (MockBootServiceLocateHandleBuffer, SearchKey, NULL);
  expect_any (MockBootServiceLocateHandleBuffer, NumberHandles);
  expect_any (MockBootServiceLocateHandleBuffer, Buffer);
  will_return (MockBootServiceLocateHandleBuffer, 0xFF);
  will_return (MockBootServiceLocateHandleBuffer, ExpectedHandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  expect_any (MockBootServiceHandleProtocol, UserHandle);
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_any (MockBootServiceHandleProtocol, Interface);
  will_return (MockBootServiceHandleProtocol, gFwVol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  expect_value (MockFwVol2ReadFile, This, gFwVol);
  expect_value (MockFwVol2ReadFile, NameGuid, &gPmaxAcpiTableStorageGuid);
  expect_any (MockFwVol2ReadFile, Buffer);
  expect_any (MockFwVol2ReadFile, BufferSize);
  expect_any (MockFwVol2ReadFile, FoundType);
  expect_any (MockFwVol2ReadFile, FileAttributes);
  expect_any (MockFwVol2ReadFile, AuthenticationStatus);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

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

  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gPmaxFeaturePkgTokenSpaceGuid_VpdPcdPmaxDevices);
  will_return (LibPcdGetExPtr, gMockPmaxCfg);

  gAcpiTableProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));

  gAcpiTableProtocol->InstallAcpiTable = MockInstallAcpiTable;

  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  expect_value (MockBootServiceLocateProtocol, Registration, NULL);
  expect_any (MockBootServiceLocateProtocol, Interface);
  will_return (MockBootServiceLocateProtocol, gAcpiTableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  Status = InstallPmaxTable (NULL,NULL);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  FreePool(gBS);
  FreePool (gMockPmaxCfg);
  FreePool (gAcpiTableProtocol);
  FreePool (gFwVol);

return UNIT_TEST_PASSED;
}