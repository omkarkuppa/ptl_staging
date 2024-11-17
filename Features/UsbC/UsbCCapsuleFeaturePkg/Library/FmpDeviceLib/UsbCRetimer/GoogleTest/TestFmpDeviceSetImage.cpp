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
// GoogleTest mock
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Library/MockTbtNvmRetimerUpdateLib.h>
#include <GoogleTest/Library/MockTimerLib.h>
#include <GoogleTest/Library/MockBasePcdLib.h>
#include <GoogleTest/Library/MockUsbcCapsuleDebugLib.h>
#include <GoogleTest/Private/MockUsbcRetimerProtocol/MockUsbcRetimerProtocol.h>
#include <GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.h>
#include <GoogleTest/Private/MockUsbCProgressCodeProtocol/MockUsbCProgressCodeProtocol.h>

/**
Google test for FmpDeviceSetImage function.
**/
class FmpDeviceSetImageTest : public Test {
  protected:
    EFI_STATUS                                    Status;
    VOID                                          *Image;
    VOID                                          *Image_Unsupport;
    VOID                                          *Image_Integrated;
    UINTN                                         ImageSize;
    VOID                                          *VendorCode;
    UINT32                                        CapsuleFwVersion;
    CHAR16                                        **AbortReason;
    UINT32                                        *LastAttemptStatus;
    USBC_RETIMER_PROTOCOL                         UsbCRetimerProtocol;
    PAYLOAD_HEADER                                *RetimerPayloadHeader;
    RETIMER_ITEM                                  *RetimerPayloadItem;
    EFI_HANDLE                                    *DeviceHandleBuffer;
    UINTN                                         DeviceHandleCount;
    RETIMER_DEV_INSTANCE                          RetimerDevice;
    MockFirmwareManagementLib                     FirmwareManagementMock;
    MockUefiBootServicesTableLib                  UefiBootServicesTableLibMock;
    MockUsbcRetimerProtocol                       UsbcRetimerProtocolMock;
    MockTbtNvmRetimerUpdateLib                    TbtNvmRetimerUpdateMock;
    MockTimerLib                                  MicroSecondDelayMock;
    MockUsbcCapsuleDebugLib                       UsbcCapsuleDebugLibMock;
    UINT32                                        *RetimerDeviceInstancesCount;
    UINT32                                        *HrDeviceInstancesCount;
    HR_DEV_INSTANCE                               *HrDeviceInstances;
    RETIMER_DEV_INSTANCE                          *RetimerDeviceInstances;
    EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS Progress;
    USBC_PROGRESS_CODE_PROTOCOL                   *UsbCProgressCodeProtocol;
    SHOW_PROGRESS_CODE                            ShowProgressCode;
    MockBasePcdLib                                BasePcdLibMock;
    MockUsbCProgressCodeProtocolLib               UsbCProgressCodeProtocolMock;

  typedef struct {
    PAYLOAD_HEADER    PayloadHeader;
    RETIMER_ITEM      PayloadItem;
  } RETIMER_PAYLOAD_DATA;

  RETIMER_PAYLOAD_DATA MockRetimerImage = {
    {
      SIGNATURE_32 ('R', 'T', 'M', 'R'),          //  Signature         // RETIMER_PAYLOAD_HEADER_SIGNATURE
      0x30,                                       //  HeaderSize
      1,                                          //  RetimerCount
      0                                           //  Reserved
    },                                            //  RETIMER_PAYLOAD_HEADER
    {
      {
        0xFF,                                     //  Bus
        0xFF,                                     //  Device
        0xFF,                                     //  Function
        0x00,                                     //  Port
        0x01,                                     //  RetimerIndex
        { 0, 0, 0, 0, 0, 0, 0, 0 }                //  Reserved[8]
      },                                          //  RetimerDevAddress // RETIMER_DEV_ADDRESS
      0x30,                                       //  ImageOffset
      0x04,                                       //  ImageSize
      0x01,                                       //  FirmwareType
      0x00,                                       //  PcieRpType
      0x09,                                       //  PcieRootPort
      { 0, 0, 0, 0, 0 }                           //  Reserved[5]
    }                                             //  RETIMER_ITEM
  };

