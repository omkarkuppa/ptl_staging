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

#ifndef __FOXVILLE_UPDATE_H__
#define __FOXVILLE_UPDATE_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>

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
EFIAPI
ProgramFlashRegion (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Address,
  IN VOID                 *BufferPtr,
  IN UINTN                BufferSize
  );

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
EFIAPI
CheckSecureModuleInImageValid (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     VOID                 *ImagePtr,
  IN     UINTN                ImageSize,
     OUT BOOLEAN              *IsValidPtr
  );

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
EFIAPI
UpdateEepromRegion (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN VOID                 *ImagePtr,
  IN UINTN                ImageSize,
  IN BOOLEAN              IsMacAddressUpdate
  );

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
  );

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
  );

#endif
