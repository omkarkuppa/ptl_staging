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

@par Specification
**/

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MsrFruLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/GenerationMsr.h>
#include <CpuRegs.h>
#include <Library/CpuCommonLib.h>
#include <Register/Cpuid.h>

//
// Resource Priority Feature Related
//
#define MSR_WORK_CONSERVING_CLOS               0x00001878
#define MSR_IA32_L2_MASK_8                     0x00000D18
#define MSR_IA32_L2_MASK_9                     0x00000D19
#define MSR_IA32_L2_MASK_10                    0x00000D1A
#define MSR_IA32_L2_MASK_11                    0x00000D1B
#define MSR_IA32_L2_MASK_12                    0x00000D1C
#define MSR_IA32_L2_MASK_13                    0x00000D1D
#define MSR_IA32_L2_MASK_14                    0x00000D1E
#define MSR_IA32_L2_MASK_15                    0x00000D1F
#define MSR_IA32_QOS_CORE_BW_THRTL_0           0x00000e00
#define MSR_IA32_QOS_CORE_BW_THRTL_1           0x00000e01

//
// Define the MSR manually, remove it after the offical RDL add this MSR
//
#define MSR_PMON_BANK1_MISC_MSR_0        (0x0000201e)

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
  )
{
  MSR_CRASHLOG_CONTROL_REGISTER    CurrentValue;
  MSR_CRASHLOG_CONTROL_REGISTER    ExpectedValue;

  CurrentValue.Uint64 = AsmReadMsr64 (MSR_CRASHLOG_CONTROL);
  ExpectedValue.Uint64 = CurrentValue.Uint64;

  //
  // Disable Gprs dump when CrashLog is disabled
  //
  if (CrashLogEnable) {
    switch (CrashLogGprsEnable) {
      //
      // 0: GPRs dump masked
      // 1: GPRs dump enabled
      // 2: GPRs dump enabled but SMM GPRs masked
      //
      case 0:
        ExpectedValue.Bits.EnGprs = 0;
        ExpectedValue.Bits.EnGprsInSmm = 0;
        break;
      case 1:
        ExpectedValue.Bits.EnGprs = 1;
        ExpectedValue.Bits.EnGprsInSmm = 1;
        break;
      case 2:
        ExpectedValue.Bits.EnGprs = 1;
        ExpectedValue.Bits.EnGprsInSmm = 0;
        break;
      default:
        break;
    }
  } else {
    ExpectedValue.Bits.EnGprs = 0;
    ExpectedValue.Bits.EnGprsInSmm = 0;
  }
  if (CurrentValue.Uint64 != ExpectedValue.Uint64) {
    AsmWriteMsr64 (MSR_CRASHLOG_CONTROL, ExpectedValue.Uint64);
  }
}
/**
  This function set PMON Address.

  @param[in] PmonAddress     Memory address for PMON
**/
VOID
MsrSetPmonAddress (
  IN  UINT64                 PmonAddress
  )
{
  UINT64    CurrentValue;

  CurrentValue = AsmReadMsr64 (MSR_PMON_BANK1_MISC_MSR_0);
  if (PmonAddress != CurrentValue) {
    AsmWriteMsr64 (MSR_PMON_BANK1_MISC_MSR_0, PmonAddress);
  }
}

/**
  This function set processor trace output.

  @param[in] OutputSchemeTopa     Is Topa or not
  @param[in] MemRegionBaseAddr    Base address of memory
  @param[in] TopaTableBaseAddr    Base address of ToPA table
  @param[in] MemSize              Memory size, 4K align.

  @retval  FALSE: not set MSR
**/
BOOLEAN
MsrSetProcTraceOutput (
  IN BOOLEAN OutputSchemeTopa,
  IN UINTN   MemRegionBaseAddr,
  IN UINTN   TopaTableBaseAddr,
  IN UINT32  MemSize
)
{
  return FALSE;
}
/**
  ProgramRpRecipe features basing on desired settings

  @param[in, out] IsBsp - Pointer to TRUE indicates the function is called from BSP.
**/
VOID
EFIAPI
ProgramRpRecipe (
  IN OUT VOID  *IsBsp
  )
{
  UINT8                         CoreType;

  CoreType = 0;

  if (*(BOOLEAN *) IsBsp) {
    AsmWriteMsr64 (MSR_MSC_CORE_ALLOC_CFG, 0x2028A28A00003FFF);
    AsmWriteMsr64 (MSR_MSC_CORE_COS_WAYS, 0x000F003F03FF3FFF);
    AsmWriteMsr64 (MSR_MSC_CORE_CLOS0_REMAP, 0x00000000);
    AsmWriteMsr64 (MSR_MSC_CORE_CLOS1_REMAP, 0x40505040);
    AsmWriteMsr64 (MSR_MSC_CORE_CLOS2_REMAP, 0xA900A900);
    AsmWriteMsr64 (MSR_MSC_CORE_CLOS3_REMAP, 0xFEFE0000);
    AsmWriteMsr64 (MSR_LLC_QOS_MASK_0, 0x000FFFFF);
    AsmWriteMsr64 (MSR_LLC_QOS_MASK_1, 0x00007FFF);
    AsmWriteMsr64 (MSR_LLC_QOS_MASK_2, 0x000003FF);
    AsmWriteMsr64 (MSR_LLC_QOS_MASK_3, 0x0000001F);
  }
  GetCoreType (&CoreType);
  if (CoreType == CPUID_CORE_TYPE_INTEL_ATOM) {
    AsmWriteMsr64 (MSR_WORK_CONSERVING_CLOS, 0x96532101);
    AsmWriteMsr64 (MSR_L2_QOS_MASK_0, 0xFFFF);
    AsmWriteMsr64 (MSR_L2_QOS_MASK_1, 0x0FFF);
    AsmWriteMsr64 (MSR_L2_QOS_MASK_2, 0x00FF);
    AsmWriteMsr64 (MSR_L2_QOS_MASK_3, 0x0FFF);
    AsmWriteMsr64 (MSR_L2_QOS_MASK_4, 0x00FF);
    AsmWriteMsr64 (MSR_L2_QOS_MASK_5, 0x0053);
    AsmWriteMsr64 (MSR_L2_QOS_MASK_6, 0x0053);
    AsmWriteMsr64 (MSR_L2_QOS_MASK_7, 0x002F);
    AsmWriteMsr64 (MSR_IA32_L2_MASK_8, 0x001A);
    AsmWriteMsr64 (MSR_IA32_L2_MASK_9, 0x000F);
    AsmWriteMsr64 (MSR_IA32_L2_MASK_10, 0xFFFF);
    AsmWriteMsr64 (MSR_IA32_L2_MASK_11, 0xFFFF);
    AsmWriteMsr64 (MSR_IA32_L2_MASK_12, 0xFFFF);
    AsmWriteMsr64 (MSR_IA32_L2_MASK_13, 0xFFFF);
    AsmWriteMsr64 (MSR_IA32_L2_MASK_14, 0xFFFF);
    AsmWriteMsr64 (MSR_IA32_L2_MASK_15, 0xFFFF);
    AsmWriteMsr64 (MSR_IA32_QOS_CORE_BW_THRTL_0, 0x0706050403020100);
    AsmWriteMsr64 (MSR_IA32_QOS_CORE_BW_THRTL_1, 0x0F0E0D0C0B0A0908);
  }
}