  RETIMER_PAYLOAD_DATA MockIntegratedRetimerImage = {
    {
      SIGNATURE_32 ('R', 'T', 'M', 'R'),          //  Signature         // RETIMER_PAYLOAD_HEADER_SIGNATURE
      0x30,                                       //  HeaderSize
      1,                                          //  RetimerCount
      0                                           //  Reserved
    },                                            //  RETIMER_PAYLOAD_HEADER
    {
      {
        0xFF,                                     //  Bus
        0xFF,                                     //  Device
        0xFF,                                     //  Function
        0x00,                                     //  Port
        0x01,                                     //  RetimerIndex
        { 0, 0, 0, 0, 0, 0, 0, 0 }                //  Reserved[8]
      },                                          //  RetimerDevAddress  // RETIMER_DEV_ADDRESS
      0x30,                                       //  ImageOffset
      0x04,                                       //  ImageSize
      0x00,                                       //  FirmwareType
      0x00,                                       //  PcieRpType
      0x09,                                       //  PcieRootPort
      { 0, 0, 0, 0, 0 }                           //  Reserved[5]
    }                                             //  RETIMER_ITEM
  };

  RETIMER_PAYLOAD_DATA MockUnsupportedRetimerImage = {
    {
      SIGNATURE_32 ('R', 'T', 'M', 'R'),          //  Signature          // RETIMER_PAYLOAD_HEADER_SIGNATURE
      0x30,                                       //  HeaderSize
      1,                                          //  RetimerCount
      0                                           //  Reserved
    },                                            //  RETIMER_PAYLOAD_HEADER
    {
      {
        0xFF,                                     //  Bus
        0xFF,                                     //  Device
        0xFF,                                     //  Function
        0x00,                                     //  Port
        0x01,                                     //  RetimerIndex
        { 0, 0, 0, 0, 0, 0, 0, 0 }                //  Reserved[8]
      },                                          //  RetimerDevAddress  // RETIMER_DEV_ADDRESS
      0x30,                                       //  ImageOffset
      0x04,                                       //  ImageSize
      0x02,                                       //  FirmwareType
      0x00,                                       //  PcieRpType
      0x09,                                       //  PcieRootPort
      { 0, 0, 0, 0, 0 }                           //  Reserved[5]
    }                                             //  RETIMER_ITEM
  };

  void SetUp() override {
    Image                                      = (VOID *) &MockRetimerImage;
    Image_Unsupport                            = (VOID *) &MockUnsupportedRetimerImage;
    Image_Integrated                           = (VOID *) &MockIntegratedRetimerImage;
    ImageSize                                  = sizeof (MockRetimerImage);
    VendorCode                                 = NULL;
    CapsuleFwVersion                           = 0;
    AbortReason                                = NULL;
    LastAttemptStatus                          = (UINT32 *) AllocateZeroPool (sizeof (LastAttemptStatus));
    RetimerPayloadHeader                       = (PAYLOAD_HEADER *) Image;
    RetimerPayloadItem                         = (RETIMER_ITEM *) (RetimerPayloadHeader + 1);
    DeviceHandleBuffer                         = (EFI_HANDLE *) AllocateZeroPool (sizeof (EFI_HANDLE));
    DeviceHandleCount                          = 0x01;
    HrDeviceInstances                          = (HR_DEV_INSTANCE *) AllocateZeroPool (sizeof (HrDeviceInstances));
    RetimerDeviceInstances                     = (RETIMER_DEV_INSTANCE *) AllocateZeroPool (sizeof (RetimerDeviceInstances));
    RetimerDeviceInstancesCount                = (UINT32 *) AllocateZeroPool (sizeof (RetimerDeviceInstancesCount));
    HrDeviceInstancesCount                     = (UINT32 *) AllocateZeroPool (sizeof (HrDeviceInstancesCount));
    UsbCProgressCodeProtocol                   = (USBC_PROGRESS_CODE_PROTOCOL *) AllocateZeroPool (sizeof (USBC_PROGRESS_CODE_PROTOCOL));
    UsbCProgressCodeProtocol->ShowProgressCode = ShowProgressCode;

    std::memcpy (&ShowProgressCode, &localUSBCPp, sizeof (SHOW_PROGRESS_CODE));
    std::memcpy (&UsbCRetimerProtocol, &LocalURP, sizeof (USBC_RETIMER_PROTOCOL));
    std::memcpy (&Progress, &LocalProgress, sizeof (EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS));
  }
};

