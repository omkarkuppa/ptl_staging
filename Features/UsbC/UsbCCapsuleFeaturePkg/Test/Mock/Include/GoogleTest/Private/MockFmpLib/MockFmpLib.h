/** @file
  Google Test mocks for FmpLib

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

#ifndef MOCK_FMP_LIB_H
#define MOCK_FMP_LIB_H

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
  #include <Uefi.h>
  #include <Library/UefiLib.h>
}

struct MockFmpLib {
  MOCK_INTERFACE_DECLARATION (MockFmpLib);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    Progress,
    (IN UINTN  Completion)
    );
};

#endif
