/** @file
  This file contains P States and Turbo Power Management configuration functions for
  processors.

  <b>Acronyms:</b>
     - PPM:  Processor Power Management
     - TM:   Thermal Monitor
     - IST:  Intel(R) Speedstep technology
     - HT:   Hyper-Threading Technology
     - ITBM: Intel(R) Turbo Boost Max Technology

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
#include <Library/BaseMemoryLib.h>
#include "PowerMgmtCommon.h"
#include <SiPolicyStruct.h>
#include <Library/PeiCpuInitFruLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/CpuLib.h>
#include <Library/OcMailboxLib.h>
#include <Library/MsrFruLib.h>
#include <Register/B2pMailbox.h>

typedef union _MAILBOX_DATA {
  UINT32   MailboxData;      ///< All bit fields as a 32-bit value.
  ///
  /// Individual bit fields.
  ///
  struct {
    UINT8  CurrentRatio: 8;  ///< CPU Current Ratio
    UINT8  FusedRatio  : 8;  ///< CPU Fused Ratio
    UINT16 Reserved    : 16; ///< Reserved for future use
  } Fields;
} MAILBOX_DATA;

typedef struct _MAILBOX_READ {
  MAILBOX_DATA *MailboxDataPtr;
  EFI_STATUS   *MailboxStatus;
} MAILBOX_READ;

/**
  Set Turbo Ratio Limit.

  @param[in] CpuPowerMgmtBasicConfig    Pointer to CPU_POWER_MGMT_BASIC_CONFIG instance

**/
VOID
EFIAPI
PeiCpuSetTurboRatioLimit (
  IN CPU_POWER_MGMT_BASIC_CONFIG    *CpuPowerMgmtBasicConfig
  )
{
  MSR_TURBO_RATIO_LIMIT_REGISTER                    MsrTurboRatioLimitRatio;
  MSR_TURBO_RATIO_LIMIT_CORES_REGISTER              MsrTurboRatioLimitCores;
  MSR_ATOM_TURBO_RATIO_LIMIT_REGISTER               AtomMsrTurboRatioLimitRatio;
  MSR_ATOM_TURBO_RATIO_LIMIT_CORES_REGISTER         AtomMsrTurboRatioLimitCores;

  UINT8                                             Index;
  UINT8                                             MaxBusRatio;
  BOOLEAN                                           Valid;

  MaxBusRatio = GetMaxNonTurboRatio ();

  Valid = TRUE;

  //
  // RATIO_[i] must be not less than Max Non-Turbo Ratio and
  // RATIO_[i+1] must be less than or equal to RATIO_[i]
  //
  for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE - 1; Index++) {
    if (Index == 0) {
      if (CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[0] != 0 &&
          CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[0] < MaxBusRatio) {
        Valid = FALSE;
        break;
      }
    }

    if (CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[Index+1] != 0 &&
        (CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[Index+1] < MaxBusRatio ||
         CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[Index+1] > CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[Index])) {
      Valid = FALSE;
      break;
    }
  }

  if (Valid) {
    MsrTurboRatioLimitRatio.Bits.RatioLimit0 = CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[0];
    MsrTurboRatioLimitRatio.Bits.RatioLimit1 = CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[1];
    MsrTurboRatioLimitRatio.Bits.RatioLimit2 = CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[2];
    MsrTurboRatioLimitRatio.Bits.RatioLimit3 = CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[3];
    MsrTurboRatioLimitRatio.Bits.RatioLimit4 = CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[4];
    MsrTurboRatioLimitRatio.Bits.RatioLimit5 = CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[5];
    MsrTurboRatioLimitRatio.Bits.RatioLimit6 = CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[6];
    MsrTurboRatioLimitRatio.Bits.RatioLimit7 = CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[7];
    AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT, MsrTurboRatioLimitRatio.Uint64);
  }

  Valid = TRUE;

  //
  // NUMCORE_[i+1] must be greater than NUMCORE_[i]
  //
  for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE - 1; Index++) {
    if (CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[Index+1] != 0 &&
        CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[Index+1] <= CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[Index]) {
      Valid = FALSE;
      break;
    }
  }

  if (Valid) {
    MsrTurboRatioLimitCores.Bits.CoreCount0 = CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[0];
    MsrTurboRatioLimitCores.Bits.CoreCount1 = CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[1];
    MsrTurboRatioLimitCores.Bits.CoreCount2 = CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[2];
    MsrTurboRatioLimitCores.Bits.CoreCount3 = CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[3];
    MsrTurboRatioLimitCores.Bits.CoreCount4 = CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[4];
    MsrTurboRatioLimitCores.Bits.CoreCount5 = CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[5];
    MsrTurboRatioLimitCores.Bits.CoreCount6 = CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[6];
    MsrTurboRatioLimitCores.Bits.CoreCount7 = CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[7];
    AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT_CORES, MsrTurboRatioLimitCores.Uint64);
  }

  Valid = TRUE;

  //
  // RATIO_[i+1] must be less than or equal to RATIO_[i]
  //
  for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE - 1; Index++) {
    if (CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[Index+1] != 0 &&
        CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[Index+1] > CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[Index]) {
      Valid = FALSE;
      break;
    }
  }

  if (Valid) {
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit0 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[0];
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit1 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[1];
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit2 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[2];
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit3 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[3];
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit4 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[4];
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit5 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[5];
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit6 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[6];
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit7 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[7];
    AsmWriteMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT, AtomMsrTurboRatioLimitRatio.Uint64);
  }

  Valid = TRUE;

  //
  // NUMCORE_[i+1] must be greater than NUMCORE_[i]
  //
  for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE - 1; Index++) {
    if (CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[Index+1] != 0 &&
        CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[Index+1] <= CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[Index]) {
      Valid = FALSE;
      break;
    }
  }

  if (Valid) {
    AtomMsrTurboRatioLimitCores.Bits.CoreCount0 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[0];
    AtomMsrTurboRatioLimitCores.Bits.CoreCount1 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[1];
    AtomMsrTurboRatioLimitCores.Bits.CoreCount2 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[2];
    AtomMsrTurboRatioLimitCores.Bits.CoreCount3 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[3];
    AtomMsrTurboRatioLimitCores.Bits.CoreCount4 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[4];
    AtomMsrTurboRatioLimitCores.Bits.CoreCount5 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[5];
    AtomMsrTurboRatioLimitCores.Bits.CoreCount6 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[6];
    AtomMsrTurboRatioLimitCores.Bits.CoreCount7 = CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[7];
    AsmWriteMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT_CORES, AtomMsrTurboRatioLimitCores.Uint64);
  }
}

