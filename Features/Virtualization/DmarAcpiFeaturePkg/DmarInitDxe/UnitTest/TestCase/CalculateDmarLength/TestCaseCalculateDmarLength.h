/** @file
  Unit test cases for CalculateDmarLength API.

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

#ifndef _UT_CALCULATE_DMAR_LENGTH_H_
#define _UT_CALCULATE_DMAR_LENGTH_H_

#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks if functions behaves properly for invalid parameters or other unexpected situations.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthInvalidParameters (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for CalculateDmarLength

  @attention There is an assumption that DMAR table contains only DRHD entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhd (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and RMRR entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndRmrr (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and OEM RMRR entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndOemRmrr (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and SATC entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndSatc (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and SIDP entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndSidp (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and RHSA entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndRhsa (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Function for CalculateDmarLength.

  This unit-test checks the length of DMAR ACPI table which contains only DRHD and ATSR entries.

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCalculateDmarLengthDrhdAndAtsr (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // _UT_CALCULATE_DMAR_LENGTH_H_
