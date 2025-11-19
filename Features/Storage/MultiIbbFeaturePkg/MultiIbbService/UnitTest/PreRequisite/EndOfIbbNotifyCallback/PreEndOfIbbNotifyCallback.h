/** @file
  PreRequisite functions of the EndOfIbbNotifyCallback function.

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
  Callback for the cacheability changes before processing the next IBB

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The gEfiEndOfIbbSignalPpiGuid PPI. Not used.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval Others               - An error occurred when processing the callback
**/
EFI_STATUS
EFIAPI
EndOfIbbNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  );

/**
  Set up services for EndOfIbbNotifyCallback function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupEndOfIbbNotifyCallbackEnvironment (
  VOID
  );

/**
  Set up the environment and execute EndOfIbbNotifyCallback function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreEndOfIbbNotifyCallback (
  IN UNIT_TEST_CONTEXT  Context
  );
