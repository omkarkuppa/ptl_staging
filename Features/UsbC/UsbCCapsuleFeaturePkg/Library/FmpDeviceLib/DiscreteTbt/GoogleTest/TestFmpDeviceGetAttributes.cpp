/** @file
  Google Test for the implementation of FmpDeviceLib instance to support 
  Discrete Thunderbolt Firmware update

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

/**
Google test for FmpDeviceGetAttributes function.
**/
class FmpDeviceGetAttributesTest : public CommonMock {
  protected:
  EFI_STATUS     Status;
  UINT64        *Supported;
  UINT64        *Setting;

  void SetUp() override {
    Supported = (UINT64*) AllocatePool(1);
    Setting = (UINT64*) AllocatePool(1);
    ASSERT_NE(Supported, nullptr);
    ASSERT_NE(Setting, nullptr);
  }
};

TEST_F (FmpDeviceGetAttributesTest, VarError_1) {

  //
  // Case 1 - Supported is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceGetAttributes (NULL, Setting);
  EXPECT_EQ(Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceGetAttributesTest, VarError_2) {

  //
  // Case 2 - Setting is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = FmpDeviceGetAttributes (Supported, NULL);
  EXPECT_EQ(Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceGetAttributesTest, VarSuccess) {

  //
  // Case 3 - Success Case
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  Status = FmpDeviceGetAttributes (Supported, Setting);
  EXPECT_EQ(Status, EFI_SUCCESS);

  cout << "FmpDeviceGetAttributes Done." << endl;

}
