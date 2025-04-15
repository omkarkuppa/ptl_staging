/** @file
  Internal file to provide the generic API for Foxville.

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

#ifndef __FOXVILLE_API_H__
#define __FOXVILLE_API_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
//
// Self-Mode Foundation.
//
#include "Defines/FoxvilleCommon.h"

#define FOXVILLE_FLASH_SECTOR_SIZE           SIZE_4KB
#define FOXVILLE_FLASH_SECTOR_ERASE_MAX      4
#define FOXVILLE_FLASH_SECTOR_ERASE_TIMEOUT  10000
#define FOXVILLE_FLASH_WRITE_BYTE_SIZE_MAX   256
#define FOXVILLE_COMMIT_EEPROM_TIMEOUT       1000

/**
  Read the value from assigned address of the word via EEPROM.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   Address           Address of the word in read.
  @param[out]  DataPtr           Pointer to return the value read from.

  @retval  EFI_SUCCESS            Succeed to read the value via EEPROM.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the value via EEPROM.

**/
EFI_STATUS
EFIAPI
ReadNvmEepromUint16 (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     UINT16               Address,
     OUT UINT16               *DataPtr
  );

/**
  Write the value to assigned address of the word via EEPROM.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address of the word in write.
  @param[in]  Data              Data used to write into assigned address.

  @retval  EFI_SUCCESS            Succeed to write the value via EEPROM.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the value via EEPROM.

**/
EFI_STATUS
EFIAPI
WriteNvmEepromUint16 (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT16               Address,
  IN UINT16               Data
  );

/**
  Update the checksum field in EEPROM.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to update checksum field in EEPROM.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to update checksum field in EEPROM.

**/
EFI_STATUS
EFIAPI
UpdateNvmEepromChecksum (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Get the validity of FW from flash.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsValidPtr        Pointer to indicate the valid of FW.

  @retval  EFI_SUCCESS            Succeed to get the FW valid bit.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the FW valid bit.

**/
EFI_STATUS
EFIAPI
GetFwValidFromFlash (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsValidPtr
  );

/**
  Get the NVM version from flash.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  VersionPtr        Pointer to return the NVM version.

  @retval  EFI_SUCCESS            Succeed to read the NVM version.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the the NVM version.

**/
EFI_STATUS
EFIAPI
GetNvmVersionFromFlash (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT16               *VersionPtr
  );

/**
  Get the flash mode.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  FlashModePtr      Pointer to return flash mode detected.

  @retval  EFI_SUCCESS            Succeed to get the flash mode.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the flash mode.

**/
EFI_STATUS
EFIAPI
GetFlashMode (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT FOXVILLE_FLASH_MODE  *FlashModePtr
  );

/**
  Get the flash size.

  @note If the flash is blank, then assume the flash is available
        to accommodate the maximum possible size of NVM image.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  FlashSizePtr      Pointer to return size of flash.

  @retval  EFI_SUCCESS            Succeed to get the flash size.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the flash size.

**/
EFI_STATUS
EFIAPI
GetFlashSize (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *FlashSizePtr
  );

/**
  Configure the required registers for Foxville device initialization.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to configure the device registers.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to configure the device registers.

**/
EFI_STATUS
EFIAPI
ConfigRegisterForDeviceInit (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Perform the flash sector erase with assigned address and length.

  @note The address and length shall be aligned with sector size.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address used to perform the sector erase.
  @param[in]  Length            Length of the region used to erase.

  @retval  EFI_SUCCESS            Succeed to perform flash sector erase.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to perform flash sector erase.

**/
EFI_STATUS
EFIAPI
FlashSectorErase (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Address,
  IN UINT32               Length
  );

/**
  Perform the flash write with assigned address and buffer.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address used to perform the write operation.
  @param[in]  BufferPtr         Pointer to the buffer to be used.
  @param[in]  BufferSize        Size of the buffer to be used.

  @retval  EFI_SUCCESS            Succeed to perform flash write operation.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_OUT_OF_RESOURCES   Failed to allocate needed memory buffer.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to perform flash write operation.

**/
EFI_STATUS
EFIAPI
FlashWrite (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Address,
  IN VOID                 *BufferPtr,
  IN UINTN                BufferSize
  );

/**
  Check the status of previous FW update.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Previous FW update is succeed.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Previous FW update is failed.

**/
EFI_STATUS
EFIAPI
CheckPreviousFwUpdateStatus (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Authenticate the FW image from flash.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to authenticate the FW image from flash.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  EFI_VOLUME_CORRUPTED   Bad FW image.
  @retval  Others                 Failed to authenticate the FW image from flash.

**/
EFI_STATUS
EFIAPI
AuthFwImageFromFlash (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Commit the EEPROM content into flash via direct flash access.

  @note This function should be used when the FW is not active
        and security protection is disabled.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to commit the EEPROM content.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to commit the EEPROM content.

**/
EFI_STATUS
EFIAPI
CommitEepromToFlashSwDump (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Commit the EEPROM content into flash.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to commit the EEPROM content.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_ACCESS_DENIED      Flash is under protected and failed to access.
  @retval  Others                 Failed to commit the EEPROM content.

**/
EFI_STATUS
EFIAPI
CommitEepromToFlash (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Reload the GPHY FW.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to reload the GPHY FW.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to reload the GPHY FW.

**/
EFI_STATUS
EFIAPI
ReloadGphyFw (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

#endif
