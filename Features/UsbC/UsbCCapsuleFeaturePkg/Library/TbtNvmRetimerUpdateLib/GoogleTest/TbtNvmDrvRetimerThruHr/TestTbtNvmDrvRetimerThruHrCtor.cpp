/** @file
  Google test for TbtNvmDrvRetimerThruHrCtor function.

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
// TbtNvmDrvRetimerThruHrCtor Unit Test                    *
//**********************************************************
class TbtNvmDrvRetimerThruHrCtorTest : public CommonMock {
  protected:
    TBT_RETIMER           *DevComRetimer;
    UINT8                 FirmwareType;
    PCIE_BDF              *TbtDmaPciLocation;
    TBT_PORT              TbtPort;
    UINT32                CascadedRetimerIndex;
    PCIE_RP_CONFIG        *PcieRpConfig;
    FORCE_PWR_HR          ForcePwrFunc;

    TBT_HOST_ROUTER       *HrPtr;
    UINT32                ExpData1;
    UINT32                ExpData2;
    UINT32                ExpData3;

    void SetUp() override {
      FirmwareType              = 0x0;
      TbtDmaPciLocation         = (PCIE_BDF *) AllocateZeroPool (sizeof (PCIE_BDF));
      TbtPort                   = PORT_A;
      CascadedRetimerIndex      = 0x1;
      PcieRpConfig              = (PCIE_RP_CONFIG *) AllocateZeroPool (sizeof (PCIE_RP_CONFIG));
      HrPtr                     = gDevComHostMock;               // Mock RetimerPtr->Hr = HrPtr; for Case 2.
    }
};

//
// Case 1 - Call TbtNvmDrvHrCtor return NULL
// Expected Result - TbtNvmDrvRetimerThruHrCtor will return NULL
//
TEST_F (TbtNvmDrvRetimerThruHrCtorTest, TbtNvmDrvHrCtorNull) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrCtor (
      FirmwareType,
      _,
      _,
      ForcePwrFunc
      ))
    .WillOnce (Return(NULL));

  DevComRetimer = TbtNvmDrvRetimerThruHrCtor (
                    FirmwareType,
                    TbtDmaPciLocation,
                    TbtPort,
                    CascadedRetimerIndex,
                    PcieRpConfig,
                    ForcePwrFunc
                    );
  EXPECT_EQ (DevComRetimer, NULL);
}

//
// Case 2 - Call CapabilityParsing return error.
// Expected Result - TbtNvmDrvRetimerThruHrCtor will return NULL
//
TEST_F (TbtNvmDrvRetimerThruHrCtorTest, CapabilityParsingError) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrCtor (
      FirmwareType,
      _,
      _,
      ForcePwrFunc
      ))
    .WillOnce (Return(HrPtr));

  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      ADAPTER_CONFIG_SPACE,
      _,
      0x1,
      _))
    .WillOnce (Return(TBT_STATUS_NON_RECOVERABLE_ERROR));

  //
  // Mock call HrPtr->Dtor (HrPtr);
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrDtor (
      gDevComHostMock
      ));

  DevComRetimer = TbtNvmDrvRetimerThruHrCtor (
                    FirmwareType,
                    TbtDmaPciLocation,
                    TbtPort,
                    CascadedRetimerIndex,
                    PcieRpConfig,
                    ForcePwrFunc
                    );
  EXPECT_EQ (DevComRetimer, NULL);
}

//
// Case 3 - Call SendCommandToLocalLc return error
// Expected Result - TbtNvmDrvRetimerThruHrCtor will return NULL
//
TEST_F (TbtNvmDrvRetimerThruHrCtorTest, SendCommandToLocalLcError) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrCtor (
      FirmwareType,
      _,
      _,
      ForcePwrFunc
      ))
    .WillOnce (Return(HrPtr));

  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData2 = 0x6;
  ExpData3 = 0x600;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      ADAPTER_CONFIG_SPACE,
      _,
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

  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendCommandToLocalLc (
      _,
      ADAPTER_CONFIG_SPACE,
      TBT_IECS_CMD_LSEN,
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //
  // Mock call HrPtr->Dtor (HrPtr);
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrDtor (
      gDevComHostMock
      ));

  DevComRetimer = TbtNvmDrvRetimerThruHrCtor (
                    FirmwareType,
                    TbtDmaPciLocation,
                    TbtPort,
                    CascadedRetimerIndex,
                    PcieRpConfig,
                    ForcePwrFunc
                    );
  EXPECT_EQ (DevComRetimer, NULL);
}

//
// Case 4 - Call SendEnumCmd return error
// Expected Result - TbtNvmDrvRetimerThruHrCtor will return NULL
//
TEST_F (TbtNvmDrvRetimerThruHrCtorTest, SendEnumCmdError) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrCtor (
      FirmwareType,
      _,
      _,
      ForcePwrFunc
      ))
    .WillOnce (Return(HrPtr));

  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData2 = 0x6;
  ExpData3 = 0x600;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      ADAPTER_CONFIG_SPACE,
      _,
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
  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendCommandToLocalLc (
      _,
      ADAPTER_CONFIG_SPACE,
      TBT_IECS_CMD_LSEN,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendEnumCmd (
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //
  // Mock call HrPtr->Dtor (HrPtr);
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrDtor (
      gDevComHostMock
      ));

  DevComRetimer = TbtNvmDrvRetimerThruHrCtor (
                    FirmwareType,
                    TbtDmaPciLocation,
                    TbtPort,
                    CascadedRetimerIndex,
                    PcieRpConfig,
                    ForcePwrFunc
                    );
  EXPECT_EQ (DevComRetimer, NULL);
}

//
// Case 5 - Call SendLsupCmdEn return error
// Expected Result - TbtNvmDrvRetimerThruHrCtor will return NULL
//
TEST_F (TbtNvmDrvRetimerThruHrCtorTest, SendLsupCmdEnError) {
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrCtor (
      FirmwareType,
      _,
      _,
      ForcePwrFunc
      ))
    .WillOnce (Return(HrPtr));

  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData2 = 0x6;
  ExpData3 = 0x600;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      ADAPTER_CONFIG_SPACE,
      _,
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
  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendCommandToLocalLc (
      _,
      ADAPTER_CONFIG_SPACE,
      TBT_IECS_CMD_LSEN,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendEnumCmd (
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendLsupCmdEn (
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //
  // Mock call HrPtr->Dtor (HrPtr);
  //
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrDtor (
      gDevComHostMock
      ));

  DevComRetimer = TbtNvmDrvRetimerThruHrCtor (
                    FirmwareType,
                    TbtDmaPciLocation,
                    TbtPort,
                    CascadedRetimerIndex,
                    PcieRpConfig,
                    ForcePwrFunc
                    );
  EXPECT_EQ (DevComRetimer, NULL);
}

//
// Case 6 - Correct Flow
// Expected Result - TbtNvmDrvRetimerThruHrCtor not return NULL
//
TEST_F (TbtNvmDrvRetimerThruHrCtorTest, CorrectFlow) {
  cout << "[---------- Case 6 ----------]"<< endl;

  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrCtor (
      FirmwareType,
      _,
      _,
      ForcePwrFunc
      ))
    .WillOnce (Return(HrPtr));

  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData1 = 0x8086;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      DEVICE_CONFIG_SPACE,
      0,
      0,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpData1, sizeof(ExpData1)),
        Return (TBT_STATUS_SUCCESS)
    ));
  //
  // Mock call ReadCioDevReg for CapabilityParsing
  //
  ExpData2 = 0x6;
  ExpData3 = 0x600;
  EXPECT_CALL (TbtNvmDrvHrMock,
    TbtNvmDrvHrReadCioDevReg (
      _,
      ADAPTER_CONFIG_SPACE,
      _,
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
  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendCommandToLocalLc (
      _,
      ADAPTER_CONFIG_SPACE,
      TBT_IECS_CMD_LSEN,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendEnumCmd (
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call SendCommandToLocalLc
  //
  EXPECT_CALL (TbtNvmDrvRetimerThruHrHelpersMock,
    SendLsupCmdEn (
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  DevComRetimer = TbtNvmDrvRetimerThruHrCtor (
                    FirmwareType,
                    TbtDmaPciLocation,
                    TbtPort,
                    CascadedRetimerIndex,
                    PcieRpConfig,
                    ForcePwrFunc
                    );
  EXPECT_NE (DevComRetimer, NULL);
}