TEST_F (FmpDeviceSetImageTest, Var_1_Error) {
  //
  // Case 1 - Image is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceSetImageWithStatus (NULL, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Var_2_Error) {
  //
  // Case 2 - Progress is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = FmpDeviceSetImageWithStatus (Image, ImageSize, VendorCode, NULL, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Var_3_Error) {
  //
  // Case 3 - Progress is Failed , gBS->LocateProtocol return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = FmpDeviceSetImageWithStatus (Image, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Var_4_Error) {
  //
  // Case 4 - Call ConstructRetimerInstances return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 4 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&LocalURP),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode);

  // For ConstructRetimerInstances >>
  ImageSize = RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize - 0x100;

  EXPECT_CALL (TbtNvmRetimerUpdateMock, CreateRetimerDevInstance (0x01, 0x00, 0x09, _, _))
    .WillOnce (Return (EFI_INVALID_PARAMETER));
  // For ConstructRetimerInstances <<

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Var_5_UnSupported) {
  //
  // Case 5 - DriveToFwUpdateMode return error ,goto RestorePdPowerMode
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock / UsbcRetimerProtocolMock
  // Expected Result - Status will return EFI_UNSUPPORTED
  //
  cout << "[---------- Case 5 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&LocalURP),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (5);

  // For ConstructRetimerInstances >>>
  ImageSize = RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize - 0x100;

  EXPECT_CALL (TbtNvmRetimerUpdateMock, CreateHrDevInstance (_, _, _, _))
    .WillOnce (Return (EFI_SUCCESS));
  // For ConstructRetimerInstances <<<

  EXPECT_CALL (BasePcdLibMock, LibPcdGet32);

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    DriveToFwUpdateMode
    )
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    RestoreToOriginalMode
    )
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_Unsupport, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_UNSUPPORTED);
}

TEST_F (FmpDeviceSetImageTest, Var_6_Success) {
  //
  // Case 6 - Success Case
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / MicroSecondDelayMock / TbtNvmRetimerUpdateMock / UsbcRetimerProtocolMock
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 6 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&LocalURP),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (7);

  // For ConstructRetimerInstances >>>
  ImageSize = RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize - 0x100;

  EXPECT_CALL (TbtNvmRetimerUpdateMock, CreateHrDevInstance (_, _, _, _))
    .WillOnce (Return (EFI_SUCCESS));

  Image = (VOID *) &MockRetimerImage;
  // For ConstructRetimerInstances <<<

  EXPECT_CALL (BasePcdLibMock, LibPcdGet32);

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    DriveToFwUpdateMode
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock, MicroSecondDelay);

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_RetimerFP
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    RestoreToOriginalMode
    )
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_RetimerFP (0)
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_Unsupport, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);
}

TEST_F (FmpDeviceSetImageTest, Var_7_Error) {
  //
  // Case 7 - TbtSendOfflineMode return error , goto OfflineModeExit and TbtSendOfflineMode then return Success.
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 7 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&LocalURP),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (3);

  // For ConstructRetimerInstances >>>
  ImageSize = RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize - 0x100;

  EXPECT_CALL (TbtNvmRetimerUpdateMock, CreateHrDevInstance (_, _, _, _))
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HrDeviceInstancesCount, sizeof (HrDeviceInstancesCount)),
         Return (EFI_SUCCESS)
         )
       );
  // For ConstructRetimerInstances <<<

  EXPECT_CALL (TbtNvmRetimerUpdateMock, TbtSendOfflineMode)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  HrDeviceInstancesCount = (UINT32 *) 0x01;

  EXPECT_CALL (TbtNvmRetimerUpdateMock, TbtSendOfflineMode (_, 1))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_Unsupport, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);
}

