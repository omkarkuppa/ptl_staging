/** @file
  Platform configuration change variable definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#ifndef __PLATFORM_CONFIGURATION_CHANGE_VARIABLE_H__
#define __PLATFORM_CONFIGURATION_CHANGE_VARIABLE_H__

#define PLATFORM_CONFIGURATION_CHANGE_VARIABLE_GUID \
  { \
    0xe3cacf62, 0x3062, 0x4e1d, { 0x97, 0x8e, 0x46, 0x80, 0x7a, 0xb9, 0x74, 0x7d }\
  }

#define PLATFORM_CONFIGURATION_CHANGE  L"PlatformConfigurationChange"

#define NO_CONFIG_CHANGE     0x00000000
#define FIRST_BOOT           0x00000001
#define SETUP_OPTION_CHANGE  0x00000002

typedef struct {
  UINT32   ConfigChangeType;  /// Significant change from previous boot
} CONFIGURATION_CHANGE;

#endif

