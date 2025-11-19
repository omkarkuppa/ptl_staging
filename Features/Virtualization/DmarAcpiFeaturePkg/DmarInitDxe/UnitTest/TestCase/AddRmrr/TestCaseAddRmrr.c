/** @file
  Unit test cases for AddRmrr API.

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
#include "TestCaseDmarRmrr.h"

#define MAGIC_NUMBER 0x12345678DEADBEEF

extern UNIT_TEST_RMRR_TABLE_STRUCTURE  UnitTestDmarRmrrTable;

/**
  This function allocates memory for VTD_TOPOLOGY_PROTOCOL and initialize this structrue.

  @retval  Pointer to structure of VTD_TOPOLOGY_PROTOCOL type
**/
static
VTD_TOPOLOGY_PROTOCOL*
CreateVtdTopologyProtocol (
  VOID
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;

  VtdTopologyProtocol = AllocateZeroPool (sizeof (VTD_TOPOLOGY_PROTOCOL));

  VtdTopologyProtocol->GetNumberOfVtdEngines            = MockGetNumberOfVtdEngines;
  VtdTopologyProtocol->GetDevices                       = MockGetDevices;
  VtdTopologyProtocol->GetPciDevicesWithAtc             = MockGetPciDevicesWithAtc;
  VtdTopologyProtocol->GetDevicesWithProperties         = MockGetDevicesWithProperties;
  VtdTopologyProtocol->GetNumberOfReservedMemoryRegions = MockGetNumberOfReservedMemoryRegions;
  VtdTopologyProtocol->GetReservedMemoryRegionInfo      = MockGetReservedMemoryRegionInfo;

  return VtdTopologyProtocol;
}

/**
  This function allocates memory for OEM_DMAR_PROTOCOL and initialize this structrue.

  @retval  Pointer to structure of OEM_DMAR_PROTOCOL type
**/
static
OEM_DMAR_PROTOCOL*
CreateOemDmarProtocol (
  VOID
  )
{
  OEM_DMAR_PROTOCOL  *OemDmarProtocol;

  OemDmarProtocol = AllocateZeroPool (sizeof (OEM_DMAR_PROTOCOL));

  OemDmarProtocol->GetNumberOfReservedMemoryRegions = MockOemGetNumberOfReservedMemoryRegions;
  OemDmarProtocol->GetReservedMemoryRegionInfo      = MockOemGetReservedMemoryRegionInfo;

  return OemDmarProtocol;
}

