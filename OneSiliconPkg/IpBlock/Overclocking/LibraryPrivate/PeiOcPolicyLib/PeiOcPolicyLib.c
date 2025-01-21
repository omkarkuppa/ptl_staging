/** @file
  This file provides services for Overclocking PEI policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <OverclockingConfig.h>
#include <Library/OcMailboxLib.h>

/**
  Print OVERCLOCKING_PREMEM_CONFIG settings.

  @param[in] SiPolicyPreMemPpi  Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
OcPreMemPrintConfig (
  IN CONST SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  EFI_STATUS                    Status;
  OVERCLOCKING_PREMEM_CONFIG    *OverClockingPreMemConfig;
  UINTN                         DieIndex;
  UINTN                         Index;
  UINTN                         DomainId;
  UINT8                         MaxDomainId;

  MaxDomainId = MAILBOX_OC_DOMAIN_ID_MAX;

  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gOverclockingPreMemConfigGuid, (VOID *) &OverClockingPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ OVERCLOCKING_PREMEM_CONFIG Begin ------------------\n"));
  DEBUG ((DEBUG_INFO, " OcSupport : 0x%X\n", OverClockingPreMemConfig->OcSupport));
  DEBUG ((DEBUG_INFO, " CorePllVoltageOffset : 0x%X\n", OverClockingPreMemConfig->CorePllVoltageOffset));
  DEBUG ((DEBUG_INFO, " Avx2RatioOffset : 0x%X\n", OverClockingPreMemConfig->Avx2RatioOffset));
  DEBUG ((DEBUG_INFO, " BclkAdaptiveVoltage  : 0x%X\n", OverClockingPreMemConfig->BclkAdaptiveVoltage));
  DEBUG ((DEBUG_INFO, " RingDownBin     : 0x%X\n", OverClockingPreMemConfig->RingDownBin));
  DEBUG ((DEBUG_INFO, " RealtimeMemoryTiming : 0x%X\n", OverClockingPreMemConfig->RealtimeMemoryTiming));
  DEBUG ((DEBUG_INFO, " PerCoreRatioOverride : 0x%X\n", OverClockingPreMemConfig->PerCoreRatioOverride));
  DEBUG ((DEBUG_INFO, " GranularRatioOverride : 0x%X\n", OverClockingPreMemConfig->GranularRatioOverride));
  DEBUG ((DEBUG_INFO, " RingPllVoltageOffset  : 0x%X\n", OverClockingPreMemConfig->RingPllVoltageOffset));
  DEBUG ((DEBUG_INFO, " IaAtomPllVoltageOffset: 0x%X\n", OverClockingPreMemConfig->IaAtomPllVoltageOffset));
  DEBUG ((DEBUG_INFO, " TjMaxOffset           : 0x%X\n", OverClockingPreMemConfig->TjMaxOffset));
  DEBUG ((DEBUG_INFO, " ThermalTurbo          : 0x%X\n", OverClockingPreMemConfig->ThermalTurbo));
  DEBUG ((DEBUG_INFO, " NegativeItd           : 0x%X\n", OverClockingPreMemConfig->NegativeItd));
  DEBUG ((DEBUG_INFO, " Avx2VoltageScaleFactor   : 0x%X\n", OverClockingPreMemConfig->Avx2VoltageScaleFactor));
  DEBUG ((DEBUG_INFO, " CpuDlvrMode              : 0x%X\n", OverClockingPreMemConfig->CpuDlvrMode));
  DEBUG ((DEBUG_INFO, " TscDisableHwFixup        : 0x%X\n", OverClockingPreMemConfig->TscDisableHwFixup));
  DEBUG ((DEBUG_INFO, " CoreOpPointReportingEn   : 0x%X\n", OverClockingPreMemConfig->CoreOpPointReportingEn));
  DEBUG ((DEBUG_INFO, " ProcessVmaxLimit         : 0x%X\n", OverClockingPreMemConfig->ProcessVmaxLimit));
  DEBUG ((DEBUG_INFO, " PcorePowerDensityThrottle: 0x%X\n", OverClockingPreMemConfig->PcorePowerDensityThrottle));
  DEBUG ((DEBUG_INFO, " EcorePowerDensityThrottle: 0x%X\n", OverClockingPreMemConfig->EcorePowerDensityThrottle));
  DEBUG ((DEBUG_INFO, " CorePllCurrentRefTuningOffset:   0x%X\n", OverClockingPreMemConfig->CorePllCurrentRefTuningOffset));
  DEBUG ((DEBUG_INFO, " RingPllCurrentRefTuningOffset:   0x%X\n", OverClockingPreMemConfig->RingPllCurrentRefTuningOffset));
  DEBUG ((DEBUG_INFO, " IaAtomPllCurrentRefTuningOffset: 0x%X\n", OverClockingPreMemConfig->IaAtomPllCurrentRefTuningOffset));
  for (Index = 0; Index < MAX_OC_BIG_CORES; Index++) {
    DEBUG ((DEBUG_INFO, " PerCoreRatio[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerCoreRatio[Index]));
  }
  for (Index = 0; Index < MAX_OC_ATOM_CLUSTERS; Index++) {
    DEBUG ((DEBUG_INFO, " PerAtomClusterVoltageOffset[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerAtomClusterVoltageOffset[Index]));
    DEBUG ((DEBUG_INFO, " PerAtomClusterVoltageMode[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerAtomClusterVoltageMode[Index]));
    DEBUG ((DEBUG_INFO, " PerAtomClusterVoltageOverride[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerAtomClusterVoltageOverride[Index]));
    DEBUG ((DEBUG_INFO, " PerAtomClusterAdaptiveVoltage[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerAtomClusterAdaptiveVoltage[Index]));
    DEBUG ((DEBUG_INFO, " PerAtomClusterMaxRatio[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerAtomClusterMaxRatio[Index]));
  }
  for (Index = 0; Index < MAX_OC_CCP_MODULES; Index++) {
    DEBUG ((DEBUG_INFO, " GranularityBins[CDIE0][%2d] : 0x%3X \t GranularityBins[CDIE1][%2d] : 0x%3X\n",
    Index, OverClockingPreMemConfig->GranularityBins[C_DIE_0][Index], Index, OverClockingPreMemConfig->GranularityBins[C_DIE_1][Index]));
  }

  DEBUG ((DEBUG_INFO, " CpuBclkOcFrequency    : 0x%X\n", OverClockingPreMemConfig->CpuBclkOcFrequency));
  DEBUG ((DEBUG_INFO, " SocBclkOcFrequency    : 0x%X\n", OverClockingPreMemConfig->SocBclkOcFrequency));
  DEBUG ((DEBUG_INFO, " PvdRatioThreshold     : 0x%X\n", OverClockingPreMemConfig->PvdRatioThreshold));
  DEBUG ((DEBUG_INFO, " PvdMode               : 0x%X\n", OverClockingPreMemConfig->PvdMode));
  DEBUG ((DEBUG_INFO, " FllOverclockMode      : 0x%X\n", OverClockingPreMemConfig->FllOverclockMode));
  DEBUG ((DEBUG_INFO, " IaIccUnlimitedMode    : 0x%X\n", OverClockingPreMemConfig->IaIccUnlimitedMode));
  DEBUG ((DEBUG_INFO, " IaIccMax              : 0x%X\n", OverClockingPreMemConfig->IaIccMax));
  DEBUG ((DEBUG_INFO, " GtIccUnlimitedMode    : 0x%X\n", OverClockingPreMemConfig->GtIccUnlimitedMode));
  DEBUG ((DEBUG_INFO, " GtIccMax              : 0x%X\n", OverClockingPreMemConfig->GtIccMax));
  DEBUG ((DEBUG_INFO, " SaIccUnlimitedMode    : 0x%X\n", OverClockingPreMemConfig->SaIccUnlimitedMode));
  DEBUG ((DEBUG_INFO, " SaIccMax              : 0x%X\n", OverClockingPreMemConfig->SaIccMax));
  DEBUG ((DEBUG_INFO, " VccsaBootVoltageSel   : 0x%X\n", OverClockingPreMemConfig->VccsaBootVoltageSel));
  DEBUG ((DEBUG_INFO, " CpuD2dRatio           : 0x%X\n", OverClockingPreMemConfig->CpuD2dRatio));
  DEBUG ((DEBUG_INFO, " CpuBandgapRefMode     : 0x%X\n", OverClockingPreMemConfig->CpuBandgapRefMode));
  DEBUG ((DEBUG_INFO, " VcciaBootVoltageSel   : 0x%X\n", OverClockingPreMemConfig->VcciaBootVoltageSel));
  DEBUG ((DEBUG_INFO, " UnderVoltProtection   : 0x%X\n", OverClockingPreMemConfig->UnderVoltProtection));

  DEBUG ((DEBUG_INFO, " OcTvb                    : 0x%X\n", OverClockingPreMemConfig->OcTvb));
  DEBUG ((DEBUG_INFO, " PcoreTvbTempThreshold0   : 0x%X\n", OverClockingPreMemConfig->PcoreTvbTempThreshold0));
  DEBUG ((DEBUG_INFO, " PcoreTvbTempThreshold1   : 0x%X\n", OverClockingPreMemConfig->PcoreTvbTempThreshold1));
  DEBUG ((DEBUG_INFO, " EcoreTvbTempThreshold0   : 0x%X\n", OverClockingPreMemConfig->EcoreTvbTempThreshold0));
  DEBUG ((DEBUG_INFO, " EcoreTvbTempThreshold1   : 0x%X\n", OverClockingPreMemConfig->EcoreTvbTempThreshold1));
  DEBUG ((DEBUG_INFO, " TvbConfigLimitSelect     : 0x%X\n", OverClockingPreMemConfig->TvbConfigLimitSelect));
  DEBUG ((DEBUG_INFO, " ComputeDieSscEnable      : 0x%x\n", OverClockingPreMemConfig->ComputeDieSscEnable));
  DEBUG ((DEBUG_INFO, " SocDieSscEnable          : 0x%x\n", OverClockingPreMemConfig->SocDieSscEnable));
  DEBUG ((DEBUG_INFO, " CoreMinRatio             : 0x%X\n", OverClockingPreMemConfig->CoreMinRatio));
  DEBUG ((DEBUG_INFO, " NegativeTemperatureReporting : 0x%X\n", OverClockingPreMemConfig->NegativeTemperatureReporting));

  if (OverClockingPreMemConfig->TvbConfigLimitSelect == 0) { // Per CCP Module
    for (Index = 0; Index < MAX_OC_BIG_CORES; Index++) {
      DEBUG ((DEBUG_INFO, " PerPcoreRatioDownBinAboveT0[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerPcoreRatioDownBinAboveT0[Index]));
      DEBUG ((DEBUG_INFO, " PerPcoreRatioDownBinAboveT1[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerPcoreRatioDownBinAboveT1[Index]));
    }
    for (Index = 0; Index < MAX_OC_ATOM_CLUSTERS; Index++) {
      DEBUG ((DEBUG_INFO, " PerEcoreCcpRatioDownBinAboveT0[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerEcoreCcpRatioDownBinAboveT0[Index]));
      DEBUG ((DEBUG_INFO, " PerEcoreCcpRatioDownBinAboveT1[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerEcoreCcpRatioDownBinAboveT1[Index]));
    }
  } else { // Per Pcore Group
    for (Index = 0; Index < MAX_OC_BIG_CORE_TRL_GROUPS; Index++) {
      DEBUG ((DEBUG_INFO, " PerPcoreGrRatioDownBinAboveT0[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerPcoreGrRatioDownBinAboveT0[Index]));
      DEBUG ((DEBUG_INFO, " PerPcoreGrRatioDownBinAboveT1[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerPcoreGrRatioDownBinAboveT1[Index]));
    }
  }

  DEBUG ((DEBUG_INFO, " CoreVfConfigScope      : %d\n", OverClockingPreMemConfig->CoreVfConfigScope));
  for (DomainId = 0; DomainId < MaxDomainId; DomainId++) {
    DEBUG ((DEBUG_INFO, " MaxOcRatio[%d]         : %d\n", DomainId, OverClockingPreMemConfig->MaxOcRatio[DomainId]));
    DEBUG ((DEBUG_INFO, " VoltageOverride[%d]    : %d\n", DomainId, OverClockingPreMemConfig->VoltageOverride[DomainId]));
    DEBUG ((DEBUG_INFO, " AdaptiveVoltage[%d]    : %d\n", DomainId, OverClockingPreMemConfig->AdaptiveVoltage[DomainId]));
    DEBUG ((DEBUG_INFO, " VoltageMode[%d]        : %d\n", DomainId, OverClockingPreMemConfig->VoltageMode[DomainId]));
    DEBUG ((DEBUG_INFO, " VoltageOffset[%d]      : %d\n", DomainId, OverClockingPreMemConfig->VoltageOffset[DomainId]));

    DEBUG ((DEBUG_INFO, "VfPointOffsetMode[%d] : %d\n", DomainId, OverClockingPreMemConfig->VfPointOffsetMode[DomainId]));
    if (OverClockingPreMemConfig->VfPointOffsetMode[DomainId] == 0) {
      if (OverClockingPreMemConfig->CoreVfConfigScope == 0) {
        DEBUG ((DEBUG_INFO, " VoltageOffset[%d] : %d\n", DomainId, OverClockingPreMemConfig->VoltageOffset[DomainId]));
      } else {
        for (Index = 0; Index < MAX_OC_BIG_CORES; Index++) {
          DEBUG ((DEBUG_INFO, " PerCoreVoltageOffset[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerCoreVoltageOffset[Index]));
          DEBUG ((DEBUG_INFO, " PerCoreVoltageMode[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerCoreVoltageMode[Index]));
          DEBUG ((DEBUG_INFO, " PerCoreVoltageOverride[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerCoreVoltageOverride[Index]));
          DEBUG ((DEBUG_INFO, " PerCoreAdaptiveVoltage[%d] : 0x%X\n", Index, OverClockingPreMemConfig->PerCoreAdaptiveVoltage[Index]));
          DEBUG ((DEBUG_INFO, " PerCoreMaxRatio[%d] : %d\n", Index, OverClockingPreMemConfig->PerCoreMaxRatio[Index]));
        }
      }
    } else {
      DEBUG ((DEBUG_INFO, " VfPointCount[%d] : %d\n", DomainId, OverClockingPreMemConfig->VfPointCount[DomainId]));
      for (Index = 0; Index < OverClockingPreMemConfig->VfPointCount[DomainId]; Index++) {
        DEBUG ((DEBUG_INFO, " VfPointOffset[%d][%d] : %d\n", DomainId, Index, OverClockingPreMemConfig->VfPointOffset[DomainId][Index]));
      }
    }
  }
  //
  // MaxVoltageLimit supports up to domain 8 (NPU), MAX_OC_DOMAINS was defined to 10
  //
  for (DieIndex = 0; DieIndex < MAX_CDIE; DieIndex++) {
    for (Index = 0; Index <= (MAX_OC_DOMAINS - 2); Index++) {
      DEBUG ((DEBUG_INFO, " OC Max Voltage Limit Die = %d, Domain = %d, Value = %d\n", DieIndex, Index, OverClockingPreMemConfig->MaxVoltageLimit[DieIndex][Index]));
    }
  }

  DEBUG ((DEBUG_INFO, "------------------ OVERCLOCKING_PREMEM_CONFIG End ------------------\n"));

}

/**
  Load Config block default

  @param[in] ConfigBlockPointer  Pointer to config block
**/
VOID
OcLoadConfigDefault (
  IN VOID *ConfigBlockPointer
  )
{
  OVERCLOCKING_PREMEM_CONFIG *OverClockingPreMemConfig;

  OverClockingPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "OverClockingPreMemConfig->Header.GuidHob.Name = %g\n", &OverClockingPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "OverClockingPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", OverClockingPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Overclocking configuration.
  ********************************/
  OverClockingPreMemConfig->RingDownBin = 1;

  OverClockingPreMemConfig->UnderVoltProtection       = 1;  // Enabled by default
  OverClockingPreMemConfig->ThermalTurbo              = 1;  // Enabled by default
  OverClockingPreMemConfig->PcorePowerDensityThrottle = 1;  // Enabled by default
  OverClockingPreMemConfig->EcorePowerDensityThrottle = 1;  // Enabled by default
}

STATIC COMPONENT_BLOCK_ENTRY mOverclockingBlock = {
  &gOverclockingPreMemConfigGuid,
  sizeof (OVERCLOCKING_PREMEM_CONFIG),
  OVERCLOCKING_PREMEM_CONFIG_REVISION,
  OcLoadConfigDefault
};

/**
  Get Overclocking config block table size.

  @retval Size of config block
**/
UINT16
EFIAPI
OcPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mOverclockingBlock.Size;
}

/**
  Add Overclocking ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
OcPreMemAddConfigBlock (
  IN VOID *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mOverclockingBlock, 1);
  return Status;
}
