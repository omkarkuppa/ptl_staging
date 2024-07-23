/**@file
    Unit test case of the Ec Dxe driver.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <TestCaseInstallSsdtAcpiTable.h>
#include <BootServicesMock.h>
#include <PrivateMock/PrivateMock.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/EcNvsArea.h>

extern
EFI_STATUS
InstallSsdtAcpiTable (
  IN GUID   TableStorageGuid,
  IN UINT32 Signature,
  IN UINT64 OemTableId
  );

/**
  Unit test for InstallSsdtAcpiTable based on SSDT AcpiTable

  @param[in]  Context    [Optional] An optional parameter that enables:
                         1) test-case reuse with varied parameters and
                         2) test-case re-entry for Target tests that need a
                         reboot.  This parameter is a VOID* and it is the
                         responsibility of the test author to ensure that the
                         contents are well understood by all test cases that may
                         consume it.
  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInstallSsdtAcpiTableSsdt (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol2Protocol;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;
  EFI_STATUS                     Status;
  EFI_HANDLE                     *HandleBuffer;
  EFI_ACPI_COMMON_HEADER         *Table;
  UINTN                          BufferSize;
  EFI_ACPI_COMMON_HEADER         *TargetTable;
  UINTN                          TargetBufferSize;
  EC_NVS_AREA_PROTOCOL           *EcNvsAreaProtocol;

  will_return (MockEcAcpiTableFile, "Build/EcFeaturePkg/HostTest/NOOPT_VS2019/X64/EcFeaturePkg/AcpiTables/EcAcpiTables/OUTPUT/EcSsdt/EcSsdt.aml");
  Status = MockEcAcpiTableFile ((UINT8**) &Table, &BufferSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);


  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));

  //
  // Mock gBS->LocateHandleBuffer
  //
  HandleBuffer = AllocateZeroPool (3 * sizeof (EFI_HANDLE));
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceLocateHandleBuffer, HandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, 3);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  //
  // Mock gEfiFirmwareVolume2ProtocolGuid
  //
  FwVol2Protocol = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  FwVol2Protocol->ReadFile = MockFwVol2ReadFile;
  will_return (MockFwVol2ReadFile, BufferSize);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  //
  // First time return EC SSDT table
  //
  FwVol2Protocol->ReadSection = MockFwVol2ReadSection;
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, Table);
  will_return (MockFwVol2ReadSection, BufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);

  //
  // Second time return ACPI table not found.
  //
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, Table);
  will_return (MockFwVol2ReadSection, BufferSize);
  will_return (MockFwVol2ReadSection, EFI_NOT_FOUND);


  //
  // Mock gBS->HandleProtocol
  //
  gBS->HandleProtocol = MockBootServiceHandleProtocol;
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceHandleProtocol, FwVol2Protocol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);


  //
  // Mock gEfiAcpiTableProtocolGuid
  //
  AcpiTableProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  AcpiTableProtocol->InstallAcpiTable = MockInstallAcpiTable;
  will_return (MockInstallAcpiTable, &TargetTable);
  will_return (MockInstallAcpiTable, &TargetBufferSize);
  will_return (MockInstallAcpiTable, EFI_SUCCESS);

  //
  // Mock gEcNvsAreaProtocolGuid
  //
  EcNvsAreaProtocol = AllocateZeroPool (sizeof (EC_NVS_AREA_PROTOCOL));
  EcNvsAreaProtocol->Area = AllocateZeroPool (sizeof (EC_NVS_AREA));

  //
  // Mock gBS->LocateProtocol
  //
  gBS->LocateProtocol = MockBootServiceLocateProtocol;
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  will_return (MockBootServiceLocateProtocol, AcpiTableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_value (MockBootServiceLocateProtocol, Protocol, &gEcNvsAreaProtocolGuid);
  will_return (MockBootServiceLocateProtocol, EcNvsAreaProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);


  //
  // Test to Update Install SSDT ACPI Table.
  //
  Status =  InstallSsdtAcpiTable (
              gEcSsdtAcpiTableStorageGuid,
              EFI_ACPI_6_5_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
              SIGNATURE_64 ('E','c','S','s','d','t',' ', 0)
              );

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Destroy Mock Data
  //
  FreePool (EcNvsAreaProtocol->Area);
  FreePool (EcNvsAreaProtocol);
  FreePool (FwVol2Protocol);
  FreePool (AcpiTableProtocol);
  FreePool (gBS);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for InstallSsdtAcpiTable based on ECDT AcpiTable

  @param[in]  Context    [Optional] An optional parameter that enables:
                         1) test-case reuse with varied parameters and
                         2) test-case re-entry for Target tests that need a
                         reboot.  This parameter is a VOID* and it is the
                         responsibility of the test author to ensure that the
                         contents are well understood by all test cases that may
                         consume it.
  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInstallSsdtAcpiTableEcdt (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol2Protocol;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;
  EFI_STATUS                     Status;
  EFI_HANDLE                     *HandleBuffer;
  EFI_ACPI_COMMON_HEADER         *Table;
  UINTN                          BufferSize;
  EFI_ACPI_COMMON_HEADER         *TargetTable;
  UINTN                          TargetBufferSize;
  EC_NVS_AREA_PROTOCOL           *EcNvsAreaProtocol;


  will_return (MockEcAcpiTableFile, "Build/EcFeaturePkg/HostTest/NOOPT_VS2019/X64/EcFeaturePkg/AcpiTables/EcAcpiTables/OUTPUT/Ecdt/Ecdt.acpi");
  Status = MockEcAcpiTableFile ((UINT8**) &Table, &BufferSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);


  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));

  //
  // Mock gBS->LocateHandleBuffer
  //
  HandleBuffer = AllocateZeroPool (3 * sizeof (EFI_HANDLE));
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceLocateHandleBuffer, HandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, 3);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  //
  // Mock gEfiFirmwareVolume2ProtocolGuid
  //
  FwVol2Protocol = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  FwVol2Protocol->ReadFile = MockFwVol2ReadFile;
  will_return (MockFwVol2ReadFile, BufferSize);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  //
  // First time return EC SSDT table
  //
  FwVol2Protocol->ReadSection = MockFwVol2ReadSection;
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, Table);
  will_return (MockFwVol2ReadSection, BufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);

  //
  // Second time return ACPI table not found.
  //
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, Table);
  will_return (MockFwVol2ReadSection, BufferSize);
  will_return (MockFwVol2ReadSection, EFI_NOT_FOUND);


  //
  // Mock gBS->HandleProtocol
  //
  gBS->HandleProtocol = MockBootServiceHandleProtocol;
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceHandleProtocol, FwVol2Protocol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);


  //
  // Mock gEfiAcpiTableProtocolGuid
  //
  AcpiTableProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  AcpiTableProtocol->InstallAcpiTable = MockInstallAcpiTable;
  will_return (MockInstallAcpiTable, &TargetTable);
  will_return (MockInstallAcpiTable, &TargetBufferSize);
  will_return (MockInstallAcpiTable, EFI_SUCCESS);

  //
  // Mock gEcNvsAreaProtocolGuid
  //
  EcNvsAreaProtocol = AllocateZeroPool (sizeof (EC_NVS_AREA_PROTOCOL));
  EcNvsAreaProtocol->Area = AllocateZeroPool (sizeof (EC_NVS_AREA));
  EcNvsAreaProtocol->Area->EcdtGpeNumber = 0x6E;

  //
  // Mock gBS->LocateProtocol
  //
  gBS->LocateProtocol = MockBootServiceLocateProtocol;
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  will_return (MockBootServiceLocateProtocol, AcpiTableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_value (MockBootServiceLocateProtocol, Protocol, &gEcNvsAreaProtocolGuid);
  will_return (MockBootServiceLocateProtocol, EcNvsAreaProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);


  //
  // Test to Update Install SSDT ACPI Table.
  //
  Status =  InstallSsdtAcpiTable (
              gEcSsdtAcpiTableStorageGuid,
              EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE_SIGNATURE,
              0
              );
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Destroy Mock Data
  //
  FreePool (EcNvsAreaProtocol->Area);
  FreePool (EcNvsAreaProtocol);
  FreePool (FwVol2Protocol);
  FreePool (AcpiTableProtocol);
  FreePool (gBS);

  return UNIT_TEST_PASSED;
}