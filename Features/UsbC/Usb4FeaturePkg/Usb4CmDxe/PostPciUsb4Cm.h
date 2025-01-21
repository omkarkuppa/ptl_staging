/** @file
  Header file for USB4 Connection Manager routines used after PCI bus driver enumeration.

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

#ifndef _POST_PCI_USB4_CM_H_
#define _POST_PCI_USB4_CM_H_

#include <Uefi.h>
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
  );

/**
  CM monitor handler that process all events received in Rx Ring.

  @param  Event      - Timer event.
  @param  Context    - Pointer to USB4 CM Dxe extension.
**/
VOID
EFIAPI
Usb4CmMonitor (
  IN EFI_EVENT            Event,
  IN VOID                 *Context
  );

/**
  Connection Manager destroy.

  @param[in] CmInst - Pointer to CM instance.
**/
VOID
PostPciCmDestroy (
  IN USB4_HR_INSTANCE    *Usb4Hr
  );
#endif
