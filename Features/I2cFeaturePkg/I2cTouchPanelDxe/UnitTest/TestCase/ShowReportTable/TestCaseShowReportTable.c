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
#include "TestCaseShowReportTable.h"
#include <I2cTouchPanel.h>

extern
VOID
ShowReportTable (
  INPUT_REPORT_TABLE ReportTable
  );
/**
  Mock of ShowReportTable

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
UnitTestShowReportTable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  INPUT_REPORT_TABLE       ReportTable;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  // Inital INPUT_REPORT_TABLE structure.
  ZeroMem (&ReportTable, sizeof (INPUT_REPORT_TABLE));
  ReportTable.Quantity = 1;
  ReportTable.Report = AllocateZeroPool (sizeof (INPUT_REPORT_FORMAT));
  ReportTable.Report[0].Id = 1;
  ReportTable.Report[0].CollectionCount = 1;
  ReportTable.Report[0].Collection[0].LogMaxX = 1;
  ReportTable.Report[0].Collection[0].LogMinX = 1;
  ReportTable.Report[0].Collection[0].LogMaxY = 1;
  ReportTable.Report[0].Collection[0].LogMinY = 1;
  ReportTable.Report[0].Collection[0].BitStartB = 1;
  ReportTable.Report[0].Collection[0].BitStopB = 1;
  ReportTable.Report[0].Collection[0].BitStartX = 1;
  ReportTable.Report[0].Collection[0].BitStopX = 1;
  ReportTable.Report[0].Collection[0].BitStartY = 1;
  ReportTable.Report[0].Collection[0].BitStopY = 1;
  // Test function ShowReportTable will be executed 1 times as follows:
  //
  //        Test1: Mock ReportTable structure.
  //        Expect Result: Print ReportTable data.
  //
  ShowReportTable (ReportTable);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return UNIT_TEST_PASSED;
}