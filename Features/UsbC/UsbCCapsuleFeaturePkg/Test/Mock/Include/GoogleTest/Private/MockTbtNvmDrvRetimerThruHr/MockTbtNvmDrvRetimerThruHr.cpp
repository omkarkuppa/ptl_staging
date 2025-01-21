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

UINT16 TBT_USB4_PORT_CAPABILITY_OFFSET;

MOCK_INTERFACE_DEFINITION (MockTbtNvmDrvRetimerThruHr);

MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvRetimerThruHr, MockThruHr_WriteIecs, 4, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvRetimerThruHr, MockThruHr_ReadIecs, 4, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvRetimerThruHr, MockThruHr_Destroy, 1, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvRetimerThruHr, MockThruHr_InitHW, 1, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvRetimerThruHr, MockThruHr_TerminateHW, 1, EFIAPI);

TBT_RETIMER  LocalCommunicationPtr = {
  NULL,                                  // VOID
  (WRITE_IECS_REG) MockThruHr_WriteIecs, // WRITE_IECS_REG
  (READ_IECS_REG) MockThruHr_ReadIecs,   // READ_IECS_REG
  (DESTROY) MockThruHr_Destroy,          // DESTROY
  NULL,                                  // INDICATE_STATE
  (INIT_HW) MockThruHr_InitHW,           // INIT_HW
  (TERMINATE_HW) MockThruHr_TerminateHW, // TERMINATE_HW
};
