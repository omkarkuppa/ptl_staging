/** @file
  This file is SampleCode of the library for Intel CPU Overclock PEI Policy initialization.

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
#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <FspmUpd.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/Cpuid.h>
#include <Library/OcMailboxLib.h>
#include <OcLimits.h>

/**
  This function performs CPU Oveclock PEI Policy initialization in Pre-memory.

  @param[in] SiPreMemPolicyPpi     The SI Pre-Mem Policy PPI instance
  @param[in] FspmUpd               The pointer of FspmUpd
**/
EFI_STATUS
EFIAPI
FspUpdatePeiOcPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN FSPM_UPD                     *FspmUpd
  )
{
  OVERCLOCKING_PREMEM_CONFIG      *OverClockingPreMemConfig;
  UINTN                           DomainId;
  UINTN                           VfPointIndex;
  UINT8                           MaxDomainId;
  EFI_STATUS                      Status;
  UINTN                           Index;

  MaxDomainId = MAILBOX_OC_DOMAIN_ID_MAX;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gOverclockingPreMemConfigGuid, (VOID *) &OverClockingPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  //
  //  Update CpuOverClockingPreMem Config Block data
  //
  OverClockingPreMemConfig->OcSupport                = FspmUpd->FspmConfig.OcSupport;
  OverClockingPreMemConfig->RingDownBin              = FspmUpd->FspmConfig.RingDownBin;
  OverClockingPreMemConfig->CorePllVoltageOffset     = FspmUpd->FspmConfig.CorePllVoltageOffset;
  OverClockingPreMemConfig->RingPllVoltageOffset     = FspmUpd->FspmConfig.RingPllVoltageOffset;
  OverClockingPreMemConfig->BclkAdaptiveVoltage      = FspmUpd->FspmConfig.BclkAdaptiveVoltage;
  OverClockingPreMemConfig->Avx2RatioOffset          = FspmUpd->FspmConfig.Avx2RatioOffset;
  OverClockingPreMemConfig->TjMaxOffset              = FspmUpd->FspmConfig.TjMaxOffset;
  OverClockingPreMemConfig->Avx2VoltageScaleFactor   = FspmUpd->FspmConfig.Avx2VoltageScaleFactor;
  OverClockingPreMemConfig->IaAtomPllCurrentRefTuningOffset = FspmUpd->FspmConfig.IaAtomPllCurrentRefTuningOffset;
  OverClockingPreMemConfig->CorePllCurrentRefTuningOffset   = FspmUpd->FspmConfig.CorePllCurrentRefTuningOffset;
  OverClockingPreMemConfig->RingPllCurrentRefTuningOffset   = FspmUpd->FspmConfig.RingPllCurrentRefTuningOffset;
  OverClockingPreMemConfig->ProcessVmaxLimit                = FspmUpd->FspmConfig.ProcessVmaxLimit;
  OverClockingPreMemConfig->CoreMinRatio                    = FspmUpd->FspmConfig.CoreMinRatio;
  OverClockingPreMemConfig->NegativeTemperatureReporting    = FspmUpd->FspmConfig.NegativeTemperatureReporting;
  OverClockingPreMemConfig->FllOverclockMode         = FspmUpd->FspmConfig.FllOverclockMode;
  OverClockingPreMemConfig->CoreOpPointReportingEn   = FspmUpd->FspmConfig.CoreOpPointReportingEn;
  OverClockingPreMemConfig->SocDieSscEnable          = FspmUpd->FspmConfig.SocDieSscEnable;
  //
  // Update Oc Domain VF Config Block Data
  //
  OverClockingPreMemConfig->CoreVfConfigScope              = FspmUpd->FspmConfig.CoreVfConfigScope;

  for (DomainId = 0; DomainId < MaxDomainId; DomainId++) {
    //
    // VF point for IA_CORE , GT, Ring, MemSS and Ngu domains
    //
    if (DomainId == MAILBOX_OC_DOMAIN_ID_IA_CORE || DomainId == MAILBOX_OC_DOMAIN_ID_GT || DomainId == MAILBOX_OC_DOMAIN_ID_RING || \
      DomainId == MAILBOX_OC_DOMAIN_ID_MEMORY_SUBSYSTEM || DomainId == MAILBOX_OC_DOMAIN_ID_COMPUTE_SAF) {
      OverClockingPreMemConfig->VfPointCount[DomainId]         = FspmUpd->FspmConfig.VfPointCount[DomainId];
      //
      // IA_CORE
      //
      for (VfPointIndex = 0; VfPointIndex < MAX_OC_VF_POINTS; VfPointIndex++) {
        // OverClockingPreMemConfig->VfPointRatio[MAILBOX_OC_DOMAIN_ID_IA_CORE][VfPointIndex] = FspmUpd->FspmConfig.CoreVfPointRatio[VfPointIndex];
        if (FspmUpd->FspmConfig.CoreVfPointOffsetPrefix[VfPointIndex] == 1) {
          // Offset is negative, need to convert
          OverClockingPreMemConfig->VfPointOffset[MAILBOX_OC_DOMAIN_ID_IA_CORE][VfPointIndex] = (INT16)(~(FspmUpd->FspmConfig.CoreVfPointOffset[VfPointIndex]) + 1);
        } else {
          OverClockingPreMemConfig->VfPointOffset[MAILBOX_OC_DOMAIN_ID_IA_CORE][VfPointIndex] = (INT16)(FspmUpd->FspmConfig.CoreVfPointOffset[VfPointIndex]);
        }
      }
      //
      // Ring
      //
      for (VfPointIndex = 0; VfPointIndex < MAX_OC_VF_POINTS; VfPointIndex++) {
        // OverClockingPreMemConfig->VfPointRatio[MAILBOX_OC_DOMAIN_ID_RING][VfPointIndex] = FspmUpd->FspmConfig.RingVfPointRatio[VfPointIndex];
        if (FspmUpd->FspmConfig.RingVfPointOffsetPrefix[VfPointIndex] == 1) {
          // Offset is negative, need to convert
          OverClockingPreMemConfig->VfPointOffset[MAILBOX_OC_DOMAIN_ID_RING][VfPointIndex] = (INT16)(~(FspmUpd->FspmConfig.RingVfPointOffset[VfPointIndex]) + 1);
        } else {
          OverClockingPreMemConfig->VfPointOffset[MAILBOX_OC_DOMAIN_ID_RING][VfPointIndex] = (INT16)(FspmUpd->FspmConfig.RingVfPointOffset[VfPointIndex]);
        }
      }
    } // for DomainId
  }

  if (OverClockingPreMemConfig->CoreVfConfigScope) { // Per Core
    for (Index = 0; Index < (FspmUpd->FspmConfig.ActiveCoreCount); Index++) {
      if (FspmUpd->FspmConfig.PerCoreVoltageOffsetPrefix[Index] == 1) {
        OverClockingPreMemConfig->PerCoreVoltageOffset[Index] = (INT16)(~(FspmUpd->FspmConfig.PerCoreVoltageOffset[Index]) + 1);
      } else {
        OverClockingPreMemConfig->PerCoreVoltageOffset[Index] = (INT16)(FspmUpd->FspmConfig.PerCoreVoltageOffset[Index]);
      }
      OverClockingPreMemConfig->PerCoreVoltageMode[Index]       = (INT8) (FspmUpd->FspmConfig.PerCoreVoltageMode[Index]);
      OverClockingPreMemConfig->PerCoreVoltageOverride[Index]   = (INT16) (FspmUpd->FspmConfig.PerCoreVoltageOverride[Index]);
    }
    // Per Atom Cluster
    for (Index = 0; Index < MAX_OC_ATOM_CLUSTERS; Index++) {
      if (FspmUpd->FspmConfig.PerAtomClusterVoltageOffsetPrefix[Index] == 1) {
        OverClockingPreMemConfig->PerAtomClusterVoltageOffset[Index] = (INT16)(~(FspmUpd->FspmConfig.PerAtomClusterVoltageOffset[Index]) + 1);
      } else {
        OverClockingPreMemConfig->PerAtomClusterVoltageOffset[Index] = (INT16)(FspmUpd->FspmConfig.PerAtomClusterVoltageOffset[Index]);
      }
      OverClockingPreMemConfig->PerAtomClusterVoltageMode[Index]       = (INT8) (FspmUpd->FspmConfig.PerAtomClusterVoltageMode[Index]);
      OverClockingPreMemConfig->PerAtomClusterVoltageOverride[Index]   = (INT16) (FspmUpd->FspmConfig.PerAtomClusterVoltageOverride[Index]);
    }
  }

  OverClockingPreMemConfig->DisablePerCoreMask  = FspmUpd->FspmConfig.DisablePerCoreMask;
  OverClockingPreMemConfig->PvdRatioThreshold   = FspmUpd->FspmConfig.PvdRatioThreshold;
  OverClockingPreMemConfig->PvdMode             = FspmUpd->FspmConfig.PvdMode;
  OverClockingPreMemConfig->UnderVoltProtection = FspmUpd->FspmConfig.UnderVoltProtection;
  OverClockingPreMemConfig->CoreOpPointReportingEn = FspmUpd->FspmConfig.CoreOpPointReportingEn;

  //
  // CPU BCLK OC Frequency
  //
  OverClockingPreMemConfig->CpuBclkOcFrequency       = FspmUpd->FspmConfig.CpuBclkOcFrequency;
  //
  // SOC BCLK OC Frequency
  //
  OverClockingPreMemConfig->SocBclkOcFrequency       = FspmUpd->FspmConfig.SocBclkOcFrequency;
  OverClockingPreMemConfig->SocDieSscEnable          = FspmUpd->FspmConfig.SocDieSscEnable;
  //
  // Core Ratio Override
  //
  if (OverClockingPreMemConfig->PerCoreRatioOverride) {
    for (Index = 0; Index < MAX_OC_BIG_CORES; Index++) {
      OverClockingPreMemConfig->PerCoreRatio[Index] = FspmUpd->FspmConfig.PerCoreRatio[Index];
    }
    for (Index = 0; Index < MAX_OC_ATOM_CLUSTERS ; Index++) {
      OverClockingPreMemConfig->AtomClusterRatio[Index] = FspmUpd->FspmConfig.AtomClusterRatio[Index];
    }
  }

  //
  // Unlimited ICCMAX
  //
  OverClockingPreMemConfig->ProcessVmaxLimit                = FspmUpd->FspmConfig.ProcessVmaxLimit;
  OverClockingPreMemConfig->CorePllCurrentRefTuningOffset   = FspmUpd->FspmConfig.CorePllCurrentRefTuningOffset;
  OverClockingPreMemConfig->RingPllCurrentRefTuningOffset   = FspmUpd->FspmConfig.RingPllCurrentRefTuningOffset;
  OverClockingPreMemConfig->IaAtomPllCurrentRefTuningOffset = FspmUpd->FspmConfig.IaAtomPllCurrentRefTuningOffset;
  OverClockingPreMemConfig->CoreMinRatio                    = FspmUpd->FspmConfig.CoreMinRatio;
  OverClockingPreMemConfig->NegativeTemperatureReporting    = FspmUpd->FspmConfig.NegativeTemperatureReporting;
  OverClockingPreMemConfig->PcorePowerDensityThrottle       = FspmUpd->FspmConfig.PcorePowerDensityThrottle;
  OverClockingPreMemConfig->EcorePowerDensityThrottle       = FspmUpd->FspmConfig.EcorePowerDensityThrottle;

  //
  // Granular Ratio Override
  //
  OverClockingPreMemConfig->GranularRatioOverride = FspmUpd->FspmConfig.GranularRatioOverride;
  OverClockingPreMemConfig->OcSupport            = FspmUpd->FspmConfig.SaOcSupport;
  OverClockingPreMemConfig->RealtimeMemoryTiming = FspmUpd->FspmConfig.RealtimeMemoryTiming;

  return EFI_SUCCESS;
}