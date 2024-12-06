/** @file
  Google test for TbtNvmDrvWaitForCmdCpl function.

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

extern "C" {
TBT_STATUS
TbtNvmDrvWaitForCmdCpl (
  IN VOID    *DevCom,
  IN UINT32  Cmd
  );
}

// **********************************************************
// TbtNvmDrvWaitForCmdCpl Unit Test                         *
// **********************************************************
class TbtNvmDrvWaitForCmdCplTest : public Test {
  protected:
    TBT_STATUS                 TbtStatus;
    VOID                       *DevComRetimer;
    VOID                       *DevComHost;
    UINT32                     IECS_Cmd;
    UINT32                     CioDev_Cmd;
    UINT32                     ExpReadData;
    TBT_RETIMER                *gDevComRetimerMock  = &LocalCommunicationPtr;
    TBT_HOST_ROUTER            *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvRetimerThruHr TbtNvmDrvRetimerThruHrMock;
    MockTbtNvmDrvHr            TbtNvmDrvHrMock;

  void SetUp() override {
    DevComRetimer = (VOID *) gDevComRetimerMock;
    DevComHost    = (VOID *) gDevComHostMock;
    IECS_Cmd      = TBT_IECS_CMD_USUP;
    CioDev_Cmd    = OPCODE_NVM_AUTH_CMD;
  }
};

//
// Case 1 - Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComRetimer, 0);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - ReadIecsReg IECS_CMD_ADDR Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, ReadIecsReg_IECS_CMD_ADDR_Error) {
  cout << "[---------- Case 2 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

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

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComRetimer, IECS_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - Remote IECS CMD is timeouted
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, IECS_CMD_IsTimeoutedError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;
  ExpReadData       = TBT_IECS_CMD_USUP;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<3>(&ExpReadData, sizeof (ExpReadData)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComRetimer, IECS_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - IECS CMD not supported.
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, CommandNotSupportedError) {
  cout << "[---------- Case 4 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;
  ExpReadData       = IECS_USB4_CMD_NOTSUPPORTED;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<3>(&ExpReadData, sizeof (ExpReadData)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComRetimer, IECS_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - IECS_USB4_CMD_ERR
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, USB4_CMD_ERR) {
  cout << "[---------- Case 5 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;
  ExpReadData       = IECS_USB4_CMD_ERR;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<3>(&ExpReadData, sizeof (ExpReadData)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComRetimer, IECS_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_RETRY);
}

//
// Case 6 - ReadCioDevReg DEVICE_CONFIG_SPACE error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, ReadCioDevRegDEVICE_CONFIG_SPACE_Error) {
  cout << "[---------- Case 6 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

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
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComHost, CioDev_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 7 - ROUTER_CMD is timeouted.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, ROUTER_CMD_Is_Timeouted) {
  cout << "[---------- Case 7 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;
  ExpReadData       = TBT_CMD_VALID;

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

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComHost, CioDev_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 8 - ROUTER_CMD not supported.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, ROUTER_CMD_NotSupportedError) {
  cout << "[---------- Case 8 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;
  ExpReadData       = TBT_CMD_NOTSUPPORTED;

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

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComHost, CioDev_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 9 - ROUTER_CMD encountered an error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, ROUTER_CMD_RncounteredAnError) {
  cout << "[---------- Case 9 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;
  ExpReadData       = TBT_METADATA_VALID;

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

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComHost, CioDev_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 10 - Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvWaitForCmdCplTest, CorrectFlow) {
  cout << "[---------- Case 10 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;
  ExpReadData       = OPCODE_NVM_AUTH_CMD;

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

  TbtStatus = TbtNvmDrvWaitForCmdCpl (DevComHost, CioDev_Cmd);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
