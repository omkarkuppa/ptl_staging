/**@file
    Unit test case mock header for InsertGenericErrorData.

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
#ifndef _INSERT_GENERIC_ERROR_DATA_MOCK_H_
#define _INSERT_GENERIC_ERROR_DATA_MOCK_H_

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
  Insert data to the APEI BERT GENERIC_ERROR_STATUS structure.

  @param[in] BertTable  BERT table instance
  @param[in] DataAddr   The addess of data
  @param[in] DataSize   data size

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InsertGenericErrorData (
  IN  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertTable,
  IN  UINT64                                       DataAddr,
  IN  UINT32                                       DataSize
  );

/**
  Check whether the size is enough to insert record.

  @param[in] BertTable  BERT table instance
  @param[in] DataSize   data size

  @retval EFI_SUCCESS            Size check pass.
  @retval EFI_INVALID_PARAMETER  Parameters is not valid.
  @retval EFI_OUT_OF_RESOURCES   Memory is not enough to insert new record.

**/
EFI_STATUS
CheckBootErrRegionSize (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertTable,
  IN UINT32                                       DataSize
  );

/**
  Unit test for CheckBootErrRegionSize success.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCheckBootErrRegionSize (
  IN UNIT_TEST_CONTEXT  Context
  );

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
  );

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
  );
#endif /*_INSERT_GENERIC_ERROR_DATA_MOCK_H_*/
