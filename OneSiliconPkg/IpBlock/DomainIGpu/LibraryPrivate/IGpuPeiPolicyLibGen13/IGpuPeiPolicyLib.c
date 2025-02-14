/** @file
  This file provides services for PEI Graphics policy default initialization

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

#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Defines/IgdDefines.h>
#include <SiPolicyStruct.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
IGpuLoadPeiPreMemDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  IGPU_PEI_PREMEM_CONFIG                         *IGpuPreMemConfig;

  IGpuPreMemConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "IGpuPreMemConfig->Header.GuidHob.Name = %g\n", &IGpuPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IGpuPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IGpuPreMemConfig->Header.GuidHob.Header.HobLength));

  ///
  /// Initialize IGPU Pre-Mem policies
  ///
  IGpuPreMemConfig->LMemBar             = 0xB0000000;
  IGpuPreMemConfig->GttMmAdr            = 0xAF000000;
  IGpuPreMemConfig->IgdDvmt50PreAlloc   = 2;
  IGpuPreMemConfig->InternalGraphics    = 2;
  IGpuPreMemConfig->PanelPowerEnable    = 1;
  IGpuPreMemConfig->IGpuGsm2Size        = 0xFF; // 0xFF no allocation
  IGpuPreMemConfig->MemoryBandwidthCompression = 1;

  //
  // Initialize the default VBT settings
  //
  IGpuPreMemConfig->DdiConfiguration.DdiPortAConfig = DdiPortEdp;
  IGpuPreMemConfig->DdiConfiguration.DdiPortBConfig = DdiPortDisabled;
  IGpuPreMemConfig->DdiConfiguration.DdiPortAHpd = DdiHpdDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPortBHpd = DdiHpdEnable;
  IGpuPreMemConfig->DdiConfiguration.DdiPortCHpd = DdiHpdDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPort1Hpd = DdiHpdDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPort2Hpd = DdiHpdDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPort3Hpd = DdiHpdDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPort4Hpd = DdiHpdDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPortADdc = DdiDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPortBDdc = DdiDdcEnable;
  IGpuPreMemConfig->DdiConfiguration.DdiPortCDdc = DdiDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPort1Ddc = DdiDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPort2Ddc = DdiDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPort3Ddc = DdiDisable;
  IGpuPreMemConfig->DdiConfiguration.DdiPort4Ddc = DdiDisable;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
IGpuLoadPeiDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  IGPU_PEI_CONFIG     *IGpuConfig;

  IGpuConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "IGpuConfig Name = %g\n", &IGpuConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IGpuConfig HobLength = 0x%x\n", IGpuConfig->Header.GuidHob.Header.HobLength));

  //
  // Initialize the Graphics configuration
  //
  IGpuConfig->RenderStandby       = 1;
  IGpuConfig->PavpEnable          = 1;
  IGpuConfig->PeiGtConfig.ConfigureGT         = 1;
  IGpuConfig->PeiMediaConfig.ConfigureMedia      = 1;
  IGpuConfig->PeiDisplayConfig.BltBufferAddress    = NULL;
  IGpuConfig->MediaStandby  = 1;
  IGpuConfig->PeiDisplayConfig.PeiGraphicsPeimInit = 1;
}

static COMPONENT_BLOCK_ENTRY  mGraphicsIpBlocksPreMem = {
  &gGraphicsPeiPreMemConfigGuid, sizeof (IGPU_PEI_PREMEM_CONFIG), IGPU_PEI_PREMEM_CONFIG_REVISION, IGpuLoadPeiPreMemDefault
};

static COMPONENT_BLOCK_ENTRY  mGraphicsIpBlocks = {
  &gGraphicsPeiConfigGuid, sizeof (IGPU_PEI_CONFIG), IGPU_PEI_CONFIG_REVISION, IGpuLoadPeiDefault
};

/**
  Get Graphics PEI config block table total size.

  @retval     Size of Graphics PEI config block table
**/
UINT16
EFIAPI
IGpuGetConfigBlockTotalSize (
  VOID
  )
{
  return mGraphicsIpBlocks.Size;
}

/**
  Get Graphics PEI Pre-Mem config block table total size.

  @retval      Size of Graphics PEI Pre-Mem config block table
**/
UINT16
EFIAPI
IGpuGetConfigBlockTotalSizePreMem  (
  VOID
  )
{
  return mGraphicsIpBlocksPreMem.Size;
}