/**
  Initializes Turbo Ratio limits in the processor.
**/
VOID
InitTurboRatioLimits (
  VOID
  )
{
  MSR_FLEX_RATIO_REGISTER            FlexRatioMsr;
  MSR_MISC_ENABLES_REGISTER          MiscEnableMsr;
  MSR_UNCORE_RATIO_LIMIT_REGISTER    RingRatioMsr;
  UINT8                              OverclockingBins;
  CPUID_THERMAL_POWER_MANAGEMENT_EAX Eax;

  ///
  /// Test for Turbo Mode supported and initialize if true.
  ///
  AsmCpuid (CPUID_THERMAL_POWER_MANAGEMENT, &Eax.Uint32, NULL, NULL, NULL);
  MiscEnableMsr.Uint64 = AsmReadMsr64 (MSR_MISC_ENABLES);
  if (mPpmFlags & PPM_TURBO) {
    ///
    /// Clear Turbo Mode disable bit in IA32 Misc Enable MSR
    ///
    MiscEnableMsr.Bits.TurboModeDisable = 0;
    AsmWriteMsr64 (MSR_MISC_ENABLES, MiscEnableMsr.Uint64);
  } else if ((MiscEnableMsr.Bits.TurboModeDisable == 0) &&
             (Eax.Bits.TurboBoostTechnology == 1)) {
    ///
    /// If Turbo mode is supported but required to be disabled (by Policy setting)
    /// Set Turbo Mode disable bit in IA32 Misc Enable MSR since it might be temporarily enabled earlier.
    ///
    MiscEnableMsr.Bits.TurboModeDisable = 1;
    AsmWriteMsr64 (MSR_MISC_ENABLES, MiscEnableMsr.Uint64);
  }

  ///
  /// Check if processor turbo-ratio can be overriden
  ///
  if (!mRatioLimitProgrammble) {
    DEBUG ((DEBUG_WARN, "Turbo Ratio Limit is NOT programmable. Platform Info MSR (0xCE) [28] is not set \n"));
    return;
  }
  ///
  /// Read the overclocking bins
  ///
  FlexRatioMsr.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
  OverclockingBins = (UINT8) FlexRatioMsr.Bits.OcBins;
  if (FlexRatioMsr.Bits.OcLock) { ///<  Check for Overclocking Lock bit
    DEBUG ((DEBUG_INFO, "OverClocking Lock Bit is set. Disable the Lock and reset the system\n"));
    return;
  }

  PeiCpuSetTurboRatioLimit (gCpuPowerMgmtBasicConfig);

  ///
  /// For fully unlocked CPU's, configure Turbo Ratio as 0xFF (max possible P-State)
  ///
  if (OverclockingBins == MAX_OVERCLOCKING_BINS) {
    mTurboBusRatio  = 0xFF;
  } else {
    ///
    /// mTurboBusRatio has been initialized to Fused Max Turbo Ratio in InitPowerManagementGlobalVariables().
    /// Max Turbo ratio or P0 = Fused Max Turbo Ratio + No of over clocking Bins.
    ///
    mTurboBusRatio  += OverclockingBins;
  }

  ///
  /// If  a non-default ring ratio is specified, we need to update the ring ratio limit MSR
  ///
  if ((gCpuPowerMgmtBasicConfig->MinRingRatioLimit != 0) || (gCpuPowerMgmtBasicConfig->MaxRingRatioLimit != 0)) {
    RingRatioMsr.Uint64 = AsmReadMsr64 (MSR_UNCORE_RATIO_LIMIT);
    if (gCpuPowerMgmtBasicConfig->MaxRingRatioLimit != 0) {
      RingRatioMsr.Bits.MaxClrRatio = gCpuPowerMgmtBasicConfig->MaxRingRatioLimit;
    }
    if (gCpuPowerMgmtBasicConfig->MinRingRatioLimit != 0) {
      RingRatioMsr.Bits.MinClrRatio = gCpuPowerMgmtBasicConfig->MinRingRatioLimit;
    }
    DEBUG ((DEBUG_INFO, "PPM: Programming Ring Ratio Limits: Min = 0x%X, Max = 0x%X\n",gCpuPowerMgmtBasicConfig->MinRingRatioLimit, gCpuPowerMgmtBasicConfig->MaxRingRatioLimit ));
    AsmWriteMsr64 (MSR_UNCORE_RATIO_LIMIT, RingRatioMsr.Uint64);
  }

  return;
}

