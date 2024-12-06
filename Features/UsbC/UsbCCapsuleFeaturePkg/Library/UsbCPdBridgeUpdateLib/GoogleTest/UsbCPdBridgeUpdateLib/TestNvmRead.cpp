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

class NvmReadTest : public Test {
  protected:
    EFI_STATUS              Status;
    USBC_PD_BRIDGE_PROTOCOL PdBridgeProtocol;
    UINT8                   PdBridgeIndex;
    UINT32                  NvmOffset;
    UINT8                   Length;
    UINT8                   *OutputData;
    UINT8                   *OutputDataSize;

    MockPdBridgeProtocol PdBridgeProtocolMock;

  void SetUp() override {
    Length         = 30;
    OutputData     = (UINT8 *) AllocateZeroPool (sizeof (OutputData));
    OutputDataSize = (UINT8 *) AllocateZeroPool (sizeof (OutputDataSize));

    std::memcpy (&PdBridgeProtocol, &localPDBG, sizeof (USBC_PD_BRIDGE_PROTOCOL));
  }
};

//
// Case 1 - Length > 32 , return error
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (NvmReadTest, case_1) {
  cout << "[---------- Case 1 ----------]"<< endl;
  Status = NvmRead (
             &localPDBG,
             PdBridgeIndex,
             NvmOffset,
             35,
             OutputData,
             OutputDataSize
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 2 - Length == 0 , return error
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (NvmReadTest, case_2) {
  cout << "[---------- Case 2 ----------]"<< endl;
  Status = NvmRead (
             &localPDBG,
             PdBridgeIndex,
             NvmOffset,
             0,
             OutputData,
             OutputDataSize
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 3 -  OutputData == NULL , return error
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (NvmReadTest, case_3) {
  cout << "[---------- Case 3 ----------]"<< endl;
  Status = NvmRead (
             &localPDBG,
             PdBridgeIndex,
             NvmOffset,
             Length,
             NULL,
             OutputDataSize
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 4 -  OutputDataSize == NULL , return error
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (NvmReadTest, case_4) {
  cout << "[---------- Case 4 ----------]"<< endl;
  Status = NvmRead (
             &localPDBG,
             PdBridgeIndex,
             NvmOffset,
             Length,
             OutputData,
             NULL
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 5 - PdBridgeProtocol_ExecuteVendorCmd return error
// Mock - PdBridgeProtocolMock
// Expected Result - Status will return EFI_INVALID_PARAMETER
//
TEST_F (NvmReadTest, case_5) {
  cout << "[---------- Case 5 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_INVALID_PARAMETER));

  Status = NvmRead (
             &localPDBG,
             PdBridgeIndex,
             NvmOffset,
             Length,
             OutputData,
             OutputDataSize
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

//
// Case 6 - PdBridgeProtocol_ExecuteVendorCmd return correct
// Mock - PdBridgeProtocolMock
// Expected Result - Status will return EFI_SUCCESS
//
TEST_F (NvmReadTest, case_6) {
  cout << "[---------- Case 6 ----------]"<< endl;
  EXPECT_CALL (PdBridgeProtocolMock, PdBridgeProtocol_ExecuteVendorCmd)
    .WillOnce (Return (EFI_SUCCESS));

  Status = NvmRead (
             &localPDBG,
             PdBridgeIndex,
             NvmOffset,
             Length,
             OutputData,
             OutputDataSize
             );
  EXPECT_EQ (Status, EFI_SUCCESS);

  cout << "NvmRead Done." << endl;
}
