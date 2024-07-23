/** @file
  Google Test for the implementation of  FmpDeviceLib instance to support 
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

/**
Google test for FmpDeviceSetImage function.
**/
class FmpDeviceSetImageTest : public CommonMock {
  protected:
  EFI_STATUS                           Status;
  VOID                                 *Image;
  VOID                                 *Image_Unsupport;
  VOID                                 *Image_Integrated;
  UINTN                                ImageSize;
  VOID                                 *VendorCode;
  UINT32                               CapsuleFwVersion;
  CHAR16                               **AbortReason;
  UINT32                               *LastAttemptStatus;
  UINT32                               OverSize;
  EFI_BOOT_SERVICES                    *gBS;
  USBC_RETIMER_PROTOCOL                *UsbCRetimerProtocol;
  PAYLOAD_HEADER                       *RetimerPayloadHeader;
  RETIMER_ITEM                         *RetimerPayloadItem;
  EFI_HANDLE                           *DeviceHandleBuffer;
  UINTN                                DeviceHandleCount;
  RETIMER_DEV_INSTANCE                 RetimerDevice;

  void SetUp() override {
    Image = (VOID *) &MockRetimerImage;
    Image_Unsupport = (VOID *) &MockUnsupportedRetimerImage;
    Image_Integrated = (VOID *) &MockIntegratedRetimerImage;
    ImageSize = sizeof (MockRetimerImage);
    VendorCode=NULL;
    CapsuleFwVersion=0;
    AbortReason=NULL;
    LastAttemptStatus=(UINT32*)AllocateZeroPool (sizeof (LastAttemptStatus));
    gBS = (EFI_BOOT_SERVICES *) AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
    UsbCRetimerProtocol = (USBC_RETIMER_PROTOCOL *) AllocateZeroPool (sizeof (USBC_RETIMER_PROTOCOL));
    UsbCRetimerProtocol->Drive = UsbcRetimerProtocol_Drive;
    UsbCRetimerProtocol->Restore = UsbcRetimerProtocol_Restore;
    RetimerPayloadHeader = (PAYLOAD_HEADER *) Image;
    RetimerPayloadItem = (RETIMER_ITEM *) (RetimerPayloadHeader + 1);
    OverSize=RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize + 0x05;
    DeviceHandleBuffer = (EFI_HANDLE *) AllocateZeroPool (sizeof (EFI_HANDLE));
    DeviceHandleCount=0x01;
  }
};

TEST_F(FmpDeviceSetImageTest, VarError_1) {

  //
  // Case 1 - Image is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceSetImageWithStatus (NULL,ImageSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);
  
}

TEST_F(FmpDeviceSetImageTest, VarError_2) {

  //
  // Case 2 - Progress is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = FmpDeviceSetImageWithStatus (Image,ImageSize,VendorCode, NULL, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarError_3) {

  //
  // Case 3 - Progress is Failed , gBS->LocateProtocol return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_INVALID_PARAMETER));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateProtocol)
    .WillOnce (Return (EFI_INVALID_PARAMETER));
  Status = FmpDeviceSetImageWithStatus (Image,ImageSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarUnSupported_1) {

  //
  // Case 4 - UsbCRetimerProtocol->Drive return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / UsbcRetimerProtocolMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 4 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateProtocol)
    .WillOnce (
      DoAll (
        SetArgPointee<2>(UsbCRetimerProtocol),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Drive)
    .WillOnce (Return (EFI_UNSUPPORTED));

  Status = FmpDeviceSetImageWithStatus (Image,ImageSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_UNSUPPORTED);

}

TEST_F(FmpDeviceSetImageTest, VarError_4) {

  //
  // Case 5 - UsbCRetimerProtocol->Restore return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / UsbcRetimerProtocolMock / MicroSecondDelayMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 5 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateProtocol)
     .WillOnce (
      DoAll (
        SetArgPointee<2>(UsbCRetimerProtocol),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Drive)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock,
    MicroSecondDelay);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Restore)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL(FirmwareManagementMock,Progress(100));

  Status = FmpDeviceSetImageWithStatus (Image,ImageSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarSuccess_1) {

  //
  // Case 6 - FirmwareType is DISCRETE_TBT , PayLoad - Update on un-support FirmwareType DISCRETE_TBT .
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / UsbcRetimerProtocolMock / MicroSecondDelayMock
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 6 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateProtocol)
     .WillOnce (
      DoAll (
        SetArgPointee<2>(UsbCRetimerProtocol),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Drive)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock,
    MicroSecondDelay);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Restore)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL(FirmwareManagementMock,Progress(100));

  Status = FmpDeviceSetImageWithStatus (Image_Unsupport,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_SUCCESS);

}

