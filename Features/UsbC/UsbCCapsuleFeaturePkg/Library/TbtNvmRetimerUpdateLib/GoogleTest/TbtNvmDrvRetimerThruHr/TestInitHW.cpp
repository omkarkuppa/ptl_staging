/** @file
  Google test for InitHW function.

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
#include <GTestTbtNvmDrvRetimerThruHr.h>
#include <GoogleTest/Library/MockUsbcCapsuleDebugLib.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Private/MockUsbCProgressCodeProtocol/MockUsbCProgressCodeProtocol.h>
// Test function define.
//
extern "C" {
TBT_STATUS
InitHW (
  IN  TBT_RETIMER  *This
  );
}

struct MockTbtNvmDRThruHrHelpers {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDRThruHrHelpers);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    SendEnumCmd,
    (RETIMER_THRU_HR *RetimerPtr)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    SendLsupCmdEn,
    (IN RETIMER_THRU_HR *RetimerPtr)
    );
};

MOCK_INTERFACE_DEFINITION (MockTbtNvmDRThruHrHelpers);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockTbtNvmDRThruHrHelpers, SendEnumCmd, 1, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockTbtNvmDRThruHrHelpers, SendLsupCmdEn, 1, EFIAPI);

// **********************************************************
// InitHW Unit Test                                         *
// **********************************************************
class InitHWTest : public Test {
  protected:
    TBT_STATUS                      TbtStatus;
    RETIMER_THRU_HR                 *RetimerPtr;
    TBT_RETIMER                     *DevComRetimer;
    USBC_PROGRESS_CODE_PROTOCOL     *UsbCProgressCodeProtocol;
    SHOW_PROGRESS_CODE              ShowProgressCode;
    MockUsbcCapsuleDebugLib         UsbcCapsuleDebugLibMock;
    MockTbtNvmDRThruHrHelpers       TbtNvmDRThruHrHelpersMock;
    MockUefiBootServicesTableLib    UefiBootServicesTableLibMock;
    MockUsbCProgressCodeProtocolLib UsbCProgressCodeProtocolMock;

  void SetUp() override {
    DevComRetimer                              = (TBT_RETIMER *) AllocateZeroPool (sizeof (TBT_RETIMER));
    RetimerPtr                                 = (RETIMER_THRU_HR *) AllocateZeroPool (sizeof (RETIMER_THRU_HR));
    DevComRetimer->Impl                        = RetimerPtr;
    UsbCProgressCodeProtocol                   = (USBC_PROGRESS_CODE_PROTOCOL *) AllocateZeroPool (sizeof (USBC_PROGRESS_CODE_PROTOCOL));
    UsbCProgressCodeProtocol->ShowProgressCode = ShowProgressCode;
    std::memcpy (&ShowProgressCode, &localUSBCPp, sizeof (SHOW_PROGRESS_CODE));
  }

  void TearDown() override {
    FreePool (RetimerPtr);
    FreePool (DevComRetimer);
  }
};

//
// Case 1 - General Case
// Expected Result - Status will return TBT_STATUS_SUCCESS
//
TEST_F (InitHWTest, Case1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (2);

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (2);

  EXPECT_CALL (TbtNvmDRThruHrHelpersMock, SendEnumCmd)
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDRThruHrHelpersMock, SendLsupCmdEn)
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  TbtStatus = InitHW (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 2 - SendEnumCmd return error
// Expected Result - Status will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (InitHWTest, Case2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (2);

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode);

  EXPECT_CALL (TbtNvmDRThruHrHelpersMock, SendEnumCmd)
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = InitHW (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - SendLsupCmdEn return error
// Expected Result - Status will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (InitHWTest, Case3) {
  cout << "[---------- Case 3 ----------]"<< endl;

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_LocateProtocol
    )
    .WillOnce (
       DoAll (
         SetArgPointee<2>(&localUSBCPp),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite)
    .Times (3);

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode)
    .Times (2);

  EXPECT_CALL (TbtNvmDRThruHrHelpersMock, SendEnumCmd)
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  EXPECT_CALL (TbtNvmDRThruHrHelpersMock, SendLsupCmdEn)
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = InitHW (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}
