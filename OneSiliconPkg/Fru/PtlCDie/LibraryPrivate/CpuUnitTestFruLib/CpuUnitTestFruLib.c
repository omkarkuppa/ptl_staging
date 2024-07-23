/** @file
  CPU Unit Test product code

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
#include "CpuUnitTestFru.h"
#include <CpuUnitTestCapability.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/CpuGenInfoRegs.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/GenerationMsr.h>
#include <Register/SncuRegs.h>
#include <Library/IoLib.h>
#include <CpuUnitTestFruData.h>

CPU_UNIT_TEST_BASIC  gCpuBasicFru[] = {
  //
  // FamilyModelStepping                       DeviceId0                 PowerUnit, ThermalDesignPower MaxPower MinPower MaxTimeWindow     MaxNonTurboRatio      MaxEfficiencyRatio
  //
  { CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_SA_DEVICE_ID_4C_8A, 3, 28, 0, 0, 100, 36, 8 }, // default config for PTL-P
  { CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_U_SA_DEVICE_ID_4C,         3, 15, 0, 0, 100, 36, 8 },
  { CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_SA_DEVICE_ID_4C_8A,  3, 45, 0, 0, 100, 36, 8 }
};

CPU_UNIT_TEST_FRU_DATA gCpuFruData = {
  SIZE_4MB
};

/**
  Modify CpuUnitTest CpuCapability CpuBasic based on product requirement.

  @param  CpuCapabilityCpuBasic    A pointer to a buffer containing CpuCapability.CpuBasic. Caller need to allocate buffer.
  @param  Size                     Size of the buffer.
  @param  Index                    The index of the CpuBasic Capability.

  @return VOID
**/
VOID
EFIAPI
InitializeCpuBasicCapabilityFru (
  IN OUT CPU_UNIT_TEST_BASIC  *CpuCapabilityCpuBasic,
  IN     UINTN                Size,
  IN     UINTN                Index
  )
{
  ASSERT (Size == sizeof (CPU_UNIT_TEST_BASIC));
  ASSERT (Index < ARRAY_SIZE (gCpuBasicFru));
  CopyMem (CpuCapabilityCpuBasic, &gCpuBasicFru[Index], Size);
}

/**
  Init registers such as mailbox, msr, mmio based on product requirement.

  @param[in]  CpuCapability    Cpu Capability
**/
EFI_STATUS
EFIAPI
CpuUnitTestInitializeRegistersFru (
  CONST CPU_UNIT_TEST_CPU_CAPABILITY  *CpuCapability
  )
{
  UINTN  PciExpressBaseAddress;

  AsmWriteMsr64 (MSR_PL3_CONTROL, 0x0);
  AsmWriteMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT, 0x0);
  AsmWriteMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT_CORES, 0x0);

  PciExpressBaseAddress = (UINTN)PcdGet64 (PcdPciExpressBaseAddress);
  MmioWrite32 (PciExpressBaseAddress + CAPID0_A_HOSTBRIDGE_CFG_REG, 0x0);     // UncoreCapIdRead() Capid0A.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, CAPID0_A_HOSTBRIDGE_CFG_REG));
  MmioWrite32 (PciExpressBaseAddress + CAPID0_B_HOSTBRIDGE_CFG_REG, 0x0);     // UncoreCapIdRead() Capid0B.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, CAPID0_B_HOSTBRIDGE_CFG_REG));
  return EFI_SUCCESS;
}

/**
  Get product specific data via Fru

  @param  CpuFruData    A pointer to CPU_UNIT_TEST_FRU_DATA structure.

  @return VOID
**/
VOID
EFIAPI
CpuUnitTestGetFruData (
  OUT CPU_UNIT_TEST_FRU_DATA  *CpuFruData
  )
{
  CopyMem (CpuFruData, &gCpuFruData, sizeof (CPU_UNIT_TEST_FRU_DATA));
}

/**
  Create UnitTest Suite for specific product

  @param[in]   Framework  Unit test framework to add unit test suite to

  @retval  EFI_SUCCESS            The unit test suite was created.
**/
EFI_STATUS
EFIAPI
CreateCpuUnitTestSuiteFru (
  IN  UNIT_TEST_FRAMEWORK_HANDLE  Framework
  )
{
  EFI_STATUS Status;

  Status = CreateUnitTestSuiteCoreCount (Framework);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed in CreateUnitFixedPointLib. Status = %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

