/** @file
  Google Test for the implementation of FmpDeviceLib instance to support
  Thunderbolt Retimer Firmware update

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
#include <GTestFmpDeviceLibUsbCRetimer.h>
#include <GoogleTest/Library/MockTbtNvmRetimerUpdateLib.h>

extern "C" {
#include <TbtNvmRetimer.h>

typedef VOID  *RETIMER_DEV_INSTANCE;
typedef VOID  *HR_DEV_INSTANCE;

EFI_STATUS
EFIAPI
ConstructRetimerInstances (
  IN  CONST VOID            *Image,
  IN  UINTN                 ImageSize,
  OUT UINT32                *LastAttemptStatus,
  OUT RETIMER_DEV_INSTANCE  *RetimerDeviceInstances,
  OUT UINT32                *RetimerDeviceInstancesCount,
  OUT HR_DEV_INSTANCE       *HrDeviceInstances,
  OUT UINT32                *HrDeviceInstancesCount
  );
}

/**
Google test for ConstructRetimerInstances function.
**/
class ConstructRetimerInstancesTest : public Test {
  protected:
    EFI_STATUS                 Status;
    CONST VOID                 *Image;
    CONST VOID                 *Image_Unsupport;
    CONST VOID                 *Image_Integrated;
    UINTN                      ImageSize;
    UINT32                     *LastAttemptStatus;
    UINT32                     *RetimerDeviceInstancesCount;
    UINT32                     *HrDeviceInstancesCount;
    HR_DEV_INSTANCE            *HrDeviceInstances;
    RETIMER_DEV_INSTANCE       *RetimerDeviceInstances;
    MockTbtNvmRetimerUpdateLib TbtNvmRetimerUpdateMock;
    RETIMER_DEV_INSTANCE       RetimerDevice;

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

  RETIMER_PAYLOAD_DATA MockIntegratedRetimerImage = {
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
      0x00,                                       //  FirmwareType
      0x00,                                       //  PcieRpType
      0x09,                                       //  PcieRootPort
      { 0, 0, 0, 0, 0 }                           //  Reserved[5]
    }                                             //  RETIMER_ITEM
  };

  RETIMER_PAYLOAD_DATA MockUnsupportedRetimerImage = {
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
      0x02,                                       //  FirmwareType
      0x00,                                       //  PcieRpType
      0x09,                                       //  PcieRootPort
      { 0, 0, 0, 0, 0 }                           //  Reserved[5]
    }                                             //  RETIMER_ITEM
  };

  PAYLOAD_HEADER *RetimerPayloadHeader;
  RETIMER_ITEM *RetimerPayloadItem;

  void SetUp() override {
    Image                       = (VOID *) &MockRetimerImage;
    Image_Unsupport             = (VOID *) &MockUnsupportedRetimerImage;
    Image_Integrated            = (VOID *) &MockIntegratedRetimerImage;
    ImageSize                   = sizeof (Image);
    LastAttemptStatus           = (UINT32 *) AllocateZeroPool (sizeof (LastAttemptStatus));
    RetimerDeviceInstancesCount = (UINT32 *) AllocateZeroPool (sizeof (RetimerDeviceInstancesCount));
    HrDeviceInstancesCount      = (UINT32 *) AllocateZeroPool (sizeof (HrDeviceInstancesCount));
    HrDeviceInstances           = (HR_DEV_INSTANCE *) AllocateZeroPool (sizeof (HrDeviceInstances));
    RetimerDeviceInstances      = (RETIMER_DEV_INSTANCE *) AllocateZeroPool (sizeof (RetimerDeviceInstances));
    RetimerPayloadHeader        = (PAYLOAD_HEADER *) Image;
    RetimerPayloadItem          = (RETIMER_ITEM *) (RetimerPayloadHeader + 1);
  }
};

TEST_F (ConstructRetimerInstancesTest, Var_1_Error) {
  //
  // Case 1 - RetimerDeviceInstancesCount >= MAX_RETIMER_INSTANCES, return error
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 1 ----------]"<< endl;
  *RetimerDeviceInstancesCount = 100;

  Status = ConstructRetimerInstances (Image, ImageSize, LastAttemptStatus, RetimerDeviceInstances, RetimerDeviceInstancesCount, HrDeviceInstances, HrDeviceInstancesCount);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (ConstructRetimerInstancesTest, Var_2_Error) {
  //
  // Case 2 - RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize > ImageSize, return error
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 2 ----------]"<< endl;

  Status = ConstructRetimerInstances (Image, ImageSize, LastAttemptStatus, RetimerDeviceInstances, RetimerDeviceInstancesCount, HrDeviceInstances, HrDeviceInstancesCount);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (ConstructRetimerInstancesTest, Var_3_Error) {
  //
  // Case 3 - FirmwareType is INTEGRATED_TBT_RETIMER , CreateRetimerDevInstance return error
  // Mock - TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    CreateRetimerDevInstance
    )
    .WillOnce (
       DoAll (
         SetArgPointee<4>(RetimerDevice),
         Return (EFI_INVALID_PARAMETER)
         )
       );

  Status = ConstructRetimerInstances (Image_Integrated, ImageSize- 0x100, LastAttemptStatus, RetimerDeviceInstances, RetimerDeviceInstancesCount, HrDeviceInstances, HrDeviceInstancesCount);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (ConstructRetimerInstancesTest, Var_4_Error) {
  //
  // Case 4 - FirmwareType is DISCRETE_TBT_RETIMER , CreateRetimerDevInstance return error
  // Mock - TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    CreateRetimerDevInstance
    )
    .WillOnce (
       DoAll (
         SetArgPointee<4>(RetimerDevice),
         Return (EFI_INVALID_PARAMETER)
         )
       );

  Status = ConstructRetimerInstances (Image, ImageSize- 0x100, LastAttemptStatus, RetimerDeviceInstances, RetimerDeviceInstancesCount, HrDeviceInstances, HrDeviceInstancesCount);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (ConstructRetimerInstancesTest, Var_5_Error) {
  //
  // Case 5 - FirmwareType is Unsupported , CreateHrDevInstance return error
  // Mock - TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_INVALID_PARAMETER
  //
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    CreateHrDevInstance
    )
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = ConstructRetimerInstances (Image_Unsupport, ImageSize- 0x100, LastAttemptStatus, RetimerDeviceInstances, RetimerDeviceInstancesCount, HrDeviceInstances, HrDeviceInstancesCount);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);
}

TEST_F (ConstructRetimerInstancesTest, Var_6_Success) {
  //
  // Case 6 - FirmwareType is Unsupported , CreateHrDevInstance return EFI_SUCCESS
  // Mock - TbtNvmRetimerUpdateMock
  // Expected Result - Status will return EFI_SUCCESS
  //
  cout << "[---------- Case 6 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmRetimerUpdateMock,
    CreateHrDevInstance
    )
    .WillOnce (Return (EFI_SUCCESS));

  Status = ConstructRetimerInstances (Image_Unsupport, ImageSize- 0x100, LastAttemptStatus, RetimerDeviceInstances, RetimerDeviceInstancesCount, HrDeviceInstances, HrDeviceInstancesCount);
  ASSERT_EQ (Status, EFI_SUCCESS);

  cout << "ConstructRetimerInstances Done." << endl;
}
