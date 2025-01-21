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

#ifndef MOCK_TBT_NVM_DRV_HR_H_
#define MOCK_TBT_NVM_DRV_HR_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/TbtNvmDrvHrProtocol.h>
#include <Protocol/TbtNvmDrvDevice.h>
#include <TbtNvmRetimer.h>
#include <Library/TbtNvmDrvHr.h>

extern TBT_HOST_ROUTER  LocalHrPtr;
}

struct MockTbtNvmDrvHr {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvHr);

  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvHr_WriteCioReg,
    (IN TBT_HOST_ROUTER *This,
     IN UINT8           ConfigurationSpace,
     IN UINT8           Adapter,
     IN UINT16          RegNum,
     IN UINT8           Length,
     IN UINT32          *DataPtr)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvHr_ReadCioDevReg,
    (IN  TBT_HOST_ROUTER *This,
     IN  UINT8           ConfigurationSpace,
     IN  UINT8           Adapter,
     IN  UINT16          RegNum,
     OUT UINT32          *DataPtr)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    TbtNvmDrvHr_Dtor,
    (IN TBT_HOST_ROUTER *This)
    );
};

#endif
