/** @file
  Header file for integrated USB4 host router PCI routines.

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

#ifndef _USB4_IHR_PCI_H_
#define _USB4_IHR_PCI_H_

#include <Library/Usb4HrInst.h>

//
// USB4 host router force power timeout value in ms
//
#define HR_FORCE_POWER_TIMEOUT_MS           500

//
// USB4 host router force power polling period in us
//
#define HR_FORCE_POWER_POLLING_US           1000

#define HR_FORCE_POWER_POLLING_MAX          ((HR_FORCE_POWER_TIMEOUT_MS * 1000)/HR_FORCE_POWER_POLLING_US)

//
// The following defitions are used for force power
// Defined in Intel\OneSiliconPkg\Fru\TglCpu\IncludePrivate\Register\HostDmaRegs.h
//
// ======================================================================================
#define R_HOST_DMA_RTD3_FORCEPOWER                0XFC
#define B_HOST_DMA_FORCEPOWER_EN                  BIT1
#define N_HOST_DMA_DMA_ACTIVE_DELAY_OFFSET        24
#define B_HOST_DMA_DMA_ACTIVE_DELAY_MASK          0xFF000000
#define V_HOST_DMA_DMA_ACTIVE_DELAY_VALUE         0x22
#define R_HOST_DMA_ITBT_NVM_FW_REVISION           0XC8
// ======================================================================================

/**
  Initialize PCI ocnfiguration space of integrated USB4 host router.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS            - PCI initialization success.
  @retval EFI_INVALID_PARAMETER  - Invalid parameter.
  @retval EFI_UNSUPPORTED        - Can't initialize PCI configuratoin space of USB4 host router.
**/
EFI_STATUS
Iusb4HrPciInit (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  );

/**
  Un-Initialize PCI ocnfiguration space of integrated USB4 host router.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.
**/
VOID
Iusb4HrPciUnInit (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  );

/**
  Force power to USB4 host router.

  @param[in] Usb4Hr      - Pointer to USB4 host router instance.
  @param[in] SetForcePwr - TRUE = Set force power, FALSE = Un-power.

  @retval EFI_SUCCESS           - Force power success.
  @retval EFI_UNSUPPORTED       - Device is not ready for force power.
  @retval EFI_TIMEOUT           - Device is not ready after timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Iusb4HrForcePwr (
  IN USB4_HR_INSTANCE    *Usb4Hr,
  IN BOOLEAN             SetForcePwr
  );

/**
  Query PCI information of integrated USB4 host router.
  This function should be called after PCI enumeration.

  @param[in, out] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Query PCI information successfully.
  @retval EFI_UNSUPPORTED       - Fail to query PCI information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Iusb4HrPciQuery (
  IN OUT USB4_HR_INSTANCE    *Usb4Hr
  );
#endif
