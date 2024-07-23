/** @file
  PreRequisite functions of the IbbCacheabilityType function.

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
#include <Library/MtrrLib.h>

/**
  Cacheability set-up for the upcoming IBB

  @param[in]  IbbIndex   Currently processing IBB index
  @param[in]  CacheType  Cache type for new IBB

  @retval EFI_SUCCESS    - The function completed successfully.
**/
EFI_STATUS
EFIAPI
IbbCacheabilityType(
  IN UINTN                      IbbIndex,
  IN MTRR_MEMORY_CACHE_TYPE     CacheType
  );

/**
  Set up services for IbbCacheabilityType function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupIbbCacheabilityTypeEnvironment (
  VOID
  );

/**
  Set up the environment and execute IbbCacheabilityType function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreIbbCacheabilityType (
  IN UNIT_TEST_CONTEXT  Context
  );
