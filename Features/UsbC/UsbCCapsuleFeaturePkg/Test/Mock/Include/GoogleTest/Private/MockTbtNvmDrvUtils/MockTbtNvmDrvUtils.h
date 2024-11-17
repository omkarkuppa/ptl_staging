/** @file
  Google Test mocks for TbtNvmDrvUtils

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

#ifndef MOCK_TBT_NVM_DRV_UTILS_H_
#define MOCK_TBT_NVM_DRV_UTILS_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Library/UefiLib.h>
}

struct MockTbtNvmDrvUtils {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvUtils);

  MOCK_FUNCTION_DECLARATION (
    VOID *,
    TbtNvmDrvAllocateMem,
    (IN UINTN  AllocationSize)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    TbtNvmDrvDeAllocateMem,
    (IN VOID *Mem)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    TbtNvmDrvSwapEndianess,
    (IN  UINT32 *Src,
     IN  UINT32 Length,
     OUT UINT32 *Dst)
    );
};

#endif
