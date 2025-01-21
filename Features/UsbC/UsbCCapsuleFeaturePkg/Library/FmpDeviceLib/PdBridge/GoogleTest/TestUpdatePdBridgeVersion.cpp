/** @file
  Google Test for the implementation of FmpDeviceLib instance to support PD Bridge Firmware update

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:

**/
#include <GTestFmpDeviceLibPdBridge.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
/**
Google test for UpdatePdBridgeVersion function.
**/

extern "C" {
EFI_STATUS
EFIAPI
UpdatePdBridgeVersion (
  IN  UINT32  FwVersion
  );
}

class UpdatePdBridgeVersionTest : public Test {
  protected:
    EFI_STATUS Status;
    UINT32     FwVersion;

    MockUefiRuntimeServicesTableLib UefiRuntimeServicesTableLibMock;
};

TEST_F (UpdatePdBridgeVersionTest, Case_1) {
  //
  // Case 1 - General Case
  // Mock - UefiRuntimeServicesTableLibMock / UpdatePdBridgeVersion
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_GetVariable)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_SetVariable)
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdatePdBridgeVersion (FwVersion);
  EXPECT_EQ (Status, EFI_SUCCESS);
}

TEST_F (UpdatePdBridgeVersionTest, Case_2) {
  //
  // Case 2 - General Case
  // Mock - UefiRuntimeServicesTableLibMock / UpdatePdBridgeVersion
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_GetVariable)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = UpdatePdBridgeVersion (FwVersion);
  EXPECT_EQ (Status, EFI_UNSUPPORTED);

  cout << "UpdatePdBridgeVersion Done." << endl;
}