TEST_F(FmpDeviceSetImageTest, VarSuccess_2) {

  //
  // Case 7 - FirmwareType is INTEGRATED_TBT_RETIMER 
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / UsbcRetimerProtocolMock / MicroSecondDelayMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 7 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateProtocol)
     .WillOnce (
      DoAll (
        SetArgPointee<2>(UsbCRetimerProtocol),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Drive)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock,
    MicroSecondDelay);

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    CreateRetimerDevInstance)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Restore)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL(FirmwareManagementMock,Progress(100));

  Status = FmpDeviceSetImageWithStatus (Image_Integrated,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_SUCCESS);

}

TEST_F(FmpDeviceSetImageTest, VarUnSupported_2) {

  //
  // Case 8 - FirmwareType is DISCRETE_TBT_RETIMER , UsbCRetimerProtocol->Restore return error.
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / UsbcRetimerProtocolMock / MicroSecondDelayMock 
  // Expected Result - Status will return EFI_UNSUPPORTED
  //
  cout << "[---------- Case 8 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateProtocol)
     .WillOnce (
      DoAll (
        SetArgPointee<2>(UsbCRetimerProtocol),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Drive)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock,
    MicroSecondDelay);

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer)
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Restore)
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL(FirmwareManagementMock,Progress(100));

  Status = FmpDeviceSetImageWithStatus (Image,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_UNSUPPORTED);

}

TEST_F(FmpDeviceSetImageTest, VarError_5) {

  //
  // Case 9 - CreateRetimerDevInstance return error.
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / UsbcRetimerProtocolMock / MicroSecondDelayMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_ALREADY_STARTED
  //
  cout << "[---------- Case 9 ----------]"<< endl;

  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateProtocol)
     .WillOnce (
      DoAll (
        SetArgPointee<2>(UsbCRetimerProtocol),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Drive)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock,
    MicroSecondDelay);

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer)
     .WillOnce (
      DoAll (
        SetArgBuffer<3>(&DeviceHandleCount, sizeof(DeviceHandleCount)),
        SetArgPointee<4>(DeviceHandleBuffer),
        Return (EFI_SUCCESS)
    ));
   
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_DisconnectController);

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    CreateRetimerDevInstance)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Restore)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL(FirmwareManagementMock,Progress(100));

  Status = FmpDeviceSetImageWithStatus (Image,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarUnSupported_3) {

  //
  // Case 10 - ReadRetimerNvmVersion return error.
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / UsbcRetimerProtocolMock / MicroSecondDelayMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_UNSUPPORTED
  //
  cout << "[---------- Case 10 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateProtocol)
     .WillOnce (
      DoAll (
        SetArgPointee<2>(UsbCRetimerProtocol),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Drive)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock,
    MicroSecondDelay);

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer)
     .WillOnce (
      DoAll (
        SetArgBuffer<3>(&DeviceHandleCount, sizeof(DeviceHandleCount)),
        SetArgPointee<4>(DeviceHandleBuffer),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_DisconnectController);

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    CreateRetimerDevInstance)
     .WillOnce (
      DoAll (
        SetArgPointee<4>(RetimerDevice),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    UpdateRetimerNvmFirmware)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    ReadRetimerNvmVersion)
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    UpdateRetimerNvmInformation);

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    DestroyRetimerDevInstance);

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_ConnectController);

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_FreePool);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_Restore)
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL(FirmwareManagementMock,Progress(100));

  Status = FmpDeviceSetImageWithStatus (Image,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_UNSUPPORTED);

  cout << "FmpDeviceSetImage Done." << endl;

}