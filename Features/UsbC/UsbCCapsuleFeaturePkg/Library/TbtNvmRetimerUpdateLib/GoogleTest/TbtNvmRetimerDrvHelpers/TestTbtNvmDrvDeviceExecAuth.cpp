/** @file
  Google test for TbtNvmDrvDeviceExecAuth function.

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

#include <GTestTbtNvmRetimerDrvHelpers.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>
#include <GoogleTest/Private/MockTbtNvmDrvRetimerThruHr/MockTbtNvmDrvRetimerThruHr.h>

struct MockSendEnumCmd {
  MOCK_INTERFACE_DECLARATION (MockSendEnumCmd);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    SendEnumCmd,
    (IN RETIMER_THRU_HR *RetimerPtr)
    );
};

MOCK_INTERFACE_DEFINITION (MockSendEnumCmd);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockSendEnumCmd, SendEnumCmd, 1, );

//**********************************************************
// TbtNvmDrvDeviceExecAuth Unit Test                       *
//**********************************************************
class TbtNvmDrvDeviceExecAuthTest : public Test {
  protected:
    TBT_STATUS                   TbtStatus;
    VOID                         *DevComRetimer;
    VOID                         *DevComHost;
    TBT_RETIMER                  *gDevComRetimerMock  = &LocalCommunicationPtr;
    TBT_HOST_ROUTER              *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvRetimerThruHr   TbtNvmDrvRetimerThruHrMock;
    MockTbtNvmDrvHr              TbtNvmDrvHrMock;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;
    MockSendEnumCmd              SendEnumCmdMock;

  void SetUp() override {
    DevComRetimer = (VOID *) gDevComRetimerMock;
    DevComHost    = (VOID *) gDevComHostMock;
  }
};

//
// Case 1 - Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = TbtNvmDrvDeviceExecAuth (NULL);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - TARGET_RETIMER first time call WriteIecsReg return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, FirstTimeWriteIecsRegError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - TARGET_RETIMER second time call WriteIecsReg return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, SecondTimeWriteIecsRegError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - TARGET_RETIMER SendEnumCmd return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, SendEnumCmdError) {
  cout << "[---------- Case 4 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    SendEnumCmdMock,
    SendEnumCmd (
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - TARGET_RETIMER ReadIecsReg IECS_METADATA_ADDR return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, TARGET_RETIMER_ReadIecsRegError) {
  cout << "[---------- Case 5 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    SendEnumCmdMock,
    SendEnumCmd (
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  // Mock call ReadIecs for TbtNvmDrvWaitForCmdCpl
  //

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 6 - TARGET_RETIMER Auth has failed.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, TARGET_RETIMER_AuthHasFailed) {
  cout << "[---------- Case 6 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    SendEnumCmdMock,
    SendEnumCmd (
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  // Mock call ReadIecsReg for TbtNvmDrvWaitForCmdCpl
  //

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 7 - TARGET_RETIMER Correct Flow.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, TARGET_RETIMER_CorrectFlow) {
  cout << "[---------- Case 7 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    SendEnumCmdMock,
    SendEnumCmd (
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  // Mock call ReadIecsReg for TbtNvmDrvWaitForCmdCpl
  //

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 8 - TARGET_TBT_HOST first time call TbtNvmDrvHrWriteCioReg ROUTER_METADATA_ADDR return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, FirstTimeWriteCioDevRegError) {
  cout << "[---------- Case 8 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_METADATA_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComHost);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 9 - TARGET_TBT_HOST second time call TbtNvmDrvHrWriteCioReg ROUTER_METADATA_ADDR return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, SecondTimeWriteCioDevRegError) {
  cout << "[---------- Case 9 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_METADATA_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_CMD_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComHost);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 10 - TARGET_TBT_HOST Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvDeviceExecAuthTest, TARGET_TBT_HOST_CorrectFlow) {
  cout << "[---------- Case 10 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_METADATA_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_CMD_ADDR,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  TbtStatus = TbtNvmDrvDeviceExecAuth (DevComHost);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
