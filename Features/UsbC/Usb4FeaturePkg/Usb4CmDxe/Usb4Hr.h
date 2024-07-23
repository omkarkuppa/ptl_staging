/** @file
  Header file for USB4 host router routines used before PCI bus driver enumeration.

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

#ifndef _USB4_HR_H_
#define _USB4_HR_H_

#include <Library/Usb4Common.h>
#include <Library/Usb4PlatformInfo.h>

//
// Allocate 1 MB MMIO size for USB4 Tx/Rx Ring programming interface.
// MMIO alignment 20 means 2^20 = 1MB
//
#define USB4_RING_MMIO_LENGTH        (1024 * 1024)
#define MMIO_ALIGNMENT_1MB           20

/**
  Create and initialize host router instance.
  - Allocate and assign MMIO space for Tx/Rx ring access
  - Initialize PCI config space.

  @param[in]  ImageHandle  - Usb4CmDxe image handle.
  @param[in]  HrInfo       - Host router information.
  @param[out] Usb4HrInst   - Pointer of pointer to the created USB4 host router instance.

  @retval EFI_SUCCESS           - Create host router instance successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_NOT_FOUND         - Device is not present.
  @retval EFI_OUT_OF_RESOURCES  - Insufficient to initialize host router instance.
**/
EFI_STATUS
Usb4HrCreate (
  IN  EFI_HANDLE         ImageHandle,
  IN  PUSB4_HR_INFO      HrInfo,
  OUT USB4_HR_INSTANCE   **Usb4HrInst
  );

/**
  Stop USB4 host router.
  - Un-initialize PCI config space.
  - Free the allocated MMIO space.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.
**/
VOID
Usb4HrStop (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  );

/**
  Destroy USB4 host router instance.
  - Un-initialize PCI config space.
  - Free the allocated MMIO space.
  - Free the host router instance.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.
**/
VOID
Usb4HrDestroy (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  );

/**
  Build USB4 domain topology of host router.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Build USB4 topology successfully.
  @retval EFI_UNSUPPORTED       - Fail to build USB4 topology.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HrBuildTopology (
  IN  USB4_HR_INSTANCE    *Usb4Hr
  );

/**
  Unpower USB4 host router

  @param[in] Usb4Hr - Pointer to USB4 host router instance.
**/
VOID
Usb4HrUnPower (
  IN USB4_HR_INSTANCE    *Usb4Hr
  );
#endif
