/** @file
  Google Test for the implementation of  FmpDeviceLib instance to support 
  Thunderbolt Retimer Firmware update

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
#include <Library/FunctionMockLib.h>
#include <iostream>
using namespace std;
using namespace testing;

extern "C" {
  #include <Uefi.h>
  #include <Base.h>
  #include <Uefi/UefiBaseType.h>
  #include <Library/MemoryAllocationLib.h>
  #include <Library/BaseMemoryLib.h>
  #include <Library/FmpDeviceLib.h>
  #include <Protocol/Usb2HostController.h>
  #include <Protocol/PciIo.h>
  #include <Protocol/UsbCRetimerProtocol.h>
  #include <TbtNvmRetimer.h>
}
//**********************************************************
// GoogleTest mock                                         *
//**********************************************************
#include <GoogleTest/Library/MockUefiLib.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Library/MockTbtNvmRetimerUpdateLib.h>
#include <GoogleTest/Library/MockTimerLib.h>
#include "GoogleTest/Private/MockUsbcRetimerProtocolLib/MockUsbcRetimerProtocolLib.cpp"
#include "GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.cpp"

typedef struct {
  PAYLOAD_HEADER                     PayloadHeader;
  RETIMER_ITEM                       PayloadItem;
} RETIMER_PAYLOAD_DATA;

class CommonMock : public Test  {
  protected:
    MockUefiRuntimeServicesTableLib     RtServicesMock;
    MockFirmwareManagementLib           FirmwareManagementMock;
    MockUefiBootServicesTableLib        UefiBootServicesTableLibMock;
    MockUsbcRetimerProtocolLib          UsbcRetimerProtocolMock;
    MockTbtNvmRetimerUpdateLib          TbtNvmRetimerUpdateMock;
    MockTimerLib                        MicroSecondDelayMock;
    RETIMER_PAYLOAD_DATA                MockRetimerImage = {
      {
        SIGNATURE_32 ('R', 'T', 'M', 'R'),      //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
        0x30,                                   //  HeaderSize
        1,                                      //  RetimerCount
        0                                       //  Reserved
      },                                        // RETIMER_PAYLOAD_HEADER
      {
        {
          0xFF,                                 //   Bus
          0xFF,                                 //   Device
          0xFF,                                 //   Function
          0x00,                                 //   Port
          0x01,                                 //   RetimerIndex
          { 0, 0, 0, 0, 0, 0, 0, 0 }            //   Reserved[8]
        },                                      //  RetimerDevAddress // RETIMER_DEV_ADDRESS
        0x30,                                   //  ImageOffset
        0x04,                                   //  ImageSize
        0x01,                                   //  FirmwareType
        0x00,                                   //  PcieRpType
        0x09,                                   //  PcieRootPort
        { 0, 0, 0, 0, 0 }                       //  Reserved[5]
      }                                         // RETIMER_ITEM
    };
    
    RETIMER_PAYLOAD_DATA MockIntegratedRetimerImage = {
      {
        SIGNATURE_32 ('R', 'T', 'M', 'R'),      //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
        0x30,                                   //  HeaderSize
        1,                                      //  RetimerCount
        0                                       //  Reserved
      },                                        // RETIMER_PAYLOAD_HEADER
      {
        {
          0xFF,                                 //   Bus
          0xFF,                                 //   Device
          0xFF,                                 //   Function
          0x00,                                 //   Port
          0x01,                                 //   RetimerIndex
          { 0, 0, 0, 0, 0, 0, 0, 0 }            //   Reserved[8]
        },                                      //  RetimerDevAddress // RETIMER_DEV_ADDRESS
        0x30,                                   //  ImageOffset
        0x04,                                   //  ImageSize
        0x00,                                   //  FirmwareType
        0x00,                                   //  PcieRpType
        0x09,                                   //  PcieRootPort
        { 0, 0, 0, 0, 0 }                       //  Reserved[5]
      }                                         // RETIMER_ITEM
    };

    RETIMER_PAYLOAD_DATA MockUnsupportedRetimerImage = {
      {
        SIGNATURE_32 ('R', 'T', 'M', 'R'),      //  Signature    // DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE
        0x30,                                   //  HeaderSize
        1,                                      //  RetimerCount
        0                                       //  Reserved
      },                                        // RETIMER_PAYLOAD_HEADER
      {
        {
          0xFF,                                 //   Bus
          0xFF,                                 //   Device
          0xFF,                                 //   Function
          0x00,                                 //   Port
          0x01,                                 //   RetimerIndex
          { 0, 0, 0, 0, 0, 0, 0, 0 }            //   Reserved[8]
        },                                      //  RetimerDevAddress // RETIMER_DEV_ADDRESS
        0x30,                                   //  ImageOffset
        0x04,                                   //  ImageSize
        0x02,                                   //  FirmwareType
        0x00,                                   //  PcieRpType
        0x09,                                   //  PcieRootPort
        { 0, 0, 0, 0, 0 }                       //  Reserved[5]
      }                                         // RETIMER_ITEM
    };
};

//**********************************************************
// GoogleTest framework                                    *
//**********************************************************
int main (int argc, char **argv) {
  testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
