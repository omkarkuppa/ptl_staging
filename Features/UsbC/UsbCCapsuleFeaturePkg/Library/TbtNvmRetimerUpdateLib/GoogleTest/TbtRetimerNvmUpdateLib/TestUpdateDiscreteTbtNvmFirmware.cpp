/** @file
  Google test for UpdateDiscreteTbtNvmFirmware function.

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
// UpdateDiscreteTbtNvmFirmware Unit Test                  *
//**********************************************************
class UpdateDiscreteTbtNvmFirmwareTest : public CommonMock {
  protected:
    EFI_STATUS                       Status;
    DISCRETE_TBT_DEV_INSTANCE        DiscreteTbtDevice;
    UINT8                            *DiscreteTbtImage;
    UINTN                            ImageSize;
    UINTN                            StartPercentage;
    UINTN                            EndPercentage;

    TBT_HOST_ROUTER                  *HrPtr;

    void SetUp() override {
      HrPtr                          = (TBT_HOST_ROUTER *) AllocateZeroPool (sizeof (TBT_HOST_ROUTER));
      DiscreteTbtDevice              = HrPtr;
      DiscreteTbtImage               = ((UINT8 *) &MockDtbtImage) + MockDtbtImage.PayloadItem.ImageOffset;
      ImageSize                      = MockDtbtImage.PayloadItem.ImageSize;
      StartPercentage                = (6 + (90 / MockDtbtImage.PayloadHeader.PayloadCount) * 0);
      EndPercentage                  = (6 + (90 / MockDtbtImage.PayloadHeader.PayloadCount) * 1);
    }

    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (HrPtr);
    }
};

//
// Case 1 - (DiscreteTbtDevice == NULL) || (DiscreteTbtImage == NULL) || (EndPercentage < StartPercentage)
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (UpdateDiscreteTbtNvmFirmwareTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  StartPercentage                = (6 + (90 / MockDtbtImage.PayloadHeader.PayloadCount) * 2);

  Status = UpdateDiscreteTbtNvmFirmware (
             DiscreteTbtDevice,
             DiscreteTbtImage,
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
TEST_F (UpdateDiscreteTbtNvmFirmwareTest, TbtNvmDrvDeviceWrOffsetReturnError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
      0x0))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (FmpLibMock,
    Progress  (
      _))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdateDiscreteTbtNvmFirmware (
             DiscreteTbtDevice,
             DiscreteTbtImage,
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
TEST_F (UpdateDiscreteTbtNvmFirmwareTest, TbtNvmDrvDeviceWrBlkReturnError1) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
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
      DiscreteTbtDevice,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  Status = UpdateDiscreteTbtNvmFirmware (
             DiscreteTbtDevice,
             DiscreteTbtImage,
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
TEST_F (UpdateDiscreteTbtNvmFirmwareTest, TbtNvmDrvDeviceWrBlkReturnError2) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
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
      DiscreteTbtDevice,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_INVALID_PARAM));

  Status = UpdateDiscreteTbtNvmFirmware (
             DiscreteTbtDevice,
             DiscreteTbtImage,
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
TEST_F (UpdateDiscreteTbtNvmFirmwareTest, TbtNvmDrvDeviceExecAuthReturnError) {
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
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
      DiscreteTbtDevice,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceExecAuth (
      DiscreteTbtDevice
    ))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  Status = UpdateDiscreteTbtNvmFirmware (
             DiscreteTbtDevice,
             DiscreteTbtImage,
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
TEST_F (UpdateDiscreteTbtNvmFirmwareTest, CorrectFlow) {
  cout << "[---------- Case 6 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
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
      DiscreteTbtDevice,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceExecAuth (
      DiscreteTbtDevice
    ))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  Status = UpdateDiscreteTbtNvmFirmware (
             DiscreteTbtDevice,
             DiscreteTbtImage,
             ImageSize,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_SUCCESS);
}
