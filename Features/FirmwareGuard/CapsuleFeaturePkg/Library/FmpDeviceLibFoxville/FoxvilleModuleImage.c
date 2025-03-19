/** @file
  Internal file to support the Foxville module in image operation.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/DebugLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleModule.h"
#include "Defines/FoxvilleNvmMap.h"
#include "FoxvilleModuleImage.h"

/**
  Get the shadow RAM module component from the buffer.

  @param[in]   BufferPtr         Pointer to the buffer.
  @param[in]   BufferSize        Size of the buffer.
  @param[out]  ModuleBufferPtr   Pointer to the module buffer.
  @param[out]  ModuleSizePtr     Pointer to the size of the module buffer.

  @retval  EFI_SUCCESS            Succeed to get the shadow RAM module from buffer.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_FOUND          Not found the shadow RAM module from buffer.
  @retval  Others                 Failed to get the shadow RAM module from buffer.

**/
EFI_STATUS
GetModuleShadowRamFromBuffer (
  IN     VOID   *BufferPtr,
  IN     UINTN  BufferSize,
     OUT VOID   **ModuleBufferPtr,
     OUT UINTN  *ModuleSizePtr
  )
{
  FOXVILLE_NVM_MAP  *FoxvilleNvmMapPtr;

  FoxvilleNvmMapPtr = NULL;

  if ((BufferPtr == NULL) || (BufferSize < (SHADOW_RAM_SECTOR_SIZE * 2))) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ModuleBufferPtr == NULL) || (ModuleSizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *ModuleBufferPtr = NULL;
  *ModuleSizePtr   = 0;

  //
  // Check on the validity in sector 0 of shadow RAM.
  //
  FoxvilleNvmMapPtr = (FOXVILLE_NVM_MAP *)((EFI_PHYSICAL_ADDRESS)BufferPtr + SHADOW_RAM_SECTOR_0_ADDRESS);
  if (FoxvilleNvmMapPtr->EepromSizeingAndProtectedFields.Bits.Signature == V_EEPROM_SIZING_AND_PROTECTED_FIELDS_SIGNATURE) {
    *ModuleBufferPtr = FoxvilleNvmMapPtr;
    *ModuleSizePtr   = SHADOW_RAM_SECTOR_SIZE;
    return EFI_SUCCESS;
  }

  //
  // Check on the validity in sector 1 of shadow RAM.
  //
  FoxvilleNvmMapPtr = (FOXVILLE_NVM_MAP *)((EFI_PHYSICAL_ADDRESS)BufferPtr + SHADOW_RAM_SECTOR_1_ADDRESS);
  if (FoxvilleNvmMapPtr->EepromSizeingAndProtectedFields.Bits.Signature == V_EEPROM_SIZING_AND_PROTECTED_FIELDS_SIGNATURE) {
    *ModuleBufferPtr = FoxvilleNvmMapPtr;
    *ModuleSizePtr   = SHADOW_RAM_SECTOR_SIZE;
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

/**
  Get the free provisioning area module component from the buffer.

  @param[in]   BufferPtr         Pointer to the buffer.
  @param[in]   BufferSize        Size of the buffer.
  @param[out]  ModuleBufferPtr   Pointer to the module buffer.
  @param[out]  ModuleSizePtr     Pointer to the size of the module buffer.

  @retval  EFI_SUCCESS            Succeed to get the free provisioning area module from buffer.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_FOUND          Not found the free provisioning area module from buffer.
  @retval  EFI_VOLUME_CORRUPTED   Input buffer is corrupted.
  @retval  Others                 Failed to get the free provisioning area module from buffer.

**/
EFI_STATUS
GetModuleFpaFromBuffer (
  IN     VOID   *BufferPtr,
  IN     UINTN  BufferSize,
     OUT VOID   **ModuleBufferPtr,
     OUT UINTN  *ModuleSizePtr
  )
{
  EFI_STATUS                    Status;
  UINT16                        *BufferInWordPtr;
  FOXVILLE_NVM_MAP              *FoxvilleNvmMapPtr;
  UINTN                         FoxvilleNvmMapSize;
  FOXVILLE_FREE_PROV_AREA_PTR   FoxvilleFpaOffset;
  FOXVILLE_FREE_PROV_AREA_SIZE  FoxvilleFpaSize;
  VOID                          *FpaModulePtr;
  UINTN                         FpaModuleSize;

  BufferInWordPtr          = NULL;
  FoxvilleNvmMapPtr        = NULL;
  FoxvilleNvmMapSize       = 0;
  FoxvilleFpaOffset.Uint16 = 0;
  FoxvilleFpaSize.Uint16   = 0;
  FpaModulePtr             = NULL;
  FpaModuleSize            = 0;

  if ((BufferPtr == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ModuleBufferPtr == NULL) || (ModuleSizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *ModuleBufferPtr = NULL;
  *ModuleSizePtr   = 0;

  //
  // Get valid shadow RAM module from image.
  //
  Status = GetModuleShadowRamFromBuffer (
             BufferPtr,
             BufferSize,
             (VOID **)&FoxvilleNvmMapPtr,
             &FoxvilleNvmMapSize
             );
  if (EFI_ERROR (Status)) {
    return EFI_VOLUME_CORRUPTED;
  }

  //
  // Get the free provisioning area information from NVM map.
  //
  BufferInWordPtr          = (UINT16 *)BufferPtr;
  FoxvilleFpaOffset.Uint16 = BufferInWordPtr[WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaPtr)];
  FoxvilleFpaSize.Uint16   = BufferInWordPtr[WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaSize)];

  if ((FoxvilleFpaOffset.Uint16 == 0x0000) || (FoxvilleFpaOffset.Uint16 == MAX_UINT16)) {
    return EFI_NOT_FOUND;
  }

  if ((FoxvilleFpaSize.Uint16 == 0x0000) || (FoxvilleFpaSize.Uint16 == MAX_UINT16)) {
    return EFI_NOT_FOUND;
  }

  //
  // Check the boundary.
  //
  if (FoxvilleFpaOffset.Bits.Is4KPtrType) {
    FpaModulePtr = (VOID *)((UINTN)BufferPtr + ((FoxvilleFpaOffset.Bits.Pointer) * SIZE_4KB));
  } else {
    FpaModulePtr = (VOID *)((UINTN)BufferPtr + (FoxvilleFpaOffset.Bits.Pointer));
  }

  FpaModuleSize = (FoxvilleFpaSize.Uint16) * SIZE_4KB;

  if (((UINTN)FpaModulePtr < (UINTN)BufferPtr) || \
      (((UINTN)FpaModulePtr + FpaModuleSize) > ((UINTN)BufferPtr + BufferSize)))
  {
    return EFI_VOLUME_CORRUPTED;
  }

  //
  // Return the find pointer and size.
  //
  *ModuleBufferPtr = FpaModulePtr;
  *ModuleSizePtr   = FpaModuleSize;

  return EFI_SUCCESS;
}

/**
  Get the offset of secure module component from the buffer.

  @param[in]   BufferPtr         Pointer to the buffer.
  @param[in]   BufferSize        Size of the buffer.
  @param[out]  OffsetPtr         Pointer to the offset to the secure module.

  @retval  EFI_SUCCESS            Succeed to get the offset to the secure module.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_VOLUME_CORRUPTED   Input buffer is corrupted.
  @retval  Others                 Failed to get the offset to the secure module.

**/
EFI_STATUS
GetModuleSecureOffsetFromBuffer (
  IN     VOID   *BufferPtr,
  IN     UINTN  BufferSize,
     OUT UINTN  *OffsetPtr
  )
{
  EFI_STATUS           Status;
  UINT16               *BufferInWordPtr;
  UINT16               OffsetInWord;
  FOXVILLE_NVM_MAP     *FoxvilleNvmMapPtr;
  UINTN                FoxvilleNvmMapSize;
  FOXVILLE_MODULE_PTR  FoxvilleModulePtr;

  BufferInWordPtr          = NULL;
  OffsetInWord             = 0;
  FoxvilleNvmMapPtr        = NULL;
  FoxvilleNvmMapSize       = 0;
  FoxvilleModulePtr.Uint16 = 0;

  if ((BufferPtr == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (OffsetPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get valid shadow RAM module from image.
  //
  Status = GetModuleShadowRamFromBuffer (
             BufferPtr,
             BufferSize,
             (VOID **)&FoxvilleNvmMapPtr,
             &FoxvilleNvmMapSize
             );
  if (EFI_ERROR (Status)) {
    return EFI_VOLUME_CORRUPTED;
  }

  //
  // If expansion ROM is not presented, then the first module is PHY module.
  //
  OffsetInWord             = WORD_OFFSET_OF (FOXVILLE_NVM_MAP, ExpRomAreaPtr);
  BufferInWordPtr          = (UINT16 *)BufferPtr;
  FoxvilleModulePtr.Uint16 = BufferInWordPtr[OffsetInWord];
  if ((FoxvilleModulePtr.Uint16 == 0x0000) || (FoxvilleModulePtr.Uint16 == MAX_UINT16)) {
    OffsetInWord = FXOVILLE_PHY_MODULE_WORD_OFFSET;
  }

  FoxvilleModulePtr.Uint16 = BufferInWordPtr[OffsetInWord];

  if ((FoxvilleModulePtr.Bits.Pointer) > BufferSize) {
    return EFI_VOLUME_CORRUPTED;
  }

  if (FoxvilleModulePtr.Bits.Is4KPtrType) {
    *OffsetPtr = FoxvilleModulePtr.Bits.Pointer * SIZE_4KB;
  } else {
    *OffsetPtr = FoxvilleModulePtr.Bits.Pointer;
  }

  return EFI_SUCCESS;
}

/**
  Get the secure module component from the buffer.

  @param[in]   BufferPtr         Pointer to the buffer.
  @param[in]   BufferSize        Size of the buffer.
  @param[out]  ModuleBufferPtr   Pointer to the module buffer.
  @param[out]  ModuleSizePtr     Pointer to the size of the module buffer.

  @retval  EFI_SUCCESS            Succeed to get the secure module from buffer.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_FOUND          Not found the secure module from buffer.
  @retval  EFI_VOLUME_CORRUPTED   Input buffer is corrupted.
  @retval  Others                 Failed to get the secure module from buffer.

**/
EFI_STATUS
GetModuleSecureFromBuffer (
  IN     VOID   *BufferPtr,
  IN     UINTN  BufferSize,
     OUT VOID   **ModuleBufferPtr,
     OUT UINTN  *ModuleBufferSizePtr
  )
{
  EFI_STATUS  Status;
  VOID        *FpaModulePtr;
  UINTN       FpaModuleSize;
  UINTN       SecureModuleOffset;
  VOID        *SecureModulePtr;
  UINTN       SecureModuleSize;

  FpaModulePtr       = NULL;
  FpaModuleSize      = 0;
  SecureModuleOffset = 0;
  SecureModulePtr    = NULL;
  SecureModuleSize   = 0;

  if ((BufferPtr == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ModuleBufferPtr == NULL) || (ModuleBufferSizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *ModuleBufferPtr     = NULL;
  *ModuleBufferSizePtr = 0;

  //
  // Get free provision area module from image.
  //
  Status = GetModuleFpaFromBuffer (
             BufferPtr,
             BufferSize,
             &FpaModulePtr,
             &FpaModuleSize
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  //
  // Get secure module offset.
  //
  Status = GetModuleSecureOffsetFromBuffer (
             BufferPtr,
             BufferSize,
             &SecureModuleOffset
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  //
  // Check the boundary.
  //
  SecureModulePtr  = (VOID *)((UINTN)BufferPtr + SecureModuleOffset);
  SecureModuleSize = FpaModuleSize;

  if (((UINTN)SecureModulePtr < (UINTN)BufferPtr) || \
      (((UINTN)SecureModulePtr + FpaModuleSize) > ((UINTN)BufferPtr + BufferSize)))
  {
    return EFI_VOLUME_CORRUPTED;
  }

  *ModuleBufferPtr     = SecureModulePtr;
  *ModuleBufferSizePtr = SecureModuleSize;

  return EFI_SUCCESS;
}

/**
  Get the module component from the buffer.

  @param[in]   Module            Module type.
  @param[in]   BufferPtr         Pointer to the buffer.
  @param[in]   BufferSize        Size of the buffer.
  @param[out]  ModuleBufferPtr   Pointer to the module buffer.
  @param[out]  ModuleSizePtr     Pointer to the size of the module buffer.

  @retval  EFI_SUCCESS            Succeed to get the module from buffer.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        Unsupported module type.
  @retval  Others                 Failed to get the module from buffer.

**/
EFI_STATUS
EFIAPI
GetModuleFromBuffer (
  IN     FOXVILLE_MODULE  Module,
  IN     VOID             *BufferPtr,
  IN     UINTN            BufferSize,
     OUT VOID             **ModuleBufferPtr,
     OUT UINTN            *ModuleSizePtr
  )
{
  EFI_STATUS  Status;

  if ((BufferPtr == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ModuleBufferPtr == NULL) || (ModuleSizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Module) {
    case MODULE_SHADOW_RAM:
      Status = GetModuleShadowRamFromBuffer (BufferPtr, BufferSize, ModuleBufferPtr, ModuleSizePtr);
      break;

    case MODULE_SECURE:
      Status = GetModuleSecureFromBuffer (BufferPtr, BufferSize, ModuleBufferPtr, ModuleSizePtr);
      break;

    case MODULE_FREE_PROV_AREA:
      Status = GetModuleFpaFromBuffer (BufferPtr, BufferSize, ModuleBufferPtr, ModuleSizePtr);
      break;

    default:
      DEBUG ((DEBUG_ERROR, "%a: Unsupported module type - [0x%04X].\n", __func__, Module));
      return EFI_UNSUPPORTED;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the module [0x%04X] - %r\n", Module, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Get the module [0x%04X] @ 0x%08X (0x%08X).\n", Module, *ModuleBufferPtr, *ModuleSizePtr));

  return EFI_SUCCESS;
}
