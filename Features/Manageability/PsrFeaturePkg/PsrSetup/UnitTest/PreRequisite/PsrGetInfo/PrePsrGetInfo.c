/** @file
  PreRequisite functions of the PsrGetInfo function.

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

#include "PrePsrGetInfo.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>

/**
  Set up services for PsrGetInfo function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupPsrGetInfoEnvironment (
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

  //
  // Mock boot service
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  if (gBS == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gBS->LocateProtocol = MockBootServiceLocateProtocol;

  //
  // Mock Protocol gPlatformServiceRecordProtocolGuid
  //
  gPlatformServiceRecord = AllocateZeroPool (sizeof (PLATFORM_SERVICE_RECORD_PROTOCOL));
  if (gPlatformServiceRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gPlatformServiceRecord->PsrGetData = MockPlatformServiceRecordPsrGetData;

  // Mock function for gBS->LocateProtocol
  expect_value (MockBootServiceLocateProtocol, Protocol, &gPlatformServiceRecordProtocolGuid);
  expect_value (MockBootServiceLocateProtocol, Registration, NULL);
  expect_any (MockBootServiceLocateProtocol, Interface);
  will_return (MockBootServiceLocateProtocol, gPlatformServiceRecord);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  // Mock function for PlatformServiceRecordProtocol->PsrGetData
  if (!GenerateRandomPsrData (&gRandomPsrData)) {
    return EFI_LOAD_ERROR;
  }
  expect_value (MockPlatformServiceRecordPsrGetData, This, gPlatformServiceRecord);
  expect_value (MockPlatformServiceRecordPsrGetData, PsrDataBlobSize, sizeof (PSR_DATA));
  expect_any (MockPlatformServiceRecordPsrGetData, PsrDataBlob);
  will_return (MockPlatformServiceRecordPsrGetData, gRandomPsrData);
  will_return (MockPlatformServiceRecordPsrGetData, EFI_SUCCESS);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute PsrGetInfo function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PrePsrGetInfo (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupPsrGetInfoEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = PsrGetInfo (&mPsrConfiguration);

  return UNIT_TEST_PASSED;
}
