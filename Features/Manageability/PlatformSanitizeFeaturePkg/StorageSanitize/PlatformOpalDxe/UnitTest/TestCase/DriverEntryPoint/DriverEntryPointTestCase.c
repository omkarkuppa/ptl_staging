/**@file
    Unit test cases of PlatformOpalEfiDriverEntryPoint.

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

@par Specification
**/

#include "DriverEntryPointMock.h"
EFI_BOOT_SERVICES  *gBS;

/**
  Unit test for PlatformOpalEfiDriverEntryPoint.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
DriverEntryPointUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;

  will_return (EfiLibInstallDriverBindingComponentName2, EFI_NOT_FOUND);
  Status = PlatformOpalEfiDriverEntryPoint (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);

  gBS = AllocateZeroPool(sizeof(EFI_BOOT_SERVICES));
  gBS->InstallProtocolInterface = MockBootServiceInstallProtocol;

  //
  // Mock the InstallProtocol for gPlatformOpalProtocolGuid
  //
  will_return (MockBootServiceInstallProtocol, EFI_SUCCESS);
  will_return (EfiLibInstallDriverBindingComponentName2, EFI_SUCCESS);
  Status = PlatformOpalEfiDriverEntryPoint (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  FreePool (gBS);
  return UNIT_TEST_PASSED;
}
