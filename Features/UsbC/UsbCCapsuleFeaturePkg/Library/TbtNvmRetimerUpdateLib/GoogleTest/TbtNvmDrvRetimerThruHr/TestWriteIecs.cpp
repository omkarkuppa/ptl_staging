/** @file
  Google test for WriteIecs function.

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
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>

//
// Test function define.
//
extern "C"
{
TBT_STATUS
WriteIecs (
  IN TBT_RETIMER  *This,
  IN UINT8        RegNum,
  IN UINT32       *DataPtr,
  IN UINT8        Length
  );
}

struct MockWaitForMsgOutTxDone {
  MOCK_INTERFACE_DECLARATION (MockWaitForMsgOutTxDone);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    WaitForMsgOutTxDone,
    (IN RETIMER_THRU_HR     *RetimerPtr,
     IN UINT16              MsgOutCmdOffset,
     IN UINT8               DbgData)
    );
};

// **********************************************************
// WriteIecs Unit Test                                      *
// **********************************************************
class WriteIecsTest : public Test {
  protected:
    TBT_STATUS              TbtStatus;
    RETIMER_THRU_HR         *RetimerPtr;
    TBT_RETIMER             *DevComRetimer;
    UINT8                   RegNum;
    UINT32                  CmdData;
    UINT8                   Length;
    TBT_HOST_ROUTER         *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvHr         TbtNvmDrvHrMock;
    MockWaitForMsgOutTxDone WaitForMsgOutTxDoneMock;

  void SetUp() override {
    RetimerPtr          = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    RetimerPtr->Hr      = gDevComHostMock; // For call Mock WriteCioDevReg
    DevComRetimer       = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
    DevComRetimer->Impl = RetimerPtr;      // For RetimerPtr = (RETIMER_THRU_HR *)This->Impl
    RegNum              = IECS_CMD_ADDR;
    CmdData             = IECS_BLKW_CMD;
    Length              = 1;
  }

  void TearDown () override {
    FreePool (RetimerPtr);
    FreePool (DevComRetimer);
  }
};

//
// Case 1 -  Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (WriteIecsTest, AssertError) {
  cout << "[---------- Case 1 ----------]" << endl;

  TbtStatus = WriteIecs (NULL, RegNum, &CmdData, Length);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - First time call WriteCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (WriteIecsTest, FirstTimeWriteCioDevRegError) {
  cout << "[---------- Case 2 ----------]" << endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      Length,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR)); // For PORT_CS_2 first time call WriteCioReg return error

  TbtStatus = WriteIecs (DevComRetimer, RegNum, &CmdData, Length);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - Second time call WriteCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (WriteIecsTest, SecondTimeWriteCioDevRegError) {
  cout << "[---------- Case 3 ----------]" << endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      Length,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS))                // For PORT_CS_2 first time call WriteCioReg return TBT_STATUS_SUCCESS
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR)); // For PORT_CS_1 second time call WriteCioReg return error

  TbtStatus = WriteIecs (DevComRetimer, RegNum, &CmdData, Length);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - Call WaitForMsgOutTxDone return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (WriteIecsTest, WaitForMsgOutTxDone_NON_RECOVERABLE_ERROR) {
  cout << "[---------- Case 4 ----------]" << endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      Length,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS))  // For PORT_CS_2 first time call WriteCioReg return TBT_STATUS_SUCCESS
    .WillOnce (Return (TBT_STATUS_SUCCESS)); // For PORT_CS_1 second time call WriteCioReg return TBT_STATUS_SUCCESS

  EXPECT_CALL (
    WaitForMsgOutTxDoneMock,
    WaitForMsgOutTxDone (
      RetimerPtr,
      _,
      0
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = WriteIecs (DevComRetimer, RegNum, &CmdData, Length);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - Call WaitForMsgOutTxDone return error
// Expected Result - TbtStatus will return TBT_STATUS_RETRY
//
TEST_F (WriteIecsTest, WaitForMsgOutTxDoneRETRY) {
  cout << "[---------- Case 5 ----------]" << endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      Length,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS))        // For PORT_CS_2 first time call WriteCioReg return TBT_STATUS_SUCCESS
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS)); // For PORT_CS_1 second time call WriteCioReg return TBT_STATUS_SUCCESS

  EXPECT_CALL (
    WaitForMsgOutTxDoneMock,
    WaitForMsgOutTxDone (
      RetimerPtr,
      _,
      0
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_RETRY));

  TbtStatus = WriteIecs (DevComRetimer, RegNum, &CmdData, Length);
  EXPECT_EQ (TbtStatus, TBT_STATUS_RETRY);
}

//
// Case 6 - Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (WriteIecsTest, CorrectFlow) {
  cout << "[---------- Case 6 ----------]" << endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      Length,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call WaitForMsgOutTxDone
  //
  EXPECT_CALL (
    WaitForMsgOutTxDoneMock,
    WaitForMsgOutTxDone (
      RetimerPtr,
      _,
      0
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  TbtStatus = WriteIecs (DevComRetimer, RegNum, &CmdData, Length);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
