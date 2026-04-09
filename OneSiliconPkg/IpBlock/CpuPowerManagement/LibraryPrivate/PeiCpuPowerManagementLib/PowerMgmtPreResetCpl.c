/** @file
  Power Management EarlyPost initializations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/CpuPlatformLib.h>
#include <IntelRcStatusCode.h>
#include <Ppi/SiPolicy.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/PeiMeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/B2pMailbox.h>

/**
  Set processor P state based on input parameter.

  @param[in out] Buffer - Pointer to UINT8 BootRatio parameter.
**/
VOID
EFIAPI
ApSafeSetBootPState (
  IN OUT VOID *Buffer
  )
{
  UINT8                 BootRatio;
  MSR_PERF_CTL_REGISTER Ia32PerfCtl;

  ///
  /// Extract the boot ratio from the buffer.
  ///
  BootRatio = *((UINT8 *) Buffer);
  //
  // Same MSR offset definition MSR_IA32_PERF_CTL 0x00000199 in ArchitecturalMsr.h
  //
  Ia32PerfCtl.Uint64 = AsmReadMsr64 (MSR_PERF_CTL);
  Ia32PerfCtl.Bits.PReq = BootRatio;
  AsmWriteMsr64 (MSR_PERF_CTL, Ia32PerfCtl.Uint64);

  return;
}

