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
InstallEcSsdt (
  VOID
  );

/**
  Unit test for Install EC Ssdt.
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
UnitTestInstallEcSsdt (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol2Protocol;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;
  EFI_STATUS                     Status;
  EFI_HANDLE                     *HandleBuffer;
  EFI_ACPI_COMMON_HEADER         *SsdtTable;
  UINTN                          SsdtBufferSize;
  EFI_ACPI_COMMON_HEADER         *EcdtTable;
  UINTN                          EcdtBufferSize;
  EFI_ACPI_COMMON_HEADER         *SsdtTargetTable;
  UINTN                          SsdtTargetBufferSize;
  EFI_ACPI_COMMON_HEADER         *EcdtTargetTable;
  UINTN                          EcdtTargetBufferSize;
  EC_NVS_AREA_PROTOCOL           *EcNvsAreaProtocol;
  UINT8                          *SsdtPointer;
  UINT32                         *Signature;
  UINT8                          *Operation;
  UINT32                         *Address;
  UINT16                         *Size;


  will_return (MockEcAcpiTableFile, "Build/EcFeaturePkg/HostTest/NOOPT_VS2019/X64/EcFeaturePkg/AcpiTables/EcAcpiTables/OUTPUT/EcSsdt/EcSsdt.aml");
  Status = MockEcAcpiTableFile ((UINT8**) &SsdtTable, &SsdtBufferSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  will_return (MockEcAcpiTableFile, "Build/EcFeaturePkg/HostTest/NOOPT_VS2019/X64/EcFeaturePkg/AcpiTables/EcAcpiTables/OUTPUT/Ecdt/Ecdt.acpi");
  Status = MockEcAcpiTableFile ((UINT8**) &EcdtTable, &EcdtBufferSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);


  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));

  //
  // Mock gBS->LocateHandleBuffer
  //
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;

  //
  // For EC SSDT handle buffer mock
  //
  HandleBuffer = AllocateZeroPool (3 * sizeof (EFI_HANDLE));
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceLocateHandleBuffer, HandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, 3);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  //
  // For EC ECDT handle buffer mock
  //
  HandleBuffer = AllocateZeroPool (3 * sizeof (EFI_HANDLE));
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceLocateHandleBuffer, HandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, 3);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);


  //
  // Mock gEfiFirmwareVolume2ProtocolGuid
  //
  FwVol2Protocol = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  FwVol2Protocol->ReadFile = MockFwVol2ReadFile;
  will_return (MockFwVol2ReadFile, SsdtBufferSize);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);
  will_return (MockFwVol2ReadFile, EcdtBufferSize);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  FwVol2Protocol->ReadSection = MockFwVol2ReadSection;
  //
  // First time return EC SSDT table
  //
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, SsdtTable);
  will_return (MockFwVol2ReadSection, SsdtBufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);

  //
  // Second time return ACPI table not found.
  //
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, SsdtTable);
  will_return (MockFwVol2ReadSection, SsdtBufferSize);
  will_return (MockFwVol2ReadSection, EFI_NOT_FOUND);

  //
  // First time return EC ECDT table
  //
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, EcdtTable);
  will_return (MockFwVol2ReadSection, EcdtBufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);

  //
  // Second time return ACPI table not found.
  //
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, EcdtTable);
  will_return (MockFwVol2ReadSection, EcdtBufferSize);
  will_return (MockFwVol2ReadSection, EFI_NOT_FOUND);

  //
  // Mock gBS->HandleProtocol
  //
  gBS->HandleProtocol = MockBootServiceHandleProtocol;
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceHandleProtocol, FwVol2Protocol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceHandleProtocol, FwVol2Protocol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);


  //
  // Mock gEfiAcpiTableProtocolGuid
  //
  AcpiTableProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  AcpiTableProtocol->InstallAcpiTable = MockInstallAcpiTable;
  will_return (MockInstallAcpiTable, &SsdtTargetTable);
  will_return (MockInstallAcpiTable, &SsdtTargetBufferSize);
  will_return (MockInstallAcpiTable, EFI_SUCCESS);

  will_return (MockInstallAcpiTable, &EcdtTargetTable);
  will_return (MockInstallAcpiTable, &EcdtTargetBufferSize);
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

  //
  // Mock for SSDT needed protocol
  //
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  will_return (MockBootServiceLocateProtocol, AcpiTableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_value (MockBootServiceLocateProtocol, Protocol, &gEcNvsAreaProtocolGuid);
  will_return (MockBootServiceLocateProtocol, EcNvsAreaProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  //
  // Mock for ECDT needed protocol
  //
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  will_return (MockBootServiceLocateProtocol, AcpiTableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  expect_value (MockBootServiceLocateProtocol, Protocol, &gEcNvsAreaProtocolGuid);
  will_return (MockBootServiceLocateProtocol, EcNvsAreaProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);


  //
  // Test to Update Install SSDT ACPI Table.
  //
  Status = InstallEcSsdt ();

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  for (SsdtPointer = (UINT8 *)SsdtTargetTable;
    SsdtPointer < ((UINT8 *)SsdtTargetTable + ((EFI_ACPI_COMMON_HEADER *) SsdtTargetTable)->Length - sizeof (UINT32));
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

  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) EcdtTargetTable)->Header.OemTableId,
    PcdGet64 (PcdAcpiDefaultOemTableId)
    );
  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) EcdtTargetTable)->Header.OemRevision,
    PcdGet32(PcdAcpiDefaultOemRevision)
    );
  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) EcdtTargetTable)->Header.CreatorId,
    PcdGet32 (PcdAcpiDefaultCreatorId)
    );
  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) EcdtTargetTable)->Header.CreatorRevision,
    PcdGet32 (PcdAcpiDefaultCreatorRevision)
    );
  UT_ASSERT_EQUAL (
    ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) (UINT8 *) EcdtTargetTable)->GpeBit,
    (UINT8) EcNvsAreaProtocol->Area->EcdtGpeNumber
    );

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