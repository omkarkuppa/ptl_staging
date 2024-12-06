/** @file
  Google test for UsbCPdBridgeUpdateLib function.

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

@par Specification
**/

#include <GTestUsbCPdBridgeUpdateLib.h>
#include <GoogleTest/Private/MockPdBridgeProtocol/MockPdBridgeProtocol.h>
#include <GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.h>
/**
Google test for UsbCPdBridgeUpdateLib function.
**/

class UpdatePdBridgeNvmFirmwareTest : public Test {
  protected:
    EFI_STATUS                                    Status;
    UINT8                                         *PDBridgeImage;
    UINT8                                         *PDBridgeImage_1;
    UINT8                                         PdCntrlIndex;
    UINT8                                         ShareFlashMode;
    UINTN                                         ImageSize;
    UINTN                                         StartPercentage;
    UINTN                                         EndPercentage;
    UINT32                                        RecoveryCount;
    USBC_PD_BRIDGE_PROTOCOL                       PdBridgeProtocol;
    EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS Progress;

    MockFirmwareManagementLib FirmwareManagementMock;
    MockPdBridgeProtocol      PdBridgeProtocolMock;

  typedef struct {
    PAYLOAD_HEADER            PayloadHeader;
    PD_BRIDGE_PAYLOAD_ITEM    PayloadItem;
  } PD_BRIDGE_PAYLOAD_DATA;

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
        0x01,                                           //  ShareFlashMode
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    //  Reserved[14]
      },
      0x30,                                             //  ImageOffset
      0x30,                                             //  ImageSize
      0x01,                                             //  FirmwareType
      { 0, 0, 0 }                                       //  Reserved[3]
    }
  };

  PD_BRIDGE_PAYLOAD_DATA MockPdbgImage_1 = {};

  void SetUp() override {
    PdCntrlIndex    = 0;
    ShareFlashMode  = 0;
    RecoveryCount   = 0;
    PDBridgeImage   = ((UINT8 *) &MockPdbgImage) + MockPdbgImage.PayloadItem.ImageOffset;
    PDBridgeImage_1 = ((UINT8 *) &MockPdbgImage_1) ;
    ImageSize       = MockPdbgImage.PayloadItem.ImageSize;
    StartPercentage = (6 + (90 / MockPdbgImage.PayloadHeader.PayloadCount) * 0);
    EndPercentage   = (6 + (90 / MockPdbgImage.PayloadHeader.PayloadCount) * 1);
    std::memcpy (&Progress, &LocalProgress, sizeof (EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS));
    std::memcpy (&PdBridgeProtocol, &localPDBG, sizeof (USBC_PD_BRIDGE_PROTOCOL));
  }
};

//
// Case 1 - (PDBridgeImage == NULL)
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (UpdatePdBridgeNvmFirmwareTest, case_1) {
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = UpdatePdBridgeNvmFirmware (
             &localPDBG,
             NULL,
             ImageSize,
             PdCntrlIndex,
             ShareFlashMode,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - NvmSetOffset return error , RecoveryCount >= PD_BRIDGE_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE , goto UpdateImageComplete
// Mock - PdBridgeProtocolMock / FirmwareManagementMock
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (UpdatePdBridgeNvmFirmwareTest, case_2) {
  cout << "[---------- Case 2 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (FirmwareManagementMock, Progress (_))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdatePdBridgeNvmFirmware (
             &localPDBG,
             PDBridgeImage,
             ImageSize,
             PdCntrlIndex,
             ShareFlashMode,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 3 - NvmWrite return error ,RecoveryCount >= PD_BRIDGE_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE , goto UpdateImageComplete
// Mock - PdBridgeProtocolMock / FirmwareManagementMock
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (UpdatePdBridgeNvmFirmwareTest, case_3) {
  cout << "[---------- Case 3 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (FirmwareManagementMock, Progress (_))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdatePdBridgeNvmFirmware (
             &localPDBG,
             PDBridgeImage,
             ImageSize,
             PdCntrlIndex,
             ShareFlashMode,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 4 - NvmWrite return correct , NvmStallNvmAccess return error
// Mock - PdBridgeProtocolMock / FirmwareManagementMock
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (UpdatePdBridgeNvmFirmwareTest, case_4) {
  cout << "[---------- Case 4 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (FirmwareManagementMock, Progress (_))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdatePdBridgeNvmFirmware (
             &localPDBG,
             PDBridgeImage_1,
             ImageSize,
             PdCntrlIndex,
             1,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 5 - NvmWrite return correct , NvmStallNvmAccess return correct , NvmAuthenticateWrite return error
// Mock - PdBridgeProtocolMock / FirmwareManagementMock
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (UpdatePdBridgeNvmFirmwareTest, case_5) {
  cout << "[---------- Case 5 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (FirmwareManagementMock, Progress (_))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdatePdBridgeNvmFirmware (
             &localPDBG,
             PDBridgeImage_1,
             ImageSize,
             PdCntrlIndex,
             1,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 6 - NvmWrite return correct , NvmStallNvmAccess return correct , NvmAuthenticateWrite return correct , NvmReset return error
// Mock - PdBridgeProtocolMock / FirmwareManagementMock
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (UpdatePdBridgeNvmFirmwareTest, case_6) {
  cout << "[---------- Case 6 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (FirmwareManagementMock, Progress (_))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdatePdBridgeNvmFirmware (
             &localPDBG,
             PDBridgeImage_1,
             ImageSize,
             PdCntrlIndex,
             1,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 7 - ShareFlashMode != SHARE_FLASH_MODE_ENABLE , NvmAuthenticateWrite return correct
// Mock - PdBridgeProtocolMock / FirmwareManagementMock
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (UpdatePdBridgeNvmFirmwareTest, case_7) {
  cout << "[---------- Case 7 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (FirmwareManagementMock, Progress (_))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdatePdBridgeNvmFirmware (
             &localPDBG,
             PDBridgeImage_1,
             ImageSize,
             PdCntrlIndex,
             2,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_SUCCESS);
}

//
// Case 8 - ShareFlashMode != SHARE_FLASH_MODE_ENABLE , NvmAuthenticateWrite return error
// Mock - PdBridgeProtocolMock / FirmwareManagementMock
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (UpdatePdBridgeNvmFirmwareTest, case_8) {
  cout << "[---------- Case 8 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (FirmwareManagementMock, Progress (_))
    .WillOnce (Return (EFI_SUCCESS));

  Status = UpdatePdBridgeNvmFirmware (
             &localPDBG,
             PDBridgeImage_1,
             ImageSize,
             PdCntrlIndex,
             2,
             Progress,
             StartPercentage,
             EndPercentage
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);

  cout << "UpdatePdBridgeNvmFirmware Done." << endl;
}
