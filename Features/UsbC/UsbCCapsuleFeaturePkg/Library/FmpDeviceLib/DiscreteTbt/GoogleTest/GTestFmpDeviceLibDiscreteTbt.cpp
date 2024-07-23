/** @file
  Google Test for the implementation of FmpDeviceLib instance to support 
  Discrete Thunderbolt Firmware update

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

@par Specification Reference:

**/

#include <Library/GoogleTestLib.h>
#include <iostream>
using namespace std;
using namespace testing;

extern "C" {
  #include <Base.h>
  #include <Uefi/UefiBaseType.h>
  #include <Library/MemoryAllocationLib.h>
  #include <Library/BaseMemoryLib.h>

  #include <TbtNvmRetimer.h>

  #include <Library/FmpDeviceLib.h>
  #include <Protocol/PciIo.h>
}
//**********************************************************
// GoogleTest mock                                         *
//**********************************************************
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Library/MockTbtNvmRetimerUpdateLib.h>
#include "GoogleTest/Private/MockPciIopProtocolLib/MockPciIopProtocolLib.cpp"
#include "GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.cpp"

typedef struct {
  PAYLOAD_HEADER                     PayloadHeader;
  DISCRETE_TBT_ITEM                  PayloadItem;
} DISCRETE_TBT_PAYLOAD_DATA;

class CommonMock : public Test {
protected:
  MockFirmwareManagementLib           FirmwareManagementMock;
  MockUefiBootServicesTableLib        UefiBootServicesTableLibMock;
  MockTbtNvmRetimerUpdateLib          TbtNvmRetimerUpdateMock;
  MockPciIopProtocolLib               PciIopProtocolMock;

  DISCRETE_TBT_PAYLOAD_DATA MockDtbtImage = {
    {
      SIGNATURE_32 ('D', 'T', 'B', 'T'),      //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
      0x20,                                   //  HeaderSize
      1,                                      //  DiscreteTbtCount
      0                                       //  Reserved
    },                                        // DISCRETE_TBT_PAYLOAD_HEADER
    {
      0x24,                                   //  ImageOffset
      0x04,                                   //  ImageSize
      0x02,                                   //  FirmwareType
      0x00,                                   //  PcieRpType
      0x19,                                   //  PcieRootPort
      { 0, 0, 0, 0, 0 }                       //  Reserved[5]
    }                                         // DISCRETE_TBT_ITEM
  };

  DISCRETE_TBT_PAYLOAD_DATA MockNonDtbtImage = {
    {
      SIGNATURE_32 ('D', 'T', 'B', 'T'),      //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
      0x20,                                   //  HeaderSize
      1,                                      //  DiscreteTbtCount
      0                                       //  Reserved
    },                                        // DISCRETE_TBT_PAYLOAD_HEADER
    {
      0x24,                                   //  ImageOffset
      0x04,                                   //  ImageSize
      0x07,                                   //  FirmwareType
      0x00,                                   //  PcieRpType
      0x19,                                   //  PcieRootPort
      { 0, 0, 0, 0, 0 }                       //  Reserved[5]
    }                                         // DISCRETE_TBT_ITEM
  };
};

//**********************************************************
// GoogleTest framework                                    *
//**********************************************************
int main (int argc, char **argv) {
  testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
