/** @file
  Internal file to support Foxville module update.

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
#include <Protocol/PciIo.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleModule.h"
#include "Defines/FoxvilleNvmMap.h"
#include "FoxvilleApi.h"
#include "FoxvilleCrc.h"
#include "FoxvilleModuleFlash.h"
#include "FoxvilleModuleImage.h"
#include "FoxvilleUpdate.h"

#define MAC_ADDRESS_WORD_SIZE  3

/**
  Update the value of the free provisioning area from EEPROM into buffer.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  BufferPtr         Pointer to the buffer.
  @param[in]  BufferSize        Size of the buffer.

  @retval  EFI_SUCCESS            Succeed to update the FPA pointer and size.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to update the FPA pointer and size.

**/
EFI_STATUS
UpdateFreeProvAreaPtrAndSize (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN VOID                 *BufferPtr,
  IN UINTN                BufferSize
  )
{
  EFI_STATUS  Status;
  UINT16      *BufferInWordPtr;

  BufferInWordPtr = NULL;

  if ((PciIoProtocolPtr == NULL) || (BufferPtr == NULL) || (BufferSize < SHADOW_RAM_SECTOR_SIZE)) {
    return EFI_INVALID_PARAMETER;
  }

  BufferInWordPtr = (UINT16 *)BufferPtr;

  Status = ReadNvmEepromUint16 (
             PciIoProtocolPtr,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaPtr),
             &(BufferInWordPtr[WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaPtr)])
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to save the FPA pointer.\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = ReadNvmEepromUint16 (
             PciIoProtocolPtr,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaSize),
             &(BufferInWordPtr[WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaSize)])
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to save the FPA size.\n"));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Write the shadow RAM image into EEPROM.

  @param[in]  PciIoProtocolPtr    Pointer to the PCI IO protocol instance.
  @param[in]  BufferPtr           Pointer to the buffer.
  @param[in]  BufferSize          Size of the buffer.
  @param[in]  IsMacAddressUpdate  Indicate to update the MAC address into EEPROM.

  @retval  EFI_SUCCESS            Succeed to write shadow RAM image into EEPROM.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write shadow RAM image into EEPROM.

**/
EFI_STATUS
WriteShadowRamImageToEeprom (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN VOID                 *BufferPtr,
  IN UINTN                BufferSize,
  IN BOOLEAN              IsMacAddressUpdate
  )
{
  EFI_STATUS  Status;
  UINT16      *BufferInWordPtr;
  UINT16      BufferInWordSize;
  UINT16      Index;
  UINT16      Offset;

  BufferInWordPtr  = NULL;
  BufferInWordSize = 0;
  Index            = 0;
  Offset           = 0;

  if ((PciIoProtocolPtr == NULL) || (BufferPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize != SHADOW_RAM_SECTOR_SIZE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!IsMacAddressUpdate) {
    Offset = MAC_ADDRESS_WORD_SIZE;
  }

  BufferInWordPtr  = (UINT16 *)BufferPtr;
  BufferInWordSize = SHADOW_RAM_SECTOR_SIZE / sizeof (UINT16);

  for (Index = Offset; Index < BufferInWordSize; Index++) {
    if ((Index == WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FwImagePtr)) || \
        (Index == WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaPtr)) || \
        (Index == WORD_OFFSET_OF (FOXVILLE_NVM_MAP, ExpRomAreaPtr)) || \
        (Index == FXOVILLE_PHY_MODULE_WORD_OFFSET) || \
        (Index == FOXVILLE_SW_AREA_WORD_OFFSET))
    {
      continue;
    }

    DEBUG ((DEBUG_INFO, "%a: Index = 0x%04X - Value = 0x%04X\n", __func__, Index, BufferInWordPtr[Index]));

    Status = WriteNvmEepromUint16 (PciIoProtocolPtr, Index, BufferInWordPtr[Index]);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

  return EFI_SUCCESS;
}