/**
  Initializes Energy efficient P-state feature.
**/
VOID
InitEnergyEfficientPState (
  VOID
  )
{
  MSR_POWER_CTL_REGISTER       PowerCtl;

  ///
  /// Configure Energy Efficient P-state : POWER_CTL[18]
  ///
  if (mPpmFlags & PPM_EEPST) {
    PowerCtl.Uint64 = AsmReadMsr64 (MSR_POWER_CTL);
    PowerCtl.Bits.PwrPerfPltfrmOvr = 1;
    AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Uint64);
  }
  return;
}

/**
  Initializes Energy efficient Turbo feature.
**/
VOID
InitEnergyEfficientTurbo (
  VOID
  )
{
  MSR_POWER_CTL_REGISTER       PowerCtl;

  ///
  /// Configure Energy Efficient Turbo Disable: POWER_CTL[19]
  /// This is a disable bit, so set 1 to disable EE Turbo
  ///
  if (gCpuPowerMgmtTestConfig->EnergyEfficientTurbo == DISABLED) {
    PowerCtl.Uint64 = AsmReadMsr64 (MSR_POWER_CTL);
    PowerCtl.Bits.EeTurboDisable = 1;
    AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Uint64);
  }
  return;
}

/**
  Initializes Race To Halt feature.
**/
VOID
InitRaceToHalt (
  VOID
  )
{
  MSR_POWER_CTL_REGISTER       PowerCtl;
  ///
  /// Configure Race To Halt feature : POWER_CTL[20]
  ///
  if (gCpuPowerMgmtTestConfig->RaceToHalt == DISABLED) {
    PowerCtl.Uint64 = AsmReadMsr64 (MSR_POWER_CTL);
    PowerCtl.Bits.RthDisable = 1;
    AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Uint64);
  }
  return;
}

