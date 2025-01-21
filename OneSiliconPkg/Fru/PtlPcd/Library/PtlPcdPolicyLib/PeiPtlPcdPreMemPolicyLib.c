/** @file

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

**/

#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataSocLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PchPcieRpLib.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <LpcConfig.h>
#include <HsioPcieConfig.h>
#include <PchPcieRpConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchGeneralPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_GENERAL_PREMEM_CONFIG  *PchGeneralPreMemConfig;
  PchGeneralPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchGeneralPreMemConfig->Header.GuidHob.Name = %g\n", &PchGeneralPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchGeneralPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchGeneralPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    PCH general premem configuration
  ********************************/
}



/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadLpcPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_LPC_PREMEM_CONFIG  *LpcPreMemConfig;
  LpcPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "LpcPreMemConfig->Header.GuidHob.Name = %g\n", &LpcPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "LpcPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", LpcPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    LPC Configuration
  ********************************/
  LpcPreMemConfig->EnhancePort8xhDecoding     = TRUE;
  LpcPreMemConfig->LpcPmHAE                   = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHsioPciePreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HSIO_PCIE_PREMEM_CONFIG  *HsioPciePreMemConfig;
  HsioPciePreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HsioPciePreMemConfig->Header.GuidHob.Name = %g\n", &HsioPciePreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HsioPciePreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HsioPciePreMemConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieRpPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PCIE_RP_PREMEM_CONFIG  *PcieRpPreMemConfig;
  UINT32                     Index;

  PcieRpPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PcieRpPreMemConfig->Header.GuidHob.Name = %g\n", &PcieRpPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PcieRpPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PcieRpPreMemConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index ++) {
    PcieRpPreMemConfig->RpEnabledMask |= (UINT32) (1 << Index);
  }
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index++) {
    PcieRpPreMemConfig->PcieClock[Index].Usage = (UINT8) PchClockUsageNotUsed;
    // PcieRpPreMemConfig->PcieClock[Index].ClkReq = GetClkReqForClkSrc ((UINT8) Index);
  }
}

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mSocMIpBlocksPreMem [] = {
  {&gPchGeneralPreMemConfigGuid,     sizeof (PCH_GENERAL_PREMEM_CONFIG),    PCH_GENERAL_PREMEM_CONFIG_REVISION,        LoadPchGeneralPreMemConfigDefault},
  {&gLpcPreMemConfigGuid,            sizeof (PCH_LPC_PREMEM_CONFIG),        PCH_LPC_PREMEM_CONFIG_REVISION,            LoadLpcPreMemConfigDefault},
  {&gHsioPciePreMemConfigGuid,       sizeof (PCH_HSIO_PCIE_PREMEM_CONFIG),  PCH_HSIO_PCIE_PREMEM_CONFIG_REVISION,      LoadHsioPciePreMemConfigDefault},
  {&gPcieRpPreMemConfigGuid,         sizeof (PCH_PCIE_RP_PREMEM_CONFIG),    PCH_PCIE_RP_PREMEM_CONFIG_REVISION,        LoadPcieRpPreMemConfigDefault}
};

/**
  Get PCH PREMEM config block table total size.

  @retval                               Size of PCH PREMEM config block table
**/
UINT16
EFIAPI
PtlPcdGetPreMemConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mSocMIpBlocksPreMem[0], ARRAY_SIZE (mSocMIpBlocksPreMem));
}

/**
  PchAddPreMemConfigBlocks add all PCH PREMEM config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH PREMEM config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PtlPcdAddPreMemConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "PCH AddPreMemConfigBlocks\n"));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mSocMIpBlocksPreMem[0], ARRAY_SIZE (mSocMIpBlocksPreMem));
}
