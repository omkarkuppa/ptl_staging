/** @file
  Internal file to support the Foxville module in flash operation.

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
#include <Library/DebugLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleModule.h"
#include "Defines/FoxvilleNvmMap.h"
#include "FoxvilleApi.h"
#include "FoxvilleModuleFlash.h"

/**
  Get the free provisioning area module component information from the flash.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   Module            Module type.
  @param[out]  AddressPtr        Pointer to the address of the free provisioning
                                 area module from flash.
  @param[out]  SizePtr           Pointer to the size of the free provisioning
                                 area module from flash.

  @retval  EFI_SUCCESS            Succeed to get the module information from flash.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_NOT_FOUND          Module not found from the flash.
  @retval  Others                 Failed to get the module information from flash.

**/
EFI_STATUS
GetModuleInfoFpaFromFlash (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINTN                *AddressPtr,
     OUT UINTN                *SizePtr
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_FREE_PROV_AREA_PTR   FreeProvAreaOffset;
  FOXVILLE_FREE_PROV_AREA_SIZE  FreeProvAreaSize;

  FreeProvAreaOffset.Uint16 = 0;
  FreeProvAreaSize.Uint16   = 0;

  if ((PciIoProtocolPtr == NULL) || (AddressPtr == NULL) || (SizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the FPA offset from EEPROM.
  //
  Status = ReadNvmEepromUint16 (
             PciIoProtocolPtr,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaPtr),
             (UINT16 *)(&(FreeProvAreaOffset.Uint16))
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if ((FreeProvAreaOffset.Uint16 == 0x0000) || (FreeProvAreaOffset.Uint16 == MAX_UINT16)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the FPA address in flash.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Get the FPA size from EEPROM.
  //
  Status = ReadNvmEepromUint16 (
             PciIoProtocolPtr,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FreeProvAreaSize),
             (UINT16 *)(&(FreeProvAreaSize.Uint16))
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if ((FreeProvAreaSize.Uint16 == 0x0000) || (FreeProvAreaSize.Uint16 == MAX_UINT16)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the FPA size in flash.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Return the FPA address and size.
  //
  if (FreeProvAreaOffset.Bits.Is4KPtrType) {
    *AddressPtr = FreeProvAreaOffset.Bits.Pointer * SIZE_4KB;
  } else {
    *AddressPtr = FreeProvAreaOffset.Bits.Pointer;
  }

  *SizePtr = FreeProvAreaSize.Uint16 * SIZE_4KB;

  return EFI_SUCCESS;
}

/**
  Get the offset of secure module component from the flash.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  OffsetPtr         Pointer to the offset to the secure module.

  @retval  EFI_SUCCESS            Succeed to get the offset to the secure module.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the offset to the secure module.

**/
EFI_STATUS
GetModuleSecureOffsetFromFlash (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINTN                *OffsetPtr
  )
{
  EFI_STATUS           Status;
  UINT16               OffsetInWord;
  FOXVILLE_MODULE_PTR  FoxvilleModulePtr;

  OffsetInWord             = 0;
  FoxvilleModulePtr.Uint16 = 0;

  if ((PciIoProtocolPtr == NULL) || (OffsetPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  OffsetInWord = WORD_OFFSET_OF (FOXVILLE_NVM_MAP, ExpRomAreaPtr);

  //
  // If expansion ROM is not presented, then the first module is PHY module.
  //
  Status = ReadNvmEepromUint16 (
             PciIoProtocolPtr,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, ExpRomAreaPtr),
             &(FoxvilleModulePtr.Uint16)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if ((FoxvilleModulePtr.Uint16 == 0x0000) || (FoxvilleModulePtr.Uint16 == MAX_UINT16)) {
    DEBUG ((DEBUG_INFO, "Expansion ROM is not presented in flash.\n"));
    OffsetInWord = FXOVILLE_PHY_MODULE_WORD_OFFSET;
  }

  //
  // Get the secure modules offset in flash.
  //
  Status = ReadNvmEepromUint16 (
             PciIoProtocolPtr,
             OffsetInWord,
             &(FoxvilleModulePtr.Uint16)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (FoxvilleModulePtr.Bits.Is4KPtrType) {
    *OffsetPtr = FoxvilleModulePtr.Bits.Pointer * SIZE_4KB;
  } else {
    *OffsetPtr = FoxvilleModulePtr.Bits.Pointer;
  }

  return EFI_SUCCESS;
}

/**
  Get the secure module component information from the flash.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   Module            Module type.
  @param[out]  AddressPtr        Pointer to the address of the secure module from flash.
  @param[out]  SizePtr           Pointer to the size of the secure module from flash.

  @retval  EFI_SUCCESS            Succeed to get the module information from flash.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_FOUND          Module not found from the flash.
  @retval  Others                 Failed to get the module information from flash.

**/
EFI_STATUS
GetModuleInfoSecureFromFlash (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINTN                *AddressPtr,
     OUT UINTN                *SizePtr
  )
{
  EFI_STATUS  Status;
  UINTN       FpaModuleOffset;
  UINTN       FpaModuleSize;
  UINTN       SecureModuleOffset;

  FpaModuleOffset    = 0;
  FpaModuleSize      = 0;
  SecureModuleOffset = 0;

  if ((PciIoProtocolPtr == NULL) || (AddressPtr == NULL) || (SizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetModuleInfoFpaFromFlash (
             PciIoProtocolPtr,
             &FpaModuleOffset,
             &FpaModuleSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the FPA information in flash.\n"));
    return EFI_NOT_FOUND;
  }

  Status = GetModuleSecureOffsetFromFlash (
             PciIoProtocolPtr,
             &SecureModuleOffset
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the secure module offset in flash.\n"));
    return EFI_NOT_FOUND;
  }

  *AddressPtr = SecureModuleOffset;
  *SizePtr    = FpaModuleSize;

  return EFI_SUCCESS;
}

/**
  Get the module component information from the flash.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   Module            Module type.
  @param[out]  AddressPtr        Pointer to indicate the address of the module from flash.
  @param[out]  SizePtr           Pointer to indicate the size of the module from flash.

  @retval  EFI_SUCCESS            Succeed to get the module information from flash.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        Unsupported module type.
  @retval  Others                 Failed to get the module information from flash.

**/
EFI_STATUS
EFIAPI
GetModuleInfoFromFlash (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     FOXVILLE_MODULE      Module,
     OUT UINTN                *AddressPtr,
     OUT UINTN                *SizePtr
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (AddressPtr == NULL) || (SizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Module) {
    case MODULE_SECURE:
      Status = GetModuleInfoSecureFromFlash (PciIoProtocolPtr, AddressPtr, SizePtr);
      break;

    case MODULE_FREE_PROV_AREA:
      Status = GetModuleInfoFpaFromFlash (PciIoProtocolPtr, AddressPtr, SizePtr);
      break;

    default:
      DEBUG ((DEBUG_ERROR, "%a: Unsupported module type - [0x%04X].\n", __func__, Module));
      return EFI_UNSUPPORTED;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the module [0x%04X] info from flash - %r\n", Module, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Get the module [0x%04X] info from flash @ 0x%08X (0x%08X).\n", Module, *AddressPtr, *SizePtr));

  return EFI_SUCCESS;
}
