/**@file
    Unit test case mock header for GetOpalState.

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
#ifndef _GET_OPAL_STATE_MOCK_H_
#define _GET_OPAL_STATE_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include "PlatformOpalDxe.h"

/**
  Provide interface to get the lock state on certain device

  @param[in]  This      Pointer of PLATFORM_OPAL_PROTOCOL.
  @param[in]  Handle    SSCP handle of the selected device.
  @param[out] IsEnabled TRUE/FALSE, TRUE: opal feature is enabled.
  @param[out] IsLocked  TRUE/FALSE, TRUE: device is locked.

  @retval EFI_SUCCESS  support Lock feature and return state.
  @retval Others       Failed due to some error.
**/
EFI_STATUS
EFIAPI
GetOpalState (
  IN  PLATFORM_OPAL_PROTOCOL *This,
  IN  EFI_HANDLE             *Handle,
  OUT BOOLEAN                *IsEnabled OPTIONAL,
  OUT BOOLEAN                *IsLocked  OPTIONAL
  );

/**
  Unit test for GetOpalState.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
GetOpalStateUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_GET_OPAL_STATE_MOCK_H_*/
