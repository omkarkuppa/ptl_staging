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
#include <GTestTbtNvmDRThruHrHelpers.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>

// **********************************************************
// SendCommandToLocalLc Unit Test                           *
// **********************************************************
class SendCommandToLocalLcTest : public Test {
  protected:
    TBT_STATUS                   TbtStatus;
    RETIMER_THRU_HR              *RetimerPtr;
    UINT8                        ConfigurationSpace;
    UINT32                       Cmd;
    UINT32                       Data;
    UINT32                       ExpData;
    UINT32                       USB4CapRegRead;
    UINT32                       USB4CapRegWrite;
    UINT16                       TBT_USB4_PORT_CAPABILITY_OFFSET = 0x6;
    TBT_HOST_ROUTER              *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvHr              TbtNvmDrvHrMock;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;

  void SetUp() override {
    RetimerPtr          = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    RetimerPtr->TbtPort = FIRST_MASTER_LANE;
    RetimerPtr->Hr      = gDevComHostMock;
    ConfigurationSpace  = ADAPTER_CONFIG_SPACE;
    Data                = 0;
    Cmd                 = TBT_IECS_CMD_LSEN;
  }

  void TearDown() override {
    FreePool (RetimerPtr);
  }
};

//
// Case 1 - RetimerPtr->Hr->WriteCioDevReg return error
// Mock - TbtNvmDrvHrMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - RetimerPtr->Hr->WriteCioDevReg second time return error.
// Mock - TbtNvmDrvHrMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (2)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - First time call WaitForMsgOutTxDone return error
// Mock - TbtNvmDrvHrMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case3) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (2)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
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
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - RetimerPtr->Hr->WriteCioDevReg third time return error
// Mock - TbtNvmDrvHrMock / UefiBootServicesTableLibMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case4) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (3)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  ExpData = TBT_MSG_OUT_ATCT1_LT0;

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
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<4>(&ExpData, sizeof (ExpData)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - RetimerPtr->Hr->WriteCioDevReg four time return error
// Mock - TbtNvmDrvHrMock / UefiBootServicesTableLibMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case5) {
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (4)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  ExpData = TBT_MSG_OUT_ATCT1_LT0;

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
         SetArgBuffer<4>(&ExpData, sizeof (ExpData)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 6 - RetimerPtr->Hr->ReadCioDevReg second time return error
// Mock - TbtNvmDrvHrMock / UefiBootServicesTableLibMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case6) {
  cout << "[---------- Case 6 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (4)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  ExpData = TBT_MSG_OUT_ATCT1_LT0;

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
    .Times (2)
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData, sizeof (ExpData)),
         Return (TBT_STATUS_SUCCESS)
         )
       )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 7 - RetimerPtr->Hr->WriteCioDevReg five time return error
// Mock - TbtNvmDrvHrMock / UefiBootServicesTableLibMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case7) {
  cout << "[---------- Case 7 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (5)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  ExpData = TBT_MSG_OUT_ATCT1_LT0;

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
    .Times (2)
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData, sizeof (ExpData)),
         Return (TBT_STATUS_SUCCESS)
         )
       )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .Times (3)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 8 - RetimerPtr->Hr->ReadCioDevReg third time return error
// Mock - TbtNvmDrvHrMock / UefiBootServicesTableLibMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case8) {
  cout << "[---------- Case 8 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (5)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  ExpData = TBT_MSG_OUT_ATCT1_LT0;

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
    .Times (3)
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData, sizeof (ExpData)),
         Return (TBT_STATUS_SUCCESS)
         )
       )
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .Times (3)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 9 - RetimerPtr->Hr->ReadCioDevReg four time return error
// Mock - TbtNvmDrvHrMock / UefiBootServicesTableLibMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case9) {
  cout << "[---------- Case 9 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (5)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  ExpData = TBT_MSG_OUT_ATCT1_LT0;

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
    .Times (4)
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData, sizeof (ExpData)),
         Return (TBT_STATUS_SUCCESS)
         )
       )
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .Times (4)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 10 - Correct Flow
// Mock - TbtNvmDrvHrMock / UefiBootServicesTableLibMock
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (SendCommandToLocalLcTest, Case10) {
  cout << "[---------- Case 10 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (5)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  ExpData = TBT_MSG_OUT_ATCT1_LT0;

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
    .Times (4)
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpData, sizeof (ExpData)),
         Return (TBT_STATUS_SUCCESS)
         )
       )
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .Times (4)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 11 - RetimerPtr->Hr->ReadCioDevReg four time return error
// Mock - TbtNvmDrvHrMock / UefiBootServicesTableLibMock
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendCommandToLocalLcTest, Case11) {
  cout << "[---------- Case 11 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ConfigurationSpace,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (5)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  ExpData = TBT_MSG_OUT_ATCT1_LT0;

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
    .Times (4)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&USB4CapRegRead, sizeof (USB4CapRegRead)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .Times (4)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone <<<

  TbtStatus = SendCommandToLocalLc (RetimerPtr, ConfigurationSpace, Cmd, &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}
