/**@file
    Unit test cases of GetBlockSidState.

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

#include "GetBlockSidStateMock.h"

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
GetBlockSidState_SscpMock (
  OUT PLATFORM_OPAL_DRIVER_DEVICE    *OpalDevice
  )
{
  if (OpalDevice == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  if (OpalDevice->Handle == NULL) {
    OpalDevice->Handle = AllocateZeroPool (sizeof (EFI_HANDLE));
  }
  OpalDevice->OpalDisk.Sscp = SscpConstructorMock ();
  OpalDevice->OpalDisk.MediaId = 1;
  OpalDevice->OpalDisk.OpalBaseComId = 0x10;
  return UNIT_TEST_PASSED;
}

/**
  Unit-Test GetOpalState CleanUp Function.

  @param[in]  OpalDevice    Input OpalDevice mock need to be cleanUp.

**/
VOID
EFIAPI
GetBlockSidState_SscpCleanUp (
  IN PLATFORM_OPAL_DRIVER_DEVICE    *OpalDevice
  )
{
  if (OpalDevice != NULL) {
    if (OpalDevice->Handle != NULL) {
      FreePool (OpalDevice->Handle);
    }
    SscpDestructorMock ();
  }
}

/**
  Unit test for GetBlockSidState.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
GetBlockSidStateUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PLATFORM_OPAL_DRIVER           *Private;
  EFI_HANDLE                     Handle;
  UNIT_TEST_STATUS               TestStatus;
  EFI_STATUS                     Status;
  UINT8                          BlockSidState;
  TCG_LEVEL0_DISCOVERY_HEADER    *DiscoveryHeader;
  OPAL_LEVEL0_FEATURE_DESCRIPTOR *Feat;

  DiscoveryHeader = NULL;
  Private = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER));
  Private->Signature = PLATFORM_OPA_DRIVER_SIGNATURE;

  Status = GetBlockSidState (&Private->PlatformOpalProtocol, NULL, &BlockSidState);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  Handle = AllocateZeroPool (sizeof (EFI_HANDLE));
  if (Handle != NULL) {
    Status = GetBlockSidState (&Private->PlatformOpalProtocol, Handle, &BlockSidState);
    UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
    FreePool (Handle);
  }

  //
  // Mock SSCP
  //
  Private->DeviceList = AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Private->DeviceList != NULL) {
    TestStatus = GetBlockSidState_SscpMock (Private->DeviceList);
    if (TestStatus == UNIT_TEST_PASSED && (Private->DeviceList->OpalDisk.Sscp != NULL)) {
      will_return (SscpReceiveDataMock, EFI_TIMEOUT);
      Status = GetBlockSidState (&Private->PlatformOpalProtocol, Private->DeviceList->Handle, &BlockSidState);
      UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);

      will_return (SscpReceiveDataMock, EFI_SUCCESS);
      will_return (SscpReceiveDataMock, NULL);
      Status = GetBlockSidState (&Private->PlatformOpalProtocol, Private->DeviceList->Handle, &BlockSidState);
      UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

      DiscoveryHeader = AllocateZeroPool (sizeof (TCG_SUPPORTED_SECURITY_PROTOCOLS) + 1);
      if (DiscoveryHeader != NULL) {
        //
        // Mock Level 0 Discovery Header, support Block Sid
        //
        Feat = (OPAL_LEVEL0_FEATURE_DESCRIPTOR *)(DiscoveryHeader + 1);
        Feat->BlockSid.Header.FeatureCode_BE = 0x0204;
        Feat->BlockSid.Header.Length = 0x12;
        Feat->BlockSid.SIDBlockedState = 1;
        DiscoveryHeader->LengthBE = sizeof (TCG_LEVEL0_DISCOVERY_HEADER) - sizeof (DiscoveryHeader->LengthBE);
        DiscoveryHeader->LengthBE += Feat->BlockSid.Header.Length + sizeof (TCG_LEVEL0_FEATURE_DESCRIPTOR_HEADER);
        SwapBytes32 (DiscoveryHeader->LengthBE);
        will_return (SscpReceiveDataMock, EFI_SUCCESS);
        will_return (SscpReceiveDataMock, (VOID*)DiscoveryHeader);
        Status = GetBlockSidState (&Private->PlatformOpalProtocol, Private->DeviceList->Handle, &BlockSidState);
        UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
        UT_ASSERT_EQUAL (BlockSidState, Feat->BlockSid.SIDBlockedState);
        FreePool (DiscoveryHeader);
        GetBlockSidState_SscpCleanUp (Private->DeviceList);
      }
    }
    FreePool (Private->DeviceList);
  }

  return UNIT_TEST_PASSED;
}
