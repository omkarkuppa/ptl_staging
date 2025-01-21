/** @file
  Google test for DestroyRetimerDevInstance function.

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

#include <GTestTbtRetimerNvmUpdateLib.h>
#include <GoogleTest/Private/MockTbtNvmDrvRetimerThruHr/MockTbtNvmDrvRetimerThruHr.h>

// **********************************************************
// DestroyRetimerDevInstance Unit Test                      *
// **********************************************************
class DestroyRetimerDevInstanceTest : public Test {
  protected:
    EFI_STATUS                 Status;
    RETIMER_DEV_INSTANCE       RetimerDevice;
    MockTbtNvmDrvRetimerThruHr TbtNvmDrvRetimerThruHrMock;
    TBT_RETIMER                *gDevComRetimerMock = &LocalCommunicationPtr;
};

//
// Case 1 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (DestroyRetimerDevInstanceTest, CorrectFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  RetimerDevice = (RETIMER_DEV_INSTANCE) gDevComRetimerMock;

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_Destroy (
      gDevComRetimerMock
      )
    );

  Status = DestroyRetimerDevInstance (RetimerDevice);
  EXPECT_EQ (Status, EFI_SUCCESS);
}