/**
  Set the Boot Frequency across all logical processors.

  @param[in] SiPolicyPpi             The SI Policy PPI instance.
  @param[in] CpuPowerDeliveryConfig  Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
SetBootFrequency (
  IN SI_POLICY_PPI             *SiPolicyPpi,
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  UINT8                                       BootRatio;
  MSR_MISC_ENABLES_REGISTER                   MiscEnable;
  MSR_TURBO_RATIO_LIMIT_REGISTER              TurboRatioLimit;
  MSR_CONFIG_TDP_NOMINAL_REGISTER             TdpNominal;
  MSR_CONFIG_TDP_LEVEL2_REGISTER              TdpLevelx;
  UINT8                                       MaxBusRatio;
  UINT8                                       MinBusRatio;
  UINT8                                       BootFrequencyMode;
  CPUID_THERMAL_POWER_MANAGEMENT_EAX          Eax;
  UINT8                                       NumberOfConfigTdpLevels;
  UINT8                                       SupportedCtdpLevels;
  UINT8                                       SelectedCtdpLevel;
  UINT8                                       CtdpTarRatioPlusOne;
  CPU_POWER_MGMT_BASIC_CONFIG                 *CpuPowerMgmtBasicConfig;
  CPU_POWER_MGMT_TEST_CONFIG                  *CpuPowerMgmtTestConfig;
  EFI_STATUS                                  Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "SetBootFrequency Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_SETBOOTFREQUENCY_START); //PostCode (0xC33)

  ///
  /// Get Maximum Non-Turbo bus ratio (HFM) from Platform Info MSR Bits[15:8]
  /// Get Maximum Efficiency bus ratio (LFM) from Platform Info MSR Bits[47:40]
  ///
  GetBusRatio (&MaxBusRatio, &MinBusRatio);

  BootRatio = MinBusRatio;
  BootFrequencyMode = (UINT8) CpuPowerMgmtBasicConfig->BootFrequency;

  ///
  /// Temporarily enable EIST on BSP.
  ///
  //
  // Same MSR offset defintion MSR_IA32_MISC_ENABLE 0x000001A0 in ArchitecturalMsr.h
  //
  MiscEnable.Uint64 = AsmReadMsr64 (MSR_MISC_ENABLES);
  MiscEnable.Bits.EnableGv3 = 1;
  AsmWriteMsr64 (MSR_MISC_ENABLES, MiscEnable.Uint64);

  if (CpuPowerMgmtBasicConfig->BootFrequency == 2) {
    if (CpuPowerMgmtBasicConfig->TurboMode && (CpuPowerMgmtTestConfig->Eist || CpuPowerMgmtBasicConfig->Hwp)) {
      //
      // Set processor P state as TURBO_RATIO_LIMIT_1C if available
      //
      AsmCpuid (CPUID_THERMAL_POWER_MANAGEMENT, &Eax.Uint32, NULL, NULL, NULL);
      if ((Eax.Bits.TurboBoostTechnology == 1) ||
          (MiscEnable.Bits.TurboModeDisable)) {
        //
        // Enable Turbo
        //
        MiscEnable.Bits.TurboModeDisable = 0;
        AsmWriteMsr64 (MSR_MISC_ENABLES, MiscEnable.Uint64);
        //
        // Set Boot Ratio to TURBO_RATIO_LIMIT_1C
        //
        TurboRatioLimit.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT);
        BootRatio = (UINT8) TurboRatioLimit.Bits.RatioLimit0;
      } else {
        //
        // Turbo is not available, down to HFM
        //
        DEBUG ((DEBUG_INFO, "CPU: Turbo mode is not available, down to HFM mode.\n"));
        BootFrequencyMode = 1;
        BootRatio = MaxBusRatio;
      }
    } else {
      //
      // Turbo is disabled, down to HFM
      //
      BootFrequencyMode = 1;
      BootRatio = MaxBusRatio;
    }
  } else if (CpuPowerMgmtBasicConfig->BootFrequency == 1) {
    BootRatio = MaxBusRatio;
  } else {
    BootRatio = MinBusRatio;
  }

  ///
  /// If there are any configurable TDP Levels supported, set the boot ratio
  /// based on BootFrequencyMode and cTDP TAR ratio.
  ///
  NumberOfConfigTdpLevels = GetConfigTdpLevels ();
  if (NumberOfConfigTdpLevels != 0) {
    //
    // Find the selected level and the TAR ratio.
    //
    SupportedCtdpLevels = NumberOfConfigTdpLevels + 1;
    SelectedCtdpLevel = (UINT8) CpuPowerDeliveryConfig->ConfigTdpLevel;

    //
    // Select cTDP Nominal if cTDP is disabled or the level is not supported.
    //
    if (SelectedCtdpLevel == CONFIG_TDP_DEACTIVATE || SelectedCtdpLevel >= SupportedCtdpLevels) {
      SelectedCtdpLevel = 0;
    }

    if (SelectedCtdpLevel >= 3) {
      DEBUG ((DEBUG_ERROR, "ERROR: Invalid SelectedCtdpLevel=%x, and SelectedCtdpLevel should be range in 0-2 \n", SelectedCtdpLevel));
      ASSERT (FALSE);
      return;
    }

    if (CpuPowerDeliveryConfig->CustomTurboActivationRatio != 0) {
      CtdpTarRatioPlusOne = (UINT8) CpuPowerDeliveryConfig->CustomTurboActivationRatio + 1;
    } else {
      if (SelectedCtdpLevel == 0) {
        TdpNominal.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_NOMINAL);
        CtdpTarRatioPlusOne = (UINT8) TdpNominal.Bits.TdpRatio;
      } else if (SelectedCtdpLevel == 1) {
        TdpLevelx.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL1);
        if (TdpLevelx.Bits.TdpRatio == 0) {
          //
          // If Level1 was not valid, then read Level2 register
          //
          TdpLevelx.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL2);
          CtdpTarRatioPlusOne = (UINT8) TdpLevelx.Bits.TdpRatio;
        }
        CtdpTarRatioPlusOne = (UINT8) TdpLevelx.Bits.TdpRatio;
      } else {
        TdpLevelx.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL2);
        CtdpTarRatioPlusOne = (UINT8) TdpLevelx.Bits.TdpRatio;
      }
    }

    //
    // Make sure the TAR+1 ratio is between the fused min and (max+1) ratios.
    //
    if (CtdpTarRatioPlusOne < MinBusRatio) {
      CtdpTarRatioPlusOne = MinBusRatio;
    } else if (CtdpTarRatioPlusOne > MaxBusRatio + 1) {
      CtdpTarRatioPlusOne = MaxBusRatio + 1;
    }

    if (BootFrequencyMode == 2) {
      //
      // Use TAR Ratio + 1 to go to Turbo mode once ConfigTDP is fully configured and the TAR ratio MSR is configured.
      //
      BootRatio = CtdpTarRatioPlusOne;
    } else if (BootFrequencyMode == 1) {
      //
      // Use TAR Ratio as max non-turbo ratio, which is equal to ConfigTDP boot ratio - 1.
      //
      BootRatio = CtdpTarRatioPlusOne - 1;

      //
      // If EIST is disabled use ConfigTDP boot ratio, which is equal to TAR+1.
      //
      if (!CpuPowerMgmtTestConfig->Eist) {
        BootRatio = BootRatio + 1;
      }
    } else {
      //
      // Use MinBusRatio (LFM) unless ConfigTDP boot ratio is equal to LFM.
      // In that case, use 'fake' P-State of ConfigTDP boot ratio - 1, which is equal to TAR ratio.
      //
      BootRatio = MinBusRatio;
      if ((CtdpTarRatioPlusOne == MinBusRatio) && CpuPowerMgmtTestConfig->Eist) {
        BootRatio = CtdpTarRatioPlusOne - 1;
      }
    }
  }

  ///
  /// Set MSR_IA32_PERF_CTL on BSP.
  ///
  ApSafeSetBootPState (&BootRatio);

  ///
  /// Set MSR_IA32_PERF_CTL on all APs unless disabled by policy setting.
  ///
  if (CpuPowerMgmtBasicConfig->SkipSetBootPState != CPU_FEATURE_ENABLE) {
    Status = mMpServices2Ppi->StartupAllAPs (
                               mMpServices2Ppi,
                               (EFI_AP_PROCEDURE) ApSafeSetBootPState,
                               FALSE,
                               0,
                               &BootRatio
                               );
    if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
      DEBUG ((DEBUG_ERROR, "ApSafeSetBootPState: StartupAllAps(): %r\n", Status));
      ASSERT (FALSE);
    }
  }
  DEBUG ((DEBUG_INFO, "SetBootFrequency Done \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_SETBOOTFREQUENCY_DONE); //PostCode (0xC34)
}

/**
  Config ForcePR Demotion algorithm.

  @param[in] SiPolicyPpi - The SI Policy PPI instance.
**/
VOID
ConfigForcePrDemotion (
  IN SI_POLICY_PPI            *SiPolicyPpi
  )
{
  CPU_POWER_MGMT_TEST_CONFIG  *CpuPowerMgmtTestConfig;
  EFI_STATUS                  Status;
  UINT32                      LibStatus;
  UINT32                      RequestedValue;
  PCODE_MAILBOX_INTERFACE     MailboxCommand;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Check to disable the ForcePR Demotion algorithm.
  //
  if (CpuPowerMgmtTestConfig->ForcePrDemotion == 0) {
    //
    // Requested Value 1 means disable ForcePR Demotion algorithm.
    // Once algorithm is disabled, it remains disabled till next warm/cold reset.
    //
    RequestedValue = 1;
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_MISC_ALG_CONFIG_INTERFACE;
    MailboxCommand.Fields.Param1 = MAILBOX_PCODE_SUBCMD_WRITE_FORCEPR_DEMOTION_DISABLE_SUBCOMMAND;
    Status = MailboxWrite (MailboxCommand.InterfaceData, RequestedValue, &LibStatus);
    if (Status != EFI_SUCCESS || LibStatus != PCODE_MAILBOX_CC_SUCCESS) {
      DEBUG ((DEBUG_WARN, "ConfigForcePrDemotion: Failed to disable FORCEPR demotion, mailbox status = %X\n", LibStatus));
    }
  }
}

