/** @file
  SW SMI Handler for enabling PEP Bugcheck Critical Device.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#include "PepBccdSmm.h"
#include <Library/CpuPlatformLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>


GLOBAL_REMOVE_IF_UNREFERENCED SETUP_DATA               mSystemConfiguration;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_SYSTEM_TABLE2    *mSmst;

#define EFI_PEP_BCCD_SW_SMI                 0xCD

VOID
PepBccdCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     CommBufferSize
  )
{
  //
  // Do nothing for other board that does not support RTD3 storage device power gating
  //
}


/**
  Register SW SMI Handler for PEP BugCheck Critical Device.

  @retval  EFI_SUCCESS      SW SMI Handler is registered successfully.
  @retval  EFI_UNSUPPORTED  SW SMI Handler is not registered because RTD3 is not supported.
**/
EFI_STATUS
RegisterPepBccdSwSmi (
  VOID
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     SwHandle;
  EFI_SMM_SW_REGISTER_CONTEXT    SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL  *SwDispatch;

  //
  // Return EFI_UNSUPPORTED if RTD3 is disabled
  //
  if (mSystemConfiguration.Rtd3Support == 0) {
    return EFI_UNSUPPORTED;
  }

  //
  //  Locate the ICH SMM SW dispatch protocol
  //
  Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
  ASSERT_EFI_ERROR (Status);

  //
  // Register PEP Bugcheck Critical Device SW SMI handler
  //
  SwContext.SwSmiInputValue = EFI_PEP_BCCD_SW_SMI;

  Status = SwDispatch->Register (SwDispatch,
                                 (EFI_SMM_HANDLER_ENTRY_POINT2)PepBccdCallback,
                                 &SwContext,
                                 &SwHandle);
  ASSERT_EFI_ERROR (Status);

  return Status;
}



/**
  Initializes the SMM PEP BCCD Driver

  @param[in] ImageHandle    Pointer to the loaded image protocol for this driver
  @param[in] SystemTable    Pointer to the EFI System Table

  @retval    EFI_SUCCESS    Assert, otherwise.
**/
EFI_STATUS
EFIAPI
InitializePepBccdSmm (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                Status;
  UINTN                                     DataSize;

  //
  // Initialize global variables
  //
  mSmst = gSmst;

  //
  // Locate setup variable
  //
  DataSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  NULL,
                  &DataSize,
                  &mSystemConfiguration
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register PEP Bugcheck Critical Device SW SMI handler
  //
  Status = RegisterPepBccdSwSmi();
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "PEP BCCD SW SMI Handler is not installed\n"));
  }

  return EFI_SUCCESS;
}
