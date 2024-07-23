/** @file
  Unit test cases for AddRmrr API.

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

@par Specification Reference:
**/

#ifndef _UT_ADD_RMRR_H_
#define _UT_ADD_RMRR_H_

#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Unit-Test Function for AddRmrr().

  This unit-test checks if functions behaves properly for invalid parameters or other unexpected situations.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddRmrrInvalidParameters (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for AddRmrr()

  This unit-test checks if AddRmrr() creates DMAR ACPI table as expected (only with RMRR structures).

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddRmrrBasicFlow (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // _UT_ADD_RMRR_H_