/**
  Unit-Test Function for AddRmrr().

  This unit-test checks if functions behaves properly for invalid parameters or other unexpected situations.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddRmrrInvalidParameters (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  EFI_STATUS             Status;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  Dmar = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER));

  //
  // These data will be validated by AddRmrr() API
  //
  Dmar->Header.Length = sizeof (EFI_ACPI_DMAR_HEADER) - 1;

  //
  // 1. Provide invalid input data to AddRmrr() API.
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRmrrEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddRmrr (NULL, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRmrrEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddRmrr (Dmar, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRmrrEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddRmrr (NULL, VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // 2. Wrong Signature in DMAR ACPI header
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRmrrEnable);
  will_return (LibPcdGetBool, TRUE);

  Dmar->Header.Signature = 0xDEADBEEF;
  Status                 = AddRmrr (Dmar, VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);

  //
  // 3. Wrong length in DMAR ACPI header
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRmrrEnable);
  will_return (LibPcdGetBool, TRUE);

  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER) - 1;
  Status                 = AddRmrr (Dmar, VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);

  //
  // 4. RMRR is disabled in PCD
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRmrrEnable);
  will_return (LibPcdGetBool, FALSE);

  Status = AddRmrr (Dmar, VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for AddRmrr()

  This unit-test checks if AddRmrr() creates DMAR ACPI table as expected (only with RMRR structures).

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddRmrrBasicFlow (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS             Status;
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  OEM_DMAR_PROTOCOL      *OemDmarProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  UINT8                  *Array;
  UINT64                 *MagicNumber;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();
  OemDmarProtocol     = CreateOemDmarProtocol ();

  Dmar = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_RMRR_TABLE_STRUCTURE) + sizeof (UINT64));
  MagicNumber = (UINT64 *)((UINT8 *) Dmar + sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_RMRR_TABLE_STRUCTURE));
  *MagicNumber = MAGIC_NUMBER;

  //
  // Prepare DMAR ACPI header
  //
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Mock MockGetNumberOfReservedMemoryRegions from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfReservedMemoryRegions, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfReservedMemoryRegions, 4);
  will_return (MockGetNumberOfReservedMemoryRegions, EFI_SUCCESS);

  //
  // Mock MockGetReservedMemoryRegionInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetReservedMemoryRegionInfo, This, VtdTopologyProtocol, 4);
  expect_in_range_count (MockGetReservedMemoryRegionInfo, ReservedMemoryRegionId, 0, 3, 4);

  DEVICE_INFO  Devices1[] = {
    {{ 0xA, 0x5, 0 }, PciEndpointDevice, 0, 1},
    {{ 0xA, 0x6, 1 }, PciBridge, 0, 1},
  };
  DEVICE_INFO  Devices2[] = {
    {{ 0, 0xA, 0 }, PciEndpointDevice, 0, 0},
  };

  DEVICE_INFO  *Devices1Ptr = AllocateZeroPool (sizeof (Devices1));
  DEVICE_INFO  *Devices2Ptr = AllocateZeroPool (sizeof (Devices2));

  CopyMem (Devices1Ptr, &Devices1, sizeof (Devices1));
  CopyMem (Devices2Ptr, &Devices2, sizeof (Devices2));

  will_return (MockGetReservedMemoryRegionInfo, EFI_INVALID_PARAMETER);

  will_return (MockGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return (MockGetReservedMemoryRegionInfo, 0);
  will_return (MockGetReservedMemoryRegionInfo, 0x1234FEAD0000);
  will_return (MockGetReservedMemoryRegionInfo, 0x1234FEAEFFFF);
  will_return (MockGetReservedMemoryRegionInfo, Devices1Ptr);
  will_return (MockGetReservedMemoryRegionInfo, ARRAY_SIZE (Devices1));

  will_return (MockGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return (MockGetReservedMemoryRegionInfo, 1);
  will_return (MockGetReservedMemoryRegionInfo, 0xBEAF02000000);
  will_return (MockGetReservedMemoryRegionInfo, 0xBEAF05FFFFFF);
  will_return (MockGetReservedMemoryRegionInfo, Devices2Ptr);
  will_return (MockGetReservedMemoryRegionInfo, ARRAY_SIZE (Devices2));

  will_return (MockGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return (MockGetReservedMemoryRegionInfo, 0);
  will_return (MockGetReservedMemoryRegionInfo, 0);
  will_return (MockGetReservedMemoryRegionInfo, 0);
  will_return (MockGetReservedMemoryRegionInfo, NULL);
  will_return (MockGetReservedMemoryRegionInfo, 0);

  //
  // Mock MockOemGetNumberOfReservedMemoryRegions from OEM_DMAR_PROTOCOL
  //
  expect_value (MockOemGetNumberOfReservedMemoryRegions, This, OemDmarProtocol);
  will_return (MockOemGetNumberOfReservedMemoryRegions, 3);
  will_return (MockOemGetNumberOfReservedMemoryRegions, EFI_SUCCESS);

  //
  // Mock MockOemGetreservedMemoryRegionInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockOemGetReservedMemoryRegionInfo, This, OemDmarProtocol, 3);
  expect_in_range_count (MockOemGetReservedMemoryRegionInfo, ReservedMemoryRegionId, 0, 2, 3);

  DEVICE_INFO  OemDevices1[] = {
    {{ 0xB, 0xA, 0x1 }, PciEndpointDevice, 0, 0},
  };

  DEVICE_INFO  *OemDevices1Ptr = AllocateZeroPool (sizeof (OemDevices1));

  CopyMem (OemDevices1Ptr, &OemDevices1, sizeof (OemDevices1));

  will_return (MockOemGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return (MockOemGetReservedMemoryRegionInfo, 0);
  will_return (MockOemGetReservedMemoryRegionInfo, 0);
  will_return (MockOemGetReservedMemoryRegionInfo, 0);
  will_return (MockOemGetReservedMemoryRegionInfo, NULL);
  will_return (MockOemGetReservedMemoryRegionInfo, 0);

  will_return (MockOemGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return (MockOemGetReservedMemoryRegionInfo, 1);
  will_return (MockOemGetReservedMemoryRegionInfo, 0xA234FEAD0000);
  will_return (MockOemGetReservedMemoryRegionInfo, 0xA234FEAEFFFF);
  will_return (MockOemGetReservedMemoryRegionInfo, OemDevices1Ptr);
  will_return (MockOemGetReservedMemoryRegionInfo, ARRAY_SIZE (OemDevices1));

  will_return (MockOemGetReservedMemoryRegionInfo, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarRmrrEnable);
  will_return (LibPcdGetBool, TRUE);

  Status = AddRmrr (Dmar, VtdTopologyProtocol, OemDmarProtocol);

  //
  // Dump DMAR header
  //
  Array = (UINT8 *)&UnitTestDmarRmrrTable;
  printf ("Original Rmrr entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_RMRR_TABLE_STRUCTURE));

  Array = ((UINT8 *)Dmar) + sizeof (EFI_ACPI_DMAR_HEADER);
  printf ("Produced Rmrr entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_RMRR_TABLE_STRUCTURE));

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL (&UnitTestDmarRmrrTable, (UINT8 *)Dmar + sizeof (EFI_ACPI_DMAR_HEADER), sizeof (UNIT_TEST_RMRR_TABLE_STRUCTURE));
  UT_ASSERT_EQUAL (Dmar->Header.Length, sizeof (UNIT_TEST_RMRR_TABLE_STRUCTURE) + sizeof (EFI_ACPI_DMAR_HEADER));
  UT_ASSERT_EQUAL (*MagicNumber, MAGIC_NUMBER);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}
