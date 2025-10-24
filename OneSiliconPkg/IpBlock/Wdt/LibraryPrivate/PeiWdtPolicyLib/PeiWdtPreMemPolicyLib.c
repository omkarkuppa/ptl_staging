/** @file
  This file provides services for Wdt policy function

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
#include <WatchDogConfig.h>

/**
  Print PCH_WDT_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI

**/
VOID
WdtPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS            Status;
  PCH_WDT_PREMEM_CONFIG *WdtPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gWatchDogPreMemConfigGuid, (VOID *) &WdtPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ WDT PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "DisableAndLock= %x\n", WdtPreMemConfig->DisableAndLock));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
WdtPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_WDT_PREMEM_CONFIG       *WdtPreMemConfig;
  WdtPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "WdtPreMemConfig->Header.GuidHob.Name = %g\n", &WdtPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "WdtPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", WdtPreMemConfig->Header.GuidHob.Header.HobLength));
}

STATIC COMPONENT_BLOCK_ENTRY  mWdtPreMemBlocks = {
  &gWatchDogPreMemConfigGuid,
  sizeof (PCH_WDT_PREMEM_CONFIG),
  PCH_WDT_PREMEM_CONFIG_REVISION,
  WdtPreMemLoadConfigDefault
};

/**
  Get Wdt config block table size.

  @retval      Size of config block
**/
UINT16
WdtPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mWdtPreMemBlocks.Size;
}

/**
  Add Wdt ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
WdtPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mWdtPreMemBlocks, 1);
}