/**
  Config VrAlert Demotion algorithm.
  @param[in] SiPolicyPpi - The SI Policy PPI instance.
**/
VOID
ConfigVrAlertDemotion (
  IN SI_POLICY_PPI            *SiPolicyPpi
  )
{
  CPU_POWER_MGMT_TEST_CONFIG  *CpuPowerMgmtTestConfig;
  EFI_STATUS                  Status;
  UINT32                      LibStatus;
  UINT32                      MailboxData;
  PCODE_MAILBOX_INTERFACE     MailboxCommand;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Check to disable the VrAlert Demotion algorithm.
  //
  if (CpuPowerMgmtTestConfig->VrAlertDemotion == 0) {
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_MISC_ALG_CONFIG_INTERFACE;
    MailboxCommand.Fields.Param1 = MAILBOX_PCODE_SUBCMD_WRITE_VRALERT_DEMOTION_DISABLE_SUBCOMMAND;
    //
    // Once algorithm is disabled, it remains disabled till next warm/cold reset.
    // "1" means to disable VrAlert Demotion Disable.
    //
    MailboxData = 1;
    Status = MailboxWrite (MailboxCommand.InterfaceData, MailboxData, &LibStatus);
    if (Status != EFI_SUCCESS || LibStatus != PCODE_MAILBOX_CC_SUCCESS) {
      DEBUG ((DEBUG_WARN, "ConfigVrAlertDemotion: Failed to disable VRAlert demotion, mailbox status = %X\n", LibStatus));
    }
  }
}

