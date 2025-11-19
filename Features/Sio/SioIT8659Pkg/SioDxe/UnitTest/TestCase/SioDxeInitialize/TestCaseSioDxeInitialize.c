/**@file
    Unit test case of the Sio Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <TestCaseUpdateAcpiDataTable_Sio.h>
#include "BootServicesMock.h"
#include <PrivateMock/PrivateMock.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/SioNvsArea.h>
#include <Library/DebugLib.h>

extern
EFI_STATUS
EFIAPI
SioDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

/**
  Unit test for Install Sio Ssdt.
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
UnitTestSioDxeInitialize (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol2Protocol;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;
  EFI_STATUS                     Status;
  EFI_HANDLE                     *HandleBuffer;
  EFI_ACPI_COMMON_HEADER         *SsdtTable;
  UINTN                          SsdtBufferSize;
  SIO_NVS_AREA_PROTOCOL           *SioNvsAreaProtocol;
  UINT8                          *SsdtPointer;
  UINT32                         *Signature;
  UINT8                          *Operation;
  UINT32                         *Address;
  UINT16                         *Size;


  will_return (MockSioAcpiTableFile, "Build/SioIT8659Pkg/HostTest/NOOPT_" BUILD_TOOLCHAIN "/X64/SioIT8659Pkg/SioDxe/UnitTest/SioDxeHost/OUTPUT/SioIT8659Pkg/AcpiTables/SioSsdt/SioSsdt.aml");
  Status = MockSioAcpiTableFile ((UINT8**) &SsdtTable, &SsdtBufferSize);
  if (EFI_ERROR (Status)) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }
  UT_ASSERT_NOT_NULL (SsdtTable);

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));

  //
  // Mock gBS->LocateHandleBuffer
  //
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;

  //
  // For Sio SSDT handle buffer mock
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

  FwVol2Protocol->ReadSection = MockFwVol2ReadSection;
  //
  // First time return Sio SSDT table
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
  expect_value (MockInstallAcpiTable, This, AcpiTableProtocol);
  expect_value (MockInstallAcpiTable, AcpiTableBuffer, SsdtTable);
  expect_value (MockInstallAcpiTable, AcpiTableBufferSize, SsdtTable->Length);
  will_return (MockInstallAcpiTable, &SsdtTable);
  will_return (MockInstallAcpiTable, &SsdtBufferSize);
  will_return (MockInstallAcpiTable, EFI_SUCCESS);

  //
  // Mock gSioNvsAreaProtocolGuid
  //
  SioNvsAreaProtocol = AllocateZeroPool (sizeof (SIO_NVS_AREA_PROTOCOL));
  SioNvsAreaProtocol->Area = AllocateZeroPool (sizeof (SIO_NVS_AREA));

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

  expect_value (MockBootServiceLocateProtocol, Protocol, &gSioNvsAreaProtocolGuid);
  will_return (MockBootServiceLocateProtocol, SioNvsAreaProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  //
  // Test to Update Install SSDT ACPI Table.
  //
  Status = SioDxeInitialize (NULL, NULL);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  for (SsdtPointer = (UINT8 *)SsdtTable;
    SsdtPointer < ((UINT8 *)SsdtTable + ((EFI_ACPI_COMMON_HEADER *)SsdtTable)->Length - sizeof (UINT32));
    SsdtPointer++
    ) {
    Signature = (UINT32 *) SsdtPointer;
    if (*Signature == SIGNATURE_32 ('S', 'I', 'O', 'R')) {
      Operation = SsdtPointer - 1;
      if (*Operation == AML_EXT_REGION_OP) {
        Address = (UINT32 *) (SsdtPointer + 6);
        UT_ASSERT_EQUAL (*Address, (UINT32)(UINTN) SioNvsAreaProtocol->Area);
        Size = (UINT16 *) (SsdtPointer + 11);
        UT_ASSERT_EQUAL (*Size, sizeof (SIO_NVS_AREA));
      }
      break;
    }
  }

  //
  // Destroy Mock Data
  //
  FreePool (SsdtTable);
  FreePool (SioNvsAreaProtocol->Area);
  FreePool (SioNvsAreaProtocol);
  FreePool (FwVol2Protocol);
  FreePool (AcpiTableProtocol);
  FreePool (gBS);

  return UNIT_TEST_PASSED;
}