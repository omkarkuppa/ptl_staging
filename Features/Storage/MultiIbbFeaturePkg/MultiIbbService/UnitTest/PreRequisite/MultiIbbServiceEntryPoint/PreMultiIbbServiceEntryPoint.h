/** @file
  PreRequisite functions of the MultiIbbServiceEntryPoint function.

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
#include <PiPei.h>

/**
  Provide Multi-Ibb service.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @retval    EFI_SUCCESS  MultiIbbInfo created and cacheability set-up for Ibb1 successfully
**/
EFI_STATUS
EFIAPI
MultiIbbServiceEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

/**
  Set up services for MultiIbbServiceEntryPoint function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupMultiIbbServiceEntryPointEnvironment (
  VOID
  );

/**
  Set up the environment and execute MultiIbbServiceEntryPoint function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreMultiIbbServiceEntryPoint (
  IN UNIT_TEST_CONTEXT  Context
  );
