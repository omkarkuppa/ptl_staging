/** @file
  Google Test for the implementation of FmpDeviceLib instance to support
  Thunderbolt Retimer Firmware update

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

@par Specification Reference:

**/
#include <GTestFmpDeviceLibUsbCRetimer.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>

/**
Google test for FmpDeviceGetVersion function.
**/
class FmpDeviceGetVersionTest : public Test {
  protected:
    EFI_STATUS                      Status;
    UINT32                          *Version1;
    UINT32                          *Version2;
    MockUefiRuntimeServicesTableLib RtServicesMock;

  void SetUp() override {
    Version1 = NULL;
    Version2 = (UINT32 *) AllocatePool (1);
    ASSERT_NE (Version2, nullptr);
  }
};

TEST_F (FmpDeviceGetVersionTest, Var_1_Error) {
  //
  // Case 1 - Version is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceGetVersion (Version1);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceGetVersionTest, Var_2_UnSupported) {
  //
  // Case 2 - gRT->GetVariable() fails
  // Mock -  RtServicesMock.
  // Expected Result - Status will return EFI_UNSUPPORTED
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  EXPECT_CALL (RtServicesMock, gRT_GetVariable)
    .WillOnce (Return (EFI_UNSUPPORTED));

  Status = FmpDeviceGetVersion (Version2);
  EXPECT_EQ (Status, EFI_UNSUPPORTED);
}

TEST_F (FmpDeviceGetVersionTest, Var_3_Success) {
  //
  // Case 3 - Success Case
  // Mock -  RtServicesMock.
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  EXPECT_CALL (RtServicesMock, gRT_GetVariable)
    .WillOnce (Return (EFI_SUCCESS));

  Status = FmpDeviceGetVersion (Version2);
  EXPECT_EQ (Status, EFI_SUCCESS);

  cout << "FmpDeviceGetVersion Done." << endl;
}
