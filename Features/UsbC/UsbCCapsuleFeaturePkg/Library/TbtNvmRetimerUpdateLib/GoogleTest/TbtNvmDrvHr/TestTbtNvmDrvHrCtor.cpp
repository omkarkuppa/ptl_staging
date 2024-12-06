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

#include <GTestTbtNvmDrvHr.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Library/MockHobLib.h>
#include <GoogleTest/Private/MockTbtNvmDrvDma/MockTbtNvmDrvDma.h>
#include <GoogleTest/Private/MockPciIopProtocolLib/MockPciIopProtocolLib.h>

struct MockTbtNvmDrvDmaCtor {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvDmaCtor);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_DMA *,
    TbtNvmDrvDmaCtor,
    (EFI_PCI_IO_PROTOCOL *PciIoProto,
     BOOLEAN             *TBTControllerWasPowered,
     UINTN                FunctionNumber)
    );
};

MOCK_INTERFACE_DEFINITION (MockTbtNvmDrvDmaCtor);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockTbtNvmDrvDmaCtor, TbtNvmDrvDmaCtor, 3, EFIAPI);

// **********************************************************
// TbtNvmDrvHrCtor Unit Test                                *
// **********************************************************
class TbtNvmDrvHrCtorTest : public Test {
  protected:
    EFI_STATUS             Status;
    TBT_HOST_ROUTER        *HrPtr;
    TBT_HR_IMPL            *HrImplPtr;
    UINT8                  FirmwareType;
    PCIE_RP_CONFIG         *PcieRpConfig;
    PCIE_BDF               *TbtDmaPcieBdf;
    DTBT_CONTROLLER_CONFIG *DTbtControllerConfig;
    FORCE_PWR_HR           ForcePwrFunc;
    UINT8                  PcieRootPort;
    UINTN                  HandleCount;
    EFI_HANDLE             Handles[5] = {0, 0, 0, 0, 0};

    UINT8 ExpClassCode[3] = {
      PCI_IF_USB4,
      PCI_CLASS_SERIAL_USB,
      PCI_CLASS_SERIAL
    };

    DTBT_INFO_HOB                *DTbtInfoHob;
    ITBT_INFO_HOB                *ITbtInfoHob;
    BOOLEAN                      *TBTControllerWasPowered;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;
    MockPciIopProtocolLib        PciIoProcotolMock;
    MockHobLib                   HobLibMock;
    MockTbtNvmDrvDma             TbtNvmDrvDmaMock;
    MockTbtNvmDrvDmaCtor         TbtNvmDrvDmaCtorMock;
    PCI_ID                       ExpPciId;
    UINTN                        BusNumber;
    UINTN                        DeviceNumber;
    UINTN                        FunctionNumber;
    UINTN                        SegmentNumber;
    TBT_DMA                      *ExpDmaPtr;
    EFI_PCI_IO_PROTOCOL          *PciIoProto = &localPp;
    TBT_DMA                      *gDmaPtrMock = &LocalDmaPtr;

  void SetUp() override {
    PcieRpConfig                         = (PCIE_RP_CONFIG *) AllocateZeroPool (sizeof (PCIE_RP_CONFIG));
    DTbtControllerConfig                 = (DTBT_CONTROLLER_CONFIG *) AllocateZeroPool (sizeof (DTBT_CONTROLLER_CONFIG));
    DTbtInfoHob                          = (DTBT_INFO_HOB *) AllocateZeroPool (sizeof (DTBT_INFO_HOB));
    TbtDmaPcieBdf                        = (PCIE_BDF *) AllocateZeroPool (sizeof (PCIE_BDF));
    TBTControllerWasPowered              = (BOOLEAN *) AllocateZeroPool (sizeof (BOOLEAN));
    HrImplPtr                            = (TBT_HR_IMPL *) AllocateZeroPool (sizeof (TBT_HR_IMPL));
    TbtDmaPcieBdf->Bus                   = (UINT16) BusNumber;
    TbtDmaPcieBdf->Device                = (UINT16) DeviceNumber;
    TbtDmaPcieBdf->Function              = (UINT16) FunctionNumber;
    ExpDmaPtr                            = gDmaPtrMock;
    gDmaPtrMock->TBTControllerWasPowered = TBTControllerWasPowered;
    HrImplPtr->pPciIoProto               = PciIoProto;
    HrImplPtr->ForcePwrFunc              = ForcePwrFunc;
    ExpPciId.VendorId                    = USB_VENDOR_ID_1;
    HandleCount                          = 0x1;
    PcieRpConfig->PcieRpType             = 0x02;
    BusNumber                            = 0x1;
    DeviceNumber                         = 0xD;
    FunctionNumber                       = 0x2;
    TbtDmaPcieBdf->Bus                   = 0x1;
    TbtDmaPcieBdf->Device                = 0xD;
    TbtDmaPcieBdf->Function              = 0x2;
  }
};