/**
  Config Power Floor Mangament.
  This configuration is locked at BIOS_RST_CPL.

  @param[in] SiPolicyPpi - The SI Policy PPI instance.
**/
VOID
ConfigPowerFloorMgmt (
  IN SI_POLICY_PPI            *SiPolicyPpi
  )
{
  CPU_POWER_MGMT_BASIC_CONFIG      *CpuPowerMgmtBasicConfig;
  EFI_STATUS                       Status;
  UINT32                           MailboxStatus;
  PCODE_MAILBOX_INTERFACE          MailboxCommand;
  MAILBOX_DATA_POWER_FLOOR_CONFIG  MailboxData;
  BOOLEAN                          SettingsChanged;
  FLASH_PARTITION_DATA             PcodePartitionData;
  UINT32                           PcodeNumOfModules;
  BOOLEAN                          PcodeVersionValid;

  SettingsChanged  = FALSE;
  MailboxData.Data = 0;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);
  //
  // Read the existing values for the supported controls
  //
  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_READ_POWER_FLOOR_CONFIG;
  Status = MailboxRead (MailboxCommand.InterfaceData, &MailboxData.Data, &MailboxStatus);
  if (Status != EFI_SUCCESS || MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
    DEBUG ((DEBUG_WARN, "ConfigPowerFloorMgmt: Failed to read mailbox command, mailbox status = %X\n", MailboxStatus));
    return;
  }
  DEBUG ((DEBUG_INFO, "ConfigPowerFloorMgmt: mailbox read value is 0x%x\n", MailboxData.Data));

  //
  // Check to disable the Power Floor Management (reverse encoding is used).
  //
  if (MailboxData.Fields.PowerFloorManagementDisable == CpuPowerMgmtBasicConfig->PowerFloorManagement) {
    //
    // Config policy value 0 means user wants to disable Power Floor management.
    // This option disables SoC aggressive throttling to manage SoC floor power (minimum power consumption).
    // Mailbox parameter is to disable the option and thus have reserve encoding, 0: Enabled(Default), 1: Disabled
    // Hence Requested Value 1 means user wants to disable (opt-out of) Power Floor management.
    // Once it is disabled, it remains disabled till next warm/cold reset.
    //
    DEBUG ((DEBUG_INFO, "ConfigPowerFloorMgmt: User wants to modify Power Floor management \n"));
    MailboxData.Fields.PowerFloorManagementDisable = !CpuPowerMgmtBasicConfig->PowerFloorManagement;
    SettingsChanged = TRUE;
  }
  //
  // Check display disconnect option (reverse encoding is used)
  //
  if (MailboxData.Fields.PowerFloorDisplayDisconnectDisable == CpuPowerMgmtBasicConfig->PowerFloorDisplayDisconnect) {
    //
    // When enabled, SoC can disconnect secondary/external display to lower SoC floor power (Default).
    // Mailbox parameter is to disable the option and thus have reserve encoding, 0: Enabled(Default), 1: Disabled
    // Hence requested value of 1 for the mailbox data, means display disconnect will not be used by SoC.
    // Once it is disabled, it remains disabled till next warm/cold reset.
    //
    DEBUG ((DEBUG_INFO, "ConfigPowerFloorMgmt: User wants to modify Display Disconnect option for SOC floor power management \n"));
    MailboxData.Fields.PowerFloorDisplayDisconnectDisable = !CpuPowerMgmtBasicConfig->PowerFloorDisplayDisconnect;
    SettingsChanged = TRUE;
  }
  //
  // Check PCIe Gen Speed downgrade option (reverse encoding is used)
  //
  if (MailboxData.Fields.PowerFloorPcieGenXDowngradeDisable == CpuPowerMgmtBasicConfig->PowerFloorPcieGenDowngrade) {
    //
    // When enabled, SoC can reduce PCIe gen speed to lower SoC floor power (Default).
    // Mailbox parameter is to disable the option and thus have reserve encoding, 0: Enabled(Default), 1: Disabled
    // Hence requested value of 1 for the mailbox data, means PCIe gen down grade will not be used by SoC.
    // Once it is disabled, it remains disabled till next warm/cold reset.
    //
    DEBUG ((DEBUG_INFO, "ConfigPowerFloorMgmt: User wants to modify PCIe Gen Speed Downgrade option for SOC floor power management \n"));
    MailboxData.Fields.PowerFloorPcieGenXDowngradeDisable = !CpuPowerMgmtBasicConfig->PowerFloorPcieGenDowngrade;
    SettingsChanged = TRUE;
  }

  //
  // Check pcode version before modifying Media Aggressive throttling option
  //
  PcodeVersionValid = FALSE;
  PcodeNumOfModules = 0;
  ZeroMem (&PcodePartitionData, sizeof (FLASH_PARTITION_DATA));

  Status = PeiHeciGetImageFwVersionMsg (FPT_PARTITION_NAME_PCOD, &PcodeNumOfModules, &PcodePartitionData);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ConfigPowerFloorMgmt: Pcode version: %d.%d.%d.%d\n",
      PcodePartitionData.Version.Major, PcodePartitionData.Version.Minor,
      PcodePartitionData.Version.Hotfix, PcodePartitionData.Version.Build));
    //
    // Only enable Media Aggressive throttling config for proper pcode version
    //
    if ((PcodePartitionData.Version.Major > 9) ||
        ((PcodePartitionData.Version.Major == 9) && (PcodePartitionData.Version.Minor > 0)) ||
        ((PcodePartitionData.Version.Major == 9) && (PcodePartitionData.Version.Minor == 0) && (PcodePartitionData.Version.Hotfix >= 33))) {
      PcodeVersionValid = TRUE;
    }
  }
  //
  // Check Media Aggressive throttling option (reverse encoding is used)
  //
  if (PcodeVersionValid &&
      (MailboxData.Fields.PowerFloorAggressiveMediaDisable == CpuPowerMgmtBasicConfig->PowerFloorAggressiveMedia)) {
    //
    // When enabled, SoC uses aggressive media throttling to lower SoC floor power.
    // Mailbox parameter is to disable the option and thus have reverse encoding, 0: Enabled(Default), 1: Disabled
    // Hence requested value of 1 for the mailbox data means aggressive media throttling will be disabled.
    // Once it is disabled, it remains disabled till next warm/cold reset.
    //
    DEBUG ((DEBUG_INFO, "ConfigPowerFloorMgmt: User wants to modify Media Aggressive throttling option for SOC floor power management \n"));
    MailboxData.Fields.PowerFloorAggressiveMediaDisable = !CpuPowerMgmtBasicConfig->PowerFloorAggressiveMedia;
    SettingsChanged = TRUE;
  }

  if (SettingsChanged) {
    DEBUG ((DEBUG_INFO, "ConfigPowerFloorMgmt: Writing value 0x%x to the mailbox\n", MailboxData.Data));
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_WRITE_POWER_FLOOR_CONFIG;
    Status = MailboxWrite (MailboxCommand.InterfaceData, MailboxData.Data, &MailboxStatus);
    if (Status != EFI_SUCCESS || MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
      DEBUG ((DEBUG_WARN, "ConfigPowerFloorMgmt: Failed to modify options for SOC floor power management, EFI_STATUS = %r, Mailbox status = %X\n", Status, MailboxStatus));
    }
  }
}

