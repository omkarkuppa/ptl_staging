/** @file
  Unit test cases for AddSatc API.

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

#ifndef _UT_ADD_SATC_H_
#define _UT_ADD_SATC_H_

#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Unit-Test Function for AddSatc

  @param[in]  Context                            An optional parameter for UnitTest.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddSatc (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // _UT_ADD_SATC_H_
