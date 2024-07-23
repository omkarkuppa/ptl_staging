/** @file
  Unit tests for the implementation of TbtNvmRetimerDrvHelpers.

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
  #include <Library/TbtNvmDrvUtils.h>
  #include <Library/TbtNvmRetimerDrvHelpers.h>
//
// Test function define.
//
  TBT_STATUS
  TbtNvmDrvWaitForCmdCpl (
    IN VOID          *DevCom,
    IN UINT32        Cmd
    );

  TBT_STATUS
  TbtDrvReadDwFromNvm (
    IN  VOID          *DevCom,
    IN  UINT32        Offset,
    IN  UINT32        DataLength,
    OUT UINT32        *RdData
    );
//
// Global variable.
//
  UPDATE_TARGET_TYPE    gUpdateTargetType; 
}

//**********************************************************
// GoogleTest mock                                         *
//**********************************************************

// Edk2 common mock
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
// Private mock
#include "GoogleTest/Private/MockTbtNvmDrvHr/MockTbtNvmDrvHr.cpp"
#include "GoogleTest/Private/MockTbtNvmDrvRetimerThruHr/MockTbtNvmDrvRetimerThruHr.cpp"
#include "GoogleTest/Private/MockTbtNvmDrvRetimerThruHrHelpers/MockTbtNvmDrvRetimerThruHrHelpers.cpp"

class CommonMock : public Test {
  protected:
    MockUefiBootServicesTableLib       UefiBootServicesTableLibMock;
    MockTbtNvmDrvRetimerThruHr         TbtNvmDrvRetimerThruHrMock;
    MockTbtNvmDrvHr                    TbtNvmDrvHrMock;
    MockTbtNvmDrvRetimerThruHrHelpers  TbtNvmDrvRetimerThruHrHelpersMock;
};

//**********************************************************
// GoogleTest framework                                    *
//**********************************************************
int main (int argc, char **argv) {
  testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
