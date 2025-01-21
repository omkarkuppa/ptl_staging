/** @file
  Common definitions used to access registers via SoC specific way.

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

#ifndef __SPECIAL_REGISTER_ACCESS_H__
#define __SPECIAL_REGISTER_ACCESS_H__

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLibClient.h>
#include <Library/SystemTopologyLib.h>
#include <Library/RegisterAddressMapLib.h>
#include <Library/SysHostPointerLibClient.h>
#include <RegisterAccess.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/CpuSbiAccessLib.h>

typedef enum {
  TYPE_SCF_BAR = 0,
  TYPE_PCU_BAR,
  TYPE_MEM_BAR0,
  TYPE_MEM_BAR1,
  TYPE_MEM_BAR2,
  TYPE_MEM_BAR3,
  TYPE_MEM_BAR4,
  TYPE_MEM_BAR5,
  TYPE_MEM_BAR6,
  TYPE_MEM_BAR7,
  TYPE_SBREG_BAR,
  TYPE_MAX_MMIO_BAR
} MMIO_BARS;


#endif // __SPECIAL_REGISTER_ACCESS_H__
