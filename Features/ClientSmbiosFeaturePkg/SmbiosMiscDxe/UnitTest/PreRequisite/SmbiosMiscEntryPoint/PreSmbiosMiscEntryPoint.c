/** @file
  PreRequisite function of the SmbiosMiscEntryPoint function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include "PreSmbiosMiscEntryPoint.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>
#include <PrivateMock.h>

#define UNIT_TEST_FUNCTION_NAME     "Pre-SmbiosMiscEntryPoint"
#define UNIT_TEST_FUNCTION_VERSION  1

EFI_BOOT_SERVICES* gBS;
/**
  Set up services for SmbiosMiscEntryPoint function.

  @retval     EFI_SUCCESS                 Environment set up success.

**/
EFI_STATUS
EFIAPI
SetupSmbiosMiscEntryPointEnvironment (
  )
{
  gSmbiosProtocol = AllocatePool (sizeof (EFI_SMBIOS_PROTOCOL));
  if (gSmbiosProtocol == NULL) {
    DEBUG ((DEBUG_ERROR, "SetupSmbiosMiscEntryPointEnvironment: Failed to allocate memory for EFI_SMBIOS_PROTOCOL\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  gSmbiosProtocol->Add = MockSmbiosAdd;

  gBS = AllocatePool (sizeof (EFI_BOOT_SERVICES));
  if (gBS == NULL) {
    DEBUG ((DEBUG_ERROR, "SetupSmbiosMiscEntryPointEnvironment: Failed to allocate memory for EFI_BOOT_SERVICES\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  gBS->LocateProtocol = MockBootServiceLocateProtocol;

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute SmbiosMiscEntryPoint function.

  @param[in]  Context                                The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreSmbiosMiscEntryPoint (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS            Status;

  DEBUG ((DEBUG_INFO, "[  PreReq  ] %a %d\n", UNIT_TEST_FUNCTION_NAME, UNIT_TEST_FUNCTION_VERSION));

  Status = SetupSmbiosMiscEntryPointEnvironment ();
  if (EFI_ERROR (Status)) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }

  return UNIT_TEST_PASSED;
}

