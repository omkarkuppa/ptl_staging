/** @file
  Google Test mocks for TbtNvmDrvRetimerThruHr

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

#include "MockTbtNvmDrvRetimerThruHr.h"

MOCK_INTERFACE_DEFINITION(MockTbtNvmDrvRetimerThruHr);

MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHr, WriteIecs,                  4, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHr, ReadIecs,                   4, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHr, TbtNvmDrvRetimerThruHrCtor, 6, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHr, Dtor,                       1, EFIAPI);


static TBT_RETIMER    LocalCommunicationPtr = {
  NULL,                                // VOID
  WriteIecs,                           // WRITE_IECS_REG
  ReadIecs,                            // READ_IECS_REG
  Dtor,                                // DESTROY
  NULL,                                // INDICATE_STATE
};

TBT_RETIMER* gDevComRetimerMock = &LocalCommunicationPtr;
