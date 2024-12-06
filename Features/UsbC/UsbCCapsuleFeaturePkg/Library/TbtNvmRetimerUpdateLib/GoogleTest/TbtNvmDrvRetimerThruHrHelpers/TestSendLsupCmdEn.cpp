/** @file
  Google test for SendLsupCmdEn function.

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
#include <GoogleTest/Private/MockTbtNvmDrvRetimerThruHr/MockTbtNvmDrvRetimerThruHr.h>

struct MockTbtNvmDrvSendCmd {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvSendCmd);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvSendCmd,
    (IN VOID          *DevCom,
     IN UINT32        Cmd,
     IN BOOLEAN       ResponseRequired)
    );
};

// **********************************************************
// SendLsupCmdEn Unit Test                                  *
// **********************************************************
class SendLsupCmdEnTest : public Test {
  protected:
    TBT_STATUS                   TbtStatus;
    RETIMER_THRU_HR              *RetimerPtr;
    TBT_RETIMER                  *gDevComRetimerMock = &LocalCommunicationPtr;
    TBT_HOST_ROUTER              *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvHr              TbtNvmDrvHrMock;
    MockTbtNvmDrvSendCmd         TbtNvmDrvSendCmdMock;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;

  void SetUp() override {
    RetimerPtr                       = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    RetimerPtr->TbtPort              = FIRST_MASTER_LANE;
    RetimerPtr->Comm                 = gDevComRetimerMock;
    RetimerPtr->Hr                   = gDevComHostMock;
    RetimerPtr->CascadedRetimerIndex = 1;
  }

  void TearDown () override {
    FreePool (RetimerPtr);
  }
};

//
// Case 1 - TbtNvmDrvSendCmd return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendLsupCmdEnTest, TbtNvmDrvSendCmdError) {
  cout << "[---------- Case 1 ----------]" << endl;

  EXPECT_CALL (
    TbtNvmDrvSendCmdMock,
    TbtNvmDrvSendCmd (
      (VOID *) RetimerPtr->Comm,
      TBT_IECS_CMD_LSUP,
      TRUE
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = SendLsupCmdEn (RetimerPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (SendLsupCmdEnTest, CorrectFlow) {
  cout << "[---------- Case 2 ----------]" << endl;

  EXPECT_CALL (
    TbtNvmDrvSendCmdMock,
    TbtNvmDrvSendCmd (
      (VOID *) RetimerPtr->Comm,
      TBT_IECS_CMD_LSUP,
      TRUE
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));

  TbtStatus = SendLsupCmdEn (RetimerPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
