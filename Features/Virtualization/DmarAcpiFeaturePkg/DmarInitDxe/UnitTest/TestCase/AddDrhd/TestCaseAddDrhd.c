/** @file
  Unit test cases for AddDrhd API.

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
#include "TestCaseDmarStructures.h"

extern UNIT_TEST_DRHD_TABLE_STRUCTURE  UnitTest1DmarTable;

/**
  Unit-Test Function for AddDrhd

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddDrhd (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS             Status;
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  EFI_ACPI_DMAR_HEADER   *Dmar;
  UINT8                  *Array;

  VtdTopologyProtocol = AllocateZeroPool (sizeof (VTD_TOPOLOGY_PROTOCOL));

  VtdTopologyProtocol->GetNumberOfVtdEngines = MockGetNumberOfVtdEngines;
  VtdTopologyProtocol->GetVtdEngineInfo      = MockGetVtdEngineInfo;
  VtdTopologyProtocol->GetDevices            = MockGetDevices;

  Dmar = AllocateZeroPool (sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (UNIT_TEST_DRHD_TABLE_STRUCTURE));

  //
  // These data will be validated by AddDrhd() API
  //

  Dmar->Header.Length = sizeof (EFI_ACPI_DMAR_HEADER) - 1;

  //
  // 1. Provide invalid input data to AddDrhd() API.
  //
  Status = AddDrhd (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  Status = AddDrhd (Dmar, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  Status = AddDrhd (NULL, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // 2. Wrong Signature in DMAR ACPI header
  //
  Dmar->Header.Signature = 0xDEADBEEF;
  Status                 = AddDrhd (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_COMPROMISED_DATA);

  //
  // 3. Wrong length in DMAR ACPI header
  //
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER) - 1;
  Status                 = AddDrhd (Dmar, VtdTopologyProtocol);
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


  Status = AddDrhd (Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // 5. Provide valid data via VtdTopologyProtocol APIs and compare created DMAR ACPI table
  //    with a pattern.
  //

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 2);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetVtdEngineInfo, This, VtdTopologyProtocol, 2);
  expect_in_range_count (MockGetVtdEngineInfo, VtdEngineId, 0, 1, 2);
  will_return (MockGetVtdEngineInfo, 0xDEADBEEF0000);
  will_return (MockGetVtdEngineInfo, SIZE_64KB);
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0xDEADBEEF1000);
  will_return (MockGetVtdEngineInfo, SIZE_4KB);
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 0);
  will_return (MockGetVtdEngineInfo, 1);

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevices, This, VtdTopologyProtocol, 2);
  expect_in_range_count (MockGetDevices, VtdEngineId, 0, 1, 2);
  DEVICE_INFO  Iommu0DevicesList[] = {
    { { 0, 0x2, 0 }, PciEndpointDevice, 0 },
    { { 0, 0xB, 0 }, PciBridge, 0 }
  };

  DEVICE_INFO  Iommu1DevicesList[] = {
    { { 0, 0x15, 0x01 }, IoApicDevice, 2 },
    { { 0, 0x15, 0x02 }, HpetDevice,   0 }
  };

  DEVICE_INFO  *Iommu0DevicesListPtr = AllocateZeroPool (sizeof (Iommu0DevicesList));
  DEVICE_INFO  *Iommu1DevicesListPtr = AllocateZeroPool (sizeof (Iommu1DevicesList));

  CopyMem (Iommu0DevicesListPtr, &Iommu0DevicesList, sizeof (Iommu0DevicesList));
  CopyMem (Iommu1DevicesListPtr, &Iommu1DevicesList, sizeof (Iommu1DevicesList));

  // call GetPciDevices for IOMMU0
  will_return (MockGetDevices, Iommu0DevicesListPtr);
  will_return (MockGetDevices, 2);
  will_return (MockGetDevices, EFI_SUCCESS);
  // call GetPciDevices for IOMMU1
  will_return (MockGetDevices, Iommu1DevicesListPtr);
  will_return (MockGetDevices, 2);
  will_return (MockGetDevices, EFI_SUCCESS);

  Status = AddDrhd (Dmar, VtdTopologyProtocol);

  //
  // Dump DMAR header
  //
  Array = (UINT8 *)&UnitTest1DmarTable;
  printf ("Original DRHD entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_DRHD_TABLE_STRUCTURE));

  Array = ((UINT8 *)Dmar) + sizeof (EFI_ACPI_DMAR_HEADER);
  printf ("Produced DRHD entries:\n");
  DumpBuffer (Array, sizeof (UNIT_TEST_DRHD_TABLE_STRUCTURE));

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL (&UnitTest1DmarTable, (UINT8 *)Dmar + sizeof (EFI_ACPI_DMAR_HEADER), sizeof (UNIT_TEST_DRHD_TABLE_STRUCTURE));

  FreePool (VtdTopologyProtocol);
  FreePool (Dmar);

  return UNIT_TEST_PASSED;
}
