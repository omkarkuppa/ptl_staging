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

#include <GTestTbtNvmDrvRetimerThruHr.h>
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>

struct MockSendEnumCmd {
  MOCK_INTERFACE_DECLARATION (MockSendEnumCmd);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    SendEnumCmd,
    (RETIMER_THRU_HR *RetimerPtr)
    );
};
MOCK_INTERFACE_DEFINITION (MockSendEnumCmd);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockSendEnumCmd, SendEnumCmd, 1, EFIAPI);
//
// Test function define.
//
extern "C" {
VOID
StateFromDriver (
  IN TBT_RETIMER   *This,
  IN DRIVER_STATE  State
  );
}

// **********************************************************
// StateFromDriver Unit Test                                *
// **********************************************************
class StateFromDriverTest : public Test {
  protected:
    TBT_STATUS      TbtStatus;
    RETIMER_THRU_HR *RetimerPtr;
    TBT_RETIMER     *DevComRetimer;
    DRIVER_STATE    State;
    TBT_HOST_ROUTER *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvHr TbtNvmDrvHrMock;
    MockSendEnumCmd SendEnumCmdMock;

  void SetUp() override {
    RetimerPtr          = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    RetimerPtr->Hr      = gDevComHostMock;
    DevComRetimer       = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
    DevComRetimer->Impl = RetimerPtr;
    State               = AFTER_AUTH;
  }

  void TearDown() override {
    FreePool (RetimerPtr);
    FreePool (DevComRetimer);
  }
};

//
// Case 1 - Correct Flow
//
TEST_F (StateFromDriverTest, CorrectFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (
    SendEnumCmdMock,
    SendEnumCmd (
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  StateFromDriver (DevComRetimer, State);
}
