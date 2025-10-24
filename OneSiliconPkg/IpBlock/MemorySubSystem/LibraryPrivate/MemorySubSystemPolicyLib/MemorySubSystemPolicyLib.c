/** @file
  This file provides services for Memory SubSystem policy default initialization

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

#include <Library/SiConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <MemorySubSystemConfig.h>
#include <Library/DebugLib.h>
#include <Library/MemorySubSystemPolicyLib.h>
/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadMemorySubSystemDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  MEMORY_SUB_SYSTEM_CONFIG              *MemorySubSystemConfig;

  MemorySubSystemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "MemorySubSystemConfig->Header.GuidHob.Name = %g\n", &MemorySubSystemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MemorySubSystemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", MemorySubSystemConfig->Header.GuidHob.Header.HobLength));
}

static COMPONENT_BLOCK_ENTRY  mMemorySubSystemIpBlock = {
  &gMemorySubSystemConfigGuid, sizeof (MEMORY_SUB_SYSTEM_CONFIG), MEMORY_SUB_SYSTEM_CONFIG_REVISION, LoadMemorySubSystemDefault
};

/**
  Get Memory SubSystem config block table total size.

  @retval      Size of Memory SubSystem PEI Pre-Mem config block table
**/
UINT16
EFIAPI
MemorySubSystemGetConfigBlockTotalSize (
  VOID
  )
{
  return mMemorySubSystemIpBlock.Size;
}

/**
  MemorySubSystemAddConfigBlock add Memory SubSystem config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Memory SubSystem config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MemorySubSystemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mMemorySubSystemIpBlock, 1);
  return Status;
}

/**
  This function prints the Memory SubSystem PreMem policy.

  @param[in] SiPolicyPreMemPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
MemorySubSystemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                            Status;
  MEMORY_SUB_SYSTEM_CONFIG              *MemorySubSystemConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *)SiPolicyPreMemPpi, &gMemorySubSystemConfigGuid, (VOID *) &MemorySubSystemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ MEMORY_SUB_SYSTEM_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", MemorySubSystemConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " AcpiReservedMemoryBase: 0x%x\n", MemorySubSystemConfig->AcpiReservedMemoryBase));
  DEBUG ((DEBUG_INFO, " AcpiReservedMemorySize: 0x%x\n", MemorySubSystemConfig->AcpiReservedMemorySize));
  DEBUG ((DEBUG_INFO, " MemMapOverlapCheckSupport: 0x%x\n", MemorySubSystemConfig->MemMapOverlapCheckSupport));
  DEBUG ((DEBUG_INFO, "\n------------------------ MEMORY_SUB_SYSTEM_CONFIG Print END -----------------\n"));
  DEBUG_CODE_END ();
}
