/** @file
  Unit test case of the Memory Health Insights driver.

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
#ifndef __MHI_TEST_CASE__
#define __MHI_TEST_CASE__

#include <Uefi.h>
#include <DxeMemoryHealthInsights.h>
#include <Library/UnitTestLib.h>
#include "MemInfoHob.h"

/**
  Randomly generates number of input length.

  @param[in]  len     Number of bytes to generate

  @retval     num     Randomly generated number
**/
UINTN
GetRandNum (
  IN UINTN  len
  );

/**
  Generates and prints MRC data info HOB for test

  @param[in, out]  MemInfoData   Memory info data hob structure pointer

  @retval     EFI_SUCCESS           Failed to set data and print
  @retval     EFI_INVALID_PARAMETER Inputs are invalid
**/
EFI_STATUS
GenMemInfoHob (
  IN OUT MEMORY_INFO_DATA_HOB   *MemInfoData
  );

/**
  Unit-Test Function for UpdateMemInfoToMhiPhat.

  @param[in]  Context                            An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
VerifyUpdateMhiTable (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // __MHI_TEST_CASE__