/**
  GraphicsAddConfigBlocksPreMem add all Graphics PEI PreMem config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IGpuAddConfigBlocksPreMem  (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mGraphicsIpBlocksPreMem, 1);
  return Status;
}

/**
  GraphicsAddConfigBlocks add all Graphics PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IGpuAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mGraphicsIpBlocks, 1);
  return Status;
}


/**
  This function prints the PEI phase Graphics PreMem policy.

  @param[in] SiPolicyPreMemPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
IGpuPrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                            Status;
  IGPU_PEI_PREMEM_CONFIG                *IGpuPreMemConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *)SiPolicyPreMemPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "IGPU_PEI_PREMEM_CONFIG Print START\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", IGpuPreMemConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " PanelPowerEnable : 0x%x\n", IGpuPreMemConfig->PanelPowerEnable));
  DEBUG ((DEBUG_INFO, " IgdDvmt50PreAlloc : 0x%x\n", IGpuPreMemConfig->IgdDvmt50PreAlloc));
  DEBUG ((DEBUG_INFO, " InternalGraphics : 0x%x\n", IGpuPreMemConfig->InternalGraphics));
  DEBUG ((DEBUG_INFO, " PrimaryDisplay : 0x%x\n", IGpuPreMemConfig->PrimaryDisplay));
  DEBUG ((DEBUG_INFO, " GttMmAdr : 0x%lx\n", IGpuPreMemConfig->GttMmAdr));
  DEBUG ((DEBUG_INFO, " LMemBar : 0x%lx\n", IGpuPreMemConfig->LMemBar));
  DEBUG ((DEBUG_INFO, " IGpuGsm2Size : 0x%x\n", IGpuPreMemConfig->IGpuGsm2Size));
  DEBUG ((DEBUG_INFO, " DdiPortAConfig : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPortAConfig));
  DEBUG ((DEBUG_INFO, " DdiPortBConfig : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPortBConfig));
  DEBUG ((DEBUG_INFO, " DdiPortAHpd : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPortAHpd));
  DEBUG ((DEBUG_INFO, " DdiPortBHpd : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPortBHpd));
  DEBUG ((DEBUG_INFO, " DdiPortCHpd : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPortCHpd));
  DEBUG ((DEBUG_INFO, " DdiPort1Hpd : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPort1Hpd));
  DEBUG ((DEBUG_INFO, " DdiPort2Hpd : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPort2Hpd));
  DEBUG ((DEBUG_INFO, " DdiPort3Hpd : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPort3Hpd));
  DEBUG ((DEBUG_INFO, " DdiPort4Hpd : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPort4Hpd));
  DEBUG ((DEBUG_INFO, " DdiPortADdc : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPortADdc));
  DEBUG ((DEBUG_INFO, " DdiPortBDdc : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPortBDdc));
  DEBUG ((DEBUG_INFO, " DdiPortCDdc : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPortCDdc));
  DEBUG ((DEBUG_INFO, " DdiPort1Ddc : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPort1Ddc));
  DEBUG ((DEBUG_INFO, " DdiPort2Ddc : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPort2Ddc));
  DEBUG ((DEBUG_INFO, " DdiPort3Ddc : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPort3Ddc));
  DEBUG ((DEBUG_INFO, " DdiPort4Ddc : 0x%x\n", IGpuPreMemConfig->DdiConfiguration.DdiPort4Ddc));
  DEBUG ((DEBUG_INFO, " DeltaT12PowerCycleDelay : 0x%x\n", IGpuPreMemConfig->DeltaT12PowerCycleDelay));
  DEBUG ((DEBUG_INFO, " MemoryBandwidthCompression : 0x%x\n", IGpuPreMemConfig->MemoryBandwidthCompression));
  DEBUG ((DEBUG_INFO, " OemT12DelayOverride : 0x%x\n", IGpuPreMemConfig->OemT12DelayOverride));
  DEBUG ((DEBUG_INFO, "IGPU_PEI_PREMEM_CONFIG Print END\n"));
  DEBUG_CODE_END ();
  return;
}

/**
  This function prints the Graphics PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
IGpuPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS        Status;
  IGPU_PEI_CONFIG   *IGpuConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &IGpuConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "IGPU_PEI_CONFIG Print START\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", IGpuConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " RenderStandby : 0x%x\n", IGpuConfig->RenderStandby));
  DEBUG ((DEBUG_INFO, " ConfigureGT : 0x%x\n", IGpuConfig->PeiGtConfig.ConfigureGT));
  DEBUG ((DEBUG_INFO, " ConfigureMedia : 0x%x\n", IGpuConfig->PeiMediaConfig.ConfigureMedia));
  DEBUG ((DEBUG_INFO, " PavpEnable : 0x%x\n", IGpuConfig->PavpEnable));
  DEBUG ((DEBUG_INFO, " PeiGraphicsPeimInit : 0x%x\n", IGpuConfig->PeiDisplayConfig.PeiGraphicsPeimInit));
  DEBUG ((DEBUG_INFO, " BltBufferAdress : 0x%x\n", IGpuConfig->PeiDisplayConfig.BltBufferAddress));
  DEBUG ((DEBUG_INFO, " GraphicsConfigPtr : 0x%x\n", IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr));
  DEBUG ((DEBUG_INFO, " RC1pGtFreqEnable : 0x%x\n", IGpuConfig->PeiGtConfig.RC1pGtFreqEnable));
  DEBUG ((DEBUG_INFO, " RC1pMediaFreqEnable : 0x%x\n", IGpuConfig->PeiMediaConfig.RC1pMediaFreqEnable));
  DEBUG ((DEBUG_INFO, " MediaStandby : 0x%x\n", IGpuConfig->MediaStandby));
  DEBUG ((DEBUG_INFO, "IGPU_PEI_CONFIG Print END\n"));

  DEBUG_CODE_END ();
  return;
}
