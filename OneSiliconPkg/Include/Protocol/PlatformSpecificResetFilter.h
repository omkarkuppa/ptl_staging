/** @file
  This Protocol provides services to register a platform specific reset filter
  for ResetSystem().  A reset filter evaluates the parameters passed to
  ResetSystem() and converts a ResetType of EfiResetPlatformSpecific to a
  non-platform specific reset type.  The registered filters are processed before
  the UEFI 2.7 Reset Notifications.

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

#ifndef _PLATFORM_SPECIFIC_RESET_FILTER_PROTOCOL_H_
#define _PLATFORM_SPECIFIC_RESET_FILTER_PROTOCOL_H_

#include <Protocol/ResetNotification.h>

#define EDKII_PLATFORM_SPECIFIC_RESET_FILTER_PROTOCOL_GUID \
  { 0x695d7835, 0x8d47, 0x4c11, { 0xab, 0x22, 0xfa, 0x8a, 0xcc, 0xe7, 0xae, 0x7a } }

typedef EFI_RESET_NOTIFICATION_PROTOCOL  EDKII_PLATFORM_SPECIFIC_RESET_FILTER_PROTOCOL;

extern EFI_GUID gEdkiiPlatformSpecificResetFilterProtocolGuid;

#endif
