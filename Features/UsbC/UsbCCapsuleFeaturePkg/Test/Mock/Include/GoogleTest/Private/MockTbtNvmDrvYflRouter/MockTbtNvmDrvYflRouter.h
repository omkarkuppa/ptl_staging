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

#ifndef MOCK_TBT_NVM_DRV_TFL_ROUTER_H_
#define MOCK_TBT_NVM_DRV_TFL_ROUTER_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
  #include <Uefi.h>
  #include <Library/UefiLib.h>
  #include <Protocol/PciIo.h>
  #include <Library/TbtNvmDrvUtils.h>
}

struct MockTbtNvmDrvYflRouter {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvYflRouter);

  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvYflForcePwrFunc,
    (IN  EFI_PCI_IO_PROTOCOL *PciIoProto,
     IN  BOOLEAN             Enable,
     OUT BOOLEAN             *TBTControllerWasPowered)
    );
};

#endif
