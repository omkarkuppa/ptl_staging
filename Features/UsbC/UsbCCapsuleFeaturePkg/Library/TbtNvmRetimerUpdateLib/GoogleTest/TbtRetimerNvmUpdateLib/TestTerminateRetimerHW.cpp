/** @file
  Google test for TerminateRetimerHW function.

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
// TerminateRetimerHW Unit Test                             *
// **********************************************************
class TerminateRetimerHWTest : public Test {
  protected:
    EFI_STATUS                 Status;
    RETIMER_DEV_INSTANCE       RetimerDevice;
    MockTbtNvmDrvRetimerThruHr TbtNvmDrvRetimerThruHrMock;
    TBT_RETIMER *gDevComRetimerMock = &LocalCommunicationPtr;
};

//
// Case 1 - RetimerDevice == NULL
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (TerminateRetimerHWTest, Case1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  RetimerDevice = (RETIMER_DEV_INSTANCE) gDevComRetimerMock;

  Status = TerminateRetimerHW (NULL);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - Device->TerminateHW(Device) return error
// Mock - TbtNvmDrvRetimerThruHrMock
// Expected Result - Status will return EFI_DEVICE_ERROR
//
TEST_F (TerminateRetimerHWTest, Case2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  RetimerDevice = (RETIMER_DEV_INSTANCE) gDevComRetimerMock;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_TerminateHW (_)
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  Status = TerminateRetimerHW (RetimerDevice);
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 3 - Device->TerminateHW(Device) not return error
// Mock - TbtNvmDrvRetimerThruHrMock
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (TerminateRetimerHWTest, Case3) {
  cout << "[---------- Case 3 ----------]"<< endl;

  RetimerDevice = (RETIMER_DEV_INSTANCE) gDevComRetimerMock;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_TerminateHW (
      gDevComRetimerMock
      )
    );

  Status = TerminateRetimerHW (RetimerDevice);
  EXPECT_EQ (Status, EFI_SUCCESS);
}