/**
  Program the flash region with assigned buffer into specific address.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address used to perform the program flash region.
  @param[in]  BufferPtr         Pointer to the buffer to be used.
  @param[in]  BufferSize        Size of the buffer to be used.

  @retval  EFI_SUCCESS            Succeed to program the assigned flash region.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to program the assigned flash region.

**/
EFI_STATUS
ProgramFlashRegion (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Address,
  IN VOID                 *BufferPtr,
  IN UINTN                BufferSize
  )
{
  EFI_STATUS  Status;
  UINT32      RegionSize;

  RegionSize = 0;

  if ((PciIoProtocolPtr == NULL) || (BufferPtr == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize > MAX_UINT32) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  RegionSize = BufferSize & MAX_UINT32;

  Status = FlashSectorErase (PciIoProtocolPtr, Address, RegionSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to erase the flash region.\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = FlashWrite (PciIoProtocolPtr, Address, BufferPtr, RegionSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to write the flash region.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a: Succeed to program the flash region.\n", __func__));

  return EFI_SUCCESS;
}

/**
  Check the validity of secure module component in image.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  ImagePtr          Pointer to the image.
  @param[in]  ImageSize         Size of the image.
  @param[in]  IsValidPtr        Pointer to indicate the validity of secure module.

  @retval  EFI_SUCCESS            Succeed to check the validity of the secure module.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_VOLUME_CORRUPTED   Input buffer is corrupted.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to check the validity of the secure module.

**/
EFI_STATUS
CheckSecureModuleInImageValid (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     VOID                 *ImagePtr,
  IN     UINTN                ImageSize,
     OUT BOOLEAN              *IsValidPtr
  )
{
  EFI_STATUS  Status;
  VOID        *ModuleInBufferPtr;
  UINTN       ModuleInBufferSize;
  UINTN       SecureModuleAddress;
  UINTN       SecureModuleSize;

  ModuleInBufferPtr   = NULL;
  ModuleInBufferSize  = 0;
  SecureModuleAddress = 0;
  SecureModuleSize    = 0;

  if ((PciIoProtocolPtr == NULL) || (ImagePtr == NULL) || (ImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (IsValidPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *IsValidPtr = FALSE;

  Status = GetModuleFromBuffer (MODULE_SECURE, ImagePtr, ImageSize, &ModuleInBufferPtr, &ModuleInBufferSize);
  if (EFI_ERROR (Status)) {
    return EFI_VOLUME_CORRUPTED;
  }

  Status = GetModuleInfoFromFlash (PciIoProtocolPtr, MODULE_SECURE, &SecureModuleAddress, &SecureModuleSize);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Current FW is running @ 0x%08X (0x%08X).\n", SecureModuleAddress, SecureModuleSize));

  if (ModuleInBufferSize != SecureModuleSize) {
    DEBUG ((DEBUG_ERROR, "Secure module size mismatched. Image (0x%08X) Flash (0x%08X).", ModuleInBufferSize, SecureModuleSize));
    goto Exit;
  }

  *IsValidPtr = TRUE;

Exit:

  return EFI_SUCCESS;
}

/**
  Update the shadow RAM image into EEPROM region.

  @param[in]  PciIoProtocolPtr    Pointer to the PCI IO protocol instance.
  @param[in]  ImagePtr            Pointer to the image.
  @param[in]  ImageSize           Size of the image.
  @param[in]  IsMacAddressUpdate  Indicate to update the MAC address into EEPROM.

  @retval  EFI_SUCCESS            Succeed to update the EEPROM region.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_OUT_OF_RESOURCES   Failed to allocate needed memory buffer.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to update the EEPROM region.

**/
EFI_STATUS
UpdateEepromRegion (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN VOID                 *ImagePtr,
  IN UINTN                ImageSize,
  IN BOOLEAN              IsMacAddressUpdate
  )
{
  EFI_STATUS  Status;
  VOID        *ModuleBufferPtr;
  UINTN       ModuleBufferSize;
  VOID        *EepromBufferPtr;

  ModuleBufferPtr  = NULL;
  ModuleBufferSize = 0;
  EepromBufferPtr  = NULL;

  if ((PciIoProtocolPtr == NULL) || (ImagePtr == NULL) || (ImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetModuleFromBuffer (
             MODULE_SHADOW_RAM,
             ImagePtr,
             ImageSize,
             &ModuleBufferPtr,
             &ModuleBufferSize
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  EepromBufferPtr = (UINT16 *)AllocateZeroPool (ModuleBufferSize);
  if (EepromBufferPtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (EepromBufferPtr, ModuleBufferPtr, ModuleBufferSize);

  Status = UpdateFreeProvAreaPtrAndSize (PciIoProtocolPtr, EepromBufferPtr, ModuleBufferSize);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = WriteShadowRamImageToEeprom (PciIoProtocolPtr, EepromBufferPtr, ModuleBufferSize, IsMacAddressUpdate);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = UpdateNvmEepromChecksum (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = CommitEepromToFlash (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

Exit:
  FreePool (EepromBufferPtr);

  DEBUG ((DEBUG_INFO, "%a - [%r].\n", __func__, Status));

  return Status;
}

/**
  Update the NVM image into flash in blank mode.

  @note Blank flash mode do not have FW running, flash in a primitive mode.

  @param[in]  PciIoProtocolPtr    Pointer to the PCI IO protocol instance.
  @param[in]  ImagePtr            Pointer to the image.
  @param[in]  ImageSize           Size of the image.

  @retval  EFI_SUCCESS            Succeed to update the NVM image into flash.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to update the NVM image into flash.

**/
EFI_STATUS
EFIAPI
FoxvilleUpdateInBlankMode (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN VOID                 *ImagePtr,
  IN UINTN                ImageSize
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (ImagePtr == NULL) || (ImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Flashing over blank flash, please turn off the power after the update.\n"));

  Status = ProgramFlashRegion (PciIoProtocolPtr, 0, ImagePtr, ImageSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to flash the NVM image into blank flash.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Succeed to flash the NVM image into blank flash.\n"));

  return EFI_SUCCESS;
}

/**
  Update the NVM image into flash in unprotected mode.

  @param[in]  HwPtr      Pointer to the HW instance.
  @param[in]  ImagePtr   Pointer to the image.
  @param[in]  ImageSize  Size of the image.

  @retval  EFI_SUCCESS            Succeed to update the NVM image into flash.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_VOLUME_CORRUPTED   Input buffer is corrupted.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to update the NVM image into flash.

**/
EFI_STATUS
EFIAPI
FoxvilleUpdateInUnprotectedMode (
  IN FOXVILLE_HW_INSTANCE  *HwPtr,
  IN VOID                  *ImagePtr,
  IN UINTN                 ImageSize
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsValid;
  VOID        *ModuleInBufferPtr;
  UINTN       ModuleInBufferSize;
  UINTN       FpaModuleAddress;
  UINTN       FpaModuleSize;

  IsValid            = FALSE;
  ModuleInBufferPtr  = NULL;
  ModuleInBufferSize = 0;
  FpaModuleAddress   = 0;
  FpaModuleSize      = 0;

  if ((HwPtr == NULL) || (ImagePtr == NULL) || (ImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check the size of secure module is matched.
  //
  Status = CheckSecureModuleInImageValid (HwPtr->PciIo, ImagePtr, ImageSize, &IsValid);
  if ((EFI_ERROR (Status)) || (!IsValid)) {
    return EFI_VOLUME_CORRUPTED;
  }

  Status = GetModuleFromBuffer (MODULE_SECURE, ImagePtr, ImageSize, &ModuleInBufferPtr, &ModuleInBufferSize);
  if (EFI_ERROR (Status)) {
    return EFI_VOLUME_CORRUPTED;
  }

  //
  // Get the address of FPA to be used for programming.
  //
  Status = GetModuleInfoFromFlash (HwPtr->PciIo, MODULE_FREE_PROV_AREA, &FpaModuleAddress, &FpaModuleSize);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Program the new FW @ 0x%08X (0x%08X).\n", FpaModuleAddress, FpaModuleSize));

  //
  // Check the previous FW update status.
  //
  Status = CheckPreviousFwUpdateStatus (HwPtr->PciIo);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Program the secure module into flash.
  //
  Status = ProgramFlashRegion (HwPtr->PciIo, (FpaModuleAddress & MAX_UINT32), ModuleInBufferPtr, ModuleInBufferSize);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = AuthFwImageFromFlash (HwPtr->PciIo);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Reverting MAC Address\n"));

  Status = UpdateMngCrcs (HwPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "MNGCrCs Updated!\n"));

  //
  // Dump the new shadow RAM image into EEPROM.
  //
  Status = UpdateEepromRegion (HwPtr->PciIo, ImagePtr, ImageSize, FALSE);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}
