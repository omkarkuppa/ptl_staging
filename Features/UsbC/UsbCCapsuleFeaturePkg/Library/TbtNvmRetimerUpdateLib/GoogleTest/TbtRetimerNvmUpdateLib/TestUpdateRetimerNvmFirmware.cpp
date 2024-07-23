/** @file
  Google test for UpdateRetimerNvmFirmware function.

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
// UpdateRetimerNvmFirmware Unit Test                      *
//**********************************************************
class UpdateRetimerNvmFirmwareTest : public CommonMock {
  protected:
    EFI_STATUS                       Status;
    RETIMER_DEV_INSTANCE             RetimerDevice;
    UINT8                            *RetimerImage;
    UINTN                            ImageSize;
    UINTN                            StartPercentage;
    UINTN                            EndPercentage;

    TBT_RETIMER                      *CommunicationPtr;

    void SetUp() override {
      CommunicationPtr               = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
      RetimerDevice                  = CommunicationPtr;
      RetimerImage                   = ((UINT8 *) &MockItbtRetimerImage) + MockItbtRetimerImage.PayloadItem.ImageOffset;
      ImageSize                      = MockItbtRetimerImage.PayloadItem.ImageSize;
      StartPercentage                = (6 + (90 / MockItbtRetimerImage.PayloadHeader.PayloadCount) * 0);
      EndPercentage                  = (6 + (90 / MockItbtRetimerImage.PayloadHeader.PayloadCount) * 1);
    }

    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (CommunicationPtr);
    }
};

//
// Case 1 - When (RetimerDevice == NULL) || (RetimerImage == NULL) || (EndPercentage < StartPercentage)
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (UpdateRetimerNvmFirmwareTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  StartPercentage                = (6 + (90 / MockItbtRetimerImage.PayloadHeader.PayloadCount) * 2);

  Status = UpdateRetimerNvmFirmware (
             RetimerDevice,
             RetimerImage,
             ImageSize,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 TbtNvmDrvDeviceWrOffset Status Return Error
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (UpdateRetimerNvmFirmwareTest, TbtNvmDrvDeviceWrOffsetReturnError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      RetimerDevice,
      0x0))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (FmpLibMock,
    Progress  (
      _))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdateRetimerNvmFirmware (
             RetimerDevice,
             RetimerImage,
             ImageSize,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 3 TbtNvmDrvDeviceWrBlk Status Return TBT_STATUS_NON_RECOVERABLE_ERROR
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (UpdateRetimerNvmFirmwareTest, TbtNvmDrvDeviceWrBlkReturnError1) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      RetimerDevice,
      0x0))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (FmpLibMock,
    Progress (
      _))
    .Times(2)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrBlk (
      RetimerDevice,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  Status = UpdateRetimerNvmFirmware (
             RetimerDevice,
             RetimerImage,
             ImageSize,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 4 TbtNvmDrvDeviceWrBlk Status Return TBT_STATUS_INVALID_PARAM
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (UpdateRetimerNvmFirmwareTest, TbtNvmDrvDeviceWrBlkReturnError2) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      RetimerDevice,
      0x0))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (FmpLibMock,
    Progress (
      _))
    .Times(2)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrBlk (
      RetimerDevice,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_INVALID_PARAM));

  Status = UpdateRetimerNvmFirmware (
             RetimerDevice,
             RetimerImage,
             ImageSize,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 5 TbtNvmDrvDeviceExecAuth Status Return TBT_STATUS_NON_RECOVERABLE_ERROR
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (UpdateRetimerNvmFirmwareTest, TbtNvmDrvDeviceExecAuthReturnError) {
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      RetimerDevice,
      0x0))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (FmpLibMock,
    Progress (
      _))
    .Times(2)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrBlk (
      RetimerDevice,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceExecAuth (
      RetimerDevice
    ))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  Status = UpdateRetimerNvmFirmware (
             RetimerDevice,
             RetimerImage,
             ImageSize,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 6 Correct flow
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (UpdateRetimerNvmFirmwareTest, CorrectFlow) {
  cout << "[---------- Case 6 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      RetimerDevice,
      0x0))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (FmpLibMock,
    Progress (
      _))
    .Times(2)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrBlk (
      RetimerDevice,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceExecAuth (
      RetimerDevice
    ))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  Status = UpdateRetimerNvmFirmware (
             RetimerDevice,
             RetimerImage,
             ImageSize,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_SUCCESS);
}
