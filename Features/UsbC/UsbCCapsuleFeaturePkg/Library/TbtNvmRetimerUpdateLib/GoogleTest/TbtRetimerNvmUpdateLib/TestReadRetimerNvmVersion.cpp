/** @file
  Google test for ReadRetimerNvmVersion function.

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

//**********************************************************
// ReadRetimerNvmVersion Unit Test                         *
//**********************************************************
class ReadRetimerNvmVersionTest : public CommonMock {
  protected:
    EFI_STATUS                     Status;
    RETIMER_DEV_INSTANCE           RetimerDevice;
    TBT_RETIMER                    *Device;
    UINT32                         Version;

    void SetUp() override {
      Device        = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
      RetimerDevice = (RETIMER_DEV_INSTANCE) Device;
      Version       = 0x123456;
    }

    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (Device);
    }
};

//
// Case 1 - When Version = NULL
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (ReadRetimerNvmVersionTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  Status = ReadRetimerNvmVersion (
             RetimerDevice,
             NULL);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - When call TbtDrvReadNvmVersion return Error.
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (ReadRetimerNvmVersionTest, TbtDrvReadNvmVersionError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtDrvReadNvmVersion (
      Device,
      _))
    .WillOnce (Return(EFI_DEVICE_ERROR));

  Status = ReadRetimerNvmVersion (
             RetimerDevice,
             &Version);
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 3 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (ReadRetimerNvmVersionTest, CorrectFlow) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtDrvReadNvmVersion (
      Device,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<1>(&Version, sizeof(Version)),
        Return (EFI_SUCCESS)
    ));

  Status = ReadRetimerNvmVersion (
             RetimerDevice,
             &Version);
  EXPECT_EQ (Status, EFI_SUCCESS);
}
