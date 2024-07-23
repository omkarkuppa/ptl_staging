/** @file
  Google Test mocks for TbtNvmDrvHr

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

#include "MockTbtNvmDrvHr.h"

MOCK_INTERFACE_DEFINITION(MockTbtNvmDrvHr);

MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvHr, TbtNvmDrvHrWriteCioReg,   6, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvHr, TbtNvmDrvHrReadCioDevReg, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvHr, TbtNvmDrvHrCtor,          4, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvHr, TbtNvmDrvHrDtor,          1, EFIAPI);

static TBT_HOST_ROUTER    LocalHrPtr = {
  NULL,                                // TBT_HR_IMPL
  TbtNvmDrvHrWriteCioReg,              // WRITE_CIO_DEV_REG
  TbtNvmDrvHrReadCioDevReg,            // ReadCioDevReg
  TbtNvmDrvHrDtor,                     // Dtor
};

TBT_HOST_ROUTER* gDevComHostMock = &LocalHrPtr;
