/**@file
    Unit test cases of PlatformOpalEfiDriverUnload.

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

#include "DriverUnloadMock.h"
#include "OpalDriverStopDeviceMock.h"

extern PLATFORM_OPAL_DRIVER mPlatformOpalDriver;
EFI_HANDLE  gImageHandle = NULL;
/**
  Unit test for PlatformOpalEfiDriverUnload.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
DriverUnloadUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  PLATFORM_OPAL_DRIVER_DEVICE *Dev;
  EFI_HANDLE                  Handle;

  Handle = AllocatePool (sizeof (EFI_HANDLE));
  if (Handle != NULL) {
    Status = PlatformOpalEfiDriverUnload (Handle);
    UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
    FreePool (Handle);
  }

  ZeroMem (&mPlatformOpalDriver, sizeof (mPlatformOpalDriver));
  Status = PlatformOpalEfiDriverUnload (gImageHandle);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Set the precondition
  //
  Dev = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Dev != NULL) {
    mPlatformOpalDriver.DeviceList = Dev;
    OpalDriverStopDevice_CloseProtocolMock (Context);
    Status = PlatformOpalEfiDriverUnload (gImageHandle);
    UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
    OpalDriverStopDevice_CloseProtocolCleanUp (Context);
  }
  return UNIT_TEST_PASSED;
}
