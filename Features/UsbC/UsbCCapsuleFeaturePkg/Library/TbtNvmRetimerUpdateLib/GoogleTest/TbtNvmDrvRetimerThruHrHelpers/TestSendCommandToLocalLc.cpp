/** @file
  Google test for SendCommandToLocalLc function.

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
// SendCommandToLocalLc Unit Test                           *
//**********************************************************
class SendCommandToLocalLcTest : public CommonMock {
  protected:
    TBT_STATUS            TbtStatus;
    RETIMER_THRU_HR       *RetimerPtr;
    UINT8                 ConfigurationSpace;
    UINT32                Cmd;
    UINT32                Data;
    UINT32                ExpData;
    UINT32                ExpUSB4CapRegRead;

    void SetUp() override {
      RetimerPtr          = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
      RetimerPtr->TbtPort = FIRST_MASTER_LANE;
      RetimerPtr->Hr      = gDevComHostMock;
      ConfigurationSpace  = ADAPTER_CONFIG_SPACE;
      Data                = 0;

      Cmd                 = TBT_IECS_CMD_LSEN;
    }

    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (RetimerPtr);
    }
};

//
// Case 1 - For PORT_CS_2 first time call WriteCioReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, PORT_CS_2FirstTimeWriteCioRegError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));   // For PORT_CS_2 first time call WriteCioReg return error

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - For PORT_CS_1 first time call WriteCioReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, PORT_CS_1FirstTimeWriteCioRegError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));    // For PORT_CS_1 first time call WriteCioReg return error

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - First time call WaitForMsgOutTxDone return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, FirstTimeWaitForMsgOutTxDoneError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));     // Call WaitForMsgOutTxDone return error

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - For PORT_CS_2 Second time call WriteCioReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, PORT_CS_2SecondTimeWriteCioRegError) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));       // Second time call WriteCioReg return error.

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  //  Mock call gBS->Stall in WaitForMsgOutTxDone
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - For PORT_CS_1 Second time call WriteCioReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, PORT_CS_1SecondTimeWriteCioRegError) {
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));       // Second time call WriteCioReg return error.
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  //  Mock call gBS->Stall in WaitForMsgOutTxDone
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 6 - Second time call WaitForMsgOutTxDone return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, SecondTimeWaitForMsgOutTxDoneError) {
  cout << "[---------- Case 6 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      _))
    .Times(2)
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));     // Second time call WaitForMsgOutTxDone return error
  //
  //  Mock call gBS->Stall in WaitForMsgOutTxDone
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 7 -  For PORT_CS_1 Third time call WriteCioReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, PORT_CS_1ThirdTimeWriteCioRegError) {
  cout << "[---------- Case 7 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .Times(3)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));       // Third time call WriteCioReg return error.
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  //  Mock call gBS->Stall for WaitForMsgOutTxDone and SendCommandToLocalLc
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 8 - Third time call WaitForMsgOutTxDone return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, ThirdTimeWaitForMsgOutTxDoneError) {
  cout << "[---------- Case 8 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .Times(3)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      _))
    .Times(3)
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));     // Third time call WaitForMsgOutTxDone return error
  //
  //  Mock call gBS->Stall for WaitForMsgOutTxDone and SendCommandToLocalLc
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 9 - For PORT_CS_2  call ReadCioDevRe return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, PORT_CS_2ReadCioDevReError) {
  cout << "[---------- Case 9 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .Times(3)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  //  Mock call gBS->Stall for WaitForMsgOutTxDone and SendCommandToLocalLc
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));  // For PORT_CS_2  call ReadCioDevRe return error

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 10 - Local LC Couldn't perform
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Local_LC_Could_not_perform) {
  cout << "[---------- Case 10 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillRepeatedly (Return(TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  //  Mock call gBS->Stall for WaitForMsgOutTxDone and SendCommandToLocalLc
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  ExpUSB4CapRegRead = TBT_IECS_CMD_LSEN;             // Local LC Couldn't perform
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpUSB4CapRegRead, sizeof(ExpUSB4CapRegRead)),
        Return (TBT_STATUS_SUCCESS)
     ));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 11 - Local LC reported error while performing
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Local_LC_Reported_Error_While_Performing) {
  cout << "[---------- Case 11 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillRepeatedly (Return(TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  //  Mock call gBS->Stall for WaitForMsgOutTxDone and SendCommandToLocalLc
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  ExpUSB4CapRegRead = TBT_IECS_CMD_LSUP;                 //  Local LC reported error while performing
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpUSB4CapRegRead, sizeof(ExpUSB4CapRegRead)),
        Return (TBT_STATUS_SUCCESS)
     ));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 12 - Correct flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (SendCommandToLocalLcTest, CorrectFlow) {
  cout << "[---------- Case 12 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      1,
      _))
    .Times(2)
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrWriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
      1,
      _))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillOnce (Return(TBT_STATUS_SUCCESS))
    .WillRepeatedly (Return(TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone
  //
  ExpData = TBT_MSG_OUT_ATCT1_LT0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpData, sizeof(ExpData)),
        Return (TBT_STATUS_SUCCESS)
    ));

  //
  //  Mock call gBS->Stall for WaitForMsgOutTxDone and SendCommandToLocalLc
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  ExpUSB4CapRegRead = TBT_LC_CMD_SUCCESS;                    // Correct flow
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpUSB4CapRegRead, sizeof(ExpUSB4CapRegRead)),
        Return (TBT_STATUS_SUCCESS)
     ));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
