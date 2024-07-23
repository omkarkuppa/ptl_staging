/** @file
  PreRequisite functions of the GetPsrEventSource function.

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

#include "PreGetPsrEventSource.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>

/**
  Set up services for GetPsrEventSource function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupGetPsrEventSourceEnvironment (
  VOID
  )
{
  //
  // Global variable initialization
  //
  gReturnResult = AllocateZeroPool (sizeof (CHAR16) * (PSR_MAX_EVENT_NAME_SIZE + 1));
  if (gReturnResult == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute GetPsrEventSource function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreGetPsrEventSource (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupGetPsrEventSourceEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  GetPsrEventSource (0, gReturnResult);

  return UNIT_TEST_PASSED;
}
