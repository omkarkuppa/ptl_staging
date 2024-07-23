/**@file
  Unit test cases for CreateHeader API.

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

@par Specification
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
#include "Utils/Utils.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

//
// defines for test purposes
//
#define UT_HOST_ADDRES_WIDTH  42
#define UT_DMAR_REVISION      1

/**
  Unit-Test Function for CreateHeader.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCreateHeader (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS             Status;
  EFI_ACPI_DMAR_HEADER   Dmar;
  VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol;
  UINT8                  *Array;

  VtdTopologyProtocol                = AllocateZeroPool (sizeof (VTD_TOPOLOGY_PROTOCOL));
  VtdTopologyProtocol->GetSystemInfo = MockGetSystemInfo;

  //
  // 1. Provide invalid input parameters to CreateHeader() API.
  //
  Status = CreateHeader (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  Status = CreateHeader (&Dmar, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  Status = CreateHeader (NULL, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // 2. GetSystemInfo() returns an error.
  //

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetSystemInfo, This, VtdTopologyProtocol);
  will_return (MockGetSystemInfo, 0);
  will_return (MockGetSystemInfo, FALSE);
  will_return (MockGetSystemInfo, 1);
  will_return (MockGetSystemInfo, EFI_UNSUPPORTED);

  Status = CreateHeader (&Dmar, VtdTopologyProtocol);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  //
  // 3. Correct flow. Provide valid data via VtdTopologyProtocol APIs and compare created DMAR ACPI header
  //    with the expectations.
  //

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdDmarFlags);
  will_return (LibPcdGet8, BIT0 | BIT2);

  //
  // Mock GetSystemInfo from VTD_TOPOLOGY_PROTOCOL
  //
  expect_value (MockGetSystemInfo, This, VtdTopologyProtocol);
  will_return (MockGetSystemInfo, UT_HOST_ADDRES_WIDTH);
  will_return (MockGetSystemInfo, FALSE);
  will_return (MockGetSystemInfo, 1);
  will_return (MockGetSystemInfo, EFI_SUCCESS);

  //
  // Execute test function with above mock environment
  //
  Status = CreateHeader (&Dmar, VtdTopologyProtocol);

  //
  // Test case
  //
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL (Dmar.Header.OemId, _PCD_VALUE_PcdAcpiDefaultOemId, 6);
  UT_ASSERT_EQUAL (Dmar.Header.Revision, 1);
  UT_ASSERT_EQUAL (Dmar.Header.OemTableId, _PCD_VALUE_PcdAcpiDefaultOemTableId);
  UT_ASSERT_EQUAL (Dmar.Header.OemRevision, _PCD_VALUE_PcdAcpiDefaultOemRevision);
  UT_ASSERT_EQUAL (Dmar.Header.CreatorId, _PCD_VALUE_PcdAcpiDefaultCreatorId);
  UT_ASSERT_EQUAL (Dmar.Header.CreatorRevision, _PCD_VALUE_PcdAcpiDefaultCreatorRevision);
  UT_ASSERT_EQUAL (Dmar.HostAddressWidth, UT_HOST_ADDRES_WIDTH);

  //
  // Dump DMAR header
  //
  Array = (UINT8 *)&Dmar;
  printf ("DMAR header:\n");
  DumpBuffer (Array, sizeof (EFI_ACPI_DMAR_HEADER));

  //
  // Destroy Mock Service
  //
  FreePool (VtdTopologyProtocol);

  return UNIT_TEST_PASSED;
}
