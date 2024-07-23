/** @file
  Unit test cases for AddAtsr API.

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

@par Specification Reference:
**/

#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include <PrivateMock.h>
#include <Protocol/VtdTopologyProtocol.h>
#include <Dmar.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UnitTestHostBaseLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "Utils/Utils.h"
#include "TestCaseDmarAtsr.h"

#define MAGIC_NUMBER  0x12345678DEADBEEF

extern UNIT_TEST_ATSR_TABLE_STRUCTURE  UnitTestDmarAtsrTable;

/**
  This function allocates memory for VTD_TOPOLOGY_PROTOCOL and initialize this structrue.

  @retval  Pointer to structure of VTD_TOPOLOGY_PROTOCOL type
**/
static
VTD_TOPOLOGY_PROTOCOL *
CreateVtdTopologyProtocol (
  VOID
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;

  VtdTopologyProtocol = AllocateZeroPool (sizeof (VTD_TOPOLOGY_PROTOCOL));

  VtdTopologyProtocol->GetNumberOfVtdEngines     = MockGetNumberOfVtdEngines;
  VtdTopologyProtocol->GetSystemInfo             = MockGetSystemInfo;
  VtdTopologyProtocol->GetVtdEngineInfo          = MockGetVtdEngineInfo;
  VtdTopologyProtocol->GetRootPortsSupportingAts = MockGetRootPortsSupportingAts;

  return VtdTopologyProtocol;
}

/**
  Unit-Test Function for AddAtsr.

  This unit test checks whether AddAtsr() behaves properly in case of invalid parameters.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddAtsrInvalidParameters (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  EFI_STATUS             Status;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  Dmar = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER));

  //
  // These data will be validated by AddAtsr() API
  //
  Dmar->Header.Length = sizeof (EFI_ACPI_DMAR_HEADER) - 1;

  //
  // 1. Provide invalid input data to AddAtsr() API.
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarAtsrEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddAtsr (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarAtsrEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddAtsr (Dmar, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarAtsrEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddAtsr (NULL, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // 2. Wrong Signature in DMAR ACPI header
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarAtsrEnable);
  will_return (LibPcdGetBool, TRUE);
  Dmar->Header.Signature = 0xDEADBEEF;
  Status                 = AddAtsr (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);

  //
  // 3. Wrong length in DMAR ACPI header
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarAtsrEnable);
  will_return (LibPcdGetBool, TRUE);
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER) - 1;
  Status                 = AddAtsr (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for AddAtsr.

  This unit test checks whether AddAtsr() behaves properly when ATSR is not supported (eg. disabled by PCD).

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddAtsrUnsupported (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  EFI_STATUS             Status;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  Dmar = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER));

  //
  // Prepare DMAR ACPI header
  //
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // 1. PcdGetBool(PcdDmarAtsrEnable) returns FALSE
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarAtsrEnable);
  will_return (LibPcdGetBool, FALSE);
  Status = AddAtsr (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  //
  // 2. GetSystemInfo() returns an error
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarAtsrEnable);
  will_return (LibPcdGetBool, TRUE);
  //
  // Mock MockGetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetSystemInfo, This, VtdTopologyProtocol);
  will_return (MockGetSystemInfo, 0);
  will_return (MockGetSystemInfo, FALSE);
  will_return (MockGetSystemInfo, 1);
  will_return (MockGetSystemInfo, EFI_UNSUPPORTED);

  Status = AddAtsr (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for AddAtsr()

  This unit test checks if AddAtsr() creates DMAR ACPI table as expected (only with ATSR structures).

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddAtsrBasicFlow (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  EFI_STATUS             Status;
  UINT64                 *MagicNumber;
  UINT8                  *Array;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  Dmar         = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_ATSR_TABLE_STRUCTURE) + sizeof (UINT64));
  MagicNumber  = (UINT64 *)((UINT8 *)Dmar + sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_ATSR_TABLE_STRUCTURE));
  *MagicNumber = MAGIC_NUMBER;

  //
  // Prepare DMAR ACPI header
  //
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarAtsrEnable);
  will_return (LibPcdGetBool, TRUE);

  //
  // Mock MockGetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetSystemInfo, This, VtdTopologyProtocol);
  will_return (MockGetSystemInfo, 0);
  will_return (MockGetSystemInfo, TRUE);
  will_return (MockGetSystemInfo, 2);
  will_return (MockGetSystemInfo, EFI_SUCCESS);

  //
  // Mock GetRootPortsSupportingAts from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetRootPortsSupportingAts, This, VtdTopologyProtocol, 2);
  expect_in_range_count (MockGetRootPortsSupportingAts, SegmentId, 0, 1, 2);
  will_return (MockGetRootPortsSupportingAts, EFI_SUCCESS);
  will_return (MockGetRootPortsSupportingAts, 1);
  will_return (MockGetRootPortsSupportingAts, NULL);
  will_return (MockGetRootPortsSupportingAts, 0);

  DEVICE_INFO  Segment1DevicesList[] = {
    { { 0, 0x2, 0 }, PciBridge, 0 },
    { { 0, 0xB, 0 }, PciBridge, 0 }
  };

  DEVICE_INFO  *Segment1DevicesListPtr = AllocateZeroPool (sizeof (Segment1DevicesList));
  CopyMem (Segment1DevicesListPtr, &Segment1DevicesList, sizeof (Segment1DevicesList));

  will_return (MockGetRootPortsSupportingAts, EFI_SUCCESS);
  will_return (MockGetRootPortsSupportingAts, 0);
  will_return (MockGetRootPortsSupportingAts, Segment1DevicesListPtr);
  will_return (MockGetRootPortsSupportingAts, ARRAY_SIZE (Segment1DevicesList));

  Status = AddAtsr (Dmar, VtdTopologyProtocol);

  //
  // Dump DMAR header
  //
  Array = (UINT8 *)&UnitTestDmarAtsrTable;
  printf ("Original Atsr entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_ATSR_TABLE_STRUCTURE));

  Array = ((UINT8 *)Dmar) + sizeof (EFI_ACPI_DMAR_HEADER);
  printf ("Produced Atsr entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_ATSR_TABLE_STRUCTURE));

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL (&UnitTestDmarAtsrTable, (UINT8 *)Dmar + sizeof (EFI_ACPI_DMAR_HEADER), sizeof (UNIT_TEST_ATSR_TABLE_STRUCTURE));
  UT_ASSERT_EQUAL (Dmar->Header.Length, sizeof (UNIT_TEST_ATSR_TABLE_STRUCTURE) + sizeof (EFI_ACPI_DMAR_HEADER));
  UT_ASSERT_EQUAL (*MagicNumber, MAGIC_NUMBER);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}
