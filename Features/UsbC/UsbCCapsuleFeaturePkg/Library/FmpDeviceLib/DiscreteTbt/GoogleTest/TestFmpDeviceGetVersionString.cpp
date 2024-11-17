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
#include <GTestFmpDeviceLibDiscreteTbt.h>

/**
Google test for FmpDeviceGetVersionString function.
**/
class FmpDeviceGetVersionStringTest : public Test {
  protected:
    EFI_STATUS Status;
    CHAR16     **VersionString;

  void SetUp() override {
    VersionString = (CHAR16 **) AllocatePool (1);
    ASSERT_NE (VersionString, nullptr);
  }
};

TEST_F (FmpDeviceGetVersionStringTest, Var_1_Error) {
  //
  // Case 1 - VersionString is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceGetVersionString (NULL);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceGetVersionStringTest, Var_2_UnSupported) {
  //
  // Case 2 - General Case
  // Expected Result - Status will return EFI_UNSUPPORTED
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = FmpDeviceGetVersionString (VersionString);
  EXPECT_EQ (Status, EFI_UNSUPPORTED);

  cout << "FmpDeviceGetVersionString Done." << endl;
}
