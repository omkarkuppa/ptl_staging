/**@file
    Unit test case mock header for PlatformOpalEfiDriverBindingSupported.

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
#ifndef _DRIVER_BINDING_SUPPORTED_MOCK_H_
#define _DRIVER_BINDING_SUPPORTED_MOCK_H_

#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/MockBootServices.h>
#include "PlatformOpalDxe.h"

/**
  Tests to see if this driver supports a given controller.

  This function checks to see if the controller contains an instance of the
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL and the EFI_BLOCK_IO_PROTOCOL
  and returns EFI_SUCCESS if it does.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The Handle of the controller to test. This Handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath   This parameter is ignored.

  @retval EFI_SUCCESS               The device contains required protocols
  @retval EFI_ALREADY_STARTED       The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by the driver
                                    specified by This.
  @retval EFI_ACCESS_DENIED         The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by a different
                                    driver or an application that requires exclusive access.
                                    Currently not implemented.
  @retval EFI_UNSUPPORTED           The device does not contain requires protocols

**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL* This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*    RemainingDevicePath
  );

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
  );
#endif /*_DRIVER_BINDING_SUPPORTED_MOCK_H_*/
