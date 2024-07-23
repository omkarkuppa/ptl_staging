/** @file
  Google test for Dtor function.

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
// Dtor Unit Test                                          *
//**********************************************************
class DtorTest : public CommonMock {
  protected:
    TBT_STATUS            TbtStatus;
    RETIMER_THRU_HR       *RetimerPtr;
    TBT_RETIMER           *DevComRetimer;

    void SetUp() override {
      RetimerPtr               = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
      RetimerPtr->Hr           = gDevComHostMock;                        // For call Mock WriteCioDevReg
      DevComRetimer            = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
      DevComRetimer->Impl      = RetimerPtr;                             // For RetimerPtr = (RETIMER_THRU_HR *)This->Impl
    }
};

//
// Case 1 - Destroy fail Flow
//
TEST_F (DtorTest, DestroyFailFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  //
  //  Mock call SendLsupCmdDis
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendLsupCmdDis (
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));
  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendCommandToLocalLc (
      _,
      ADAPTER_CONFIG_SPACE,
      TBT_IECS_CMD_LSEN,
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));
  //
  // Mock call RetimerPtr->Hr->Dtor (RetimerPtr->Hr)
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrDtor (
      gDevComHostMock
      ));

  Dtor (DevComRetimer);
}
