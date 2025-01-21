/**@file
    Unit test cases of PlatformOpalEfiDriverBindingSupported.

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

#include "DriverBindingSupportedMock.h"
EFI_BOOT_SERVICES  *gBS;

/**
  Unit test for PlatformOpalEfiDriverBindingSupported.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
DriverBindingSupportedUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  EFI_DRIVER_BINDING_PROTOCOL DriverBinding;

  will_return (LibPcdGetBool, FALSE);
  Status = PlatformOpalEfiDriverBindingSupported (NULL, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->CloseProtocol = MockBootServiceCloseProtocol;
  gBS->OpenProtocol  = MockBootServiceOpenProtocol;

  //
  // Mock the OpenProtocol for gEfiStorageSecurityCommandProtocolGuid
  // gEfiStorageSecurityCommandProtocolGuid already open
  //
  will_return (LibPcdGetBool, TRUE);
  will_return (MockBootServiceOpenProtocol, EFI_ALREADY_STARTED);
  will_return (MockBootServiceOpenProtocol, NULL);
  Status = PlatformOpalEfiDriverBindingSupported (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Mock the OpenProtocol for gEfiStorageSecurityCommandProtocolGuid
  // Not found the gEfiStorageSecurityCommandProtocolGuid
  //
  will_return (LibPcdGetBool, TRUE);
  will_return (MockBootServiceOpenProtocol, EFI_NOT_FOUND);
  will_return (MockBootServiceOpenProtocol, NULL);
  Status = PlatformOpalEfiDriverBindingSupported (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);

  //
  // Mock the OpenProtocol for gEfiStorageSecurityCommandProtocolGuid
  // Open gEfiStorageSecurityCommandProtocolGuid success
  //
  will_return (LibPcdGetBool, TRUE);
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, NULL);
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);
  Status = PlatformOpalEfiDriverBindingSupported (&DriverBinding, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  FreePool (gBS);
  return UNIT_TEST_PASSED;
}
