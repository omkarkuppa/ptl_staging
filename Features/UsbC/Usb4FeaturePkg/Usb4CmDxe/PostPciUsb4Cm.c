/** @file
  USB4 Connection Manager routines used after PCI bus driver enumeration.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4PlatformHob.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4DomainLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/CmEvtLib.h>
#include "PostPciUsb4Hr.h"
#include "Usb4CmDxe.h"

/**
  Start connection manager on the USB4 host router.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Start connection manager successfully.
  @retval EFI_UNSUPPORTED       - Fail to start connection manager.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
PostPciCmStart (
  IN USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS   Status;

  DEBUG ((DEBUG_INFO, "PostPciCmStart entry - Usb4Hr = %p\n", Usb4Hr));

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "PostPciCmStart: Null Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Start USB4 host router to enable USB4 host interface.
  //
  Status = PostPciUsb4HrStart (Usb4Hr);

  //
  // Bind USB4 host router to the USB4 domain.
  // Domain pointer should come from pre-installed protocol for driver model driver case.
  //
  if (Usb4Hr->Usb4Domain != NULL) {
    Status = Usb4DomainBindHr (Usb4Hr, Usb4Hr->Usb4Domain);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PostPciCmStart: Fail to bind USB4 domain to USB4 host router, %r\n", Status));
      goto Exit;
    }
  }

  DEBUG ((DEBUG_INFO, "PostPciCmStart exit\n"));

Exit:
  return Status;
}

/**
  CM monitor handler that process all events received in Rx Ring.

  @param  Event      - Timer event.
  @param  Context    - Pointer to USB4 host router context
  .
**/
VOID
EFIAPI
Usb4CmMonitor (
  IN EFI_EVENT            Event,
  IN VOID                 *Context
  )
{
  USB4_HR_INSTANCE    *Usb4Hr;
  EFI_TPL             OldTpl;

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  Usb4Hr = (USB4_HR_INSTANCE *) Context;
  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CmMonitor: Null Usb4Hr\n"));
    goto Exit;
  }

  if (Usb4Hr->Signature != USB4_HR_INSTANCE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4CmMonitor: Invalid USB4 HR signature 0x%0x\n", Usb4Hr->Signature));
    goto Exit;
  }
  //
  // Process Rx events
  //
  CmProcessRxEvents (Usb4Hr);

Exit:
  gBS->RestoreTPL (OldTpl);
  return;
}

/**
  Connection Manager destroy.

  @param[in] CmInst - Pointer to CM instance.
**/
VOID
PostPciCmDestroy (
  IN USB4_HR_INSTANCE    *Usb4Hr
  )
{
  DEBUG ((DEBUG_INFO, "PostPciCmDestroy entry - Usb4Hr = %p\n", Usb4Hr));

  if (Usb4Hr != NULL) {

    if (Usb4Hr->Usb4Domain != NULL) {
      //
      // Remove and destroy all Routers from Domain.
      //
      Usb4DomainDestroy (Usb4Hr->Usb4Domain);
      Usb4Hr->Usb4Domain = NULL;
    }

    PostPciUsb4HrDestroy (Usb4Hr);
  }

  DEBUG ((DEBUG_INFO, "PostPciCmDestroy exit\n"));
}