/** @file
  Google test for CreateTBTDevInstance function.

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
#include <Library/FunctionMockLib.h>

struct MockTbtNvmDrvUtils {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvUtils);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    TbtNvmDrvAllocateMem,
    (IN UINTN  AllocationSize)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID,
    TbtNvmDrvDeAllocateMem,
    (IN VOID *Mem)
    );
};

struct MockTbtNvmDrvHrCtor {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvHrCtor);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_HOST_ROUTER *,
    TbtNvmDrvHrCtor,
    (IN UINT8           FirmwareType,
     IN PCIE_RP_CONFIG  *PcieRpConfig,
     IN PCIE_BDF        *TbtDmaPcieBdf,
     IN FORCE_PWR_HR    ForcePwrFunc OPTIONAL)
    );
};

MOCK_INTERFACE_DEFINITION (MockTbtNvmDrvHrCtor);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockTbtNvmDrvHrCtor, TbtNvmDrvHrCtor, 4, EFIAPI);

typedef struct {
  PAYLOAD_HEADER       PayloadHeader;
  DISCRETE_TBT_ITEM    PayloadItem;
  UINT32               ImageContent;
} MOCK_DISCRETE_TBT_PAYLOAD_DATA;

// **********************************************************
// CreateTBTDevInstance Unit Test                           *
// **********************************************************
class CreateTBTDevInstanceTest : public Test {
  protected:
    EFI_STATUS Status;
    UINT8                     FirmwareType;
    UINT8                     PcieRpType;
    UINT8                     PcieRootPort;
    DISCRETE_TBT_DEV_INSTANCE *DiscreteTbtDevice;
    PCIE_RP_CONFIG            *PcieRpConfig;
    TBT_HOST_ROUTER           *HrPtr;
    MockTbtNvmDrvUtils        TbtNvmDrvUtilsMock;
    MockTbtNvmDrvHrCtor       TbtNvmDrvHrCtorMock;

    MOCK_DISCRETE_TBT_PAYLOAD_DATA MockDtbtImage = {
      {
        SIGNATURE_32 ('D', 'T', 'B', 'T'),          //  Signature          // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
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
    FirmwareType      = MockDtbtImage.PayloadItem.FirmwareType;
    PcieRpType        = MockDtbtImage.PayloadItem.PcieRpType;
    PcieRootPort      = MockDtbtImage.PayloadItem.PcieRootPort;
    DiscreteTbtDevice = (DISCRETE_TBT_DEV_INSTANCE *) AllocateZeroPool (sizeof (DISCRETE_TBT_DEV_INSTANCE));
    PcieRpConfig      = (PCIE_RP_CONFIG *) AllocateZeroPool (sizeof (PCIE_RP_CONFIG));
    HrPtr             = (TBT_HOST_ROUTER *) AllocateZeroPool (sizeof (TBT_HOST_ROUTER));
  }

  void TearDown () override {
    FreePool (DiscreteTbtDevice);
    FreePool (PcieRpConfig);
    FreePool (HrPtr);
  }
};

//
// Case 1 - When DiscreteTbtDevice == NULL
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (CreateTBTDevInstanceTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = CreateTBTDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             NULL
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - When PcieRpConfig == NULL
// Expected Result - Status will return EFI_OUT_OF_RESOURCES
//
TEST_F (CreateTBTDevInstanceTest, PcieRpConfigIsNullError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_RP_CONFIG)
      )
    )
    .WillOnce (Return ((VOID *) NULL));

  Status = CreateTBTDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DiscreteTbtDevice
             );
  EXPECT_EQ (Status, EFI_OUT_OF_RESOURCES);
}

//
// Case 3 - When call TbtNvmDrvHrCtor return Null, HrPtr == NULL
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (CreateTBTDevInstanceTest, HrPtrIsNullError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_RP_CONFIG)
      )
    )
    .WillOnce (Return ((VOID *) PcieRpConfig));

  EXPECT_CALL (
    TbtNvmDrvHrCtorMock,
    TbtNvmDrvHrCtor (
      FirmwareType,
      _,
      NULL,
      NULL
      )
    )
    .WillOnce (Return (NULL));

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvDeAllocateMem (
      PcieRpConfig
      )
    );

  Status = CreateTBTDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DiscreteTbtDevice
             );
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
  EXPECT_EQ (*DiscreteTbtDevice, (TBT_HOST_ROUTER *) NULL);
}

//
// Case 4 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (CreateTBTDevInstanceTest, CorrectFlow) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_RP_CONFIG)
      )
    )
    .WillOnce (Return ((VOID *) PcieRpConfig));

  EXPECT_CALL (
    TbtNvmDrvHrCtorMock,
    TbtNvmDrvHrCtor (
      FirmwareType,
      _,
      NULL,
      NULL
      )
    )
    .WillOnce (Return (HrPtr));

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvDeAllocateMem (
      PcieRpConfig
      )
    );

  Status = CreateTBTDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DiscreteTbtDevice
             );
  EXPECT_EQ (Status, EFI_SUCCESS);
  EXPECT_EQ (*DiscreteTbtDevice, HrPtr);
}
