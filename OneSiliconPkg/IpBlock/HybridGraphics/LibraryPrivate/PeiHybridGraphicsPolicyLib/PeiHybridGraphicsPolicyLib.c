/** @file
  This file provides services for PEI HybridGraphics policy default initialization

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

#include <Library/SiConfigBlockLib.h>
#include <Ppi/SiPolicy.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHybridGraphichsDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  return;
}

static COMPONENT_BLOCK_ENTRY  mHybridGraphicsIpBlocks = {
  &gHybridGraphicsConfigGuid, sizeof (HYBRID_GRAPHICS_CONFIG), HYBRID_GRAPHICS_CONFIG_REVISION, LoadHybridGraphichsDefault
};

/**
  Get Hybrid Graphics PEI config block table total size.

  @retval      Size of HybridGraphics PEI Pre-Mem config block table
**/
UINT16
EFIAPI
HybridGraphicsGetConfigBlockTotalSize (
  VOID
  )
{
  return mHybridGraphicsIpBlocks.Size;
}

/**
  HybridGraphicsAddConfigBlocks add all Hybrid Graphics PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
HybridGraphicsAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mHybridGraphicsIpBlocks, 1);
  return Status;
}
