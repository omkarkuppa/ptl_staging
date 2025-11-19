/**@file
    Unit test case mock header for OpalDriverStopDevice.

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
#ifndef _OPAL_STOP_DEVICE_MOCK_H_
#define _OPAL_STOP_DEVICE_MOCK_H_

#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/MockBootServices.h>
#include "PlatformOpalDxe.h"

/**
  Unit-Test Prerequisite Function pointer.

  Do not found the EFI_ATA_PASS_THRU_PROTOCOL when LocateHandleBuffer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
OpalDriverStopDevice_CloseProtocolMock (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test OpalDriverStopDevice CleanUp Function.
  Clean the parameter for CloseProtocol mock

  @param[in]  Context    Input context for mock.

**/
VOID
OpalDriverStopDevice_CloseProtocolCleanUp (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Stop this Controller.

  @param[in] Dev  The device need to be stopped.

**/
VOID
OpalDriverStopDevice (
  IN PLATFORM_OPAL_DRIVER_DEVICE  *Dev
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
OpalDriverStopDeviceUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_OPAL_STOP_DEVICE_MOCK_H_*/