/**
  Return back Ctdp Level and lock Ctdp if Ctdp Level adjusted before Early PM Done

  @param[in] ConfigTdpLevel  ConfigTdpLevel policy set by user
  @param[in] ConfigTdpLock   ConfigTdpLock policy set by user
**/
VOID
RestoreCtdpLevel (
  IN UINT8  ConfigTdpLevel,
  IN UINT8  ConfigTdpLock
  )
{
  MSR_CONFIG_TDP_CONTROL_REGISTER     ConfigTdpControlMsr;

  ConfigTdpControlMsr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_CONTROL);
  if ((ConfigTdpControlMsr.Bits.ConfigTdpLock == 0) && (ConfigTdpControlMsr.Bits.TdpLevel != ConfigTdpLevel)) {
    ConfigTdpControlMsr.Bits.TdpLevel = ConfigTdpLevel;
    if (ConfigTdpLock == TRUE ) {
      ConfigTdpControlMsr.Bits.ConfigTdpLock = 1;
      DEBUG ((DEBUG_INFO, "ConfigTDP is locked\n"));
    }
    AsmWriteMsr64 (MSR_CONFIG_TDP_CONTROL, ConfigTdpControlMsr.Uint64);
    DEBUG ((DEBUG_INFO, "Restore Ctdp Level: ConfigTDP MSR_CONFIG_TDP_CONTROL = 0x%x\n", ConfigTdpControlMsr.Uint64));
  } else {
    DEBUG ((DEBUG_INFO, "No need to return back Ctdp level or Ctdp has been locked\n"));
  }
}

