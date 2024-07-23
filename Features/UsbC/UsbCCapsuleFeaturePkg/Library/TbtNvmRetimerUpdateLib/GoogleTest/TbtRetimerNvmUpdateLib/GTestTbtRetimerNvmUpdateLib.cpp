/** @file
  Unit tests for the implementation of TbtRetimerNvmUpdateLib.

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

#include <Library/GoogleTestLib.h>
using namespace testing;

#include <iostream>
using namespace std;

extern "C" {
  #include <Base.h>
  #include <Uefi/UefiBaseType.h>
  #include <Library/MemoryAllocationLib.h>
  #include <Library/BaseMemoryLib.h>
//
// Test fucntion header file.
//
  #include <Library/TbtNvmDrvYflRouter.h>
  #include <Library/TbtNvmRetimerUpdateLib.h>
  #include <Protocol/TbtNvmDrvHrProtocol.h>
  #include <UsbCRetimerSetup.h>
}

//**********************************************************
// GoogleTest mock                                         *
//**********************************************************

// Edk2 common mock
#include <GoogleTest/Library/MockBasePcdLib.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
// Private mock
#include "GoogleTest/Private/MockTbtNvmDrvUtils/MockTbtNvmDrvUtils.cpp"
#include "GoogleTest/Private/MockTbtNvmDrvYflRouter/MockTbtNvmDrvYflRouter.cpp"
#include "GoogleTest/Private/MockTbtNvmDrvRetimerThruHr/MockTbtNvmDrvRetimerThruHr.cpp"
#include "GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.cpp"
#include "GoogleTest/Private/MockTbtNvmRetimerDrvHelpers/MockTbtNvmRetimerDrvHelpers.cpp"
#include "GoogleTest/Private/MockFmpLib/MockFmpLib.cpp"

typedef struct {
  PAYLOAD_HEADER                     PayloadHeader;
  DISCRETE_TBT_ITEM                  PayloadItem;
  UINT32                             ImageContent;
} MOCK_DISCRETE_TBT_PAYLOAD_DATA;

typedef struct {
  PAYLOAD_HEADER                     PayloadHeader;
  RETIMER_ITEM                       PayloadItem;
  UINT32                             ImageContent;
} MOCK_RETIMER_PAYLOAD_DATA;

class CommonMock : public Test {
  protected:
    MockTbtNvmDrvUtils               TbtNvmDrvUtilsMock;
    MockTbtNvmDrvYflRouter           TbtNvmDrvYflRouterMock;
    MockTbtNvmDrvRetimerThruHr       TbtNvmDrvRetimerThruHrMock;
    MockTbtNvmDrvHr                  TbtNvmDrvHrMock;
    MockTbtNvmRetimerDrvHelpers      TbtNvmRetimerDrvHelpersMock;
    MockFmpLib                       FmpLibMock;

    MockBasePcdLib                   BasePcdLibMock;
    MockUefiRuntimeServicesTableLib  UefiRuntimeServicesTableLibMock;

    RETIMER_CONFIG MockRetimerConfig [4] = {
     { INTEGRATED_TBT_RETIMER, 0xFF, 0xFF, {0x00, 0x0D, 0x02, 0x00, 0x01, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
     { DISCRETE_TBT_RETIMER  , 0xFF, 0xFF, {0x00, 0x0D, 0x02, 0x00, 0x01, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
     { DISCRETE_TBT_RETIMER  , 0x00, 0xFF, {0x00, 0x0D, 0x02, 0x00, 0x01, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
     { DISCRETE_TBT_RETIMER  , 0x00, 0x01, {0x00, 0x0D, 0x02, 0x00, 0x01, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}
    };

    RETIMER_DEV_ADDRESS  MockRetimerDevAddress [1] = {
      0x00, 0x0D, 0x02, 0x00, 0x01, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };

    MOCK_DISCRETE_TBT_PAYLOAD_DATA MockDtbtImage = {
      {
        SIGNATURE_32 ('D', 'T', 'B', 'T'),      //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
        0x20,                                   //  HeaderSize
        1,                                      //  DiscreteTbtCount
        0                                       //  Reserved
      },                                        // DISCRETE_TBT_PAYLOAD_HEADER
      {
        0x20,                                   //  ImageOffset
        0x04,                                   //  ImageSize
        0x02,                                   //  FirmwareType
        0x00,                                   //  PcieRpType
        0x19,                                   //  PcieRootPort
        { 0, 0, 0, 0, 0 }                       //  Reserved[5]
      },                                        // DISCRETE_TBT_ITEM
      0x00000000                                // ImageContent       // The value use to BufferPointer[0]
    };

    MOCK_RETIMER_PAYLOAD_DATA MockItbtRetimerImage = {
      {
        SIGNATURE_32 ('R', 'T', 'M', 'R'),      //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
        0x30,                                   //  HeaderSize
        1,                                      //  RetimerCount
        0                                       //  Reserved
      },                                        // RETIMER_PAYLOAD_HEADER
      {
        {
          0x00,                                 //   Bus
          0x0D,                                 //   Device
          0x03,                                 //   Function
          0x00,                                 //   Port
          0x01,                                 //   RetimerIndex
          { 0, 0, 0, 0, 0, 0, 0, 0 }            //   Reserved[8]
        },                                      //  RetimerDevAddress // RETIMER_DEV_ADDRESS
        0x30,                                   //  ImageOffset
        0x04,                                   //  ImageSize
        0x00,                                   //  FirmwareType
        0xFF,                                   //  PcieRpType
        0xFF,                                   //  PcieRootPort
        { 0, 0, 0, 0, 0 }                       //  Reserved[5]
      },                                        // RETIMER_ITEM
      0x00000000                                // ImageContent       // The value use to BufferPointer[0]
    };
};

//**********************************************************
// GoogleTest framework                                    *
//**********************************************************
int main (int argc, char **argv) {
  testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
