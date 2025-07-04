/** @file
  This file provides services for TCC policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <TccConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TccLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  TCC_CONFIG      *TccConfig;

  TccConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "TccConfig->Header.GuidHob.Name = %g\n", &TccConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TccConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TccConfig->Header.GuidHob.Header.HobLength));

  TccConfig->IfuEnable = 0;
  TccConfig->GtClosEnable= 0;
  TccConfig->L2QosEnumerationEn= 0;
}

STATIC COMPONENT_BLOCK_ENTRY  mTccBlocks = {
  &gTccConfigGuid,
  sizeof (TCC_CONFIG),
  TCC_CONFIG_REVISION,
  TccLoadConfigDefault
};
/**
  Get Tcc config block table size.

  @retval      Size of config block
**/
UINT16
TccGetConfigBlockTotalSize (
  VOID
  )
{
  return mTccBlocks.Size;
}

/**
  Add Tcc ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
TccAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mTccBlocks, 1);
}
