/** @file
  Google test for WaitForMsgOutTxDone function.

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

//**********************************************************
// WaitForMsgOutTxDone Unit Test                           *
//**********************************************************
class WaitForMsgOutTxDoneTest : public Test {
  protected:
    TBT_STATUS                   TbtStatus;
    RETIMER_THRU_HR              *RetimerPtr;
    UINT16                       MsgOutCmdOffset;
    UINT8                        DbgData;
    UINT32                       ExpData;
    TBT_HOST_ROUTER              *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvHr              TbtNvmDrvHrMock;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;

  void SetUp() override {
    RetimerPtr = (RETIMER_THRU_HR *)AllocateZeroPool(sizeof(RETIMER_THRU_HR));
    RetimerPtr->TbtPort = FIRST_MASTER_LANE;
    RetimerPtr->Hr = gDevComHostMock;
    MsgOutCmdOffset = 0;
    DbgData = 0;
  }

  void TearDown() override
  {
    FreePool(RetimerPtr);
  }
};

//
// Case 1 - ReadCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (WaitForMsgOutTxDoneTest, ReadCioDevRegError) {
  cout << "[---------- Case 1 ----------]" << endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      MsgOutCmdOffset,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = WaitForMsgOutTxDone (RetimerPtr, MsgOutCmdOffset, DbgData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 -  ERROR! Local LC seems to be stuck
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (WaitForMsgOutTxDoneTest, LocalLC_SeemsToBeStuck) {
  cout << "[---------- Case 2 ----------]" << endl;

  ExpData = TBT_MSG_OUT_CMD_VALID;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      MsgOutCmdOffset,
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
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = WaitForMsgOutTxDone (RetimerPtr, MsgOutCmdOffset, DbgData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 -  IECS transaction was timeouted
// Expected Result - TbtStatus will return TBT_STATUS_RETRY
//
TEST_F (WaitForMsgOutTxDoneTest, IECS_TransactionWasTimeouted) {
  cout << "[---------- Case 3 ----------]" << endl;

  ExpData = TBT_MSG_OUT_TIMEOUT;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      MsgOutCmdOffset,
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
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = WaitForMsgOutTxDone (RetimerPtr, MsgOutCmdOffset, DbgData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_RETRY);
}

//
// Case 4 -  IECS transaction was invalid
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (WaitForMsgOutTxDoneTest, IECS_TransactionWasInvalid) {
  cout << "[---------- Case 4 ----------]" << endl;

  ExpData = TBT_MSG_OUT_INVALID;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      MsgOutCmdOffset,
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
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = WaitForMsgOutTxDone (RetimerPtr, MsgOutCmdOffset, DbgData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 -  Correct Flow.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (WaitForMsgOutTxDoneTest, CorrectFlow) {
  cout << "[---------- Case 5 ----------]" << endl;

  ExpData = TBT_MSG_OUT_ATCT1_LT0;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      MsgOutCmdOffset,
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
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = WaitForMsgOutTxDone (RetimerPtr, MsgOutCmdOffset, DbgData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
