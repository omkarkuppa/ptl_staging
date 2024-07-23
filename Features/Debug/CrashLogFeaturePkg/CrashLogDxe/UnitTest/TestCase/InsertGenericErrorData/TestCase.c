/**@file
    Unit test cases of InsertGenericErrorData.

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

#include "InsertGenericErrorDataMock.h"

/**
  Unit test for CheckBootErrRegionSize.

  @param[in] Context  An optional parameter from precondition.

  @retval UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCheckBootErrRegionSize (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                                   Status;
  UINT32                                       DataSize;
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertTable;
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;

  BertTable = AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));

  BertTable->BootErrorRegionLength = 100;
  BertTable->BootErrorRegion = (UINTN)AllocateZeroPool (BertTable->BootErrorRegionLength);

  ErrStsBlk = (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)(BertTable->BootErrorRegion);
  ErrStsBlk->DataLength = 10;

  Status = CheckBootErrRegionSize (NULL, 0);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  DataSize = 100;
  Status = CheckBootErrRegionSize (BertTable, DataSize);
  UT_ASSERT_EQUAL (Status, EFI_OUT_OF_RESOURCES);

  DataSize = 90;
  Status = CheckBootErrRegionSize (BertTable, DataSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  FreePool ((VOID *)(BertTable->BootErrorRegion));
  FreePool (BertTable);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for InsertGenericErrorData success.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInsertGenericErrorData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertTable;
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;
  UINT64                                       DataAddr;
  UINT32                                       DataSize;
  UINT8                                        *PayloadAddr;
  EFI_STATUS                                   Status;

  //
  // Mock Input
  //
  UINT8 TempData[0x100] = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, };
  DataSize = 0x20;
  DataAddr = (UINT64)TempData;

  BertTable = AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));
  ErrStsBlk = AllocateZeroPool (0x100);
  BertTable->BootErrorRegionLength           = 0x100;
  BertTable->BootErrorRegion                 = (UINT64)ErrStsBlk;
  ErrStsBlk->DataLength                      = 0x40;
  ErrStsBlk->BlockStatus.ErrorDataEntryCount = 1;

  //
  // Running platform function
  //
  Status = InsertGenericErrorData (BertTable, DataAddr, DataSize);
  PayloadAddr  = (UINT8 *)(VOID *)ErrStsBlk;
  PayloadAddr += sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE) + 0x40 + sizeof (FIRMWARE_GENERIC_ERROR);

  //
  // Check result
  //
  UT_ASSERT_MEM_EQUAL (PayloadAddr, TempData, DataSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  Status = InsertGenericErrorData (NULL, DataAddr, DataSize);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  FreePool(ErrStsBlk);
  FreePool(BertTable);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for InsertGenericErrorData failure.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInsertGenericErrorData_Fail (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertTable;
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;
  UINT64                                       DataAddr;
  UINT32                                       DataSize;
  EFI_STATUS                                   Status;

  //
  // Mock Input
  //
  UINT8 TempData[0x100] = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, };
  DataSize = 0x30;
  DataAddr = (UINT64)TempData;

  BertTable = AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));
  ErrStsBlk = AllocateZeroPool (0x100);
  BertTable->BootErrorRegionLength           = 0x100;
  BertTable->BootErrorRegion                 = (UINT64)ErrStsBlk;
  ErrStsBlk->DataLength                      = 0xE0;
  ErrStsBlk->BlockStatus.ErrorDataEntryCount = 1;

  //
  // Running platform function
  //
  Status = InsertGenericErrorData (BertTable, DataAddr, DataSize);
  FreePool (ErrStsBlk);
  FreePool (BertTable);

  //
  // Check result
  //
  UT_ASSERT_EQUAL (Status, EFI_OUT_OF_RESOURCES);
  return UNIT_TEST_PASSED;
}

