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

#ifndef _CLEAN_UP_INSTALL_I2C_TABLE_H_
#define _CLEAN_UP_INSTALL_I2C_TABLE_H_

#include <PrivateMock.h>

/**
  Clean up all variables used in testing InstallI2cTable function.

  @retval EFI_SUCCESS  Clean up success.

**/
EFI_STATUS
EFIAPI
CleanInstallI2cTableEnvironment (
  VOID
  );

/**
  Clean up the environment for next test case.

  @param[in] Context                      The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                Clean up success.
  @retval UNIT_TEST_ERROR_CLEANUP_FAILED  Fail to clean up.

**/
UNIT_TEST_STATUS
EFIAPI
CleanUpInstallI2cTable (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif  // _CLEAN_UP_INSTALL_I2C_TABLE_H_
