/** @file
  This file provides services for Dci PreMem policy function

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
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <DciConfig.h>

/**
  Print PCH_DCI_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI

**/
VOID
DciPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                Status;
  PCH_DCI_PREMEM_CONFIG     *DciPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ DCI PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "DciEn = %x\n", DciPreMemConfig->DciEn));
  DEBUG ((DEBUG_INFO, "DciClkEnable = %x\n", DciPreMemConfig->DciClkEnable));
  DEBUG ((DEBUG_INFO, "DciDbcMode = %x\n", DciPreMemConfig->DciDbcMode));
  DEBUG ((DEBUG_INFO, "KeepEarlyTrace = %x\n", DciPreMemConfig->KeepEarlyTrace));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
DciPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_DCI_PREMEM_CONFIG  *DciPreMemConfig;
  DciPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "DciPreMemConfig->Header.GuidHob.Name = %g\n", &DciPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "DciPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", DciPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    DCI Configuration
  ********************************/
  DciPreMemConfig->DciClkEnable = TRUE;
  DciPreMemConfig->DciDbcMode = DciDbcNoChange;
}

STATIC COMPONENT_BLOCK_ENTRY  mDciPreMemBlocks = {
  &gDciPreMemConfigGuid,
  sizeof (PCH_DCI_PREMEM_CONFIG),
  PCH_DCI_PREMEM_CONFIG_REVISION,
  DciPreMemLoadConfigDefault
};

/**
  Get Dci config block table size.

  @retval      Size of config block
**/
UINT16
DciPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mDciPreMemBlocks.Size;
}

/**
  Add Dci ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
DciPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mDciPreMemBlocks, 1);
}
