/** @file
  This PPI provides services to register a platform specific notification callback for
  ResetSystem().  The registered handlers are processed after
  EDKII_PLATFORM_SPECIFIC_RESET_FILTER_PPI notifications and before
  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PPI notifications.

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

#ifndef _PLATFORM_SPECIFIC_RESET_NOTIFICATION_PPI_H_
#define _PLATFORM_SPECIFIC_RESET_NOTIFICATION_PPI_H_

#include <Protocol/ResetNotification.h>

#define EDKII_PLATFORM_SPECIFIC_RESET_NOTIFICATION_PPI_GUID \
  { 0xe09f355d, 0xdae8, 0x4910, { 0xb1, 0x4a, 0x92, 0x78, 0x0f, 0xdc, 0xf7, 0xcb } }

typedef EFI_RESET_NOTIFICATION_PROTOCOL  EDKII_PLATFORM_SPECIFIC_RESET_NOTIFICATION_PPI;

extern EFI_GUID gEdkiiPlatformSpecificResetNotificationPpiGuid;

#endif
