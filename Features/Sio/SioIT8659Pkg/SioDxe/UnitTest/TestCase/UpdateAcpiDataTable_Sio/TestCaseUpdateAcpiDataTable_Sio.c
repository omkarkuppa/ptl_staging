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
UpdateAcpiDataTable_Sio (
  IN OUT  EFI_ACPI_COMMON_HEADER        *Table
  );

/**
  Unit test for UpdateAcpiDataTable_Sio Ssdt.
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
UnitTestUpdateAcpiDataTableSsdt_Sio (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN                   FileSize;
  EFI_ACPI_COMMON_HEADER  *Table;
  EFI_STATUS              Status;
  SIO_NVS_AREA_PROTOCOL   *SioNvsAreaProtocol;
  UINT8                   *SsdtPointer;
  UINT32                  *Signature;
  UINT8                   *Operation;
  UINT32                  *Address;
  UINT16                  *Size;

  //
  // Mock SioSsdt.asl
  //
  will_return (MockSioAcpiTableFile, "Build/SioIT8659Pkg/HostTest/NOOPT_" BUILD_TOOLCHAIN "/X64/SioIT8659Pkg/SioDxe/UnitTest/SioDxeHost/OUTPUT/SioIT8659Pkg/AcpiTables/SioSsdt/SioSsdt.aml");
  Status = MockSioAcpiTableFile ((UINT8**) &Table, &FileSize);
  if (EFI_ERROR (Status)) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }
  UT_ASSERT_NOT_NULL (Table);

  //
  // Mock gSioNvsAreaProtocolGuid
  //
  SioNvsAreaProtocol = AllocateZeroPool (sizeof (SIO_NVS_AREA_PROTOCOL));

  //
  // Mock gBS->LocateProtocol
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->LocateProtocol = MockBootServiceLocateProtocol;
  expect_value (MockBootServiceLocateProtocol, Protocol, &gSioNvsAreaProtocolGuid);
  will_return (MockBootServiceLocateProtocol, SioNvsAreaProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  //
  // Test Update Acpi Table for SSDT
  //
  Status = UpdateAcpiDataTable_Sio (Table);

  UT_ASSERT_MEM_EQUAL (
    &(((EFI_ACPI_DESCRIPTION_HEADER *) Table)->OemId),
    PcdGetPtr (PcdAcpiDefaultOemId),
    AsciiStrLen ((CHAR8 *)PcdGetPtr (PcdAcpiDefaultOemId))
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
  FreePool (Table);
  FreePool (gBS);
  FreePool (SioNvsAreaProtocol);

  return UNIT_TEST_PASSED;
}