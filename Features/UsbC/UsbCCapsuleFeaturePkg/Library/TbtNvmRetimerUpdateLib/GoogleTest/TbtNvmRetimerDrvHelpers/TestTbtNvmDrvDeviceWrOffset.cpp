/** @file
  Google test for TbtNvmDrvDeviceWrOffset function.

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
// TbtNvmDrvDeviceWrOffset Unit Test                        *
// **********************************************************
class TbtNvmDrvDeviceWrOffsetTest : public Test {
  protected:
    TBT_STATUS                 TbtStatus;
    VOID                       *DevComRetimer;
    VOID                       *DevComHost;
    UINT32                     OffsetInDW;
    UINT32                     ExpReadData;
    TBT_RETIMER                *gDevComRetimerMock  = &LocalCommunicationPtr;
    TBT_HOST_ROUTER            *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvRetimerThruHr TbtNvmDrvRetimerThruHrMock;
    MockTbtNvmDrvHr            TbtNvmDrvHrMock;

  void SetUp() override {
    DevComRetimer = (VOID *) gDevComRetimerMock;
    DevComHost    = (VOID *) gDevComHostMock;
    OffsetInDW    = 0;
  }
};

//
// Case 1 - Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtNvmDrvDeviceWrOffsetTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = TbtNvmDrvDeviceWrOffset (NULL, OffsetInDW);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - TARGET_RETIMER call WriteIecsReg return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceWrOffsetTest, WriteIecsRegError) {
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

  TbtStatus = TbtNvmDrvDeviceWrOffset (DevComRetimer, OffsetInDW);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - TARGET_RETIMER call TbtNvmDrvSendCmd return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceWrOffsetTest, TARGET_RETIMER_TbtNvmDrvSendCmdError) {
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
  //
  // Mock call WriteIecs for TbtNvmDrvSendCmd
  //
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

  TbtStatus = TbtNvmDrvDeviceWrOffset (DevComRetimer, OffsetInDW);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - TARGET_RETIMER Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvDeviceWrOffsetTest, TARGET_RETIMER_CorrectFlow) {
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
  //
  // Mock call WriteIecs for TbtNvmDrvSendCmd
  //
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

  ExpReadData = NVM_READ;
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
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<3>(&ExpReadData, sizeof (ExpReadData)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  TbtStatus = TbtNvmDrvDeviceWrOffset (DevComRetimer, OffsetInDW);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 5 - TARGET_TBT_HOST call TbtNvmDrvHrWriteCioReg retun error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceWrOffsetTest, TbtNvmDrvHrWriteCioRegError) {
  cout << "[---------- Case 5 ----------]"<< endl;

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

  TbtStatus = TbtNvmDrvDeviceWrOffset (DevComHost, OffsetInDW);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 6 -  TARGET_TBT_HOST call TbtNvmDrvSendCmd return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvDeviceWrOffsetTest, TARGET_TBT_HOST_TbtNvmDrvSendCmdError) {
  cout << "[---------- Case 6 ----------]"<< endl;

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

  //
  // Mock call TbtNvmDrvHrWriteCioReg for TbtNvmDrvSendCmd
  //
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

  TbtStatus = TbtNvmDrvDeviceWrOffset (DevComHost, OffsetInDW);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 7 - TARGET_TBT_HOST Correct Flow.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvDeviceWrOffsetTest, TARGET_TBT_HOST_CorrectFlow) {
  cout << "[---------- Case 7 ----------]"<< endl;

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

  //
  // Mock call TbtNvmDrvHrWriteCioReg for TbtNvmDrvSendCmd
  //
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

  //
  // Mock call TbtNvmDrvHrReadCioDevReg for TbtNvmDrvWaitForCmdCpl
  //
  ExpReadData = OPCODE_NVM_SETOFFSET_CMD;
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

  TbtStatus = TbtNvmDrvDeviceWrOffset (DevComHost, OffsetInDW);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
