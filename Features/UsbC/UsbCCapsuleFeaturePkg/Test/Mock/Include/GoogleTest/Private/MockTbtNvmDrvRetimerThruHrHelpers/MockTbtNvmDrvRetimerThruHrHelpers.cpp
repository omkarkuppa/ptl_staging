/** @file
  Google Test mocks for TbtNvmDrvRetimerThruHrHelpers

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

#include "MockTbtNvmDrvRetimerThruHrHelpers.h"

MOCK_INTERFACE_DEFINITION(MockTbtNvmDrvRetimerThruHrHelpers);

MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHrHelpers, WaitForMsgOutTxDone,  3, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHrHelpers, SendCommandToLocalLc, 4, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHrHelpers, SendEnumCmd,          1, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHrHelpers, SendLsupCmdDis,       1, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvRetimerThruHrHelpers, SendLsupCmdEn,        1, EFIAPI);