/**
  Initialize performance and power management features before RESET_CPL at Post-memory phase.

  @param[in] SiPolicyPpi             The SI Policy PPI instance.
  @param[in] CpuPowerDeliveryConfig  Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
EFIAPI
CpuPowerMgmtInitPreResetCpl (
  IN SI_POLICY_PPI             *SiPolicyPpi,
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  CPUID_VERSION_INFO_ECX      Ecx;

  DEBUG ((DEBUG_INFO, "PowerMgmtPreResetCpl Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PRERESETCPL_START); //PostCode (0xC30)

  ///
  /// Check to see if CPU supports EIST capability.
  ///
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
  if (Ecx.Bits.EIST == 1) {
    ///
    /// Program Boot Frequency on all threads.
    ///
    SetBootFrequency (SiPolicyPpi, CpuPowerDeliveryConfig);
  }

  ///
  /// Config ForcePR Demotion
  ///
  ConfigForcePrDemotion (SiPolicyPpi);

  ///
  /// Config VR Alert Demotion
  ///
  ConfigVrAlertDemotion (SiPolicyPpi);

  ///
  /// Config Power Floor Management
  ///
  ConfigPowerFloorMgmt (SiPolicyPpi);

  ///
  /// Return back Ctdp Level and lock Ctdp
  ///
  RestoreCtdpLevel ((UINT8) CpuPowerDeliveryConfig->ConfigTdpLevel, (UINT8) CpuPowerDeliveryConfig->ConfigTdpLock);

  DEBUG ((DEBUG_INFO, "PowerMgmtPreResetCpl Done \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PRERESETCPL_DONE); //PostCode (0xC3F)

  return;
}
