/**@file
    Unit test case of the I2c TouchPanel Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _TEST_CASE_TOUCH_DEV_INIT_H_
#define _TEST_CASE_TOUCH_DEV_INIT_H_

#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Event used for first read HID Descriptor transaction from Touch Device

  @param[in]  Event    Event called by I2C HOST on the I2C request completion
  @param[in]  Context  Context passed by the event creator
**/
extern
VOID
EFIAPI
HidDescriptorReceivedEvent (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  );
/**
  Mock of TouchDevInit

  @param[in]  Context                   [Optional] An optional parameter that enables:
                                        1) test-case reuse with varied parameters and
                                        2) test-case re-entry for Target tests that need a
                                        reboot.  This parameter is a VOID* and it is the
                                        responsibility of the test author to ensure that the
                                        contents are well understood by all test cases that may
                                        consume it.
  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestTouchDevInit (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif
