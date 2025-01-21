/**@file
    Unit test cases of RemoveDevice.

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

#include "RemoveDeviceMock.h"

extern PLATFORM_OPAL_DRIVER mPlatformOpalDriver;
/**
  Unit test for RemoveDevice.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
RemoveDeviceUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PLATFORM_OPAL_DRIVER_DEVICE Dev  = {0};
  PLATFORM_OPAL_DRIVER_DEVICE Dev2 = {0};

  mPlatformOpalDriver.DeviceList = &Dev;
  RemoveDevice (&Dev);
  UT_ASSERT_EQUAL (mPlatformOpalDriver.DeviceList, NULL);

  mPlatformOpalDriver.DeviceList = &Dev;
  mPlatformOpalDriver.DeviceList->Next = &Dev2;
  RemoveDevice (&Dev);
  UT_ASSERT_EQUAL (mPlatformOpalDriver.DeviceList, &Dev2);

  return UNIT_TEST_PASSED;
}
