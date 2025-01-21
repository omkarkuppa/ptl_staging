/** @file
  Google Test mocks for UsbCPdBridgeUpdateLib

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

#ifndef MOCK_TBT_NVM_PDBG_UPDATE_LIB_H
#define MOCK_TBT_NVM_PDBG_UPDATE_LIB_H

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>

extern "C" {
#include <Uefi.h>
#include <Library/UsbCPdBridgeUpdateLib.h>
}

struct MockUsbCPdBridgeUpdateLib {
  MOCK_INTERFACE_DECLARATION (MockUsbCPdBridgeUpdateLib);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    UpdatePdBridgeNvmFirmware,
    (IN  USBC_PD_BRIDGE_PROTOCOL                           *This,
     IN  UINT8                                             *PDBridgeImage,
     IN  UINTN                                             ImageSize,
     IN  UINT8                                             PdCntrlIndex,
     IN  UINT8                                             ShareFlashMode,
     IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS     Progress, OPTIONAL
     IN  UINTN                                             StartPercentage,
     IN  UINTN                                             EndPercentage)
    );
};

#endif
