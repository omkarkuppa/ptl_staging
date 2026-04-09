/** @file
  This file provides services for PEI HostBridge policy default initialization

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
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HostBridgeLoadPeiConfigPreMemDefault (
  IN VOID   *ConfigBlockPointer
  )
{
  HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig;

  HostBridgePreMemConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "HostBridgePreMemConfig->Header.GuidHob.Name = %g\n", &HostBridgePreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HostBridgePreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HostBridgePreMemConfig->Header.GuidHob.Header.HobLength));

  HostBridgePreMemConfig->MchBar              = PcdGet64 (PcdMchBaseAddress);
  HostBridgePreMemConfig->RegBar              = PcdGet64 (PcdRegBarBaseAddress);
  HostBridgePreMemConfig->SafBar              = PcdGet64 (PcdSafBarBaseAddress);

  HostBridgePreMemConfig->ApicLocalAddress    = 0xFEE00000;
  HostBridgePreMemConfig->NvmeHcPeiMmioBase   = 0xA0000000;
  HostBridgePreMemConfig->NvmeHcPeiMmioLimit  = 0xA1000000;
  HostBridgePreMemConfig->AhciPeiMmioBase     = 0x90000000;
  HostBridgePreMemConfig->AhciPeiMmioLimit    = 0x91000000;
  HostBridgePreMemConfig->EcExtraIoBase       = 0x6A0;
  HostBridgePreMemConfig->SioBaseAddress      = 0x0680;
  HostBridgePreMemConfig->VmdCfgBarBar        = 0xA0000000;
  HostBridgePreMemConfig->VmdMemBar1Bar       = 0xA2000000;
  HostBridgePreMemConfig->VmdMemBar2Bar       = 0xA4000000;
  HostBridgePreMemConfig->FabricGvEnable      = 0x1;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HostBridgeLoadPeiConfigDefault (
  IN VOID   *ConfigBlockPointer
  )
{
  HOST_BRIDGE_PEI_CONFIG   *HostBridgePeiConfig;

  HostBridgePeiConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "HostBridgePeiConfig->Header.GuidHob.Name = %g\n", &HostBridgePeiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HostBridgePeiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HostBridgePeiConfig->Header.GuidHob.Header.HobLength));

  HostBridgePeiConfig->Device4Enable = 1;
}


static COMPONENT_BLOCK_ENTRY  mHostBridgeIpBlockPreMem = {
  &gHostBridgePeiPreMemConfigGuid,  sizeof (HOST_BRIDGE_PREMEM_CONFIG), HOST_BRIDGE_PREMEM_CONFIG_REVISION, HostBridgeLoadPeiConfigPreMemDefault
};

static COMPONENT_BLOCK_ENTRY  mHostBridgeIpBlocks = {
  &gHostBridgePeiConfigGuid, sizeof (HOST_BRIDGE_PEI_CONFIG), HOST_BRIDGE_PEI_CONFIG_REVISION, HostBridgeLoadPeiConfigDefault
};

/**
  Get HostBridge PEI PreMem config block table total size.

  @retval     Size of HostBridge PEI PreMem config block table
**/
UINT16
EFIAPI
HostBridgeGetPeiConfigBlockTotalSizePreMem (
  VOID
  )
{
  return mHostBridgeIpBlockPreMem.Size;
}

/**
  Get HostBridge PEI phase config block table total size.

  @retval     Size of HostBridge PEI config block table
**/
UINT16
EFIAPI
HostBridgeGetPeiConfigBlockTotalSize (
  VOID
  )
{
  return mHostBridgeIpBlocks.Size;
}

/**
  HostBridgeAddPeiConfigBlockPreMem add all HostBridge PreMem PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
HostBridgeAddPeiConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mHostBridgeIpBlockPreMem, 1);
  return Status;
}

/**
  HostBridgeAddPeiConfigBlock add all HostBridge PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
HostBridgeAddPeiConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mHostBridgeIpBlocks, 1);
  return Status;
}

/**
  Print HostBridge PEI PreMem Config

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
HostBridgePreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                  Status;
  HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "Revision:%d\n", HostBridgePreMemConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, "MchBar:0x%x\n", HostBridgePreMemConfig->MchBar));
  DEBUG ((DEBUG_INFO, "RegBar:0x%x\n", HostBridgePreMemConfig->RegBar));
  DEBUG ((DEBUG_INFO, "CridEnable:%d\n", HostBridgePreMemConfig->CridEnable));
  DEBUG ((DEBUG_INFO, "SafBar:0x%lx\n", HostBridgePreMemConfig->SafBar));
  DEBUG ((DEBUG_INFO, "FabricGvEnable:%d\n", HostBridgePreMemConfig->FabricGvEnable));
}

/**
  Print HostBridge PEI Config

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
HostBridgePeiPrintConfig (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS                  Status;
  HOST_BRIDGE_PEI_CONFIG      *HostBridgePeiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHostBridgePeiConfigGuid, (VOID *) &HostBridgePeiConfig);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "Revision : %d\n", HostBridgePeiConfig->Header.Revision));

  DEBUG ((DEBUG_INFO, "Device4Enable : 0x%x\n", HostBridgePeiConfig->Device4Enable));
  DEBUG ((DEBUG_INFO, "SkipPamLock : 0x%x\n", HostBridgePeiConfig->SkipPamLock));

}