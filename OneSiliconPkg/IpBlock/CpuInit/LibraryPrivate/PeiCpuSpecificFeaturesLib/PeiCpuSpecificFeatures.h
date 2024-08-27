/** @file
  This file has APIs for CPU specific features.

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

#ifndef _CPU_SPECIFIC_FEATURES_H_
#define _CPU_SPECIFIC_FEATURES_H_

#include <PiPei.h>
#include <Library/RegisterCpuFeaturesLib.h>

/**
  Initializes Mlc Streamer Prefetcher feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Mlc Streamer Prefetcher feature
                               must be enabled.
                               If FALSE, then the Mlc Streamer Prefetcher feature
                               must be disabled.

  @retval RETURN_SUCCESS       Mlc Streamer Prefetcher feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
MlcStreamerPrefetcherInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  );

/**
  Initializes Mlc Spatial Prefetcher feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Mlc Spatial Prefetcher feature
                               must be enabled.
                               If FALSE, then the Mlc Spatial Prefetcher feature
                               must be disabled.

  @retval RETURN_SUCCESS       Mlc Spatial Prefetcher feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
MlcSpatialPrefetcherInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  );

/**
  Initializes three Strike Counter feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the three Strike Counter feature
                               must be enabled.
                               If FALSE, then the three Strike Counter feature
                               must be disabled.

  @retval RETURN_SUCCESS       Three Strike Counter feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
ThreeStrikeCounterInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  );

/**
  Prepares for the data used by CPU feature detection and initialization.

  @param[in]  NumberOfProcessors  The number of CPUs in the platform.

  @return  Pointer to a buffer of CPU related configuration data.

  @note This service could be called by BSP only.
**/
VOID *
EFIAPI
SpecificPpinGetConfigData (
  IN UINTN               NumberOfProcessors
  );

/**
  Detects if Protected Processor Inventory Number feature supported on current
  processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     Protected Processor Inventory Number feature is supported.
  @retval FALSE    Protected Processor Inventory Number feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
IsSpecificPpinSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  );

/**
  Initializes Protected Processor Inventory Number feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Protected Processor Inventory
                               Number feature must be enabled.
                               If FALSE, then the Protected Processor Inventory
                               Number feature must be disabled.

  @retval RETURN_SUCCESS       Protected Processor Inventory Number feature is
                               initialized.
  @retval RETURN_DEVICE_ERROR  Device can't change state because it has been
                               locked.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
SpecificPpinInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData, OPTIONAL
  IN BOOLEAN                           State
  );

#endif
