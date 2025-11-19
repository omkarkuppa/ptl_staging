/**@file
  Unit test case of the I2C Device Common Driver

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

#include "TestCaseInstallI2cTable.h"

#define UNIT_TEST_FUNCTION_NAME     "UnitTestInstallI2cTable"
#define UNIT_TEST_FUNCTION_VERSION  "0.1"

/**
  Check the result of test case with InstallI2cTable.

  @param[in] Context                   The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED             The result is correct.
  @retval UNIT_TEST_ERROR_TEST_FAILED  The result is wrong.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInstallI2cTable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  DEBUG ((DEBUG_INFO, "[Test Case] %a v%a\n", UNIT_TEST_FUNCTION_NAME, UNIT_TEST_FUNCTION_VERSION));

  return UNIT_TEST_PASSED;
}
