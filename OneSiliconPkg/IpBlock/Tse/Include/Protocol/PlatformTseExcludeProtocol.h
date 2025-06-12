/** @file
  Definitions for PlatformTseExcludeProtocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _PLATFORM_TSE_EXCLUDE_PROTOCOL_H
#define _PLATFORM_TSE_EXCLUDE_PROTOCOL_H

#include <Uefi.h>
#include <Protocol/PciIo.h>

typedef struct _PLATFORM_TSE_EXCLUDE_PROTOCOL   PLATFORM_TSE_EXCLUDE_PROTOCOL;

/**
  Any NVMe device subjected to runtime BDF change is not supported by TSE design currently.
  The function is for checking if the device is under a hotplug able daisy chain, such as TBT
  daisy chain typically.

  @param[in]   This               The Platform TSE Exclude Protocol Instance.
  @param[in]   PciIo              Target Device PciIo Protocol
  @param[out]  IsTseSupported     A pointer to a Boolean determining TSE is supported or not

  @retval  EFI_SUCCESS            Successfully check whether device is under TBT daisy chain.
  @retval  EFI_NOT_FOUND          Failed to get DTbtInfoHob.
  @retval  EFI_INVALID_PARAMETER  Invalid parameter passed in.

**/
typedef
EFI_STATUS
(EFIAPI *PLATFORM_TSE_EXCLUDE_CHECK)(
  IN   PLATFORM_TSE_EXCLUDE_PROTOCOL  *This,
  IN   EFI_PCI_IO_PROTOCOL            *PciIo,
  OUT  BOOLEAN                        *IsTseSupported
  );

//
// Platform TSE Exclude structure.
//
struct _PLATFORM_TSE_EXCLUDE_PROTOCOL  {
  PLATFORM_TSE_EXCLUDE_CHECK  TseExcludeCheck;
};

//
// Platform TSE Exclude GUID variable.
//
extern EFI_GUID  gPlatformTseExcludeProtocolGuid;

#endif
