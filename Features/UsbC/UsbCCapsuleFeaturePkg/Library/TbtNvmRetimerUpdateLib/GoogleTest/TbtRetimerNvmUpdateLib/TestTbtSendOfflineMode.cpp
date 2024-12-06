/** @file
  Google test for TbtSendOfflineMode function.

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
#include <Library/FunctionMockLib.h>

struct MockSendOfflineMode {
  MOCK_INTERFACE_DECLARATION (MockSendOfflineMode);
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    EFI_STATUS,
    SendOfflineMode,
    (IN  RETIMER_THRU_HR *HrDevice,
     IN  UINT32          Data)
    );
};

MOCK_INTERFACE_DEFINITION (MockSendOfflineMode);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockSendOfflineMode, SendOfflineMode, 2, EFIAPI);

// **********************************************************
// TbtSendOfflineMode Unit Test                             *
// **********************************************************
class TbtSendOfflineModeTest : public Test {
  protected:
    EFI_STATUS          Status;
    HR_DEV_INSTANCE     *HrDevice;
    UINT32              Data;
    MockSendOfflineMode SendOfflineModeMock;

  void SetUp() override {
    HrDevice = (HR_DEV_INSTANCE *) AllocateZeroPool (sizeof (HR_DEV_INSTANCE));
  }
};

//
// Case 1 - Correct Flow
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (TbtSendOfflineModeTest, CorrectFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  EXPECT_CALL (SendOfflineModeMock, SendOfflineMode);

  Status = TbtSendOfflineMode (HrDevice, Data);
  EXPECT_EQ (Status, EFI_SUCCESS);
}
