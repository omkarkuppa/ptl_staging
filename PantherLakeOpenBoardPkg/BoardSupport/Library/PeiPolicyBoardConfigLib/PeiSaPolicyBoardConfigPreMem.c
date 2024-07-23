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
  TCSS_PEI_PREMEM_CONFIG             *TcssPeiPreMemConfig;
  HOST_BRIDGE_PREMEM_CONFIG          *HostBridgePreMemConfig;
  MEMORY_CONFIG_NO_CRC               *MemConfigNoCrc;
  VTD_CONFIG                         *VtdConfig;
  MEMORY_SUB_SYSTEM_CONFIG           *MemorySubSystemConfig;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  FSPM_ARCH_CONFIG_PPI               *FspmArchConfigPpi;
  EFI_PEI_PPI_DESCRIPTOR             *FspmArchConfigPpiDesc;
#endif
  DEBUG ((DEBUG_INFO, "Updating SA Policy by board config in Pre-Mem\n"));

  Status = EFI_SUCCESS;
  TcssPeiPreMemConfig    = NULL;
  HostBridgePreMemConfig = NULL;
  MemConfigNoCrc         = NULL;
  VtdConfig              = NULL;
  MemorySubSystemConfig  = NULL;

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

  ZeroMem ((VOID *) MemConfigNoCrc->SpdData->SpdData, sizeof (SPD_DATA_BUFFER));

  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[0], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[0]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[1], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[1]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[2], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[2]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[3], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[3]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[4], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[4]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[5], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[5]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[6], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[6]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[7], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[7]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[8], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[8]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[9], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[9]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[10], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[10]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[11], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[11]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[12], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[12]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[13], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[13]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[14], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[14]);
  UPDATE_POLICY (MemConfigNoCrc->SpdAddressTable[15], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[15]);

  if (((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->OverrideSpd) {
    if ((UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData) {
      DEBUG((DEBUG_INFO, "VpdPcdMrcSpdData != NULL, MemConfigNoCrc->SpdData\n"));
      COPY_POLICY ((VOID *)(UINTN)MemConfigNoCrc->SpdData->SpdData[0][0][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, PcdGet16 (VpdPcdMrcSpdDataSize));
      COPY_POLICY ((VOID *)(UINTN)MemConfigNoCrc->SpdData->SpdData[0][1][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, PcdGet16 (VpdPcdMrcSpdDataSize));
      COPY_POLICY ((VOID *)(UINTN)MemConfigNoCrc->SpdData->SpdData[0][2][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, PcdGet16 (VpdPcdMrcSpdDataSize));
      COPY_POLICY ((VOID *)(UINTN)MemConfigNoCrc->SpdData->SpdData[0][3][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, PcdGet16 (VpdPcdMrcSpdDataSize));
      COPY_POLICY ((VOID *)(UINTN)MemConfigNoCrc->SpdData->SpdData[1][0][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, PcdGet16 (VpdPcdMrcSpdDataSize));
      COPY_POLICY ((VOID *)(UINTN)MemConfigNoCrc->SpdData->SpdData[1][1][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, PcdGet16 (VpdPcdMrcSpdDataSize));
      COPY_POLICY ((VOID *)(UINTN)MemConfigNoCrc->SpdData->SpdData[1][2][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, PcdGet16 (VpdPcdMrcSpdDataSize));
      COPY_POLICY ((VOID *)(UINTN)MemConfigNoCrc->SpdData->SpdData[1][3][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, PcdGet16 (VpdPcdMrcSpdDataSize));
    }
  }

  UPDATE_POLICY (HostBridgePreMemConfig->MmioSizeAdjustment, PcdGet16 (PcdSaMiscMmioSizeAdjustment));

  ///
  /// Initialize the VTD Configuration
  ///
  UPDATE_POLICY (VtdConfig->VtdDisable,   1);

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
