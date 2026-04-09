/** @file
  This file contains policy related to Cpu Powermanagement.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Library/CpuPowerManagementPolicyLib.h>
#include <IndustryStandard/Pci22.h>
#include <IntelRcStatusCode.h>
#include <Library/PeiCpuInitFruLib.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuLib.h>
#include <Register/GenerationMsr.h>

/**
  Print CPU_POWER_MGMT_BASIC_CONFIG and serial out.

  @param[in] CpuPowerMgmtBasicConfig   Pointer to a CPU_POWER_MGMT_BASIC_CONFIG
**/
VOID
CpuPowerMgmtBasicConfigPrint (
  IN CONST CPU_POWER_MGMT_BASIC_CONFIG   *CpuPowerMgmtBasicConfig
  )
{
  UINT8 Index;

  DEBUG ((DEBUG_INFO, "------------------ CPU_POWER_MGMT_BASIC_CONFIG Begin ------------------\n"));
  DEBUG ((DEBUG_INFO, " BootFrequency : 0x%x\n", CpuPowerMgmtBasicConfig->BootFrequency));
  DEBUG ((DEBUG_INFO, " SkipSetBootPState : 0x%x\n", CpuPowerMgmtBasicConfig->SkipSetBootPState));
  DEBUG ((DEBUG_INFO, " Hwp : 0x%x\n", CpuPowerMgmtBasicConfig->Hwp));
  DEBUG ((DEBUG_INFO, " TurboPowerLimitLock : 0x%x\n", CpuPowerMgmtBasicConfig->TurboPowerLimitLock));
  DEBUG ((DEBUG_INFO, " TurboMode : 0x%x\n", CpuPowerMgmtBasicConfig->TurboMode));
  DEBUG ((DEBUG_INFO, " HwpInterruptControl : 0x%x\n", CpuPowerMgmtBasicConfig->HwpInterruptControl));
  DEBUG ((DEBUG_INFO, " HwpLock : 0x%x\n", CpuPowerMgmtBasicConfig->HwpLock));
  DEBUG ((DEBUG_INFO, " OneCoreRatioLimit : 0x%X\n", CpuPowerMgmtBasicConfig->OneCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " TwoCoreRatioLimit : 0x%X\n", CpuPowerMgmtBasicConfig->TwoCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " ThreeCoreRatioLimit : 0x%X\n", CpuPowerMgmtBasicConfig->ThreeCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " FourCoreRatioLimit : 0x%X\n", CpuPowerMgmtBasicConfig->FourCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " FiveCoreRatioLimit : 0x%X\n", CpuPowerMgmtBasicConfig->FiveCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " SixCoreRatioLimit : 0x%X\n", CpuPowerMgmtBasicConfig->SixCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " SevenCoreRatioLimit : 0x%X\n", CpuPowerMgmtBasicConfig->SevenCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " EightCoreRatioLimit : 0x%X\n", CpuPowerMgmtBasicConfig->EightCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " (Resource Priority Feature)EnableRp : 0x%X\n", CpuPowerMgmtBasicConfig->EnableRp));
  DEBUG ((DEBUG_INFO, " EnableHwpAutoPerCorePstate : 0x%x\n", CpuPowerMgmtBasicConfig->EnableHwpAutoPerCorePstate));
  DEBUG ((DEBUG_INFO, " EnableHwpAutoEppGrouping : 0x%x\n", CpuPowerMgmtBasicConfig->EnableHwpAutoEppGrouping));
  DEBUG ((DEBUG_INFO, " EnableDynamicEfficiencyControl : 0x%x\n", CpuPowerMgmtBasicConfig->EnableDynamicEfficiencyControl));
  DEBUG ((DEBUG_INFO, " MinRingRatioLimit : 0x%x\n", CpuPowerMgmtBasicConfig->MinRingRatioLimit));
  DEBUG ((DEBUG_INFO, " MaxRingRatioLimit : 0x%x\n", CpuPowerMgmtBasicConfig->MaxRingRatioLimit));
  DEBUG ((DEBUG_INFO, " PowerFloorManagement : 0x%x\n", CpuPowerMgmtBasicConfig->PowerFloorManagement));
  DEBUG ((DEBUG_INFO, " PowerFloorDisplayDisconnect : 0x%x\n", CpuPowerMgmtBasicConfig->PowerFloorDisplayDisconnect));
  DEBUG ((DEBUG_INFO, " PowerFloorPcieGenDowngrade  : 0x%x\n", CpuPowerMgmtBasicConfig->PowerFloorPcieGenDowngrade));
  DEBUG ((DEBUG_INFO, " PowerFloorAggressiveMedia  : 0x%x\n", CpuPowerMgmtBasicConfig->PowerFloorAggressiveMedia));
  for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE; Index++) {
    DEBUG ((DEBUG_INFO, " TurboRatioLimitRatio[0x%x] : 0x%X\n", Index, CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[Index]));
    DEBUG ((DEBUG_INFO, " TurboRatioLimitNumCore[0x%x] : 0x%X\n", Index, CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[Index]));
    DEBUG ((DEBUG_INFO, " AtomTurboRatioLimitRatio[0x%x] : 0x%X\n", Index, CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[Index]));
    DEBUG ((DEBUG_INFO, " AtomTurboRatioLimitNumCore[0x%x] : 0x%X\n", Index, CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[Index]));
  }
  DEBUG ((DEBUG_INFO, "------------------ CPU_POWER_MGMT_BASIC_CONFIG End ------------------\n"));
}

