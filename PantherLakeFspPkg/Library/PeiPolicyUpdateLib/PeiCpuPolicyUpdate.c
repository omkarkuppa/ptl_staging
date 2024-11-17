/** @file
  This file is SampleCode of the library for Intel CPU PEI Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <PiPei.h>
#include <Ppi/SiPolicy.h>
#include <Library/FspCommonLib.h>
#include <FspsUpd.h>
#include <Library/DebugLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/Cpuid.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiVrDomainLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

/**
  This function performs CPU PEI Policy initialization.

  @param[in] SiPolicyPpi           The SI Policy PPI instance
  @param[in] FspsUpd               The pointer of FspsUpd

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiCpuPolicy (
  IN OUT SI_POLICY_PPI        *SiPolicyPpi,
  IN FSPS_UPD                 *FspsUpd
  )
{
  EFI_STATUS                       Status;
  UINT64                           MicrocodeRegionBase;
  UINT64                           MicrocodeRegionSize;
  CPU_INIT_CONFIG                  *CpuInitConfig;
  CPU_POWER_MGMT_BASIC_CONFIG      *CpuPowerMgmtBasicConfig;
  CPU_POWER_MGMT_CUSTOM_CONFIG     *CpuPowerMgmtCustomConfig;
  CPU_TEST_CONFIG                  *CpuTestConfig;
  CPU_POWER_MGMT_TEST_CONFIG       *CpuPowerMgmtTestConfig;
  UINTN                            Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuInitConfigGuid, (VOID *) &CpuInitConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtCustomConfigGuid, (VOID *) &CpuPowerMgmtCustomConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuTestConfigGuid, (VOID *) &CpuTestConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);

  MicrocodeRegionBase = FspsUpd->FspsConfig.MicrocodeRegionBase;
  MicrocodeRegionSize = FspsUpd->FspsConfig.MicrocodeRegionSize;

  if (MicrocodeRegionBase == 0) {
    DEBUG((DEBUG_INFO, "Microcode update not available in UPD region. Get it from FSP Global data Pointer\n"));

    ///
    /// Get the data from Fsp Global Data
    ///
    MicrocodeRegionBase = GetFspGlobalDataPointer ()->PlatformData.MicrocodeRegionBase;
    MicrocodeRegionSize = GetFspGlobalDataPointer ()->PlatformData.MicrocodeRegionSize;

    if (MicrocodeRegionBase == 0) {
      DEBUG ((DEBUG_INFO, "Microcode update not available in FSP_TEMP_RAM_INIT_PARAMS region.\n"));
    }
  }

  if (MicrocodeRegionBase != 0) {
    DEBUG((DEBUG_INFO, "Microcode update found:  MicrocodeRegionBase = 0x%lx, MicrocodeRegionSize = 0x%lx.\n", MicrocodeRegionBase, MicrocodeRegionSize));
    CpuInitConfig->MicrocodePatchAddress = (UINTN) MicrocodeRegionBase;
    CpuInitConfig->MicrocodePatchRegionSize = (UINT32) MicrocodeRegionSize;
    PcdSetEx64S (&gUefiCpuPkgTokenSpaceGuid, PcdCpuMicrocodePatchAddress, MicrocodeRegionBase);
    PcdSetEx64S (&gUefiCpuPkgTokenSpaceGuid, PcdCpuMicrocodePatchRegionSize, MicrocodeRegionSize);
  }

  CpuInitConfig->TxtEnable = FspsUpd->FspsConfig.TxtEnable;

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  CpuInitConfig->AcSplitLock = FspsUpd->FspsConfig.AcSplitLock;
#endif

  ///
  ///Production RC Policies
  ///

  CpuInitConfig->AesEnable                   = FspsUpd->FspsConfig.AesEnable;
  CpuInitConfig->TxtEnable                   = FspsUpd->FspsConfig.TxtEnable;
  CpuInitConfig->PpinSupport                 = FspsUpd->FspsConfig.PpinSupport;
  CpuInitConfig->AvxDisable                  = FspsUpd->FspsConfig.AvxDisable;
  CpuInitConfig->X2ApicEnable                = FspsUpd->FspsConfig.X2ApicEnable;

  CpuPowerMgmtBasicConfig->TurboMode         = FspsUpd->FspsConfig.TurboMode;
  CpuPowerMgmtBasicConfig->BootFrequency     = FspsUpd->FspsConfig.BootFrequency;

  ///
  ///Test RC Policies
  ///
  CpuTestConfig->MlcStreamerPrefetcher      = FspsUpd->FspsConfig.MlcStreamerPrefetcher;
  CpuTestConfig->MlcSpatialPrefetcher       = FspsUpd->FspsConfig.MlcSpatialPrefetcher;
  CpuTestConfig->MonitorMwaitEnable         = FspsUpd->FspsConfig.MonitorMwaitEnable;
  CpuTestConfig->ProcessorTraceOutputScheme = FspsUpd->FspsConfig.ProcessorTraceOutputScheme;
  CpuTestConfig->ProcessorTraceEnable       = FspsUpd->FspsConfig.ProcessorTraceEnable;
  CpuTestConfig->ProcessorTraceMemSize      = FspsUpd->FspsConfig.ProcessorTraceMemSize;
  CpuTestConfig->ProcessorTraceBspOnly      = FspsUpd->FspsConfig.ProcessorTraceBspOnly;
  CpuTestConfig->ProcessorTraceTimingPacket = FspsUpd->FspsConfig.ProcessorTraceTimingPacket;
  CpuTestConfig->ThreeStrikeCounter         = FspsUpd->FspsConfig.ThreeStrikeCounter;
  CpuTestConfig->MachineCheckEnable         = FspsUpd->FspsConfig.MachineCheckEnable;

  for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE; Index++) {
    CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[Index] = FspsUpd->FspsConfig.TurboRatioLimitNumCore[Index];
    CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[Index] = FspsUpd->FspsConfig.TurboRatioLimitRatio[Index];
    CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[Index] = FspsUpd->FspsConfig.AtomTurboRatioLimitNumCore[Index];
    CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[Index] = FspsUpd->FspsConfig.AtomTurboRatioLimitRatio[Index];
  }

  CpuPowerMgmtBasicConfig->Hwp                                               = FspsUpd->FspsConfig.Hwp;
  CpuPowerMgmtBasicConfig->TurboPowerLimitLock                               = FspsUpd->FspsConfig.TurboPowerLimitLock;
  CpuPowerMgmtBasicConfig->HwpInterruptControl                               = FspsUpd->FspsConfig.HwpInterruptControl;
  CpuPowerMgmtBasicConfig->EnableRp                                          = FspsUpd->FspsConfig.EnableRp;
  CpuPowerMgmtBasicConfig->EnableHwpAutoPerCorePstate                        = FspsUpd->FspsConfig.EnableHwpAutoPerCorePstate;
  CpuPowerMgmtBasicConfig->EnableHwpAutoEppGrouping                          = FspsUpd->FspsConfig.EnableHwpAutoEppGrouping;
  CpuPowerMgmtBasicConfig->MinRingRatioLimit                                 = FspsUpd->FspsConfig.MinRingRatioLimit;
  CpuPowerMgmtBasicConfig->MaxRingRatioLimit                                 = FspsUpd->FspsConfig.MaxRingRatioLimit;
  CpuPowerMgmtBasicConfig->HwpLock                                           = FspsUpd->FspsConfig.HwpLock;
  CpuPowerMgmtBasicConfig->PowerFloorManagement                              = FspsUpd->FspsConfig.PowerFloorManagement;
  CpuPowerMgmtBasicConfig->PowerFloorDisplayDisconnect                       = FspsUpd->FspsConfig.PowerFloorDisplayDisconnect;
  CpuPowerMgmtBasicConfig->EnableFastMsrHwpReq                               = FspsUpd->FspsConfig.EnableFastMsrHwpReq;
  CpuPowerMgmtBasicConfig->PowerFloorPcieGenDowngrade                        = FspsUpd->FspsConfig.PowerFloorPcieGenDowngrade;

  CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio                        = FspsUpd->FspsConfig.MaxRatio;
  CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries                 = FspsUpd->FspsConfig.NumberOfEntries;
  if (FspsUpd->FspsConfig.NumberOfEntries > MAX_CUSTOM_RATIO_TABLE_ENTRIES) {
    DEBUG((DEBUG_WARN, "[WARN] Number of Custom Ratio State Entries (%x) exceeds MAX_CUSTOM_RATIO_TABLE_ENTRIES (%x)\n", FspsUpd->FspsConfig.NumberOfEntries, MAX_CUSTOM_RATIO_TABLE_ENTRIES));
    DEBUG((DEBUG_WARN, "[WARN] Setting number of entries to MAX_CUSTOM_RATIO_TABLE (%x)\n", MAX_CUSTOM_RATIO_TABLE_ENTRIES));
    ASSERT (FspsUpd->FspsConfig.NumberOfEntries < MAX_CUSTOM_RATIO_TABLE_ENTRIES);
    CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries = MAX_CUSTOM_RATIO_TABLE_ENTRIES;
  }

  for (Index = 0; Index < CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries; Index++) {
    CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatio[Index]           = FspsUpd->FspsConfig.StateRatio[Index];
  }

  CpuPowerMgmtTestConfig->Eist                          = FspsUpd->FspsConfig.Eist;
  CpuPowerMgmtTestConfig->EnergyEfficientPState         = FspsUpd->FspsConfig.EnergyEfficientPState;
  CpuPowerMgmtTestConfig->EnergyEfficientTurbo          = FspsUpd->FspsConfig.EnergyEfficientTurbo;
  CpuPowerMgmtTestConfig->TStates                       = FspsUpd->FspsConfig.TStates;
  CpuPowerMgmtTestConfig->EnableAllThermalFunctions     = FspsUpd->FspsConfig.EnableAllThermalFunctions;
  CpuPowerMgmtTestConfig->Cx                            = FspsUpd->FspsConfig.Cx;
  CpuPowerMgmtTestConfig->PmgCstCfgCtrlLock             = FspsUpd->FspsConfig.PmgCstCfgCtrlLock;
  CpuPowerMgmtTestConfig->C1AutoDemotion                = FspsUpd->FspsConfig.C1StateAutoDemotion;
  CpuPowerMgmtTestConfig->C1UnDemotion                  = FspsUpd->FspsConfig.C1StateUnDemotion;
  CpuPowerMgmtTestConfig->PkgCStateDemotion             = FspsUpd->FspsConfig.PkgCStateDemotion;
  CpuPowerMgmtTestConfig->PkgCStateUnDemotion           = FspsUpd->FspsConfig.PkgCStateUnDemotion;
  CpuPowerMgmtTestConfig->CStatePreWake                 = FspsUpd->FspsConfig.CStatePreWake;
  CpuPowerMgmtTestConfig->TimedMwait                    = FspsUpd->FspsConfig.TimedMwait;
  CpuPowerMgmtTestConfig->PkgCStateLimit                = FspsUpd->FspsConfig.PkgCStateLimit;
  CpuPowerMgmtTestConfig->PpmIrmSetting                 = FspsUpd->FspsConfig.PpmIrmSetting;
  CpuPowerMgmtTestConfig->ForcePrDemotion               = FspsUpd->FspsConfig.ForcePrDemotion;
  CpuPowerMgmtTestConfig->VrAlertDemotion               = FspsUpd->FspsConfig.VrAlertDemotion;
  CpuPowerMgmtTestConfig->RaceToHalt                    = FspsUpd->FspsConfig.RaceToHalt;

  return EFI_SUCCESS;
}

