/** @file
  PreRequisite functions of the AdjustInitNemRange function.

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
#include <PiPei.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>

/**
  Adjust initial NEM range to cover Common IBB only if it is not set by ACM at the beginning.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval Others               - An error occurred when processing the function.
**/
EFI_STATUS
EFIAPI
AdjustInitNemRange (
  VOID
  );

/**
  Set up services for AdjustInitNemRange function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupAdjustInitNemRangeEnvironment (
  VOID
  );

/**
  Set up the environment and execute AdjustInitNemRange function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreAdjustInitNemRange (
  IN UNIT_TEST_CONTEXT  Context
  );
