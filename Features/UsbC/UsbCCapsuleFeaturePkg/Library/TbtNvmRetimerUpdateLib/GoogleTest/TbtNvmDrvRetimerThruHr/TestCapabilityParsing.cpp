/** @file
  Google test for CapabilityParsing function.

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
#include <GTestTbtNvmDrvRetimerThruHr.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>
#include <GoogleTest/Library/MockBasePcdLib.h>
//
// Test function define.
//
extern "C" {
TBT_STATUS
CapabilityParsing (
  IN TBT_RETIMER  *This
  );
}

// **********************************************************
// CapabilityParsing Unit Test                              *
// **********************************************************
class CapabilityParsingTest : public Test {
  protected:
    TBT_STATUS                   TbtStatus;
    RETIMER_THRU_HR              *RetimerPtr;
    TBT_RETIMER                  *DevComRetimer;
    UINT32                       ExpData1;
    UINT32                       ExpData2;
    UINT32                       ExpData3;
    TBT_HOST_ROUTER              *gDevComHostMock = &LocalHrPtr;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;
    MockTbtNvmDrvHr              TbtNvmDrvHrMock;
    MockBasePcdLib               BasePcdLibMock;

  void SetUp() override {
    RetimerPtr          = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    DevComRetimer       = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
    RetimerPtr->Hr      = gDevComHostMock;
    DevComRetimer->Impl = RetimerPtr;
  }

  void TearDown() override {
    FreePool (RetimerPtr);
    FreePool (DevComRetimer);
  }
};

//
// Case 1 -  Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (CapabilityParsingTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = CapabilityParsing (NULL);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - First time ADAPTER_CONFIG_SPACE call ReadCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (CapabilityParsingTest, FirstTimeReadCioDevRegError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  ExpData1 = 0x8086;

  EXPECT_CALL (BasePcdLibMock, LibPcdGet32 (_));

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR))
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData1, sizeof (ExpData1)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillRepeatedly (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      0x1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - ERROR, USB4 Port Capability was not found.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (CapabilityParsingTest, USB4_PortCapabilityWasNotFound) {
  cout << "[---------- Case 3 ----------]"<< endl;

  ExpData1 = 0x8086;

  EXPECT_CALL (BasePcdLibMock, LibPcdGet32 (_));

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData1, sizeof (ExpData1)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  ExpData2 = 0x0;
  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      0x1,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData2, sizeof (ExpData2)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - Second time ADAPTER_CONFIG_SPACE call ReadCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (CapabilityParsingTest, SecondTimeReadCioDevRegError) {
  cout << "[---------- Case 4 ----------]"<< endl;

  ExpData1 = 0x8086;

  EXPECT_CALL (BasePcdLibMock, LibPcdGet32 (_));

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData1, sizeof (ExpData1)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  ExpData2 = 0x6;
  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData2, sizeof (ExpData2)),
         Return (TBT_STATUS_SUCCESS)
         )
       )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (CapabilityParsingTest, CorrectFlow) {
  cout << "[---------- Case 5 ----------]"<< endl;

  ExpData1 = 0x8086;

  EXPECT_CALL (BasePcdLibMock, LibPcdGet32 (_));

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData1, sizeof (ExpData1)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  ExpData2 = 0x6;
  ExpData3 = 0x600;
  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData2, sizeof (ExpData2)),
         Return (TBT_STATUS_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData3, sizeof (ExpData3)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
