/** @file
  This file contains power management C State configuration functions for
  processors.

  Acronyms:
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

#include "PowerMgmtCommon.h"
#include <Library/CpuPowerManagementLib.h>

/**
  Initializes C States Power management features
**/
VOID
InitCState (
  VOID
  )
{
  ///
  /// Initialize C states, some are general, some are processor specific.
  ///
  /// AcpiIoBase + 0x14 (PM_CST_LVL2) register no longer exists in PCH.
  /// When the IO in this range is read, the CPU puts itself into a mwait
  /// and does not forward this IO to the PCH. MSR_PMG_IO_CAPTURE_BASE was created because
  /// the functionality was moved from the PCH to the CPU.
  ///
  EnableCStates ();

  InitCstatePreWake ();
}

/**
  Disable/Enable the CState Pre-Wake Feature
**/
VOID
InitCstatePreWake (
  VOID
  )
{
  MSR_POWER_CTL_REGISTER PowerCtl;

  PowerCtl.Uint64 = AsmReadMsr64 (MSR_POWER_CTL);
  PowerCtl.Bits.CstatePrewakeDisable = 0;
  if (gCpuPowerMgmtTestConfig->CStatePreWake == FALSE) {
    PowerCtl.Bits.CstatePrewakeDisable = 1;
  }
  AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Uint64);

  return;
}

