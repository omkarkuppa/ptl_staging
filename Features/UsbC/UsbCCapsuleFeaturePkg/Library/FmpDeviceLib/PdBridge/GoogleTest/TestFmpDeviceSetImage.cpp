/** @file
  Google Test for the implementation of FmpDeviceLib instance to support PD Bridge Firmware update

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <GTestFmpDeviceLibPdBridge.h>
// GoogleTest mock
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Library/MockUsbCPdBridgeUpdateLib.h>
#include <GoogleTest/Library/MockUsbcCapsuleDebugLib.h>
#include <GoogleTest/Private/MockPciIopProtocolLib/MockPciIopProtocolLib.h>
#include <GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.h>
#include <GoogleTest/Private/MockPdBridgeProtocol/MockPdBridgeProtocol.h>

/**
Google test for FmpDeviceSetImage function.
**/
class FmpDeviceSetImageTest : public Test {
  protected:
    EFI_STATUS                                    Status;
    VOID                                          *Image;
    VOID                                          *Image_1;
    VOID                                          *Image_2;
    VOID                                          *Image_NonPdbg;
    UINTN                                         ImageSize;
    VOID                                          *VendorCode;
    UINT32                                        CapsuleFwVersion;
    CHAR16                                        **AbortReason;
    UINT32                                        *LastAttemptStatus;
    UINT32                                        OverSize;
    UINT32                                        LessSize;
    EFI_PCI_IO_PROTOCOL                           PciIopProtocol;
    PAYLOAD_HEADER                                *PDBridgePayloadHeader;
    PD_BRIDGE_PAYLOAD_ITEM                        *PDBridgePayloadItem;
    EFI_HANDLE                                    *DeviceHandleBuffer;
    EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS Progress;
    USBC_PD_BRIDGE_PROTOCOL                       PdBridgeProtocol;

    MockPdBridgeProtocol                          PdBridgeProtocolMock;
    MockPciIopProtocolLib                         PciIopProtocolMock;
    MockUefiBootServicesTableLib                  UefiBootServicesTableLibMock;
    MockFirmwareManagementLib                     FirmwareManagementMock;
    MockUsbcCapsuleDebugLib                       UsbcCapsuleDebugLibMock;
    MockUsbCPdBridgeUpdateLib                     UsbCPdBridgeUpdateLibMock;
    MockUefiRuntimeServicesTableLib               UefiRuntimeServicesTableLibMock;

  typedef struct {
    PAYLOAD_HEADER            PayloadHeader;
    PD_BRIDGE_PAYLOAD_ITEM    PayloadItem;
  } PD_BRIDGE_PAYLOAD_DATA;

  PD_BRIDGE_PAYLOAD_DATA MockNonPdbgImage = {
    {
      SIGNATURE_32 ('P', 'D', 'B', 'G'),                //  Signature    // PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE
      29,                                               //  HeaderSize
      1,                                                //  RetimerCount
      0                                                 //  Reserved
    },                                                  //  PD_BRIDGE_PAYLOAD_HEADER
    {
      {
        0x00,                                           //  PdCntrlIndex
        0x01,                                           //  ShareFlashMode
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    //  Reserved[14]
      },
      0x30,                                             //  ImageOffset
      0x04,                                             //  ImageSize
      0x07,                                             //  FirmwareType
      { 0, 0, 0 }                                       //  Reserved[3]
    }
  };

  PD_BRIDGE_PAYLOAD_DATA MockPdbgImage = {
    {
      SIGNATURE_32 ('P', 'D', 'B', 'G'),                //  Signature    // PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE
      29,                                               //  HeaderSize
      1,                                                //  RetimerCount
      0                                                 //  Reserved
    },                                                  //  PD_BRIDGE_PAYLOAD_HEADER
    {
      {
        0x00,                                           //  PdCntrlIndex
        0x02,                                           //  ShareFlashMode
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    //  Reserved[14]
      },
      0x30,                                             //  ImageOffset
      0x04,                                             //  ImageSize
      0x03,                                             //  FirmwareType
      { 0, 0, 0 }                                       //  Reserved[3]
    }
  };

  PD_BRIDGE_PAYLOAD_DATA MockPdbgImage_1 = {
    {
      SIGNATURE_32 ('P', 'D', 'B', 'G'),                //  Signature    // PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE
      29,                                               //  HeaderSize
      1,                                                //  RetimerCount
      0                                                 //  Reserved
    },                                                  //  PD_BRIDGE_PAYLOAD_HEADER
    {
      {
        0x00,                                           //  PdCntrlIndex
        0x01,                                           //  ShareFlashMode
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    //  Reserved[14]
      },
      0x30,                                             //  ImageOffset
      0x04,                                             //  ImageSize
      0x03,                                             //  FirmwareType
      { 0, 0, 0 }                                       //  Reserved[3]
    }
  };

  PD_BRIDGE_PAYLOAD_DATA MockPdbgImage_2 = {
    {
      SIGNATURE_32 ('P', 'D', 'B', 'G'),                //  Signature    // PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE
      29,                                               //  HeaderSize
      1,                                                //  RetimerCount
      0                                                 //  Reserved
    },                                                  //  PD_BRIDGE_PAYLOAD_HEADER
    {
      {
        0x00,                                           //  PdCntrlIndex
        0x00,                                           //  ShareFlashMode
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    //  Reserved[14]
      },
      0x30,                                             //  ImageOffset
      0x04,                                             //  ImageSize
      0x03,                                             //  FirmwareType
      { 0, 0, 0 }                                       //  Reserved[3]
    }
  };

