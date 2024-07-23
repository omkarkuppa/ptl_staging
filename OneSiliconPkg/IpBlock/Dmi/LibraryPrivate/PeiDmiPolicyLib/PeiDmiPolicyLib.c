/** @file
  This file provides services for PEI DMI policy default initialization

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

#include <Library/DebugLib.h>
#include <Library/PeiDmiPolicyLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <PeiDmiConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPeiDmiDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  UINT8                       Index;
  PEI_DMI_CONFIG              *PeiDmiConfig;

  DEBUG ((DEBUG_INFO, "[Dmi] LoadPeiDmiDefault Start\n"));

  PeiDmiConfig = ConfigBlockPointer;
  if (PeiDmiConfig == NULL) {
    return;
  }

  PeiDmiConfig->DmiMaxLinkSpeed                            = 1;
  PeiDmiConfig->DmiAspm                                    = DmiAspmAutoConfig;
  PeiDmiConfig->DmiAspmCtrl                                = DmiAspmAutoConfig;
  PeiDmiConfig->DmiAspmL1ExitLatency                       = 6;
  PeiDmiConfig->DmiHweq                                    = 2;
  PeiDmiConfig->DmiGen3DsPresetEnable                      = 0;
  PeiDmiConfig->DmiGen3UsPresetEnable                      = 0;
  PeiDmiConfig->DmiL1ssEnable                              = 0;
  PeiDmiConfig->Gen3EqPhase23Bypass                        = 0;
  PeiDmiConfig->Gen3EqPhase3Bypass                         = 0;
  PeiDmiConfig->Gen3LtcoEnable                             = 0;
  PeiDmiConfig->Gen3RtcoRtpoEnable                         = 0;

  PeiDmiConfig->DmiGen4DsPresetEnable                      = 0;
  PeiDmiConfig->DmiGen4UsPresetEnable                      = 0;
  PeiDmiConfig->Gen4EqPhase23Bypass                        = 0;
  PeiDmiConfig->Gen4EqPhase3Bypass                         = 0;
  PeiDmiConfig->Gen4LtcoEnable                             = 0;
  PeiDmiConfig->Gen4RtcoRtpoEnable                         = 0;
  for (Index = 0; Index < SA_DMI_MAX_LANE; Index++) {
    PeiDmiConfig->DmiGen3DsPortRxPreset[Index]             = 7;
    PeiDmiConfig->DmiGen3DsPortTxPreset[Index]             = 7;
    PeiDmiConfig->DmiGen3UsPortRxPreset[Index]             = 7;
    PeiDmiConfig->DmiGen3UsPortTxPreset[Index]             = 7;
    PeiDmiConfig->DmiGen3Ltcpre[Index]                     = 2;
    PeiDmiConfig->DmiGen3Ltcpo[Index]                      = 2;
    PeiDmiConfig->DmiGen4DsPortTxPreset[Index]             = 7;
    PeiDmiConfig->DmiGen4UsPortTxPreset[Index]             = 7;
    PeiDmiConfig->DmiGen4Ltcpre[Index]                     = 0;
    PeiDmiConfig->DmiGen4Ltcpo[Index]                      = 0;
  }

  PeiDmiConfig->DmiHwEqGen3CoeffList[0].Cp           = 4;
  PeiDmiConfig->DmiHwEqGen3CoeffList[0].Cm           = 5;
  PeiDmiConfig->DmiHwEqGen3CoeffList[1].Cp           = 6;
  PeiDmiConfig->DmiHwEqGen3CoeffList[1].Cm           = 7;
  PeiDmiConfig->DmiHwEqGen3CoeffList[2].Cp           = 8;
  PeiDmiConfig->DmiHwEqGen3CoeffList[2].Cm           = 8;
  PeiDmiConfig->DmiHwEqGen3CoeffList[3].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[3].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[4].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[4].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[5].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[5].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[6].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[6].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[7].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen3CoeffList[7].Cm           = 0;

  PeiDmiConfig->DmiHwEqGen4CoeffList[0].Cp           = 4;
  PeiDmiConfig->DmiHwEqGen4CoeffList[0].Cm           = 5;
  PeiDmiConfig->DmiHwEqGen4CoeffList[1].Cp           = 6;
  PeiDmiConfig->DmiHwEqGen4CoeffList[1].Cm           = 7;
  PeiDmiConfig->DmiHwEqGen4CoeffList[2].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[2].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[3].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[3].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[4].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[4].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[5].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[5].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[6].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[6].Cm           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[7].Cp           = 0;
  PeiDmiConfig->DmiHwEqGen4CoeffList[7].Cm           = 0;

  DEBUG ((DEBUG_INFO, "[Dmi] LoadPeiDmiDefault End\n"));
}

static COMPONENT_BLOCK_ENTRY  mDmiIpBlocks = {
  &gPeiDmiConfigGuid, sizeof (PEI_DMI_CONFIG), PEI_DMI_CONFIG_REVISION, LoadPeiDmiDefault
};

/**
  Get PEI DMI config block table total size.

  @retval     Size of PEI DMI config block table
**/
UINT16
EFIAPI
DmiGetConfigBlockTotalSize (
  VOID
  )
{
  return mDmiIpBlocks.Size;
}

