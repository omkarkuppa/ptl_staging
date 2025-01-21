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

#ifndef _PRE_I2C_DEV_PATCH_ENTRY_H_
#define _PRE_I2C_DEV_PATCH_ENTRY_H_

#include <BootServicesMock.h>
#include <PrivateMock.h>

EFI_STATUS
EFIAPI
I2cDevPatchEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

/**
  Set up services for I2cDevPatchEntry function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupI2cDevPatchEntryEnvironment (
  VOID
  );

/**
  Set up the environment and execute I2cDevPatchEntry function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreI2cDevPatchEntry (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif  // _PRE_I2C_DEV_PATCH_ENTRY_H_
