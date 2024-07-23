/** @file
  This file provides services for PEI Telemetry policy default initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <TelemetryPeiConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TelemetryLoadPeiConfigPreMemDefault (
  IN VOID   *ConfigBlockPointer
  )
{
  TELEMETRY_PEI_PREMEM_CONFIG   *TelemetryPreMemConfig;

  TelemetryPreMemConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "TelemetryConfig->Header.GuidHob.Name = %g\n", &TelemetryPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TelemetryConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TelemetryPreMemConfig->Header.GuidHob.Header.HobLength));

  TelemetryPreMemConfig->CpuCrashLogDevice  = 1;
  TelemetryPreMemConfig->StreamTracerSize   = 64;
}

static COMPONENT_BLOCK_ENTRY  mTelemetryIpBlockPreMem = {
  &gTelemetryPeiPreMemConfigGuid,  sizeof (TELEMETRY_PEI_PREMEM_CONFIG), TELEMETRY_PEI_PREMEM_CONFIG_REVISION, TelemetryLoadPeiConfigPreMemDefault
};

/**
  Get Telemetry PEI PreMem config block table total size.

  @retval     Size of Telemetry PEI PreMem config block table
**/
UINT16
EFIAPI
TelemetryGetPeiConfigBlockTotalSizePreMem (
  VOID
  )
{
  return mTelemetryIpBlockPreMem.Size;
}

/**
  TelemetryAddPeiConfigBlockPreMem add all Telemetry PreMem PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
TelemetryAddPeiConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mTelemetryIpBlockPreMem, 1);
  return Status;
}

/**
  This function prints the Telemetry PreMem PEI phase policy.

  @param[in] SiPreMemPolicyPpi    Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
TelemetryPrintPeiPolicyPpiPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                    Status;
  TELEMETRY_PEI_PREMEM_CONFIG   *TelemetryPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "----------------------- Telemetry_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", TelemetryPreMemConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " Telemetry CpuCrashLogDevice : %01d\n", TelemetryPreMemConfig->CpuCrashLogDevice));
  DEBUG ((DEBUG_INFO, " Telemetry StreamTracerMode  : 0x%x\n", TelemetryPreMemConfig->StreamTracerMode));
  DEBUG ((DEBUG_INFO, " Telemetry StreamTracerSize  : 0x%x\n", TelemetryPreMemConfig->StreamTracerSize));
  DEBUG ((DEBUG_INFO, "\n-------------------- Telemetry_CONFIG Print END -----------------\n"));
  return;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TelemetryLoadPeiConfigDefault (
  IN VOID   *ConfigBlockPointer
  )
{
  TELEMETRY_PEI_CONFIG   *TelemetryConfig;

  TelemetryConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "TelemetryConfig->Header.GuidHob.Name = %g\n", &TelemetryConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TelemetryConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TelemetryConfig->Header.GuidHob.Header.HobLength));

  TelemetryConfig->CpuCrashLogEnable = 1;
}

static COMPONENT_BLOCK_ENTRY  mTelemetryIpBlock = {
  &gTelemetryPeiConfigGuid,  sizeof (TELEMETRY_PEI_CONFIG), TELEMETRY_PEI_CONFIG_REVISION, TelemetryLoadPeiConfigDefault
};

/**
  Get Telemetry PEI config block table total size.

  @retval     Size of Telemetry PEI config block table
**/
UINT16
EFIAPI
TelemetryGetPeiConfigBlockTotalSize (
  VOID
  )
{
  return mTelemetryIpBlock.Size;
}

/**
  TelemetryAddConfigBlocks add all Telemetry PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
TelemetryAddPeiConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mTelemetryIpBlock, 1);
  return Status;
}

/**
  This function prints the Telemetry PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
TelemetryPrintPeiPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS             Status;
  TELEMETRY_PEI_CONFIG   *TelemetryPeiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTelemetryPeiConfigGuid, (VOID *) &TelemetryPeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "----------------------- Telemetry_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", TelemetryPeiConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " Telemetry CpuCrashLogEnable : %01d\n", TelemetryPeiConfig->CpuCrashLogEnable));
  DEBUG ((DEBUG_INFO, "\n-------------------- Telemetry_CONFIG Print END -----------------\n"));
  return;
}
