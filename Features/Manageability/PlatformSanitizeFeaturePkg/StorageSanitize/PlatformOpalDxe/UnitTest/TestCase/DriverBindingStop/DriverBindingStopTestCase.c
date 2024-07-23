/**@file
    Unit test cases of PlatformOpalEfiDriverBindingStop.

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

#include "DriverBindingStopMock.h"
#include "OpalDriverStopDeviceMock.h"

extern PLATFORM_OPAL_DRIVER mPlatformOpalDriver;

/**
  Unit test for PlatformOpalEfiDriverBindingStop.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
DriverBindingStopUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  PLATFORM_OPAL_DRIVER_DEVICE *Dev;
  PLATFORM_OPAL_DRIVER_DEVICE *Dev2;
  EFI_HANDLE                  Controller;

  ZeroMem (&mPlatformOpalDriver, sizeof (mPlatformOpalDriver));
  Status = PlatformOpalEfiDriverBindingStop (NULL, NULL, 0, NULL);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
  //
  // Set the precondition
  //
  Controller = AllocatePool (sizeof (EFI_HANDLE));
  Dev = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Dev != NULL) {
    Dev->Handle = Controller;
    mPlatformOpalDriver.DeviceList = Dev;
  }
  Dev2 = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Dev2 != NULL) {
    Dev2->Handle = Controller;
    mPlatformOpalDriver.DeviceList->Next = Dev2;
  }

  if ((Dev != NULL) && (Dev2 != NULL)) {
    Status = PlatformOpalEfiDriverBindingStop (NULL, NULL, 0, NULL);
    UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);

    if (Controller != NULL) {
      OpalDriverStopDevice_CloseProtocolMock (Context);
      Status = PlatformOpalEfiDriverBindingStop (NULL, Controller, 0, NULL);
      UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
      OpalDriverStopDevice_CloseProtocolCleanUp (Context);
    }
  } else {
    if (Dev != NULL) {
      FreePool (Dev);
    }
    if (Dev2 != NULL) {
      FreePool (Dev2);
    }
  }
  return UNIT_TEST_PASSED;
}
