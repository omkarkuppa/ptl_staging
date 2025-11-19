/** @file
  Unit test cases for CalculateDmarLength API.

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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

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
  VtdTopologyProtocol->GetSystemInfo                    = MockGetSystemInfo;
  VtdTopologyProtocol->GetRootPortsSupportingAts        = MockGetRootPortsSupportingAts;

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
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks if functions behaves properly for invalid parameters or other unexpected situations.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthInvalidParameters (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  UINT32                 Length;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  //
  // 1. Invalid parameters. Provide invalid input parameter to CalculateDmarLength() API.
  //
  Length = CalculateDmarLength (NULL, NULL);
  UT_ASSERT_EQUAL (Length, 0);


  //
  // 2. In this test case GetNumberOfVtdEngines() returns an error.
  //
  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 0);
  will_return (MockGetNumberOfVtdEngines, EFI_INVALID_PARAMETER);

  Length = CalculateDmarLength (VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Length, 0);

  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhd (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  UINT32                 Length;
  UINTN                  Index;
  UINT32                 ExpectedLength;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  ExpectedLength = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Calculate the size of DMAR ACPI table which have only DHRDs entries. In this testcase, th function GetPciDevices
  // returns number of PCI devices which are within a specific VT-d engine scope.
  //

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 3);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  ExpectedLength += (3 * sizeof (EFI_ACPI_DMAR_DRHD_HEADER));

  //
  // Mock GetPciDevices from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevices, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevices, VtdEngineId, 0, 2, 3);
  will_return (MockGetDevices, 2);
  will_return (MockGetDevices, EFI_SUCCESS);
  will_return (MockGetDevices, 4);
  will_return (MockGetDevices, EFI_SUCCESS);
  will_return (MockGetDevices, 0);
  will_return (MockGetDevices, EFI_SUCCESS);

  ExpectedLength += (6 * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH)));

  //
  // Mock GetPciDevicesWithAtc from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetPciDevicesWithAtc, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetPciDevicesWithAtc, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetPciDevicesWithAtc, 0);
    will_return (MockGetPciDevicesWithAtc, EFI_UNSUPPORTED);
  }

  //
  // Mock GetDevicesWithProperties from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevicesWithProperties, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevicesWithProperties, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevicesWithProperties, 0);
    will_return (MockGetDevicesWithProperties, EFI_UNSUPPORTED);
  }

  //
  // Mock GetNumberOfReservedMemoryRegions from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfReservedMemoryRegions, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfReservedMemoryRegions, 0);
  will_return (MockGetNumberOfReservedMemoryRegions, EFI_UNSUPPORTED);

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetSystemInfo, This, VtdTopologyProtocol, 2);
  for (Index = 0; Index < 2; Index++) {
    will_return (MockGetSystemInfo, 0);
    will_return (MockGetSystemInfo, FALSE);
    will_return (MockGetSystemInfo, 1);
    will_return (MockGetSystemInfo, EFI_SUCCESS);
  }

  //
  // Mock GetRootPortsSupportingAts from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetRootPortsSupportingAts, This, VtdTopologyProtocol);
  expect_value (MockGetRootPortsSupportingAts, SegmentId, 0);
  will_return (MockGetRootPortsSupportingAts, EFI_UNSUPPORTED);

  //
  // Mock LibPcdGetBool
  //
  expect_value_count (LibPcdGetBool, TokenNumber, 0, -1);
  will_return_always (LibPcdGetBool, TRUE);

  //
  // Execute test function with above mock environment
  //
  Length = CalculateDmarLength (VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Length, ExpectedLength);

  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and RMRR entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndRmrr (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  UINT32                 Length;
  UINTN                  Index;
  UINT32                 ExpectedLength;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  ExpectedLength = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Calculate the size of DMAR ACPI table which have DHRD and RMRR entries.
  //

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 3);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  ExpectedLength += (3 * sizeof (EFI_ACPI_DMAR_DRHD_HEADER));

  //
  // Mock GetPciDevices from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevices, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevices, VtdEngineId, 0, 2, 3);
  will_return (MockGetDevices, 2);
  will_return (MockGetDevices, EFI_SUCCESS);
  will_return (MockGetDevices, 4);
  will_return (MockGetDevices, EFI_SUCCESS);
  will_return (MockGetDevices, 0);
  will_return (MockGetDevices, EFI_SUCCESS);

  ExpectedLength += (6 * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH)));

  //
  // Mock GetPciDevicesWithAtc from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetPciDevicesWithAtc, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetPciDevicesWithAtc, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetPciDevicesWithAtc, 0);
    will_return (MockGetPciDevicesWithAtc, EFI_UNSUPPORTED);
  }

  //
  // Mock GetDevicesWithProperties from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevicesWithProperties, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevicesWithProperties, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevicesWithProperties, 0);
    will_return (MockGetDevicesWithProperties, EFI_UNSUPPORTED);
  }

  //
  // Mock GetNumberOfReservedMemoryRegions from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfReservedMemoryRegions, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfReservedMemoryRegions, 2);
  will_return (MockGetNumberOfReservedMemoryRegions, EFI_SUCCESS);

  ExpectedLength += (2 * sizeof (EFI_ACPI_DMAR_RMRR_HEADER));

  //
  // Mock GetReservedMemoryRegionInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetReservedMemoryRegionInfo, This, VtdTopologyProtocol, 2);
  expect_in_range_count (MockGetReservedMemoryRegionInfo, ReservedMemoryRegionId, 0, 1, 2);
  will_return (MockGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return_count (MockGetReservedMemoryRegionInfo, 0, 3); // fill Info variable with 0s
  will_return (MockGetReservedMemoryRegionInfo, 1);          // number of devices
  will_return (MockGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return_count (MockGetReservedMemoryRegionInfo, 0, 3); // fill Info variable with 0s
  will_return (MockGetReservedMemoryRegionInfo, 2);          // number of devices

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetSystemInfo, This, VtdTopologyProtocol, 2);
  for (Index = 0; Index < 2; Index++) {
    will_return (MockGetSystemInfo, 0);
    will_return (MockGetSystemInfo, FALSE);
    will_return (MockGetSystemInfo, 1);
    will_return (MockGetSystemInfo, EFI_SUCCESS);
  }

  //
  // Mock GetRootPortsSupportingAts from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetRootPortsSupportingAts, This, VtdTopologyProtocol);
  expect_value (MockGetRootPortsSupportingAts, SegmentId, 0);
  will_return (MockGetRootPortsSupportingAts, EFI_UNSUPPORTED);

  //
  // Mock LibPcdGetBool
  //
  expect_value_count (LibPcdGetBool, TokenNumber, 0, -1);
  will_return_always (LibPcdGetBool, TRUE);

  ExpectedLength += (3 * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH)));

  Length = CalculateDmarLength (VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Length, ExpectedLength);

  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and OEM RMRR entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndOemRmrr (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  OEM_DMAR_PROTOCOL      *OemDmarProtocol;
  UINT32                 Length;
  UINTN                  Index;
  UINT32                 ExpectedLength;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();
  OemDmarProtocol = CreateOemDmarProtocol ();

  ExpectedLength = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Calculate the size of DMAR ACPI table which have DHRD and RMRR entries.
  //

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 3);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  ExpectedLength += (3 * sizeof (EFI_ACPI_DMAR_DRHD_HEADER));

  //
  // Mock GetPciDevices from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevices, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevices, VtdEngineId, 0, 2, 3);
  will_return (MockGetDevices, 2);
  will_return (MockGetDevices, EFI_SUCCESS);
  will_return (MockGetDevices, 4);
  will_return (MockGetDevices, EFI_SUCCESS);
  will_return (MockGetDevices, 0);
  will_return (MockGetDevices, EFI_SUCCESS);

  ExpectedLength += (6 * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH)));

  //
  // Mock GetPciDevicesWithAtc from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetPciDevicesWithAtc, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetPciDevicesWithAtc, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetPciDevicesWithAtc, 0);
    will_return (MockGetPciDevicesWithAtc, EFI_UNSUPPORTED);
  }

  //
  // Mock GetDevicesWithProperties from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevicesWithProperties, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevicesWithProperties, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevicesWithProperties, 0);
    will_return (MockGetDevicesWithProperties, EFI_UNSUPPORTED);
  }

  //
  // Mock GetNumberOfReservedMemoryRegions from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfReservedMemoryRegions, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfReservedMemoryRegions, 0);
  will_return (MockGetNumberOfReservedMemoryRegions, EFI_UNSUPPORTED);

  //
  // Mock GetNumberOfReservedMemoryRegions from OEM_DMAR_PROTOCOL
  //
  expect_value (MockOemGetNumberOfReservedMemoryRegions, This, OemDmarProtocol);
  will_return (MockOemGetNumberOfReservedMemoryRegions, 2);
  will_return (MockOemGetNumberOfReservedMemoryRegions, EFI_SUCCESS);

  ExpectedLength += (2 * sizeof (EFI_ACPI_DMAR_RMRR_HEADER));

  //
  // Mock GetReservedMemoryRegionInfo from OEM_DMAR_PROTOCOL
  //
  expect_value_count (MockOemGetReservedMemoryRegionInfo, This, OemDmarProtocol, 2);
  expect_in_range_count (MockOemGetReservedMemoryRegionInfo, ReservedMemoryRegionId, 0, 1, 2);
  will_return (MockOemGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return_count (MockOemGetReservedMemoryRegionInfo, 0, 3); // fill Info variable with 0s
  will_return (MockOemGetReservedMemoryRegionInfo, 1);          // number of devices
  will_return (MockOemGetReservedMemoryRegionInfo, EFI_SUCCESS);
  will_return_count (MockOemGetReservedMemoryRegionInfo, 0, 3); // fill Info variable with 0s
  will_return (MockOemGetReservedMemoryRegionInfo, 4);          // number of devices

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetSystemInfo, This, VtdTopologyProtocol, 2);
  for (Index = 0; Index < 2; Index++) {
    will_return (MockGetSystemInfo, 0);
    will_return (MockGetSystemInfo, FALSE);
    will_return (MockGetSystemInfo, 1);
    will_return (MockGetSystemInfo, EFI_SUCCESS);
  }

  //
  // Mock GetRootPortsSupportingAts from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetRootPortsSupportingAts, This, VtdTopologyProtocol);
  expect_value (MockGetRootPortsSupportingAts, SegmentId, 0);
  will_return (MockGetRootPortsSupportingAts, EFI_UNSUPPORTED);

  //
  // Mock LibPcdGetBool
  //
  expect_value_count (LibPcdGetBool, TokenNumber, 0, -1);
  will_return_always (LibPcdGetBool, TRUE);

  ExpectedLength += (5 * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH)));

  Length = CalculateDmarLength (VtdTopologyProtocol, OemDmarProtocol);
  UT_ASSERT_EQUAL (Length, ExpectedLength);

  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and SATC entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndSatc (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  UINT32                 Length;
  UINTN                  Index;
  UINT32                 ExpectedLength;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  ExpectedLength = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Calculate the size of DMAR ACPI table which have only DRHD + SATC entries.
  //

  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 3);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  ExpectedLength += (3 * sizeof (EFI_ACPI_DMAR_DRHD_HEADER));

  //
  // Mock GetPciDevices from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevices, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevices, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevices, 0);
    will_return (MockGetDevices, EFI_UNSUPPORTED);
  }

  //
  // Mock GetPciDevicesWithAtc from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetPciDevicesWithAtc, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetPciDevicesWithAtc, VtdEngineId, 0, 2, 3);
  will_return (MockGetPciDevicesWithAtc, 1);
  will_return (MockGetPciDevicesWithAtc, EFI_SUCCESS);
  will_return (MockGetPciDevicesWithAtc, 3);
  will_return (MockGetPciDevicesWithAtc, EFI_SUCCESS);
  will_return (MockGetPciDevicesWithAtc, 0);
  will_return (MockGetPciDevicesWithAtc, EFI_SUCCESS);

  ExpectedLength += 4 * (sizeof (EFI_ACPI_DMAR_SATC_HEADER) + (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH)));

  //
  // Mock GetDevicesWithProperties from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevicesWithProperties, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevicesWithProperties, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevicesWithProperties, 0);
    will_return (MockGetDevicesWithProperties, EFI_UNSUPPORTED);
  }

  //
  // Mock MockGetNumberOfReservedMemoryRegions from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfReservedMemoryRegions, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfReservedMemoryRegions, 0);
  will_return (MockGetNumberOfReservedMemoryRegions, EFI_UNSUPPORTED);

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetSystemInfo, This, VtdTopologyProtocol, 2);
  for (Index = 0; Index < 2; Index++) {
    will_return (MockGetSystemInfo, 0);
    will_return (MockGetSystemInfo, FALSE);
    will_return (MockGetSystemInfo, 1);
    will_return (MockGetSystemInfo, EFI_SUCCESS);
  }

  //
  // Mock GetRootPortsSupportingAts from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetRootPortsSupportingAts, This, VtdTopologyProtocol);
  expect_value (MockGetRootPortsSupportingAts, SegmentId, 0);
  will_return (MockGetRootPortsSupportingAts, EFI_UNSUPPORTED);

  //
  // Mock LibPcdGetBool
  //
  expect_value_count (LibPcdGetBool, TokenNumber, 0, -1);
  will_return_always (LibPcdGetBool, TRUE);

  //
  // Execute test function with above mock environment
  //
  Length = CalculateDmarLength (VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Length, ExpectedLength); // 160

  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and SIDP entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndSidp (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  UINT32                 Length;
  UINTN                  Index;
  UINT32                 ExpectedLength;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  ExpectedLength = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Calculate the size of DMAR ACPI table which have only DRHD + SIDP entries.
  //
  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 3);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  ExpectedLength += (3 * sizeof (EFI_ACPI_DMAR_DRHD_HEADER));

  //
  // Mock GetPciDevices from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevices, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevices, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevices, 0);
    will_return (MockGetDevices, EFI_UNSUPPORTED);
  }

  //
  // Mock GetPciDevicesWithAtc from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetPciDevicesWithAtc, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetPciDevicesWithAtc, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetPciDevicesWithAtc, 0);
    will_return (MockGetPciDevicesWithAtc, EFI_UNSUPPORTED);
  }

  //
  // Mock GetDevicesWithProperties from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevicesWithProperties, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevicesWithProperties, VtdEngineId, 0, 2, 3);
  will_return (MockGetDevicesWithProperties, 1);
  will_return (MockGetDevicesWithProperties, EFI_SUCCESS);
  will_return (MockGetDevicesWithProperties, 3);
  will_return (MockGetDevicesWithProperties, EFI_SUCCESS);
  will_return (MockGetDevicesWithProperties, 0);
  will_return (MockGetDevicesWithProperties, EFI_SUCCESS);

  ExpectedLength += 2 * sizeof (EFI_ACPI_DMAR_SIDP_HEADER) + 4 * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));

  //
  // Mock GetNumberOfReservedMemoryRegions from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfReservedMemoryRegions, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfReservedMemoryRegions, 0);
  will_return (MockGetNumberOfReservedMemoryRegions, EFI_UNSUPPORTED);

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetSystemInfo, This, VtdTopologyProtocol, 2);
  for (Index = 0; Index < 2; Index++) {
    will_return (MockGetSystemInfo, 0);
    will_return (MockGetSystemInfo, FALSE);
    will_return (MockGetSystemInfo, 1);
    will_return (MockGetSystemInfo, EFI_SUCCESS);
  }

  //
  // Mock GetRootPortsSupportingAts from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetRootPortsSupportingAts, This, VtdTopologyProtocol);
  expect_value (MockGetRootPortsSupportingAts, SegmentId, 0);
  will_return (MockGetRootPortsSupportingAts, EFI_UNSUPPORTED);

  //
  // Mock LibPcdGetBool
  //
  expect_value_count (LibPcdGetBool, TokenNumber, 0, -1);
  will_return_always (LibPcdGetBool, TRUE);

  //
  // Execute test function with above mock environment
  //
  Length = CalculateDmarLength (VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Length, ExpectedLength);

  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and RHSA entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndRhsa (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  UINT32                 ExpectedLength;
  UINT32                 Length;
  UINTN                  Index;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  ExpectedLength = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Calculate the size of DMAR ACPI table which have only DRHD + RHSA entries.
  //
  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 3);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  ExpectedLength += (3 * (sizeof (EFI_ACPI_DMAR_DRHD_HEADER) + sizeof (EFI_ACPI_DMAR_RHSA_HEADER)));

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetSystemInfo, This, VtdTopologyProtocol, 2);
  for (Index = 0; Index < 2; Index++) {
    will_return (MockGetSystemInfo, 0);
    will_return (MockGetSystemInfo, TRUE);
    will_return (MockGetSystemInfo, 1);
    will_return (MockGetSystemInfo, EFI_SUCCESS);
  }

  //
  // Mock GetPciDevices from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevices, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevices, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevices, 0);
    will_return (MockGetDevices, EFI_UNSUPPORTED);
  }

  //
  // Mock GetPciDevicesWithAtc from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetPciDevicesWithAtc, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetPciDevicesWithAtc, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetPciDevicesWithAtc, 0);
    will_return (MockGetPciDevicesWithAtc, EFI_UNSUPPORTED);
  }

  //
  // Mock GetDevicesWithProperties from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevicesWithProperties, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevicesWithProperties, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevicesWithProperties, 0);
    will_return (MockGetDevicesWithProperties, EFI_UNSUPPORTED);
  }

  //
  // Mock GetNumberOfReservedMemoryRegions from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfReservedMemoryRegions, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfReservedMemoryRegions, 0);
  will_return (MockGetNumberOfReservedMemoryRegions, EFI_UNSUPPORTED);

  //
  // Mock MockGetRootPortsSupportingAts from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetRootPortsSupportingAts, This, VtdTopologyProtocol);
  expect_value (MockGetRootPortsSupportingAts, SegmentId, 0);
  will_return (MockGetRootPortsSupportingAts, EFI_UNSUPPORTED);

  //
  // Mock LibPcdGetBool
  //
  expect_value_count (LibPcdGetBool, TokenNumber, 0, -1);
  will_return_always (LibPcdGetBool, TRUE);

  //
  // Execute test function with above mock environment
  //
  Length = CalculateDmarLength (VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Length, ExpectedLength);

  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and ATSR entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndAtsr (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  UINT32                 ExpectedLength;
  UINT32                 Length;
  UINTN                  Index;

  VtdTopologyProtocol = CreateVtdTopologyProtocol ();

  ExpectedLength = sizeof (EFI_ACPI_DMAR_HEADER);

  //
  // Calculate the size of DMAR ACPI table which have only DRHD + ATSR entries.
  //
  //
  // Mock GetNumberOfVtdEngines from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfVtdEngines, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfVtdEngines, 3);
  will_return (MockGetNumberOfVtdEngines, EFI_SUCCESS);

  ExpectedLength += 3 * (sizeof (EFI_ACPI_DMAR_DRHD_HEADER));

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetSystemInfo, This, VtdTopologyProtocol, 2);
  for (Index = 0; Index < 2; Index++) {
    will_return (MockGetSystemInfo, 0);
    will_return (MockGetSystemInfo, FALSE);
    will_return (MockGetSystemInfo, 2);
    will_return (MockGetSystemInfo, EFI_SUCCESS);
  }

  //
  // Mock GetPciDevices from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevices, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevices, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevices, 0);
    will_return (MockGetDevices, EFI_UNSUPPORTED);
  }

  //
  // Mock GetPciDevicesWithAtc from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetPciDevicesWithAtc, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetPciDevicesWithAtc, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetPciDevicesWithAtc, 0);
    will_return (MockGetPciDevicesWithAtc, EFI_UNSUPPORTED);
  }

  //
  // Mock GetDevicesWithProperties from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetDevicesWithProperties, This, VtdTopologyProtocol, 3);
  expect_in_range_count (MockGetDevicesWithProperties, VtdEngineId, 0, 2, 3);
  for (Index = 0; Index < 3; Index++) {
    will_return (MockGetDevicesWithProperties, 0);
    will_return (MockGetDevicesWithProperties, EFI_UNSUPPORTED);
  }

  //
  // Mock GetNumberOfReservedMemoryRegions from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetNumberOfReservedMemoryRegions, This, VtdTopologyProtocol);
  will_return (MockGetNumberOfReservedMemoryRegions, 0);
  will_return (MockGetNumberOfReservedMemoryRegions, EFI_UNSUPPORTED);

  //
  // Mock MockGetRootPortsSupportingAts from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value_count (MockGetRootPortsSupportingAts, This, VtdTopologyProtocol, 2);
  expect_in_range_count (MockGetRootPortsSupportingAts, SegmentId, 0, 1, 2);
  will_return (MockGetRootPortsSupportingAts, EFI_SUCCESS);
  will_return (MockGetRootPortsSupportingAts, 2);
  will_return (MockGetRootPortsSupportingAts, EFI_SUCCESS);
  will_return (MockGetRootPortsSupportingAts, 0);

  ExpectedLength += 2 * sizeof (EFI_ACPI_DMAR_ATSR_HEADER) + 2 * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));

  //
  // Mock LibPcdGetBool
  //
  expect_value_count (LibPcdGetBool, TokenNumber, 0, -1);
  will_return_always (LibPcdGetBool, TRUE);

  //
  // Execute test function with above mock environment
  //
  Length = CalculateDmarLength (VtdTopologyProtocol, NULL);
  UT_ASSERT_EQUAL (Length, ExpectedLength);

  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}