/**
  Create default FVID table with max and min states only.

  @param[in out] FvidPointer  Pointer to a table to be updated
**/
VOID
CreateDefaultFvidTable (
  IN OUT FVID_TABLE *FvidPointer
  )
{
  UINT64 wPower1;
  UINT64 wPower2;

  DEBUG ((DEBUG_INFO, "CreateDefaultFvidTable - Start\n"));

  ///
  /// Fill in the FVid table header.
  ///
  FvidPointer[0].FvidHeader.Stepping = GetCpuFamilyModel() | GetCpuSteppingId();
  FvidPointer[0].FvidHeader.MaxBusRatio = mMaxBusRatio;
  FvidPointer[0].FvidHeader.EistStates  = 2;
  ///
  /// First entry is state 0, highest state.
  ///
  FvidPointer[1].FvidState.State    = 0;
  FvidPointer[1].FvidState.BusRatio = mMaxBusRatio;
  FvidPointer[1].FvidState.Power    = (mPackageTdpWatt * 1000);  /// Power is calculated in milliwatts

  ///
  /// Second entry is state 1, lowest state.
  ///
  FvidPointer[2].FvidState.State    = 1;
  FvidPointer[2].FvidState.BusRatio = (UINT16) mMinBusRatio;
  ///
  /// Calculate Relative Power
  ///
  wPower1 = (mMaxBusRatio - FvidPointer[2].FvidState.BusRatio) * 625;
  wPower1 = (110000 - wPower1);
  wPower1 = DivU64x32 (wPower1, 11);
  wPower1 = DivU64x32 (MultU64x64 (wPower1, wPower1), 1000);
  //
  // Power is calculated in milliwatts
  //
  wPower2 = (UINT64) (((FvidPointer[2].FvidState.BusRatio * 100000) / mMaxBusRatio) / 100);
  wPower2 = DivU64x32 (MultU64x32 (MultU64x64 (wPower2, DivU64x32 (wPower1, 100)), mPackageTdpWatt), 1000);
  FvidPointer[2].FvidState.Power = (UINT16) wPower2;
  ///
  /// First entry is state 0, highest state. (Limit16 states)
  ///
  FvidPointer[1].FvidState.Limit16State    = 0;
  FvidPointer[1].FvidState.Limit16BusRatio = mMaxBusRatio;
  FvidPointer[1].FvidState.Limit16Power    = (mPackageTdpWatt * 1000);  /// Power is calculated in milliwatts
  ///
  /// Second entry is state 1, lowest state.
  ///
  FvidPointer[2].FvidState.Limit16State    = 1;
  FvidPointer[2].FvidState.Limit16BusRatio = (UINT16) mMinBusRatio;

  ///
  /// Calculate Relative Power
  ///
  wPower1 = (mMaxBusRatio - FvidPointer[2].FvidState.Limit16BusRatio) * 625;
  wPower1 = (110000 - wPower1);
  wPower1 = DivU64x32 (wPower1, 11);
  wPower1 = DivU64x32 (MultU64x64 (wPower1, wPower1), 1000);
  //
  // Power is calculated in milliwatts
  //
  wPower2 = (UINT64) (((FvidPointer[2].FvidState.Limit16BusRatio * 100000) / mMaxBusRatio) / 100);
  wPower2 = DivU64x32 (MultU64x32 (MultU64x64 (wPower2, DivU64x32 (wPower1, 100)), mPackageTdpWatt), 1000);
  FvidPointer[2].FvidState.Limit16Power = (UINT16) wPower2;

  DEBUG ((DEBUG_INFO, "CreateDefaultFvidTable - End\n"));
}

/**
  Calculate the ratio for the requested p state

  @param[in] MaxRatio           Maximum Supported Ratio (HFM)
  @param[in] MinRatio           Minimum Supported Ratio (LFM)
  @param[in] MaxNumberOfStates  Number of entries in the table pointed to by FvidPointer
  @param[in] PStateNumber       Desired P State from range 0..MaxNumberOfStates

  @retval Ratio for the requested Pstate
**/
UINT16
ComputePstateRatio (
  IN UINT16 MaxRatio,
  IN UINT16 MinRatio,
  IN UINT16 MaxNumberOfStates,
  IN UINT16 PStateNumber
  )
{
  UINT16 RatioRange;
  UINT16 NumGaps;
  UINT16 PStateRatio;

  RatioRange  = MaxRatio - MinRatio;
  NumGaps     = MaxNumberOfStates - 1;
  PStateRatio = MaxRatio - (((PStateNumber * RatioRange) + (NumGaps / 2)) / NumGaps);

  return PStateRatio;
}

