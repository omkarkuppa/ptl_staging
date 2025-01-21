/** @file
  This PPI provides services to register a platform specific handler for
  ResetSystem().  The registered handlers are processed after
  EDKII_PLATFORM_SPECIFIC_RESET_NOTIFICATION_PPI notifications.

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

#ifndef _PLATFORM_SPECIFIC_RESET_HANDLER_PPI_H_
#define _PLATFORM_SPECIFIC_RESET_HANDLER_PPI_H_

#include <Protocol/ResetNotification.h>

#define EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PPI_GUID \
  { 0x75cf14ae, 0x3441, 0x49dc, { 0xaa, 0x10, 0xbb, 0x35, 0xa7, 0xba, 0x8b, 0xab } }

typedef EFI_RESET_NOTIFICATION_PROTOCOL  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PPI;

extern EFI_GUID gEdkiiPlatformSpecificResetHandlerPpiGuid;

#endif
