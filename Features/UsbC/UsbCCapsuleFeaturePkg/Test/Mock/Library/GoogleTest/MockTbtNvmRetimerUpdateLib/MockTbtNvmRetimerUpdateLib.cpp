/** @file
  Google Test mocks for TbtNvmRetimerUpdateLib

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

#include <GoogleTest/Library/MockTbtNvmRetimerUpdateLib.h>

MOCK_INTERFACE_DEFINITION(MockTbtNvmRetimerUpdateLib);

MOCK_FUNCTION_DEFINITION(MockTbtNvmRetimerUpdateLib, ReadRetimerNvmVersion, 2, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmRetimerUpdateLib, DestroyRetimerDevInstance, 1, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmRetimerUpdateLib, UpdateRetimerNvmInformation, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmRetimerUpdateLib, UpdateRetimerNvmFirmware, 6, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmRetimerUpdateLib, CreateRetimerDevInstance, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmRetimerUpdateLib, CreateTBTDevInstance, 4, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmRetimerUpdateLib, UpdateDiscreteTbtNvmFirmware, 6, EFIAPI);
MOCK_FUNCTION_DEFINITION(MockTbtNvmRetimerUpdateLib, DestroyTbtDevInstance, 1, EFIAPI);