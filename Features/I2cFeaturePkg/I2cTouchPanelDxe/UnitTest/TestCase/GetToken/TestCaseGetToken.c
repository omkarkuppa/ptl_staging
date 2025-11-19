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
#include "TestCaseGetToken.h"
#include <I2cTouchPanel.h>
/*
  HID's InputReportDescriptor consists of variable length tokens.
  On entry, Descriptor points to InputReportDescriptor's start or a boundary
    between two tokens somewhere inside descriptor. Size means number of bytes
    before end of Descriptor.
  Function returns a single Token.
  On exit, Descriptor points next boundary between tokens, just after the token
    that was returned. Size is decreased by as many bytes as Descriptor pointer
    was moved forward.
  Tokens in Long Item format are only partially supported; they will return
    invalid value but Descriptor pointer and Size will be updated correctly to
    allow further parsing
*/
extern
TOKEN
GetToken (
  IN OUT UINT8** Descriptor,
  IN OUT UINT16* Size
  );

/**
  Mock of GetToken

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
UnitTestGetToken (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT8                *pDescriptor;
  UINT8                Descriptor;
  UINT16               Size;
  TOKEN                Token;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
    // Test function GetToken will be executed 2 times as follows:
    //
    //        Test1: Mock Descriptor equal to 0xFE.
    //        Expect Result: return Token successful.
    //
    Descriptor = 0xFE;
    pDescriptor = &Descriptor;
    Token = GetToken (&pDescriptor, &Size);
    //
    //        Test1: Mock Descriptor not equal to 0xFE.
    //        Expect Result: return Token successful.
    //
    Descriptor = 0;
    Token = GetToken (&pDescriptor, &Size);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return UNIT_TEST_PASSED;
}