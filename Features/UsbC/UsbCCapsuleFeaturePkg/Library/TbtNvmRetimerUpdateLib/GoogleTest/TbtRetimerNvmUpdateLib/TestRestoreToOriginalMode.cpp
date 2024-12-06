/** @file
  Google test for RestoreToOriginalMode function.

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
#include <GoogleTest/Library/MockBasePcdLib.h>
#include <GoogleTest/Library/MockBaseMemoryLib.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>

// **********************************************************
// RestoreToOriginalMode Unit Test                          *
// **********************************************************
class RestoreToOriginalModeTest : public Test {
  protected:
    EFI_STATUS                      Status;
    EFI_GUID                        RetimerGuid;
    USBC_RETIMER_PROTOCOL           UsbCRetimerProtocol;
    MockUsbcRetimerProtocol         UsbcRetimerProtocolMock;
    MockUsbcCapsuleDebugLib         UsbcCapsuleDebugLibMock;
    MockBaseMemoryLib               BaseMemoryLibMock;
    MockBasePcdLib                  BasePcdLibMock;
    MockUefiRuntimeServicesTableLib UefiRuntimeServicesTableLibMock;
    UINT8                           PdControllerMode;
    UINT8                           DeviceMode;

  void TearDown () override {
    std::memcpy (&UsbCRetimerProtocol, &LocalURP, sizeof (USBC_RETIMER_PROTOCOL));
  }
};

//
// Case 1 - CompareGuid Return FALSE
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (RestoreToOriginalModeTest, 1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite);

  EXPECT_CALL (BaseMemoryLibMock, CompareGuid)
    .WillOnce (Return (FALSE));

  Status = RestoreToOriginalMode (&LocalURP, RetimerGuid);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - gRT_GetVariable return success
// Expected Result - Status will return EFI_UNSUPPORTED
//
TEST_F (RestoreToOriginalModeTest, 2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite);

  EXPECT_CALL (BaseMemoryLibMock, CompareGuid)
    .WillOnce (Return (TRUE));

  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_GetVariable)
    .WillOnce (Return (EFI_SUCCESS));

  Status = RestoreToOriginalMode (&LocalURP, RetimerGuid);
  EXPECT_EQ (Status, EFI_UNSUPPORTED);
}

//
// Case 3 - TbtPdRetimerFwUpdateModeChange return error
// Expected Result - Status will return EFI_UNSUPPORTED
//
TEST_F (RestoreToOriginalModeTest, 3) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (3);

  EXPECT_CALL (BaseMemoryLibMock, CompareGuid)
    .WillOnce (Return (TRUE));

  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_GetVariable)
    .WillOnce (Return (EFI_UNSUPPORTED));

  // TbtPdRetimerFwUpdateModeChange>>
  PdControllerMode = 0;

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode
    )
    .WillOnce (Return (EFI_INVALID_PARAMETER));
  // TbtPdRetimerFwUpdateModeChange<<

  Status = RestoreToOriginalMode (&LocalURP, RetimerGuid);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 4 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (RestoreToOriginalModeTest, 4) {
  cout << "[---------- Case 4 ----------]"<< endl;

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (5);

  EXPECT_CALL (BaseMemoryLibMock, CompareGuid)
    .WillOnce (Return (TRUE));

  EXPECT_CALL (UefiRuntimeServicesTableLibMock, gRT_GetVariable)
    .WillOnce (Return (EFI_UNSUPPORTED));

  PdControllerMode = 1;

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_GetPdControllerMode
    )
    .WillOnce (Return (EFI_SUCCESS))
    .WillOnce (
       DoAll (
         SetArgPointee<0>(DeviceMode = 0),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UsbcRetimerProtocolMock,
    UsbcRetimerProtocol_SetPdControllerMode
    )
    .WillOnce (Return (EFI_SUCCESS));

  Status = RestoreToOriginalMode (&LocalURP, RetimerGuid);
  EXPECT_EQ (Status, EFI_SUCCESS);
}
