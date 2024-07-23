/** @file
  Google test for TbtNvmDrvHrWriteCioReg function.

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
// TbtNvmDrvHrWriteCioReg Unit Test                        *
//**********************************************************
class TbtNvmDrvHrWriteCioRegTest : public CommonMock {
  protected:
    TBT_STATUS            TbtStatus;
    TBT_HOST_ROUTER       *HrPtr;
    TBT_HR_IMPL           *HrImplPtr;
    UINT8                 ConfigurationSpace;
    UINT8                 Adapter;
    UINT16                RegNum;
    UINT8                 Length;
    UINT32                Data;

    void SetUp() override {
      HrPtr              = (TBT_HOST_ROUTER *) AllocateZeroPool (sizeof (TBT_HOST_ROUTER));
      HrImplPtr          = (TBT_HR_IMPL *) AllocateZeroPool (sizeof (TBT_HR_IMPL));
      ConfigurationSpace = ADAPTER_CONFIG_SPACE;
      Adapter            = 1;
      Length             = 1;
      RegNum             = 0x8;
      HrImplPtr->pDma    = gDmaPtrMock;
      HrPtr->Impl        = HrImplPtr;
    }
    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (HrPtr);
      FreePool (HrImplPtr);
    }
};

//
// Case 1 - Length > 16
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvHrWriteCioRegTest, LengthError) {
  cout << "[---------- Case 1 ----------]"<< endl;
  Length = 32;

  TbtStatus = TbtNvmDrvHrWriteCioReg (
                HrPtr,
                ConfigurationSpace,
                Adapter,
                RegNum,
                Length,
                &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - Call DmaPtr->TxCfgPkt return error.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvHrWriteCioRegTest, TxCfgPktError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  //
  //  Mock call TbtNvmDrvTxCfgPkt
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvTxCfgPkt (
      gDmaPtrMock,
      PDF_WRITE_CONFIGURATION_REGISTERS,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //
  //  Mock call TbtNvmDrvDmaDtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaDebugPrint (
      gDmaPtrMock
    ));

  TbtStatus = TbtNvmDrvHrWriteCioReg (
                HrPtr,
                ConfigurationSpace,
                Adapter,
                RegNum,
                Length,
                &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - Call DmaPtr->RxCfgPkt return error.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvHrWriteCioRegTest, RxCfgPktError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  //
  //  Mock call TbtNvmDrvTxCfgPkt
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvTxCfgPkt (
      gDmaPtrMock,
      PDF_WRITE_CONFIGURATION_REGISTERS,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvRxCfgPkt
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvRxCfgPkt (
      gDmaPtrMock,
      PDF_WRITE_CONFIGURATION_REGISTERS,
      _,
      NULL))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //
  //  Mock call TbtNvmDrvDmaDtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaDebugPrint (
      gDmaPtrMock
    ));

  TbtStatus = TbtNvmDrvHrWriteCioReg (
                HrPtr,
                ConfigurationSpace,
                Adapter,
                RegNum,
                Length,
                &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - Correct Flow.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvHrWriteCioRegTest, CorrectFlow) {
  cout << "[---------- Case 4 ----------]"<< endl;

  //
  //  Mock call TbtNvmDrvTxCfgPkt
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvTxCfgPkt (
      gDmaPtrMock,
      PDF_WRITE_CONFIGURATION_REGISTERS,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvRxCfgPkt
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvRxCfgPkt (
      gDmaPtrMock,
      PDF_WRITE_CONFIGURATION_REGISTERS,
      _,
      NULL))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  TbtStatus = TbtNvmDrvHrWriteCioReg (
                HrPtr,
                ConfigurationSpace,
                Adapter,
                RegNum,
                Length,
                &Data);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
