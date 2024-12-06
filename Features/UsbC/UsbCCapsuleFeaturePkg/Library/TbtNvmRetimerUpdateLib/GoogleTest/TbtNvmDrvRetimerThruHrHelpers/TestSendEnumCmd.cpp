/** @file
  Google test for SendEnumCmd function.

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

#include <GTestTbtNvmDRThruHrHelpers.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>

// **********************************************************
// SendEnumCmd Unit Test                                    *
// **********************************************************
class SendEnumCmdTest : public Test {
  protected:
    TBT_STATUS                   TbtStatus;
    RETIMER_THRU_HR              *RetimerPtr;
    UINT32                       ExpData1;
    UINT32                       ExpData2;
    TBT_HOST_ROUTER              *gDevComHostMock = &LocalHrPtr;
    MockTbtNvmDrvHr              TbtNvmDrvHrMock;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;

  void SetUp() override {
    RetimerPtr          = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    RetimerPtr->TbtPort = FIRST_MASTER_LANE;
    RetimerPtr->Hr      = gDevComHostMock;
  }

  void TearDown() override {
    FreePool (RetimerPtr);
  }
};

//
// Case 1 - SendCommandToLocalLc return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendEnumCmdTest, SendCommandToLocalLcError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = SendEnumCmd (RetimerPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - Correct flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (SendEnumCmdTest, CorrectFlow) {
  cout << "[---------- Case 2 ----------]"<< endl;

  //
  //  Mock call WriteCioReg for SendCommandToLocalLc
  //
  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      1,
      _
      )
    )
    .WillRepeatedly (Return (TBT_STATUS_SUCCESS));
  //
  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone in SendCommandToLocalLc
  //
  ExpData1 = TBT_MSG_OUT_ATCT1_LT0;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<4>(&ExpData1, sizeof (ExpData1)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  ExpData2 = TBT_LC_CMD_SUCCESS;
  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<4>(&ExpData2, sizeof (ExpData2)),
         Return (TBT_STATUS_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillRepeatedly (Return (EFI_SUCCESS));

  TbtStatus = SendEnumCmd (RetimerPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
