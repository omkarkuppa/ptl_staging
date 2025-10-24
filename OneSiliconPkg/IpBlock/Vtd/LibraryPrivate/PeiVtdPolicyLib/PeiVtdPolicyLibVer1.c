/** @file
  This file provides services for PEI VTD policy default initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/PeiVtdPolicyLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <SiPolicyStruct.h>
#include <Library/PcdLib.h>
#include <Library/VtdInfoLib.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadVtdDefault (
  IN VOID   *ConfigBlockPointer
  )
{
  VTD_CONFIG     *Vtd;
  UINT8          Index;
  EFI_BOOT_MODE  SysBootMode;

  SysBootMode = 0;
  Vtd = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "Vtd->Header.GuidHob.Name = %g\n", &Vtd->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "Vtd->Header.GuidHob.Header.HobLength = 0x%x\n", Vtd->Header.GuidHob.Header.HobLength));

  //
  // Initialize the Vtd Configuration
  //
  if (IsVtdSupported ()) {
    Vtd->VtdDisable =  0;
  } else {
    DEBUG ((DEBUG_INFO, "VT-d is not supported by Silicon.\n"));
    Vtd->VtdDisable =  1;
  }
  Vtd->DmaControlGuarantee = 1;
  Vtd->PreBootDmaMask = PcdGet8 (PcdVTdPolicyPropertyMask);
  for (Index = 0; Index < GetMaxVtdEngineNumber (); Index++) {
    Vtd->BaseAddress[Index] = GetVtdBaseAddress (Index);
  }

  PeiServicesGetBootMode (&SysBootMode);
  DEBUG ((DEBUG_INFO, " boot mode : 0x%x\n",  SysBootMode));
  if (SysBootMode == BOOT_ON_S3_RESUME) {
    Vtd->DmaBufferSize = PcdGet32 (PcdVTdPeiDmaBufferSizeS3);
  } else {
    Vtd->DmaBufferSize = PcdGet32 (PcdVTdPeiDmaBufferSize);
  }

}


static COMPONENT_BLOCK_ENTRY  mVtdIpBlock = {
  &gVtdConfigGuid, sizeof (VTD_CONFIG), VTD_CONFIG_REVISION, LoadVtdDefault
};

/**
  Get VTD PEI config block table total size.

  @retval     Size of VTD PEI config block table
**/
UINT16
EFIAPI
VtdGetConfigBlockTotalSize (
  VOID
  )
{
  return mVtdIpBlock.Size;
}

/**
  VtdAddConfigBlocks add VTD PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
VtdAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mVtdIpBlock, 1);
  return Status;
}


/**
  This function prints the PEI phase VTD policy.

  @param[in] SiPolicyPreMemPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
VtdPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS    Status;
  VTD_CONFIG    *Vtd;
  UINT8         Index;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gVtdConfigGuid, (VOID *) &Vtd);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ VTD_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", Vtd->Header.Revision));
  DEBUG ((DEBUG_INFO, " VtdDisable : 0x%x\n", Vtd->VtdDisable));
  DEBUG ((DEBUG_INFO, " DmaControlGuarantee : 0x%x\n", Vtd->DmaControlGuarantee));
  for (Index = 0; Index < GetMaxVtdEngineNumber (); Index++) {
    DEBUG ((DEBUG_INFO, " VtdBaseAddress[%d] : 0x%x\n", Index, Vtd->BaseAddress[Index]));
  }
  DEBUG ((DEBUG_INFO, " PreBootDmaMask : 0x%x\n", Vtd->PreBootDmaMask));
  DEBUG ((DEBUG_INFO, " DmaBufferSize : 0x%x\n", Vtd->DmaBufferSize));
  DEBUG ((DEBUG_INFO, " CapabilityControl : 0x%x\n", Vtd->CapabilityControl));
  DEBUG_CODE_END ();
  return;
}