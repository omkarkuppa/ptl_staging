/** @file
  This file contains internal Fusa test result reporting
  routines for Fusa lib usage

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include "PeiFusaResultReportingLib.h"

/**
  Generates crc32 for reporting structure except for the last 4
  bytes. It uses IA instruction crc32 for the calculation which
  implies its polynomial is 0x11EDC6F41. It uses 0xffffffff as
  initial value. The final CRC32 value is not negated.

  @note it expects FUSA_TEST_RESULT structure size to be
        multiple of 4

  @param[in, out] pFusaTestResult - summarized reporting
  structure for the whole test

  @retval FusaNoError - if succeed
  @retval FusaInvalidParam - if pStlResult is NULL
**/
FUSA_LIB_STATUS
GenerateCrc(
  IN FUSA_TEST_RESULT * const pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  //expect size to be multiple of 4
  ASSERT ((sizeof(FUSA_TEST_RESULT) & 0x3) == 0);

  if (NULL != pFusaTestResult) {
    pFusaTestResult->Crc32  = AsmCrc32Calc (
                                (UINT8 *)pFusaTestResult,
                                sizeof(FUSA_TEST_RESULT)-4,
                                0xffffffffU
                                );
    LibStatus = FusaNoError;
  } else {
    LibStatus = FusaInvalidParam;
  }

  return LibStatus;
}

/**
  Initializes the test result structure with default values
  beside the dedicated TestNumber and TotalChecks

  @param[in] TestNumber - unique id assigned to a specific test
  @param[in] TotalNumberOfChecks - total number of checks for
        the test
  @param[in,out] pFusaTestResult - summarized reporting
    structure for the whole test

  @retval FusaNoError - if succeed
  @retval FusaInvalidParam - if pFusaTestResult is NULL
  @retval FusaInternalError - if TotalNumberOfChecks is 0 or
           > sizeof(CheckResults)
**/
FUSA_LIB_STATUS
InitializeResults(
  IN FUSA_TEST_NUMBER TestNumber,
  IN UINT32 TotalNumberOfChecks,
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;

  if ((0U == TotalNumberOfChecks)
      || (TotalNumberOfChecks > sizeof(pFusaTestResult->CheckResults))
     )
  {
    //Either totalNumberOfChecks is 0 or exceed the size of CheckResults[]
    LibStatus = FusaInternalError;
  }
  else if (NULL != pFusaTestResult)
  {
    UINT8 i;

    pFusaTestResult->TestNumber = (UINT32) TestNumber;
    pFusaTestResult->TotalChecks = TotalNumberOfChecks;

    pFusaTestResult->TestResult = FUSA_TEST_NOTRUN;

    for (i = 0U; i < sizeof(pFusaTestResult->CheckResults); i++) {
      pFusaTestResult->CheckResults[i] = FUSA_TEST_NOTRUN;
    }

    LibStatus = FusaNoError;
  } else {
    LibStatus = FusaInvalidParam;
  }

  return LibStatus;
}

/**
  Updates the reporting structure with check result.
  Will need to be called after every check is done.

  @note It is expected that for every test, InitializeResults()
        routine has called once before the first call to this
        routine.

  @param[in] TestCheckVal - result of executing a check
  @param[in]  CheckNum - check number for subtest
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
  )
{
  FUSA_LIB_STATUS LibStatus;

  if (NULL != pFusaTestResult) {
    if (((UINT32) CheckNum < pFusaTestResult->TotalChecks)
        &&  (FUSA_TEST_NOTRUN == pFusaTestResult->CheckResults[CheckNum])
       )
    {
      pFusaTestResult->CheckResults[CheckNum] = TestCheckVal;

      if (FUSA_TEST_FAIL != pFusaTestResult->TestResult)
      {
        pFusaTestResult->TestResult = (pFusaTestResult->CheckResults)[CheckNum];
      }

      LibStatus = FusaNoError;
    } else {
      //Either checkNum exceed the TotalChecks or the checkResults[CheckNum] is not init or used more than once
      LibStatus = FusaInternalError;
    }
  } else {
    LibStatus = FusaInvalidParam;
  }

  return LibStatus;
}

/**
 * Map TestResult to its matching string for FuSa test
 *
 * @param[in] TestResult
 *
 * @return matching string in related to the TestResult value
 */
STATIC
VOID
TestResultString (
  IN UINT8 TestResult
  )
{
  switch (TestResult) {
    case FUSA_TEST_DEVICE_NOTAVAILABLE:
      DEBUG((DEBUG_INFO, "TEST_DEV_NA\n"));
      break;
    case FUSA_TEST_NOTRUN:
      DEBUG((DEBUG_INFO, "TEST_NOTRUN\n"));
      break;
    case FUSA_TEST_FAIL:
      DEBUG((DEBUG_INFO, "TEST_FAIL\n"));
      break;
    case FUSA_TEST_PASS:
      DEBUG((DEBUG_INFO, "TEST_PASS\n"));
      break;
    case FUSA_TEST_NOT_SUPPORTED:
      DEBUG((DEBUG_INFO, "TEST_NOT_SUPPORTED\n"));
      break;
    default:
      DEBUG((DEBUG_INFO, "ILLEGAL_STATE\n"));
      break;
  }
  return;
}

/**
  Dump Fusa test result for debug purpose. Does nothing in
  release build.

  @param[in] pFusaTestResult - pointer to the result buffer to
        be dumped
**/
VOID
DumpResults(
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  DEBUG ((DEBUG_INFO,   "Test Number         = %d \n", pFusaTestResult->TestNumber));
  DEBUG ((DEBUG_INFO,   "Total Checks        = %d \n", pFusaTestResult->TotalChecks));
  DEBUG ((DEBUG_INFO,   "Test Result         = "));
  TestResultString(pFusaTestResult->TestResult);
  for (UINT32 i = 0; i < pFusaTestResult->TotalChecks; i++) {
    DEBUG ((DEBUG_INFO, "  CheckResults %d    = ", i));
    TestResultString(pFusaTestResult->CheckResults[i]);
  }
  DEBUG ((DEBUG_INFO,   "Crc32               = %lx \n", pFusaTestResult->Crc32));
}

/**
  Wraps the calling to FusaTest with the generic Fusa test
  result reporting flow.

  @param[in] FusaTest - function pointer to the test. NULL
        implies FUSA_TEST_DEVICE_NOTAVAILABLE conditon
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
  IN FUSA_TEST_FUNC FusaTest,
  IN FUSA_TEST_NUMBER TestNum,
  IN UINT32 TotalNumberOfChecks,
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  LibStatus = InitializeResults (TestNum, TotalNumberOfChecks, pFusaTestResult);

  if (FusaNoError == LibStatus) {
    //NULL implies target device not available
    if (NULL == FusaTest) {
      pFusaTestResult->TestResult = FUSA_TEST_DEVICE_NOTAVAILABLE;
    } else {
       LibStatus = FusaTest (pFusaTestResult);
    }
  }

  if (FusaNoError == LibStatus) {
    LibStatus = GenerateCrc (pFusaTestResult);
  }

  return LibStatus;
}