/**
  Create FVID table

  @param[in out] FvidPointer         Pointer to a table to be updated
  @param[in]     MaxNumberOfStates   Number of entries in the table pointed to by FvidPointer

  @retval EFI_SUCCESS            FVID table created successfully.
  @retval EFI_INVALID_PARAMETER  The bus ratio range don't permit FVID table calculation;
                                 a default FVID table should be constructed.
**/
EFI_STATUS
CreateFvidTable (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT16         MaxNumberOfStates
  )
{
  UINT16 BusRatioRange;
  UINT16 NumberOfStates;
  UINT16 NumberOfNonTurboStates;
  UINT16 Turbo;
  UINT16 Index;
  UINT64 wPower1;
  UINT64 wPower2;

  DEBUG((DEBUG_INFO, "CreateFvidTable Start\n"));

  ///
  /// Determine the bus ratio range, P1 to Pn. The +1 is required
  /// to ensure Pn entry is counted.
  ///
  BusRatioRange = mMaxBusRatio - mMinBusRatio + 1;
  if (((INT16) BusRatioRange < 1) || (MaxNumberOfStates == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// Determine the HFM state index
  ///
  Turbo = ((mPpmFlags & PPM_TURBO) ? 1 : 0);
  ///
  ///  Determine the number of states as cpu supported range or Maximum _PSS limit
  ///
  NumberOfStates = ((BusRatioRange + Turbo) < MaxNumberOfStates ? (BusRatioRange + Turbo) : MaxNumberOfStates);
  ///
  /// Ensure we have at least two states
  ///
  if (NumberOfStates < 2) {
    ///
    /// In case HFM = LFM and no Turbo, at least have two states with same ratio values.
    ///
    NumberOfStates = 2;
  }
  ///
  /// Number of allowed P-states between P1 to Pn.
  ///
  NumberOfNonTurboStates = NumberOfStates - Turbo;
  ///
  /// Fill in the table header
  ///
  FvidPointer[0].FvidHeader.Stepping = GetCpuFamilyModel() | GetCpuSteppingId();
  FvidPointer[0].FvidHeader.MaxBusRatio = (Turbo ? mTurboBusRatio : mMaxBusRatio);
  FvidPointer[0].FvidHeader.EistStates  = (UINT16) NumberOfStates;
  ///
  /// Add Turbo as P0 if Turbo Mode supported and initialize.
  ///
  if (mPpmFlags & PPM_TURBO) {
    FvidPointer[1].FvidState.BusRatio = mTurboBusRatio;
    FvidPointer[1].FvidState.Power = (mPackageTdpWatt * 1000); // power is calculated in milliwatts
    FvidPointer[1].FvidState.Limit16BusRatio = mTurboBusRatio;
    FvidPointer[1].FvidState.Limit16Power = (mPackageTdpWatt * 1000);
  }

  ///
  /// Add HFM as P1 based on Max Turbo availablity
  ///
  FvidPointer[1 + Turbo].FvidState.State    = Turbo;
  FvidPointer[1 + Turbo].FvidState.BusRatio = mMaxBusRatio;
  FvidPointer[1 + Turbo].FvidState.Power = (mPackageTdpWatt * 1000);

  FvidPointer[1 + Turbo].FvidState.Limit16State    = Turbo;
  FvidPointer[1 + Turbo].FvidState.Limit16BusRatio = mMaxBusRatio;
  FvidPointer[1 + Turbo].FvidState.Limit16Power    = (mPackageTdpWatt * 1000);

  ///
  /// Fill in the table starting at the last entry
  /// The algorithm is available in the processor BIOS writer's guide.
  ///
  for (Index = 1; Index < (LPSS_FVID_MAX_STATES - Turbo); Index++) {
    FvidPointer[Index + 1 + Turbo].FvidState.Limit16State    = Index + Turbo;
    FvidPointer[Index + 1 + Turbo].FvidState.Limit16BusRatio = ComputePstateRatio (mMaxBusRatio, mMinBusRatio, (LPSS_FVID_MAX_STATES - Turbo), Index);
    ///
    /// Calculate Relative Power
    ///
    wPower1 = (mMaxBusRatio - FvidPointer[Index + 1 + Turbo].FvidState.Limit16BusRatio) * 625;
    wPower1 = (110000 - wPower1);
    wPower1 = DivU64x32 (wPower1, 11);
    wPower1 = MultU64x64 (wPower1, wPower1);
    //
    // Power is calculated in milliwatts
    //
    wPower2 = (((FvidPointer[Index + 1 + Turbo].FvidState.Limit16BusRatio * 100) / mMaxBusRatio));
    wPower2 = DivU64x32 (MultU64x32 (MultU64x64 (wPower2, wPower1), mPackageTdpWatt), 10000000);
    FvidPointer[Index + 1 + Turbo].FvidState.Limit16Power = (UINT32) wPower2;
  }
  for (Index = 1; Index < NumberOfNonTurboStates; Index++) {
    FvidPointer[Index + 1 + Turbo].FvidState.State    = Index + Turbo;
    FvidPointer[Index + 1 + Turbo].FvidState.BusRatio = ComputePstateRatio (mMaxBusRatio, mMinBusRatio, NumberOfNonTurboStates, Index);
    ///
    /// Calculate Relative Power
    ///
    wPower1 = (mMaxBusRatio - FvidPointer[Index + 1 + Turbo].FvidState.BusRatio) * 625;
    wPower1 = (110000 - wPower1);
    wPower1 = DivU64x32 (wPower1, 11);
    wPower1 = MultU64x64 (wPower1, wPower1);
    //
    // Power is calculated in milliwatts
    //
    wPower2 = (((FvidPointer[Index + 1 + Turbo].FvidState.BusRatio * 100) / mMaxBusRatio));
    wPower2 = DivU64x32 (MultU64x32 (MultU64x64 (wPower2, wPower1), mPackageTdpWatt), 10000000);
    FvidPointer[Index + 1 + Turbo].FvidState.Power = (UINT32) wPower2;
  }

  DEBUG ((DEBUG_INFO, "CreateFvidTable End\n"));
  return EFI_SUCCESS;
}

/**
  Create a custom FVID table based on setup options.

  @param[in out] FvidPointer         Pointer to a table to be updated
  @param[in]     MaxNumberOfStates   Number of entries in the table pointed to by FvidPointer
**/
VOID
CreateCustomFvidTable (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT16         MaxNumberOfStates
  )
{
  UINT16  Index;
  BOOLEAN UseStateRatioMax16;
  UINT16  CustomRatioTableNumberOfEntries;

  CustomRatioTableNumberOfEntries = gCpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries;

  if (CustomRatioTableNumberOfEntries > MaxNumberOfStates) {
    DEBUG (
            (DEBUG_WARN,
             "VidNumber(%d) is greater than maximum(%d) supported.",
             CustomRatioTableNumberOfEntries,
             MaxNumberOfStates)
            );
    CustomRatioTableNumberOfEntries = MaxNumberOfStates;
  }

  ///
  /// Fill in the table header
  ///
  FvidPointer[0].FvidHeader.Stepping    = gCpuPowerMgmtCustomConfig->CustomRatioTable.Cpuid;
  FvidPointer[0].FvidHeader.MaxBusRatio = (UINT16) gCpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio;
  FvidPointer[0].FvidHeader.EistStates  = CustomRatioTableNumberOfEntries;

  UseStateRatioMax16 = TRUE;
  if (gCpuPowerMgmtCustomConfig->CustomRatioTable.StateRatioMax16[0] == 0) {
    UseStateRatioMax16 = FALSE;
  }

  ///
  /// Fill in the state data
  ///
  for (Index = 0; Index < CustomRatioTableNumberOfEntries; Index++) {
    FvidPointer[Index + 1].FvidState.State    = Index;
    FvidPointer[Index + 1].FvidState.BusRatio = (UINT16) gCpuPowerMgmtCustomConfig->CustomRatioTable.StateRatio[Index];
    if (Index < LPSS_FVID_MAX_STATES) {
      FvidPointer[Index + 1].FvidState.Limit16State    = Index;
      if (UseStateRatioMax16 == TRUE) {
        FvidPointer[Index + 1].FvidState.Limit16BusRatio = (UINT16) gCpuPowerMgmtCustomConfig->CustomRatioTable.StateRatioMax16[Index];
      } else {
        FvidPointer[Index + 1].FvidState.Limit16BusRatio = (UINT16) gCpuPowerMgmtCustomConfig->CustomRatioTable.StateRatio[Index];
      }
    }
  }
}

/**
  This function updates the table provided with the FVID data for the processor.
  If CreateDefaultTable is TRUE, a minimum FVID table will be provided.
  The maximum number of states must be greater then or equal to two.
  The table should be initialized in such a way as for the caller to determine if the
  table was updated successfully.

  @param[in out] FvidPointer         Pointer to a table to be updated
  @param[in]     MaxNumberOfStates   Number of entries in the table pointed to by FvidPointer
**/
VOID
InitFvidTable (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT16         MaxNumberOfStates
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "InitFvidTable - Start\n"));
  ///
  /// Create FVID table.
  /// If specified, create a custom FVID table.
  ///
  if (gCpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries >= 2) {
    CreateCustomFvidTable (FvidPointer, MaxNumberOfStates);
  } else {
    Status = CreateFvidTable (FvidPointer, MaxNumberOfStates);
  }

  if (EFI_ERROR (Status)) {
    CreateDefaultFvidTable (FvidPointer);
    mPpmFlags &= ~PPM_TURBO;
  }

  DEBUG ((DEBUG_INFO, "InitFvidTable - End\n"));
  return;
}

