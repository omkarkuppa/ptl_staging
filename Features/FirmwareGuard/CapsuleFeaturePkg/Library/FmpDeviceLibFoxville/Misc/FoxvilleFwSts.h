/** @file
  Internal file to get Foxville firmware status.

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

#ifndef __FOXVILLE_FW_STS_H__
#define __FOXVILLE_FW_STS_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>

/**
  Check the FW valid bit via FWSM register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsValidPtr        Pointer to indicate the valid of FW.

  @retval  EFI_SUCCESS            Succeed to check the FW valid bit.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to check the FW valid bit.

**/
EFI_STATUS
EFIAPI
CheckFwsmFwValidBit (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsValidPtr
  );

/**
  Check the flash detected bit via EEC register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsDetectedPtr     Pointer to indicate the flash is detected.

  @retval  EFI_SUCCESS            Succeed to check the flash detected bit.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to check the flash detected bit.

**/
EFI_STATUS
EFIAPI
CheckEecFlashDetectedBit (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsDetectedPtr
  );

/**
  Check the NVM locked bit via FLA register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsLockedPtr       Pointer to indicate the NVM is in secure mode.

  @retval  EFI_SUCCESS            Succeed to check the NVM locked bit.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to check the NVM locked bit.

**/
EFI_STATUS
EFIAPI
CheckFlaLocked (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsLockedPtr
  );

/**
  Get the address of valid shadow RAM sector.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  AddressPtr        Pointer to the address of valid shadow RAM sector.

  @retval  EFI_SUCCESS            Succeed to get the address of valid shadow RAM sector.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the address of valid shadow RAM sector.

**/
EFI_STATUS
EFIAPI
GetValidShadowRamSectorAddress (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *AddressPtr
  );

/**
  Check EEPROM dump status.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsDonePtr        Pointer to check the status of EEPROM dump.

  @retval  EFI_SUCCESS            Succeed to check the EEPROM dump status.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to check the EEPROM dump status.

**/
EFI_STATUS
EFIAPI
CheckEepromUpdateDone (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsDonePtr
  );

/**
  Set the FLASHOP register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to set the FLASHOP register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set the FLASHOP register.

**/
EFI_STATUS
EFIAPI
SetFlashOpRegister (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Check the flash FW image authentication done status.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsAuthDonePtr     Pointer to indicate the authentication done status.

  @retval  EFI_SUCCESS            Succeed to indicate authentication done status.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to indicate authentication done status.

**/
EFI_STATUS
EFIAPI
CheckFlashFwImageAuthDone (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsAuthDonePtr
  );

/**
  Authenticate the flash FW image.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to authenticate the flash FW image.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  EFI_VOLUME_CORRUPTED   Bad FW image.
  @retval  Others                 Failed to authenticate the flash FW image.

**/
EFI_STATUS
EFIAPI
AuthFlashFwImage (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Check the block protected SW access bit via FLSECU register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsBlockedPtr      Pointer to indicate the block protected SW access bit.

  @retval  EFI_SUCCESS            Succeed to check the block protected SW access bit.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to check the block protected SW access bit.

**/
EFI_STATUS
EFIAPI
CheckFlashSecurityBlockProtectedSwAccess (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsBlockedPtr
  );

#endif
