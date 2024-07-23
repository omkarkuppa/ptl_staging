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

#ifndef _PRE_INSTALL_I2C_TABLE_H_
#define _PRE_INSTALL_I2C_TABLE_H_

#include <PrivateMock.h>
#include <Uefi/UefiBaseType.h>

/**
  Call Back function to Install Table at EndofDxe.
  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
VOID
EFIAPI
InstallI2cTable (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

/**
  Set up services for InstallI2cTable function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupInstallI2cTableEnvironment (
  VOID
  );

/**
  Set up the environment and execute InstallI2cTable function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreInstallI2cTable (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif  // _PRE_INSTALL_I2C_TABLE_H_
