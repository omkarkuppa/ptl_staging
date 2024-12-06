/** @file
  Google Test mocks for PdBridgeProtocol

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

#include "MockPdBridgeProtocol.h"

MOCK_INTERFACE_DEFINITION (MockPdBridgeProtocol);

MOCK_FUNCTION_DEFINITION (MockPdBridgeProtocol, PdBridgeProtocol_GetVersion, 3, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPdBridgeProtocol, PdBridgeProtocol_Lock, 2, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPdBridgeProtocol, PdBridgeProtocol_ExecuteVendorCmd, 8, EFIAPI);


USBC_PD_BRIDGE_PROTOCOL  localPDBG = {
    0,
    (GET_PD_BRIDGE_VERSION) PdBridgeProtocol_GetVersion,
    (LOCK_COMMUNICATION) PdBridgeProtocol_Lock,
    (EXECUTE_VENDOR_CMD) PdBridgeProtocol_ExecuteVendorCmd
};