/** @file
  Google Test mocks for BaseMemoryLib

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

#ifndef MOCK_BASE_MEMORY_LIB_H_
#define MOCK_BASE_MEMORY_LIB_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
  #include <Base.h>
  #include <Library/BaseMemoryLib.h>
}

struct MockBaseMemoryLib {
  MOCK_INTERFACE_DECLARATION (MockBaseMemoryLib);

  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    CopyMem,
    (VOID  *DestinationBuffer,
     CONST VOID  *SourceBuffer,
     UINTN  Length)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    SetMem,
    (VOID  *Buffer,
     UINTN  Length,
     UINT8  Value)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    SetMem16,
    (VOID  *Buffer,
     UINTN  Length,
     UINT16  Value)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    SetMem32,
    (VOID  *Buffer,
     UINTN  Length,
     UINT32  Value)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    SetMem64,
    (VOID  *Buffer,
     UINTN  Length,
     UINT64  Value)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    SetMemN,
    (VOID  *Buffer,
     UINTN  Length,
     UINTN  Value)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    ZeroMem,
    (VOID  *Buffer,
     UINTN  Length)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    INTN,
    CompareMem,
    (CONST VOID  *DestinationBuffer,
     CONST VOID  *SourceBuffer,
     UINTN  Length)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    ScanMem8,
    (CONST VOID  *Buffer,
     UINTN  Length,
     UINT8  Value)
    );

  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    ScanMem16,
    (CONST VOID  *Buffer,
     UINTN  Length,
     UINT16  Value)
    );

  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    ScanMem32,
    (CONST VOID  *Buffer,
     UINTN  Length,
     UINT32  Value)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    ScanMem64,
    (CONST VOID  *Buffer,
     UINTN  Length,
     UINT64  Value)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    ScanMemN,
    (CONST VOID  *Buffer,
     UINTN  Length,
     UINTN  Value)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    GUID *,
    CopyGuid,
    (GUID  *DestinationGuid,
     CONST GUID  *SourceGuid)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    BOOLEAN,
    CompareGuid,
    (CONST GUID  *Guid1,
     CONST GUID  *Guid2)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    VOID *,
    ScanGuid,
    (CONST VOID  *Buffer,
     UINTN  Length,
     CONST GUID  *Guid)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    BOOLEAN,
    IsZeroGuid,
    (CONST GUID  *Guid)
    );
  MOCK_FUNCTION_INTERNAL_DECLARATION (
    BOOLEAN,
    IsZeroBuffer,
    (CONST VOID  *Buffer,
     UINTN  Length)
    );
};

#endif
