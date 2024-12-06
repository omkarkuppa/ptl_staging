/** @file
  Google test for TerminateHW function.

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

//
// Test function define.
//
extern "C" {
TBT_STATUS
TerminateHW (
  IN  TBT_RETIMER  *This
  );
}

struct MockSendLsupCmdDis {
  MOCK_INTERFACE_DECLARATION (MockSendLsupCmdDis);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    TBT_STATUS,
    SendLsupCmdDis,
    (IN RETIMER_THRU_HR *RetimerPtr)
    );
};

MOCK_INTERFACE_DEFINITION (MockSendLsupCmdDis);
MOCK_FUNCTION_INTERNAL_DEFINITION(MockSendLsupCmdDis, SendLsupCmdDis, 1, EFIAPI);

// **********************************************************
// TerminateHW Unit Test                                    *
// **********************************************************
class TerminateHWTest : public Test {
  protected:
    TBT_STATUS                      TbtStatus;
    EFI_STATUS                      Status;
    RETIMER_THRU_HR                 *RetimerPtr;
    TBT_RETIMER                     *DevComRetimer;
    USBC_PROGRESS_CODE_PROTOCOL     *UsbCProgressCodeProtocol;
    SHOW_PROGRESS_CODE              ShowProgressCode;
    MockUsbcCapsuleDebugLib         UsbcCapsuleDebugLibMock;
    MockSendLsupCmdDis              SendLsupCmdDisMock;
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
TEST_F (TerminateHWTest, Case1) {
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

  EXPECT_CALL (UsbcCapsuleDebugLibMock, CapsuleLogWrite);

  EXPECT_CALL (UsbCProgressCodeProtocolMock, ShowProgressCode);

  EXPECT_CALL (SendLsupCmdDisMock, SendLsupCmdDis)
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  TbtStatus = TerminateHW (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 2 - SendLsupCmdDis return error
// Expected Result - Status will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TerminateHWTest, Case2) {
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

  EXPECT_CALL (SendLsupCmdDisMock, SendLsupCmdDis)
    .WillOnce (Return (TBT_STATUS_NON_RECOVERABLE_ERROR));

  TbtStatus = TerminateHW (DevComRetimer);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}
