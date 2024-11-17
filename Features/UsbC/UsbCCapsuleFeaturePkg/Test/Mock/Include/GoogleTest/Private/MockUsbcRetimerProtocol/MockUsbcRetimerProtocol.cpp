/** @file
  Google Test mocks for UsbcRetimerProtocol

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

#include "MockUsbcRetimerProtocol.h"

MOCK_INTERFACE_DEFINITION (MockUsbcRetimerProtocol);

MOCK_FUNCTION_DEFINITION (MockUsbcRetimerProtocol, UsbcRetimerProtocol_GetPdControllerMode, 3, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockUsbcRetimerProtocol, UsbcRetimerProtocol_SetPdControllerMode, 1, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockUsbcRetimerProtocol, UsbcRetimerProtocol_RetimerFP, 1, EFIAPI);

USBC_RETIMER_PROTOCOL  LocalURP = {
  (GET_PD_CONTROLLER_MODE) UsbcRetimerProtocol_GetPdControllerMode,
  (SET_PD_CONTROLLER_MODE) UsbcRetimerProtocol_SetPdControllerMode,
  (CONTROL_RETIMER_FP_GPIO) UsbcRetimerProtocol_RetimerFP        ///  Control Retimer FP GPIO
};
