/** @file
  Google test for StateFromDriver function.

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
// StateFromDriver Unit Test                               *
//**********************************************************
class StateFromDriverTest : public CommonMock {
  protected:
    TBT_STATUS            TbtStatus;
    RETIMER_THRU_HR       *RetimerPtr;
    TBT_RETIMER           *DevComRetimer;
    DRIVER_STATE          State;

    void SetUp() override {
      RetimerPtr               = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
      RetimerPtr->Hr           = gDevComHostMock;                        // For call Mock WriteCioDevReg
      DevComRetimer            = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
      DevComRetimer->Impl      = RetimerPtr;                             // For RetimerPtr = (RETIMER_THRU_HR *)This->Impl
      State                    = AFTER_AUTH;
    }

    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (RetimerPtr);
      FreePool (DevComRetimer);
    }
};

//
// Case 1 - Correct Flow
//
TEST_F (StateFromDriverTest, CorrectFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  //
  //  Mock call SendEnumCmd
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendEnumCmd (
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  StateFromDriver (DevComRetimer, State);
}
