/** @file
  This header file provides platform specific definitions used
  by other modules for platform specific initialization.
  This is not suitable for consumption by ASL or VRF files.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "CommonDefinitions.h"

//
// Need minimum of 48MB during PEI phase for IAG and some buffer for boot.
//
#define  PEI_MIN_MEMORY_SIZE               (10 * 0x800000)   // 80MB
#define  PEI_RECOVERY_MIN_MEMORY_SIZE      (10 * 0x800000)   // 80MB

#define FLASH_BLOCK_SIZE  0x10000

#define CPU_EXTERNAL_CLOCK_FREQ  0x64
#define CPU_FREQUENCY_MODE_100  0x64
#define FREQUENCY_RESOLUTION_3182  0xc6e
#define NDIVIDER_BASE_VALUE  0x19d
#define MDIVIDER_VALUE_13  0xd

#endif
