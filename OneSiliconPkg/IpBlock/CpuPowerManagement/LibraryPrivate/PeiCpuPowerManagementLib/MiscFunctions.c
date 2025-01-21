/** @file
  This file contains Processor Power Management ACPI related functions for
  processors.

  <b>Acronyms:</b>
     - PPM: Processor Power Management
     - TM:  Thermal Monitor
     - IST: Intel(R) Speedstep technology
     - HT:  Hyper-Threading Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#include <Library/BaseLib.h>
#include "PowerMgmtCommon.h"
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuPowerManagementLib.h>

/**
  This will perform Miscellaneous Power Management related programming.

  @param[in]      CpuPowerDeliveryConfig     Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
InitMiscFeatures (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  ///
  /// Configure PL3 limits
  ///
  ConfigurePl3PowerLimits (CpuPowerDeliveryConfig);

  ///
  /// Configure PL4 limits
  ///
  ConfigurePl4PowerLimits (CpuPowerDeliveryConfig);

  ///
  /// Configure DDR RAPL PowerLimits
  ///
  ConfigureDdrPowerLimits ();

  ///
  /// Configure Platform Power Limits
  ///
  ConfigurePlatformPowerLimits (CpuPowerDeliveryConfig);
}

/**
  Locks down all settings.
**/
VOID
PpmLockDown (
  VOID
  )
{
  MSR_PACKAGE_RAPL_LIMIT_REGISTER   PkgRaplLimitMsr;
  MSR_DDR_RAPL_LIMIT_REGISTER       DdrRaplLimitMsr;
  MSR_MISC_PWR_MGMT_REGISTER        MiscPwrMgmtMsr;
  EFI_STATUS                        Status;

  ///
  /// Program PMG_CST_CONFIG MSR [15] (CFG lock bit)
  ///
  ApSafeLockDown (gCpuPowerMgmtTestConfig);
  Status = mMpServices2Ppi->StartupAllAPs (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) ApSafeLockDown,
                             FALSE,
                             0,
                             (VOID *) gCpuPowerMgmtTestConfig
                             );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "%a: StartupAllAps(): %r\n", __func__, Status));
  }

  ///
  /// Lock Package power limit MSR
  ///
  if (gCpuPowerMgmtBasicConfig->TurboPowerLimitLock) {
    PkgRaplLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_RAPL_LIMIT);
    PkgRaplLimitMsr.Bits.PkgPwrLimLock = 1;
    AsmWriteMsr64 (MSR_PACKAGE_RAPL_LIMIT, PkgRaplLimitMsr.Uint64);
  }

  ///
  /// Program Ddr RAPL LIMIT Lock
  ///
  if (gCpuPowerMgmtBasicConfig->TurboPowerLimitLock) {
    DdrRaplLimitMsr.Uint64 = AsmReadMsr64 (MSR_DDR_RAPL_LIMIT);
    DdrRaplLimitMsr.Bits.Locked = 1;
    AsmWriteMsr64 (MSR_DDR_RAPL_LIMIT, DdrRaplLimitMsr.Uint64);
  }

  ///
  /// Program the HWP Lock BIT in MISC PWR MGMT MSR
  ///
  if (gCpuPowerMgmtBasicConfig->HwpLock) {
    MiscPwrMgmtMsr.Uint64 = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
    MiscPwrMgmtMsr.Bits.Lock = 1;
    AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MiscPwrMgmtMsr.Uint64);
  }
  return;
}

/**
  Lock MSR_CST_CONFIG_CONTROL.
  This function must be MP safe.

  @param[in] Buffer    Pointer to the function parameters passed in.
**/
VOID
EFIAPI
ApSafeLockDown (
  IN OUT VOID *Buffer
  )
{
  MSR_CLOCK_CST_CONFIG_CONTROL_REGISTER     PmCfgCtrlMsr;
  CPU_POWER_MGMT_TEST_CONFIG                *CpuPowerMgmtTestConfig;
  UINT8                                     CfgLock;

  CfgLock = TRUE;

  CpuPowerMgmtTestConfig = (CPU_POWER_MGMT_TEST_CONFIG *) Buffer;
  if (CpuPowerMgmtTestConfig != NULL) {
    CfgLock = (UINT8) CpuPowerMgmtTestConfig->PmgCstCfgCtrlLock;
  }
  PmCfgCtrlMsr.Uint64 = AsmReadMsr64 (MSR_CLOCK_CST_CONFIG_CONTROL);
  PmCfgCtrlMsr.Bits.Lock = CfgLock;
  AsmWriteMsr64 (MSR_CLOCK_CST_CONFIG_CONTROL, PmCfgCtrlMsr.Uint64);

  return;
}

/**
  Dump FVID Tables.

  @param[in out] FvidPointer         Pointer to a table to be updated
  @param[in]     LpssNumberOfStates  Number of entries in the table pointed to by FvidPointer for LPSS
  @param[in]     LpssNumberOfStates  Number of entries in the table pointed to by FvidPointer for TPSS
**/
VOID
DumpFvidTable (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT16         LpssNumberOfStates,
  IN UINT16         TpssNumberOfStates
  )
{
  UINTN             Index;

  //
  // Print LPSS and TPSS FVID Tables
  //
  DEBUG ((DEBUG_INFO, "LPSS FVID Table (%d)\n", LpssNumberOfStates));
  DEBUG ((DEBUG_INFO, "Index\tState\tRatio\tPower\n"));
  for (Index = 1; Index <= LpssNumberOfStates ; Index++) {
    DEBUG ((DEBUG_INFO, "%d\t%d\t%d\t%d\n",Index, FvidPointer[Index].FvidState.Limit16State, FvidPointer[Index].FvidState.Limit16BusRatio, FvidPointer[Index].FvidState.Limit16Power));
  }

  DEBUG ((DEBUG_INFO, "TPSS FVID Table (%d)\n", TpssNumberOfStates));
  DEBUG ((DEBUG_INFO, "Index\tState\tRatio\tPower\n"));
  for (Index = 1; Index <= TpssNumberOfStates ; Index++) {
    DEBUG ((DEBUG_INFO, "%d\t%d\t%d\t%d\n",Index, FvidPointer[Index].FvidState.State, FvidPointer[Index].FvidState.BusRatio, FvidPointer[Index].FvidState.Power));
  }
}

