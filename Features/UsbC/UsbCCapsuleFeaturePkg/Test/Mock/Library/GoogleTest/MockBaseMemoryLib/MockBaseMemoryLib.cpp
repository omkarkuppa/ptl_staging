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

#include <GoogleTest/Library/MockBaseMemoryLib.h>

MOCK_INTERFACE_DEFINITION (MockBaseMemoryLib);

MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, CopyMem, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, SetMem, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, SetMem16, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, SetMem32, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, SetMem64, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, SetMemN, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, ZeroMem, 2, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, CompareMem, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, ScanMem8, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, ScanMem16, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, ScanMem32, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, ScanMem64, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, ScanMemN, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, CopyGuid, 2, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, CompareGuid, 2, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, ScanGuid, 3, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, IsZeroGuid, 1, EFIAPI);
MOCK_FUNCTION_INTERNAL_DEFINITION (MockBaseMemoryLib, IsZeroBuffer, 2, EFIAPI);