/**
  Print CPU_POWER_MGMT_CUSTOM_CONFIG and serial out.

  @param[in] CpuPowerMgmtCustomConfig   Pointer to a CPU_POWER_MGMT_CUSTOM_CONFIG
**/
VOID
CpuPowerMgmtCustomConfigPrint (
  IN CONST CPU_POWER_MGMT_CUSTOM_CONFIG   *CpuPowerMgmtCustomConfig
  )
{
  UINT32 Index = 0;
  DEBUG ((DEBUG_INFO, "------------------ CPU_POWER_MGMT_CUSTOM_CONFIG Begin ------------------\n"));
  DEBUG ((DEBUG_INFO, "\n CustomRatioTable... \n"));
  DEBUG ((DEBUG_INFO, " VidMaxRatio : 0x%x\n", CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio));
  DEBUG ((DEBUG_INFO, " VidNumber : 0x%x\n", CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries));
  DEBUG ((DEBUG_INFO, " VidCpuid : 0x%x\n", CpuPowerMgmtCustomConfig->CustomRatioTable.Cpuid));
  for (Index = 0; Index < MAX_CUSTOM_RATIO_TABLE_ENTRIES; Index++) {
    DEBUG ((DEBUG_INFO, " StateRatio[%d] : 0x%x\n", Index, CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatio[Index]));
  }
  for (Index = 0; Index < MAX_16_CUSTOM_RATIO_TABLE_ENTRIES; Index++) {
    DEBUG ((DEBUG_INFO, " StateRatioMax16[%d] : 0x%x\n", Index, CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatioMax16[Index]));
  }
  DEBUG ((DEBUG_INFO, "------------------ CPU_POWER_MGMT_CUSTOM_CONFIG End ------------------\n"));
}

