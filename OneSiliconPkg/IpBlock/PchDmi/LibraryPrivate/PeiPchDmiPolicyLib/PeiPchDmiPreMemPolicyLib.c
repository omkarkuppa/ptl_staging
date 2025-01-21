/** @file
  This file provides services for PchDmi PreMem policy function

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
#include <Library/PchInfoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <PchDmiConfig.h>
#include <DmiConfig.h>


/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
DmiUpPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  DMI_CONFIG   *DmiUpConfig;
  DmiUpConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "DmiConfig->Header.GuidHob.Name = %g\n", &DmiUpConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "DmiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", DmiUpConfig->Header.GuidHob.Header.HobLength));
}

STATIC COMPONENT_BLOCK_ENTRY  mDmiBlocks [1] = {
  {
    &gDmiConfigGuid,
    sizeof (DMI_CONFIG),
    DMI_CONFIG_REVISION,
    DmiUpPreMemLoadConfigDefault
  }
};

/**
  Get Dmi config block table size.

  @retval      Size of config block
**/
UINT16
DmiUpPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mDmiBlocks[0], sizeof (mDmiBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Add Dmi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
DmiUpPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mDmiBlocks[0], sizeof (mDmiBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}