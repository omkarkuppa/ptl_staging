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
    PAYLOAD_HEADER    PayloadHeader;
    RETIMER_ITEM      PayloadItem;
  } RETIMER_PAYLOAD_DATA;

  RETIMER_PAYLOAD_DATA MockRetimerImage = {
    {
      SIGNATURE_32 ('R', 'T', 'M', 'R'),          //  Signature    // RETIMER_PAYLOAD_HEADER_SIGNATURE
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

  void SetUp() override {
    Image                                       = (VOID *) &MockRetimerImage;
    ImageSize                                   = sizeof (MockRetimerImage);
    ImageUpdatable                              = (UINT32 *) AllocateZeroPool (sizeof (ImageUpdatable));
    MockRetimerImage.PayloadHeader.Signature    =  RETIMER_PAYLOAD_HEADER_SIGNATURE;
    MockRetimerImage.PayloadHeader.PayloadCount =  0;
  }
};

TEST_F (FmpDeviceCheckImageTest, Var_1_Error) {
  //
  // Case 1 - ImageUpdatable is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceCheckImage (Image, ImageSize, NULL);
  ASSERT_EQ (EFI_INVALID_PARAMETER, Status);
}

TEST_F (FmpDeviceCheckImageTest, Var_2_Error) {
  //
  // Case 2 - Image is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = FmpDeviceCheckImage (NULL, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_INVALID_PARAMETER, Status);
}

TEST_F (FmpDeviceCheckImageTest, Var_3_Error) {
  //
  // Case 3 - ImageSize < sizeof (PAYLOAD_HEADER)
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  ImageSize = (sizeof (PAYLOAD_HEADER) - sizeof (UINT8));
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_INVALID_PARAMETER, Status);
}

TEST_F (FmpDeviceCheckImageTest, Var_4_Error) {
  //
  // Case 4 - Image signature is DTBT
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 4 ----------]"<< endl;
  MockRetimerImage.PayloadHeader.Signature =  DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_INVALID_PARAMETER, Status);
}

TEST_F (FmpDeviceCheckImageTest, Var_5_Error) {
  //
  // Case 5 - PayloadCount is 0
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 5 ----------]"<< endl;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_INVALID_PARAMETER, Status);
}

TEST_F (FmpDeviceCheckImageTest, Var_6_Error) {
  //
  // Case 6 - ImageSize less than sizeof PAYLOAD_HEADER add Retimer Item
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 6 ----------]"<< endl;
  MockRetimerImage.PayloadHeader.PayloadCount =  1;
  ImageSize = ((sizeof (PAYLOAD_HEADER) + \
              (MockRetimerImage.PayloadHeader.PayloadCount * sizeof (RETIMER_ITEM))) - sizeof (UINT8));
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_INVALID_PARAMETER, Status);
}

TEST_F (FmpDeviceCheckImageTest, Var_7_Error) {
  //
  // Case 7 - ImageSize less than sizeof PAYLOAD_HEADER
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 7 ----------]"<< endl;
  MockRetimerImage.PayloadHeader.PayloadCount =  1;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_INVALID_PARAMETER, Status);
}

TEST_F (FmpDeviceCheckImageTest, Var_8_Success) {
  //
  // Case 8 - Success Case
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 8 ----------]"<< endl;
  ImageSize = MockRetimerImage.PayloadItem.ImageOffset + \
              MockRetimerImage.PayloadItem.ImageSize;
  MockRetimerImage.PayloadHeader.PayloadCount =  1;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_SUCCESS, Status);

  cout << "FmpDeviceCheckImageWithStatus Done." << endl;
}
