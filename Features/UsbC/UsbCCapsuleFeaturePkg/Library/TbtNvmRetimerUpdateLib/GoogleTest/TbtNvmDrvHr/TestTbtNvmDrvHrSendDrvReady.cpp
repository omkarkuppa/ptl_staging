/** @file
  Google test for TbtNvmDrvHrSendDrvReady function.

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
#include <GTestTbtNvmDrvHr.h>
#include <GoogleTest/Private/MockTbtNvmDrvDma/MockTbtNvmDrvDma.h>

extern "C" {
TBT_STATUS
TbtNvmDrvHrSendDrvReady (
  TBT_DMA  *DmaPtr
  );
}
// **********************************************************
// TbtNvmDrvHrSendDrvReady Unit Test                        *
// **********************************************************
class TbtNvmDrvHrSendDrvReadyTest : public Test {
  protected:
    TBT_STATUS       TbtStatus;
    TBT_DMA          *DmaPtr;
    MockTbtNvmDrvDma TbtNvmDrvDmaMock;
    TBT_DMA          *gDmaPtrMock = &LocalDmaPtr;

  void SetUp() override {
    DmaPtr = gDmaPtrMock;
  }
};

//
// Case 1 - DmaPtr is NUll.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvHrSendDrvReadyTest, DmaPtrIsNullError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = TbtNvmDrvHrSendDrvReady (NULL);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - Call DmaPtr->TxCfgPkt return error.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvHrSendDrvReadyTest, TxCfgPktError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvDmaMock,
    TbtNvmDrvTxCfgPkt (
      DmaPtr,
      PDF_SW_TO_FW_COMMAND,
      sizeof (DRV_RDY_CMD),
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvHrSendDrvReady (DmaPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - Call DmaPtr->RxCfgPkt return error.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvHrSendDrvReadyTest, RxCfgPktError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvDmaMock,
    TbtNvmDrvTxCfgPkt (
      DmaPtr,
      PDF_SW_TO_FW_COMMAND,
      sizeof (DRV_RDY_CMD),
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvDmaMock,
    TbtNvmDrvRxCfgPkt (
      DmaPtr,
      PDF_FW_TO_SW_RESPONSE,
      NULL,
      NULL
      )
    )
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TbtNvmDrvHrSendDrvReady (DmaPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvHrSendDrvReadyTest, CorrectFlow) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (
    TbtNvmDrvDmaMock,
    TbtNvmDrvTxCfgPkt (
      DmaPtr,
      PDF_SW_TO_FW_COMMAND,
      sizeof (DRV_RDY_CMD),
      _
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvDmaMock,
    TbtNvmDrvRxCfgPkt (
      DmaPtr,
      PDF_FW_TO_SW_RESPONSE,
      NULL,
      NULL
      )
    )
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  TbtStatus = TbtNvmDrvHrSendDrvReady (DmaPtr);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
