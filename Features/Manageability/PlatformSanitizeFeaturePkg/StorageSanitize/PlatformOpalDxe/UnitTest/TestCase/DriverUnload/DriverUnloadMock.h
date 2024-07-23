/**@file
    Unit test case mock header for PlatformOpalEfiDriverUnload.

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
#ifndef _DRIVER_UNLOAD_MOCK_H_
#define _DRIVER_UNLOAD_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include "PlatformOpalDxe.h"

/**
  Unloads UEFI Driver.  Very useful for debugging and testing.

  @param[in] ImageHandle  Image Handle this driver.

  @retval EFI_SUCCESS            This function always complete successfully.
  @retval EFI_INVALID_PARAMETER  The input ImageHandle is not valid.
**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverUnload (
  IN EFI_HANDLE  ImageHandle
  );

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
  );
#endif /*_DRIVER_UNLOAD_MOCK_H_*/
