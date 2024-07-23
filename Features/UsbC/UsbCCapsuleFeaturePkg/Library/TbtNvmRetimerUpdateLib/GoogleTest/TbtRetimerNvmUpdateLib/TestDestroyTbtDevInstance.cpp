/** @file
  Google test for DestroyTbtDevInstance function.

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
// DestroyTbtDevInstance Unit Test                         *
//**********************************************************
class DestroyTbtDevInstanceTest : public CommonMock {
  protected:
    EFI_STATUS                     Status;
    DISCRETE_TBT_DEV_INSTANCE      DiscreteTbtDevice;
};

//
// Case 1 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (DestroyTbtDevInstanceTest, CorrectFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  DiscreteTbtDevice = (DISCRETE_TBT_DEV_INSTANCE) gDevComHostMock;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrDtor (
      gDevComHostMock
      ));
  Status = DestroyTbtDevInstance (
             DiscreteTbtDevice);
  EXPECT_EQ (Status, EFI_SUCCESS);
}
