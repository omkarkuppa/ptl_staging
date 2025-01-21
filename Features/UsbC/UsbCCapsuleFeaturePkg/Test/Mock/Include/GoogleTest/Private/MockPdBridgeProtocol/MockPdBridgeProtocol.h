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

#ifndef MOCK_PDBG_PROTOCOL_LIB_H_
#define MOCK_PDBG_PROTOCOL_LIB_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Protocol/UsbCPdBridgeProtocol.h>
extern USBC_PD_BRIDGE_PROTOCOL localPDBG;
}

struct MockPdBridgeProtocol {
  MOCK_INTERFACE_DECLARATION (MockPdBridgeProtocol);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    PdBridgeProtocol_GetVersion,
    (IN USBC_PD_BRIDGE_PROTOCOL  *This,
     IN UINT8                    PdCntrlIndex,
     IN UINT64                   *PdBridgeVersion)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    PdBridgeProtocol_Lock,
    (IN  USBC_PD_BRIDGE_PROTOCOL  *This,
     IN  UINT8                    Lock)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    PdBridgeProtocol_ExecuteVendorCmd,
    (IN  USBC_PD_BRIDGE_PROTOCOL  *This,
     IN  UINT8                    PdCntrlIndex,
     IN  UINT8                    VendorCmd,
     IN  BOOLEAN                  Lock,
     IN  UINT8                    *InputData,
     IN  UINT8                    *InputDataSize,
     OUT UINT8                    *OutputData OPTIONAL,
     OUT UINT8                    *OutputDataSize OPTIONAL)
    );
};

#endif
