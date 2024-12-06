/** @file
  Google Test for the implementation of UsbcCapsuleDebugLib.

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

@par Specification Reference:

**/
#include <GTestUsbcCapsuleDebugLib.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>

/**
Google test for UsbcCapsuleDebugLib function.
**/
struct MockUsb4DebugProtocol {
  MOCK_INTERFACE_DECLARATION (MockUsb4DebugProtocol);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    EFI_STATUS,
    Usb4LogWrite,
    (IN EFI_USB4_DEBUG_PROTOCOL    *This,
     IN UINT8                      LogLevel,
     IN UINT16                     EventCode,
     IN UINT32                     EvtArg0,
     IN UINT32                     EvtArg1)
    );
};

MOCK_INTERFACE_DEFINITION (MockUsb4DebugProtocol);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockUsb4DebugProtocol, Usb4LogWrite, 5, EFIAPI);
USB4_LOG_WRITE  localUsb4LogWrite = Usb4LogWrite;

extern "C" {
extern USB4_LOG_WRITE  localUsb4LogWrite;
}

class CapsuleLogWriteTest : public Test {
  protected:
    EFI_STATUS                   Status;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;
    EFI_USB4_DEBUG_PROTOCOL      *Usb4DebugProtocol;
    USB4_LOG_WRITE               Usb4LogWrite;
    MockUsb4DebugProtocol        Usb4DebugProtocolMock;
    UINT8                        LogLevel;
    UINT16                       EventCode;
    UINT32                       EvtArg0;
    UINT32                       EvtArg1;

  void SetUp() override {
    Usb4DebugProtocol               = (EFI_USB4_DEBUG_PROTOCOL *) AllocateZeroPool (sizeof (EFI_USB4_DEBUG_PROTOCOL));
    Usb4DebugProtocol->Usb4LogWrite = Usb4LogWrite;
    std::memcpy (&Usb4LogWrite, &localUsb4LogWrite, sizeof (USB4_LOG_WRITE));
  }
};

//
// Case 1 - Call UefiBootServicesTableLibMock return error.
// Expected Result - UefiBootServicesTableLibMock will return EFI_INVALID_PARAMETER
//
TEST_F (CapsuleLogWriteTest, Test1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = CapsuleLogWrite (LogLevel, EventCode, EvtArg0, EvtArg1);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - General Case
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (CapsuleLogWriteTest, Test2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(Usb4DebugProtocol),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    Usb4DebugProtocolMock,
    Usb4LogWrite (_, LogLevel, EventCode, EvtArg0, EvtArg1)
    );

  Status = CapsuleLogWrite (LogLevel, EventCode, EvtArg0, EvtArg1);
  EXPECT_EQ (Status, EFI_SUCCESS);
}
