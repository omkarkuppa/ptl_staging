/** @file
  Google test for TbtDrvReadDwFromNvm function.

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

//**********************************************************
// TbtDrvReadDwFromNvm Unit Test                           *
//**********************************************************
class TbtDrvReadDwFromNvmTest : public CommonMock {
  protected:
    TBT_STATUS            TbtStatus;
    VOID                  *DevComRetimer;
    VOID                  *DevComHost;
    UINT32                Offset;
    UINT32                DataLength;
    UINT32                RdData;

    UINT32                ExpReadData;

    void SetUp() override {
      DevComRetimer = (VOID *) gDevComRetimerMock;
      DevComHost    = (VOID *) gDevComHostMock;

      Offset = 0;
      DataLength = 1;
    }

};

//
// Case 1 - Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtDrvReadDwFromNvmTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = TbtDrvReadDwFromNvm (NULL, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - TARGET_RETIMER first time call WriteIecsReg return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtDrvReadDwFromNvmTest, WriteIecsRegError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtDrvReadDwFromNvm (DevComRetimer, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - TARGET_RETIMER TbtNvmDrvSendCmd return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtDrvReadDwFromNvmTest, TARGET_RETIMER_TbtNvmDrvSendCmdError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  // Mock call WriteIecs for TbtNvmDrvSendCmd 
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtDrvReadDwFromNvm (DevComRetimer, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - TARGET_RETIMER ReadIecsReg IECS_MSG_OUT_RDATA return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtDrvReadDwFromNvmTest, ReadIecsRegError) {
  cout << "[---------- Case 4 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  // Mock call WriteIecsReg for TbtNvmDrvSendCmd 
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  // Mock call ReadIecsReg for TbtNvmDrvWaitForCmdCpl in TbtNvmDrvSendCmd
  //
  ExpReadData       = OPCODE_NVM_READ_CMD;
  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      ))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<3>(&ExpReadData, sizeof(ExpReadData)),
        Return (TBT_STATUS_SUCCESS)
    ));

  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    ReadIecs (
      gDevComRetimerMock,
      IECS_MSG_OUT_RDATA,
      (DataLength<16?DataLength:16),
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtDrvReadDwFromNvm (DevComRetimer, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - TARGET_RETIMER Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtDrvReadDwFromNvmTest, TARGET_RETIMER_CorrectFlow) {
  cout << "[---------- Case 5 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;

  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  // Mock call WriteIecsReg for TbtNvmDrvSendCmd 
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  // Mock call ReadIecsReg for TbtNvmDrvWaitForCmdCpl in TbtNvmDrvSendCmd
  //
  ExpReadData       = OPCODE_NVM_READ_CMD;
  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      ))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<3>(&ExpReadData, sizeof(ExpReadData)),
        Return (TBT_STATUS_SUCCESS)
    ));

  EXPECT_CALL (TbtNvmDrvRetimerThruHrMock,
    ReadIecs (
      gDevComRetimerMock,
      IECS_MSG_OUT_RDATA,
      (DataLength<16?DataLength:16),
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  TbtStatus = TbtDrvReadDwFromNvm (DevComRetimer, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 6 - TARGET_TBT_HOST call TbtNvmDrvHrWriteCioReg ROUTER_METADATA_ADDR return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtDrvReadDwFromNvmTest, WriteCioDevRegError) {
  cout << "[---------- Case 6 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_METADATA_ADDR,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtDrvReadDwFromNvm (DevComHost, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 7 - TARGET_TBT_HOST call TbtNvmDrvSendCmd return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtDrvReadDwFromNvmTest, TARGET_TBT_HOST_TbtNvmDrvSendCmdError) {
  cout << "[---------- Case 7 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_METADATA_ADDR,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  //
  // Mock call WriteCioReg for TbtNvmDrvSendCmd 
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_CMD_ADDR,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtDrvReadDwFromNvm (DevComHost, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 8 - TARGET_TBT_HOST call ReadCioDevReg return Error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtDrvReadDwFromNvmTest, TARGET_TBT_HOST_ReadCioDevRegError) {
  cout << "[---------- Case 8 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_METADATA_ADDR,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  //
  // Mock call WriteCioReg for TbtNvmDrvSendCmd 
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_CMD_ADDR,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  // Mock call ReadIecsReg for TbtNvmDrvWaitForCmdCpl in TbtNvmDrvSendCmd
  //
  ExpReadData       = OPCODE_NVM_READ_CMD;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_CMD_ADDR,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpReadData, sizeof(ExpReadData)),
        Return (TBT_STATUS_SUCCESS)
    ));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_DATA_ADDR,
      _))
     .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtDrvReadDwFromNvm (DevComHost, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 9 - TARGET_TBT_HOST Correct Flow.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtDrvReadDwFromNvmTest, TARGET_TBT_HOST_CorrectFlow) {
  cout << "[---------- Case 9 ----------]"<< endl;

  gUpdateTargetType = TARGET_TBT_HOST;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_METADATA_ADDR,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  //
  // Mock call WriteCioReg for TbtNvmDrvSendCmd 
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_CMD_ADDR,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  // Mock call ReadIecsReg for TbtNvmDrvWaitForCmdCpl in TbtNvmDrvSendCmd
  //
  ExpReadData       = OPCODE_NVM_READ_CMD;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_CMD_ADDR,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpReadData, sizeof(ExpReadData)),
        Return (TBT_STATUS_SUCCESS)
    ));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      gDevComHostMock,
      DEVICE_CONFIG_SPACE,
      0,
      ROUTER_DATA_ADDR,
      _))
     .WillOnce (Return(TBT_STATUS_SUCCESS));

  TbtStatus = TbtDrvReadDwFromNvm (DevComHost, Offset, DataLength, &RdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