/**
  Print CPU_POWER_DELIVERY_CONFIG and serial out.

  @param[in] CpuPowerDeliveryConfig   Pointer to a CPU_POWER_DELIVERY_CONFIG
**/
VOID
EFIAPI
CpuPowerDeliveryConfigPrint (
  IN CONST CPU_POWER_DELIVERY_CONFIG   *CpuPowerDeliveryConfig
  )
{
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, " PsysPowerLimit1 : 0x%x\n", CpuPowerDeliveryConfig->PsysPowerLimit1));
  DEBUG ((DEBUG_INFO, " PsysPowerLimit1Time : 0x%x\n", CpuPowerDeliveryConfig->PsysPowerLimit1Time));
  DEBUG ((DEBUG_INFO, " PsysPowerLimit2 : 0x%x\n", CpuPowerDeliveryConfig->PsysPowerLimit2));
  DEBUG ((DEBUG_INFO, " PsysPowerLimit1Power = 0x%x\n", CpuPowerDeliveryConfig->PsysPowerLimit1Power));
  DEBUG ((DEBUG_INFO, " PsysPowerLimit2Power = 0x%x\n", CpuPowerDeliveryConfig->PsysPowerLimit2Power));
  DEBUG ((DEBUG_INFO, " PowerLimit2 : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit2));
  DEBUG ((DEBUG_INFO, " PowerLimit3DutyCycle : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit3DutyCycle));
  DEBUG ((DEBUG_INFO, " PowerLimit3Lock : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit3Lock));
  DEBUG ((DEBUG_INFO, " ResponseMode : 0x%x\n", CpuPowerDeliveryConfig->ResponseMode));
  DEBUG ((DEBUG_INFO, " PlatformAtxTelemetryUnit : 0x%x\n", CpuPowerDeliveryConfig->PlatformAtxTelemetryUnit));
  DEBUG ((DEBUG_INFO, " PowerLimit4Lock : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit4Lock));
  DEBUG ((DEBUG_INFO, " TccOffsetLock : 0x%X\n", CpuPowerDeliveryConfig->TccOffsetLock));
  DEBUG ((DEBUG_INFO, " ApplyConfigTdp : 0x%x\n", CpuPowerDeliveryConfig->ApplyConfigTdp));
  DEBUG ((DEBUG_INFO, " TccActivationOffset : 0x%X\n", CpuPowerDeliveryConfig->TccActivationOffset));
  DEBUG ((DEBUG_INFO, " PowerLimit1 : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit1));
  DEBUG ((DEBUG_INFO, " PowerLimit2Power : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit2Power));
  DEBUG ((DEBUG_INFO, " PowerLimit3 : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit3));
  DEBUG ((DEBUG_INFO, " PowerLimit4 : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit4));
  DEBUG ((DEBUG_INFO, " PowerLimit1Time : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit1Time));
  DEBUG ((DEBUG_INFO, " PowerLimit3Time : 0x%x\n", CpuPowerDeliveryConfig->PowerLimit3Time));
  DEBUG ((DEBUG_INFO, " ConfigTdpLock : 0x%x\n", CpuPowerDeliveryConfig->ConfigTdpLock));
  DEBUG ((DEBUG_INFO, " ConfigTdpBios : 0x%x\n", CpuPowerDeliveryConfig->ConfigTdpBios));
  DEBUG ((DEBUG_INFO, " DisableVrThermalAlert : 0x%x\n", CpuPowerDeliveryConfig->DisableVrThermalAlert));
  DEBUG ((DEBUG_INFO, " ThermalMonitor : 0x%x\n", CpuPowerDeliveryConfig->ThermalMonitor));
  DEBUG ((DEBUG_INFO, " ConfigTdpLevel : 0x%x\n", CpuPowerDeliveryConfig->ConfigTdpLevel));
  DEBUG ((DEBUG_INFO, " DualTauBoost : 0x%x\n", CpuPowerDeliveryConfig->DualTauBoost));

  DEBUG ((DEBUG_INFO, " CustomPowerLimit1Time : 0x%x\n", CpuPowerDeliveryConfig->CustomPowerLimit1Time));
  DEBUG ((DEBUG_INFO, " CustomTurboActivationRatio : 0x%x\n", CpuPowerDeliveryConfig->CustomTurboActivationRatio));
  DEBUG ((DEBUG_INFO, " CustomPowerLimit1 : 0x%x\n", CpuPowerDeliveryConfig->CustomPowerLimit1));
  DEBUG ((DEBUG_INFO, " CustomPowerLimit2 : 0x%x\n", CpuPowerDeliveryConfig->CustomPowerLimit2));
  DEBUG ((DEBUG_INFO, " AcDcPowerState : 0x%x\n", CpuPowerDeliveryConfig->AcDcPowerState));

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  Print CPU_POWER_MGMT_TEST_CONFIG and serial out.

  @param[in] CpuPowerMgmtTestConfig   Pointer to a CPU_POWER_MGMT_TEST_CONFIG
**/
VOID
CpuPowerMgmtTestConfigPrint (
  IN CONST CPU_POWER_MGMT_TEST_CONFIG   *CpuPowerMgmtTestConfig
  )
{
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, " Eist : 0x%x\n", CpuPowerMgmtTestConfig->Eist));
  DEBUG ((DEBUG_INFO, " EnergyEfficientPState : 0x%x\n", CpuPowerMgmtTestConfig->EnergyEfficientPState));
  DEBUG ((DEBUG_INFO, " EnergyEfficientTurbo : 0x%x\n", CpuPowerMgmtTestConfig->EnergyEfficientTurbo));
  DEBUG ((DEBUG_INFO, " TStates : 0x%x\n", CpuPowerMgmtTestConfig->TStates));
  DEBUG ((DEBUG_INFO, " EnableAllThermalFunctions : 0x%x\n", CpuPowerMgmtTestConfig->EnableAllThermalFunctions));
  DEBUG ((DEBUG_INFO, " Cx : 0x%x\n", CpuPowerMgmtTestConfig->Cx));
  DEBUG ((DEBUG_INFO, " PmgCstCfgCtrlLock : 0x%x\n", CpuPowerMgmtTestConfig->PmgCstCfgCtrlLock));
  DEBUG ((DEBUG_INFO, " C1Autodemotion : 0x%x\n", CpuPowerMgmtTestConfig->C1AutoDemotion));
  DEBUG ((DEBUG_INFO, " C1Undemotion : 0x%x\n", CpuPowerMgmtTestConfig->C1UnDemotion));
  DEBUG ((DEBUG_INFO, " PkgCState Demotion : 0x%x\n", CpuPowerMgmtTestConfig->PkgCStateDemotion));
  DEBUG ((DEBUG_INFO, " PkgCstateUndemotion : 0x%x\n", CpuPowerMgmtTestConfig->PkgCStateUnDemotion));
  DEBUG ((DEBUG_INFO, " CStatePreWake : 0x%x\n", CpuPowerMgmtTestConfig->CStatePreWake));
  DEBUG ((DEBUG_INFO, " TimedMwait : 0x%x\n", CpuPowerMgmtTestConfig->TimedMwait));
  DEBUG ((DEBUG_INFO, " ForcePRDemotion : 0x%x\n", CpuPowerMgmtTestConfig->ForcePrDemotion));
  DEBUG ((DEBUG_INFO, " RaceToHalt  : 0x%x\n",  CpuPowerMgmtTestConfig->RaceToHalt));
  DEBUG ((DEBUG_INFO, " PkgCStateLimit : 0x%x\n", CpuPowerMgmtTestConfig->PkgCStateLimit));
  DEBUG ((DEBUG_INFO, " PpmIrmSetting : 0x%x\n", CpuPowerMgmtTestConfig->PpmIrmSetting));
  DEBUG ((DEBUG_INFO, " VrAlertDemotion : 0x%x\n", CpuPowerMgmtTestConfig->VrAlertDemotion));
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  Print whole CPU config blocks of SI_POLICY_PPI and serial out in PostMem.

  @param[in] SiPolicyPpi The SI Policy PPI instance
**/
VOID
EFIAPI
CpuPowerManagementPrintPolicy (
  IN  SI_POLICY_PPI       *SiPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                       Status;

  CPU_POWER_MGMT_BASIC_CONFIG      *CpuPowerMgmtBasicConfig;
  CPU_POWER_MGMT_CUSTOM_CONFIG     *CpuPowerMgmtCustomConfig;
  CPU_POWER_MGMT_TEST_CONFIG       *CpuPowerMgmtTestConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtCustomConfigGuid, (VOID *) &CpuPowerMgmtCustomConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPolicyPpi->TableHeader.Header.Revision));

  CpuPowerMgmtBasicConfigPrint (CpuPowerMgmtBasicConfig);
  CpuPowerMgmtCustomConfigPrint (CpuPowerMgmtCustomConfig);
  CpuPowerMgmtTestConfigPrint (CpuPowerMgmtTestConfig);
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
DEBUG_CODE_END();
}

/**
  Load CPU power management basic Config block default.

  @param[in, out] CpuPowerMgmtBasicConfig   Pointer to CPU_POWER_MGMT_BASIC_CONFIG instance

**/
VOID
EFIAPI
PeiCpuLoadPowerMgmtBasicConfigDefault (
  IN OUT CPU_POWER_MGMT_BASIC_CONFIG    *CpuPowerMgmtBasicConfig
  )
{
  MSR_TURBO_RATIO_LIMIT_REGISTER            MsrTurboRatioLimitRatio;
  MSR_TURBO_RATIO_LIMIT_CORES_REGISTER      MsrTurboRatioLimitCores;
  MSR_ATOM_TURBO_RATIO_LIMIT_REGISTER               AtomMsrTurboRatioLimitRatio;
  MSR_ATOM_TURBO_RATIO_LIMIT_CORES_REGISTER         AtomMsrTurboRatioLimitCores;

  MsrTurboRatioLimitRatio.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT);
  CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[0] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit0;
  CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[1] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit1;
  CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[2] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit2;
  CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[3] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit3;
  CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[4] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit4;
  CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[5] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit5;
  CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[6] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit6;
  CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[7] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit7;

  MsrTurboRatioLimitCores.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT_CORES);
  CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[0] = (UINT8) MsrTurboRatioLimitCores.Bits.CoreCount0;
  CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[1] = (UINT8) MsrTurboRatioLimitCores.Bits.CoreCount1;
  CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[2] = (UINT8) MsrTurboRatioLimitCores.Bits.CoreCount2;
  CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[3] = (UINT8) MsrTurboRatioLimitCores.Bits.CoreCount3;
  CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[4] = (UINT8) MsrTurboRatioLimitCores.Bits.CoreCount4;
  CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[5] = (UINT8) MsrTurboRatioLimitCores.Bits.CoreCount5;
  CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[6] = (UINT8) MsrTurboRatioLimitCores.Bits.CoreCount6;
  CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[7] = (UINT8) MsrTurboRatioLimitCores.Bits.CoreCount7;

  AtomMsrTurboRatioLimitRatio.Uint64 = AsmReadMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT);

  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[0] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit0;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[1] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit1;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[2] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit2;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[3] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit3;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[4] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit4;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[5] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit5;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[6] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit6;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[7] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit7;

  AtomMsrTurboRatioLimitCores.Uint64 = AsmReadMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT_CORES);

  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[0] = (UINT8) AtomMsrTurboRatioLimitCores.Bits.CoreCount0;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[1] = (UINT8) AtomMsrTurboRatioLimitCores.Bits.CoreCount1;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[2] = (UINT8) AtomMsrTurboRatioLimitCores.Bits.CoreCount2;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[3] = (UINT8) AtomMsrTurboRatioLimitCores.Bits.CoreCount3;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[4] = (UINT8) AtomMsrTurboRatioLimitCores.Bits.CoreCount4;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[5] = (UINT8) AtomMsrTurboRatioLimitCores.Bits.CoreCount5;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[6] = (UINT8) AtomMsrTurboRatioLimitCores.Bits.CoreCount6;
  CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[7] = (UINT8) AtomMsrTurboRatioLimitCores.Bits.CoreCount7;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuPowerMgmtBasicConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_POWER_MGMT_BASIC_CONFIG     *CpuPowerMgmtBasicConfig;

  CpuPowerMgmtBasicConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuPowerMgmtBasicConfig->Header.GuidHob.Name = %g\n", &CpuPowerMgmtBasicConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuPowerMgmtBasicConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuPowerMgmtBasicConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Power Management Basic configuration
  ********************************/
  CpuPowerMgmtBasicConfig->Hwp                            = TRUE;
  CpuPowerMgmtBasicConfig->EnableRp                       = TRUE;
  CpuPowerMgmtBasicConfig->HwpLock                        = TRUE;
  CpuPowerMgmtBasicConfig->HwpInterruptControl            = TRUE;
  CpuPowerMgmtBasicConfig->PowerFloorManagement           = TRUE;
  CpuPowerMgmtBasicConfig->PowerFloorPcieGenDowngrade     = TRUE;
  CpuPowerMgmtBasicConfig->EnableDynamicEfficiencyControl = TRUE;

  ///
  /// Hwp misc functions
  ///
  CpuPowerMgmtBasicConfig->EnableHwpAutoPerCorePstate     = TRUE;
  CpuPowerMgmtBasicConfig->EnableHwpAutoEppGrouping       = TRUE;
  CpuPowerMgmtBasicConfig->TurboMode                      = TRUE;
  CpuPowerMgmtBasicConfig->BootFrequency                  = 2;  // Turbo Performance

  PeiCpuLoadPowerMgmtBasicConfigDefault (CpuPowerMgmtBasicConfig);
}


/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuPowerMgmtCustomConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_POWER_MGMT_CUSTOM_CONFIG  *CpuPowerMgmtCustomConfig;
  UINT8                         MinBusRatio;

  CpuPowerMgmtCustomConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuPowerMgmtCustomConfig->Header.GuidHob.Name = %g\n", &CpuPowerMgmtCustomConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuPowerMgmtCustomConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuPowerMgmtCustomConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Power Management Custom configuration
  ********************************/
  CpuPowerMgmtCustomConfig->CustomRatioTable.Cpuid = (UINT16) ((GetCpuFamilyModel() | GetCpuSteppingId()) & (0x0FFF));
  GetBusRatio (NULL, &MinBusRatio);
  CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio = MinBusRatio;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuPowerDeliveryConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_POWER_DELIVERY_CONFIG  *CpuPowerDeliveryConfig;

  CpuPowerDeliveryConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuPowerDeliveryConfig Name = %g HobLength = 0x%x\n", &CpuPowerDeliveryConfig->Header.GuidHob.Name, CpuPowerDeliveryConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Power Management Delivery configuration
  ********************************/
  CpuPowerDeliveryConfig->PowerLimit2                  = TRUE;
  CpuPowerDeliveryConfig->PowerLimit3Lock              = FALSE;
  CpuPowerDeliveryConfig->TccOffsetLock                = TRUE;
  CpuPowerDeliveryConfig->ApplyConfigTdp               = TRUE;
  CpuPowerDeliveryConfig->ThermalMonitor               = TRUE;

  //
  // Defaults for Isys current Limit1/2 and tau.
  //
  CpuPowerDeliveryConfig->IsysCurrentLimitL1           = 0x0;
  CpuPowerDeliveryConfig->IsysCurrentL1Tau             = 0x0;
  CpuPowerDeliveryConfig->IsysCurrentLimitL2           = 0x0;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuPowerMgmtTestConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_POWER_MGMT_TEST_CONFIG  *CpuPowerMgmtTestConfig;
  CpuPowerMgmtTestConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuPowerMgmtTestConfig->Header.GuidHob.Name = %g\n", &CpuPowerMgmtTestConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuPowerMgmtTestConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuPowerMgmtTestConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Power Management Test configuration
  ********************************/
  CpuPowerMgmtTestConfig->Eist                          = TRUE;
  CpuPowerMgmtTestConfig->EnergyEfficientPState         = TRUE;
  CpuPowerMgmtTestConfig->ForcePrDemotion               = TRUE; /// Keep Hardware Default setting
  CpuPowerMgmtTestConfig->EnableAllThermalFunctions     = TRUE;
  CpuPowerMgmtTestConfig->Cx                            = TRUE;
  CpuPowerMgmtTestConfig->PmgCstCfgCtrlLock             = TRUE;
  CpuPowerMgmtTestConfig->C1AutoDemotion                = TRUE;
  CpuPowerMgmtTestConfig->C1UnDemotion                  = TRUE;
  CpuPowerMgmtTestConfig->PkgCStateDemotion             = TRUE;
  CpuPowerMgmtTestConfig->PkgCStateUnDemotion           = TRUE;
  CpuPowerMgmtTestConfig->CStatePreWake                 = TRUE;
  CpuPowerMgmtTestConfig->RaceToHalt                    = TRUE;
  CpuPowerMgmtTestConfig->VrAlertDemotion               = TRUE;

  CpuPowerMgmtTestConfig->PkgCStateLimit                = PkgAuto;
  CpuPowerMgmtTestConfig->PpmIrmSetting                 = PpmIrmFixedPriority;
  if (GetCpuSku () == EnumCpuUlt ||
      GetCpuSku () == EnumCpuUlx) {
    CpuPowerMgmtTestConfig->EnergyEfficientTurbo        = TRUE;
  }

}

static COMPONENT_BLOCK_ENTRY  mCpuPowerManagementBlocks [] = {
  {&gCpuPowerMgmtBasicConfigGuid,       sizeof (CPU_POWER_MGMT_BASIC_CONFIG),        CPU_POWER_MGMT_BASIC_CONFIG_REVISION,       LoadCpuPowerMgmtBasicConfigDefault},
  {&gCpuPowerMgmtCustomConfigGuid,      sizeof (CPU_POWER_MGMT_CUSTOM_CONFIG),       CPU_POWER_MGMT_CUSTOM_CONFIG_REVISION,      LoadCpuPowerMgmtCustomConfigDefault},
  {&gCpuPowerMgmtTestConfigGuid,        sizeof (CPU_POWER_MGMT_TEST_CONFIG),         CPU_POWER_MGMT_TEST_CONFIG_REVISION,        LoadCpuPowerMgmtTestConfigDefault},
};

static COMPONENT_BLOCK_ENTRY  mCpuPowerDeliveryBlocks [] = {
  {&gCpuPowerDeliveryConfigGuid,        sizeof (CPU_POWER_DELIVERY_CONFIG),          CPU_POWER_DELIVERY_CONFIG_REVISION,         LoadCpuPowerDeliveryConfigDefault},
};

/**
  Get Cpu power management related config block table total size.

  @retval Size of CPU config block table
**/
UINT16
EFIAPI
CpuPowerManagementGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mCpuPowerManagementBlocks[0], sizeof (mCpuPowerManagementBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  CpuPowerManagementAddConfigBlocks add all Cpu power management related config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CpuPowerManagementAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mCpuPowerManagementBlocks[0], sizeof (mCpuPowerManagementBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  DEBUG ((DEBUG_INFO, "CpuPowerManagementAddConfigBlocks Done \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PWRMGMT_ADDCFGBLKS); //PostCode (0xC7D)

  return Status;
}

/**
  Get Cpu Power Delivery related config block table total size.

  @retval Size of CPU Power Delivery block table
**/
UINT16
EFIAPI
CpuPowerDeliveryConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mCpuPowerDeliveryBlocks[0], sizeof (mCpuPowerDeliveryBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  AddCpuPowerDeliveryConfigBlocks add all Cpu Power Delivery config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AddCpuPowerDeliveryConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mCpuPowerDeliveryBlocks[0], sizeof (mCpuPowerDeliveryBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));

  DEBUG ((DEBUG_INFO, "AddCpuPowerDeliveryConfigBlocks Done \n"));

  return Status;
}