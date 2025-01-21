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

MOCK_INTERFACE_DEFINITION (MockTbtNvmDrvHr);

MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvHr, TbtNvmDrvHr_WriteCioReg, 6, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvHr, TbtNvmDrvHr_ReadCioDevReg, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvHr, TbtNvmDrvHr_Dtor, 1, EFIAPI);

TBT_HOST_ROUTER  LocalHrPtr = {
  NULL,
  {
    0,
    0
  },
  0,
  0,
  {
    0,
    0,
    0
  },
  NULL,                                         // TBT_HR_IMPL
  (WRITE_CIO_DEV_REG) TbtNvmDrvHr_WriteCioReg,  // WRITE_CIO_DEV_REG
  (READ_CIO_DEV_REG) TbtNvmDrvHr_ReadCioDevReg, // ReadCioDevReg
  (TBT_HR_DTOR) TbtNvmDrvHr_Dtor,               // Dtor
};
