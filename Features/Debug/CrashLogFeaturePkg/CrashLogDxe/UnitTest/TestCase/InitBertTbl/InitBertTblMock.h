/**@file
    Unit test case mock header for InitBertTbl.

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
#ifndef _INIT_BERT_TABLE_MOCK_H_
#define _INIT_BERT_TABLE_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/AcpiTable.h>
#include <IndustryStandard/Acpi.h>
#include <Guid/CrashLogHob.h>

/**
  Initialize the APEI BERT table

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InitBertTbl (
  VOID
  );

/**
  Unit test for InitBertTbl.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitBertTbl (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_INIT_BERT_TABLE_MOCK_H_*/
