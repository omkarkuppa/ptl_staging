/** @file
  Header file for USB4 host router initialization routines in PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _USB4_PLATFORM_DRIVER_H_
#define _USB4_PLATFORM_DRIVER_H_

#include <Library/Usb4PlatformInfo.h>

//
// USB4 PEI reserved DMA buffer for each host router
//
#define MAX_USB4_RING_SUPPORT       16
#define USB4_PEI_DMA_BUFFER_PAGES   (1 * MAX_USB4_RING_SUPPORT)

//
// Firmware CM delay in us for PCIe tunneling (= 600ms)
//
#define USB4_FW_CM_PCIE_DELAY_IN_US                 600000

/**
  Query the USB4 host router information in the platform.

  @param[in,out] Usb4PlatformInfo - Pointer to USB4 host routers information in the platform.

  @retval EFI_SUCCESS           - Query the USB4 host router information successfully.
  @retval EFI_NOT_FOUND         - No USB4 host router found in platform.
  @retval EFI_UNSUPPORTED       - Fail to query the USB4 host router information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4PlatformInfoQuery (
  OUT USB4_PLATFORM_INFO    *Usb4PlatformInfo
  );

/**
  USB4 platform initialization for SW CM execution.
  - Reserved PEI DMA buffer for SW CM Tx/Rx ring if IOMMU is enabled.

  @param[in] Usb4PlatformInfo - Pointer to USB4 host routers information in the platform.

  @retval EFI_SUCCESS           - Initialize all USB4 host routers successfully.
  @retval EFI_UNSUPPORTED       - Unable to initialize all USB4 host routers to the preferred CM mode.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4PlatformInit (
  IN USB4_PLATFORM_INFO    *Usb4PlatformInfo
  );

#endif
