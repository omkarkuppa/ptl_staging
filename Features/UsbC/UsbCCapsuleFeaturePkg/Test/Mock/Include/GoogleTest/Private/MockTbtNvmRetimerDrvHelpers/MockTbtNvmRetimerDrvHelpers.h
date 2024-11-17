/** @file
  Google Test mocks for TbtNvmRetimerDrvHelpers

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

#ifndef MOCK_TBT_NVM_RETIMER_DRV_HELPER_H_
#define MOCK_TBT_NVM_RETIMER_DRV_HELPER_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/TbtNvmDrvUtils.h>
}

struct MockTbtNvmRetimerDrvHelpers {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmRetimerDrvHelpers);

  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvSendCmd,
    (IN VOID          *DevCom,
     IN UINT32        Cmd,
     IN BOOLEAN       ResponseRequired)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvDeviceWrBlk,
    (IN VOID           *DevCom,
     UINT32            *Data,
     UINT8             Length)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvDeviceWrOffset,
    (IN VOID   *DevCom,
     IN UINT32 OffsetInDW)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvDeviceExecAuth,
    (IN VOID   *DevCom)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    TbtDrvReadNvmVersion,
    (IN  VOID          *DevCom,
     OUT UINT32        *Version)
    );
};

#endif
