/** @file
  Unit test cases for AddRhsa API.

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
#include "TestCaseDmarRhsa.h"

#define MAGIC_NUMBER  0x12345678DEADBEEF

extern UNIT_TEST_RHSA_TABLE_STRUCTURE  UnitTestDmarRhsaTable;

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

  VtdTopologyProtocol->GetNumberOfVtdEngines = MockGetNumberOfVtdEngines;
  VtdTopologyProtocol->GetSystemInfo         = MockGetSystemInfo;
  VtdTopologyProtocol->GetVtdEngineInfo      = MockGetVtdEngineInfo;

  return VtdTopologyProtocol;
}

/**
  Unit-Test Function for AddRhsa.

  This unit test checks whether AddRhsa() behaves properly in case of invalid parameters.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddRhsaInvalidParameters (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  EFI_STATUS             Status;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  Dmar = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER));

  //
  // These data will be validated by AddRhsa() API
  //
  Dmar->Header.Length = sizeof (EFI_ACPI_DMAR_HEADER) - 1;

  //
  // 1. Provide invalid input data to AddRhsa() API.
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddRhsa (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddRhsa (Dmar, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddRhsa (NULL, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // 2. Wrong Signature in DMAR ACPI header
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);
  Dmar->Header.Signature = 0xDEADBEEF;
  Status                 = AddRhsa (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);

  //
  // 3. Wrong length in DMAR ACPI header
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER) - 1;
  Status                 = AddRhsa (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for AddRhsa.

  This unit test checks whether AddRhsa() behaves properly when RHSA is not supported (eg. disabled by PCD).

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddRhsaUnsupported (
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
  // 1. Provide invalid input data to AddRhsa() API.
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, FALSE);
  Status = AddRhsa (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  //
  // 2. GetSystemInfo() returns an error
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);
  //
  // Mock MockGetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetSystemInfo, This, VtdTopologyProtocol);
  will_return (MockGetSystemInfo, 0);
  will_return (MockGetSystemInfo, FALSE);
  will_return (MockGetSystemInfo, 1);
  will_return (MockGetSystemInfo, EFI_UNSUPPORTED);

  Status = AddRhsa (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  //
  // 3. GetSystemInfo() returns info that NUMA is not supported
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);
  //
  // Mock MockGetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetSystemInfo, This, VtdTopologyProtocol);
  will_return (MockGetSystemInfo, 0);
  will_return (MockGetSystemInfo, FALSE);
  will_return (MockGetSystemInfo, 1);
  will_return (MockGetSystemInfo, EFI_SUCCESS);

  Status = AddRhsa (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  //
  // 3. GetNumberOfVtdEngines() returns an error
  //

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);

  //
  // Mock MockGetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetSystemInfo, This, VtdTopologyProtocol);
  will_return (MockGetSystemInfo, 0);
  will_return (MockGetSystemInfo, TRUE);
  will_return (MockGetSystemInfo, 1);
  will_return (MockGetSystemInfo, EFI_SUCCESS);

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 0);
  will_return (MockGetNumberOfVtdEngines, EFI_INVALID_PARAMETER);

  Status = AddRhsa (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for AddRhsa()

  This unit test checks if AddRhsa() creates DMAR ACPI table as expected (only with RHSA structures).

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddRhsaBasicFlow (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  EFI_STATUS             Status;
  UINT64                 *MagicNumber;
  UINT8                  *Array;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  Dmar         = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_RHSA_TABLE_STRUCTURE) + sizeof (UINT64));
  MagicNumber  = (UINT64 *)((UINT8 *)Dmar + sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_RHSA_TABLE_STRUCTURE));
  *MagicNumber = MAGIC_NUMBER;

  //
  // Prepare DMAR ACPI header
  //
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRhsaEnable);
  will_return (LibPcdGetBool, TRUE);

  //
  // Mock MockGetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetSystemInfo, This, VtdTopologyProtocol);
  will_return (MockGetSystemInfo, 0);
  will_return (MockGetSystemInfo, TRUE);
  will_return (MockGetSystemInfo, 1);
  will_return (MockGetSystemInfo, EFI_SUCCESS);

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 3);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  expect_value_count (MockGetVtdEngineInfo, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetVtdEngineInfo, VtdEngineId, 0, 2, 3);
  will_return (MockGetVtdEngineInfo, 0xDEADBEEF0000);
  will_return (MockGetVtdEngineInfo, SIZE_64KB);
  will_return (MockGetVtdEngineInfo, 0); // Segment=0
  will_return (MockGetVtdEngineInfo, 2); // ProximityDomain=2
  will_return (MockGetVtdEngineInfo, 0); // IncludePciAll=0
  will_return (MockGetVtdEngineInfo, 0xDEADBEF00000);
  will_return (MockGetVtdEngineInfo, SIZE_64KB);
  will_return (MockGetVtdEngineInfo, 0); // Segment=0
  will_return (MockGetVtdEngineInfo, 1); // ProximityDomain=1
  will_return (MockGetVtdEngineInfo, 0); // IncludePciAll=0
  will_return (MockGetVtdEngineInfo, 0xDEADBEF11000);
  will_return (MockGetVtdEngineInfo, SIZE_64KB);
  will_return (MockGetVtdEngineInfo, 0); // Segment=0
  will_return (MockGetVtdEngineInfo, 0); // ProximityDomain=0
  will_return (MockGetVtdEngineInfo, 0); // IncludePciAll=0

  Status = AddRhsa (Dmar, VtdTopologyProtocol);

  //
  // Dump DMAR header
  //
  Array = (UINT8 *)&UnitTestDmarRhsaTable;
  printf ("Original Rhsa entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_RHSA_TABLE_STRUCTURE));

  Array = ((UINT8 *)Dmar) + sizeof (EFI_ACPI_DMAR_HEADER);
  printf ("Produced Rhsa entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_RHSA_TABLE_STRUCTURE));

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL (&UnitTestDmarRhsaTable, (UINT8 *)Dmar + sizeof (EFI_ACPI_DMAR_HEADER), sizeof (UNIT_TEST_RHSA_TABLE_STRUCTURE));
  UT_ASSERT_EQUAL (Dmar->Header.Length, sizeof (UNIT_TEST_RHSA_TABLE_STRUCTURE) + sizeof (EFI_ACPI_DMAR_HEADER));
  UT_ASSERT_EQUAL (*MagicNumber, MAGIC_NUMBER);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}
