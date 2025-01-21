/** @file
  Google Test mocks for DebugLib

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
**/

#ifndef MOCK_DEBUG_LIB_H_
#define MOCK_DEBUG_LIB_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Base.h>
#include <Library/DebugLib.h>
}

struct MockDebugLib {
  MOCK_INTERFACE_DECLARATION (MockDebugLib);

  MOCK_FUNCTION_DECLARATION (
    VOID,
    DebugVPrint,
    (UINTN  ErrorLevel,
     CONST CHAR8  *Format,
     VA_LIST  VaListMarker)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    DebugBPrint,
    (UINTN  ErrorLevel,
     CONST CHAR8  *Format,
     BASE_LIST  BaseListMarker)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    DebugAssert,
    (CONST CHAR8  *FileName,
     UINTN  LineNumber,
     CONST CHAR8  *Description)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID *,
    DebugClearMemory,
    (VOID  *Buffer,
     UINTN  Length)
    );
  MOCK_FUNCTION_DECLARATION (
    BOOLEAN,
    DebugAssertEnabled,
    ()
    );
  MOCK_FUNCTION_DECLARATION (
    BOOLEAN,
    DebugPrintEnabled,
    ()
    );
  MOCK_FUNCTION_DECLARATION (
    BOOLEAN,
    DebugCodeEnabled,
    ()
    );
  MOCK_FUNCTION_DECLARATION (
    BOOLEAN,
    DebugClearMemoryEnabled,
    ()
    );
  MOCK_FUNCTION_DECLARATION (
    BOOLEAN,
    DebugPrintLevelEnabled,
    (CONST UINTN  ErrorLevel)
    );
};

#endif
