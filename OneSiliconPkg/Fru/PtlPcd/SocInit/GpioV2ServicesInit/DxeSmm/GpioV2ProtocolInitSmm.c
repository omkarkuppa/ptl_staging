/** @file
  This file provides initialization code for Gpio V2 Controller SMM module

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

#include <Library/SmmServicesTableLib.h>
#include <Library/GpioV2SmmAccessLib.h>
#include "GpioV2ProtocolInit.h"

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SmmExitBootServices.h>

static GPIOV2_INTERFACE *mGpioInterface = NULL;

/**
  This function is invoked at ExitBootServices()

  @param[in] Protocol        Protocol unique ID.
  @param[in] Interface       Interface instance.
  @param[in] Handle          The handle on which the interface is installed..

  @retval    Status.
**/
EFI_STATUS
EFIAPI
DisableGpiIeExitBootServicesCallback (
  IN      CONST EFI_GUID *Protocol,
  IN      VOID           *Interface,
  IN      EFI_HANDLE        Handle
  )
{
  return mGpioInterface->Public.DisableAllGpiIe (&(mGpioInterface->Public));
}

/**
  The GPIOV2 SMM driver entry point - Intalls GPIOV2 SMM Protocol.

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
  @param[in] SystemTable          EFI system table.

  @retval EFI_INVALID_PARAMETER   Invalid parameter
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
  @retval EFI_SUCCESS             Protocol interface successfully installed
**/
EFI_STATUS
EFIAPI
GpioV2ProtocolSmmInitEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;
  //GPIOV2_PWM                    *Pwm;
  VOID                          *Registration;

  DEBUG ((DEBUG_INFO, "[GPIOV2][SMM]: InstallGpioV2SmmProtocol Start\n"));

  //
  // Allocate Memory for mGpioInterface in SMRAM
  //
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData,
                                   sizeof (GPIOV2_INTERFACE),
                                   (VOID **) &mGpioInterface);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (mGpioInterface, sizeof (GPIOV2_INTERFACE));

  //
  // Allocate Memory for GPIO PWM in SMRAM
  //
  /**Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData,
                                   sizeof (GPIOV2_PWM),
                                   (VOID **) &Pwm);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][SMM]: Allocating memory for Pwm failed (%r)!\n", Status));
    Pwm = NULL;
  } else {
    ZeroMem (Pwm, sizeof (GPIOV2_PWM));
  }
  mGpioInterface->Pwm = Pwm;**/

  //
  // Construct Gpio Protocol for SMM
  //
  Status = InternalGpioInterfaceConstructor (mGpioInterface);
  if (EFI_ERROR (Status)) {
    gSmst->SmmFreePool ((VOID*) mGpioInterface);
    /**if (Pwm != NULL) {
      gSmst->SmmFreePool ((VOID*) Pwm);
    }**/
    return Status;
  }

  //
  // Install GpioV2SmmProtocol
  //
  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                  &Handle,
                  &gGpioV2SmmProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &(mGpioInterface->Public)
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][SMM]: Install GpioV2 protocol failed (%r)!\n", Status));
    gSmst->SmmFreePool ((VOID*) mGpioInterface);
    /**if (Pwm != NULL) {
      gSmst->SmmFreePool ((VOID*) Pwm);
    }**/
    return Status;
  }

  //
  // Install GpioV2SmmPwmProtocol
  //
  //if (Pwm != NULL) {
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gGpioV2SmmPwmProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &(mGpioInterface->Pwm)
                    );
    /**if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[GPIOV2][SMM]: Install GpioV2 PWM protocol failed (%r)!\n", Status));
      gSmst->SmmFreePool ((VOID*) Pwm);
      return Status;
    }**/
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][SMM]: Install GpioV2 PWM protocol failed (%r)!\n", Status));
    return Status;
  }

  //
  // Register callback to clear GPIO interrupt beafore OS handoff
  //
  gSmst->SmmRegisterProtocolNotify (
    &gEdkiiSmmExitBootServicesProtocolGuid,
    DisableGpiIeExitBootServicesCallback,
    &Registration
  );
  DEBUG ((DEBUG_INFO, "[GPIOV2][SMM]: InstallGpioV2SmmProtocol End\n"));
  return Status;
}