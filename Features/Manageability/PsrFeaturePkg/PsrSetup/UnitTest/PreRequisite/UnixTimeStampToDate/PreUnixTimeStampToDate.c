/** @file
  PreRequisite functions of the UnixTimeStampToDate function.

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

#include "PreUnixTimeStampToDate.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>

/**
  Set up services for UnixTimeStampToDate function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupUnixTimeStampToDateEnvironment (
  VOID
  )
{
  //
  // Global variable initialization
  //
  gReturnResult = AllocateZeroPool (sizeof (EFI_TIME));
  if (gReturnResult == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute UnixTimeStampToDate function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreUnixTimeStampToDate (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupUnixTimeStampToDateEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  UnixTimeStampToDate (1703751936, (EFI_TIME*)gReturnResult);

  return UNIT_TEST_PASSED;
}
