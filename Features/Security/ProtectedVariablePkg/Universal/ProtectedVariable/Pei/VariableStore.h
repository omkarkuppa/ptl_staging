/** @file
  Implement ReadOnly Variable Services required by PEIM and install
  PEI ReadOnly Variable2 PPI. These services operates the non volatile storage space.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef PEI_VARIABLE_STORE_H_
#define PEI_VARIABLE_STORE_H_

/**
  Get variable store status.

  @param[in]  VarStoreHeader  Pointer to the Variable Store Header.

  @retval  EfiRaw      Variable store is raw
  @retval  EfiValid    Variable store is valid
  @retval  EfiInvalid  Variable store is invalid

**/
VARIABLE_STORE_STATUS
GetVariableStoreStatus (
  IN VARIABLE_STORE_HEADER  *VarStoreHeader
  );

/**
  Get HOB variable store.

  @param[out] StoreInfo             Return the store info.

**/
VOID
GetHobVariableStore (
  OUT VARIABLE_STORE_INFO  *StoreInfo
  );

/**
  Get NV variable store.

  @param[out] StoreInfo             Return the store info.
  @param[out] VariableStoreHeader   Return header of FV containing the store.

**/
VOID
GetNvVariableStore (
  OUT VARIABLE_STORE_INFO         *StoreInfo,
  OUT EFI_FIRMWARE_VOLUME_HEADER  **VariableFvHeader
  );

/**
  Return the variable store header and the store info based on the Index.

  @param[in]  Type       The type of the variable store.
  @param[out] StoreInfo  Return the store info.

  @return  Pointer to the variable store header.
**/
VARIABLE_STORE_HEADER *
GetVariableStore (
  IN VARIABLE_STORE_TYPE   Type,
  OUT VARIABLE_STORE_INFO  *StoreInfo
  );

/**
  Make a cached copy of NV variable storage.

  To save memory in PEI phase, only valid variables are copied into cache.
  An IndexTable could be used to store the offset (relative to NV storage
  base) of each copied variable, in case we need to restore the storage
  as the same (valid) variables layout as in original one.

  Variables with valid format and following state can be taken as valid:
    - with state VAR_ADDED;
    - with state VAR_IN_DELETED_TRANSITION but without the same variable
      with state VAR_ADDED;
    - with state VAR_ADDED and/or VAR_IN_DELETED_TRANSITION for variable
      MetaDataHmacVar.

  @param[out]     StoreCacheBase    Base address of variable storage cache.
  @param[in,out]  StoreCacheSize    Size of space in StoreCacheBase.
  @param[out]     IndexTable        Buffer of index (offset) table with entries of
                                    VariableNumber.
  @param[out]     VariableNumber    Number of valid variables.
  @param[out]     AuthFlag          Aut-variable indicator.

  @return EFI_INVALID_PARAMETER Invalid StoreCacheSize and/or StoreCacheBase.
  @return EFI_VOLUME_CORRUPTED  Invalid or no NV variable storage found.
  @return EFI_BUFFER_TOO_SMALL  StoreCacheSize is smaller than needed.
  @return EFI_SUCCESS           NV variable storage is cached successfully.
**/
EFI_STATUS
EFIAPI
InitNvVariableStore (
  OUT  EFI_PHYSICAL_ADDRESS  StoreCacheBase OPTIONAL,
  IN OUT  UINT32             *StoreCacheSize,
  OUT  UINT32                *IndexTable OPTIONAL,
  OUT  UINT32                *VariableNumber OPTIONAL,
  OUT  BOOLEAN               *AuthFlag OPTIONAL
  );

#endif
