/** @file
  Google test for ReadIecs function.

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
extern "C" {
TBT_STATUS
ReadIecs (
  IN  TBT_RETIMER  *This,
  IN  UINT8        RegNum,
  IN  UINT32       DataLength,
  OUT UINT32       *DataPtr
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

MOCK_INTERFACE_DEFINITION (MockWaitForMsgOutTxDone);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockWaitForMsgOutTxDone, WaitForMsgOutTxDone, 3, EFIAPI);

// **********************************************************
// ReadIecs Unit Test                                       *
// **********************************************************
class ReadIecsTest : public Test {
  protected:
    TBT_STATUS              TbtStatus;
    RETIMER_THRU_HR         *RetimerPtr;
    TBT_RETIMER             *DevComRetimer;
    UINT8                   RegNum;
    UINT32                  CmdData;
    UINT8                   Length;
    TBT_HOST_ROUTER         *gDevComHostMock = &LocalHrPtr;
    MockWaitForMsgOutTxDone WaitForMsgOutTxDoneMock;
    MockTbtNvmDrvHr         TbtNvmDrvHrMock;

  void SetUp() override {
    RetimerPtr          = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    RetimerPtr->Hr      = gDevComHostMock;
    DevComRetimer       = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
    DevComRetimer->Impl = RetimerPtr;
    RegNum              = IECS_CMD_ADDR;
    CmdData             = IECS_BLKW_CMD;
    Length              = 1;
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
TEST_F (ReadIecsTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = ReadIecs (DevComRetimer, RegNum, Length, NULL);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - First time call WriteCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (ReadIecsTest, WriteCioDevRegError) {
  cout << "[---------- Case 2 ----------]"<< endl;

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
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = ReadIecs (DevComRetimer, RegNum, Length, &CmdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - Call WaitForMsgOutTxDone return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (ReadIecsTest, WaitForMsgOutTxDone_NON_RECOVERABLE_ERROR) {
  cout << "[---------- Case 3 ----------]"<< endl;

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
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));
  //
  //  Mock call WaitForMsgOutTxDone
  //
  EXPECT_CALL (
    WaitForMsgOutTxDoneMock,
    WaitForMsgOutTxDone (
      RetimerPtr,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = ReadIecs (DevComRetimer, RegNum, Length, &CmdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 -  Call WaitForMsgOutTxDone return TBT_STATUS_RETRY
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (ReadIecsTest, WaitForMsgOutTxDoneRETRY) {
  cout << "[---------- Case 4 ----------]"<< endl;

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
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    WaitForMsgOutTxDoneMock,
    WaitForMsgOutTxDone (
      RetimerPtr,
      _,
      1
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_RETRY));

  TbtStatus = ReadIecs (DevComRetimer, RegNum, Length, &CmdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - Call ReadCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (ReadIecsTest, ReadCioDevRegError) {
  cout << "[---------- Case 5 ----------]"<< endl;

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
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    WaitForMsgOutTxDoneMock,
    WaitForMsgOutTxDone (
      RetimerPtr,
      _,
      1
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));

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

  TbtStatus = ReadIecs (DevComRetimer, RegNum, Length, &CmdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 6 - Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (ReadIecsTest, CorrectFlow) {
  cout << "[---------- Case 6 ----------]"<< endl;

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
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    WaitForMsgOutTxDoneMock,
    WaitForMsgOutTxDone (
      RetimerPtr,
      _,
      1
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));

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
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  TbtStatus = ReadIecs (DevComRetimer, RegNum, Length, &CmdData);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
