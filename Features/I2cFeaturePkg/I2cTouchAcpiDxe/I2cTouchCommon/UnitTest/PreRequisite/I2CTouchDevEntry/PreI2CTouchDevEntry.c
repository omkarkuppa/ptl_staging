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

#include "PreI2CTouchDevEntry.h"

#define PRE_FUNCTION_NAME     "PreI2CTouchDevEntry"
#define PRE_FUNCTION_VERSION  "0.1"

extern TABLE_POINTER_ADDR_PROTOCOL  mTablePointer;

/**
  Set up services for I2CTouchDevEntry function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupI2CTouchDevEntryEnvironment (
  VOID
  )
{
  EFI_HANDLE  Handle = NULL;

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  UT_ASSERT_NOT_NULL (gBS);

  //
  // Mock boot service InstallProtocolInterface
  //
  gBS->InstallProtocolInterface = MockBootServiceInstallProtocolInterface;

  will_return (MockBootServiceInstallProtocolInterface, Handle);
  expect_value (MockBootServiceInstallProtocolInterface, Protocol, &gI2cDeviceProtocolGuid);
  expect_value (MockBootServiceInstallProtocolInterface, InterfaceType, EFI_NATIVE_INTERFACE);
  will_return (MockBootServiceInstallProtocolInterface, &mTablePointer);
  will_return (MockBootServiceInstallProtocolInterface, EFI_SUCCESS);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute I2CTouchDevEntry function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreI2CTouchDevEntry (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                   Status;

  DEBUG ((DEBUG_INFO, "[PreRequisite] %a v%a\n", PRE_FUNCTION_NAME, PRE_FUNCTION_VERSION));

  Status = SetupI2CTouchDevEntryEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Execute test function with above mock environment
  //
  I2CTouchDevEntry (NULL, NULL);

  return UNIT_TEST_PASSED;
}

