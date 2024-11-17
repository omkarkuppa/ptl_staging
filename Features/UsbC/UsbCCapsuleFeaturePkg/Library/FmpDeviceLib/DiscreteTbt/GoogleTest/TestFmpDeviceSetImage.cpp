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
// GoogleTest mock
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Library/MockTbtNvmRetimerUpdateLib.h>
#include <GoogleTest/Library/MockUsbcCapsuleDebugLib.h>
#include <GoogleTest/Private/MockPciIopProtocolLib/MockPciIopProtocolLib.h>
#include <GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.h>
#include <GoogleTest/Private/MockUsbCProgressCodeProtocol/MockUsbCProgressCodeProtocol.h>

/**
Google test for FmpDeviceSetImage function.
**/
class FmpDeviceSetImageTest : public Test {
  protected:
    EFI_STATUS                                    Status;
    VOID                                          *Image;
    VOID                                          *Image_NonDtbt;
    UINTN                                         ImageSize;
    VOID                                          *VendorCode;
    UINT32                                        CapsuleFwVersion;
    CHAR16                                        **AbortReason;
    UINT32                                        *LastAttemptStatus;
    UINT32                                        OverSize;
    UINT32                                        LessSize;
    EFI_PCI_IO_PROTOCOL                           PciIopProtocol;
    PAYLOAD_HEADER                                *DiscreteTbtPayloadHeader;
    DISCRETE_TBT_ITEM                             *DiscreteTbtPayloadItem;
    EFI_HANDLE                                    *DeviceHandleBuffer;
    UINTN                                         DeviceHandleCount;
    DISCRETE_TBT_DEV_INSTANCE                     DiscreteTbtDevice;
    UINTN                                         BusNumber;
    EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS Progress;
    USBC_PROGRESS_CODE_PROTOCOL                   *UsbCProgressCodeProtocol;
    SHOW_PROGRESS_CODE                            ShowProgressCode;

    MockUsbCProgressCodeProtocolLib               UsbCProgressCodeProtocolMock;
    MockTbtNvmRetimerUpdateLib                    TbtNvmRetimerUpdateMock;
    MockPciIopProtocolLib                         PciIopProtocolMock;
    MockUefiBootServicesTableLib                  UefiBootServicesTableLibMock;
    MockFirmwareManagementLib                     FirmwareManagementMock;
    MockUsbcCapsuleDebugLib                       UsbcCapsuleDebugLibMock;

  typedef struct {
    PAYLOAD_HEADER       PayloadHeader;
    DISCRETE_TBT_ITEM    PayloadItem;
  } DISCRETE_TBT_PAYLOAD_DATA;

  DISCRETE_TBT_PAYLOAD_DATA MockNonDtbtImage = {
    {
      SIGNATURE_32 ('D', 'T', 'B', 'T'),        //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
      0x20,                                     //  HeaderSize
      1,                                        //  DiscreteTbtCount
      0                                         //  Reserved
    },                                          //  DISCRETE_TBT_PAYLOAD_HEADER
    {
      0x24,                                     //  ImageOffset
      0x04,                                     //  ImageSize
      0x07,                                     //  FirmwareType
      0x00,                                     //  PcieRpType
      0x19,                                     //  PcieRootPort
      { 0, 0, 0, 0, 0 }                         //  Reserved[5]
    }                                           //  DISCRETE_TBT_ITEM
  };

  DISCRETE_TBT_PAYLOAD_DATA MockDtbtImage = {
    {
      SIGNATURE_32 ('D', 'T', 'B', 'T'),        //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
      0x20,                                     //  HeaderSize
      1,                                        //  DiscreteTbtCount
      0                                         //  Reserved
    },                                          //  DISCRETE_TBT_PAYLOAD_HEADER
    {
      0x24,                                     //  ImageOffset
      0x04,                                     //  ImageSize
      0x02,                                     //  FirmwareType
      0x00,                                     //  PcieRpType
      0x19,                                     //  PcieRootPort
      { 0, 0, 0, 0, 0 }                         //  Reserved[5]
    }                                           //  DISCRETE_TBT_ITEM
  };

