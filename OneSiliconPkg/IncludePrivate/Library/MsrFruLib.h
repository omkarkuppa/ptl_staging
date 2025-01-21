/** @file
  This file contains MSR access for specific generation.

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
#ifndef _MSR_FRU_LIB_H_
#define _MSR_FRU_LIB_H_

#include <Uefi/UefiBaseType.h>

/**
  This function set GrashLog GPRs (General Purpose Registers) Mask
  Which allows GPRs to not be included in the Crash Log CPU register data.

  @param[in] BOOLEAN     Policy for CrashLog Enable
  @param[in] UINT8       Policy for CrashLog GPRs dump
**/
VOID
EFIAPI
MsrSetCrashLogGprsMask (
  IN  BOOLEAN                 CrashLogEnable,
  IN  UINT8                   CrashLogGprsEnable
  );

/**
  This function set processor trace output.

  @param[in] OutputSchemeTopa     Is Topa or not
  @param[in] MemRegionBaseAddr    Base address of memory
  @param[in] TopaTableBaseAddr    Base address of ToPA table
  @param[in] MemSize              Memory size, 4K align.

  @retval  set MSR or not
**/
BOOLEAN
MsrSetProcTraceOutput (
  IN BOOLEAN OutputSchemeTopa,
  IN UINTN   MemRegionBaseAddr,
  IN UINTN   TopaTableBaseAddr,
  IN UINT32  MemSize
);

/**
  This function set PMON Address.

  @param[in] PmonAddress     Memory address for PMON
**/
VOID
MsrSetPmonAddress (
  IN  UINT64                 PmonAddress
  );

/**
  ProgramRpRecipe features basing on desired settings

  @param[in, out] IsBsp - Pointer to TRUE indicates the function is called from BSP.
**/
VOID
EFIAPI
ProgramRpRecipe (
  IN OUT VOID  *IsBsp
  );
#endif // _MSR_FRU_LIB_H_
