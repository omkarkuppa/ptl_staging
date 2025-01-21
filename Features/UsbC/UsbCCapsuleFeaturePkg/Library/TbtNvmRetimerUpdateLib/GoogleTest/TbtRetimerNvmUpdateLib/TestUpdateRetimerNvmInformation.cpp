/** @file
  Google test for UpdateRetimerNvmInformation function.

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

#include <GTestTbtRetimerNvmUpdateLib.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
#include <GoogleTest/Library/MockBasePcdLib.h>

typedef struct {
  UINT32    UsbCRetimer0Version;
  UINT32    UsbCRetimer1Version;
  UINT32    UsbCRetimer2Version;
  UINT32    UsbCRetimer3Version;
} USBC_RETIMER_SETUP;

#define USBC_RETIMER_NAME  L"UsbCRetimer"

// **********************************************************
// UpdateRetimerNvmInformation Unit Test                    *
// **********************************************************
class UpdateRetimerNvmInformationTest : public Test {
  protected:
    UINT8                           FirmwareType;
    UINT8                           PcieRpType;
    UINT8                           PcieRootPort;
    UINT32                          RetimerVersion;
    UINT8                           MockUsbCRetimerNumber;
    RETIMER_CONFIG                  *MockRetimerConfigTable;
    USBC_RETIMER_SETUP              ExpUsbCRetimerSetup;
    MockBasePcdLib                  BasePcdLibMock;
    MockUefiRuntimeServicesTableLib UefiRuntimeServicesTableLibMock;

    RETIMER_DEV_ADDRESS MockRetimerDevAddress[1] = {
      0x00, 0x0D, 0x02, 0x00, 0x01, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
    };

    RETIMER_CONFIG MockRetimerConfig[4] = {
      { INTEGRATED_TBT_RETIMER, 0xFF, 0xFF, { 0x00, 0x0D, 0x02, 0x00, 0x01, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }
      } ,
      { DISCRETE_TBT_RETIMER,   0xFF, 0xFF, { 0x00, 0x0D, 0x02, 0x00, 0x01, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }
      },
      { DISCRETE_TBT_RETIMER,   0x00, 0xFF, { 0x00, 0x0D, 0x02, 0x00, 0x01, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }
      },
      { DISCRETE_TBT_RETIMER,   0x00, 0x01, { 0x00, 0x0D, 0x02, 0x00, 0x01, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
       }
     }
   };

  void SetUp() override {
    MockUsbCRetimerNumber                   = 4;
    MockRetimerConfigTable                  = &MockRetimerConfig[0];
    RetimerVersion                          = 0x12345678;
    ExpUsbCRetimerSetup.UsbCRetimer0Version = RetimerVersion;
    ExpUsbCRetimerSetup.UsbCRetimer1Version = RetimerVersion;
    ExpUsbCRetimerSetup.UsbCRetimer2Version = RetimerVersion;
    ExpUsbCRetimerSetup.UsbCRetimer3Version = RetimerVersion;
  }
};

//
// Case 1 - Error Flow
//
TEST_F (UpdateRetimerNvmInformationTest, ErrorFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  MockUsbCRetimerNumber = 1;
  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGet8 (
      _
      )
    )
    .WillOnce (Return (MockUsbCRetimerNumber));

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGetExPtr (
      _,
      _
      )
    )
    .WillOnce (Return (MockRetimerConfigTable));

  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_GetVariable)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_SetVariable)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  UpdateRetimerNvmInformation (
    FirmwareType,
    PcieRpType,
    PcieRootPort,
    MockRetimerDevAddress,
    RetimerVersion
    );
}

//
// Case 2 - Correct flow for UsbC Retimer 0 Version is matched
//
TEST_F (UpdateRetimerNvmInformationTest, CorrectFlowfForUsbCRetimer0Version) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGet8 (
      _
      )
    )
    .WillOnce (Return (MockUsbCRetimerNumber));

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGetExPtr (
      _,
      _
      )
    )
    .WillOnce (Return (MockRetimerConfigTable));

  EXPECT_CALL (
    UefiRuntimeServicesTableLibMock,
    gRT_GetVariable (
      Char16StrEq (USBC_RETIMER_NAME),
      BufferEq (&gUsbCRetimerSetupGuid, sizeof (EFI_GUID)),
      _,
      Pointee (Eq (sizeof (ExpUsbCRetimerSetup))),
      NotNull ()
      )
    )
    .WillOnce (
       DoAll (
         SetArgPointee<3>(sizeof (ExpUsbCRetimerSetup)),
         SetArgBuffer<4>(&ExpUsbCRetimerSetup, sizeof (ExpUsbCRetimerSetup)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiRuntimeServicesTableLibMock,
    gRT_SetVariable (
      Char16StrEq (USBC_RETIMER_NAME),
      BufferEq (&gUsbCRetimerSetupGuid, sizeof (EFI_GUID)),
      _,
      sizeof (ExpUsbCRetimerSetup),
      BufferEq (&ExpUsbCRetimerSetup, sizeof (ExpUsbCRetimerSetup))
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  UpdateRetimerNvmInformation (
    MockRetimerConfigTable[0].FirmwareType,
    MockRetimerConfigTable[0].PcieRpType,
    MockRetimerConfigTable[0].PcieRootPort,
    MockRetimerDevAddress,
    RetimerVersion
    );
}

//
// Case 3 - Correct flow for UsbC Retimer 1 Version is matched
//
TEST_F (UpdateRetimerNvmInformationTest, CorrectFlowfForUsbCRetimer1Version) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGet8 (
      _
      )
    )
    .WillOnce (Return (MockUsbCRetimerNumber));

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGetExPtr (
      _,
      _
      )
    )
    .WillOnce (Return (MockRetimerConfigTable));

  EXPECT_CALL (
    UefiRuntimeServicesTableLibMock,
    gRT_GetVariable (
      Char16StrEq (USBC_RETIMER_NAME),
      BufferEq (&gUsbCRetimerSetupGuid, sizeof (EFI_GUID)),
      _,
      Pointee (Eq (sizeof (ExpUsbCRetimerSetup))),
      NotNull ()
      )
    )
    .WillOnce (
       DoAll (
         SetArgPointee<3>(sizeof (ExpUsbCRetimerSetup)),
         SetArgBuffer<4>(&ExpUsbCRetimerSetup, sizeof (ExpUsbCRetimerSetup)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiRuntimeServicesTableLibMock,
    gRT_SetVariable (
      Char16StrEq (USBC_RETIMER_NAME),
      BufferEq (&gUsbCRetimerSetupGuid, sizeof (EFI_GUID)),
      _,
      sizeof (ExpUsbCRetimerSetup),
      BufferEq (&ExpUsbCRetimerSetup, sizeof (ExpUsbCRetimerSetup))
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  FirmwareType = DISCRETE_TBT_RETIMER;
  UpdateRetimerNvmInformation (
    FirmwareType,
    MockRetimerConfigTable[0].PcieRpType,
    MockRetimerConfigTable[0].PcieRootPort,
    MockRetimerDevAddress,
    RetimerVersion
    );
}

//
// Case 4 - Correct flow for UsbC Retimer 2 Version is matched
//
TEST_F (UpdateRetimerNvmInformationTest, CorrectFlowfForUsbCRetimer2Version) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGet8 (
      _
      )
    )
    .WillOnce (Return (MockUsbCRetimerNumber));

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGetExPtr (
      _,
      _
      )
    )
    .WillOnce (Return (MockRetimerConfigTable));

  EXPECT_CALL (
    UefiRuntimeServicesTableLibMock,
    gRT_GetVariable (
      Char16StrEq (USBC_RETIMER_NAME),
      BufferEq (&gUsbCRetimerSetupGuid, sizeof (EFI_GUID)),
      _,
      Pointee (Eq (sizeof (ExpUsbCRetimerSetup))),
      NotNull ()
      )
    )
    .WillOnce (
       DoAll (
         SetArgPointee<3>(sizeof (ExpUsbCRetimerSetup)),
         SetArgBuffer<4>(&ExpUsbCRetimerSetup, sizeof (ExpUsbCRetimerSetup)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiRuntimeServicesTableLibMock,
    gRT_SetVariable (
      Char16StrEq (USBC_RETIMER_NAME),
      BufferEq (&gUsbCRetimerSetupGuid, sizeof (EFI_GUID)),
      _,
      sizeof (ExpUsbCRetimerSetup),
      BufferEq (&ExpUsbCRetimerSetup, sizeof (ExpUsbCRetimerSetup))
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  FirmwareType = DISCRETE_TBT_RETIMER;
  PcieRpType   = 0x00;
  UpdateRetimerNvmInformation (
    FirmwareType,
    PcieRpType,
    MockRetimerConfigTable[0].PcieRootPort,
    MockRetimerDevAddress,
    RetimerVersion
    );
}

//
// Case 5 - Correct flow for UsbCRetimerNumber is more than Index setting.
//
TEST_F (UpdateRetimerNvmInformationTest, UsbCRetimerNumberIsMoreThanSetting) {
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGet8 (
      _
      )
    )
    .WillOnce (Return (MockUsbCRetimerNumber));

  EXPECT_CALL (
    BasePcdLibMock,
    LibPcdGetExPtr (
      _,
      _
      )
    )
    .WillOnce (Return (MockRetimerConfigTable));

  EXPECT_CALL (
    UefiRuntimeServicesTableLibMock,
    gRT_GetVariable (
      Char16StrEq (USBC_RETIMER_NAME),
      BufferEq (&gUsbCRetimerSetupGuid, sizeof (EFI_GUID)),
      _,
      Pointee (Eq (sizeof (ExpUsbCRetimerSetup))),
      NotNull ()
      )
    )
    .WillOnce (
       DoAll (
         SetArgPointee<3>(sizeof (ExpUsbCRetimerSetup)),
         SetArgBuffer<4>(&ExpUsbCRetimerSetup, sizeof (ExpUsbCRetimerSetup)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiRuntimeServicesTableLibMock,
    gRT_SetVariable (
      Char16StrEq (USBC_RETIMER_NAME),
      BufferEq (&gUsbCRetimerSetupGuid, sizeof (EFI_GUID)),
      _,
      sizeof (ExpUsbCRetimerSetup),
      BufferEq (&ExpUsbCRetimerSetup, sizeof (ExpUsbCRetimerSetup))
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  FirmwareType = DISCRETE_TBT_RETIMER;
  PcieRpType   = 0x00;
  PcieRootPort = 0x01;
  UpdateRetimerNvmInformation (
    FirmwareType,
    PcieRpType,
    PcieRootPort,
    MockRetimerDevAddress,
    RetimerVersion
    );
}
