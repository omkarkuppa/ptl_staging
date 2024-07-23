/** @file
    Clean-up functions of the InitializeDptf function.
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
#include "CleanUpInitializeDptf.h"
#include "PrivateMock.h"
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Clean up all variables used in testing InitializeDptf function.
  @retval     EFI_SUCCESS                 Clean up success.
**/
EFI_STATUS
EFIAPI
CleanInitializeDptfEnvironment (
  )
{
  gReturnResult = EFI_UNSUPPORTED;

  FreePool (gRT);
  FreePool (gBS);
  FreePool (gFwVol);
  FreePool (gAcpiTableProtocol);
  FreePool (gSsdtTargetTable);
  
  ZeroMem ((VOID*)&mDptfSetupData, sizeof (DPTF_CONFIG));
  ZeroMem ((VOID*)mDptfNvsAreaProtocol.Area, sizeof (DPTF_NVS_AREA));

  return EFI_SUCCESS;
}

/**
  Clean up the environment for next test case.
  @param[in]  Context                          The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                 Clean up success.
  @retval     UNIT_TEST_ERROR_CLEANUP_FAILED   Fail to clean up.
**/
UNIT_TEST_STATUS
EFIAPI
CleanUpInitializeDptf (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS   Status;

  Status = CleanInitializeDptfEnvironment ();

  if (Status != EFI_SUCCESS) {
    return UNIT_TEST_ERROR_CLEANUP_FAILED;
  }
  
  return UNIT_TEST_PASSED;
}
