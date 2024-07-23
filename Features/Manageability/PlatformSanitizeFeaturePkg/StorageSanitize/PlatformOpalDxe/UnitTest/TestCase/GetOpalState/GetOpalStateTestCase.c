/**@file
    Unit test cases of GetOpalState.

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

#include "GetOpalStateMock.h"

/**
  Unit-Test Prerequisite Function pointer.

  Mock the Supported Attributes for Opal Disk.

  @param[in]  Context    An optional parameter for precondition.
                         Pointer to the OPAL_DISK_SUPPORT_ATTRIBUTE.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
OpalDiskLockingFeatureMock (
  OUT PLATFORM_OPAL_DRIVER_DEVICE    *OpalDevice,
  IN  TCG_LOCKING_FEATURE_DESCRIPTOR LockingFeature
  )
{
  if (OpalDevice == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  if (OpalDevice->Handle == NULL) {
    OpalDevice->Handle = AllocateZeroPool (sizeof (EFI_HANDLE));
  }
  OpalDevice->OpalDisk.SupportedAttributes.Sp1 = 1;
  OpalDevice->OpalDisk.SupportedAttributes.OpalSsc1 = 1;
  CopyMem (&OpalDevice->OpalDisk.LockingFeature, &LockingFeature, sizeof (TCG_LOCKING_FEATURE_DESCRIPTOR));
  return UNIT_TEST_PASSED;
}

/**
  Unit-Test GetOpalState CleanUp Function.

  @param[in]  OpalDevice    Input OpalDevice mock need to be cleanUp.

**/
VOID
EFIAPI
OpalDiskLockingFeatureCleanUp (
  IN PLATFORM_OPAL_DRIVER_DEVICE    *OpalDevice
  )
{
  if (OpalDevice != NULL) {
    if (OpalDevice->Handle != NULL) {
      FreePool (OpalDevice->Handle);
      OpalDevice->Handle = NULL;
    }
  }
}

/**
  Unit test for GetOpalState.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
GetOpalStateUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PLATFORM_OPAL_DRIVER           *Private;
  EFI_HANDLE                     Handle;
  TCG_LOCKING_FEATURE_DESCRIPTOR LockingFeature;
  UNIT_TEST_STATUS               TestStatus;
  EFI_STATUS                     Status;
  BOOLEAN                        Enable;
  BOOLEAN                        Lock;

  Private = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER));
  Private->Signature = PLATFORM_OPA_DRIVER_SIGNATURE;

  Status = GetOpalState (&Private->PlatformOpalProtocol, NULL, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  Handle = AllocateZeroPool (sizeof (EFI_HANDLE));
  if (Handle != NULL) {
    Status = GetOpalState (&Private->PlatformOpalProtocol, Handle, NULL, NULL);
    UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
    FreePool (Handle);
  }

  //
  // Mock LockingFeature enable
  //
  LockingFeature.LockingSupported = 1;
  LockingFeature.LockingEnabled = 1;
  LockingFeature.Locked = 1;
  Private->DeviceList = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Private->DeviceList != NULL) {
    TestStatus = OpalDiskLockingFeatureMock (Private->DeviceList, LockingFeature);
    if (TestStatus == UNIT_TEST_PASSED) {
      Status = GetOpalState (&Private->PlatformOpalProtocol, Private->DeviceList->Handle, &Enable, &Lock);
      UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
      UT_ASSERT_EQUAL (Enable, 1);
      UT_ASSERT_EQUAL (Lock, 1);
      OpalDiskLockingFeatureCleanUp (Private->DeviceList);
    }
    FreePool (Private->DeviceList);
  }

  return UNIT_TEST_PASSED;
}
