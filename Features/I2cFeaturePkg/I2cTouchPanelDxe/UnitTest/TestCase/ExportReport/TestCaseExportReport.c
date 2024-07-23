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
#include "TestCaseExportReport.h"
#include <I2cTouchPanel.h>

/*
  Stack contains a set of data retrieved from parsing InputReportDescriptor.
  This function checks if that set of data constitutes a valid InputData
  dictionary, and if so then puts it into ReportTable, table of dictionaries.
  Unless this is the 1st dictionary for a particular device, this means
  allocating new bigger table and deallocating old table

  @param Stack                 A pointer to the PARSER_STACK
  @param ReportTable           Pointer to the final report table that contains all the Reports

*/
extern
VOID
ExportReport (
  PARSER_STACK* Stack,
  INPUT_REPORT_TABLE* ReportTable
  );
/**
  Mock of ExportReport

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
UnitTestExportReport (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  INPUT_REPORT_TABLE     *ReportTable;
  PARSER_STACK           Stack;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test function ExportReport will be executed 4 times as follows:
  //
  //        Test1: Mock Stack->TouchPanelUsage not equal to zero.
  //        Expect Result: ReportTable->TouchPanel = 1
  //

  // Inital parameters.
  ReportTable = AllocateZeroPool (sizeof (INPUT_REPORT_TABLE));
  ZeroMem (&Stack, sizeof (PARSER_STACK));
  Stack.TouchPanelUsage = 1;

  ExportReport (&Stack, ReportTable);
  UT_ASSERT_EQUAL (ReportTable->TouchPanel, 1);
  //
  //        Test2: Mock Stack->TouchPanelUsage equal to zero.
  //
  Stack.TouchPanelUsage = 0;
  ExportReport (&Stack, ReportTable);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return UNIT_TEST_PASSED;
}