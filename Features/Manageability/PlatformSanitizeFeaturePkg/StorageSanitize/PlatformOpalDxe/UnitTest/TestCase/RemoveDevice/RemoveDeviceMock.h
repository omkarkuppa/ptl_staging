/**@file
    Unit test case mock header for RemoveDevice.

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
#ifndef _REMOVE_DEVICE_MOCK_H_
#define _REMOVE_DEVICE_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include "PlatformOpalDxe.h"

/**
  Remove one device in the global device list.

  @param[in] Dev  The device need to be removed.

**/
VOID
RemoveDevice (
  IN PLATFORM_OPAL_DRIVER_DEVICE *Dev
  );

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
  );
#endif /*_REMOVE_DEVICE_MOCK_H_*/
