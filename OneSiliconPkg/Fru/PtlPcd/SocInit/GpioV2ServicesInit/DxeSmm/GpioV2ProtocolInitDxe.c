/** @file
  This file provides initialization code for Gpio V2 Controller PEI module

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

#include <PiDxe.h>
#include <Library/DxeServicesLib.h>
#include "GpioV2ProtocolInit.h"

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  The GPIOV2 DXE driver entry point - Intalls GPIOV2 Protocol.

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
  @param[in] SystemTable          EFI system table.

  @retval EFI_INVALID_PARAMETER   Invalid parameter
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
  @retval EFI_SUCCESS             Protocol interface successfully installed
**/
EFI_STATUS
EFIAPI
GpioV2ProtocolDxeInitEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_HANDLE          Handle;
  GPIOV2_INTERFACE    *GpioInterface;
  GPIOV2_PWM          *Pwm;

  DEBUG ((DEBUG_INFO, "[GPIOV2][DXE]: InstallGpioV2Protocol Start\n"));

  GpioInterface = (GPIOV2_INTERFACE *) AllocateZeroPool (sizeof (GPIOV2_INTERFACE));
  if (NULL == GpioInterface) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: Allocating memory for GpioInterface failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Pwm = (GPIOV2_PWM *) AllocateZeroPool (sizeof (GPIOV2_PWM));
  if (NULL == Pwm) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: Allocating memory for Pwm failed\n"));
  }
  GpioInterface->Pwm = Pwm;

  Status = InternalGpioInterfaceConstructor (GpioInterface);
  if (EFI_ERROR (Status)) {
    FreePool (GpioInterface);
    if (Pwm != NULL) {
      FreePool (Pwm);
    }
    return Status;
  }

  //
  // Install on a new handle
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gGpioV2ProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &(GpioInterface->Public)
                  );
  if (EFI_ERROR (Status)) {
    FreePool (GpioInterface);
    if (Pwm != NULL) {
      FreePool (Pwm);
    }
    return Status;
  }

  if (NULL != Pwm) {
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gGpioV2PwmProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    GpioInterface->Pwm
                    );
    if (EFI_ERROR (Status)) {
      FreePool (Pwm);
    }
  }

  DEBUG ((DEBUG_INFO, "[GPIOV2][DXE]: InstallGpioV2Protocol End\n"));

  return Status;
}

