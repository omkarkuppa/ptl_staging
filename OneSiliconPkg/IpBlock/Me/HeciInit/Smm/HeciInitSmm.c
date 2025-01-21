/** @file
  HECI SMM driver

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

@par Specification Reference:
**/

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/HeciInitLib.h>
#include "HeciInitSmm.h"

//
// Global driver data
//
GLOBAL_REMOVE_IF_UNREFERENCED HECI_INSTANCE         *mHeciContext;

/**
  Returns if HECI retry is enabled.

  @retval TRUE             HECI retry is enabled
  @retval FALSE            HECI retry is disabled
**/
BOOLEAN
MeHeciRetryEnabled (
  VOID
  )
{
  return TRUE;
}

/**
  HECI driver entry point used to initialize support for the HECI device.

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
InitializeHeciSmm (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "ME-BIOS: HECI SMM Protocol Entry.\n"));

  mHeciContext = AllocateZeroPool (sizeof (HECI_INSTANCE));

  ///
  /// Initialize HECI protocol pointers
  ///
  if (mHeciContext == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI SMM Protocol Exit - Error by lack of memory for HECI_INSTANCE.\n"));
    return Status;
  }
  mHeciContext->HeciCtlr.ResetHeci    = ResetHeciInterface;
  mHeciContext->HeciCtlr.SendwAck     = HeciSendwAck;
  mHeciContext->HeciCtlr.ReadMsg      = HeciReceive;
  mHeciContext->HeciCtlr.SendMsg      = HeciSend;
  mHeciContext->HeciCtlr.InitHeci     = HeciInitialize;
  mHeciContext->HeciCtlr.ReInitHeci   = HeciReInitialize;
  mHeciContext->HeciCtlr.MeResetWait  = MeResetWait;
  mHeciContext->HeciCtlr.GetMeStatus  = HeciGetMeStatus;
  mHeciContext->HeciCtlr.GetMeMode    = HeciGetMeMode;

  ///
  /// Install the HECI interface
  ///
  Status = gSmst->SmmInstallProtocolInterface (
                    &mHeciContext->Handle,
                    &gHeciProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mHeciContext->HeciCtlr
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI SMM Protocol Exit - Error by install HECI protocol fail, Status: %r\n", Status));
    FreePool (mHeciContext);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "ME-BIOS: HECI SMM Protocol Exit - Success.\n"));

  return Status;
}
