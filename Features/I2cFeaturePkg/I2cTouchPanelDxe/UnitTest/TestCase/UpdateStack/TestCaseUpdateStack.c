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
#include "TestCaseUpdateStack.h"
#include <I2cTouchPanel.h>
/*
  Stack is pointer to a not-yet-complete InputReport dictionary.
  This function consumes Token retrieved from InputReportDescriptor stream
  and uses it to update the dictionary.
  Once a dictionary is completed, it calls ExportReport() which puts the
  dictionary in a table of dictionaries. Then it clears internal data
  and prepares to build a new one.
  This implementation of Descriptor parser ignores all types of data except for
  information on how to decode Button presses and X/Y touch coordinates, as this
  is the only info relevant for touchpanels.

  @param Stack                 A pointer to the PARSER_STACK
  @param Token                 Current Token
  @param ReportTable           Pointer to the final report table that contains all the Reports
*/
extern
VOID
UpdateStack (
  PARSER_STACK* Stack,
  TOKEN Token,
  INPUT_REPORT_TABLE* ReportTable
  );
/**
  Mock of UpdateStack

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
UnitTestUpdateStack (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PARSER_STACK           *Stack;
  TOKEN                  Token;
  INPUT_REPORT_TABLE     *ReportTable;

  ZeroMem (&Token, sizeof (TOKEN));
  Stack = AllocateZeroPool (sizeof (PARSER_STACK));
  ReportTable = AllocateZeroPool (sizeof (INPUT_REPORT_TABLE));

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test function UpdateStack will be executed 8 times as follows:
  //
  //        Test1: Mock Token.ID : USE_PAGE
  //        Expect Result: Stack->GlobalUsage = Token.Value.
  //
  Token.ID = USE_PAGE;
  Token.Value = 1;
  UpdateStack (Stack, Token, ReportTable);
  UT_ASSERT_EQUAL (Stack->GlobalUsage, Token.Value);
  //
  //        Test2: Mock Token.ID : USAGE ,Stack->GlobalUsage = DESKTOP and Token.Value = Y_AXIS
  //        Expect Result: Stack->UsageY = Stack->Usages.
  //
  Token.ID = USAGE;
  Stack->GlobalUsage = DESKTOP;
  Token.Value = Y_AXIS;
  UpdateStack (Stack, Token, ReportTable);
  UT_ASSERT_EQUAL (Stack->UsageY, Stack->Usages);
  //
  //        Test3: Mock Token.ID : LOG_MIN
  //        Expect Result: Stack->LogMin = Token.Value.
  //
  Token.ID = LOG_MIN;
  Token.Value = Y_AXIS;
  UpdateStack (Stack, Token, ReportTable);
  UT_ASSERT_EQUAL (Stack->LogMin, Token.Value);
  //
  //        Test4: Mock Token.ID : LOG_MAX
  //        Expect Result: Stack->LogMax = Token.Value.
  //
  Token.ID = LOG_MAX;
  Token.Value = Y_AXIS;
  UpdateStack (Stack, Token, ReportTable);
  UT_ASSERT_EQUAL (Stack->LogMax, Token.Value);
  //
  //        Test5: Mock Token.ID : REP_SIZE
  //        Expect Result: Stack->ReportSize = Token.Value.
  //
  Token.ID = REP_SIZE;
  Token.Value = Y_AXIS;
  UpdateStack (Stack, Token, ReportTable);
  UT_ASSERT_EQUAL (Stack->ReportSize, Token.Value);
  //
  //        Test6: Mock Token.ID : REP_COUNT
  //        Expect Result: Stack->ReportCount = Token.Value.
  //
  Token.ID = REP_COUNT;
  Token.Value = Y_AXIS;
  UpdateStack (Stack, Token, ReportTable);
  UT_ASSERT_EQUAL (Stack->ReportCount, Token.Value);
  //
  //        Test7: Mock Token.ID : REPORT_ID
  //        Expect Result: Stack->Usages. Stack->UsageB. Stack->UsageX. Stack->UsageY. Stack->LogMin.Stack->TouchPanelUsage
  //                       Stack->LogMax. Stack->ReportSize.Stack->ReportCount are equal to 0 and
  //                       Stack->TempReport.Id is equal to Token.Value.
  //
  Token.ID = REPORT_ID;
  UpdateStack (Stack, Token, ReportTable);
  UT_ASSERT_EQUAL (Stack->Usages, 0);
  UT_ASSERT_EQUAL (Stack->UsageB, 0);
  UT_ASSERT_EQUAL (Stack->UsageX, 0);
  UT_ASSERT_EQUAL (Stack->UsageY, 0);
  UT_ASSERT_EQUAL (Stack->LogMin, 0);
  UT_ASSERT_EQUAL (Stack->LogMax, 0);
  UT_ASSERT_EQUAL (Stack->ReportSize, 0);
  UT_ASSERT_EQUAL (Stack->ReportCount, 0);
  UT_ASSERT_EQUAL (Stack->TouchPanelUsage, 0);
  UT_ASSERT_EQUAL (Stack->TempReport.Id, Token.Value);
  //
  //        Test8: Mock Token.ID : END_COLLECTION
  //        Expect Result: Stack->Usages. Stack->UsageB. Stack->UsageX. Stack->UsageY. Stack->LogMin.
  //                       Stack->LogMax. Stack->ReportSize.Stack->ReportCount are equal to 0.
  Token.ID = END_COLLECTION;
  UpdateStack (Stack, Token, ReportTable);
  UT_ASSERT_EQUAL (Stack->Usages, 0);
  UT_ASSERT_EQUAL (Stack->UsageB, 0);
  UT_ASSERT_EQUAL (Stack->UsageX, 0);
  UT_ASSERT_EQUAL (Stack->UsageY, 0);
  UT_ASSERT_EQUAL (Stack->LogMin, 0);
  UT_ASSERT_EQUAL (Stack->LogMax, 0);
  UT_ASSERT_EQUAL (Stack->ReportSize, 0);
  UT_ASSERT_EQUAL (Stack->ReportCount, 0);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return UNIT_TEST_PASSED;
}