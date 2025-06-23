/** @file
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/PchInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <PsfConfig.h>

/**
  Print PSF_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to SI_POLICY_PPI
**/
VOID
PchPsfPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  PSF_CONFIG        *PsfConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPsfConfigGuid, (VOID *) &PsfConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ PSF Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " DwbConfigDwbFlushThreshold= 0x%1X\n", PsfConfig->DwbConfigDwbFlushThreshold));
  DEBUG ((DEBUG_INFO, " DwbConfigNonxHCIEn= %u\n", PsfConfig->DwbConfigNonxHCIEn));
  DEBUG ((DEBUG_INFO, " DwbConfigOBFFEn= %u\n", PsfConfig->DwbConfigOBFFEn));
  DEBUG ((DEBUG_INFO, " DwbConfigDWBEn= %u\n", PsfConfig->DwbConfigDWBEn));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PchPsfLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PSF_CONFIG *PsfConfig;
  PsfConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PsfConfig->Header.GuidHob.Name = %g\n", &PsfConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PsfConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PsfConfig->Header.GuidHob.Header.HobLength));

  PsfConfig->DwbConfigDwbFlushThreshold = 7;
  PsfConfig->DwbConfigNonxHCIEn = FALSE;
  PsfConfig->DwbConfigOBFFEn = TRUE;
  PsfConfig->DwbConfigDWBEn = TRUE;
}

STATIC COMPONENT_BLOCK_ENTRY  mPsfConfigBlocks = {
  &gPsfConfigGuid,
  sizeof (PSF_CONFIG),
  PSF_CONFIG_REVISION,
  PchPsfLoadConfigDefault
};

/**
  Get PSF config block table size.

  @retval      Size of config block
**/
UINT16
PchPsfGetConfigBlockTotalSize (
  VOID
  )
{
  return mPsfConfigBlocks.Size;
}

/**
  Add PSF ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PchPsfAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mPsfConfigBlocks, 1);
}