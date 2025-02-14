/** @file
  Platform Watchdog timer library to support BIOS Resiliency

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __PLATFORM_WDT_LIB_H__
#define __PLATFORM_WDT_LIB_H__

#define MAX_EC_WDT_TIMEOUT  0xFF

/**
  Disarm platform watchdog timer

  @retval EFI_SUCCESS  The operation succeed.
  @retval Other        The operation failed.

**/
EFI_STATUS
EFIAPI
DisarmPlatformWdt (
  VOID
  );

/**
  Arm platform watchdog timer with specific timeout

  @param[in] timeout value, 1 stands for 1 second, max is 255 seconds with input 0xFF

  @retval     EFI_SUCCESS       The operation succeed.
  @retval     Other             The operation failed.

**/
EFI_STATUS
EFIAPI
ArmPlatformWdt (
  IN UINT8          TimeOut
  );

#endif
