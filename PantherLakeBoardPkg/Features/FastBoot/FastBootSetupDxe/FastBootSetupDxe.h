/** @file
  Header file of Fast Boot platform setup DXE driver.

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

@par Specification
**/

#ifndef _FAST_BOOT_INFO_DRIVER_H_
#define _FAST_BOOT_INFO_DRIVER_H_

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <PlatformNvRamHookLib.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/FastBootException.h>
#include <FastBootExceptionInfoHob.h>
#include <FastBootFunctionEnabledHob.h>
#include <Guid/HobList.h>
#include <Guid/EventGroup.h>
#include <Register/TcoRegs.h>
#include <Setup.h>
#include <SetupVariable.h>

#define FAST_BOOT_WDT_RESET_SIGNATURE   0xFB
#define FAST_BOOT_WDT_30_SECONDS_COUNT  ((30 * 10 - 24) / 6)

#endif
