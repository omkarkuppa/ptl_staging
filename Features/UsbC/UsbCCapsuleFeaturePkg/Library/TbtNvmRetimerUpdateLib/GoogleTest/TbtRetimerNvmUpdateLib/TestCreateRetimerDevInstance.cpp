/** @file
  Google test for CreateRetimerDevInstance function.

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
extern "C" {
#include <TbtNvmRetimer.h>
typedef VOID *RETIMER_DEV_INSTANCE;
}

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

MOCK_INTERFACE_DEFINITION(MockTbtNvmDrvUtils);
MOCK_FUNCTION_INTERNAL_DEFINITION(MockTbtNvmDrvUtils, TbtNvmDrvAllocateMem, 1, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION(MockTbtNvmDrvUtils, TbtNvmDrvDeAllocateMem, 1, EFIAPI);

struct MockTbtNvmDrvRetimerThruHrCtor {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvRetimerThruHrCtor);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_RETIMER *,
    TbtNvmDrvRetimerThruHrCtor,
    (IN UINT8          FirmwareType,
     IN PCIE_BDF       *TbtDmaPcieBdf,
     IN TBT_PORT       TbtPort,
     IN UINT32         CascadedRetimerIndex,
     IN PCIE_RP_CONFIG *PcieRpConfig,
     IN FORCE_PWR_HR   ForcePwrFunc OPTIONAL)
    );
};

MOCK_INTERFACE_DEFINITION(MockTbtNvmDrvRetimerThruHrCtor);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockTbtNvmDrvRetimerThruHrCtor, TbtNvmDrvRetimerThruHrCtor, 6, EFIAPI);

typedef struct {
  PAYLOAD_HEADER    PayloadHeader;
  RETIMER_ITEM      PayloadItem;
  UINT32            ImageContent;
} MOCK_RETIMER_PAYLOAD_DATA;

// **********************************************************
// CreateRetimerDevInstance Unit Test                       *
// **********************************************************
class CreateRetimerDevInstanceTest : public Test {
  protected:
    EFI_STATUS                     Status;
    UINT8                          FirmwareType;
    UINT8                          PcieRpType;
    UINT8                          PcieRootPort;
    RETIMER_DEV_ADDRESS            *DevAddress;
    RETIMER_DEV_INSTANCE           *RetimerDevice;
    PCIE_BDF                       *TbtDmaPciLocation;
    PCIE_RP_CONFIG                 *PcieRpConfig;
    TBT_RETIMER                    *CommunicationPtr;
    MockTbtNvmDrvUtils             TbtNvmDrvUtilsMock;
    MockTbtNvmDrvRetimerThruHrCtor TbtNvmDrvRetimerThruHrCtorMock;

    MOCK_RETIMER_PAYLOAD_DATA MockItbtRetimerImage = {
      {
        SIGNATURE_32 ('R', 'T', 'M', 'R'),          //  Signature         // RETIMER_PAYLOAD_HEADER_SIGNATURE
        0x30,                                       //  HeaderSize
        1,                                          //  RetimerCount
        0                                           //  Reserved
      },                                            //  RETIMER_PAYLOAD_HEADER
      {
        {
          0x00,                                     //  Bus
          0x0D,                                     //  Device
          0x03,                                     //  Function
          0x00,                                     //  Port
          0x01,                                     //  RetimerIndex
          { 0, 0, 0, 0, 0, 0, 0, 0 }                //  Reserved[8]
        },                                          //  RetimerDevAddress // RETIMER_DEV_ADDRESS
        0x30,                                       //  ImageOffset
        0x04,                                       //  ImageSize
        0x00,                                       //  FirmwareType
        0xFF,                                       //  PcieRpType
        0xFF,                                       //  PcieRootPort
        { 0, 0, 0, 0, 0 }                           //  Reserved[5]
      },                                            //  RETIMER_ITEM
      0x00000000                                    //  ImageContent      // The value use to BufferPointer[0]
    };

  void SetUp() override {
    PcieRpType        = MockItbtRetimerImage.PayloadItem.PcieRpType;
    PcieRootPort      = MockItbtRetimerImage.PayloadItem.PcieRootPort;
    DevAddress        = &MockItbtRetimerImage.PayloadItem.RetimerDevAddress;
    RetimerDevice     = (RETIMER_DEV_INSTANCE *) AllocateZeroPool (sizeof (RETIMER_DEV_INSTANCE));
    TbtDmaPciLocation = (PCIE_BDF *) AllocateZeroPool (sizeof (PCIE_BDF));
    PcieRpConfig      = (PCIE_RP_CONFIG *) AllocateZeroPool (sizeof (PCIE_RP_CONFIG));
    CommunicationPtr  = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
  }

  void TearDown () override {
    FreePool (RetimerDevice);
    FreePool (TbtDmaPciLocation);
    FreePool (PcieRpConfig);
  }
};

//
// Case 1 - When ((DevAddress == NULL) || (RetimerDevice == NULL))
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (CreateRetimerDevInstanceTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  Status = CreateRetimerDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DevAddress,
             NULL
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - When TbtDmaPciLocation == NULL
// Expected Result - Status will return EFI_OUT_OF_RESOURCES
//
TEST_F (CreateRetimerDevInstanceTest, TbtDmaPciLocationAllocateMemError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_BDF)
      )
    )
    .WillOnce (Return ((VOID *) NULL));

  Status = CreateRetimerDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DevAddress,
             RetimerDevice
             );
  EXPECT_EQ (Status, EFI_OUT_OF_RESOURCES);
}

//
// Case 3 - When PcieRpConfig == NULL
// Expected Result - Status will return EFI_OUT_OF_RESOURCES
//
TEST_F (CreateRetimerDevInstanceTest, PcieRpConfigAllocateMemError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_BDF)
      )
    )
    .WillOnce (Return ((VOID *) TbtDmaPciLocation));

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_RP_CONFIG)
      )
    )
    .WillOnce (Return ((VOID *) NULL));

  Status = CreateRetimerDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DevAddress,
             RetimerDevice
             );
  EXPECT_EQ (Status, EFI_OUT_OF_RESOURCES);
}

//
// Case 4 - When DevAddress->Port = 2, Error DevAddress->Port value.
// Expected Result - Status will return EFI_UNSUPPORTED
//
TEST_F (CreateRetimerDevInstanceTest, DevAddress_Port_ErrorValue) {
  cout << "[---------- Case 4 ----------]"<< endl;
  DevAddress->Port = 2;                         // Override DevAddress->Port value for expected the value unsupported.

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_BDF)
      )
    )
    .WillOnce (Return ((VOID *) TbtDmaPciLocation));

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_RP_CONFIG)
      )
    )
    .WillOnce (Return ((VOID *) PcieRpConfig));

  Status = CreateRetimerDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DevAddress,
             RetimerDevice
             );
  EXPECT_EQ (Status, EFI_UNSUPPORTED);
}

//
// Case 5 - When FirmwareType == INTEGRATED_TBT_RETIMER,
//          call TbtNvmDrvRetimerThruHrCtor return NULL
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (CreateRetimerDevInstanceTest, DeviceIsNullError) {
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_BDF)
      )
    )
    .WillOnce (Return ((VOID *) TbtDmaPciLocation));

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_RP_CONFIG)
      )
    )
    .WillOnce (Return ((VOID *) PcieRpConfig));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrCtorMock,
    TbtNvmDrvRetimerThruHrCtor (
      FirmwareType,
      TbtDmaPciLocation,
      _,
      (UINT32) DevAddress->CascadedRetimerIndex,
      PcieRpConfig,
      _
      )
    )
    .WillOnce (Return ((TBT_RETIMER *) NULL));

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvDeAllocateMem (
      TbtDmaPciLocation
      )
    );

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvDeAllocateMem (
      PcieRpConfig
      )
    );

  Status = CreateRetimerDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DevAddress,
             RetimerDevice
             );
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
  EXPECT_EQ (*RetimerDevice, (TBT_RETIMER *) NULL);
}

//
// Case 6 - FirmwareType = DISCRETE_TBT_RETIMER, DevAddress->Port = 1
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (CreateRetimerDevInstanceTest, CorrectFlow) {
  cout << "[---------- Case 6 ----------]"<< endl;
  FirmwareType     = DISCRETE_TBT_RETIMER;
  DevAddress->Port = 1;                              // Override DevAddress->Port value to 1.

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_BDF)
      )
    )
    .WillOnce (Return ((VOID *) TbtDmaPciLocation));

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvAllocateMem (
      sizeof (PCIE_RP_CONFIG)
      )
    )
    .WillOnce (Return ((VOID *) PcieRpConfig));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrCtorMock,
    TbtNvmDrvRetimerThruHrCtor (
      FirmwareType,
      TbtDmaPciLocation,
      _,
      (UINT32) DevAddress->CascadedRetimerIndex,
      PcieRpConfig,
      _
      )
    )
    .WillOnce (Return (CommunicationPtr));

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvDeAllocateMem (
      TbtDmaPciLocation
      )
    );

  EXPECT_CALL (
    TbtNvmDrvUtilsMock,
    TbtNvmDrvDeAllocateMem (
      PcieRpConfig
      )
    );

  Status = CreateRetimerDevInstance (
             FirmwareType,
             PcieRpType,
             PcieRootPort,
             DevAddress,
             RetimerDevice
             );
  EXPECT_EQ (Status, EFI_SUCCESS);
  EXPECT_EQ (*RetimerDevice, CommunicationPtr);
}
