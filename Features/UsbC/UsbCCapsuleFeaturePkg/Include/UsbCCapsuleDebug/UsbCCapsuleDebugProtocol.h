/** @file
  Header file for UsbC Capsule Capability Protocol implementation

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

#ifndef _EFI_USBC_CAPSULE_DEBUG_PROTOCOL_H_
#define _EFI_USBC_CAPSULE_DEBUG_PROTOCOL_H_

#include <Usb4Debug/Usb4DebugProtocol.h>
///
/// Global ID for the EFI_USBC_CAPSULE_DEBUG_PROTOCOL
///
#define EFI_USBC_CAPSULE_DEBUG_PROTOCOL_GUID \
  { \
    0x70CE41F4, 0x51AC, 0x5A2B, { 0x12, 0xAD, 0x1B, 0xC9, 0x87, 0x29, 0xEE, 0x7C } \
  }

//
// Log level definitions of Log data
//
#define USBC_CAPSULE_DBG_DISABLED   0
#define USBC_CAPSULE_DBG_ERROR      1
#define USBC_CAPSULE_DBG_VERBOSE    2

extern EFI_GUID  gEfiUsbCCapsuleDebugProtocolGuid;
#endif
