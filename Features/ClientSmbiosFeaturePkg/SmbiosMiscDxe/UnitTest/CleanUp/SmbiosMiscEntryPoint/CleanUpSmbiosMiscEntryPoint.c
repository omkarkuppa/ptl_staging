/** @file
  Clean-up functions of the SmbiosMiscEntryPoint function.

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
#include "CleanUpSmbiosMiscEntryPoint.h"
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <PrivateMock.h>

extern EFI_BOOT_SERVICES* gBS;
/**
  Clean up all variables used in testing SmbiosMiscEntryPoint function.

  @retval     EFI_SUCCESS                 Clean up success.

**/
EFI_STATUS
EFIAPI
CleanSmbiosMiscEntryPointEnvironment(
  )
{
  UINTN Index;
  FreePool (gBS);
  FreePool (gSmbiosProtocol);
  for (Index = 0; Index < gSmbiosTableIndex; Index++) {
    FreePool (gSmbiosTable[Index]);
  }
  gSmbiosTableIndex = 0;

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
CleanUpSmbiosMiscEntryPoint (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS            Status;

  DEBUG ((DEBUG_INFO, "[ Clean Up ]\n"));

  Status = CleanSmbiosMiscEntryPointEnvironment ();
  if (Status != EFI_SUCCESS) {
    return UNIT_TEST_ERROR_CLEANUP_FAILED;
  }
  return UNIT_TEST_PASSED;
}