TEST_F (FmpDeviceSetImageTest, Var_8_Error) {
  //
  // Case 8 - UsbcRetimerProtocol_RetimerFP return error , goto DeAssertRetimerForcePowerGpio and RestoreToOriginalMode then return error, finally return success.
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / MicroSecondDelayMock / TbtNvmRetimerUpdateMock / UsbcRetimerProtocolMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 8 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_SUCCESS));

   EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&LocalURP),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (7);

  // For ConstructRetimerInstances >>>
  ImageSize = RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize - 0x100;

  EXPECT_CALL (TbtNvmRetimerUpdateMock, CreateHrDevInstance (_, _, _, _))
    .WillOnce (Return (EFI_SUCCESS));

  Image = (VOID *) &MockRetimerImage;
  // For ConstructRetimerInstances <<<

  EXPECT_CALL (BasePcdLibMock, LibPcdGet32);

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    DriveToFwUpdateMode
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock, MicroSecondDelay);

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_RetimerFP
    )
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_RetimerFP (0)
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    RestoreToOriginalMode
    )
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_Unsupport, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);
}

TEST_F (FmpDeviceSetImageTest, Var_9_Error) {
  //
  // Case 9 - gBS_LocateHandleBuffer return error , goto ConnectHc return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 9 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&LocalURP),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode);

  // For ConstructRetimerInstances >>>
  ImageSize = RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize - 0x100;

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    CreateRetimerDevInstance
    )
    .WillOnce (
       DoAll (
         SetArgPointee<4>(RetimerDevice),
         Return (EFI_SUCCESS)
         )
       );

  Image = (VOID *) &MockRetimerImage;
  // For ConstructRetimerInstances <<<

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&DeviceHandleCount, sizeof (DeviceHandleCount)),
         SetArgPointee<4>(DeviceHandleBuffer),
         Return (EFI_INVALID_PARAMETER)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_ConnectController);

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  RetimerDeviceInstancesCount = (UINT32 *) 0x01;

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Var_10_Error) {
  //
  // Case 10 - UsbcRetimerProtocol_RetimerFP return error , goto ConnectHc
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock  / UsbcRetimerProtocolMock / UsbcCapsuleDebugLibMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 10 ----------]"<< endl;
  RetimerDevice = (UINT32 *) 0x01;

  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&LocalURP),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (8);

  // For ConstructRetimerInstances >>>
  ImageSize = RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize - 0x100;

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    CreateRetimerDevInstance
    )
    .WillOnce (
       DoAll (
         SetArgPointee<4>(RetimerDevice),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (TbtNvmRetimerUpdateMock, CreateHrDevInstance (_, _, _, _))
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&RetimerDeviceInstancesCount, sizeof (RetimerDeviceInstancesCount)),
         Return (EFI_SUCCESS)
         )
       );

  Image = (VOID *) &MockRetimerImage;
  // For ConstructRetimerInstances <<<

  EXPECT_CALL (BasePcdLibMock, LibPcdGet32);

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&DeviceHandleCount, sizeof (DeviceHandleCount)),
         SetArgPointee<4>(DeviceHandleBuffer),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_DisconnectController);

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    DriveToFwUpdateMode
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (MicroSecondDelayMock, MicroSecondDelay);

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_RetimerFP (_)
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerUpdateMock, InitRetimerHW (_))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerUpdateMock, UpdateRetimerNvmFirmware)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerUpdateMock, ReadRetimerNvmVersion)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmRetimerUpdateMock, UpdateRetimerNvmInformation);

  EXPECT_CALL (TbtNvmRetimerUpdateMock, TerminateRetimerHW)
    .WillOnce (Return (EFI_DEVICE_ERROR));

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_RetimerFP (0)
    )
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    RestoreToOriginalMode
    )
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_ConnectController);

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  RetimerDeviceInstancesCount = (UINT32 *) 0x01;

  EXPECT_CALL (TbtNvmRetimerUpdateMock, DestroyRetimerDevInstance);

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

  cout << "FmpDeviceSetImage Done." << endl;
}
