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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "TestCaseHidDescriptorReceivedEvent.h"
#include <Library/DebugLib.h>
#include <I2cTouchPanel.h>

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
  Mock of HidDescriptorReceivedEvent

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
UnitTestHidDescriptorReceivedEvent (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN           *ContextBoolean;
  BOOLEAN            Parameter;
  EFI_EVENT          Event;

  Parameter = TRUE;
  ContextBoolean = &Parameter;
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test function UnitTestHidDescriptorReceivedEvent will be executed 2 times as follows:
  //
  //        Test1: Mock Event or Context are NULL, *Context will not modify to FALSE.
  //        Expect Result: *Context still TRUE.
  HidDescriptorReceivedEvent (NULL, ContextBoolean);
  UT_ASSERT_EQUAL (*ContextBoolean, TRUE);
  //        Test2: Mock Event and Context aren't NULL, *Context will modify to FALSE.
  //        Expect Result: *Context change to FALSE.
  //
  Event = AllocateZeroPool (sizeof (EFI_EVENT));
  HidDescriptorReceivedEvent (Event, ContextBoolean);
  UT_ASSERT_EQUAL (*ContextBoolean, FALSE);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return UNIT_TEST_PASSED;
}