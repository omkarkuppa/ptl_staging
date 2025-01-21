/** @file
  This protocol provides services to register a platform specific handler for
  ResetSystem().  The registered handlers are called after the UEFI 2.7 Reset
  Notifications are processed

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL_H_
#define _PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL_H_

#include <Protocol/ResetNotification.h>

#define EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL_GUID \
  { 0x2df6ba0b, 0x7092, 0x440d, { 0xbd, 0x4, 0xfb, 0x9, 0x1e, 0xc3, 0xf3, 0xc1 } }

typedef EFI_RESET_NOTIFICATION_PROTOCOL  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL;

extern EFI_GUID gEdkiiPlatformSpecificResetHandlerProtocolGuid;

#endif
