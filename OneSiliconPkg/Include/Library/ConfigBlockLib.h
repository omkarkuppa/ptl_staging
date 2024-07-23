/** @file
  Header file for Config Block Lib implementation

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

@par Specification
**/

#ifndef _CONFIG_BLOCK_LIB_H_
#define _CONFIG_BLOCK_LIB_H_

#include <Uefi/UefiBaseType.h>

/**
  Create config block table

  @param[in]     TotalSize                    - Max size to be allocated for the Config Block Table
  @param[out]    ConfigBlockTableAddress      - On return, points to a pointer to the beginning of Config Block Table Address

  @retval EFI_INVALID_PARAMETER - Invalid Parameter
  @retval EFI_OUT_OF_RESOURCES  - Out of resources
  @retval EFI_SUCCESS           - Successfully created Config Block Table at ConfigBlockTableAddress
**/
EFI_STATUS
EFIAPI
CreateConfigBlockTable (
  IN     UINT16    TotalSize,
  OUT    VOID      **ConfigBlockTableAddress
  );

/**
  Add config block into config block table structure

  @param[in]     ConfigBlockTableAddress      - A pointer to the beginning of Config Block Table Address
  @param[out]    ConfigBlockAddress           - On return, points to a pointer to the beginning of Config Block Address

  @retval EFI_OUT_OF_RESOURCES - Config Block Table is full and cannot add new Config Block or
                                 Config Block Offset Table is full and cannot add new Config Block.
  @retval EFI_SUCCESS          - Successfully added Config Block
**/
EFI_STATUS
EFIAPI
AddConfigBlock (
  IN     VOID      *ConfigBlockTableAddress,
  OUT    VOID      **ConfigBlockAddress
  );

/**
  Retrieve a specific Config Block data by GUID

  @param[in]      ConfigBlockTableAddress      - A pointer to the beginning of Config Block Table Address
  @param[in]      ConfigBlockGuid              - A pointer to the GUID uses to search specific Config Block
  @param[out]     ConfigBlockAddress           - On return, points to a pointer to the beginning of Config Block Address

  @retval EFI_NOT_FOUND         - Could not find the Config Block
  @retval EFI_SUCCESS           - Config Block found and return
**/
EFI_STATUS
EFIAPI
GetConfigBlock (
  IN     VOID      *ConfigBlockTableAddress,
  IN     EFI_GUID  *ConfigBlockGuid,
  OUT    VOID      **ConfigBlockAddress
  );

#endif // _CONFIG_BLOCK_LIB_H_

