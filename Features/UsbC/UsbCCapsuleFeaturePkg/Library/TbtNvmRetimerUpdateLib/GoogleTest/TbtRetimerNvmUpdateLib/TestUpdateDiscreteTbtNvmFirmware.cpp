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

#include <GTestTbtRetimerNvmUpdateLib.h>
#include <GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.h>

struct MockTbtNvmRetimerDrvHelpers {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmRetimerDrvHelpers);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvDeviceExecAuth,
    (IN VOID   *DevCom)
    );
   MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvDeviceWrBlk,
    (IN VOID           *DevCom,
     UINT32            *Data,
     UINT8             Length)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvDeviceWrOffset,
    (IN VOID   *DevCom,
     IN UINT32 OffsetInDW)
    );
};
MOCK_INTERFACE_DEFINITION(MockTbtNvmRetimerDrvHelpers);
MOCK_FUNCTION_INTERNAL_DEFINITION(MockTbtNvmRetimerDrvHelpers, TbtNvmDrvDeviceWrBlk,    3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION(MockTbtNvmRetimerDrvHelpers, TbtNvmDrvDeviceWrOffset, 2, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION(MockTbtNvmRetimerDrvHelpers, TbtNvmDrvDeviceExecAuth, 1, EFIAPI);

typedef struct {
  PAYLOAD_HEADER       PayloadHeader;
  DISCRETE_TBT_ITEM    PayloadItem;
  UINT32               ImageContent;
} MOCK_DISCRETE_TBT_PAYLOAD_DATA;

// **********************************************************
// UpdateDiscreteTbtNvmFirmware Unit Test                   *
// **********************************************************
class UpdateDiscreteTbtNvmFirmwareTest : public Test {
  protected:
    EFI_STATUS                                    Status;
    DISCRETE_TBT_DEV_INSTANCE                     DiscreteTbtDevice;
    UINT8                                         *DiscreteTbtImage;
    UINTN                                         ImageSize;
    UINTN                                         StartPercentage;
    UINTN                                         EndPercentage;
    TBT_HOST_ROUTER                               *HrPtr;
    MockTbtNvmRetimerDrvHelpers                   TbtNvmRetimerDrvHelpersMock;
    MockFirmwareManagementLib                     FirmwareManagementMock;
    EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS Progress;

    MOCK_DISCRETE_TBT_PAYLOAD_DATA MockDtbtImage = {
      {
        SIGNATURE_32 ('D', 'T', 'B', 'T'),          //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
        0x20,                                       //  HeaderSize
        1,                                          //  DiscreteTbtCount
        0                                           //  Reserved
      },                                            //  DISCRETE_TBT_PAYLOAD_HEADER
      {
        0x20,                                       //  ImageOffset
        0x04,                                       //  ImageSize
        0x02,                                       //  FirmwareType
        0x00,                                       //  PcieRpType
        0x19,                                       //  PcieRootPort
        { 0, 0, 0, 0, 0 }                           //  Reserved[5]
      },                                            //  DISCRETE_TBT_ITEM
      0x00000000                                    //  ImageContent       // The value use to BufferPointer[0]
    };

  void SetUp() override {
    HrPtr             = (TBT_HOST_ROUTER *) AllocateZeroPool (sizeof (TBT_HOST_ROUTER));
    DiscreteTbtDevice = HrPtr;
    DiscreteTbtImage  = ((UINT8 *) &MockDtbtImage) + MockDtbtImage.PayloadItem.ImageOffset;
    ImageSize         = MockDtbtImage.PayloadItem.ImageSize;
    StartPercentage   = (6 + (90 / MockDtbtImage.PayloadHeader.PayloadCount) * 0);
    EndPercentage     = (6 + (90 / MockDtbtImage.PayloadHeader.PayloadCount) * 1);
    std::memcpy (&Progress, &LocalProgress, sizeof (EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS));
  }

  void TearDown () override {
    FreePool (HrPtr);
  }
};

//
// Case 1 - (DiscreteTbtDevice == NULL) || (DiscreteTbtImage == NULL) || (EndPercentage < StartPercentage)
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (UpdateDiscreteTbtNvmFirmwareTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  StartPercentage = (6 + (90 / MockDtbtImage.PayloadHeader.PayloadCount) * 2);

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

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
      0x0
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (
    FirmwareManagementMock,
    Progress (
      _
      )
    )
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

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
      0x0
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    FirmwareManagementMock,
    Progress (
      _
      )
    )
    .Times (2)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrBlk (
      DiscreteTbtDevice,
      _,
      _
      )
    )
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

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
      0x0
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    FirmwareManagementMock,
    Progress (
      _
      )
    )
    .Times (2)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrBlk (
      DiscreteTbtDevice,
      _,
      _
      )
    )
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

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
      0x0
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    FirmwareManagementMock,
    Progress (
      _
      )
    )
    .Times (2)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrBlk (
      DiscreteTbtDevice,
      _,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceExecAuth (
      DiscreteTbtDevice
      )
    )
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

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrOffset (
      DiscreteTbtDevice,
      0x0
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    FirmwareManagementMock,
    Progress (
      _
      )
    )
    .Times (2)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceWrBlk (
      DiscreteTbtDevice,
      _,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvDeviceExecAuth (
      DiscreteTbtDevice
      )
    )
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