//
// Case 1 - gBS->LocateHandleBuffer return error
// Mock - UefiBootServicesTableLibMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  HrPtr = TbtNvmDrvHrCtor (FirmwareType, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 2 - When PcieRpConfig->PcieRpType = 0x06(other) , goto free_hr_impl .
// Mock - UefiBootServicesTableLibMock / HobLibMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x06;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  //
  //  Mock call GetFirstGuidHob
  //
  EXPECT_CALL (
    HobLibMock,
    GetFirstGuidHob (
      BufferEq (&gDTbtInfoHobGuid, sizeof (EFI_GUID))
      )
    )
    .WillOnce (Return (DTbtInfoHob));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x01, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 3 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , gBS->HandleProtocol return error.
// Mock - UefiBootServicesTableLibMock / HobLibMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case3) {
  cout << "[---------- Case 3 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  //
  //  Mock call GetFirstGuidHob
  //
  EXPECT_CALL (
    HobLibMock,
    GetFirstGuidHob (
      BufferEq (&gDTbtInfoHobGuid, sizeof (EFI_GUID))
      )
    )
    .WillOnce (Return (DTbtInfoHob));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_LOAD_ERROR)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x01, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 4 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , HrImplPtr->pPciIoProto->Pci.Read return error.
// Mock - UefiBootServicesTableLibMock / HobLibMock / PciIoProcotolMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case4) {
  cout << "[---------- Case 4 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillRepeatedly (Return (EFI_LOAD_ERROR));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 5 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , PciId.VendorId != USB_VENDOR_ID_1 return error.
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case5) {
  cout << "[---------- Case 5 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillRepeatedly (Return (EFI_SUCCESS));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 6 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , HrImplPtr->pPciIoProto->Pci.Read return error.
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case6) {
  cout << "[---------- Case 6 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )                        // First time return error
    .WillOnce (Return (EFI_LOAD_ERROR));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 7 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , ProgInterface != PCI_IF_USB4 ,Status return error.
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case7) {
  cout << "[---------- Case 7 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillRepeatedly (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 8 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , ProgInterface == PCI_IF_USB4 ,
//          HrImplPtr->pPciIoProto->GetLocation return error.
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case8) {
  cout << "[---------- Case 8 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_LOAD_ERROR)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 9 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , BusNumber != TbtDmaPcieBdf->Bus ,
//          HrImplPtr->pPciIoProto->GetLocation return error.
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case9) {
  cout << "[---------- Case 9 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  BusNumber                = 0x2;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )                        // First time return error
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 10 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , HrImplPtr->pPciIoProto->Attributes return error.
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case10) {
  cout << "[---------- Case 10 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Attributes
    )
    .WillRepeatedly (Return (EFI_LOAD_ERROR));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 11 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , Second HrImplPtr->pPciIoProto->Attributes return error.
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case11) {
  cout << "[---------- Case 11 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )                        // First time return error
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Attributes
    )
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_LOAD_ERROR));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 12 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , !HrImplPtr->pDma, goto free_hr_impl .
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock / TbtNvmDrvDmaMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case12) {
  cout << "[---------- Case 12 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Attributes
    )
    .WillRepeatedly (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvDmaCtorMock,
    TbtNvmDrvDmaCtor
    )
    .WillOnce (Return (NULL));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, NULL);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 13 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , !HrImplPtr->pDma, goto free_dma .
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock / HobLibMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, Case13) {
  cout << "[---------- Case 13 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )                        // First time return error
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Attributes
    )
    .WillRepeatedly (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvDmaCtorMock,
    TbtNvmDrvDmaCtor
    )
    .WillOnce (Return (ExpDmaPtr));

  EXPECT_CALL (
    HobLibMock,
    GetFirstGuidHob (
      BufferEq (&gITbtInfoHobGuid, sizeof (EFI_GUID))
      )
    )
    .WillOnce (Return (NULL));

  EXPECT_CALL (
    TbtNvmDrvDmaMock,
    TbtNvmDrvDmaDtor (
      ExpDmaPtr
      )
    );

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x0, PcieRpConfig, TbtDmaPcieBdf, NULL);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 14 - When PcieRpConfig->PcieRpType = 0x02(PCIE_RP_TYPE_PCH) , IsDeviceAvailable = FALSE , Status return EFI_NOT_FOUND.
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock / HobLibMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, 14) {
  cout << "[---------- Case 14 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x02;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    HobLibMock,
    GetFirstGuidHob (
      BufferEq (&gDTbtInfoHobGuid, sizeof (EFI_GUID))
      )
    )
    .WillOnce (Return (DTbtInfoHob));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )                        // First time return error
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Attributes
    )
    .WillRepeatedly (Return (EFI_LOAD_ERROR));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x01, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 15 - When PcieRpConfig->PcieRpType = 0x4(PCIE_RP_TYPE_CPU) ,
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock / HobLibMock / TbtNvmDrvDmaMock
// Expected Result - TbtNvmDrvHrCtor will return NULL
//
TEST_F (TbtNvmDrvHrCtorTest, 15) {
  cout << "[---------- Case 15 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x04;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    HobLibMock,
    GetFirstGuidHob (
      BufferEq (&gDTbtInfoHobGuid, sizeof (EFI_GUID))
      )
    )
    .WillOnce (Return (DTbtInfoHob));

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )                        // First time return error
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  //
  //  Mock call HrImplPtr->pPciIoProto->GetLocation
  //
  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Attributes
    )
    .WillRepeatedly (Return (EFI_LOAD_ERROR));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x01, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  EXPECT_EQ (HrPtr, NULL);
}

