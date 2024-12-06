/** @file
  Google test for TbtDrvReadNvmVersion function.

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
#include <GTestTbtNvmRetimerDrvHelpers.h>
#include <GoogleTest/Private/MockTbtNvmDrvRetimerThruHr/MockTbtNvmDrvRetimerThruHr.h>
// **********************************************************
// TbtDrvReadNvmVersion Unit Test                           *
// **********************************************************
class TbtDrvReadNvmVersionTest : public Test {
  protected:
    EFI_STATUS                 Status;
    VOID                       *DevComRetimer;
    UINT32                     Version;
    UINT32                     ExpReadData;
    TBT_RETIMER                *gDevComRetimerMock = &LocalCommunicationPtr;
    MockTbtNvmDrvRetimerThruHr TbtNvmDrvRetimerThruHrMock;

  void SetUp() override {
    DevComRetimer = (VOID *) gDevComRetimerMock;
    Version       = 0x123456;
  }
};

//
// Case 1 - Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (TbtDrvReadNvmVersionTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  Status = TbtDrvReadNvmVersion (NULL, &Version);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - TARGET_RETIMER WriteIecsReg return Error
// Expected Result - TbtStatus will return EFI_DEVICE_ERROR
//
TEST_F (TbtDrvReadNvmVersionTest, WriteIecsRegError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;
  //
  // Mock call WriteIecs for TbtDrvReadDwFromNvm
  //
  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  Status = TbtDrvReadNvmVersion (DevComRetimer, &Version);
  EXPECT_EQ (Status, EFI_DEVICE_ERROR);
}

//
// Case 3 - TARGET_RETIMER first time call WriteIecsReg return Error
// Expected Result - TbtStatus will return EFI_SUCCESS
//
TEST_F (TbtDrvReadNvmVersionTest, CorrectFlow) {
  cout << "[---------- Case 3 ----------]"<< endl;

  gUpdateTargetType = TARGET_RETIMER;
  //
  // Mock call WriteIecs for TbtDrvReadDwFromNvm
  //
  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_METADATA_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));
  //
  // Mock call WriteIecsReg for TbtNvmDrvSendCmd
  //
  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_WriteIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      _,
      1
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));
  //
  // Mock call ReadIecsReg for TbtNvmDrvWaitForCmdCpl in TbtNvmDrvSendCmd
  //
  ExpReadData = OPCODE_NVM_READ_CMD;
  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_CMD_ADDR,
      1,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<3>(&ExpReadData, sizeof (ExpReadData)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  EXPECT_CALL (
    TbtNvmDrvRetimerThruHrMock,
    MockThruHr_ReadIecs (
      gDevComRetimerMock,
      IECS_MSG_OUT_RDATA,
      _,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  Status = TbtDrvReadNvmVersion (DevComRetimer, &Version);
  EXPECT_EQ (Status, EFI_SUCCESS);
}
