/**@file
    Unit test cases of InstallApeiBertTables.

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

#include "InstallApeiBertTablesMock.h"

/**
  Unit test for SetBootErrRegionLen.

  @param[in] Context  An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSetBootErrRegionLen (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN                                        ExpectedLen;
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *Bert;

  Bert = AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));
  Bert->BootErrorRegion = (UINTN)AllocateZeroPool (sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE));

  SetBootErrRegionLen (NULL);

  UT_ASSERT_EQUAL (Bert->BootErrorRegionLength, 0);

  ExpectedLen = sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE);

  SetBootErrRegionLen (Bert);

  UT_ASSERT_EQUAL (Bert->BootErrorRegionLength, ExpectedLen);

  FreePool ((VOID *)(Bert->BootErrorRegion));
  FreePool (Bert);

  return UNIT_TEST_PASSED;
} 

/**
  Unit test for InstallApeiBertTables.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInstallApeiBertTables (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                                   Status;
  EFI_ACPI_TABLE_PROTOCOL                      *AcpiTbl;
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *Bert;
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *BertErrorBlk;

  //
  // Mock Input
  //
  AcpiTbl      = gAcpiTbl;
  Bert         = AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));
  BertErrorBlk = AllocateZeroPool (sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE));
  Bert->BootErrorRegion    = (UINT64)BertErrorBlk;
  BertErrorBlk->DataLength = 0x100;

  Status = InstallApeiBertTables (AcpiTbl, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  //
  // Running platform function
  //
  Status = InstallApeiBertTables (AcpiTbl, Bert);

  //
  // Check result
  //
  UT_ASSERT_EQUAL (Bert->BootErrorRegionLength, 0x100 + sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE));
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  FreePool(BertErrorBlk);
  FreePool(Bert);
  return UNIT_TEST_PASSED;
}
