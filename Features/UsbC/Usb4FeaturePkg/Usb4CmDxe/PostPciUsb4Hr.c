/** @file
  USB4 host router routines used after PCI bus driver enumeration.

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
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4HrSrvLib.h>
#include <Library/Usb4HiCoreLib.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4PlatformInfo.h>
#include <Library/Usb4HrInst.h>
#include "Usb4IhrPci.h"
#include "Usb4DhrPci.h"

/**
  Destroy USB4 host interface and host router instance.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.
**/
VOID
PostPciUsb4HrDestroy (
  IN USB4_HR_INSTANCE    *Usb4Hr
  )
{
  DEBUG ((DEBUG_INFO, "PostPciUsb4HrDestroy entry\n"));
  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "PostPciUsb4HrDestroy: Null Usb4Hr\n"));
    goto Exit;
  }

  Usb4HiCoreDestroy (&(Usb4Hr->HiCore));
  CmFreeMem (Usb4Hr);
Exit:
  DEBUG ((DEBUG_INFO, "PostPciUsb4HrDestroy exit\n"));
}

/**
  Start USB4 host router.
  - Query USB4 host router PCI resources assigned by PCI bus driver.
  - Initialize USB4 host interface.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Start USB4 host router successfully.
  @retval EFI_UNSUPPORTED       - Fail to start USB4 host router.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
PostPciUsb4HrStart (
  IN USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS       Status;
  PCI_DEV_BDF      *NhiBdf;

  DEBUG ((DEBUG_INFO, "PostPciUsb4HrStart entry\n"));

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "PostPciUsb4HrStart: Null Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Query PCI information of USB4 host router after PCI enumeration
  //
  if (Usb4Hr->HrInfo.IntegratedHr) {
    Status = Iusb4HrPciQuery (Usb4Hr);
  } else {
    Status = Dusb4HrPciQuery (Usb4Hr);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PostPciUsb4HrStart: Query USB4 HR PCI information failure, %r\n", Status));
    goto Exit;
  }

  NhiBdf = &(Usb4Hr->HrInfo.NhiBdf);
  DEBUG ((DEBUG_INFO, "USB4 host BDF (%d/%d/%d)\n", NhiBdf->Bus, NhiBdf->Dev, NhiBdf->Func));
  DEBUG ((DEBUG_INFO, "Dump BAR0 MMIO (0x%016llx) content:\n", Usb4Hr->MmioBase));
  HrSrvMmioDump (Usb4Hr->MmioBase, 12 *4);

  //
  // Initialize USB4 host interface.
  // Note: Call Usb4HiCoreCreate instead in driver model driver.
  //
  Status = Usb4HiCoreStart (&(Usb4Hr->HiCore));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PostPciUsb4HrStart: Fail to initialize USB4 host interface.\n"));
    goto Exit;
  }
Exit:

  DEBUG ((DEBUG_INFO, "PostPciUsb4HrStart exit\n"));
  return Status;
}
