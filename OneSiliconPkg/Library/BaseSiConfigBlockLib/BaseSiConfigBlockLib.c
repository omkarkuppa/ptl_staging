/** @file
  This file is BaseSiConfigBlockLib library is used to add config blocks
  to config block header.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#include <ConfigBlock.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>


/**
  GetComponentConfigBlockTotalSize get config block table total size.

  @param[in] ComponentBlocks    Component blocks array
  @param[in] TotalBlockCount    Number of blocks

  @retval                       Size of config block table
**/
UINT16
EFIAPI
GetComponentConfigBlockTotalSize (
  IN COMPONENT_BLOCK_ENTRY *ComponentBlocks,
  IN UINT16                TotalBlockCount
  )
{
  UINT16            TotalBlockSize;
  UINT16            BlockCount;

  TotalBlockSize = 0;
  for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
    TotalBlockSize += (UINT32) ComponentBlocks[BlockCount].Size;
  }

  return TotalBlockSize;
}

/**
  AddComponentConfigBlocks add all config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks
  @param[in] ComponentBlocks            Config blocks array
  @param[in] TotalBlockCount            Number of blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AddComponentConfigBlocks (
  IN VOID                  *ConfigBlockTableAddress,
  IN COMPONENT_BLOCK_ENTRY *ComponentBlocks,
  IN UINT16                TotalBlockCount
  )
{
  UINT16            BlockCount;
  VOID              *ConfigBlockPointer;
  CONFIG_BLOCK      ConfigBlockBuf;
  EFI_STATUS        Status;

  Status = EFI_SUCCESS;

  //
  // Initialize ConfigBlockPointer to NULL
  //
  ConfigBlockPointer = NULL;
  //
  // Loop to identify each config block from ComponentBlocks[] Table and add each of them
  //
  for (BlockCount = 0; BlockCount < TotalBlockCount; BlockCount++) {
    ZeroMem (&ConfigBlockBuf, sizeof (CONFIG_BLOCK));
    CopyMem (&(ConfigBlockBuf.Header.GuidHob.Name), ComponentBlocks[BlockCount].Guid, sizeof (EFI_GUID));
    ConfigBlockBuf.Header.GuidHob.Header.HobLength = ComponentBlocks[BlockCount].Size;
    ConfigBlockBuf.Header.Revision                 = ComponentBlocks[BlockCount].Revision;
    ConfigBlockPointer = (VOID *)&ConfigBlockBuf;
    Status = AddConfigBlock ((VOID *)ConfigBlockTableAddress, (VOID *)&ConfigBlockPointer);
    ASSERT_EFI_ERROR (Status);
    if (ComponentBlocks[BlockCount].LoadDefault != NULL) {
      ComponentBlocks[BlockCount].LoadDefault (ConfigBlockPointer);
    }
  }
  return Status;
}
