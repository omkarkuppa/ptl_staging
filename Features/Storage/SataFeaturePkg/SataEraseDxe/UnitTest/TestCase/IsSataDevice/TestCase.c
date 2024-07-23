/**@file
    Unit test cases of IsSataDevice.

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

#include "IsSataDeviceMock.h"

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
  )
{
  BOOLEAN  Status;
  BOOLEAN  *Output;

  PreRequisiteRunMock ();
  Status = IsSataDevice (NULL);

  Output = (BOOLEAN*)PreRequisiteExpectOutput ();
  UT_ASSERT_EQUAL (Status, (*Output));
  return UNIT_TEST_PASSED;
}