/**
  Enables EIST support in a logical processor.
  This function must be MP safe.
**/
VOID
EFIAPI
ApSafeEnableEist (
  VOID
  )
{
  MSR_MISC_ENABLES_REGISTER      MiscEnable;
  MSR_MISC_PWR_MGMT_REGISTER     PwrMgmtMsr;

  ///
  /// Enable EIST in the CPU MSR.
  ///
  MiscEnable.Uint64 = AsmReadMsr64 (MSR_MISC_ENABLES);
  MiscEnable.Bits.EnableGv3 = 1;
  AsmWriteMsr64 (MSR_MISC_ENABLES, MiscEnable.Uint64);

  ///
  /// If CMP is disabled, disable hardware coordination.
  ///
  if (!(mPpmFlags & PPM_CMP)) {
    PwrMgmtMsr.Uint64 = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
    PwrMgmtMsr.Bits.SinglePctlEn = 1;
    AsmWriteMsr64 (MSR_MISC_PWR_MGMT, PwrMgmtMsr.Uint64);
  }

  return;
}

/**
  Enables EIST support in the processor.
**/
VOID
InitEist (
  VOID
  )
{
  EFI_STATUS  Status;

  ///
  /// Enable EIST on all APs. For BSP, it was already enabled during pre-RESET_CPL initialization.
  ///
  ApSafeEnableEist ();
  Status = mMpServices2Ppi->StartupAllAPs (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) ApSafeEnableEist,
                             FALSE,
                             0,
                             NULL
                             );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "%a: StartupAllAps(): %r\n", __func__, Status));
  }
  return;
}

