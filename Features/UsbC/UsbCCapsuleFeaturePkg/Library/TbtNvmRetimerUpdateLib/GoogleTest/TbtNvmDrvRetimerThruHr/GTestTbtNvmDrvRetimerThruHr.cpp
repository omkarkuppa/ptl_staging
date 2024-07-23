/** @file
  Unit tests for the implementation of TbtNvmDrvRetimerThruHr.

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
  #include <Protocol/TbtNvmDrvDef.h>
  #include <Protocol/TbtNvmDrvRetimerThruHrProtocol.h>
  #include <Protocol/TbtNvmDrvDevice.h>
  #include <Library/TbtNvmDrvUtils.h>
  #include <Library/TbtNvmDrvRetimerThruHr.h>
  #include <TbtNvmRetimer.h>
//
// Test function define.
//
TBT_STATUS
WriteIecs (
  IN TBT_RETIMER      *This,
  IN UINT8            RegNum,
  IN UINT32           *DataPtr,
  IN UINT8            Length
  );

TBT_STATUS
ReadIecs (
  IN  TBT_RETIMER    *This,
  IN  UINT8          RegNum,
  IN  UINT32         DataLength,
  OUT UINT32         *DataPtr
  );

TBT_STATUS
CapabilityParsing (
  IN TBT_RETIMER *This
  );

VOID
StateFromDriver (
  IN TBT_RETIMER      *This,
  IN DRIVER_STATE     State
  );

VOID
Dtor (
  IN TBT_RETIMER *This
  );
}

//**********************************************************
// GoogleTest mock                                         *
//**********************************************************

// Edk2 common mock
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
// Private mock
#include "GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.cpp"
#include "GoogleTest/Private/MockTbtNvmRetimerDrvHelpers/MockTbtNvmRetimerDrvHelpers.cpp"
#include "GoogleTest/Private/MockTbtNvmDrvRetimerThruHrHelpers/MockTbtNvmDrvRetimerThruHrHelpers.cpp"

class CommonMock : public Test {
  protected:
    MockUefiBootServicesTableLib       UefiBootServicesTableLibMock;
    MockTbtNvmDrvHr                    TbtNvmDrvHrMock;
    MockTbtNvmRetimerDrvHelpers        TbtNvmRetimerDrvHelpersMock;
    MockTbtNvmDrvRetimerThruHrHelpers  TbtNvmDrvRetimerThruHrHelpersMock;
};

//**********************************************************
// GoogleTest framework                                    *
//**********************************************************
int main (int argc, char **argv) {
  testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
