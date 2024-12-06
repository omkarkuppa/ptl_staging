/** @file
  Google test for SendOfflineMode function.

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
#include <GTestTbtNvmDRThruHrHelpers.h>
#include <GoogleTest/Library/MockUsbcCapsuleDebugLib.h>
#include <GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>

// **********************************************************
// SendOfflineMode Unit Test                                *
// **********************************************************
class SendOfflineModeTest : public Test {
  protected:
    EFI_STATUS                   Status;
    RETIMER_THRU_HR              *HrDevice;
    UINT32                       Data;
    UINT8                        ConfigurationSpace;
    TBT_HOST_ROUTER              *gDevComHostMock = &LocalHrPtr;
    MockUsbcCapsuleDebugLib      UsbcCapsuleDebugLibMock;
    MockTbtNvmDrvHr              TbtNvmDrvHrMock;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;

  void SetUp() override {
    HrDevice           = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    HrDevice->TbtPort  = FIRST_MASTER_LANE;
    Data               = 0;
    ConfigurationSpace = ADAPTER_CONFIG_SPACE;
    HrDevice->Hr       = gDevComHostMock;
  }

  void TearDown() override {
    FreePool (HrDevice);
  }
};

//
// Case 1 - General Case , call SendCommandToLocalLc return TBT_STATUS_NON_RECOVERABLE_ERROR
// Expected Result - Status will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (SendOfflineModeTest, case1) {
  cout << "[---------- Case 1 ----------]" << endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (2);

  //for SendCommandToLocalLc >>>
  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      HrDevice->Hr,
      ConfigurationSpace,
      (UINT8) HrDevice->TbtPort,
      _,
      1,
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));
  //for SendCommandToLocalLc <<<

  Status = SendOfflineMode (HrDevice, Data);
  EXPECT_EQ (Status, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - General Case , call SendCommandToLocalLc return TBT_STATUS_SUCCESS
// Expected Result - Status will return TBT_STATUS_SUCCESS
//
TEST_F (SendOfflineModeTest, case2) {
  cout << "[---------- Case 2 ----------]" << endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite);

  //for SendCommandToLocalLc >>>
  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_WriteCioReg (
      HrDevice->Hr,
      ConfigurationSpace,
      (UINT8) HrDevice->TbtPort,
      _,
      1,
      _
      )
    )
    .Times (5)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //  Mock call ReadCioDevReg for WaitForMsgOutTxDone >>>
  Data = TBT_MSG_OUT_ATCT1_LT0;

  EXPECT_CALL (
    TbtNvmDrvHrMock,
    TbtNvmDrvHr_ReadCioDevReg (
      HrDevice->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) HrDevice->TbtPort,
      _,
      _
      )
    )
    .Times (4)
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&Data, sizeof (Data)),
         Return (TBT_STATUS_SUCCESS)
         )
       )
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .Times (4)
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS))
    .WillOnce (Return (TBT_STATUS_SUCCESS));
  //for SendCommandToLocalLc <<<

  Status = SendOfflineMode (HrDevice, Data);
  EXPECT_EQ (Status, TBT_STATUS_SUCCESS);
}
