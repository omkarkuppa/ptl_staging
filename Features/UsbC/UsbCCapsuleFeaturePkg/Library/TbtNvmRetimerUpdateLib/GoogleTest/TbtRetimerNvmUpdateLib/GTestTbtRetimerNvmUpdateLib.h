/** @file
  Unit tests for the implementation of TbtRetimerNvmUpdate.

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

@par Specification Reference:

**/

#include <Library/GoogleTestLib.h>
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
#include <Protocol/UsbCRetimerProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TbtNvmDrvUtils.h>
#include <Library/TbtNvmDrvHr.h>
#include <Library/TbtNvmRetimerUpdateLib.h>
#include <Library/TbtNvmDrvRetimerThruHr.h>
#include <Library/TbtNvmRetimerDrvHelpers.h>
#include <Library/TbtNvmDrvRetimerThruHrHelpers.h>
#include <TbtNvmRetimer.h>
}

using namespace testing;