//
// Case 16 - When PcieRpConfig->PcieRpType != 0x02(PCIE_RP_TYPE_PCH) != 0x04(PCIE_RP_TYPE_CPU)
// Mock - UefiBootServicesTableLibMock / PciIoProcotolMock / TbtNvmDrvDmaCtorMock
// Expected Result - TbtNvmDrvHrCtor will return HrPtr
//
TEST_F (TbtNvmDrvHrCtorTest, Case16) {
  cout << "[---------- Case 16 ----------]"<< endl;

  PcieRpConfig->PcieRpType = 0x03;
  //
  //  Mock call gBS->LocateHandleBuffer
  //
  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateHandleBuffer (
      ByProtocol,
      _,
      NULL,
      _,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<3>(&HandleCount, sizeof (HandleCount)),
         SetArgPointee<4>(Handles),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_HandleProtocol (
      _,
      _,
      _
      )
    )
    .WillRepeatedly (
       DoAll (
         SetArgPointee<2>(PciIoProto),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpPciId, sizeof (ExpPciId)),
         Return (EFI_SUCCESS)
         )
       )                        // First time return error
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&ExpClassCode, sizeof (ExpClassCode)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    PciIopProtocol_GetLocation
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<1>(&SegmentNumber, sizeof (SegmentNumber)),
         SetArgBuffer<2>(&BusNumber, sizeof (BusNumber)),
         SetArgBuffer<3>(&DeviceNumber, sizeof (DeviceNumber)),
         SetArgBuffer<4>(&FunctionNumber, sizeof (FunctionNumber)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Attributes
    )
    .WillRepeatedly (Return (EFI_SUCCESS));

  EXPECT_CALL (
    TbtNvmDrvDmaCtorMock,
    TbtNvmDrvDmaCtor
    )
    .WillOnce (Return (ExpDmaPtr));

  EXPECT_CALL (UefiBootServicesTableLibMock, gBS_FreePool);

  HrPtr = TbtNvmDrvHrCtor (0x3, PcieRpConfig, TbtDmaPcieBdf, NULL);
}
