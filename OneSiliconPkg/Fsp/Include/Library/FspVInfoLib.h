/** @file
  FSP Validation support library.

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

#ifndef _FSPV_INFO_LIB_H_
#define _FSPV_INFO_LIB_H_

#define MAX_LENGTH_FAILURE_SIGNATURE  256
#define MAX_LENGTH_TEST_CASE_NAME     64

#pragma pack(1)
typedef struct {
  BOOLEAN  IsTestPassed;
  CHAR16   TestCaseName[MAX_LENGTH_TEST_CASE_NAME];
  CHAR16   FailureSignature[MAX_LENGTH_FAILURE_SIGNATURE];
} FSPV_INFO_HOB;
#pragma pack()

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
  );

extern EFI_GUID gFspVInfoGuid;
#endif // _FSPV_INFO_LIB_H_
