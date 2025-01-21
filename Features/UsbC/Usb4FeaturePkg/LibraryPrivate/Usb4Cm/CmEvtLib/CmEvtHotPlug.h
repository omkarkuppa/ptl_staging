/** @file
  Header file for USB4 Hot plug event handler.

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

#ifndef CM_EVT_HOTPLUG_H_
#define CM_EVT_HOTPLUG_H_

#include <Library/Usb4HiCoreLib.h>

#define MAX_PROCESSED_RX_EVENTS    256

/**
  Hot plug event handler

  @param[in] Usb4Hr   - Pointer to USB4 host router instance.
  @param[in] RxEvent  - Pointer to Rx event.

  @retval EFI_SUCCESS           - Hot plug process success.
  @retval EFI_UNSUPPORTED       - Hot plug process failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
CmHotPlugHandler (
  IN USB4_HR_INSTANCE    *Usb4Hr,
  IN PRX_EVENT           RxEvent
  );

#endif
