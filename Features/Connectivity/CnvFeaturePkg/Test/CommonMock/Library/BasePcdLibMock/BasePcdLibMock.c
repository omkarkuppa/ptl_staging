/**@file

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

@par Specification
**/

#include <Uefi.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/**
  This function provides a means by which to retrieve a value for a given PCD token.

  Returns the Boolean value of the token specified by TokenNumber.

  @param[in]  TokenNumber The PCD token number to retrieve a current value for.

  @return Returns the Boolean value of the token specified by TokenNumber.

**/
BOOLEAN
EFIAPI
LibPcdGetBool (
  IN UINTN  TokenNumber
  )
{
  check_expected (TokenNumber);

  return mock_type (BOOLEAN);
}

/**
  This function provides a means by which to retrieve a value for a given PCD token.

  Returns the 8-bit value for the token specified by TokenNumber.

  @param[in]  TokenNumber The PCD token number to retrieve a current value for.

  @return Returns the 8-bit value for the token specified by TokenNumber.

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

/**
  This function provides a means by which to retrieve a value for a given PCD token.

  Returns the 8-bit value for the token specified by TokenNumber.

  @param[in]  TokenNumber The PCD token number to retrieve a current value for.

  @return Returns the 8-bit value for the token specified by TokenNumber.

**/
UINT32
EFIAPI
LibPcdGet32 (
  IN UINTN  TokenNumber
  )
{
  check_expected (TokenNumber);

  return mock_type (UINT32);
}

/**
  Mock of LibPcdSet8S

  @param[in] TokenNumber    The PCD token number to set a current value for.
  @param[in] Value          The 8-bit value to set.

  @return The status of the set operation.

**/
RETURN_STATUS
EFIAPI
LibPcdSet8S (
  IN UINTN  TokenNumber,
  IN UINT8  Value
  )
{
  Value = mock_type (UINT8);

  return mock_type (EFI_STATUS);
}
