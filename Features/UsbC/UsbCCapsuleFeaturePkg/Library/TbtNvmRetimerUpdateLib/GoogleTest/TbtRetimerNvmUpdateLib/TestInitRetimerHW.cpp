/** @file
  Google test for InitRetimerHW function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <GTestTbtRetimerNvmUpdateLib.h>
#include <GoogleTest/Private/MockTbtNvmDrvRetimerThruHr/MockTbtNvmDrvRetimerThruHr.h>

// **********************************************************
// InitRetimerHW Unit Test                                  *
// **********************************************************
class InitRetimerHWTest : public Test {
  protected:
    EFI_STATUS                 Status;
    RETIMER_DEV_INSTANCE       RetimerDevice;
    MockTbtNvmDrvRetimerThruHr TbtNvmDrvRetimerThruHrMock;
    TBT_RETIMER *gDevComRetimerMock = &LocalCommunicationPtr;
};

//
// Case 1 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (InitRetimerHWTest, 1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  Status = InitRetimerHW (NULL);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (InitRetimerHWTest, 2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  RetimerDevice = (RETIMER_DEV_INSTANCE) gDevComRetimerMock;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_InitHW (
      gDevComRetimerMock
      )
    );

  Status = InitRetimerHW (RetimerDevice);
  EXPECT_EQ (Status, EFI_SUCCESS);
}

//
// Case 3 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (InitRetimerHWTest, 3) {
  cout << "[---------- Case 3 ----------]"<< endl;

  RetimerDevice = (RETIMER_DEV_INSTANCE) gDevComRetimerMock;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_InitHW (_)
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  Status = InitRetimerHW (RetimerDevice);
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}
