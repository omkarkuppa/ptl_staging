/**@file
    Unit test cases of OpalDiskUpdateOwnerShip.

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

#include "OpalDiskUpdateOwnerShipMock.h"

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
UpdateOwnerShip_SscpMock (
  OUT PLATFORM_OPAL_DRIVER_DEVICE    *OpalDevice
  )
{
  if (OpalDevice == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }

  OpalDevice->OpalDisk.Sscp = SscpConstructorMock ();
  OpalDevice->OpalDisk.MediaId = 1;
  OpalDevice->OpalDisk.OpalBaseComId = 0x0010;
  OpalDevice->OpalDisk.MsidLength = 8;
  CopyMem (OpalDevice->OpalDisk.Msid, "12345678", OpalDevice->OpalDisk.MsidLength);
  return UNIT_TEST_PASSED;
}

/**
  Unit-Test GetOpalState CleanUp Function.

  @param[in]  OpalDevice    Input OpalDevice mock need to be cleanUp.

**/
VOID
EFIAPI
UpdateOwnerShip_SscpCleanUp (
  IN PLATFORM_OPAL_DRIVER_DEVICE    *OpalDevice
  )
{
  if (OpalDevice != NULL) {
    SscpDestructorMock ();
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
OpalDiskUpdateOwnerShipUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PLATFORM_OPAL_DRIVER           *Private;
  UNIT_TEST_STATUS               TestStatus;
  EFI_STATUS                     Status;

  Private = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER));
  Private->Signature = PLATFORM_OPA_DRIVER_SIGNATURE;

  Private->DeviceList = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Private->DeviceList != NULL) {
    Status = OpalDiskUpdateOwnerShip (&Private->DeviceList->OpalDisk);
    UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

    //
    // Mock SSCP
    //
    TestStatus = UpdateOwnerShip_SscpMock (Private->DeviceList);
    if (TestStatus == UNIT_TEST_PASSED && (Private->DeviceList->OpalDisk.Sscp != NULL)) {
      will_return (SscpSendDataMock, EFI_SUCCESS);
      will_return (SscpReceiveDataMock, EFI_SUCCESS);
      will_return (SscpReceiveDataMock, NULL);
      will_return (SscpSendDataMock, EFI_SUCCESS);
      will_return (SscpReceiveDataMock, EFI_SUCCESS);
      will_return (SscpReceiveDataMock, NULL);
      Status = OpalDiskUpdateOwnerShip (&Private->DeviceList->OpalDisk);
      UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
      UpdateOwnerShip_SscpCleanUp (Private->DeviceList);
    }
    FreePool (Private->DeviceList);
  }

  return UNIT_TEST_PASSED;
}
