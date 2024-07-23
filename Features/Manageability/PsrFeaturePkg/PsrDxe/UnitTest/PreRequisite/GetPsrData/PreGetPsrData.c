/** @file
  PreRequisite functions of the GetPsrData function.

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

#include "PreGetPsrData.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <PsrMsgs.h>

/**
  Set up services for GetPsrData function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupGetPsrDataEnvironment (
  VOID
  )
{
  //
  // Global variable initialization
  //
  gReturnResult = AllocateZeroPool (sizeof (EFI_STATUS));
  if (gReturnResult == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *(EFI_STATUS*)gReturnResult = EFI_UNSUPPORTED;

  gReturnPsrData = AllocateZeroPool (sizeof (PSR_DATA));
  if (gReturnPsrData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Mock function for WaitFirmwareInitComplete
  will_return (WaitFirmwareInitComplete, EFI_SUCCESS);

  // Mock function for HeciGetPsrRecord
  gRandomPsrData = AllocateZeroPool (sizeof (PSR_DATA));
  if (gRandomPsrData == NULL || !RandomData (gRandomPsrData, sizeof (PSR_DATA), FALSE)) {
    return EFI_OUT_OF_RESOURCES;
  }
  expect_any (HeciGetPsrRecord, PsrData);
  will_return (HeciGetPsrRecord, (PSR_DATA*) gRandomPsrData);
  will_return (HeciGetPsrRecord, EFI_SUCCESS);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute GetPsrData function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreGetPsrData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupGetPsrDataEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = GetPsrData (
                                  NULL,
                                  sizeof (PSR_DATA),
                                  gReturnPsrData
                                  );

  return UNIT_TEST_PASSED;
}