/**
  Configures the Interrupt Redirection Mode Selection for Logical Interrupts.
**/
VOID
InitPpmIrmConfiguration (
  VOID
  )
{
  UINTN  MchBar;
  UINT32 Data32And;
  UINT32 Data32Or;

  ///
  /// Get the MCH space base address and program MMIO register MCHBAR+0x5418 to enable specific routing algorithm.
  ///
  MchBar    = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  Data32And = (UINT32) ~(BIT2 + BIT1 + BIT0);
  Data32Or  = (UINT32) (gCpuPowerMgmtTestConfig->PpmIrmSetting & (BIT2 + BIT1 + BIT0));
  MmioAndThenOr32 (MchBar + 0x5418, Data32And, Data32Or);
}

/**
  Initializes HWP
  Enable HWP and if LVT interrupt supported, enable APIC to generate SMI
**/
VOID
InitializeHwp (
  VOID
  )
{
  MSR_MISC_PWR_MGMT_REGISTER     MiscPwrMgmtMsr;

  MiscPwrMgmtMsr.Uint64 = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
  if (gCpuPowerMgmtBasicConfig->Hwp == 1) {
    ///
    /// Set bit 6 of MSR MISC_PWR_MGMT (1AAh) to enable HWP support
    ///
    if (mPpmFlags & PPM_HWP) {
      MiscPwrMgmtMsr.Bits.EnableHwp = 1;
    }

    ///
    /// Set bit 7 of MSR MISC_PWR_MGMT (1AAh) to enable HWP interrupt
    ///
    if (MiscPwrMgmtMsr.Bits.EnableHwp) {
      if (MiscPwrMgmtMsr.Bits.EnableHwpInterrupt) {
        ///
        /// If HWP Interrupt was enabled in setup, 0x1AA[7] will be set
        ///
        MiscPwrMgmtMsr.Bits.EnableHwpInterrupt = gCpuPowerMgmtBasicConfig->HwpInterruptControl;
      }
    }
  } else {
    ///
    /// To disable HWP, Clear BIT6(ENABLE_HWP), BIT7(HWP_INTERRUPT), BIT12(HWP_EPP_MASK) and BIT1 (HWP Voting rights)
    ///
    MiscPwrMgmtMsr.Bits.EnableHwp = 0;
    MiscPwrMgmtMsr.Bits.EnableHwpInterrupt = 0;
    MiscPwrMgmtMsr.Bits.EnableHwpEpp = 0;
    //
    // BIT1 HWP Voting rights is not there in SDM
    //
    MiscPwrMgmtMsr.Uint64 &= (UINT64) ~(BIT1);
  }

  AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MiscPwrMgmtMsr.Uint64);
  return;
}

/**
  Initializes Dynamic Efficiency Control feature.
**/
VOID
InitDynamicEfficiencyControl (
  VOID
  )
{
  MSR_POWER_CTL_REGISTER  PowerCtl;

  ///
  /// Configure Dynamic Efficiency Control: POWER_CTL[27]
  /// When set, allows SoC to control energy efficiency targets autonomously, regardless of EPP, EPB and other SW inputs.
  ///
  if (gCpuPowerMgmtBasicConfig->EnableDynamicEfficiencyControl) {
    PowerCtl.Uint64 = AsmReadMsr64 (MSR_POWER_CTL);
    ///
    /// DisableOok actually represents DYNAMIC_EFFICIENCY_CONTROL_EN (POWER_CTL[27])
    ///
    PowerCtl.Bits.DisableOok = 1;
    AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Uint64);
  }

  return;
}

