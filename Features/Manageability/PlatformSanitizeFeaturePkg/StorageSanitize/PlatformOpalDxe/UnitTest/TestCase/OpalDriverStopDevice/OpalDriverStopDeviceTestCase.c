/**@file
    Unit test cases of OpalDriverStopDevice.

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

#include "OpalDriverStopDeviceMock.h"

extern PLATFORM_OPAL_DRIVER mPlatformOpalDriver;

EFI_BOOT_SERVICES* gBS;


/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
OpalDriverStopDevice_CloseProtocolMock (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->CloseProtocol = MockBootServiceCloseProtocol;
  //
  // Mock the CloseProtocol for gEfiStorageSecurityCommandProtocolGuid
  //
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);
  //
  // Mock the CloseProtocol for gEfiBlockIoProtocolGuid
  //
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test OpalDriverStopDevice CleanUp Function.
  Clean the parameter for CloseProtocol mock

  @param[in]  Context    Input context for mock.

**/
VOID
OpalDriverStopDevice_CloseProtocolCleanUp (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  FreePool (gBS);
}
/**
  Unit test for OpalDriverStopDevice.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
OpalDriverStopDeviceUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PLATFORM_OPAL_DRIVER_DEVICE *Dev;
  PLATFORM_OPAL_DRIVER_DEVICE *Dev2;

  //
  // Set the precondition
  //
  Dev = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Dev != NULL) {
    mPlatformOpalDriver.DeviceList = Dev;
  }
  Dev2 = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Dev2 != NULL) {
    mPlatformOpalDriver.DeviceList->Next = Dev2;
  }

  if ((Dev != NULL) && (Dev2 != NULL)) {
    OpalDriverStopDevice_CloseProtocolMock (Context);
    OpalDriverStopDevice (Dev);
    UT_ASSERT_EQUAL (mPlatformOpalDriver.DeviceList, Dev2);
    OpalDriverStopDevice_CloseProtocolCleanUp (Context);

    OpalDriverStopDevice_CloseProtocolMock (Context);
    OpalDriverStopDevice (Dev2);
    UT_ASSERT_EQUAL (mPlatformOpalDriver.DeviceList, NULL);
    OpalDriverStopDevice_CloseProtocolCleanUp (Context);
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
