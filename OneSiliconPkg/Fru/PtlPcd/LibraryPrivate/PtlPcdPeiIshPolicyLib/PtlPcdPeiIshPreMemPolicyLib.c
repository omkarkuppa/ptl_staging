/** @file
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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <IshConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PtlPcdIshPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  ISH_PREMEM_CONFIG  *IshPreMemConfig;
  IshPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "IshPreMemConfig->Header.GuidHob.Name = %g\n", &IshPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IshPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IshPreMemConfig->Header.GuidHob.Header.HobLength));

  IshPreMemConfig->Enable = TRUE;
}

STATIC COMPONENT_BLOCK_ENTRY  mIshPreMemBlocks = {
  &gIshPreMemConfigGuid,
  sizeof (ISH_PREMEM_CONFIG),
  ISH_PREMEM_CONFIG_REVISION,
  PtlPcdIshPreMemLoadConfigDefault
};

/**
  Get Ish config block table size.

  @retval      Size of config block
**/
UINT16
PtlPcdIshPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mIshPreMemBlocks.Size;
}

/**
  Add Ish ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PtlPcdIshPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mIshPreMemBlocks, 1);
}