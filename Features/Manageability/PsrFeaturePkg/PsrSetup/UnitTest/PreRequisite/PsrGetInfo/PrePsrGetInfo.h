/** @file
  PreRequisite functions of the PsrGetInfo function.

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
#include <PsrSetup/PsrSetup.h>

/**
  Initial PSR configuration.

  @param[out] PsrConfiguration    Pointer of PSR Configuration.

  @retval EFI_SUCCESS             Initial PSR Configuration successfully.
  @retval Others                  Failed to get PSR data.
**/
EFI_STATUS
PsrGetInfo (
  OUT PSR_CONFIGURATION    *PsrConfiguration
  );

/**
  Set up services for PsrGetInfo function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupPsrGetInfoEnvironment (
  VOID
  );

/**
  Set up the environment and execute PsrGetInfo function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PrePsrGetInfo (
  IN UNIT_TEST_CONTEXT  Context
  );
