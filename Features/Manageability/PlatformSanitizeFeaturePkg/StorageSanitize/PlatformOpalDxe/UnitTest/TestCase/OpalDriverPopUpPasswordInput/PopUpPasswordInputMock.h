/**@file
    Unit test case mock header for OpalDriverPopUpPasswordInput.

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
#ifndef _POPUP_PASSWORD_INPUT_MOCK_H_
#define _POPUP_PASSWORD_INPUT_MOCK_H_

#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include "PlatformOpalDxe.h"

/**
  Get password input from the popup window.

  @param[in]  PopUpString1  Pop up string 1.
  @param[out] PressEsc      Whether user escape function through Press ESC.

  @retval Password string if success. NULL if failed.

**/
CHAR8 *
OpalDriverPopUpPasswordInput (
  IN CHAR16                 *PopUpString1,
  OUT BOOLEAN               *PressEsc
  );

/**
  Unit test for OpalDriverPopUpPasswordInput.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
PopUpPasswordInputUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_POPUP_PASSWORD_INPUT_MOCK_H_*/
