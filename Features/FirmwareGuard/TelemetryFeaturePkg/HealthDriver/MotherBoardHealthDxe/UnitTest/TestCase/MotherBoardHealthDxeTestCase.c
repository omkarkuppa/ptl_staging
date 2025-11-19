/**@file
    Unit test case of the MotherBoard Health Dxe driver.
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

#include "MotherBoardHealthDxeTestCase.h"

UNIT_TEST_STATUS
EFIAPI
UnitTestHelloWorld (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  DEBUG ((DEBUG_INFO, "Unit Test Hello World\n"));
  return UNIT_TEST_PASSED;
}

UNIT_TEST_STATUS
EFIAPI
UnitTestOther (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  DEBUG ((DEBUG_INFO, "Unit Test Other\n"));
  return UNIT_TEST_PASSED;
}