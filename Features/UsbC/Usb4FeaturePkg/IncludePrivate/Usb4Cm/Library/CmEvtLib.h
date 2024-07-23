/** @file
  USB4 Connection Manager Event Library Header.

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

#ifndef CM_EVT_LIB_H_
#define CM_EVT_LIB_H_

#include <Uefi.h>

/**
  Process Rx events if any pending Rx events are available in the Rx event queue.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Process Rx events successfully.
  @retval EFI_UNSUPPORTED       - Unexpected process Rx events failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
CmProcessRxEvents (
  IN USB4_HR_INSTANCE    *Usb4Hr
  );

#endif
