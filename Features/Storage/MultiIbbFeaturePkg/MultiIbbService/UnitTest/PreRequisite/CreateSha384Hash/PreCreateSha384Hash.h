/** @file
  PreRequisite functions of the CreateSha384Hash function.

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
  Calculate SHA384 Hash

  @param[in]  Data   data
  @param[in]  Size   data size
  @param[out] Digest SHA384 digest
**/
VOID
CreateSha384Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  );

/**
  Set up services for CreateSha384Hash function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupCreateSha384HashEnvironment (
  VOID
  );

/**
  Set up the environment and execute CreateSha384Hash function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreCreateSha384Hash (
  IN UNIT_TEST_CONTEXT  Context
  );
