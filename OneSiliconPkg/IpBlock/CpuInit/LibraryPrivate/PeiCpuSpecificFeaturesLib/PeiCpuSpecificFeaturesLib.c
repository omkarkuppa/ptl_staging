/** @file
  Pei CPU Specific Features Library.

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

@par Specification Reference:
**/

#include "PeiCpuSpecificFeatures.h"
#include <Library/DebugLib.h>

/**
  Common function to return FALSE (unsupported) for some features.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     The feature is supported.
  @retval FALSE    The feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
CpuFeatureUnsupported (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  return FALSE;
}

/**
  If the feature is supported by CPU, CpuFeaturePei will configure the feature
  to enable or disabled according to PcdCpuFeatureSetting. However, in below
  we override those features to unsupported. Those features are not really
  unsupported by CPU, it due to we don't want to let the CpuFeaturePei to configure
  those feature to enable or disable again after CPU RC since those features are already configured in CPU RC
**/
VOID
OverrideCommonFeatures (
  VOID
  )
{
  EFI_STATUS         Status;

  if (IsCpuFeatureSupported (CPU_FEATURE_MCG_CTL)) {
    Status = RegisterCpuFeature (
               "MCG_CTL",
               NULL,
               CpuFeatureUnsupported,
               NULL,
               CPU_FEATURE_MCG_CTL,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_PENDING_BREAK)) {
    Status = RegisterCpuFeature (
               "Pending Break",
               NULL,
               CpuFeatureUnsupported,
               NULL,
               CPU_FEATURE_PENDING_BREAK,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_C1E)) {
    Status = RegisterCpuFeature (
               "C1E",
               NULL,
               CpuFeatureUnsupported,
               NULL,
               CPU_FEATURE_C1E,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  //
  // Return Unsupported for the common features that client doesnt program
  //
  if (IsCpuFeatureSupported (CPU_FEATURE_ACPI)) {
    Status = RegisterCpuFeature (
               "ACPI",
               NULL,
               CpuFeatureUnsupported,
               NULL,
               CPU_FEATURE_ACPI,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_FASTSTRINGS)) {
    Status = RegisterCpuFeature (
               "FastStrings",
               NULL,
               CpuFeatureUnsupported,
               NULL,
               CPU_FEATURE_FASTSTRINGS,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_LMCE)) {
    Status = RegisterCpuFeature (
               "LMCE",
               NULL,
               CpuFeatureUnsupported,
               NULL,
               CPU_FEATURE_LMCE,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_LIMIT_CPUID_MAX_VAL)) {
    Status = RegisterCpuFeature (
               "Limit CpuId Maximum Value",
               NULL,
               CpuFeatureUnsupported,
               NULL,
               CPU_FEATURE_LIMIT_CPUID_MAX_VAL,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  //
  // EIST will be updated in PowerManagement Flows
  //
  if (IsCpuFeatureSupported (CPU_FEATURE_EIST)) {
    Status = RegisterCpuFeature (
               "EIST",
               NULL,
               CpuFeatureUnsupported,
               NULL,
               CPU_FEATURE_EIST,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Override for Specific PPIN support
  //
  if (IsCpuFeatureSupported (CPU_FEATURE_PPIN)) {
    Status = RegisterCpuFeature (
               "PPIN",
               SpecificPpinGetConfigData,
               IsSpecificPpinSupport,
               SpecificPpinInitialize,
               CPU_FEATURE_PPIN,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Reuse the common code completely from UefiCpuPkg CpuCommonFeaturesLib
  // for the features below.
  //   CPU_FEATURE_AESNI               : Aesni.c
  //   CPU_FEATURE_MWAIT               : MonitorMwait.c
  //   CPU_FEATURE_SMX                 : FeatureControl.c
  //   CPU_FEATURE_VMX                 : FeatureControl.c
  //   CPU_FEATURE_LOCK_FEATURE_CONTROL_REGISTER  :FeatureControl.c
  //   CPU_FEATURE_PROC_TRACE          : ProcTrace.c
}

/**
  Register CPU features.

  @retval  RETURN_SUCCESS            Register successfully
**/
RETURN_STATUS
EFIAPI
CpuSpecificFeaturesLibConstructor (
  VOID
  )
{
  EFI_STATUS         Status;

  //
  // Override some functions for some common features
  // which have original implementation in UefiCpuPkg CpuCommonFeaturesLib.
  //
  OverrideCommonFeatures ();

  //
  // The features below are not implemented in UefiCpuPkg CpuCommonFeaturesLib at all.
  //
  if (IsCpuFeatureSupported (CPU_FEATURE_MLC_STREAMER_PREFETCHER)) {
    Status = RegisterCpuFeature (
               "Mlc Streamer Prefetcher",
               NULL,
               NULL,
               MlcStreamerPrefetcherInitialize,
               CPU_FEATURE_MLC_STREAMER_PREFETCHER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_MLC_SPATIAL_PREFETCHER)) {
    Status = RegisterCpuFeature (
               "Mlc Spatial Prefetcher",
               NULL,
               NULL,
               MlcSpatialPrefetcherInitialize,
               CPU_FEATURE_MLC_SPATIAL_PREFETCHER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_THREE_STRIKE_COUNTER)) {
    Status = RegisterCpuFeature (
               "Three Strike Counter",
               NULL,
               NULL,
               ThreeStrikeCounterInitialize,
               CPU_FEATURE_THREE_STRIKE_COUNTER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  return RETURN_SUCCESS;
}
