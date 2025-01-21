/** @file
  PreRequisite functions of the PsrSetupInit function.

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

#include "PrePsrSetupInit.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>

unsigned char PsrSetupStrings[100] = {0}; //dummy

/**
  Set up services for PsrSetupInit function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupPsrSetupInitEnvironment (
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
  gBS->InstallMultipleProtocolInterfaces = MockBootServiceInstallMultipleProtocolInterfaces;

  // Mock function for gBS->LocateProtocol
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiHiiConfigRoutingProtocolGuid);
  expect_value (MockBootServiceLocateProtocol, Registration, NULL);
  expect_any (MockBootServiceLocateProtocol, Interface);
  will_return (MockBootServiceLocateProtocol, NULL);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  // Mock function for gBS->InstallMultipleProtocolInterfaces
  gMockHandle = AllocateZeroPool (sizeof (EFI_HANDLE));
  if (gMockHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  expect_any (MockBootServiceInstallMultipleProtocolInterfaces, Handle);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, gMockHandle);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, EFI_SUCCESS);

  // Mock function for HiiAddPackages
  gMockHiiHandle = AllocateZeroPool (sizeof (EFI_HII_HANDLE));
  if (gMockHiiHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  expect_value (HiiAddPackages, PackageListGuid, &mPsrFormSetGuid);
  expect_value (HiiAddPackages, DeviceHandle, *gMockHandle);
  will_return (HiiAddPackages, gMockHiiHandle);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute PsrSetupInit function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PrePsrSetupInit (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupPsrSetupInitEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = PsrSetupInit ();

  return UNIT_TEST_PASSED;
}