/**
  Enable C-State support as specified by the input flags on a logical processor.
    Configure BIOS C1 Coordination (SMI coordination)
    Enable IO redirection coordination
    Choose proper coordination method
    Configure extended C-States

  This function must be MP safe.

  @retval EFI_SUCCESS  Processor C-State support configured successfully.
**/
VOID
EFIAPI
ApSafeEnableCStates (
  VOID
  )
{
  MSR_CLOCK_CST_CONFIG_CONTROL_REGISTER     PmCfgCtrlMsr;
  MSR_IA32_MISC_ENABLE_REGISTER             MiscEnable;

  ///
  /// If C-states are disabled in setup, disable C-states
  ///
  if (!(mPpmFlags & PPM_C_STATES)) {
    PmCfgCtrlMsr.Uint64 = AsmReadMsr64 (MSR_CLOCK_CST_CONFIG_CONTROL);
    PmCfgCtrlMsr.Bits.MaxPkgCState = 0;
    AsmWriteMsr64 (MSR_CLOCK_CST_CONFIG_CONTROL, PmCfgCtrlMsr.Uint64);
    return;
  }
  ///
  /// Set C-state package limit to the highest C-state enabled
  ///
  PmCfgCtrlMsr.Uint64 = AsmReadMsr64 (MSR_CLOCK_CST_CONFIG_CONTROL);
  ///
  /// mPpmFlags might be override by others. So update the MaxPkgCState based on mPpmFlags setting.
  ///
  if ((mPpmFlags & PPM_C10) && (PmCfgCtrlMsr.Bits.MaxPkgCState >= V_CSTATE_LIMIT_C10)) {
    PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C10;
  } else if ((mPpmFlags & PPM_C9) && (PmCfgCtrlMsr.Bits.MaxPkgCState >= V_CSTATE_LIMIT_C9)) {
    PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C9;
  } else if ((mPpmFlags & PPM_C8) && (PmCfgCtrlMsr.Bits.MaxPkgCState >= V_CSTATE_LIMIT_C8)) {
    PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C8;
  } else if ((mPpmFlags & PPM_C7S) && (PmCfgCtrlMsr.Bits.MaxPkgCState >= V_CSTATE_LIMIT_C7S)) {
    PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C7S;
  } else if ((mPpmFlags & PPM_C7) && (PmCfgCtrlMsr.Bits.MaxPkgCState >= V_CSTATE_LIMIT_C7)) {
    PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C7;
  } else if ((mPpmFlags & PPM_C6) && (PmCfgCtrlMsr.Bits.MaxPkgCState >= V_CSTATE_LIMIT_C6)) {
    PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C6;
  } else if ((mPpmFlags & PPM_C1) && (PmCfgCtrlMsr.Bits.MaxPkgCState >= V_CSTATE_LIMIT_C1)) {
    PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C1;
  }

  if (gCpuPowerMgmtTestConfig->PkgCStateLimit != PkgCpuDefault) {
    PmCfgCtrlMsr.Bits.MaxPkgCState = 0;

    if (gCpuPowerMgmtTestConfig->PkgCStateLimit < PkgCMax) {
      PmCfgCtrlMsr.Bits.MaxPkgCState = gCpuPowerMgmtTestConfig->PkgCStateLimit;
    } else if ((mPpmFlags & PPM_C10) && (gCpuPowerMgmtTestConfig->PkgCStateLimit == PkgAuto)) {
      PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C10;
    } else if ((mPpmFlags & PPM_C9) && (gCpuPowerMgmtTestConfig->PkgCStateLimit == PkgAuto)) {
      PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C9;
    } else if ((mPpmFlags & PPM_C8) && (gCpuPowerMgmtTestConfig->PkgCStateLimit == PkgAuto)) {
      PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C8;
    } else if ((mPpmFlags & PPM_C7S) && (gCpuPowerMgmtTestConfig->PkgCStateLimit == PkgAuto)) {
      PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C7S;
    } else if ((mPpmFlags & PPM_C7) && (gCpuPowerMgmtTestConfig->PkgCStateLimit == PkgAuto)) {
      PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C7;
    } else if (mPpmFlags & PPM_C6) {
      PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C6;
    } else if (mPpmFlags & PPM_C1) {
      PmCfgCtrlMsr.Bits.MaxPkgCState = V_CSTATE_LIMIT_C1;
    }
  }

  //
  // Enable TimedMwait
  //
  if (mPpmFlags & PPM_TIMED_MWAIT) {
    PmCfgCtrlMsr.Bits.TimedMwaitEnable = 1;
  }

  ///
  /// Configure C-state auto-demotion
  ///
  PmCfgCtrlMsr.Bits.C1StateAutoDemotionEnable = 0;
  if (gCpuPowerMgmtTestConfig->C1AutoDemotion) {
    ///
    /// Enable C6/C7 Auto-demotion to C1
    ///
    PmCfgCtrlMsr.Bits.C1StateAutoDemotionEnable = 1;
  }
  ///
  /// Configure C-state un-demotion
  ///
  PmCfgCtrlMsr.Bits.Enc1undemotion = 0;
  if (gCpuPowerMgmtTestConfig->C1UnDemotion) {
    ///
    /// Enable un-demotion from demoted C1
    ///
    PmCfgCtrlMsr.Bits.Enc1undemotion = 1;
  }
  ///
  /// Configure Package C-state Demotion / un-demotion
  ///
  PmCfgCtrlMsr.Bits.Enpkgcautodemotion = 0;
  PmCfgCtrlMsr.Bits.Enpkgcundemotion = 0;
  if (gCpuPowerMgmtTestConfig->PkgCStateDemotion) {
    ///
    /// Enable Package C-state Demotion
    ///
    PmCfgCtrlMsr.Bits.Enpkgcautodemotion = 1;
  }
  if (gCpuPowerMgmtTestConfig->PkgCStateUnDemotion) {
    ///
    /// Enable Package C-state un-demotion
    ///
    PmCfgCtrlMsr.Bits.Enpkgcundemotion = 1;
  }
  AsmWriteMsr64 (MSR_CLOCK_CST_CONFIG_CONTROL, PmCfgCtrlMsr.Uint64);
  ///
  /// Enable MONITOR/MWAIT support
  /// (already done on BSP, but must be done on all components.)
  ///
  MiscEnable.Uint64 = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);
  MiscEnable.Bits.MONITOR = 1;
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLE, MiscEnable.Uint64);

  return;
}

/**
  Enables C-State support as specified by the input flags on all logical
  processors and sets associated timing requirements in the chipset.

  @param[in] C3IoAddress  IO address to generate C3 states (PM base + 014 usually)
**/
VOID
EnableCStates (
  VOID
  )
{
  MSR_POWER_CTL_REGISTER  PowerCtl;
  EFI_STATUS              Status;

  ///
  /// Enable C-States on all logical processors.
  ///
  ApSafeEnableCStates();
  Status = mMpServices2Ppi->StartupAllAPs (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) ApSafeEnableCStates,
                             FALSE,
                             0,
                             NULL
                             );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "%a: StartupAllAps(): %r\n", __func__, Status));
    ASSERT (FALSE);
  }
  ///
  /// Configure supported enhanced C-states
  ///
  /// Read Power Ctl MSR
  ///
  PowerCtl.Uint64 = AsmReadMsr64 (MSR_POWER_CTL);
  DEBUG ((DEBUG_INFO, "MSR(1FC) before configuring C1E: 0x%08x\n", PowerCtl.Uint64));
  ///
  /// Update Power Control MSR
  ///
  AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Uint64);
  DEBUG ((DEBUG_INFO, "MSR(1FC) after configuring C1E: 0x%08x\n", PowerCtl.Uint64));
}