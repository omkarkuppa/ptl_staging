/** @file
  Google test for CapabilityParsing function.

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
// CapabilityParsing Unit Test                             *
//**********************************************************
class CapabilityParsingTest : public CommonMock {
  protected:
    TBT_STATUS            TbtStatus;
    RETIMER_THRU_HR       *RetimerPtr;
    TBT_RETIMER           *DevComRetimer;

    UINT32                ExpData1;
    UINT32                ExpData2;
    UINT32                ExpData3;

    void SetUp() override {
      RetimerPtr               = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
      RetimerPtr->Hr           = gDevComHostMock;                        // For call Mock WriteCioDevReg
      DevComRetimer            = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
      DevComRetimer->Impl      = RetimerPtr;                             // For RetimerPtr = (RETIMER_THRU_HR *)This->Impl
    }

    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (RetimerPtr);
      FreePool (DevComRetimer);
    }
};

//
// Case 1 -  Assert Error
// Expected Result - TbtStatus will return TBT_STATUS_INVALID_PARAM
//
TEST_F (CapabilityParsingTest, AssertError) {
  cout << "[---------- Case 1 ----------]"<< endl;

  TbtStatus = CapabilityParsing (NULL);
  EXPECT_EQ (TbtStatus, TBT_STATUS_INVALID_PARAM);
}

//
// Case 2 - First time ADAPTER_CONFIG_SPACE call ReadCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (CapabilityParsingTest, FirstTimeReadCioDevRegError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));

  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _))
    .WillRepeatedly (Return (EFI_SUCCESS));

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      0x1,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - ERROR, USB4 Port Capability was not found.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (CapabilityParsingTest, USB4_PortCapabilityWasNotFound) {
  cout << "[---------- Case 3 ----------]"<< endl;

  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));

  ExpData2 = 0x0;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      0x1,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData2, sizeof(ExpData2)),
        Return (TBT_STATUS_SUCCESS)
    ));

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 4 - Second time ADAPTER_CONFIG_SPACE call ReadCioDevReg return error
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (CapabilityParsingTest, SecondTimeReadCioDevRegError) {
  cout << "[---------- Case 4 ----------]"<< endl;

  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));

  ExpData2 = 0x6;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData2, sizeof(ExpData2)),
        Return (TBT_STATUS_SUCCESS)
    ))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - Correct Flow
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (CapabilityParsingTest, CorrectFlow) {
  cout << "[---------- Case 5 ----------]"<< endl;

  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));

  ExpData2 = 0x6;
  ExpData3 = 0x600;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData2, sizeof(ExpData2)),
        Return (TBT_STATUS_SUCCESS)
    ))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData3, sizeof(ExpData3)),
        Return (TBT_STATUS_SUCCESS)
    ));

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}
/*
//
// Case 6 - Failed to parse capabilities
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (CapabilityParsingTest, FailedToParseCapabilities) {
  cout << "[---------- Case 6 ----------]"<< endl;

  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));

  ExpData2 = 0x6;
  ExpData3 = 0x6;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      RetimerPtr->Hr,
      ADAPTER_CONFIG_SPACE,
      (UINT8) RetimerPtr->TbtPort,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData2, sizeof(ExpData2)),
        Return (TBT_STATUS_SUCCESS)
    ))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpData3, sizeof(ExpData3)),
        Return (TBT_STATUS_SUCCESS)
    ));

  TbtStatus = CapabilityParsing (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}
*/