  void SetUp() override {
    Image                 = (VOID *) &MockPdbgImage;
    Image_1               = (VOID *) &MockPdbgImage_1;
    Image_2               = (VOID *) &MockPdbgImage_2;
    Image_NonPdbg         = (VOID *) &MockNonPdbgImage;
    ImageSize             = sizeof (MockPdbgImage);
    VendorCode            = NULL;
    CapsuleFwVersion      = 0;
    AbortReason           = NULL;
    LastAttemptStatus     = (UINT32 *) AllocateZeroPool (sizeof (LastAttemptStatus));
    PDBridgePayloadHeader = (PAYLOAD_HEADER *) Image;
    PDBridgePayloadItem   = (PD_BRIDGE_PAYLOAD_ITEM *) (PDBridgePayloadHeader + 1);
    OverSize              = PDBridgePayloadItem->ImageOffset + PDBridgePayloadItem->ImageSize + 0x05;
    LessSize              = PDBridgePayloadItem->ImageOffset + PDBridgePayloadItem->ImageSize - 0x05;
    DeviceHandleBuffer    = (EFI_HANDLE *) AllocateZeroPool (sizeof (EFI_HANDLE));
    std::memcpy (&PdBridgeProtocol, &localPDBG, sizeof (USBC_PD_BRIDGE_PROTOCOL));
    std::memcpy (&Progress, &LocalProgress, sizeof (EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS));
    std::memcpy (&PciIopProtocol, &localPp, sizeof (EFI_PCI_IO_PROTOCOL));
  }
};

TEST_F (FmpDeviceSetImageTest, Case_1) {
  //
  // Case 1 - Image is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceSetImageWithStatus (NULL, ImageSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Case_2) {
  //
  // Case 2 - Progress is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = FmpDeviceSetImageWithStatus (Image, ImageSize, VendorCode, NULL, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Case_3) {
  //
  // Case 3 - Progress (5) return error
  // Mock - FirmwareManagementMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  EXPECT_CALL (FirmwareManagementMock, Progress (5))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image, LessSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Case_4) {
  //
  // Case 4 - gBS->LocateProtocol return error
  // Mock - FirmwareManagementMock / UefiBootServicesTableLibMock
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
         SetArgPointee<2>(&localPDBG),
         Return (EFI_INVALID_PARAMETER)
         )
       );

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_NonPdbg, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceSetImageTest, Case_5) {
  //
  // Case 5 - PDBridgePayloadItem->FirmwareType != PD_BRIDGE
  // Mock - FirmwareManagementMock / UefiBootServicesTableLibMock / PdBridgeProtocolMock
  // Expected Result - Status will return EFI_SUCCESS
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
         SetArgPointee<2>(&localPDBG),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_Lock)
    .Times (2);

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_NonPdbg, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);
}

TEST_F (FmpDeviceSetImageTest, Case_6) {
  //
  // Case 6 - (PdBridgePayloadItem->ImageOffset + PdBridgePayloadItem->ImageSize) > ImageSize
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / PdBridgeProtocolMock
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
         SetArgPointee<2>(&localPDBG),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_Lock)
    .Times (2);

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_NonPdbg, LessSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);
}

TEST_F (FmpDeviceSetImageTest, Case_7) {
  //
  // Case 7 - PDBridgePayloadItem->PrivateData.PDBridge.ShareFlashMode > SHARE_FLASH_MODE_ENABLE
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / PdBridgeProtocolMock
  // Expected Result - Status will return EFI_SUCCESS
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
         SetArgPointee<2>(&localPDBG),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_Lock)
    .Times (2);

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);
}

TEST_F (FmpDeviceSetImageTest, Case_8) {
  //
  // Case 8 - UpdatePDBridgeNvmFirmware return error
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / PdBridgeProtocolMock / UsbCPdBridgeUpdateLibMock
  // Expected Result - Status will return EFI_SUCCESS
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
         SetArgPointee<2>(&localPDBG),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_Lock)
    .Times (2);

  EXPECT_CALL (UsbCPdBridgeUpdateLibMock, UpdatePdBridgeNvmFirmware)
    .WillOnce (Return (EFI_UNSUPPORTED));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_1, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);
}

TEST_F (FmpDeviceSetImageTest, Case_9) {
  //
  // Case 9 - PdBridgeProtocol->GetVersion return EFI_SUCCESS , goto UpdatePdBridgeVersion
  // Mock -  FirmwareManagementMock / UefiBootServicesTableLibMock / PdBridgeProtocolMock / UsbCPdBridgeUpdateLibMock / UefiRuntimeServicesTableLibMock
  // Expected Result - Status will return EFI_SUCCESS
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
         SetArgPointee<2>(&localPDBG),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_Lock)
    .Times (2);

  EXPECT_CALL (UsbCPdBridgeUpdateLibMock, UpdatePdBridgeNvmFirmware)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_GetVersion)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_GetVariable)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (FirmwareManagementMock, Progress (100));

  Status = FmpDeviceSetImageWithStatus (Image_1, OverSize, VendorCode, Progress, CapsuleFwVersion, AbortReason, LastAttemptStatus);
  ASSERT_EQ (Status, EFI_SUCCESS);

  cout << "FmpDeviceSetImage Done." << endl;
}
