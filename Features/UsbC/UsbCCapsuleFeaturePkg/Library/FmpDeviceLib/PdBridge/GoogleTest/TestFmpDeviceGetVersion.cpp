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
Google test for FmpDeviceGetVersion function.
**/

class FmpDeviceGetVersionTest : public Test {
  protected:
    EFI_STATUS Status;
    UINT32     *Version;

    MockUefiRuntimeServicesTableLib UefiRuntimeServicesTableLibMock;

  void SetUp() override {
    Version = (UINT32 *) AllocatePool (1);
    ASSERT_NE (Version, nullptr);
  }
};

TEST_F (FmpDeviceGetVersionTest, Case_1) {
  //
  // Case 1 - Version is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceGetVersion (NULL);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceGetVersionTest, Case_2) {
  //
  // Case 2 - General Case
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_GetVariable)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_SetVariable)
    .WillOnce (Return (EFI_SUCCESS));

  Status = FmpDeviceGetVersion (Version);
  EXPECT_EQ (Status, EFI_SUCCESS);

  cout << "FmpDeviceGetVersion Done." << endl;
}
