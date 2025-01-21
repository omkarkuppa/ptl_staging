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
/**
Google test for FmpDeviceCheckImage function.
**/

class FmpDeviceCheckImageTest : public Test {
  protected:
    EFI_STATUS Status;
    VOID       *Image;
    UINTN      ImageSize;
    UINT32     *ImageUpdatable;

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
      0x04,                                             //  ImageSize
      0x01,                                             //  FirmwareType
      { 0, 0, 0 }                                       //  Reserved[3]
    }
  };

  void SetUp() override {
    Image                                    = (VOID *) &MockPdbgImage;
    ImageSize                                = sizeof (MockPdbgImage);
    ImageUpdatable                           = (UINT32 *) AllocateZeroPool (sizeof (ImageUpdatable));
    MockPdbgImage.PayloadHeader.Signature    =  PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE;
    MockPdbgImage.PayloadHeader.PayloadCount =  0;
  }
};

TEST_F (FmpDeviceCheckImageTest, Case_1) {
  //
  // Case 1 - ImageUpdatable is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceCheckImage (Image, ImageSize, NULL);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceCheckImageTest, Case_2) {
  //
  // Case 2 - Image is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = FmpDeviceCheckImage (NULL, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceCheckImageTest, Case_3) {
  //
  // Case 3 - Image size is too small
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  ImageSize = (sizeof (PAYLOAD_HEADER) - sizeof (UINT8));
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceCheckImageTest, Case_4) {
  //
  // Case 4 - PayloadCount is 0
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 4 ----------]"<< endl;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceCheckImageTest, Case_5) {
  //
  // Case 5 - ImageSize less than sizeof PAYLOAD_HEADER add PDBridge Item
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 5 ----------]"<< endl;
  MockPdbgImage.PayloadHeader.PayloadCount =  1;
  ImageSize = ((sizeof (PAYLOAD_HEADER) + \
              (MockPdbgImage.PayloadHeader.PayloadCount * sizeof (PD_BRIDGE_PAYLOAD_ITEM))) - sizeof (UINT8));
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceCheckImageTest, Case_6) {
  //
  // Case 6 - ImageSize less than sizeof PAYLOAD_HEADER
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 6 ----------]"<< endl;
  MockPdbgImage.PayloadHeader.PayloadCount =  1;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (FmpDeviceCheckImageTest, Case_7) {
  //
  // Case 7 - Success Case
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 7 ----------]"<< endl;
  ImageSize = MockPdbgImage.PayloadItem.ImageOffset + \
              MockPdbgImage.PayloadItem.ImageSize;
  MockPdbgImage.PayloadHeader.PayloadCount =  1;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_SUCCESS, Status);

  cout << "FmpDeviceCheckImageWithStatus Done." << endl;
}
