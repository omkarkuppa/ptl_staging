/**@file
    Unit test case mock header for IsSataDevice.

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
#ifndef _IS_SATA_DEVICE_MOCK_H_
#define _IS_SATA_DEVICE_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "PreRequisite.h"

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreIsSataDevice_True (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreIsSataDevice_False (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test IsSataDevice CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
EFIAPI
CleanUpIsSataDevice (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Check if EFI_ATA_PASS_THRU_PROTOCOL exist on the parent handle.

  @param  ChildHandle  Child Handle to retrieve Parent information.

  @retval  TRUE    Should produce Block Io & Block Io2.
  @retval  FALSE   Should not produce Block Io & Block Io2.

**/
BOOLEAN
IsSataDevice (
  IN  EFI_HANDLE  ChildHandle
  );

/**
  Unit test for IsSataDevice.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestIsSataDevice (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_IS_SATA_DEVICE_MOCK_H_*/
