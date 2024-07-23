/** @file
  Prototype of the SiConfigBlockLib library.

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
#ifndef _SI_CONFIG_BLOCK_LIB_H_
#define _SI_CONFIG_BLOCK_LIB_H_


typedef
VOID
(*LOAD_DEFAULT_FUNCTION) (
  IN VOID   *ConfigBlockPointer
  );

typedef struct {
  EFI_GUID               *Guid;
  UINT16                 Size;
  UINT8                  Revision;
  LOAD_DEFAULT_FUNCTION  LoadDefault;
} COMPONENT_BLOCK_ENTRY;

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
  );

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
  );
#endif // _SI_CONFIG_BLOCK_LIB_H_
