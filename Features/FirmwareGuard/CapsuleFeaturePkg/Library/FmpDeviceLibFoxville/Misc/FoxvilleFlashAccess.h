/** @file
  Internal file to access Foxville flash.

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

#ifndef __FOXVILLE_FLASH_ACCESS_H__
#define __FOXVILLE_FLASH_ACCESS_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include "Protocol/PciIo.h"

/**
  Set the number of bytes into the flash burst counter.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  NumOfByte         Indicate number of bytes to set into the counter.

  @retval  EFI_SUCCESS            Succeed to set the flash burst counter.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set the flash burst counter.

**/
EFI_STATUS
EFIAPI
SetFlashBurstCounter (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               NumOfByte
  );

/**
  Polling to check the flash is free.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Device is in free state.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Device is in busy state.

**/
EFI_STATUS
EFIAPI
PollFlashFree (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Set the flash sector erase command via FLSWCTL register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address used to perform the sector erase.

  @retval  EFI_SUCCESS            Succeed to erase the sector in assigned address.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to erase the sector in assigned address.

**/
EFI_STATUS
EFIAPI
SetFlashSwCtlSectorEraseCmd (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Address
  );

/**
  Set the flash write command with assigned address via FLSWCTL register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address used to perform the write.

  @retval  EFI_SUCCESS            Succeed to set the address to be written.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set the address to be written.

**/
EFI_STATUS
EFIAPI
SetFlashSwCtlWriteCmd (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Address
  );

/**
  Set the flash data by sequence via FLSWDATA register.

  @note It is caller responsibility to provide the bursted buffer
        aligned with 4 bytes.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  BufferPtr         Pointer to the buffer to be used.
  @param[in]  BufferSize        Size of the buffer to be used.

  @retval  EFI_SUCCESS            Succeed to set flash data by sequence.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set flash data by sequence.

**/
EFI_STATUS
EFIAPI
SetFlashSwDataInBurst (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN VOID                 *BufferPtr,
  IN UINTN                BufferSize
  );

/**
  Commit the EEPROM content into flash via EEC register.

  @note This function should be used when the FW is active.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to commit the EEPROM content.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to commit the EEPROM content.

**/
EFI_STATUS
EFIAPI
CommitEepromToFlashEec (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

#endif
