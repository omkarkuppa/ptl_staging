/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef __FAST_BOOT_DATA_DEF_H__
#define __FAST_BOOT_DATA_DEF_H__

#define FAST_BOOT_WDT_RESET_SIGNATURE      0xFB

typedef enum {
  NoException = 0,
  ExceptionType1,
  ExceptionType2,
  ExceptionType3A,
  ExceptionType3B,
  ExceptionTypeMax
} FAST_BOOT_EXCEPTION_TYPE;

typedef enum {
  NoExceptionCategory = 0,
  FastBootDisabled,
  FastBootOverridden,
  FastBootBreakByUser,
  FirstBoot,
  BootFailure,
  FirmwareUpdate,
  HardwareChanged,
  SetupConfigurationChanged,
  ConsoleDeviceChanged,
  BootDeviceChanged,
  ContentLost,
  PowerFailure,
  HardwareError,
  SpecialBoot,
  AnyOfOtherCategories,
  ExceptionCategoryMax
} FAST_BOOT_EXCEPTION_CATEGORY;

#endif
