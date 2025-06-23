/** @file
  Header file for Fusa Result Reporting Lib functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/
#ifndef _FUSA_RESULT_REPORTING_LIB_H_
#define _FUSA_RESULT_REPORTING_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/PeiFusaE2eCtcLib.h>

/**
  Function pointer for the generic FUSA test
  @note Expect the pointed function to use the UpdateResults()
  to update the test result.
**/
typedef FUSA_LIB_STATUS (* const FUSA_TEST_FUNC)( FUSA_TEST_RESULT *pFusaTestResult );

/**
  Updates the reporting structure with check result.
  Will need to be called after every check is done.

  @note It is expected that for every test, InitializeResults()
        routine has called once before the first call to this
        routine.

  @param[in] TestCheckVal - result of executing a check
  @param[in] CheckNum - check number for subtest
  @param[in,out] pFusaTestResult - summarized reporting
  structure for the whole test

  @retval FusaNoError - if succeed
  @retval FusaInvalidParam - if pFusaTestResult is NULL
  @retval FusalInternalError - if CheckNum >=
          pFusaTestResult->TotalChecks or
          pFusaTestResult->CheckResults[CheckNum] has been used
          before or not been initialized
**/
FUSA_LIB_STATUS
UpdateResults(
  IN UINT8 TestCheckVal,
  IN UINT8 CheckNum,
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Dump Fusa test result for debug purpose. Does nothing in
  release build.

  @param[in] pFusaTestResult - pointer to the result buffer to
        be dumped
**/
VOID
DumpResults(
  IN FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  The FusaTestGeneric wraps the calling to stlTest with the
  generic Fusa test result reporting flow.

  @param[in] FusaTest - function pointer to the test
  @param[in] TestNum - test number assigned to the test
  @param[in] TotalNumberOfChecks - total number of checks for
        the test
  @param[in,out] pFusaTestResult - summarized reporting
    structure for the whole test

  @retval FusaNoError - if succeed
  @retval FusaInvalidParam - if pFusaTestResult is NULL
  @retval FusaInternalError - if error caused by internal
          implementation or defensive programming path taken

**/
FUSA_LIB_STATUS
FusaTestAndReporting (
  IN FUSA_TEST_FUNC fusaTest,
  IN FUSA_TEST_NUMBER testNum,
  IN UINT32 totalNumberOfChecks,
  IN FUSA_TEST_RESULT *pFusaTestResult
  );


#endif // _FUSA_RESULT_REPORTING_LIB_H_
