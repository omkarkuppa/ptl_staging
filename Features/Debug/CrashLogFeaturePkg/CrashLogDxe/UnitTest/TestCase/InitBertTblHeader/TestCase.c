/**@file
    Unit test cases of InitBertTblHeader.

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

#include "InitBertTblHeaderMock.h"

/**
  Unit test for InitAcpiDescHeader.

  @param[in] Context  An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitAcpiDescHeader (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertPtr;
  BertPtr = AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));

  InitAcpiDescHeader (BertPtr);

  UT_ASSERT_EQUAL (BertPtr->Header.Signature, EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_SIGNATURE);
  UT_ASSERT_EQUAL (BertPtr->Header.Length, sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));
  UT_ASSERT_EQUAL (BertPtr->Header.Revision, EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_REVISION);
  UT_ASSERT_EQUAL (BertPtr->Header.Checksum, 0);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for InitErrorStatusBlk.

  @param[in] Context  An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitErrorStatusBlk (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                                   Status;
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertPtr;
  UINT32                                       Size;
  UINT32                                       Count;
  UINT32                                       BootErrorRegionSize;

  Status = InitErrorStatusBlk (NULL, 0, 0);

  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  BertPtr = AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));

  Count = 10;
  Size  = 10;
  BootErrorRegionSize = sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE) + sizeof (FIRMWARE_GENERIC_ERROR) * Count + Size;

  Status = InitErrorStatusBlk (BertPtr, Count, Size);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (BertPtr->BootErrorRegionLength, BootErrorRegionSize);
  UT_ASSERT_NOT_EQUAL (BertPtr->BootErrorRegion, 0);

  FreePool (BertPtr);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for InitBertTblHeader.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitBertTblHeader (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertTable;
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;
  UINT32                                       PayloadCount;
  UINT32                                       PayloadSize;
  EFI_STATUS                                   Status;
  UINT32                                       ErrStsHeaderSize;

  //
  // Mock Input
  //
  PayloadCount = 3;
  PayloadSize  = 0x400;

  //
  // Running platform function
  //
  Status = InitBertTblHeader (&BertTable, PayloadSize, PayloadCount);
  ErrStsBlk = (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)BertTable->BootErrorRegion;
  ErrStsHeaderSize = sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE);

  //
  // Check result
  //
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (BertTable->Header.Signature, SIGNATURE_32('B', 'E', 'R', 'T'));
  UT_ASSERT_EQUAL (BertTable->Header.Length, sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));
  UT_ASSERT_EQUAL (BertTable->Header.Revision, EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_REVISION);
  UT_ASSERT_EQUAL (BertTable->Header.Checksum, 0);
  UT_ASSERT_EQUAL (BertTable->BootErrorRegionLength, ErrStsHeaderSize + PayloadSize + PayloadCount * sizeof (FIRMWARE_GENERIC_ERROR));
  UT_ASSERT_EQUAL (ErrStsBlk->ErrorSeverity, EFI_ACPI_6_5_ERROR_SEVERITY_FATAL);
  UT_ASSERT_EQUAL (ErrStsBlk->DataLength, 0);
  UT_ASSERT_EQUAL (ErrStsBlk->RawDataLength, 0);
  UT_ASSERT_EQUAL (ErrStsBlk->RawDataOffset, 0);
  UT_ASSERT_EQUAL (ErrStsBlk->BlockStatus.ErrorDataEntryCount, 0);
  UT_ASSERT_EQUAL (ErrStsBlk->BlockStatus.MultipleUncorrectableErrors, 1);
  UT_ASSERT_EQUAL (ErrStsBlk->BlockStatus.UncorrectableErrorValid, 1);

  return UNIT_TEST_PASSED;
}
