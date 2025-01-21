/**@file

    PreRequisite of the UpdatePmaxTable function.

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

#include <PiPei.h>
#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <PrivateMock.h>


/**
  Set up the environment and execute FindPmaxTable function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreUpdatePmaxTable (
  IN UNIT_TEST_CONTEXT        Context
  );

/**
  Get PCD and update the values.

  @param[in, out] PmaxTable     Point to PMAX table.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval Other                 The function encountered an error and could not complete process.
**/
EFI_STATUS
UpdatePmaxTable (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER *PmaxTable
  );

/**
  Mock of LibPcdGetExPtr.

  @param[in]  Guid          The pointer to a 128-bit unique value that designates
                            which namespace to retrieve a value from.
  @param[in]  TokenNumber   The PCD token number to retrieve a current value for.

  @return Return the VOID* pointer.

**/
VOID*
EFIAPI
LibPcdGetExPtr (
  IN CONST GUID  *Guid,
  IN UINTN       TokenNumber
  );

/**
  Set up the environment for GetPmaxConfig.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/

UNIT_TEST_STATUS
EFIAPI
PreCheckPmaxConfig (
  IN UNIT_TEST_CONTEXT        Context
  );
