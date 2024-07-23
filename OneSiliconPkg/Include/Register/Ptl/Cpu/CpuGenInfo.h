/** @file
  Header file for Cpu Information

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _CPU_GEN_INFO_PTL_CDIE_H_
#define _CPU_GEN_INFO_PTL_CDIE_H_

#include <CpuRegs.h>

//
// Processor Definitions
//
#define CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE 0x000C06C0

//
// Processor Definitions
//
#define CPUID_FULL_STEPPING                        0x0000000F
#define CPUID_FULL_FAMILY_MODEL                    0x0FFF0FF0
#define CPUID_FULL_FAMILY_MODEL_STEPPING           0x0FFF0FFF

typedef UINT32       CPU_FAMILY;

///
/// Enums for CPU SKU IDs
///
typedef enum {
  EnumCpuUlt        = 0,
  EnumCpuTrad,
  EnumCpuUlx,
  EnumCpuHalo,
  EnumCpuUnknown
} CPU_SKU;

///
/// Enums for CPU Stepping IDs
///
typedef enum {
  ///
  /// PantherLake-U/H Mobile Steppings
  ///
  EnumPtlHA0  = 0,
  EnumPtlUA0  = 1,
  EnumPtlUHB0 = 2,
  ///
  /// Max Stepping
  ///
  EnumCpuSteppingMax  = CPUID_FULL_STEPPING
} CPU_STEPPING;
#endif // _CPU_GEN_INFO_PTL_CDIE_H_