/**
  DmiAddConfigBlocks add all PEI DMI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
DmiAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mDmiIpBlocks, 1);
  return Status;
}


/**
  This function prints the PEI DMI phase policy.

  @param[in] SiPreMemPolicyPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
DmiPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                  Status;
  UINT8                       Index;
  PEI_DMI_CONFIG              *PeiDmiConfig;

  PeiDmiConfig = NULL;
  Status = EFI_NOT_FOUND;
  DEBUG ((DEBUG_INFO, "[Dmi] DmiPrintPolicyConfig Start\n"));

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPeiDmiConfigGuid, (VOID *) &PeiDmiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n------------------------ DMI Policy (PEI) Print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : 0x%x\n", SiPreMemPolicyPpi->TableHeader.Header.Revision));

  DEBUG ((DEBUG_INFO, " Revision : %d\n", PeiDmiConfig->Header.Revision));
  //
  // Print DpDmi Policy
  //
  DEBUG ((DEBUG_INFO, " DmiMaxLinkSpeed               : %d\n", PeiDmiConfig->DmiMaxLinkSpeed));
  DEBUG ((DEBUG_INFO, " DmiAspm                       : %d\n", PeiDmiConfig->DmiAspm));
  DEBUG ((DEBUG_INFO, " DmiAspmCtrl                   : %d\n", PeiDmiConfig->DmiAspmCtrl));
  DEBUG ((DEBUG_INFO, " DmiAspmL1ExitLatency          : %d\n", PeiDmiConfig->DmiAspmL1ExitLatency));
  DEBUG ((DEBUG_INFO, " DmiL1ssEnable                 : %d\n", PeiDmiConfig->DmiL1ssEnable));
  DEBUG ((DEBUG_INFO, " DmiHweq                       : %d\n", PeiDmiConfig->DmiHweq));
  DEBUG ((DEBUG_INFO, " DmiGen3DsPresetEnable         : %d\n", PeiDmiConfig->DmiGen3DsPresetEnable));
  DEBUG ((DEBUG_INFO, " DmiGen3UsPresetEnable         : %d\n", PeiDmiConfig->DmiGen3UsPresetEnable));
  DEBUG ((DEBUG_INFO, " Gen3EqPhase23Bypass           : %d\n", PeiDmiConfig->Gen3EqPhase23Bypass));
  DEBUG ((DEBUG_INFO, " Gen3EqPhase3Bypass            : %d\n", PeiDmiConfig->Gen3EqPhase3Bypass));
  DEBUG ((DEBUG_INFO, " Gen3LtcoEnable                : %d\n", PeiDmiConfig->Gen3LtcoEnable));
  DEBUG ((DEBUG_INFO, " Gen3RtcoRtpoEnable            : %d\n", PeiDmiConfig->Gen3RtcoRtpoEnable));

  DEBUG ((DEBUG_INFO, " DmiGen4DsPresetEnable         : %d\n", PeiDmiConfig->DmiGen4DsPresetEnable));
  DEBUG ((DEBUG_INFO, " DmiGen4UsPresetEnable         : %d\n", PeiDmiConfig->DmiGen4UsPresetEnable));
  DEBUG ((DEBUG_INFO, " Gen4EqPhase23Bypass           : %d\n", PeiDmiConfig->Gen4EqPhase23Bypass));
  DEBUG ((DEBUG_INFO, " Gen4EqPhase3Bypass            : %d\n", PeiDmiConfig->Gen4EqPhase3Bypass));
  DEBUG ((DEBUG_INFO, " Gen4LtcoEnable                : %d\n", PeiDmiConfig->Gen4LtcoEnable));
  DEBUG ((DEBUG_INFO, " Gen4RtcoRtpoEnable            : %d\n", PeiDmiConfig->Gen4RtcoRtpoEnable));
  for (Index = 0; Index < SA_DMI_MAX_LANE; Index++) {
    DEBUG ((DEBUG_INFO, " DmiGen3DsPortRxPreset[%d]  : %d\n", Index, PeiDmiConfig->DmiGen3DsPortRxPreset[Index]));
    DEBUG ((DEBUG_INFO, " DmiGen3DsPortTxPreset[%d]  : %d\n", Index, PeiDmiConfig->DmiGen3DsPortTxPreset[Index]));
    DEBUG ((DEBUG_INFO, " DmiGen3UsPortRxPreset[%d]  : %d\n", Index, PeiDmiConfig->DmiGen3UsPortRxPreset[Index]));
    DEBUG ((DEBUG_INFO, " DmiGen3UsPortTxPreset[%d]  : %d\n", Index, PeiDmiConfig->DmiGen3UsPortTxPreset[Index]));
    DEBUG ((DEBUG_INFO, " DmiGen3Ltcpre[%d]          : %d\n", Index, PeiDmiConfig->DmiGen3Ltcpre[Index]));
    DEBUG ((DEBUG_INFO, " DmiGen3Ltcpo[%d]           : %d\n", Index, PeiDmiConfig->DmiGen3Ltcpo[Index]));
    DEBUG ((DEBUG_INFO, " DmiHwEqGen3CoeffListCm[%d] : %d\n", Index, PeiDmiConfig->DmiHwEqGen3CoeffList[Index].Cp));
    DEBUG ((DEBUG_INFO, " DmiHwEqGen3CoeffListCp[%d] : %d\n", Index, PeiDmiConfig->DmiHwEqGen3CoeffList[Index].Cm));
    DEBUG ((DEBUG_INFO, " DmiHwEqGen4CoeffListCm[%d] : %d\n", Index, PeiDmiConfig->DmiHwEqGen4CoeffList[Index].Cp));
    DEBUG ((DEBUG_INFO, " DmiHwEqGen4CoeffListCp[%d] : %d\n", Index, PeiDmiConfig->DmiHwEqGen4CoeffList[Index].Cm));
    DEBUG ((DEBUG_INFO, " DmiGen4DsPortTxPreset[%d]  : %d\n", Index, PeiDmiConfig->DmiGen4DsPortTxPreset[Index]));
    DEBUG ((DEBUG_INFO, " DmiGen4UsPortTxPreset[%d]  : %d\n", Index, PeiDmiConfig->DmiGen4UsPortTxPreset[Index]));
    DEBUG ((DEBUG_INFO, " DmiGen4Ltcpre[%d]          : %d\n", Index, PeiDmiConfig->DmiGen4Ltcpre[Index]));
    DEBUG ((DEBUG_INFO, " DmiGen4Ltcpo[%d]           : %d\n", Index, PeiDmiConfig->DmiGen4Ltcpo[Index]));
  }

  DEBUG_CODE_END ();
  return;
}