/**
  Initialize Per Core P State OS control mode.
  Initialize HwP Autonomous Per Core P State.
  Initialize HwP Autonomous EPP grouping
  Initialize EPB override over PECI.
  Enable/Disable Fast Msr Support for IA32_HWP_REQUEST.
**/
VOID
InitializeHwpMiscFeatures (
  VOID
  )
{
  EFI_STATUS                     Status;
  UINT32                         LibStatus;
  MAILBOX_DATA_AUTONOMOUS_PARAMS MailBoxData;
  PCODE_MAILBOX_INTERFACE        MailboxCommand;

  MailBoxData.Data32 = 0;

  if (gCpuPowerMgmtBasicConfig->Hwp == 1) {

    ///
    /// HwP Autonomous Per Core P State (PCPS)
    ///
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_READ_AUTONOMOUS_PARAMS;
    MailboxRead (MailboxCommand.InterfaceData, &MailBoxData.Data32, &LibStatus);
    ///
    /// Autonomous PCPS mailbox follows reverse encoding.
    /// Disable by seting Bit 30 and autonomous will request the same value for all cores all the time.
    ///
    if(MailBoxData.Fields.PcpsDisable == gCpuPowerMgmtBasicConfig->EnableHwpAutoPerCorePstate) {
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_WRITE_AUTONOMOUS_PARAMS;
      MailBoxData.Fields.PcpsDisable = ~(gCpuPowerMgmtBasicConfig->EnableHwpAutoPerCorePstate);
      Status = MailboxWrite (MailboxCommand.InterfaceData, MailBoxData.Data32, &LibStatus);
      if (Status != EFI_SUCCESS || LibStatus != PCODE_MAILBOX_CC_SUCCESS) {
        DEBUG ((DEBUG_WARN, "(Hwp) Failed to change HwP Autonomous PCPS, mailbox status = %X\n", LibStatus));
      }
    }

    ///
    /// HwP Autonomous EPP grouping
    ///
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_READ_AUTONOMOUS_PARAMS;
    MailboxRead (MailboxCommand.InterfaceData, &MailBoxData.Data32, &LibStatus);
    ///
    /// Autonomous EPP grouping mailbox follows reverse encoding.
    /// Disable by seting Bit 29 and autonomous will not necessarily request the same value for all cores
    /// with the same EPP.
    ///
    if (MailBoxData.Fields.EppGroupingDisable == gCpuPowerMgmtBasicConfig->EnableHwpAutoEppGrouping) {
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_WRITE_AUTONOMOUS_PARAMS;
      MailBoxData.Fields.EppGroupingDisable = ~(gCpuPowerMgmtBasicConfig->EnableHwpAutoEppGrouping);
      Status = MailboxWrite (MailboxCommand.InterfaceData, MailBoxData.Data32, &LibStatus);
      if (Status != EFI_SUCCESS || LibStatus != PCODE_MAILBOX_CC_SUCCESS) {
        DEBUG ((DEBUG_WARN, "(Hwp) Failed to change HwP Autonomous EPP Grouping, mailbox status = %X\n", LibStatus));
      }
    }
  }

  ///
  /// Initializes Dynamic Efficiency Control feature.
  ///
  InitDynamicEfficiencyControl ();

  return;
}

/**
  Initializes P States and Turbo Power management features
**/
VOID
InitPStates (
  VOID
  )
{
  MSR_MISC_ENABLES_REGISTER      MiscEnableMsr;

  DEBUG ((DEBUG_INFO, "InitPStates Start\n"));

  //
  // InitTurboRatioLimits has to be called before InitFvidTable as InitFvidTable uses the Turbo Ratio Limit programmed.
  //
  InitTurboRatioLimits (); ///< Initialize InitTurboRatioLimits

  InitEnergyEfficientPState (); ///< Initialize Energy Efficient P-state

  InitEnergyEfficientTurbo (); ///< Initialize Energy Efficient Turbo

  InitRaceToHalt (); ///< Initialize Race To Halt feature

  //
  // Initialize P states
  //
  if (mPpmFlags & PPM_EIST) {
    ///
    /// Initialize the FVID tables.
    ///
    if (mMaxBusRatio != mMinBusRatio) {
      InitFvidTable (mFvidPointer, TPSS_FVID_MAX_STATES);
      ASSERT (mFvidPointer->FvidHeader.EistStates != 0);

      InitEist ();
    } else {
      DEBUG ((DEBUG_WARN, "CPU MaxBusRatio is equal to MinBusRatio, InitEist() is skipped!\n"));
    }
  } else {
    ///
    /// Clear EIST bit in IA32 Misc Enable MSR that was initially set earilier in PEI
    ///
    MiscEnableMsr.Uint64 = AsmReadMsr64 (MSR_MISC_ENABLES);
    MiscEnableMsr.Bits.EnableGv3 = 0;

    ///
    /// Disable Turbo if EIST and HwP are both disabled
    ///
    if ((mPpmFlags & PPM_TURBO) && !(mPpmFlags & PPM_HWP)) {
      MiscEnableMsr.Bits.TurboModeDisable = 1;
    }
    AsmWriteMsr64 (MSR_MISC_ENABLES, MiscEnableMsr.Uint64);
  }

  ///
  /// Initialize Power Aware Interrupt Routing (PAIR) Configuration
  ///
  InitPpmIrmConfiguration ();
  DEBUG ((DEBUG_INFO, "InitPStates End\n"));

}

/**
  Initializes Resource Priority feature.
**/
VOID
InitializeRpFeature (
  VOID
  )
{
  BOOLEAN     IsBsp;
  EFI_STATUS  Status;

  if (gCpuPowerMgmtBasicConfig->EnableRp == 1) {
    IsBsp = TRUE;
    ProgramRpRecipe ((VOID *) &IsBsp);
    IsBsp = FALSE;

    Status = mMpServices2Ppi->StartupAllAPs (
                               mMpServices2Ppi,
                               (EFI_AP_PROCEDURE) ProgramRpRecipe,
                               FALSE,
                               0,
                               (VOID *) &IsBsp
                               );
    if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
      DEBUG ((DEBUG_ERROR, "%a: StartupAllAps(): %r\n", __func__, Status));
    }
  }
  return;
}