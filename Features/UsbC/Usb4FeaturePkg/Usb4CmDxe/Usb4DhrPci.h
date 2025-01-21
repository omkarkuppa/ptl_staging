/** @file
  Header file for discrete USB4 host router PCI routines.

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

#ifndef _USB4_DHR_PCI_H_
#define _USB4_DHR_PCI_H_

#include <Library/Usb4HrInst.h>

/**
  Initialize PCI configuration space of discrete USB4 host router.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS            - PCI initialization success.
  @retval EFI_INVALID_PARAMETER  - Invalid parameter.
  @retval EFI_NOT_FOUND          - Device is not present.
  @retval EFI_UNSUPPORTED        - Can't initialize PCI configuration space of USB4 host router.
**/
EFI_STATUS
Dusb4HrPciInit (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  );

/**
  Un-Initialize PCI configuration space of discrete USB4 host router.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.
**/
VOID
Dusb4HrPciUnInit (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  );

/**
  Query PCI information of discrete USB4 host router.
  This function should be called after PCI enumeration.

  @param[in, out] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Query PCI information successfully.
  @retval EFI_NOT_FOUND         - Discrete USB4 host router is not found.
  @retval EFI_UNSUPPORTED       - Fail to query PCI information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Dusb4HrPciQuery (
  IN OUT USB4_HR_INSTANCE    *Usb4Hr
  );
#endif
