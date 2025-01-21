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
#include "TestCaseParseInput.h"
#include <I2cTouchPanel.h>
/*
  This function uses dictionaries to parse incoming InputReport and convert it into X/Y coordinates plus Button info.

*/
extern
EFI_STATUS
ParseInput (
  INPUT_REPORT_TABLE ReportTable,
  UINT8              HidSolution,
  UINT8              *InputStream,
  TOUCH_OUTPUT       *Output,
  TOUCH_XY_BOUNDARY  *MinMax
  );

/**
  Mock of ParseInput

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
UnitTestParseInput (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  INPUT_REPORT_TABLE        ReportTable;
  UINT8                     HidSolution; 
  TOUCH_OUTPUT              *Output;
  TOUCH_XY_BOUNDARY         *MinMax;
  UINT8                     *InputStream;
  EFI_STATUS                Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  // Initial parameters.
  ZeroMem (&ReportTable, sizeof (INPUT_REPORT_TABLE));
  ReportTable.Report = AllocateZeroPool (sizeof (INPUT_REPORT_FORMAT));
  Output = AllocateZeroPool (sizeof (TOUCH_OUTPUT));
  MinMax = AllocateZeroPool (sizeof (TOUCH_XY_BOUNDARY));
  InputStream = AllocateZeroPool (sizeof (UINT8) * 3);
  HidSolution=2;
  //
  // Test function ParseInput will be executed 2 times as follows:
  //
  //        Test1: Mock ReportTable.Quantity is equal to zero.
  //        Expect Result: EFI_NOT_FOUND
  //
  Status = ParseInput (ReportTable, HidSolution, NULL, Output, MinMax);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
  //
  //        Test2: Mock all functions work success.
  //        Expect Result: EFI_SUCCESS
  //
  ReportTable.Quantity = 1;
  Status = ParseInput (ReportTable,HidSolution, InputStream, Output, MinMax);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  FreePool (InputStream);
  FreePool (Output);
  FreePool (MinMax);
  return UNIT_TEST_PASSED;
}