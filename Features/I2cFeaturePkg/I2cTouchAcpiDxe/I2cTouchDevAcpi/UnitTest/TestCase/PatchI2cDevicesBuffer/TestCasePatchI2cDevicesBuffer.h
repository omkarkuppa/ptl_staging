/**@file
  Unit test case of the I2c Touch Device Acpi Driver

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

#ifndef _TEST_CASE_PATCH_I2C_DEVICES_BUFFER_H_
#define _TEST_CASE_PATCH_I2C_DEVICES_BUFFER_H_

#include <PrivateMock.h>

/**
  Check the result of test case with PatchI2cDevicesBuffer.

  @param[in] Context                   The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED             The result is correct.
  @retval UNIT_TEST_ERROR_TEST_FAILED  The result is wrong.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPatchI2cDevicesBuffer (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif  // _TEST_CASE_PATCH_I2C_DEVICES_BUFFER_H_
