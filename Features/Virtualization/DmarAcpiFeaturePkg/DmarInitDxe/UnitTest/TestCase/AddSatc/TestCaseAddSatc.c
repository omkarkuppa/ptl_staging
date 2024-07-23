/** @file
  Unit test cases for AddSatc API.

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
#include "TestCaseDmarSatc.h"

#define MAGIC_NUMBER 0x12345678DEADBEEF

extern UNIT_TEST_SATC_TABLE_STRUCTURE  UnitTestDmarSatcTable;

/**
  Unit-Test Function for AddSatc

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddSatc (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS             Status;
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  UINT8                  *Array;
  UINT64                 *MagicNumber;

  VtdTopologyProtocol = AllocateZeroPool (sizeof (VTD_TOPOLOGY_PROTOCOL));

  VtdTopologyProtocol->GetNumberOfVtdEngines = MockGetNumberOfVtdEngines;
  VtdTopologyProtocol->GetVtdEngineInfo      = MockGetVtdEngineInfo;
  VtdTopologyProtocol->GetPciDevicesWithAtc  = MockGetPciDevicesWithAtc;

  Dmar = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_SATC_TABLE_STRUCTURE) + sizeof (UINT64));
  MagicNumber = (UINT64 *)((UINT8 *) Dmar + sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_SATC_TABLE_STRUCTURE));
  *MagicNumber = MAGIC_NUMBER;


  //
  // These data will be validated by AddSatc() API
  //

  Dmar->Header.Length = sizeof (EFI_ACPI_DMAR_HEADER) - 1;

  //
  // 1. Provide invalid input data to AddSatc() API.
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarSatcEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddSatc (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarSatcEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddSatc (Dmar, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarSatcEnable);
  will_return (LibPcdGetBool, TRUE);
  Status = AddSatc (NULL, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // 2. Wrong Signature in DMAR ACPI header
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarSatcEnable);
  will_return (LibPcdGetBool, TRUE);
  Dmar->Header.Signature = 0xDEADBEEF;
  Status                 = AddSatc (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);

  //
  // 3. Wrong length in DMAR ACPI header
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarSatcEnable);
  will_return (LibPcdGetBool, TRUE);
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER) - 1;
  Status                 = AddSatc (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);


  //
  // 4. In this test case GetNumberOfVtdEngines() returns an error.
  //

  //
  // Prepare DMAR ACPI header
  //
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 0);
  will_return (MockGetNumberOfVtdEngines, EFI_INVALID_PARAMETER);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarSatcEnable);
  will_return (LibPcdGetBool, TRUE);

  Status = AddSatc (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // 5. SATC is disabled in PCD
  //
  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarSatcEnable);
  will_return (LibPcdGetBool, FALSE);

  Status = AddSatc (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  //
  // 6. Provide valid data via VtdTopologyProtocol APIs and compare created DMAR ACPI table
  //    (only with SATC entries) with a pattern.
  //    An example system:
  //    1. VT-d engine #1 has BAR=0xDEADBEEF0000 (size=SIZE_64KB), it is in segment=0.
  //       It supports Device-TLB (bit2 in ECAP register). It has under its scope integrated
  //       device at [00|02|00], but it's disabled.
  //    2. VT-d engine #2 has BAR=0xDEADBEF00000 (size=SIZE_64KB), it is in segment=0.
  //       It supports Device-TLB (bit2 in ECAP register). It has under its scope integrated
  //       devices at [0A|05|00], [0A|06|00], [0A|07|00] and they are enabled. These devices
  //       have a functional requirement to enable theirs ATC.
  //    3. VT-d engine #2 has BAR=0xDEADBEF10000 (size=SIZE_4KB), it is in segment=1.
  //       It supports Device-TLB (bit2 in ECAP register). It has under its scope integrated
  //       devices at [00|0A|00], [00|0C|01], [00|0F|00] and they are enabled. [00|0F|00]
  //       device has  a functional requirement to enable its ATC.
  //    4. VT-d engine #2 has BAR=0xDEADBEF11000 (size=SIZE_4KB), it is in segment=2.
  //       It doesn't support Device-TLB (bit2 in ECAP register).
  //

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 4);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetVtdEngineInfo, This, VtdTopologyProtocol, 4);
  expect_in_range_count (MockGetVtdEngineInfo, VtdEngineId, 0, 3, 4);
  will_return (MockGetVtdEngineInfo, 0xDEADBEEF0000);
  will_return (MockGetVtdEngineInfo, SIZE_64KB);
  will_return (MockGetVtdEngineInfo, 0); // segment=0
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0xDEADBEF00000);
  will_return (MockGetVtdEngineInfo, SIZE_64KB);
  will_return (MockGetVtdEngineInfo, 0); // segment=0
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0xDEADBEF10000);
  will_return (MockGetVtdEngineInfo, SIZE_4KB);
  will_return (MockGetVtdEngineInfo, 1); // segment=1
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0xDEADBEF11000);
  will_return (MockGetVtdEngineInfo, SIZE_4KB);
  will_return (MockGetVtdEngineInfo, 2); // segment=2
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0);

  //
  // Mock MmioRead64
  //
  will_return_count (MmioRead64, BIT2, 3);
  will_return (MmioRead64, 0);

  //
  // Mock MockGetPciDevicesWithAtc from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetPciDevicesWithAtc, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetPciDevicesWithAtc, VtdEngineId, 0, 2, 3);

  DEVICE_INFO  PciDevicesWithAtcIommu1[] = {
    {{ 0xA, 0x5, 0 }, PciEndpointDevice, 0, 1},
    {{ 0xA, 0x6, 0 }, PciEndpointDevice, 0, 1},
    {{ 0xA, 0x7, 0 }, PciEndpointDevice, 0, 1}
  };
  DEVICE_INFO  PciDevicesWithAtcIommu2[] = {
    {{ 0, 0xA, 0 }, PciEndpointDevice, 0, 0},
    {{ 0, 0xC, 1 }, PciEndpointDevice, 0, 0},
    {{ 0, 0xF, 0 }, PciEndpointDevice, 0, 1}
  };

  DEVICE_INFO  *PciDevicesWithAtcIommu1Ptr = AllocateZeroPool (sizeof (PciDevicesWithAtcIommu1));
  DEVICE_INFO  *PciDevicesWithAtcIommu2Ptr = AllocateZeroPool (sizeof (PciDevicesWithAtcIommu2));

  CopyMem (PciDevicesWithAtcIommu1Ptr, &PciDevicesWithAtcIommu1, sizeof (PciDevicesWithAtcIommu1));
  CopyMem (PciDevicesWithAtcIommu2Ptr, &PciDevicesWithAtcIommu2, sizeof (PciDevicesWithAtcIommu2));

  // call GetPciDevicesWithAtc for IOMMU0
  will_return (MockGetPciDevicesWithAtc, NULL);
  will_return (MockGetPciDevicesWithAtc, 0);
  will_return (MockGetPciDevicesWithAtc, EFI_SUCCESS);
  // call GetPciDevicesWithAtc for IOMMU1
  will_return (MockGetPciDevicesWithAtc, PciDevicesWithAtcIommu1Ptr);
  will_return (MockGetPciDevicesWithAtc, ARRAY_SIZE (PciDevicesWithAtcIommu1));
  will_return (MockGetPciDevicesWithAtc, EFI_SUCCESS);
  // call GetPciDevicesWithAtc for IOMMU2
  will_return (MockGetPciDevicesWithAtc, PciDevicesWithAtcIommu2Ptr);
  will_return (MockGetPciDevicesWithAtc, ARRAY_SIZE (PciDevicesWithAtcIommu2));
  will_return (MockGetPciDevicesWithAtc, EFI_SUCCESS);

  //
  // Mock LibPcdGetBool
  //
  expect_value (LibPcdGetBool, TokenNumber, _PCD_TOKEN_PcdDmarSatcEnable);
  will_return (LibPcdGetBool, TRUE);

  Status = AddSatc (Dmar, VtdTopologyProtocol);

  //
  // Dump DMAR header
  //
  Array = (UINT8 *)&UnitTestDmarSatcTable;
  printf ("Original SATC entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_SATC_TABLE_STRUCTURE));

  Array = ((UINT8 *)Dmar) + sizeof (EFI_ACPI_DMAR_HEADER);
  printf ("Produced SATC entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_SATC_TABLE_STRUCTURE));

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL (&UnitTestDmarSatcTable, (UINT8 *)Dmar + sizeof (EFI_ACPI_DMAR_HEADER), sizeof (UNIT_TEST_SATC_TABLE_STRUCTURE));
  UT_ASSERT_EQUAL (Dmar->Header.Length, sizeof (UNIT_TEST_SATC_TABLE_STRUCTURE) + sizeof (EFI_ACPI_DMAR_HEADER));
  UT_ASSERT_EQUAL (*MagicNumber, MAGIC_NUMBER);

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}
