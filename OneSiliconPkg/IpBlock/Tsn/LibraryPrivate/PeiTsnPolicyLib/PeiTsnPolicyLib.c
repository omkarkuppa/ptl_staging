/** @file
  This file provides services for TSN policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <TsnConfig.h>

/**
  Print TSN_CONFIG and serial out.

  @param[in] SiPolicyPpi   Pointer to a SI_POLICY_PPI
**/
VOID
TsnPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS   Status;
  TSN_CONFIG   *TsnConfig;
  UINT8        Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTsnConfigGuid, (VOID *) &TsnConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ Tsn Config ------------------\n"));
  for (Index = 0; Index < PCH_MAX_TSN_PORT; Index++) {
    DEBUG ((DEBUG_INFO, " Enable[%d] = %x\n", Index, TsnConfig->Enable[Index]));
    DEBUG ((DEBUG_INFO, " MacAddr high[%d] = 0x%x MacAddr low = 0x%x\n", Index, TsnConfig->Port[Index].MacAddr[1], TsnConfig->Port[Index].MacAddr[0]));
  }
  DEBUG ((DEBUG_INFO, " TsnMultiVcEnable = 0x%x\n", TsnConfig->MultiVcEnable));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer   Pointer to config block
**/
VOID
TsnLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  TSN_CONFIG   *TsnConfig;
  UINT8        Index;

  TsnConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "TsnConfig->Header.GuidHob.Name = %g\n", &TsnConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TsnConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TsnConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < PCH_MAX_TSN_PORT; Index++) {
    TsnConfig->Enable[Index] = TRUE;
    TsnConfig->Port[Index].MacAddr[0] = 0;
    TsnConfig->Port[Index].MacAddr[1] = 0;
  }
  TsnConfig->MultiVcEnable = (UINT8) 0x0;
}

STATIC COMPONENT_BLOCK_ENTRY  mTsnBlocks = {
  &gTsnConfigGuid,
  sizeof (TSN_CONFIG),
  TSN_CONFIG_REVISION,
  TsnLoadConfigDefault
};

/**
  Get TSN config block table size.

  @retval      Size of config block
**/
UINT16
TsnGetConfigBlockTotalSize (
  VOID
  )
{
  return mTsnBlocks.Size;
}

/**
  Add TSN ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
TsnAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mTsnBlocks, 1);
}
