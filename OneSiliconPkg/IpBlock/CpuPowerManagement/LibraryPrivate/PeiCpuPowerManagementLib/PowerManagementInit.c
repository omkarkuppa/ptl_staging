/** @file
  Processor Power Management initialization code. This code determines current
  user configuration and modifies and loads ASL as well as initializing chipset
  and processor features to enable the proper power management.

  Acronyms:
    PPM - Processor Power Management
    TM  - Thermal Monitor
    IST - Intel(R) Speedstep technology
    HT  - Hyper-Threading Technology
   ITBM - Intel(R) Turbo Boost Max Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include "PowerMgmtInit.h"
#include <Library/PciSegmentLib.h>
#include <Library/ConfigBlockLib.h>
#include <IntelRcStatusCode.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/CpuPowerManagementLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

//
// Global variables
//

///
/// Power Managment policy configurations
///
SI_POLICY_PPI                          *gSiCpuPolicy                  = NULL;
SI_PREMEM_POLICY_PPI                   *gSiPreMemPolicy               = NULL;
CPU_POWER_MGMT_BASIC_CONFIG            *gCpuPowerMgmtBasicConfig      = NULL;
CPU_POWER_MGMT_CUSTOM_CONFIG           *gCpuPowerMgmtCustomConfig     = NULL;
CPU_POWER_MGMT_TEST_CONFIG             *gCpuPowerMgmtTestConfig       = NULL;
CPU_POWER_MANAGEMENT_DATA_HOB          *mCpuPowerMgmtDataHob          = NULL;

//
// Values for FVID table calculate.
//
UINT32 mPpmFlags                  = 0;
UINT16 mTurboBusRatio             = 0;
UINT8 mMaxBusRatio                = 0;
UINT8 mMinBusRatio                = 0;
UINT16 mProcessorFlavor           = 0;
UINT16 mPackageTdpWatt            = 0;                        ///< Processor TDP value in Watts.
UINT8 mConfigurablePpc;


UINT8 mRatioLimitProgrammble      = 0; ///< Programmable Ratio Limit

//
// FVID Table Information
// Default FVID table
// One header field plus states
//
FVID_TABLE *mFvidPointer              = NULL;

/**
  Initialize CPU power management private Data Hob
**/
VOID
EFIAPI
InitializeCpuPowerMgmtPrivateDataHob (
  VOID
  )
{
  CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB  *CpuPowerMgmtPrivateDataHob;

  CpuPowerMgmtPrivateDataHob = NULL;

  DEBUG ((DEBUG_INFO, "InitializeCpuPowerMgmtPrivateDataHob Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PVTDATAHOB_START); //PostCode (0xC72)

  CpuPowerMgmtPrivateDataHob = BuildGuidHob (
                                  &gCpuPowerManagementPrivateDataHobGuid,
                                  (UINTN) sizeof (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB)
                                  );
  ASSERT (CpuPowerMgmtPrivateDataHob != NULL);

  ZeroMem (CpuPowerMgmtPrivateDataHob, sizeof (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB));

  DEBUG ((DEBUG_INFO, "InitializeCpuPowerMgmtPrivateDataHob Done\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PVTDATAHOB_DONE); //PostCode (0xC73)

  return ;
}

/**
  Initialize CPU Power Management Data Hob

  @retval EFI_SUCCESS           The driver installed/initialized correctly.
  @retval EFI_OUT_OF_RESOURCES  Allocation of the hob failed.
**/
VOID
InitializeCpuPowerMgmtDataHob (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "InitializeCpuPowerMgmtDataHob Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PWRMGMT_DATAHOB_START); //PostCode (0xC74)

  mCpuPowerMgmtDataHob = BuildGuidHob (
                           &gCpuPowerManagementDataHobGuid,
                           (UINTN) sizeof (CPU_POWER_MANAGEMENT_DATA_HOB)
                           );
  ASSERT (mCpuPowerMgmtDataHob != NULL);

  if (mCpuPowerMgmtDataHob == NULL) {
    return;
  }
  ZeroMem (mCpuPowerMgmtDataHob, sizeof (CPU_POWER_MANAGEMENT_DATA_HOB));

  DEBUG ((DEBUG_INFO, "InitializeCpuPowerMgmtDataHob Done \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PWRMGMT_DATAHOB_DONE); //PostCode (0xC75)

  return ;
}

/**
  Power Management init after memory PEI module

  @param[in] CpuPowerDeliveryConfig  Pointer to CPU_POWER_DELIVERY_CONFIG

  @retval EFI_SUCCESS     The driver installes/initialized correctly.
  @retval Driver will ASSERT in debug builds on error.  PPM functionality is considered critical for mobile systems.
**/
EFI_STATUS
EFIAPI
CpuPowerMgmtInit (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  EFI_STATUS                            Status;
  VOID                                  *Hob;
  CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *CpuPowerMgmtPrivateDataHob;

  DEBUG ((DEBUG_INFO, "CpuPowerMgmtInit Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PWRMGMTINIT_START); //PostCode (0xC70)

  DEBUG ((DEBUG_INFO, " PeimInitializePowerMgmt Entry\n"));

  ///
  /// Locate platform configuration information
  ///
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &gSiCpuPolicy);
  ASSERT_EFI_ERROR (Status);

    Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &gSiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize the Global pointer for Power Managment Policy
  ///
  Status = GetConfigBlock ((VOID *) gSiCpuPolicy, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &gCpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) gSiCpuPolicy, &gCpuPowerMgmtCustomConfigGuid, (VOID *) &gCpuPowerMgmtCustomConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) gSiCpuPolicy, &gCpuPowerMgmtTestConfigGuid, (VOID *) &gCpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Locate MpService Ppi
  ///
  Status = PeiServicesLocatePpi (
             &gEdkiiPeiMpServices2PpiGuid,
             0,
             NULL,
             (VOID **) &mMpServices2Ppi);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize Power management Global variables
  ///
  InitPowerManagementGlobalVariables (CpuPowerDeliveryConfig);

  ///
  /// Get CPU power management private data hob
  ///
  Hob = GetFirstGuidHob (&gCpuPowerManagementPrivateDataHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "CPU power management private data hob not available\n"));
    ASSERT (Hob != NULL);
    return EFI_NOT_FOUND;
  }

  CpuPowerMgmtPrivateDataHob = (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *) GET_GUID_HOB_DATA (Hob);

  ///
  /// Initialize auto thermal reporting when EnableAllThermalFunctions is enabled from BIOS setup.
  ///
  CpuPowerMgmtPrivateDataHob->EnableAllThermalFunctions = (UINT8) gCpuPowerMgmtTestConfig->EnableAllThermalFunctions;

  ///
  /// Initialize CPU Power Management Data Hob
  ///
  InitializeCpuPowerMgmtDataHob ();

  ///
  /// Initialize FVID table pointer
  ///
  DEBUG((DEBUG_INFO, "Initialize FVID table pointer\n"));
  mFvidPointer = (FVID_TABLE *) &(CpuPowerMgmtPrivateDataHob->FvidTable);

  ///
  /// Initialize CPU Power management code (determine HW and configured state, configure hardware and software accordingly)
  ///
  Status = InitPpm ();
  ASSERT_EFI_ERROR (Status);
  DEBUG((DEBUG_INFO, "CpuPowerMgmtInit Done \n"));
  DEBUG((DEBUG_INFO, "CPU Post-Mem Exit \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_POST_MEM_EXIT); //PostCode (0xC7F)

  return EFI_SUCCESS;
}

/**
  Initializes the platform power management global variable.
  This must be called prior to any of the functions being used.

  @param[in] CpuPowerDeliveryConfig  Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
InitPowerManagementGlobalVariables (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  MSR_PACKAGE_POWER_SKU_REGISTER        PackagePowerSkuMsr;

  ///
  /// Get Platform ID by reading System Agent's Device ID (B0:D0:F0:R02)
  ///
  mProcessorFlavor = (UINT16) GetHostBridgeRegisterData (HostBridgeDeviceId, HostBridgeDeviceIdData);

  ///
  /// Get Maximum Non-Turbo bus ratio (HFM) from Platform Info MSR Bits[15:8]
  /// Get Maximum Efficiency bus ratio (LFM) from Platform Info MSR Bits[47:40]
  ///
  GetBusRatio (&mMaxBusRatio, &mMinBusRatio);

  mTurboBusRatio = GetMaxTurboRatio ();

  ///
  /// Check if Turbo Ratio Limit is programmable
  ///  Platform Info MSR (0xCE) [28]
  ///
  mRatioLimitProgrammble = (UINT8) IsCoreRatioLimitSupported ();

  ///
  /// Get Processor TDP
  /// Get Maximum Power from Turbo Power Limit MSR Bits[46:32]
  /// and convert it to units specified by Package Power SKU
  /// Unit MSR [3:0]
  ///
  PackagePowerSkuMsr.Uint64     = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  mPackageTdpWatt = (UINT16) DivU64x32 ((UINT16) PackagePowerSkuMsr.Bits.PkgTdp , GetPowerUnit ());

  ///
  /// Initialize flags based on processor capablities
  ///
  SetPpmFlags ();

  ///
  /// Determine current user configuration
  ///
  SetUserConfigurationPpmFlags (CpuPowerDeliveryConfig);

  return;
}

/**
  Set the PPM flags
**/
VOID
SetPpmFlags (
  VOID
  )
{
  MSR_MISC_ENABLES_REGISTER          MiscEnable;
  MSR_FLEX_RATIO_REGISTER            FlexRatioMsr;
  CPUID_VERSION_INFO_ECX             VersionInfoEcx;
  CPUID_VERSION_INFO_EDX             VersionInfoEdx;
  CPUID_MONITOR_MWAIT_ECX            MonitorMwaitEcx;
  CPUID_MONITOR_MWAIT_EDX            MonitorMwaitEdx;
  CPUID_THERMAL_POWER_MANAGEMENT_EAX PowerEax;
  UINT8                              OverclockingBins;
  UINTN                              States;
  BOOLEAN                            CpuidLimitingEnabled;
  UINT32                             PpmFlags;

  PpmFlags = 0;

  ///
  /// Check if the processor has multiple cores.
  ///
  if (GetEnabledCoreCount() > 1) {
    PpmFlags |= PPM_CMP;
  }

  ///
  /// Disable CPUID limiting (and save current setting) if enabled
  /// and enable MONITOR/MWAIT support
  ///
  MiscEnable.Uint64 = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);
  CpuidLimitingEnabled = (BOOLEAN) MiscEnable.Bits.BootNt4;
  if (CpuidLimitingEnabled) {
    MiscEnable.Bits.BootNt4 = 0;
    AsmWriteMsr64 (MSR_IA32_MISC_ENABLE, MiscEnable.Uint64);
  }

  ///
  /// Read the CPUID values we care about. To get the correct MONITOR/MWAIT-related values,
  /// we need to read after we have disabled limiting and enabled MONITOR/MWAIT
  ///
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &VersionInfoEcx.Uint32, &VersionInfoEdx.Uint32);
  AsmCpuid (CPUID_MONITOR_MWAIT, NULL, NULL, &MonitorMwaitEcx.Uint32, &MonitorMwaitEdx.Uint32);
  AsmCpuid (CPUID_THERMAL_POWER_MANAGEMENT, &PowerEax.Uint32, NULL, NULL, NULL);

  ///
  /// Check Thermal Monitor capable and update the flag.
  ///
  if (VersionInfoEdx.Bits.TM == 1) {
    PpmFlags |= PPM_TM;
  }

  ///
  /// Check Enhanced Intel SpeedStep(R) technology capable.
  ///
  if (VersionInfoEcx.Bits.EIST == 1) {
    PpmFlags |= (PPM_EIST);
    DEBUG ((DEBUG_INFO, "EIST capable\n"));
  }

  ///
  /// Determine if the MONITOR/MWAIT instructions are supported.
  ///
  if ((VersionInfoEcx.Bits.MONITOR == 1) && (MonitorMwaitEcx.Bits.ExtensionsSupported == 1)) {
    PpmFlags |= PPM_MWAIT_EXT;
  }

  ///
  /// Determine the C-State and Enhanced C-State support present.
  /// Monitor/MWAIT parameters function describes the numbers supported.
  ///
  States = MonitorMwaitEdx.Bits.C1States;
  if (States == CSTATE_SUPPORTED) {
    PpmFlags |= PPM_C1;
  }

  States = MonitorMwaitEdx.Bits.C3States;
  if (States >= C6_C7_LONG_LATENCY_SUPPORTED) { // Both Long and Short Latency C6 supported
    PpmFlags |= (PPM_C6 | C6_LONG_LATENCY_ENABLE);
  } else if (States >= C6_C7_SHORT_LATENCY_SUPPORTED) { // Only Short Latency C6 supported.
    PpmFlags |= PPM_C6;
  }

  States = MonitorMwaitEdx.Bits.C4States;
  switch (States) {
    case C7s_LONG_LATENCY_SUPPORTED:
      //
      // C7 & C7s Long and Short supported
      //
      PpmFlags |= (PPM_C7S | C7s_LONG_LATENCY_ENABLE | PPM_C7 | C7_LONG_LATENCY_ENABLE);
      break;
    case C7s_SHORT_LATENCY_SUPPORTED:
      //
      // C7s Long Latency is not supported.
      //
      PpmFlags |= (PPM_C7S | PPM_C7 | C7_LONG_LATENCY_ENABLE);
      break;
    case C6_C7_LONG_LATENCY_SUPPORTED:
      //
      // C7 Long and Short supported
      //
      PpmFlags |= (PPM_C7 | C7_LONG_LATENCY_ENABLE);
      break;
    case C6_C7_SHORT_LATENCY_SUPPORTED:
      //
      // C7 Long Latency is not supported.
      //
      PpmFlags |= PPM_C7;
      break;
    default:
      break;
  }

  States = MonitorMwaitEdx.Bits.C5States;
  if (States >= CSTATE_SUPPORTED) {
    PpmFlags |= PPM_C8;
  }
  States = MonitorMwaitEdx.Bits.C6States;
  if (States >= CSTATE_SUPPORTED) {
    PpmFlags |= PPM_C9;
  }
  States = MonitorMwaitEdx.Bits.C7States;
  if (States >= CSTATE_SUPPORTED) {
    PpmFlags |= PPM_C10;
  }

  ///
  /// Check if TimedMwait is supported and update the flag
  ///
  if (IsTimedMwaitSupported ()) {
    PpmFlags |= PPM_TIMED_MWAIT;
  }
  if (PpmFlags & (PPM_C8 |PPM_C9 | PPM_C10)) {
    PpmFlags |= PPM_CD;
  }

  ///
  /// Check if turbo mode is supported and update the flag
  ///
  if ((PowerEax.Bits.TurboBoostTechnology == 0) &&
      (MiscEnable.Bits.TurboModeDisable == 0)
      ) {
    ///
    /// Turbo Mode is not available in this physical processor package.
    /// BIOS should not attempt to enable Turbo Mode via IA32_MISC_ENABLE MSR.
    /// BIOS should show Turbo Mode as Disabled and Not Configurable.
    ///
  } else if (PowerEax.Bits.TurboBoostTechnology == 0) {
    ///
    /// Turbo Mode is available but globally disabled for the all logical
    /// processors in this processor package.
    /// BIOS can enable Turbo Mode by IA32_MISC_ENABLE MSR 1A0h bit [38] = 0.
    ///
    PpmFlags |= PPM_TURBO;
  } else if (PowerEax.Bits.TurboBoostTechnology == 1) {
    ///
    /// Turbo Mode is factory-configured as available and enabled for all logical processors in this processor package.
    /// This case handles the cases where turbo mode is enabled before PPM gets chance to enable it
    ///
    PpmFlags |= PPM_TURBO;
  }

  ///
  /// Restore the CPUID limit setting.
  ///
  if (CpuidLimitingEnabled) {
    MiscEnable.Bits.BootNt4 = 1;
    AsmWriteMsr64 (MSR_IA32_MISC_ENABLE, MiscEnable.Uint64);
  }

  ///
  /// Set the T-states flag
  ///
  PpmFlags |= PPM_TSTATES;

  ///
  /// Determine if Clock modulation duty cycle extension is supported
  ///
  if (PowerEax.Bits.ECMD == 1) {
    PpmFlags |= PPM_TSTATE_FINE_GRAINED;
  }
  PpmFlags |= PPM_EEPST;                                ///< Energy Efficient P-state feature is supported

  ///
  /// Check HWP support
  ///
  if (PowerEax.Bits.HWP == 1) {
    PpmFlags |= PPM_HWP;
    DEBUG ((DEBUG_INFO, "HWP capable\n"));
  }

  ///
  /// Check if overclocking is fully unlocked
  ///
  FlexRatioMsr.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
  OverclockingBins = (UINT8) FlexRatioMsr.Bits.OcBins;
  if (OverclockingBins == MAX_OVERCLOCKING_BINS) {
    PpmFlags |= PPM_OC_UNLOCKED;
  }

  ///
  /// Check Intel Turbo Boost Max Technology support.
  ///
  if (IsItbmSupported ()) {
    PpmFlags |= PPM_TURBO_BOOST_MAX;
    DEBUG ((DEBUG_INFO, "Itbm: Intel Turbo Boost Max Technology supported\n"));
  }

  mPpmFlags = PpmFlags;
  return;
}

/**
  Set the PPM flags based on current user configuration

  @param[in] CpuPowerDeliveryConfig  Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
SetUserConfigurationPpmFlags (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  UINT32 UserPpmFlag;

  //
  // In advance to clear following PPM flags which are related with policies that user can enabled/disabled.
  //
  UserPpmFlag = (UINT32) ~(PPM_EIST | PPM_C1 | PPM_TM | PPM_TURBO | PPM_TSTATES |
                           PPM_TSTATE_FINE_GRAINED | PPM_EEPST | PPM_TIMED_MWAIT | PPM_HWP | PPM_OC_UNLOCKED);
  ///
  /// Configure flag based on user selections
  ///
  if (gCpuPowerMgmtTestConfig->Eist) {
    UserPpmFlag |= PPM_EIST;
  }
  if (gCpuPowerMgmtTestConfig->Cx) {
    UserPpmFlag |= PPM_C1;
  } else {
    UserPpmFlag &= ~( PPM_C6 | C6_LONG_LATENCY_ENABLE |
                     PPM_C7S | PPM_C7 | C7_LONG_LATENCY_ENABLE | C7s_LONG_LATENCY_ENABLE | PPM_CD |
                     PPM_C8 | PPM_C9 | PPM_C10);
  }
  if (CpuPowerDeliveryConfig->ThermalMonitor) {
    UserPpmFlag |= PPM_TM;
  }
  if (gCpuPowerMgmtBasicConfig->TurboMode) {
    UserPpmFlag |= PPM_TURBO;
  }
  if (gCpuPowerMgmtTestConfig->TStates) {
    UserPpmFlag |= (PPM_TSTATES | PPM_TSTATE_FINE_GRAINED);
  }
  if (gCpuPowerMgmtTestConfig->EnergyEfficientPState) {
    UserPpmFlag |= PPM_EEPST;
  }
  if (gCpuPowerMgmtTestConfig->TimedMwait) {
    UserPpmFlag |= PPM_TIMED_MWAIT;
  }
  if (gCpuPowerMgmtBasicConfig->Hwp) {
    UserPpmFlag |= PPM_HWP;
  }
  if (IsOcEnabled ()) {
    UserPpmFlag |= PPM_OC_UNLOCKED;
  }
  mPpmFlags &= UserPpmFlag;

}

/**
  Initialize Ctdp related NVS Variables in Hob

  @param[in] ApplyConfigTdp             If apply ConfigTdp
  @param[in] ConfigTdpPowerLimit1       ConfigTdpPowerLimit1
  @param[in] ConfigTdpPowerLimit2       ConfigTdpPowerLimit2
  @param[in] ConfigTdpPowerLimitWindow  ConfigTdpPowerLimitWindow
  @param[in] ConfigTdpTar               ConfigTdpTar
  @param[in] ConfigTdpBootModeIndex     ConfigTdpBootModeIndex
**/
VOID
InitCpuCtdpNvsDataHob (
  IN BOOLEAN                   ApplyConfigTdp,
  IN UINT16                    ConfigTdpPowerLimit1,
  IN UINT16                    ConfigTdpPowerLimit2,
  IN UINT8                     ConfigTdpPowerLimitWindow,
  IN UINT8                     ConfigTdpTar,
  IN UINT8                     ConfigTdpBootModeIndex,
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  UINT8                                 CtdpLevelsSupported;
  VOID                                  *Hob;
  CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *CpuPowerMgmtPrivateDataHob;

  ///
  /// Get CPU power management private data hob
  ///
  Hob = GetFirstGuidHob (&gCpuPowerManagementPrivateDataHobGuid);
  ASSERT (Hob != NULL);

  CpuPowerMgmtPrivateDataHob = (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *) GET_GUID_HOB_DATA (Hob);

  CtdpLevelsSupported = GetConfigTdpLevels () + 1;
  if (!ApplyConfigTdp) {
    CtdpLevelsSupported = 1;
  }

  CpuPowerMgmtPrivateDataHob->ThETAIbattEnable = CpuPowerDeliveryConfig->ThETAIbattEnable;
  CpuPowerMgmtPrivateDataHob->IsysLimitL1Enable  = CpuPowerDeliveryConfig->IsysCurrentLimitL1Enable;
  CpuPowerMgmtPrivateDataHob->IsysLimitL2Enable  = CpuPowerDeliveryConfig->IsysCurrentLimitL2Enable;

  CpuPowerMgmtPrivateDataHob->ConfigTdpBootModeIndex = ConfigTdpBootModeIndex;                    /// CTDP Boot Mode Index
  CpuPowerMgmtPrivateDataHob->CtdpLevelsSupported = CtdpLevelsSupported;                          /// ConfigTdp Number Of Levels
  CpuPowerMgmtPrivateDataHob->ApplyConfigTdp = (UINT8) ApplyConfigTdp;                            /// Enable Configurable TDP

  CpuPowerMgmtPrivateDataHob->CtdpTar = ConfigTdpTar;                           /// CTDP active level TAR
  CpuPowerMgmtPrivateDataHob->CtdpPowerLimitWindow = ConfigTdpPowerLimitWindow; /// CTDP active level Power Limits Time Window
  CpuPowerMgmtPrivateDataHob->CtdpPowerLimit1= ConfigTdpPowerLimit1;            /// CTDP active level Power Limit1
  CpuPowerMgmtPrivateDataHob->CtdpPowerLimit2= ConfigTdpPowerLimit2;            /// CTDP active level Power Limit2
}

/**
  Update CPU NVS Variables in Hob.
**/
VOID
UpdateCpuNvsDataHob (
  VOID
  )
{
  VOID                                  *Hob;
  VOID                                  *CpuPowerDataHob;
  CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *CpuPowerMgmtPrivateDataHob;
  CPU_POWER_MANAGEMENT_DATA_HOB         *CpuPowerMgmtDataHob;
  ///
  /// Get CPU power management private data hob
  ///
  Hob = GetFirstGuidHob (&gCpuPowerManagementPrivateDataHobGuid);
  ASSERT (Hob != NULL);
  CpuPowerMgmtPrivateDataHob = (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *) GET_GUID_HOB_DATA (Hob);

  ///
  /// Get CPU Power Management Data Hob
  ///
  CpuPowerDataHob = GetFirstGuidHob (&gCpuPowerManagementDataHobGuid);
  ASSERT (CpuPowerDataHob != NULL);
  CpuPowerMgmtDataHob = (CPU_POWER_MANAGEMENT_DATA_HOB *) GET_GUID_HOB_DATA (CpuPowerDataHob);

  CpuPowerMgmtPrivateDataHob->ConfigurablePpc = mConfigurablePpc;                                 /// Boot Mode vlues for _PPC
  CpuPowerMgmtPrivateDataHob->PpmFlags = mPpmFlags;
  CpuPowerMgmtDataHob->EnableRp = gCpuPowerMgmtBasicConfig->EnableRp;                             /// Rp Enable/Disable
}

/**
  Initialize the processor power management based on hardware capabilities
  and user configuration settings.

  @retval EFI_SUCCESS - on success
  @retval Appropiate failure code on error
**/
EFI_STATUS
InitPpm (
  VOID
  )
{
  EFI_STATUS                            Status;
  BOOLEAN                               ApplyConfigTdp;
  UINT16                                NumberOfStates;
  UINT8                                 ConfigTdpTar;
  VOID                                  *Hob;
  CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *CpuPowerMgmtPrivateDataHob;

  Status         = EFI_SUCCESS;
  ApplyConfigTdp = FALSE;

  DEBUG((DEBUG_INFO, "InitPpm Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PPM_INIT_START); //PostCode (0xC71)

  ///
  /// Get CPU power management private data hob
  ///
  Hob = GetFirstGuidHob (&gCpuPowerManagementPrivateDataHobGuid);
  ASSERT (Hob != NULL);
  CpuPowerMgmtPrivateDataHob = (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *) GET_GUID_HOB_DATA (Hob);

  //
  // Get ApplyConfigTdp and ConfigTdpTar via hob
  //
  ApplyConfigTdp = CpuPowerMgmtPrivateDataHob->ApplyConfigTdp;

  ConfigTdpTar = CpuPowerMgmtPrivateDataHob->CtdpTar;


  ///
  /// In case of LFM == TDP active Ratio, consider TDP active TAR as the new LFM to insert fake P state.
  ///
  if (mMinBusRatio == ConfigTdpTar + 1) {
    mMinBusRatio = ConfigTdpTar;
    DEBUG ((DEBUG_INFO, "PPM:: mMinBusRatio Modified for Ctdp %d\n", mMinBusRatio));
  }

  ///
  /// Initialize P states
  ///
  InitPStates ();

  ///
  ///Initializes Resource Priority feature.
  ///
  InitializeRpFeature ();

  ///
  /// Initialize Hwp
  ///
  InitializeHwp ();

  ///
  /// Initialize Hwp misc features
  ///
  InitializeHwpMiscFeatures ();

  ///
  /// Initialize C State (IdleStates)
  ///
  InitCState ();

  //
  // Dump FVID table for LPSS and TPSS
  //
  NumberOfStates = mFvidPointer[0].FvidHeader.EistStates;

  //
  // Dump FVID table for LPSS and TPSS
  //
  DumpFvidTable (mFvidPointer, LPSS_FVID_MAX_STATES, NumberOfStates);

  if (ApplyConfigTdp) {
    CtdpPatchFvidTable (mFvidPointer, ConfigTdpTar);
    CtdpPatchFvidTableforLimit16Pstate (mFvidPointer, ConfigTdpTar);

    //
    // Print Ctdp TAR and PPC patch table
    //
    DEBUG ((DEBUG_INFO, "CPU Ctdp is supported\n"));
    DEBUG ((DEBUG_INFO, " Ratio\t:%d\n", ConfigTdpTar));
    DEBUG ((DEBUG_INFO, " ConfigurablePpc: \t%d\n", mConfigurablePpc));
    DumpFvidTable (mFvidPointer, LPSS_FVID_MAX_STATES, NumberOfStates);
  }
  ///
  /// This will sync thermal features from BSP to all logical processors.
  ///
  SyncThermal ();

  ///
  /// Lock down all settings
  ///
  PpmLockDown ();

  ///
  /// Update CPU NVS Variables in Hob
  ///
  UpdateCpuNvsDataHob ();

  DEBUG((DEBUG_INFO, "InitPpm Done \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PPM_INIT_DONE); //PostCode (0xC76)

  return Status;
}
