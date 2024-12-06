/** @file
  Google test for UsbCPdBridgeUpdateLib function.

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

#include <GTestUsbCPdBridgeUpdateLib.h>
#include <GoogleTest/Private/MockPdBridgeProtocol/MockPdBridgeProtocol.h>
/**
Google test for UsbCPdBridgeUpdateLib function.
**/

class NvmResetTest : public Test {
  protected:
    EFI_STATUS              Status;
    USBC_PD_BRIDGE_PROTOCOL PdBridgeProtocol;
    UINT8                   PdBridgeIndex;

    MockPdBridgeProtocol PdBridgeProtocolMock;

  void SetUp() override {
    std::memcpy (&PdBridgeProtocol, &localPDBG, sizeof (USBC_PD_BRIDGE_PROTOCOL));
  }
};

//
// Case 1 - PdBridgeProtocol_ExecuteVendorCmd return error
// Mock - PdBridgeProtocolMock
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (NvmResetTest, case_1) {
  cout << "[---------- Case 1 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = NvmReset (&localPDBG, PdBridgeIndex);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - PdBridgeProtocol_ExecuteVendorCmd return correct
// Mock - PdBridgeProtocolMock
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (NvmResetTest, case_2) {
  cout << "[---------- Case 2 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_SUCCESS));

  Status = NvmReset (&localPDBG, PdBridgeIndex);
  EXPECT_EQ (Status, EFI_SUCCESS);

  cout << "NvmReset Done." << endl;
}
