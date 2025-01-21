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
#include <TestCaseUpdateAcpiDataTable.h>
#include "BootServicesMock.h"
#include <PrivateMock/PrivateMock.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/EcNvsArea.h>
#include <Library/DebugLib.h>

extern
EFI_STATUS
UpdateAcpiDataTable (
  IN OUT  EFI_ACPI_COMMON_HEADER        *Table
  );

/**
  Unit test for UpdateAcpiDataTable Ssdt.
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
UnitTestUpdateAcpiDataTableSsdt (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN                   FileSize;
  EFI_ACPI_COMMON_HEADER  *Table;
  EFI_STATUS              Status;
  EC_NVS_AREA_PROTOCOL    *EcNvsAreaProtocol;
  UINT8                   *SsdtPointer;
  UINT32                  *Signature;
  UINT8                   *Operation;
  UINT32                  *Address;
  UINT16                  *Size;

  //
  // Mock EcSsdt.asl
  //
  will_return (MockEcAcpiTableFile, "Build/EcFeaturePkg/HostTest/NOOPT_VS2019/X64/EcFeaturePkg/AcpiTables/EcAcpiTables/OUTPUT/EcSsdt/EcSsdt.aml");
  Status = MockEcAcpiTableFile ((UINT8**) &Table, &FileSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Mock gEcNvsAreaProtocolGuid
  //
  EcNvsAreaProtocol = AllocateZeroPool (sizeof (EC_NVS_AREA_PROTOCOL));

  //
  // Mock gBS->LocateProtocol
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->LocateProtocol = MockBootServiceLocateProtocol;
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEcNvsAreaProtocolGuid);
  will_return (MockBootServiceLocateProtocol, EcNvsAreaProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  //
  // Test Update Acpi Table for SSDT
  //
  Status = UpdateAcpiDataTable (Table);

  UT_ASSERT_MEM_EQUAL (
    &(((EFI_ACPI_DESCRIPTION_HEADER *) Table)->OemId),
    PcdGetPtr (PcdAcpiDefaultOemId),
    AsciiStrLen (PcdGetPtr (PcdAcpiDefaultOemId))
    );
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  for (SsdtPointer = (UINT8 *)Table;
    SsdtPointer < ((UINT8 *)Table + ((EFI_ACPI_COMMON_HEADER *) Table)->Length - sizeof (UINT32));
    SsdtPointer++
    ) {
    Signature = (UINT32 *) SsdtPointer;
    if (*Signature == SIGNATURE_32 ('E', 'N', 'V', 'S')) {
      Operation = SsdtPointer - 1;
      if (*Operation == AML_EXT_REGION_OP) {
        Address = (UINT32 *) (SsdtPointer + 6);
        UT_ASSERT_EQUAL (*Address, (UINT32)(UINTN) EcNvsAreaProtocol->Area);
        Size = (UINT16 *) (SsdtPointer + 11);
        UT_ASSERT_EQUAL (*Size, sizeof (EC_NVS_AREA));
      }
      break;
    }
  }

  //
  // Destroy Mock Data
  //
  FreePool (gBS);
  FreePool (EcNvsAreaProtocol->Area);
  FreePool (EcNvsAreaProtocol);
  FreePool (Table);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for UpdateAcpiDataTable Ecdt.
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
UnitTestUpdateAcpiDataTableEcdt (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN                   FileSize;
  EFI_ACPI_COMMON_HEADER  *Table;
  EFI_STATUS              Status;
  EC_NVS_AREA_PROTOCOL    *EcNvsAreaProtocol;

  //
  // Mock Ecdt ACPI Table
  //
  will_return (MockEcAcpiTableFile, "Build/EcFeaturePkg/HostTest/NOOPT_VS2019/X64/EcFeaturePkg/AcpiTables/EcAcpiTables/OUTPUT/Ecdt/Ecdt.acpi");
  Status = MockEcAcpiTableFile ((UINT8**) &Table, &FileSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Mock gEcNvsAreaProtocolGuid
  //
  EcNvsAreaProtocol = AllocateZeroPool (sizeof (EC_NVS_AREA_PROTOCOL));
  EcNvsAreaProtocol->Area = AllocateZeroPool (sizeof (EC_NVS_AREA));
  EcNvsAreaProtocol->Area->EcdtGpeNumber = 0x6E;

  //
  // Mock gBS->LocateProtocol
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->LocateProtocol = MockBootServiceLocateProtocol;
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEcNvsAreaProtocolGuid);
  will_return (MockBootServiceLocateProtocol, EcNvsAreaProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);
  //
  // Test Update Acpi Table for SSDT
  //
  Status = UpdateAcpiDataTable (Table);



  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) Table)->Header.OemTableId,
    PcdGet64 (PcdAcpiDefaultOemTableId)
    );
  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) Table)->Header.OemRevision,
    PcdGet32(PcdAcpiDefaultOemRevision)
    );
  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) Table)->Header.CreatorId,
    PcdGet32 (PcdAcpiDefaultCreatorId)
    );
  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) Table)->Header.CreatorRevision,
    PcdGet32 (PcdAcpiDefaultCreatorRevision)
    );
  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) Table)->GpeBit,
    (UINT8) EcNvsAreaProtocol->Area->EcdtGpeNumber
    );


  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Destroy Mock Data
  //
  FreePool (gBS);
  FreePool (EcNvsAreaProtocol->Area);
  FreePool (EcNvsAreaProtocol);
  FreePool (Table);

  return UNIT_TEST_PASSED;
}