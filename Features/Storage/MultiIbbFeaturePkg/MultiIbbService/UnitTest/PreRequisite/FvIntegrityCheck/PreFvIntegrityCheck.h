/** @file
  PreRequisite functions of the FvIntegrityCheck function.

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
  Check the integrity of the new Ibb before installing it

  @param[in]  IbbIndex  Ibb index
  @param[in]  FvBuffer  Ibb base address
  @param[in]  FvSize    Ibb size

  @retval EFI_SUCCESS     - The integrity of the new Ibb is verified
  @retval EFI_LOAD_ERROR  - New Ibb is corrupted and lead to load error
**/
EFI_STATUS
EFIAPI
FvIntegrityCheck (
  IN UINTN                      IbbIndex,
  IN VOID                       *FvBuffer,
  IN UINTN                      FvSize
  );

/**
  Set up services for FvIntegrityCheck function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupFvIntegrityCheckEnvironment (
  VOID
  );

/**
  Set up the environment and execute FvIntegrityCheck function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreFvIntegrityCheck (
  IN UNIT_TEST_CONTEXT  Context
  );
