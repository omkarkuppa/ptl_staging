/** @file
  Platform Sanitize storage service library function

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

@par Specification Reference:
**/
#ifndef _PS_STORAGE_SERVICE_H_
#define _PS_STORAGE_SERVICE_H_

#include <Protocol/StorageSanitizeNotifyProtocol.h>

/**
  Attempts to erase ATA and NVMe drives.
  This function should be called as a BDS Phase Event, once all the devices/protocols are available.

  @param[in]   EraseConfiguration  Configuration for erase operation.
  @param[in]   HostPassword        Host password for storage unlock.
  @param[in]   CompleteFunction    Callback function when erase completed.

  @retval      EFI_SUCCESS         Erase device succeed.
  @retval      EFI_UNSUPPORTED     The device is not supported.
  @retval      EFI_ACCESS_DENIED   User has entered wrong password too many times.
  @retval      EFI_ABORTED         The device is supported, but the system
                                   has failed to erase it.
**/
EFI_STATUS
PerformSsdErase (
  IN ERASE_CONFIGURATION    EraseConfiguration,
  IN CHAR8                  *HostPassword,
  IN STORAGE_ERASE_COMPLETE CompleteFunction
  );

#endif   // _PS_STORAGE_SERVICE_
