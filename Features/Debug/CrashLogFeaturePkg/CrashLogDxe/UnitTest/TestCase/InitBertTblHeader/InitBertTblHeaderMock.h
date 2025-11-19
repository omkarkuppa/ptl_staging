/**@file
    Unit test case mock header for InitBertTblHeader.

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
#ifndef _INIT_BERT_TABLE_HEADER_MOCK_H_
#define _INIT_BERT_TABLE_HEADER_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <IndustryStandard/Acpi.h>
#include <Guid/Cper.h>
#include <BertTable.h>

/**
  Initialize the APEI BERT EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER structure.

  @param[in,out] BertTable  point of the BERT EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER instance.
  @param[in] Size           Total size of all records.
  @param[in] Count          Count of crashlog record.

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InitBertTblHeader (
  IN OUT EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  **BertTable,
  IN     UINT32                                       Size,
  IN     UINT32                                       Count
  );

/**
  Initialize ACPI descriptor header.

  @param[in] BertPtr  pointer of boot error record table header.

**/
VOID
InitAcpiDescHeader (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertPtr
  );

/**
  Initialize boot error status block.

  @param[in] BertPtr  pointer of boot error record table header.
  @param[in] Size     Total size of all records.
  @param[in] Count    Count of crashlog record.

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InitErrorStatusBlk (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertPtr,
  IN UINT32                                       Size,
  IN UINT32                                       Count
  );

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
  );

/**
  Unit test for InitErrorStatusBlk.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitErrorStatusBlk (
  IN UNIT_TEST_CONTEXT  Context
  );

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
  );
#endif /*_INIT_BERT_TABLE_HEADER_MOCK_H_*/
