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
Google test for FmpDeviceSetImage function.
**/
class FmpDeviceSetImageTest : public CommonMock {
  protected:
  EFI_STATUS                          Status;
  VOID                                *Image;
  VOID                                *Image_NonDtbt;
  UINTN                               ImageSize;
  VOID                                *VendorCode;
  UINT32                              CapsuleFwVersion;
  CHAR16                              **AbortReason;
  UINT32                              *LastAttemptStatus;
  UINT32                              OverSize;
  UINT32                              LessSize;
  EFI_BOOT_SERVICES                   *gBS;
  EFI_PCI_IO_PROTOCOL                 *PciIopProtocol;
  PAYLOAD_HEADER                      *DiscreteTbtPayloadHeader;
  DISCRETE_TBT_ITEM                   *DiscreteTbtPayloadItem;
  EFI_HANDLE                          *DeviceHandleBuffer;
  UINTN                               DeviceHandleCount;
  DISCRETE_TBT_DEV_INSTANCE           DiscreteTbtDevice;
  UINTN                               BusNumber;

  void SetUp() override {
    Image = (VOID *) &MockDtbtImage;
    Image_NonDtbt = (VOID *) &MockNonDtbtImage;
    ImageSize = sizeof (MockDtbtImage);
    VendorCode=NULL;
    CapsuleFwVersion=0;
    AbortReason=NULL;
    LastAttemptStatus=(UINT32*)AllocateZeroPool (sizeof (LastAttemptStatus));
    gBS = (EFI_BOOT_SERVICES *) AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
    PciIopProtocol=(EFI_PCI_IO_PROTOCOL*)AllocateZeroPool (sizeof (EFI_PCI_IO_PROTOCOL));
    DiscreteTbtPayloadHeader = (PAYLOAD_HEADER *) Image;
    DiscreteTbtPayloadItem = (DISCRETE_TBT_ITEM *) (DiscreteTbtPayloadHeader + 1);
    OverSize=DiscreteTbtPayloadItem->ImageOffset + DiscreteTbtPayloadItem->ImageSize + 0x05;
    LessSize=DiscreteTbtPayloadItem->ImageOffset + DiscreteTbtPayloadItem->ImageSize - 0x05;
    DeviceHandleBuffer = (EFI_HANDLE *) AllocateZeroPool (sizeof (EFI_HANDLE));
    PciIopProtocol->GetLocation = PciIopProtocol_GetLocation;
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
  // Case 3 - Progress is Failed , Image size is too small
  // Mock -  FirmwareManagementMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_INVALID_PARAMETER));
 
  Status = FmpDeviceSetImageWithStatus (Image,LessSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarError_4) {

  //
  // Case 4 - FirmwareType is not DISCRETE_TBT
  // Mock -  FirmwareManagementMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 4 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  Status = FmpDeviceSetImageWithStatus (Image_NonDtbt,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarUnSupported_1) {

  //
  // Case 5 - FirmwareType is DISCRETE_TBT ,gBS->LocateHandleBuffer return UnSupported
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock
  // Expected Result - Status will return EFI_UNSUPPORTED
  //
  cout << "[---------- Case 5 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer)
    .WillOnce (Return (EFI_UNSUPPORTED));  

  Status = FmpDeviceSetImageWithStatus (Image,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_UNSUPPORTED);

}

TEST_F(FmpDeviceSetImageTest, VarError_5) {

  //
  // Case 6 - FirmwareType is DISCRETE_TBT , gBS->LocateHandleBuffer return not found ,UpdateDiscreteTbtNvmFirmware return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 6 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer)
     .WillOnce (
      DoAll (
        SetArgBuffer<3>(&DeviceHandleCount, sizeof(DeviceHandleCount)),
        SetArgPointee<4>(DeviceHandleBuffer),
        Return (EFI_NOT_FOUND)
    ));

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    CreateTBTDevInstance)
     .WillOnce (
      DoAll (
        SetArgPointee<3>(DiscreteTbtDevice),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    UpdateDiscreteTbtNvmFirmware)
    .WillOnce(Return(EFI_INVALID_PARAMETER));

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    DestroyTbtDevInstance(_));

  EXPECT_CALL(FirmwareManagementMock,Progress(100));

  Status = FmpDeviceSetImageWithStatus (Image,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarError_6) {

  //
  // Case 7 - FirmwareType is DISCRETE_TBT , CreateTBTDevInstance return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 7 ----------]"<< endl;
  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer)
     .WillOnce (
      DoAll (
        SetArgBuffer<3>(&DeviceHandleCount, sizeof(DeviceHandleCount)),
        SetArgPointee<4>(DeviceHandleBuffer),
        Return (EFI_NOT_FOUND)
    ));

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    CreateTBTDevInstance)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = FmpDeviceSetImageWithStatus (Image,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarError_7) {

  //
  // Case 8 - FirmwareType is DISCRETE_TBT ,gBS->HandleProtocol return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 8 ----------]"<< endl;

  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer)
     .WillOnce (
      DoAll (
        SetArgBuffer<3>(&DeviceHandleCount, sizeof(DeviceHandleCount)),
        SetArgPointee<4>(DeviceHandleBuffer),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol)
    .WillOnce(Return(EFI_INVALID_PARAMETER));

  Status = FmpDeviceSetImageWithStatus (Image,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}

TEST_F(FmpDeviceSetImageTest, VarError_8) {

  //
  // Case 9 - FirmwareType is DISCRETE_TBT ,CreateTBTDevInstance return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / PciIopProtocolMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 9 ----------]"<< endl;

  EXPECT_CALL(FirmwareManagementMock,Progress(5))
    .WillOnce(Return(EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer)
     .WillOnce (
      DoAll (
        SetArgBuffer<3>(&DeviceHandleCount, sizeof(DeviceHandleCount)),
        SetArgPointee<4>(DeviceHandleBuffer),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol)
     .WillOnce (
        DoAll (
          SetArgPointee<2>(PciIopProtocol),
          Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (PciIopProtocolMock,
    PciIopProtocol_GetLocation)
      .WillOnce (
      DoAll (
        SetArgBuffer<3>(&BusNumber, sizeof(BusNumber)),
        Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_DisconnectController);

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_FreePool);

  EXPECT_CALL (TbtNvmRetimerUpdateMock,
    CreateTBTDevInstance)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = FmpDeviceSetImageWithStatus (Image,OverSize,VendorCode, Progress, CapsuleFwVersion,AbortReason,LastAttemptStatus);
  ASSERT_EQ(Status,EFI_INVALID_PARAMETER);

}
