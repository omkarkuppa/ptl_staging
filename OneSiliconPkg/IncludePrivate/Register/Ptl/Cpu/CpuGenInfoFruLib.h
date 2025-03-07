 /** @file
  This file contains Cpu Information for specific generation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _CPU_GEN_INFO_FRU_LIB_PTL_CDIE_H_
#define _CPU_GEN_INFO_FRU_LIB_PTL_CDIE_H_
#include <CpuRegs.h>
#include <Register/CpuGenInfoRegs.h>
#include <Register/GenerationMsr.h>

///
/// Used to identify the CPU used for programming with the VR override table
///
typedef enum {
  EnumUnknownCpuId            = 0,
  ///
  /// PTL U, H 12Xe/4Xe
  ///
  EnumPtlU15Watt40CpuId           = 0x10,
  EnumPtlU15Watt20CpuId           = 0x11,
  EnumPtlH12Xe25Watt48CpuId       = 0x12,
  EnumPtlH12Xe25Watt44CpuId       = 0x13,
  EnumPtlH12Xe25Watt28CpuId       = 0x14,
  EnumPtlH12Xe25Watt24CpuId       = 0x15,
  EnumPtlH4Xe25Watt48CpuId        = 0x16,
  EnumPtlH4Xe25Watt44CpuId        = 0x17,
  EnumPtlH4Xe25Watt28CpuId        = 0x18,
  EnumPtlH4Xe25Watt24CpuId        = 0x19,
  EnumPtlH4Xe65Watt48Pd2CpuId     = 0x1A,
  EnumPtlH12Xe65Watt48Pd2CpuId    = 0x1B,
  EnumPtlH4Xe65Watt48Pd3CpuId     = 0x1C,
  EnumPtlH12Xe65Watt48Pd3CpuId    = 0x1D,
  ///
  /// WCL U
  ///
  EnumWclU15Watt10CpuId           = 0x30,
  EnumWclU15Watt20CpuId           = 0x31,
} CPU_IDENTIFIER;

/**
  Return CPU Identifier used to identify various CPU types

  @param[in]  CPU_SKU                       CpuSku
  @param[in]  CPU_STEPPING                  CpuStepping

  @retval     CPU_IDENTIFIER                CPU Identifier
**/
CPU_IDENTIFIER
EFIAPI
GetCpuSkuIdentifier (
  IN  CPU_SKU         CpuSku,
  IN  CPU_STEPPING    CpuStepping
  );
#endif // _CPU_GEN_INFO_FRU_LIB_PTL_CDIE_H_
