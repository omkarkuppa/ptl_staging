/** @file
  USB4 Connection Manager PEI driver header.

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

#ifndef _USB4_PLATFORM_PEI_H_
#define _USB4_PLATFORM_PEI_H_

#include <PiPei.h>

/**
  The callback function for EndOfPeiPpi.
  - Query USB4 platform information
  - Allocate IOMMU DMA buffer for use in Dxe
  - Create USB4 platform hob to pass USB4 platform information to Dxe

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval EFI_UNSUPPORTED      - The function failed.
**/
EFI_STATUS
EFIAPI
Usb4PlatformEndOfPeiPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

#endif
