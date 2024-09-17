/** @file
 Intel PEI SA Pre-Memory Policy update by board configuration

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include "PeiPolicyBoardConfig.h"
#include <TcssPeiPreMemConfig.h>
#include <Ppi/FspmArchConfigPpi.h>
#include <VtdConfig.h>
#include <MemorySubSystemConfig.h>
#include <FspmUpd.h>
#include <Library/PreSiliconEnvDetectLib.h>

/**
  This function performs PEI SA Pre-Memory Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfigPreMem (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                         Status;
  BOOLEAN                            ExternalSpdPresent;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  TCSS_PEI_PREMEM_CONFIG             *TcssPeiPreMemConfig;
  HOST_BRIDGE_PREMEM_CONFIG          *HostBridgePreMemConfig;
  MEMORY_CONFIG_NO_CRC               *MemConfigNoCrc;
  VTD_CONFIG                         *VtdConfig;
  MEMORY_SUB_SYSTEM_CONFIG           *MemorySubSystemConfig;
  MEMORY_CONFIGURATION               *MemConfig;
  FSPM_ARCH_CONFIG_PPI               *FspmArchConfigPpi;
  EFI_PEI_PPI_DESCRIPTOR             *FspmArchConfigPpiDesc;
#endif

  DEBUG ((DEBUG_INFO, "OpenBoard Updating SA Policy by board config in Pre-Mem\n"));
  Status = EFI_SUCCESS;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  TcssPeiPreMemConfig    = NULL;
  HostBridgePreMemConfig = NULL;
  MemConfigNoCrc         = NULL;
  VtdConfig              = NULL;
  MemorySubSystemConfig  = NULL;
  MemConfig              = NULL;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *) &VtdConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemorySubSystemConfigGuid, (VOID *) &MemorySubSystemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR(Status);
#endif

  if (IsSimicsEnvironment()) {
    PcdSetBoolS (PcdSpdPresent, TRUE);
  }

  // Assume internal SPD is used
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[0], MemConfigNoCrc->SpdAddressTable[0], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[1], MemConfigNoCrc->SpdAddressTable[1], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[2], MemConfigNoCrc->SpdAddressTable[2], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[3], MemConfigNoCrc->SpdAddressTable[3], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[4], MemConfigNoCrc->SpdAddressTable[4], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[5], MemConfigNoCrc->SpdAddressTable[5], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[6], MemConfigNoCrc->SpdAddressTable[6], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[7], MemConfigNoCrc->SpdAddressTable[7], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[8], MemConfigNoCrc->SpdAddressTable[8], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[9], MemConfigNoCrc->SpdAddressTable[9], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[10], MemConfigNoCrc->SpdAddressTable[10], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[11], MemConfigNoCrc->SpdAddressTable[11], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[12], MemConfigNoCrc->SpdAddressTable[12], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[13], MemConfigNoCrc->SpdAddressTable[13], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[14], MemConfigNoCrc->SpdAddressTable[14], 0);
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[15], MemConfigNoCrc->SpdAddressTable[15], 0);

  // Check for external SPD presence
  ExternalSpdPresent = PcdGetBool (PcdSpdPresent);
  if (ExternalSpdPresent) {
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[0], MemConfigNoCrc->SpdAddressTable[0], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[0]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[1], MemConfigNoCrc->SpdAddressTable[1], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[1]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[2], MemConfigNoCrc->SpdAddressTable[2], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[2]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[3], MemConfigNoCrc->SpdAddressTable[3], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[3]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[4], MemConfigNoCrc->SpdAddressTable[4], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[4]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[5], MemConfigNoCrc->SpdAddressTable[5], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[5]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[6], MemConfigNoCrc->SpdAddressTable[6], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[6]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[7], MemConfigNoCrc->SpdAddressTable[7], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[7]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[8], MemConfigNoCrc->SpdAddressTable[8], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[8]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[9], MemConfigNoCrc->SpdAddressTable[9], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[9]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[10], MemConfigNoCrc->SpdAddressTable[10], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[10]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[11], MemConfigNoCrc->SpdAddressTable[11], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[11]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[12], MemConfigNoCrc->SpdAddressTable[12], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[12]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[13], MemConfigNoCrc->SpdAddressTable[13], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[13]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[14], MemConfigNoCrc->SpdAddressTable[14], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[14]);
    UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[15], MemConfigNoCrc->SpdAddressTable[15], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[15]);
  }

  if (((MRC_DQS *)PcdGetPtr (VpdPcdMrcDqsMapCpu2Dram))->DqsMapCpu2Dram) {
    COPY_POLICY_V2 ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.DqsMapCpu2DramMc0Ch0, (VOID *)MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram, (VOID *)(UINTN)(((MRC_DQS *)PcdGetPtr (VpdPcdMrcDqsMapCpu2Dram))->DqsMapCpu2Dram), sizeof (UINT8) * MEM_CFG_MAX_CONTROLLERS * MEM_CFG_MAX_CHANNELS * MEM_CFG_NUM_BYTES_MAPPED);
  }
  if (((MRC_DQ *)PcdGetPtr (VpdPcdMrcDqMapCpu2Dram))->DqMapCpu2Dram) {
    COPY_POLICY_V2 ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.DqMapCpu2DramMc0Ch0, (VOID *)MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram, (VOID *)(UINTN)(((MRC_DQ *)PcdGetPtr (VpdPcdMrcDqMapCpu2Dram))->DqMapCpu2Dram), sizeof (UINT8) * MEM_CFG_MAX_CONTROLLERS * MEM_CFG_MAX_CHANNELS * MEM_CFG_NUM_BYTES_MAPPED * 8);
  }

  if (((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->OverrideSpd) {
    if ((UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData) {
      DEBUG((DEBUG_INFO, "VpdPcdMrcSpdData != NULL, MemConfigNoCrc->SpdData\n"));
      COPY_POLICY_V2 ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr000,(VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][0][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
      COPY_POLICY_V2 ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr010,(VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][1][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
      COPY_POLICY_V2 ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr020, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][2][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
      COPY_POLICY_V2 ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr030, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][3][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
      COPY_POLICY_V2 ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr100, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][0][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
      COPY_POLICY_V2 ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr110, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][1][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
      COPY_POLICY_V2 ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr120, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][2][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
      COPY_POLICY_V2 ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr130, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][3][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
    }
  }

  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.MmioSizeAdjustment, HostBridgePreMemConfig->MmioSizeAdjustment, PcdGet16 (PcdSaMiscMmioSizeAdjustment));

  COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.DdrFreqLimit, MemConfig->ExternalInputs.DdrFreqLimit,    0);
  COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.Lp5CccConfig, MemConfig->ExternalInputs.Lp5CccConfig,    PcdGet8 (VpdPcdMrcLp5CccConfig)); // BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].  0 selects Ascending mapping and 1 selects Descending mapping.
  COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.CmdMirror, MemConfig->ExternalInputs.CmdMirror,          PcdGet8 (VpdPcdMrcCmdMirror)); // BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].  0 = No Command Mirror and 1 = Command Mirror.

  ///
  /// Initialize the VTD Configuration
  ///
  UPDATE_POLICY_V2 (((FSPS_UPD *) FspsUpd)->FspsConfig.VtdDisable, VtdConfig->VtdDisable, 1);

#if FixedPcdGet8(PcdFspModeSelection) == 0
  FspmArchConfigPpi = (FSPM_ARCH_CONFIG_PPI *) AllocateZeroPool (sizeof (FSPM_ARCH_CONFIG_PPI));
  if (FspmArchConfigPpi == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  FspmArchConfigPpi->Revision            = 1;
  FspmArchConfigPpi->NvsBufferPtr        = NULL;
  FspmArchConfigPpi->BootLoaderTolumSize = 0;

  FspmArchConfigPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (FspmArchConfigPpiDesc == NULL) {
    ASSERT (FALSE);
    FreePool (FspmArchConfigPpi);
    return EFI_OUT_OF_RESOURCES;
  }
  FspmArchConfigPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  FspmArchConfigPpiDesc->Guid  = &gFspmArchConfigPpiGuid;
  FspmArchConfigPpiDesc->Ppi   = FspmArchConfigPpi;
  //
  // Install FSP-M Arch Config PPI
  //
  Status = PeiServicesInstallPpi (FspmArchConfigPpiDesc);
  ASSERT_EFI_ERROR (Status);
#endif

  return Status;
}
