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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/DebugLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleNvmMap.h"
#include "Defines/FoxvilleRegs.h"
#include "Misc/FoxvilleFwSts.h"
#include "Misc/FoxvilleRegAccess.h"

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
  )
{
  EFI_STATUS              Status;
  FOXVILLE_FWSM_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (IsValidPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsValidPtr = FALSE;

  Status = ReadFwsm (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.FwValid == V_FWSM_FW_VALID_VALID) {
    *IsValidPtr = TRUE;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS             Status;
  FOXVILLE_EEC_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (IsDetectedPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsDetectedPtr = FALSE;

  Status = ReadEec (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.FlashDetected == V_EEC_FLASH_DETECTED_SET) {
    *IsDetectedPtr = TRUE;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS             Status;
  FOXVILLE_FLA_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (IsLockedPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsLockedPtr = TRUE;

  Status = ReadFla (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.Locked == V_FLA_LOCKED_CLEAR) {
    *IsLockedPtr = FALSE;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsValid;

  IsValid = FALSE;

  if ((PciIoProtocolPtr == NULL) || (AddressPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CheckEecSector1Valid (PciIoProtocolPtr, &IsValid);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (!IsValid) {
    *AddressPtr = SHADOW_RAM_SECTOR_0_ADDRESS;
  } else {
    *AddressPtr = SHADOW_RAM_SECTOR_1_ADDRESS;
  }

  return EFI_SUCCESS;
}

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
  )
{
  if ((PciIoProtocolPtr == NULL) || (IsDonePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  return CheckEecFlashUpdateDone (PciIoProtocolPtr, IsDonePtr);
}

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
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLASHOP_REGISTER  Request;

  Request.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Request.Bits.FlashEraseOp = FLASHOP_FLASH_ERASE;
  Request.Bits.SuspendOp    = FLASHOP_SECTOR_ERASE;

  Status = WriteFlashOp (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsDone;

  IsDone = FALSE;

  if ((PciIoProtocolPtr == NULL) || (IsAuthDonePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsAuthDonePtr = FALSE;

  Status = CheckFlashFwUpdateAuthDone (PciIoProtocolPtr, &IsDone);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (IsDone) {
    *IsAuthDonePtr = TRUE;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_FLFWUPDATE_REGISTER  Response;

  Response.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Trigger flash FW image auth.\n"));

  Status = SetFlashFwUpdateUpdate (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Waiting for flash FW image auth.\n"));

  Status = PollFlashFwUpdateAuthDone (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Flash FW image auth timeout.\n"));
    return EFI_TIMEOUT;
  }

  Status = ReadFlashFwUpdate (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Flash FW image auth finished.\n"));

  if (Response.Bits.AuthFailed == V_FLFWUPDATE_AUTH_FAILED_SET) {
    DEBUG ((DEBUG_ERROR, "Bad flash FW image.\n"));
    return EFI_VOLUME_CORRUPTED;
  }

  DEBUG ((DEBUG_INFO, "Flash FW image authenticated.\n"));

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                Status;
  FOXVILLE_FLSECU_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (IsBlockedPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsBlockedPtr = TRUE;

  Status = ReadFlashSecurity (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.BlockProtectedSwAccess == V_FLSECU_BLOCK_PROTECTED_SW_ACCESS_CLEAR) {
    *IsBlockedPtr = FALSE;
  }

  return EFI_SUCCESS;
}
