/**@file

    CleanUp of UpdatePmaxTable function.

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

@par Specification
**/

#include "CleanUpUpdatePmaxTable.h"
#include <Library/MemoryAllocationLib.h>
#include <PmaxDevDef.h>

extern  VPD_PMAX_DEV* gMockPmaxCfg;
/**
  Clean up all variables used in testing FindPmaxTable function.
  @retval     EFI_SUCCESS                 Clean up success.
**/
EFI_STATUS
EFIAPI
CleanUpdatePmaxTableEnvironment (
  )
{
  FreePool (gMockPmaxCfg);

  return EFI_SUCCESS;
}

/**
  Clean up the UpdatePmaxTable environment for next test case.
  @param[in]  Context                          The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                 Clean up success.
  @retval     UNIT_TEST_ERROR_CLEANUP_FAILED   Fail to clean up.
**/
UNIT_TEST_STATUS
EFIAPI
CleanUpUpdatePmaxTable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS   Status;

  Status = CleanUpdatePmaxTableEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  return UNIT_TEST_PASSED;
}

