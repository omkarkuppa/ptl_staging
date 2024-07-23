/** @file
  FSP Information Library.

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

@par Specification Reference:
**/

#include <Library/HobLib.h>
#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FspVInfoLib.h>

/**
  Create FspV Test Result HOB.

  @param[in] IN CHAR16   *TestName,         Name of the test case.
  @param[in] IN BOOLEAN  TestResult,        Result of the test case execution.
  @param[in] IN CHAR16   *FailureSignature  Failure description if test case fails.
**/
VOID
CreateFspVResultHob (
  IN CHAR16   *TestName,
  IN BOOLEAN  TestResult,
  IN CHAR16   *FailureSignature
  )
{
  FSPV_INFO_HOB  *FspVInfo;
  //
  // Create FSP Information HOB
  //
  FspVInfo = BuildGuidHob (
    &gFspVInfoGuid,
    sizeof (FSPV_INFO_HOB)
    );
  ASSERT (FspVInfo != NULL);

  FspVInfo->IsTestPassed = TestResult;
  CopyMem (FspVInfo->TestCaseName, TestName, StrSize (TestName));
  CopyMem (FspVInfo->FailureSignature, FailureSignature, StrSize (FailureSignature));

  DEBUG ((DEBUG_INFO, "  TestName          - %s\n", FspVInfo->TestCaseName));
  DEBUG ((DEBUG_INFO, "  TestCaseResult    - %d\n", FspVInfo->IsTestPassed));
  DEBUG ((DEBUG_INFO, "  FailureSignature  - %s\n", FspVInfo->FailureSignature));
}
