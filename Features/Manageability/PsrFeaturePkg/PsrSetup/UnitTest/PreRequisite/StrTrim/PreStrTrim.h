/** @file
  PreRequisite functions of the StrTrim function.

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
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>

/**
  Removes (trims) specified leading and trailing characters from a string.

  @param[in][out]  Str      Pointer to the null-terminated string to be trimmed.
                            On return, Str will hold the trimmed string.

  @param[in]       CharC    Character will be trimmed from str.
**/
VOID
EFIAPI
StrTrim (
  IN OUT CHAR16    *Str,
  IN     CHAR16    CharC
  );

/**
  Set up services for StrTrim function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupStrTrimEnvironment (
  VOID
  );

/**
  Set up the environment and execute StrTrim function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreStrTrim (
  IN UNIT_TEST_CONTEXT  Context
  );
