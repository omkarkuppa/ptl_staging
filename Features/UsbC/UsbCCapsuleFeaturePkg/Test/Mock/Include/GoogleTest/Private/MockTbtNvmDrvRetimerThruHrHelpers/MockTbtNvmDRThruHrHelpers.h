/** @file
  Google Test mocks for TbtNvmDrvRetimerThruHrHelpers

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

#ifndef MOCK_TBT_NVM_DRV_RETIMER_THRU_HR_HELPERS_H_
#define MOCK_TBT_NVM_DRV_RETIMER_THRU_HR_HELPERS_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/TbtNvmDrvUtils.h>
#include <Protocol/TbtNvmDrvRetimerThruHrProtocol.h>
#include <Library/TbtNvmDrvRetimerThruHrHelpers.h>
}

struct MockTbtNvmDRThruHrHelpers {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDRThruHrHelpers);

  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    WaitForMsgOutTxDone,
    (IN RETIMER_THRU_HR     *RetimerPtr,
     IN UINT16              MsgOutCmdOffset,
     IN UINT8               DbgData)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    SendCommandToLocalLc,
    (IN  RETIMER_THRU_HR *RetimerPtr,
     IN  UINT8           ConfigurationSpace,
     IN  UINT32          Cmd,
     OUT UINT32          *DataPtr)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    SendEnumCmd,
    (IN RETIMER_THRU_HR *RetimerPtr)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    SendLsupCmdDis,
    (IN RETIMER_THRU_HR *RetimerPtr)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    SendLsupCmdEn,
    (IN RETIMER_THRU_HR *RetimerPtr)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    SendOfflineMode,
    (IN  RETIMER_THRU_HR *HrDevice,
     IN  UINT32          Data)
    );
};

#endif
