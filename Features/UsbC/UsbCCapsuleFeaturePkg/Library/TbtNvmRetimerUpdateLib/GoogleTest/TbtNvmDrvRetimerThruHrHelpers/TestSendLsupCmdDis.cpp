/** @file
  Google test for SendLsupCmdDis function.

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
// SendLsupCmdDis Unit Test                                   *
//**********************************************************
class SendLsupCmdDisTest : public CommonMock {
  protected:
    TBT_STATUS            TbtStatus;
    RETIMER_THRU_HR       *RetimerPtr;

    void SetUp() override {
      RetimerPtr                       = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
      RetimerPtr->TbtPort              = FIRST_MASTER_LANE;
      RetimerPtr->Comm                 = gDevComRetimerMock;
      RetimerPtr->Hr                   = gDevComHostMock;
      RetimerPtr->CascadedRetimerIndex = 1;
    }

    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (RetimerPtr);
    }
};

//
// Case 1 - TbtNvmDrvSendCmd return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendLsupCmdDisTest, TbtNvmDrvSendCmdError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvSendCmd (
      (VOID *) RetimerPtr->Comm,
      TBT_IECS_CMD_USUP,
      FALSE))
    .WillRepeatedly (Return(TBT_STATUS_NON_RECOVERABLE_ERROR)); 

  TbtStatus = SendLsupCmdDis (RetimerPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (SendLsupCmdDisTest, CorrectFlow) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (TbtNvmRetimerDrvHelpersMock,
    TbtNvmDrvSendCmd (
      (VOID *) RetimerPtr->Comm,
      TBT_IECS_CMD_USUP,
      FALSE))
    .WillRepeatedly (Return(TBT_STATUS_SUCCESS)); 

  TbtStatus = SendLsupCmdDis (RetimerPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
