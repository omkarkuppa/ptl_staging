/** @file
  Google Test mocks for TbtNvmDrvYflRouter

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

#include "MockTbtNvmDrvYflRouter.h"

MOCK_INTERFACE_DEFINITION(MockTbtNvmDrvYflRouter);

MOCK_FUNCTION_DEFINITION(MockTbtNvmDrvYflRouter, TbtNvmDrvYflForcePwrFunc, 3, EFIAPI);
