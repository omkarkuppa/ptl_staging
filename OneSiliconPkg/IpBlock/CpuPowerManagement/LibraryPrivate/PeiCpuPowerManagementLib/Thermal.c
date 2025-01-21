/** @file
  This library contains power management configuration functions for processors.

  <b>Acronyms:</b>
     - PPM:  Processor Power Management
     - TM:   Thermal Monitor
     - IST:  Intel(R) Speedstep technology
     - HT:   Hyper-Threading Technology

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
#include <Library/CpuInfoFruLib.h>

typedef struct {
  UINT32 AutomaticThermalControlCircuit;
  UINT32 CriticalTempEnable;
} THERMAL_CONTROL_REGISTER;

/**
  Get TCC cross throttling teamperature.
  Set Thermal trip point temperature indicated by MSR 1A2h
  If (RATL mode), T0L = MSR 1A2h[23:16]
  Else          , T0L = MSR 1A2h[23:16] - MSR 1A2h[29:24]

  @retval  Value of the activation temperature
**/
UINT32
EFIAPI
CpuGetCrossThrottlingTripPoint (
  VOID
  )
{
  MSR_TEMPERATURE_TARGET_REGISTER   TemperatureTargetMsr;
  UINT32                            Temperature;

  TemperatureTargetMsr.Uint64 = AsmReadMsr64 (MSR_TEMPERATURE_TARGET);
  //
  // Check RATL mode by checking MSR 1A2h[6:0] != 0
  //
  if (TemperatureTargetMsr.Bits.TccOffsetTimeWindow != 0) {
    Temperature = TemperatureTargetMsr.Bits.RefTemp;
  } else {
    Temperature = TemperatureTargetMsr.Bits.RefTemp - TemperatureTargetMsr.Bits.TjMaxTccOffset;
  }

  return Temperature;
}

/**
  This will get value of thermal realted from Register. Will sync to all Aps

  @param[in] ThermalControl       Pointer to THERMAL_CONTROL_REGISTER
**/
VOID
EFIAPI
ApInitThermalControl (
  THERMAL_CONTROL_REGISTER *ThermalControl
  )
{
  MSR_IA32_MISC_ENABLE_REGISTER     MiscEnable;
  MSR_IA32_THERM_INTERRUPT_REGISTER ThermInterruptMsr;

  if (ThermalControl == NULL) {
    return;
  }

  ///
  /// Configure Adaptive thermal monitor. IA32_MISC_ENABLE[3]
  /// (1A0h)IA32_MISC_ENABLE - Bit3:Intel Adaptive Thermal Monitor Enable
  /// System BIOS must always set this bit to be operating within spec.
  ///
  MiscEnable.Uint64 = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);
  MiscEnable.Bits.AutomaticThermalControlCircuit = ThermalControl->AutomaticThermalControlCircuit;
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLE, MiscEnable.Uint64);

  ///
  /// Enable Critical Temperature Interrupt
  ///
  ThermInterruptMsr.Uint64 = AsmReadMsr64 (MSR_IA32_THERM_INTERRUPT);
  ThermInterruptMsr.Bits.CriticalTempEnable = ThermalControl->CriticalTempEnable;
  AsmWriteMsr64 (MSR_IA32_THERM_INTERRUPT, ThermInterruptMsr.Uint64);

  return;
}

/**
  This will sync thermal features from BSP to all logical processors.
**/
VOID
SyncThermal (
  VOID
  )
{
  MSR_IA32_MISC_ENABLE_REGISTER     MiscEnable;
  MSR_IA32_THERM_INTERRUPT_REGISTER ThermInterruptMsr;
  THERMAL_CONTROL_REGISTER          ThermalControl;
  EFI_STATUS                        Status;

  MiscEnable.Uint64 = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);
  ThermalControl.AutomaticThermalControlCircuit = MiscEnable.Bits.AutomaticThermalControlCircuit;
  ThermInterruptMsr.Uint64 = AsmReadMsr64 (MSR_IA32_THERM_INTERRUPT);
  ThermalControl.CriticalTempEnable = ThermInterruptMsr.Bits.CriticalTempEnable;

  ///
  /// Run thermal code on all logical processors.
  ///
  Status = mMpServices2Ppi->StartupAllAPs (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) ApInitThermalControl,
                             FALSE,
                             0,
                             &ThermalControl
                             );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "%a: StartupAllAps(): %r\n", __func__, Status));
    ASSERT (FALSE);
  }
  return;
}

/**
  This will perform general thermal initialization on BSP. Include Below features:

  1. Configure Adaptive thermal monitor
  2. Enable Critical Temperature Interrupt

  @param[in] CpuPowerDeliveryConfig       Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
EarlyThermalInitOnBsp (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  MSR_IA32_THERM_INTERRUPT_REGISTER ThermInterruptMsr;
  MSR_IA32_MISC_ENABLE_REGISTER     MiscEnable;
  MSR_POWER_CTL_REGISTER    PowerCtl;
  UINT8                     PpmTmFlag;
  CPUID_VERSION_INFO_EDX    VersionInfoEdx;
  PpmTmFlag = 0;

  if (CpuPowerDeliveryConfig == NULL) {
    return;
  }

  ///
  /// Configure Adaptive thermal monitor. IA32_MISC_ENABLE[3]
  /// (1A0h)IA32_MISC_ENABLE - Bit3:Intel Adaptive Thermal Monitor Enable
  ///   System BIOS must always set this bit to be operating within spec.
  ///
  MiscEnable.Uint64 = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);
  if (CpuPowerDeliveryConfig->ThermalMonitor == 0) {
    MiscEnable.Bits.AutomaticThermalControlCircuit = 0;
  } else {
    MiscEnable.Bits.AutomaticThermalControlCircuit = 1;
  }
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLE, MiscEnable.Uint64);

  ///
  /// Enable Critical Temperature Interrupt
  ///
  ThermInterruptMsr.Uint64 = AsmReadMsr64 (MSR_IA32_THERM_INTERRUPT);
  ThermInterruptMsr.Bits.CriticalTempEnable = 1;
  AsmWriteMsr64 (MSR_IA32_THERM_INTERRUPT, ThermInterruptMsr.Uint64);

  ///
  /// Read the CPUID values we care about. To get the correct MONITOR/MWAIT-related values,
  /// we need to read after we have disabled limiting and enabled MONITOR/MWAIT
  ///
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, &VersionInfoEdx.Uint32);
  ///
  /// Check Thermal Monitor capable and update the flag.
  ///
  if (VersionInfoEdx.Bits.TM == 1) {
    PpmTmFlag |= PPM_TM;
  }
  if (CpuPowerDeliveryConfig->ThermalMonitor) {
    PpmTmFlag |= PPM_TM;
  }


  if (PpmTmFlag & (PPM_TM)) {
    PowerCtl.Uint64 = AsmReadMsr64 (MSR_POWER_CTL);
    if (CpuPowerDeliveryConfig->DisableVrThermalAlert == 1) {
      DEBUG ((DEBUG_INFO, "VR Thermal Alert is disabled\n"));
      PowerCtl.Bits.VrThermAlertDisable = 1;
    } else {
      PowerCtl.Bits.VrThermAlertDisable = 0;
    }
    AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Uint64);
  }

  return;
}