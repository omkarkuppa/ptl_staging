/** @file
  Google test for TbtPdRetimerFwUpdateModeChange function.

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

#include <GTestTbtRetimerNvmUpdateLib.h>
#include <GoogleTest/Private/MockUsbcRetimerProtocol/MockUsbcRetimerProtocol.h>
#include <GoogleTest/Library/MockUsbcCapsuleDebugLib.h>

extern "C" {
EFI_STATUS
TbtPdRetimerFwUpdateModeChange (
  IN  USBC_RETIMER_PROTOCOL  *This,
  IN  UINT8                  PdControllerMode,
  IN  UINT8                  PdNumber
  );
}

// **********************************************************
// TbtPdRetimerFwUpdateModeChange Unit Test                 *
// **********************************************************
class TbtPdRetimerFwUpdateModeChangeTest : public Test {
  protected:
  EFI_STATUS              Status;
  UINT8                   PdControllerMode;
  UINT8                   PdNumber;
  UINT8                   DeviceMode;
  USBC_RETIMER_PROTOCOL   UsbCRetimerProtocol;
  MockUsbcRetimerProtocol UsbcRetimerProtocolMock;
  MockUsbcCapsuleDebugLib UsbcCapsuleDebugLibMock;

  void SetUp() override {
    std::memcpy (&UsbCRetimerProtocol, &LocalURP, sizeof (USBC_RETIMER_PROTOCOL));
  }
};

//
// Case 1 - GetPdControllerMode at first time get fail
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (TbtPdRetimerFwUpdateModeChangeTest, 1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (1);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = TbtPdRetimerFwUpdateModeChange (&LocalURP, 0, PdNumber);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - GetPdControllerMode at first time get DeviceMode = PdControllerMode = 1
// Expected Result - Status will return EFI_ALREADY_STARTED
//
TEST_F (TbtPdRetimerFwUpdateModeChangeTest, 2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (2);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode)
     .WillOnce (
        DoAll (
          SetArgPointee<0>(DeviceMode=1),
          Return (EFI_SUCCESS)
    ));

  Status = TbtPdRetimerFwUpdateModeChange (&LocalURP, 1, PdNumber);
  EXPECT_EQ (Status, EFI_ALREADY_STARTED);
}

//
// Case 3 - GetPdControllerMode at first time get DeviceMode = PdControllerMode = 0
// Expected Result - Status will return EFI_NOT_STARTED
//
TEST_F (TbtPdRetimerFwUpdateModeChangeTest, 3) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (2);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode)
     .WillOnce (
        DoAll (
          SetArgPointee<0>(DeviceMode=0),
          Return (EFI_SUCCESS)
    ));

  Status = TbtPdRetimerFwUpdateModeChange (&LocalURP, 0, PdNumber);
  EXPECT_EQ (Status, EFI_NOT_STARTED);
}

//
// Case 4 - SetPdControllerMode set fail
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (TbtPdRetimerFwUpdateModeChangeTest, 4) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (2);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_SetPdControllerMode)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = TbtPdRetimerFwUpdateModeChange (&LocalURP, 0, PdNumber);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 5 - GetPdControllerMode at second time get fail
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (TbtPdRetimerFwUpdateModeChangeTest, 5) {
  cout << "[---------- Case 5 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (2);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_SetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS));

  Status = TbtPdRetimerFwUpdateModeChange (&LocalURP, 0, PdNumber);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 6 - GetPdControllerMode at second time get DeviceMode != PdControllerMode
// Expected Result - Status will return EFI_TIMEOUT
//
TEST_F (TbtPdRetimerFwUpdateModeChangeTest, 6) {
  cout << "[---------- Case 6 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (3);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_SetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS));

  Status = TbtPdRetimerFwUpdateModeChange (&LocalURP, 1, PdNumber);
  EXPECT_EQ (Status, EFI_TIMEOUT);
}

//
// Case 7 - GetPdControllerMode at second time get PdControllerMode = DeviceMode = 1
// Expected Result - Status will return
//
TEST_F (TbtPdRetimerFwUpdateModeChangeTest, 7) {
  cout << "[---------- Case 7 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (3);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS))
     .WillOnce (
        DoAll (
          SetArgPointee<0>(DeviceMode=1),
          Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_SetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS));

  Status = TbtPdRetimerFwUpdateModeChange (&LocalURP, 1, PdNumber);
  EXPECT_EQ (Status, EFI_SUCCESS);
}

//
// Case 8 - GetPdControllerMode at second time get PdControllerMode = DeviceMode = 0
// Expected Result - Status will return
//
TEST_F (TbtPdRetimerFwUpdateModeChangeTest, 8) {
  cout << "[---------- Case 8 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (3);

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS))
     .WillOnce (
        DoAll (
          SetArgPointee<0>(DeviceMode=0),
          Return (EFI_SUCCESS)
    ));

  EXPECT_CALL (UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_SetPdControllerMode)
    .WillOnce (Return (EFI_SUCCESS));

  Status = TbtPdRetimerFwUpdateModeChange (&LocalURP, 0, PdNumber);
  EXPECT_EQ (Status, EFI_SUCCESS);
}