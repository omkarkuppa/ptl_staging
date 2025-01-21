/** @file
  Pei CPU Specific Features.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Register/GenerationMsr.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/CpuInfoFruLib.h>

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
  )
{
  CPU_REGISTER_TABLE_WRITE_FIELD (
    ProcessorNumber,
    Msr,
    MSR_PREFETCH_CTL,
    MSR_PREFETCH_CTL_REGISTER,
    Bits.MlcStreamerPrefetchDisable,
    (State) ? 0 : 1
    );
  return RETURN_SUCCESS;
}

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
  )
{
  CPU_REGISTER_TABLE_WRITE_FIELD (
    ProcessorNumber,
    Msr,
    MSR_PREFETCH_CTL,
    MSR_PREFETCH_CTL_REGISTER,
    Bits.MlcSpatialPrefetchDisable,
    (State) ? 0 : 1
    );
  return RETURN_SUCCESS;
}


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
  )
{
  if (!IsHfpgaEnvironment ()) {
    CPU_REGISTER_TABLE_WRITE_FIELD (
      ProcessorNumber,
      Msr,
      MSR_DISABLE_SIGNALING_THREE_STRIKE_EVENT,
      MSR_DISABLE_SIGNALING_THREE_STRIKE_EVENT_REGISTER,
      Bits.Cnt,
      (State) ? 0 : 1
      );
  }

  return RETURN_SUCCESS;
}

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
  )
{
  VOID          *ConfigData;

  ConfigData = AllocateZeroPool (sizeof (MSR_PPIN_CTL_REGISTER) * NumberOfProcessors);
  ASSERT (ConfigData != NULL);
  return ConfigData;
}

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
  )
{
  if (IsPpinFeatureAvailable ()) {
    return TRUE;
  }
  return FALSE;
}

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
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  MSR_PPIN_CTL_REGISTER     *MsrPpinCtrl;

  if (ConfigData == NULL) {
    ASSERT (ConfigData != NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  MsrPpinCtrl = (MSR_PPIN_CTL_REGISTER *) ConfigData;

  //
  // If already locked, just based on the request state and
  // the current state to return the status.
  //
  if (MsrPpinCtrl[ProcessorNumber].Bits.Lock != 0) {
    return MsrPpinCtrl[ProcessorNumber].Bits.Enable == State ? RETURN_SUCCESS : RETURN_DEVICE_ERROR;
  }

  //
  // The scope of the MSR_PPIN_CTL_REGISTER is package level, only program MSR_PPIN_CTL_REGISTER once for each package.
  //
  if ((CpuInfo->First.Thread == 0) || (CpuInfo->First.Core == 0)) {
    return RETURN_SUCCESS;
  }

  if (State) {
    //
    // Enable and Unlock.
    //
    MsrPpinCtrl[ProcessorNumber].Bits.Enable = 1;
    MsrPpinCtrl[ProcessorNumber].Bits.Lock = 0;
  } else {
    //
    // Disable and Lock.
    //
    MsrPpinCtrl[ProcessorNumber].Bits.Enable = 0;
    MsrPpinCtrl[ProcessorNumber].Bits.Lock = 1;
  }

  CPU_REGISTER_TABLE_WRITE64 (
    ProcessorNumber,
    Msr,
    MSR_PPIN_CTL,
    MsrPpinCtrl[ProcessorNumber].Uint64
    );

  return RETURN_SUCCESS;
}