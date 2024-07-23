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
Google test for FmpDeviceCheckImage function.
**/

class FmpDeviceCheckImageTest : public CommonMock {
  protected:
  EFI_STATUS     Status;
  VOID          *Image;
  UINTN          ImageSize;
  UINT32        *ImageUpdatable;

  void SetUp() override {
    Image = (VOID *) &MockDtbtImage;
    ImageSize = sizeof (MockDtbtImage);
    ImageUpdatable = (UINT32 *) AllocateZeroPool (sizeof (ImageUpdatable));
    MockDtbtImage.PayloadHeader.Signature =  DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE;
    MockDtbtImage.PayloadHeader.PayloadCount =  0;
  }
};

TEST_F (FmpDeviceCheckImageTest, VarError_1) {

  //
  // Case 1 - ImageUpdatable is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = FmpDeviceCheckImage (Image, ImageSize, NULL);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceCheckImageTest, VarError_2) {

  //
  // Case 2 - Image is NULL
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = FmpDeviceCheckImage (NULL, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceCheckImageTest, VarError_3) {

  //
  // Case 3 - Image size is too small
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;
  ImageSize = (sizeof (PAYLOAD_HEADER) - sizeof (UINT8));
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceCheckImageTest, VarError_4) {

  //
  // Case 4 - Image signature is DTBT
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 4 ----------]"<< endl;
  MockDtbtImage.PayloadHeader.Signature =  DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceCheckImageTest, VarError_5) {

  //
  // Case 5 - PayloadCount is 0
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 5 ----------]"<< endl;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceCheckImageTest, Var_UnSupported) {

  //
  // Case 6 - PayloadCount is greater than 1
  // Expected Result - Status will return EFI_UNSUPPORTED
  //
  cout << "[---------- Case 6 ----------]"<< endl;
  MockDtbtImage.PayloadHeader.PayloadCount =  2;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_UNSUPPORTED);

}

TEST_F (FmpDeviceCheckImageTest, VarError_6) {

  //
  // Case 7 - ImageSize less than sizeof PAYLOAD_HEADER add Retimer Item
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 7 ----------]"<< endl;
  MockDtbtImage.PayloadHeader.PayloadCount =  1;
  ImageSize = ((sizeof (PAYLOAD_HEADER) + \
               (MockDtbtImage.PayloadHeader.PayloadCount * sizeof (DISCRETE_TBT_ITEM))) - sizeof (UINT8));
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceCheckImageTest, VarError_7) {

  //
  // Case 8 - ImageSize less than sizeof PAYLOAD_HEADER
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 8 ----------]"<< endl;
  MockDtbtImage.PayloadHeader.PayloadCount =  1;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

}

TEST_F (FmpDeviceCheckImageTest, VarSuccess) {

  //
  // Case 9 - Success Case
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 9 ----------]"<< endl;
  ImageSize = MockDtbtImage.PayloadItem.ImageOffset + \
              MockDtbtImage.PayloadItem.ImageSize;
  MockDtbtImage.PayloadHeader.PayloadCount =  1;
  Status = FmpDeviceCheckImage (Image, ImageSize, ImageUpdatable);
  ASSERT_EQ (EFI_SUCCESS, Status);

  cout << "FmpDeviceCheckImageWithStatus Done." << endl;

}
