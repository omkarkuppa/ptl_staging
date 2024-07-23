/** @file
  Header file for USB4 host router control routines used after PCI bus driver enumeration.

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

#ifndef _POST_PCI_USB4_HR_H_
#define _POST_PCI_USB4_HR_H_

#include <Library/Usb4Common.h>
#include <Library/Usb4PlatformInfo.h>
#include <Library/Usb4HrInst.h>

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
  );

/**
  Destroy USB4 host interface and host router instance.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.
**/
VOID
PostPciUsb4HrDestroy (
  IN USB4_HR_INSTANCE    *Usb4Hr
  );
#endif
