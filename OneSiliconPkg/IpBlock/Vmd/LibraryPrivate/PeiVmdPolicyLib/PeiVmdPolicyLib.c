/** @file
  This file provides services for PEI VMD policy default initialization

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

#include <Library/DebugLib.h>
#include <Library/PeiVmdPolicyLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <SiPolicyStruct.h>
#include <VmdPeiConfig.h>
/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
VmdLoadPeiConfigDefault (
  IN VOID   *ConfigBlockPointer
  )
{
  VMD_PEI_CONFIG   *VmdConfig;

  VmdConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "VmdConfig->Header.GuidHob.Name = %g\n", &VmdConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "VmdConfig->Header.GuidHob.Header.HobLength = 0x%x\n", VmdConfig->Header.GuidHob.Header.HobLength));

  VmdConfig->VmdEnable = 1;
  VmdConfig->VmdGlobalMapping = 0;
  VmdConfig->VmdVariablePtr = NULL;
}

static COMPONENT_BLOCK_ENTRY  mVmdIpBlock = {
  &gVmdPeiConfigGuid,  sizeof (VMD_PEI_CONFIG), VMD_PEI_CONFIG_REVISION, VmdLoadPeiConfigDefault
};

/**
  Get VMD PEI config block table total size.

  @retval     Size of VMD PEI config block table
**/
UINT16
EFIAPI
VmdGetPeiConfigBlockTotalSize (
  VOID
  )
{
  return mVmdIpBlock.Size;
}

/**
  VmdAddConfigBlocks add all Vmd PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
VmdAddPeiConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mVmdIpBlock, 1);
  return Status;
}

/**
  This function prints the Vmd PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
VmdPrintPeiPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS       Status;
  VMD_PEI_CONFIG   *VmdPeiConfig;
  UINT16           Index;
  EFI_VMD_OS_DATA  *VmdVariablePtr;
  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gVmdPeiConfigGuid, (VOID *) &VmdPeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "----------------------- VMD_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", VmdPeiConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " VMD Enable : %01d\n", VmdPeiConfig->VmdEnable));
  DEBUG ((DEBUG_INFO, " VMD Global Mapping : %01d\n", VmdPeiConfig->VmdGlobalMapping));
  DEBUG ((DEBUG_INFO, " VMD PEI VmdCfgBarBase  : 0x%0x\n", VmdPeiConfig->VmdCfgBarBase));
  DEBUG ((DEBUG_INFO, " VMD PEI VmdMemBar1Base : 0x%0x\n", VmdPeiConfig->VmdMemBar1Base));
  DEBUG ((DEBUG_INFO, " VMD PEI VmdMemBar2Base : 0x%0x\n", VmdPeiConfig->VmdMemBar2Base));
  if (VmdPeiConfig->VmdVariablePtr != NULL) {
    VmdVariablePtr = (EFI_VMD_OS_DATA *)VmdPeiConfig->VmdVariablePtr;
    DEBUG ((DEBUG_INFO, " Vmd OS Variable details\n"));
    DEBUG ((DEBUG_INFO, " VMD Request Entry Count is %d\n", VmdVariablePtr->BYTE0_1.VREC));
    for (Index = 0; Index < VmdVariablePtr->BYTE0_1.VREC; ++Index) {
      DEBUG ((DEBUG_INFO, " Storage device B/D/F is %d/%d/%d and RpIndex is %d\n", VmdVariablePtr->DevDetails[Index].RpBus,
      VmdVariablePtr->DevDetails[Index].RpDevice, VmdVariablePtr->DevDetails[Index].RpFunction, VmdVariablePtr->DevDetails[Index].RpNumber));
    }
  }
  DEBUG ((DEBUG_INFO, "\n-------------------- VMD_CONFIG Print END -----------------\n"));
  return;
}
