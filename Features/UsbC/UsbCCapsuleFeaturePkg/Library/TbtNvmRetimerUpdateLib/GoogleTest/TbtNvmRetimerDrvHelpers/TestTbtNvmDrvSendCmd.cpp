/** @file
  Google test for TbtNvmDrvSendCmd function.

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
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>
#include <GoogleTest/Private/MockTbtNvmDrvRetimerThruHr/MockTbtNvmDrvRetimerThruHr.h>

// **********************************************************
// TbtNvmDrvSendCmd Unit Test                               *
// **********************************************************
class TbtNvmDrvSendCmdTest : public Test {
  protected:
    TBT_STATUS                 TbtStatus;
    VOID                       *DevComRetimer;
    VOID                       *DevComHost;
    UINT32                     IECS_Cmd;
    UINT32                     CioDev_Cmd;
    BOOLEAN                    ResponseRequired;
    UINT32                     ExpReadData;
    TBT_RETIMER                *gDevComRetimerMock  = &LocalCommunicationPtr;
    TBT_HOST_ROUTER            *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvHr            TbtNvmDrvHrMock;
    MockTbtNvmDrvRetimerThruHr TbtNvmDrvRetimerThruHrMock;

  void SetUp() override {
    DevComRetimer    = (VOID *) gDevComRetimerMock;
    DevComHost       = (VOID *) gDevComHostMock;
    IECS_Cmd         = TBT_IECS_CMD_USUP;
    CioDev_Cmd       = OPCODE_NVM_AUTH_CMD;
    ResponseRequired = FALSE;
  }
};

//
// Case 1 - Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvSendCmdTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = TbtNvmDrvSendCmd (DevComRetimer, 0, ResponseRequired);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - WriteIecsReg IECS_CMD_ADDR Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvSendCmdTest, WriteIecsRegError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

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

  TbtStatus = TbtNvmDrvSendCmd (DevComRetimer, IECS_Cmd, ResponseRequired);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - WriteIecsReg Success.
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvSendCmdTest, WriteIecsReg_Success) {
  cout << "[---------- Case 3 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

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

  TbtStatus = TbtNvmDrvSendCmd (DevComRetimer, IECS_Cmd, ResponseRequired);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 4 - TbtNvmDrvHrWriteCioReg DEVICE_CONFIG_SPACE error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvSendCmdTest, TbtNvmDrvHrWriteCioRegError) {
  cout << "[---------- Case 4 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

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

  TbtStatus = TbtNvmDrvSendCmd (DevComHost, CioDev_Cmd, ResponseRequired);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - TbtNvmDrvHrWriteCioReg Success Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvSendCmdTest, TbtNvmDrvHrWriteCioRegSuccessCorrectFlow) {
  cout << "[---------- Case 5 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

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

  ExpReadData = OPCODE_NVM_AUTH_CMD;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_CMD_ADDR,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<4>(&ExpReadData, sizeof (ExpReadData)),
         Return (TBT_STATUS_SUCCESS)
         )
       );
  ResponseRequired = TRUE;
  TbtStatus = TbtNvmDrvSendCmd (DevComHost, CioDev_Cmd, ResponseRequired);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
