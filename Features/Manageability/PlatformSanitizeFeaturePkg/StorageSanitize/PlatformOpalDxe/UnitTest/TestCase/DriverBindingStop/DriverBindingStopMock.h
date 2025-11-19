/**@file
    Unit test case mock header for PlatformOpalEfiDriverBindingStop.

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
#ifndef _DRIVER_BINDING_STOP_MOCK_H_
#define _DRIVER_BINDING_STOP_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include "PlatformOpalDxe.h"

/**
  Stop this driver on Controller.

  @param[in] This              Protocol instance pointer.
  @param[in] Controller        Handle of device to stop driver on
  @param[in] NumberOfChildren  Number of Handles in ChildHandleBuffer. If number of
                               children is zero stop the entire bus driver.
  @param[in] ChildHandleBuffer List of Child Handles to Stop.

  @retval EFI_SUCCESS  This driver is removed Controller.
  @retval other        This driver could not be removed from this device.

**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN UINTN                         NumberOfChildren,
  IN EFI_HANDLE*                   ChildHandleBuffer
  );

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
  );
#endif /*_DRIVER_BINDING_STOP_MOCK_H_*/
