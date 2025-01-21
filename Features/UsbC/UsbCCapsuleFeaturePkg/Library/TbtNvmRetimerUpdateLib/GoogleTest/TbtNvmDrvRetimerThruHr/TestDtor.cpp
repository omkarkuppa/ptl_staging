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
#include <GTestTbtNvmDrvRetimerThruHr.h>
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>

extern "C" {
VOID
Dtor (
  IN TBT_RETIMER  *This
  );
}
// **********************************************************
// Dtor Unit Test                                           *
// **********************************************************
class DtorTest : public Test {
  protected:
    TBT_STATUS      TbtStatus;
    RETIMER_THRU_HR *RetimerPtr;
    TBT_RETIMER     *DevComRetimer;
    TBT_HOST_ROUTER *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvHr TbtNvmDrvHrMock;

  void SetUp() override {
    RetimerPtr          = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    RetimerPtr->Hr      = gDevComHostMock;                                 // For call Mock WriteCioDevReg
    DevComRetimer       = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
    DevComRetimer->Impl = RetimerPtr;                                      // For RetimerPtr = (RETIMER_THRU_HR *)This->Impl
  }
};

//
// Case 1 - Destroy fail Flow
//
TEST_F (DtorTest, DestroyFailFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_Dtor (gDevComHostMock)
    );

  Dtor (DevComRetimer);
}
