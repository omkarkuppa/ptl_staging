/**@file
    Unit test case mock header for InitializePrivateData.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _INITIALIZE_PRIVATE_DATA_MOCK_H_
#define _INITIALIZE_PRIVATE_DATA_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AcpiTable.h>
#include <setjmp.h>
#include <cmocka.h>

/**
  Locate AcpiTbl Protocol.

  @retval  AcpiTblProtocol  Pointer of EFI_ACPI_TABLE_PROTOCOL instance.

**/
EFI_ACPI_TABLE_PROTOCOL *
LocateAcpiProtocol (
  VOID
  );

/**
  Unit test for InitializePrivateData.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestLocateAcpiProtocol (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Initialize private data structure and allocate memory for HobDataPtr.

  @retval  EFI_SUCCESS           mCrashlogDxe initialized successfully.
           EFI_OUT_OF_RESOURCES  Could not allocate needed resources.
           EFI_NOT_FOUND         Could not find AcpiProtocol.

**/
EFI_STATUS
InitializePrivateData (
  VOID
  );

/**
  Unit test for InitializePrivateData.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitializePrivateData (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_INITIALIZE_PRIVATE_DATA_MOCK_H_*/
