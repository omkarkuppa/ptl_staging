/** @file
  Unit test cases for AddAtsr API.

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

#ifndef _UT_ADD_ATSR_H_
#define _UT_ADD_ATSR_H_

#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Unit-Test Function for AddAtsr.

  This unit test checks whether AddAtsr() behaves properly in case of invalid parameters.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddAtsrInvalidParameters (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for AddAtsr.

  This unit test checks whether AddAtsr() behaves properly when ATSR is not supported (eg. disabled by PCD).

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddAtsrUnsupported (
  IN UNIT_TEST_CONTEXT  Context
  );


/**
  Unit-Test Function for AddAtsr()

  This unit test checks if AddAtsr() creates DMAR ACPI table as expected (only with ATSR structures).

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddAtsrBasicFlow (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // _UT_ADD_ATSR_H_
