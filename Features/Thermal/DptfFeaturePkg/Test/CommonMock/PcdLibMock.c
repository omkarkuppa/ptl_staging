/**@file
    PcdLib mock.
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

@par Specification
**/

#include <Uefi.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>

/**
  Mock of LibPcdGet8.
  @param[in]  TokenNumber   The PCD token number to retrieve a current value for.
  @return Return the VOID* pointer.
**/
UINT8
EFIAPI
LibPcdGet8 (
  IN UINTN  TokenNumber
  )
{
  check_expected (TokenNumber);
  return mock_type (UINT8);
}

