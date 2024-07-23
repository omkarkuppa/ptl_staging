/** @file
  Google test for TbtNvmDrvHrCtor function.

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
// TbtNvmDrvHrCtor Unit Test                               *
//**********************************************************
class TbtNvmDrvHrCtorTest : public CommonMock {
  protected:
    TBT_HOST_ROUTER       *HrPtr;
    UINT8                 FirmwareType;
    PCIE_RP_CONFIG        *PcieRpConfig;
    PCIE_BDF              *TbtDmaPcieBdf;
    FORCE_PWR_HR          ForcePwrFunc;

    EFI_HANDLE            ExpHandlesPtr;
    EFI_HANDLE            ExpHandles[5] = {
      0,0,0,0,0
    };
    UINTN                 ExpHandleCount;
    UINT64                ExpPcieRpBaseAddr;
    UINT8                 ExpSecondaryBus;
    UINT8                 ExpSubordinateBus;
    EFI_PCI_IO_PROTOCOL   *ExpPciIoProto;
    PCI_ID                ExpPciId1;
    PCI_ID                ExpPciId2;
    UINT8                 ExpClassCode[3] = {
      PCI_IF_USB4,
      PCI_CLASS_SERIAL_USB,
      PCI_CLASS_SERIAL
    };
    UINTN                 ExpSegmentNumber;
    UINTN                 ExpBusNumber;
    UINTN                 ExpMissMatchBusNumber;
    UINTN                 ExpDeviceNumber;
    UINTN                 ExpFunctionNumber;
    UINTN                 ExpMissMatchFunctionNumber;
    TBT_DMA               *ExpDmaPtr;
    ITBT_INFO_HOB         *ExpITbtInfoHob;
    BOOLEAN               *TBTControllerWasPowered;

    void SetUp() override {
      PcieRpConfig                = (PCIE_RP_CONFIG *) AllocateZeroPool (sizeof (PCIE_RP_CONFIG));
      TbtDmaPcieBdf               = (PCIE_BDF *) AllocateZeroPool (sizeof (PCIE_BDF));
      FirmwareType                = DISCRETE_TBT;             // Default value
      PcieRpConfig->PcieRpType    = SUPPORT_PCH_TYPE;         // Default value
      ForcePwrFunc                = TbtNvmDrvYflForcePwrFunc;
      //
      // Default value
      //
      ExpHandleCount              = 0x5;
      ExpPcieRpBaseAddr           = 0x86000000;
      ExpPciIoProto               = gPciIoInterface;
      ExpPciId1.VendorId          = 0x8888;
      ExpPciId2.VendorId          = USB_VENDOR_ID_1;
      ExpSecondaryBus             = 0x0;
      ExpSubordinateBus           = 0x6;
      ExpSegmentNumber            = 0x0;
      ExpBusNumber                = 0x1;
      ExpMissMatchBusNumber       = 0x7;
      ExpDeviceNumber             = 0xD;
      ExpFunctionNumber           = 0x2;
      ExpMissMatchFunctionNumber  = 0x0;
      TbtDmaPcieBdf->Bus          = (UINT16) ExpBusNumber;
      TbtDmaPcieBdf->Device       = (UINT16) ExpDeviceNumber;
      TbtDmaPcieBdf->Function     = (UINT16) ExpFunctionNumber;
      TBTControllerWasPowered     = (BOOLEAN *) AllocateZeroPool (sizeof (BOOLEAN));
      gDmaPtrMock->TBTControllerWasPowered  = TBTControllerWasPowered;
      ExpDmaPtr                   = gDmaPtrMock;
      ExpITbtInfoHob              = (ITBT_INFO_HOB *) AllocateZeroPool (sizeof (ITBT_INFO_HOB));
      ExpITbtInfoHob->Usb4CmMode  = 0;
    }
    void TearDown() override {
      //
      // Destroy Mock Service
      //
      FreePool (PcieRpConfig);
      FreePool (TbtDmaPcieBdf);
    }
};

//
// Case 1 - Mock call LocateHandleBuffer return error
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, LocateHandleBufferError) {
  cout << "[---------- Case 1 ----------]"<< endl;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 2 - When PcieRpConfig->PcieRpType = PCIE_RP_TYPE_MAX_VALUE
//          That would failed at unsupported type.
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, FailedAtUnsupportedPcieRpType) {
  cout << "[---------- Case 2 ----------]"<< endl;

  PcieRpConfig->PcieRpType = PCIE_RP_TYPE_MAX_VALUE;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (Return (EFI_SUCCESS));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 3 - When FirmwareType = DISCRETE_TBT, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//          BusNumber > SubordinateBus, that would executed PCI device is not found with root port.
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, DISCRETE_TBT_PCI_DeviceIsNotFound) {
  cout << "[---------- Case 3 ----------]"<< endl;

  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call PchPcieRpPciCfgBase
  //
  EXPECT_CALL (PchPciBdfLibMock,
    PchPcieRpPciCfgBase (
      _))
    .WillOnce (Return (ExpPcieRpBaseAddr));

  //
  //  Mock call PciSegmentRead8
  //
  EXPECT_CALL (PciSegmentLibMock,
    PciSegmentRead8 (
      _))
    .WillOnce (Return (ExpSecondaryBus))
    .WillOnce (Return (ExpSubordinateBus));
  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillOnce (Return (EFI_LOAD_ERROR))                            // First time return error
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillOnce (Return (EFI_LOAD_ERROR))                           // First time return error
    .WillOnce (
      DoAll (
        SetArgBuffer<4>(&ExpPciId1, sizeof(ExpPciId1)),
        Return (EFI_SUCCESS)
    ))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillOnce (Return (EFI_LOAD_ERROR))                           // First time return error
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillOnce (Return (EFI_NOT_FOUND))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpMissMatchBusNumber, sizeof(ExpMissMatchBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 4 - When FirmwareType = INTEGRATED_TBT_RETIMER, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//          BusNumber > SubordinateBus, that would executed PCI device with bdf is not found.
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, INTEGRATED_TBT_RETIMER_PCI_DeviceIsNotFound) {
  cout << "[---------- Case 4 ----------]"<< endl;
  FirmwareType = INTEGRATED_TBT_RETIMER;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpMissMatchBusNumber, sizeof(ExpMissMatchBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 5 - When FirmwareType = INTEGRATED_TBT_RETIMER, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//          That would attributes get is failed.
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, AttributesGetIsFailed) {
  cout << "[---------- Case 5 ----------]"<< endl;
  FirmwareType = INTEGRATED_TBT_RETIMER;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpBusNumber, sizeof(ExpBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationSupported,
      0,
      _))
    .WillOnce (Return (EFI_UNSUPPORTED));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 6 - When FirmwareType = INTEGRATED_TBT_RETIMER, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//          That would attributes set is failed.
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, AttributesSetIsFailed) {
  cout << "[---------- Case 6 ----------]"<< endl;
  FirmwareType = INTEGRATED_TBT_RETIMER;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpBusNumber, sizeof(ExpBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Get the PCI Command options that are supported by this controller.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationSupported,
      0,
      _))
    .WillOnce (Return (EFI_SUCCESS));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Set the PCI Command options to enable device memory mapped IO,
  //  port IO, and bus mastering.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationEnable,
      _,
      _))
    .WillOnce (Return (EFI_UNSUPPORTED));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 7 - When FirmwareType = INTEGRATED_TBT_RETIMER, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//          Could not perform device into force pwr
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, CouldNotPerformDeviceIntoForcePwr) {
  cout << "[---------- Case 7 ----------]"<< endl;
  FirmwareType = INTEGRATED_TBT_RETIMER;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpBusNumber, sizeof(ExpBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Get the PCI Command options that are supported by this controller.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationSupported,
      0,
      _))
    .WillOnce (Return (EFI_SUCCESS));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Set the PCI Command options to enable device memory mapped IO,
  //  port IO, and bus mastering.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationEnable,
      _,
      _))
    .WillOnce (Return (EFI_SUCCESS));
  //
  //  Mock call ForcePwrFunc
  //
  EXPECT_CALL (TbtNvmDrvYflRouterMock,
    TbtNvmDrvYflForcePwrFunc (
      ExpPciIoProto,
      TRUE,      
      _
    ))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 8 - When FirmwareType = INTEGRATED_TBT_RETIMER, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//          Could not initialize DMA for device
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, CouldNotInitializeDMA_ForDevice) {
  cout << "[---------- Case 8 ----------]"<< endl;
  FirmwareType = INTEGRATED_TBT_RETIMER;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpBusNumber, sizeof(ExpBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Get the PCI Command options that are supported by this controller.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationSupported,
      0,
      _))
    .WillOnce (Return (EFI_SUCCESS));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Set the PCI Command options to enable device memory mapped IO,
  //  port IO, and bus mastering.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationEnable,
      _,
      _))
    .WillOnce (Return (EFI_SUCCESS));
  //
  //  Mock call ForcePwrFunc
  //
  EXPECT_CALL (TbtNvmDrvYflRouterMock,
    TbtNvmDrvYflForcePwrFunc (
      ExpPciIoProto,
      TRUE,      
      _
    ))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvTxCfgPkt
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaCtor (
      ExpPciIoProto,
      _))
    .WillOnce (Return (NULL));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 9 - When FirmwareType = INTEGRATED_TBT_RETIMER, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//          Mock call GetFirstGuidHob return Error
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, GetFirstGuidHobError) {
  cout << "[---------- Case 9 ----------]"<< endl;
  FirmwareType = INTEGRATED_TBT_RETIMER;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpBusNumber, sizeof(ExpBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Get the PCI Command options that are supported by this controller.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationSupported,
      0,
      _))
    .WillOnce (Return (EFI_SUCCESS));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Set the PCI Command options to enable device memory mapped IO,
  //  port IO, and bus mastering.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationEnable,
      _,
      _))
    .WillOnce (Return (EFI_SUCCESS));
  //
  //  Mock call ForcePwrFunc
  //
  EXPECT_CALL (TbtNvmDrvYflRouterMock,
    TbtNvmDrvYflForcePwrFunc (
      ExpPciIoProto,
      TRUE,      
      _
    ))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvDmaCtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaCtor (
      ExpPciIoProto,
      _))
    .WillOnce (Return (ExpDmaPtr));

  //
  //  Mock call GetFirstGuidHob
  //
  EXPECT_CALL (HobLibMock,
    GetFirstGuidHob (
      BufferEq(&gITbtInfoHobGuid, sizeof(EFI_GUID))
    ))
    .WillOnce(Return(NULL));

  //
  //  Mock call HrImplPtr->pDma->Dtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaDtor (
      ExpDmaPtr
    ));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 10 - When FirmwareType = INTEGRATED_TBT_RETIMER, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//           Mock call TbtNvmDrvHrSendDrvReady return Error
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, TbtNvmDrvHrSendDrvReadyError) {
  cout << "[---------- Case 10 ----------]"<< endl;
  FirmwareType = INTEGRATED_TBT_RETIMER;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpBusNumber, sizeof(ExpBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Get the PCI Command options that are supported by this controller.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationSupported,
      0,
      _))
    .WillOnce (Return (EFI_SUCCESS));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Set the PCI Command options to enable device memory mapped IO,
  //  port IO, and bus mastering.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationEnable,
      _,
      _))
    .WillOnce (Return (EFI_SUCCESS));
  //
  //  Mock call ForcePwrFunc
  //
  EXPECT_CALL (TbtNvmDrvYflRouterMock,
    TbtNvmDrvYflForcePwrFunc (
      ExpPciIoProto,
      TRUE,      
      _
    ))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvDmaCtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaCtor (
      ExpPciIoProto,
      _))
    .WillOnce (Return (ExpDmaPtr));

  //
  //  Mock call GetFirstGuidHob
  //
  EXPECT_CALL (HobLibMock,
    GetFirstGuidHob (
      BufferEq(&gITbtInfoHobGuid, sizeof(EFI_GUID))
    ))
    .WillOnce(Return(ExpITbtInfoHob));

  //
  //  Mock call TbtNvmDrvTxCfgPkt in TbtNvmDrvHrSendDrvReady
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvTxCfgPkt (
      ExpDmaPtr,
      _,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  //
  //  Mock call HrImplPtr->pDma->DbgPrint (HrImplPtr->pDma);
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaDebugPrint (
      gDmaPtrMock
    ));

  //
  //  Mock call HrImplPtr->pDma->Dtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaDtor (
      ExpDmaPtr
    ));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 11 - When FirmwareType = INTEGRATED_TBT_RETIMER, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//           Usb4CmMode  = USB4_CM_MODE_SW_CM, Correct Flow 1
// Expected Result - TbtNvmDrvHrCtor will not return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, CorrectFlow1) {
  FirmwareType = INTEGRATED_TBT_RETIMER;
  ExpITbtInfoHob->Usb4CmMode  = USB4_CM_MODE_SW_CM;
  cout << "[---------- Case 11 ----------]"<< endl;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpBusNumber, sizeof(ExpBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Get the PCI Command options that are supported by this controller.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationSupported,
      0,
      _))
    .WillOnce (Return (EFI_SUCCESS));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Set the PCI Command options to enable device memory mapped IO,
  //  port IO, and bus mastering.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationEnable,
      _,
      _))
    .WillOnce (Return (EFI_SUCCESS));
  //
  //  Mock call ForcePwrFunc
  //
  EXPECT_CALL (TbtNvmDrvYflRouterMock,
    TbtNvmDrvYflForcePwrFunc (
      ExpPciIoProto,
      TRUE,      
      _
    ))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvDmaCtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaCtor (
      ExpPciIoProto,
      _))
    .WillOnce (Return (ExpDmaPtr));

  //
  //  Mock call GetFirstGuidHob
  //
  EXPECT_CALL (HobLibMock,
    GetFirstGuidHob (
      BufferEq(&gITbtInfoHobGuid, sizeof(EFI_GUID))
    ))
    .WillOnce(Return(ExpITbtInfoHob));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_NE (HrPtr, NULL);
}

//
// Case 12 - When FirmwareType = DISCRETE_TBT, PcieRpConfig->PcieRpType = SUPPORT_PCH_TYPE
//           Correct Flow 2
// Expected Result - TbtNvmDrvHrCtor will not return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, CorrectFlow2) {
  cout << "[---------- Case 12 ----------]"<< endl;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<3>(&ExpHandleCount, sizeof(ExpHandleCount)),
        SetArgPointee<4>(ExpHandles),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call PchPcieRpPciCfgBase
  //
  EXPECT_CALL (PchPciBdfLibMock,
    PchPcieRpPciCfgBase (
      _))
    .WillOnce (Return (ExpPcieRpBaseAddr));

  //
  //  Mock call PciSegmentRead8
  //
  EXPECT_CALL (PciSegmentLibMock,
    PciSegmentRead8 (
      _))
    .WillOnce (Return (ExpSecondaryBus))
    .WillOnce (Return (ExpSubordinateBus));
  //
  //  Mock call gBS->HandleProtocol
  //
  EXPECT_CALL (UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgPointee<2>(ExpPciIoProto),
        Return (EFI_SUCCESS)
    ));
  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint32),
      0,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpPciId2, sizeof(ExpPciId2)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Pci.Read
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoConfigRead (
      ExpPciIoProto,
      Eq(EfiPciIoWidthUint8),
      PCI_CLASSCODE_OFFSET,
      _,
      _))
    .WillRepeatedly (
      DoAll (
        SetArgBuffer<4>(&ExpClassCode, sizeof(ExpClassCode)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoGetLocation (
      ExpPciIoProto,
      _,
      _,
      _,
      _))
    .WillOnce (
      DoAll (
        SetArgBuffer<1>(&ExpSegmentNumber, sizeof(ExpSegmentNumber)),
        SetArgBuffer<2>(&ExpBusNumber, sizeof(ExpBusNumber)),
        SetArgBuffer<3>(&ExpDeviceNumber, sizeof(ExpDeviceNumber)),
        SetArgBuffer<4>(&ExpFunctionNumber, sizeof(ExpFunctionNumber)),
        Return (EFI_SUCCESS)
    ));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Get the PCI Command options that are supported by this controller.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationSupported,
      0,
      _))
    .WillOnce (Return (EFI_SUCCESS));

  //
  //  Mock call HrImplPtr->pPciIoProto->Attributes
  //  For Set the PCI Command options to enable device memory mapped IO,
  //  port IO, and bus mastering.
  //
  EXPECT_CALL (PciIoProcotolMock,
    MockPciIoAttributes (
      ExpPciIoProto,
      EfiPciIoAttributeOperationEnable,
      _,
      _))
    .WillOnce (Return (EFI_SUCCESS));
  //
  //  Mock call ForcePwrFunc
  //
  EXPECT_CALL (TbtNvmDrvYflRouterMock,
    TbtNvmDrvYflForcePwrFunc (
      ExpPciIoProto,
      TRUE,      
      _
    ))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvDmaCtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaCtor (
      ExpPciIoProto,
      _))
    .WillOnce (Return (ExpDmaPtr));

  //
  //  Mock call TbtNvmDrvTxCfgPkt in TbtNvmDrvHrSendDrvReady
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvTxCfgPkt (
      ExpDmaPtr,
      _,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvRxCfgPkt in TbtNvmDrvHrSendDrvReady
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvRxCfgPkt (
      ExpDmaPtr,
      _,
      _,
      _))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  HrPtr = TbtNvmDrvHrCtor (
                FirmwareType,
                PcieRpConfig,
                TbtDmaPcieBdf,
                ForcePwrFunc);
  EXPECT_NE (HrPtr, NULL);
}
