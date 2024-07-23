/** @file
  This file load BIOS Guard and config block.

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
  Print BIOS_GUARD_CONFIG and serial out.

  @param[in] SiPolicyPpi  Instance of SI_POLICY_PPI
**/
VOID
BiosGuardConfigPrint (
  IN  SI_POLICY_PPI       *SiPolicyPpi
  )
{
  BIOS_GUARD_CONFIG  *BiosGuardConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gBiosGuardConfigGuid, (VOID *) &BiosGuardConfig);
  ASSERT_EFI_ERROR(Status);

  DEBUG ((DEBUG_INFO, "------------------ BIOS_GUARD_CONFIG Begin ------------------\n"));
  DEBUG ((DEBUG_INFO, " \n"));
  DEBUG ((DEBUG_INFO, " Platform attributes : 0x%X\n", BiosGuardConfig->BiosGuardAttr));
  DEBUG ((DEBUG_INFO, " BiosGuardModulePtr : 0x%016llX\n", BiosGuardConfig->BiosGuardModulePtr));
  DEBUG ((DEBUG_INFO, " EcProvisionEav : 0x%016llX\n", BiosGuardConfig->EcProvisionEav));
  DEBUG ((DEBUG_INFO, " EcBiosGuardCmdLock : 0x%016llX\n", BiosGuardConfig->EcBiosGuardCmdLock));
  DEBUG ((DEBUG_INFO, "------------------ BIOS_GUARD_CONFIG End ------------------\n"));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadBiosGuardConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  BIOS_GUARD_CONFIG  *BiosGuardConfig;
  BiosGuardConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "BiosGuardConfig->Header.GuidHob.Name = %g\n", &BiosGuardConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "BiosGuardConfig->Header.GuidHob.Header.HobLength = 0x%x\n", BiosGuardConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU BIOS Guard configuration
  ********************************/
    BiosGuardConfig->BiosGuardModulePtr = 0xFFFFFFFFFFFFFFFF;
    BiosGuardConfig->BiosGuardAttr      = 0xFFFFFFFF;
    BiosGuardConfig->EcProvisionEav     = 0xFFFFFFFFFFFFFFFF;
    BiosGuardConfig->EcBiosGuardCmdLock = 0xFFFFFFFFFFFFFFFF;
}

STATIC COMPONENT_BLOCK_ENTRY mBiosGuardIpBlock = {
  &gBiosGuardConfigGuid,
  sizeof (BIOS_GUARD_CONFIG),
  BIOS_GUARD_CONFIG_REVISION,
  LoadBiosGuardConfigDefault
};

/**
  Get BIOS Guard config block total size.

  @retval Size of config block
**/
UINT16
BiosGuardGetConfigBlockTotalSize (
  VOID
  )
{
  return mBiosGuardIpBlock.Size;
}

/**
  Add BIOS Guard ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
BiosGuardInitAddConfigBlocks (
  IN VOID *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mBiosGuardIpBlock, 1);
  return Status;
}
