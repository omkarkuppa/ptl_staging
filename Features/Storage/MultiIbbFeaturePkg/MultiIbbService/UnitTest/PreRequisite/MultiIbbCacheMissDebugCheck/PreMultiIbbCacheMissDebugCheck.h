/** @file
  PreRequisite functions of the MultiIbbCacheMissDebugCheck function.

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
  This function is mainly for the debugging purpose that allows user to observe if any cache miss happen.
  It cannot be executed together with GetAllFvInfo() or it will cause an error.

  The Cache Miss debugging mechanism is to cache the previous IBB region as WB and corrupted its content,
  so that any callback to this corrupted IBB will be easily observed as system hang.

  @param[in]  CurrentIbbIndex  //The index of current IBB

  @retval EFI_SUCCESS     - The Cache Miss check enabled successfully.
**/
EFI_STATUS
EFIAPI
MultiIbbCacheMissDebugCheck(
  IN UINTN                      CurrentIbbIndex
  );

/**
  Set up services for MultiIbbCacheMissDebugCheck function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupMultiIbbCacheMissDebugCheckEnvironment (
  VOID
  );

/**
  Set up the environment and execute MultiIbbCacheMissDebugCheck function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreMultiIbbCacheMissDebugCheck (
  IN UNIT_TEST_CONTEXT  Context
  );