  void SetUp() override {
    Image                                      = (VOID *) &MockDtbtImage;
    Image_NonDtbt                              = (VOID *) &MockNonDtbtImage;
    ImageSize                                  = sizeof (MockDtbtImage);
    VendorCode                                 = NULL;
    CapsuleFwVersion                           = 0;
    AbortReason                                = NULL;
    LastAttemptStatus                          = (UINT32 *) AllocateZeroPool (sizeof (LastAttemptStatus));
    DiscreteTbtPayloadHeader                   = (PAYLOAD_HEADER *) Image;
    DiscreteTbtPayloadItem                     = (DISCRETE_TBT_ITEM *) (DiscreteTbtPayloadHeader + 1);
    OverSize                                   = DiscreteTbtPayloadItem->ImageOffset + DiscreteTbtPayloadItem->ImageSize + 0x05;
    LessSize                                   = DiscreteTbtPayloadItem->ImageOffset + DiscreteTbtPayloadItem->ImageSize - 0x05;
    DeviceHandleBuffer                         = (EFI_HANDLE *) AllocateZeroPool (sizeof (EFI_HANDLE));
    DeviceHandleCount                          = 0x01;
    UsbCProgressCodeProtocol                   = (USBC_PROGRESS_CODE_PROTOCOL *) AllocateZeroPool (sizeof (USBC_PROGRESS_CODE_PROTOCOL));
    UsbCProgressCodeProtocol->ShowProgressCode = ShowProgressCode;

    std::memcpy (&ShowProgressCode, &localUSBCPp, sizeof (SHOW_PROGRESS_CODE));
    std::memcpy (&Progress, &LocalProgress, sizeof (EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS));
    std::memcpy (&PciIopProtocol, &localPp, sizeof (EFI_PCI_IO_PROTOCOL));
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
  // Case 3 - Progress return error
  // Mock -  FirmwareManagementMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode);

  Status = FmpDeviceSetImageWithStatus (Image, LessSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Var_4_Error) {
  //
  // Case 4 - FirmwareType is not DISCRETE_TBT , Progress return error
  // Mock -  FirmwareManagementMock / UsbcCapsuleDebugLibMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 4 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode);

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_START, _, _));

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_INFO, _, _));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_NonDtbt, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);
}

TEST_F (FmpDeviceSetImageTest, Var_5_UnSupported) {
  //
  // Case 5 - FirmwareType is DISCRETE_TBT ,gBS->LocateHandleBuffer return error
  // Mock -  FirmwareManagementMock / UsbcCapsuleDebugLibMock / UefiBootServicesTableLibMock
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
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode);

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_START, _, _));

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_INFO, _, _));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer
    )
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  Status = FmpDeviceSetImageWithStatus (Image, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_UNSUPPORTED);
}

TEST_F (FmpDeviceSetImageTest, Var_6_UnSupported) {
  //
  // Case 6 - FirmwareType is DISCRETE_TBT ,gBS_HandleProtocol return error
  // Mock -  FirmwareManagementMock / UsbcCapsuleDebugLibMock / UefiBootServicesTableLibMock
  // Expected Result - Status will return EFI_UNSUPPORTED
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
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode);

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_START, _, _));

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_INFO, _, _));

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

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localPp),
         Return (EFI_INVALID_PARAMETER)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  Status = FmpDeviceSetImageWithStatus (Image, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Var_7_UnSupported) {
  //
  // Case 7 - FirmwareType is DISCRETE_TBT , CreateTBTDevInstance return error
  // Mock -  FirmwareManagementMock / UsbcCapsuleDebugLibMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock / PciIopProtocolMock
  // Expected Result - Status will return EFI_UNSUPPORTED
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
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (2);

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_START, _, _));

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_INFO, _, _));

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

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIopProtocolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&BusNumber, sizeof (BusNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_DisconnectController);

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_CREATE_DTBT_DEV_INST, _, _));

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    CreateTBTDevInstance (_, _, _, _)
    )
    .WillOnce (
       DoAll (
         SetArgPointee<3>(DiscreteTbtDevice),
         Return (EFI_UNSUPPORTED)
         )
       );

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_CREATE_DTBT_DEV_INST_FAIL_STATUS, _, _));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_UNSUPPORTED);
}

TEST_F (FmpDeviceSetImageTest, Var_8_Error) {
  //
  // Case 8 - FirmwareType is DISCRETE_TBT , UpdateDiscreteTbtNvmFirmware return error
  // Mock -  FirmwareManagementMock / UsbcCapsuleDebugLibMock / UefiBootServicesTableLibMock / TbtNvmRetimerUpdateMock / PciIopProtocolMock
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
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (3);

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_START, _, _));

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_INFO, _, _));

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

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIopProtocolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&BusNumber, sizeof (BusNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_DisconnectController);

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_CREATE_DTBT_DEV_INST, _, _));

  DiscreteTbtDevice = (DISCRETE_TBT_DEV_INSTANCE) AllocateZeroPool (sizeof (DISCRETE_TBT_DEV_INSTANCE));

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    CreateTBTDevInstance (_, _, _, _)
    )
    .WillOnce (
       DoAll (
         SetArgPointee<3>(DiscreteTbtDevice),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_UPDATE_DTBT_NVM_FW, _, _));

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    UpdateDiscreteTbtNvmFirmware
    )
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL ( TbtNvmRetimerUpdateMock, DestroyTbtDevInstance (_));

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite (_, USBC_DTBT_CAPSULE_EVT_CODE_UPDATE_DTBT_NVM_FW_FAIL_STATUS, _, _));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_UNSUPPORTED);
}
