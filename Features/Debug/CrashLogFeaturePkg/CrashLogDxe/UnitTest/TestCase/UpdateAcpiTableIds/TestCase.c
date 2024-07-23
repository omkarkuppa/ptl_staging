/**@file
    Unit test cases of UpdateAcpiTableIds.

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

#include "UpdateAcpiTableIdsMock.h"

/**
  Unit test for UpdateAcpiTableIds.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestUpdateAcpiTableIds (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_ACPI_DESCRIPTION_HEADER  *TableHeader;

  UINT8 OemId[6] = {'I', 'N', 'T', 'E', 'L', ' '};

  //
  // Mock Input
  //
  TableHeader = AllocateZeroPool (sizeof (EFI_ACPI_DESCRIPTION_HEADER));

  //
  // Running platform function
  //
  UpdateAcpiTableIds (TableHeader);

  //
  // Check result
  //
  UT_ASSERT_MEM_EQUAL (TableHeader->OemId, OemId, 6);
  UT_ASSERT_EQUAL (TableHeader->OemTableId, SIGNATURE_64 ('E', 'D', 'K', '2', ' ', ' ', ' ', ' '));
  UT_ASSERT_EQUAL (TableHeader->OemRevision, 0x0001);
  UT_ASSERT_EQUAL (TableHeader->CreatorId, SIGNATURE_32 ('I', 'N', 'T', 'L'));
  UT_ASSERT_EQUAL (TableHeader->CreatorRevision, 0x00000001);

  FreePool (TableHeader);
  return UNIT_TEST_PASSED;
}
