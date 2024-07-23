/** @file
  Unit tests for the implementation of TbtNvmDrvHr.

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
  #include <PiDxe.h>
  #include <Base.h>
  #include <Uefi/UefiBaseType.h>
  #include <Library/MemoryAllocationLib.h>
  #include <Library/BaseMemoryLib.h>
  #include <IndustryStandard/Pci22.h>
//
// Test fucntion header file.
//
  #include <Protocol/PciIo.h>
  #include <Protocol/TbtNvmDrvDmaProtocol.h>
  #include <Protocol/TbtNvmDrvHrProtocol.h>
  #include <Library/TbtNvmDrvHr.h>
  #include <Library/TbtNvmDrvUtils.h>
  #include <TbtNvmRetimer.h>
  #include <ITbtInfoHob.h>
//
// Test function define.
//
TBT_STATUS
TbtNvmDrvHrSendDrvReady (
  TBT_DMA *DmaPtr
  );

TBT_STATUS
TbtNvmDrvHrWriteCioReg (
  IN TBT_HOST_ROUTER *This,
  IN UINT8           ConfigurationSpace,
  IN UINT8           Adapter,
  IN UINT16          RegNum,
  IN UINT8           Length,
  IN UINT32          *DataPtr
  );

TBT_STATUS
TbtNvmDrvHrReadCioDevReg (
  IN  TBT_HOST_ROUTER *This,
  IN  UINT8           ConfigurationSpace,
  IN  UINT8           Adapter,
  IN  UINT16          RegNum,
  OUT UINT32          *DataPtr
  );

VOID
TbtNvmDrvHrDtor (
  TBT_HOST_ROUTER *This
  );
}

//**********************************************************
// GoogleTest mock                                         *
//**********************************************************

// Edk2 common mock
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Library/MockHobLib.h>

// Private mock
#include "GoogleTest/Private/MockTbtNvmDrvDma/MockTbtNvmDrvDma.cpp"
#include "GoogleTest/Private/MockTbtNvmDrvYflRouter/MockTbtNvmDrvYflRouter.cpp"
#include "GoogleTest/Private/MockPchPciBdfLib/MockPchPciBdfLib.cpp"
#include "GoogleTest/Private/MockBasePciSegmentLib/MockPciSegmentLib.cpp"
#include "GoogleTest/Private/MockPciIoProcotol/MockPciIoProcotol.cpp"

class CommonMock : public Test {
  protected:
    MockUefiBootServicesTableLib        UefiBootServicesTableLibMock;
    MockHobLib                          HobLibMock;
    MockTbtNvmDrvDma                    TbtNvmDrvDmaMock;
    MockTbtNvmDrvYflRouter              TbtNvmDrvYflRouterMock;
    MockPchPciBdfLib                    PchPciBdfLibMock;
    MockPciSegmentLib                   PciSegmentLibMock;
    MockPciIoProcotol                   PciIoProcotolMock;
};

//**********************************************************
// GoogleTest framework                                    *
//**********************************************************
int main (int argc, char **argv) {
  testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
