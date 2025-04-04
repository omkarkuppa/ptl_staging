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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleCommon.h"
#include "Defines/FoxvilleNvmMap.h"
#include "Misc/FoxvilleFlashAccess.h"
#include "Misc/FoxvilleFwSts.h"
#include "Misc/FoxvilleNvmAccess.h"
#include "Misc/FoxvillePhy.h"
#include "Misc/FoxvilleSemaphore.h"
#include "FoxvilleApi.h"

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
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (DataPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AcquireNvmAccessSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = ReadNvmEerdUint16 (PciIoProtocolPtr, Address, DataPtr);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

Exit:
  if (EFI_ERROR (ReleaseNvmAccessSemaphore (PciIoProtocolPtr))) {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

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
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AcquireNvmAccessSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = WriteNvmEewrUint16 (PciIoProtocolPtr, Address, Data);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

Exit:
  if (EFI_ERROR (ReleaseNvmAccessSemaphore (PciIoProtocolPtr))) {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

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
  )
{
  EFI_STATUS  Status;
  UINT16      NvmValue;
  UINT16      Offset;
  UINT16      Checksum;

  NvmValue = 0;
  Offset   = 0;
  Checksum = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read the 1st WORD from EEPROM to prevent waiting for long time for EEPROM READ/WRITE timeout.
  //
  Status = ReadNvmEepromUint16 (PciIoProtocolPtr, 0, &NvmValue);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Read needed WORD from EEPROM to calculate the checksum.
  //
  for (Offset = 0; Offset < WORD_OFFSET_OF (FOXVILLE_NVM_MAP, Checksum); Offset++) {
    Status = ReadNvmEepromUint16 (PciIoProtocolPtr, Offset, &NvmValue);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    Checksum += NvmValue;
  }

  Checksum = (UINT16)(FOXVILLE_CHECKSUM_VALUE - Checksum);

  //
  // Write the checksum value into EEPROM.
  //
  Status = WriteNvmEepromUint16 (
             PciIoProtocolPtr,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, Checksum),
             Checksum
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to update Checksum field in EEPROM.\n"));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (IsValidPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CheckFwsmFwValidBit (PciIoProtocolPtr, IsValidPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (VersionPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadNvmEepromUint16 (
             PciIoProtocolPtr,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, DevStarterVersion),
             VersionPtr
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsFlashDetected;
  BOOLEAN     IsNvmLocked;
  BOOLEAN     IsFwValid;

  if ((PciIoProtocolPtr == NULL) || (FlashModePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if flash is detected.
  //
  Status = CheckEecFlashDetectedBit (PciIoProtocolPtr, &IsFlashDetected);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (!IsFlashDetected) {
    *FlashModePtr = FLASH_MODE_NO_FLASH_DETECTED;
    return EFI_SUCCESS;
  }

  //
  // Check if flash is protected.
  //
  Status = CheckFlaLocked (PciIoProtocolPtr, &IsNvmLocked);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (IsNvmLocked) {
    *FlashModePtr = FLASH_MODE_PROTECTED;
    return EFI_SUCCESS;
  }

  //
  // Check if flash is blank or content is invalid.
  //
  Status = GetFwValidFromFlash (PciIoProtocolPtr, &IsFwValid);
  if ((EFI_ERROR (Status)) || (!IsFwValid)) {
    *FlashModePtr = FLASH_MODE_BLANK;
    return EFI_SUCCESS;
  }

  //
  // Not matched the cases above, flash in under unprotected.
  //
  *FlashModePtr = FLASH_MODE_UNPROTECTED;

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                  Status;
  FOXVILLE_FLASH_MODE         FlashMode;
  FOXVILLE_FLASH_DEVICE_SIZE  FlashDeviceSize;

  if ((PciIoProtocolPtr == NULL) || (FlashSizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetFlashMode (PciIoProtocolPtr, &FlashMode);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *FlashSizePtr = 0;

  if (FlashMode == FLASH_MODE_BLANK) {
    *FlashSizePtr = SIZE_2MB;
    return EFI_SUCCESS;
  }

  Status = ReadNvmEepromUint16 (
             PciIoProtocolPtr,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FlashDeviceSize),
             &(FlashDeviceSize.Uint16)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = EFI_SUCCESS;
  switch (FlashDeviceSize.Bits.FlashSize) {
    case V_FOXVILLE_FLASH_SIZE_1MB:
      *FlashSizePtr = SIZE_1MB;
      break;

    case V_FOXVILLE_FLASH_SIZE_2MB:
      *FlashSizePtr = SIZE_2MB;
      break;

    default:
      DEBUG ((DEBUG_ERROR, "Unrecognized the flash size = [0x%X]\n", FlashDeviceSize.Bits.FlashSize));
      return EFI_DEVICE_ERROR;
  }

  return Status;
}

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
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SetFlashOpRegister (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  UINT32      CurrentNumOfSectors;
  UINT32      EraseNumOfSectors;
  UINT32      TotalNumOfSectors;
  UINT32      Index;
  UINT32      EraseAddress;

  CurrentNumOfSectors = 0;
  EraseNumOfSectors   = 0;
  TotalNumOfSectors   = 0;
  Index               = 0;
  EraseAddress        = 0;

  DEBUG ((DEBUG_INFO, "%a: Address - 0x%08X; Length - 0x%08X\n", __func__, Address, Length));

  if ((PciIoProtocolPtr == NULL) || (Length == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (((Address % FOXVILLE_FLASH_SECTOR_SIZE) != 0) || ((Length % FOXVILLE_FLASH_SECTOR_SIZE) != 0)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Wait for the flash is free.
  //
  Status = PollFlashFree (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  TotalNumOfSectors = Length / FOXVILLE_FLASH_SECTOR_SIZE;

  //
  // Looping to erase the assigned address and length.
  //
  while (CurrentNumOfSectors < TotalNumOfSectors) {
    //
    // Calculate the number of sectors to be erased.
    //
    EraseNumOfSectors = FOXVILLE_FLASH_SECTOR_ERASE_MAX;
    if ((TotalNumOfSectors - CurrentNumOfSectors) < FOXVILLE_FLASH_SECTOR_ERASE_MAX) {
      EraseNumOfSectors = TotalNumOfSectors - CurrentNumOfSectors;
    }

    Status = AcquireNvmAccessSemaphore (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    for (Index = 0; Index < EraseNumOfSectors; Index++) {
      EraseAddress = Address + ((CurrentNumOfSectors + Index) * FOXVILLE_FLASH_SECTOR_SIZE);

      Status = SetFlashSwCtlSectorEraseCmd (PciIoProtocolPtr, EraseAddress);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to erase the sector @ 0x%08X\n", EraseAddress));
        goto Release;
      }
    }

    Status = ReleaseNvmAccessSemaphore (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    gBS->Stall (FOXVILLE_FLASH_SECTOR_ERASE_TIMEOUT);

    CurrentNumOfSectors += EraseNumOfSectors;
  }

  //
  // Finish the sectors erase operation.
  //
  goto Exit;

Release:
  if (EFI_ERROR (ReleaseNvmAccessSemaphore (PciIoProtocolPtr))) {
    return EFI_DEVICE_ERROR;
  }

Exit:

  return Status;
}

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
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  FlashContentAddress;
  VOID                  *FlashContentPtr;
  UINTN                 FlashContentSize;
  UINT32                CurrentOffset;
  UINT32                BurstCounter;
  UINT32                AlignedBurstCounter;

  FlashContentAddress = 0;
  FlashContentPtr     = NULL;
  FlashContentSize    = 0;
  CurrentOffset       = 0;
  BurstCounter        = 0;
  AlignedBurstCounter = 0;

  DEBUG ((DEBUG_INFO, "%a: Address - 0x%08X; Size - 0x%08X; Buffer - 0x%08X\n", __func__, Address, BufferSize, BufferPtr));

  if ((PciIoProtocolPtr == NULL) || (BufferPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((BufferSize == 0) || (BufferSize > MAX_UINT32)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the aligned size with multiple of 4.
  //
  FlashContentSize = BufferSize;
  if ((FlashContentSize % sizeof (UINT32)) != 0) {
    FlashContentSize += sizeof (UINT32) - (BufferSize % sizeof (UINT32));
  }

  //
  // Make the flash content aligned with multiple of 4.
  //
  FlashContentPtr = AllocateZeroPool (FlashContentSize);
  if (FlashContentPtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a: Aligned Buffer - 0x%08X; Size - 0x%08X\n", __func__, FlashContentPtr, FlashContentSize));

  FlashContentAddress = (EFI_PHYSICAL_ADDRESS)FlashContentPtr;

  CopyMem (FlashContentPtr, BufferPtr, BufferSize);

  //
  // Wait for the flash is free.
  //
  Status = PollFlashFree (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Looping to write the content into flash.
  //
  while (CurrentOffset < BufferSize) {
    //
    // Attempt to burst the content with possible maximum bytes.
    // Otherwise, burst the remain bytes based on actual input buffer size.
    //
    BurstCounter = FOXVILLE_FLASH_WRITE_BYTE_SIZE_MAX;
    if ((BufferSize - CurrentOffset) < FOXVILLE_FLASH_WRITE_BYTE_SIZE_MAX) {
      BurstCounter = (BufferSize & MAX_UINT32) - CurrentOffset;
    }

    Status = SetFlashBurstCounter (PciIoProtocolPtr, BurstCounter);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    //
    // Input buffer is aligned with multiple of 4.
    // Burst counter must set into actual size but the input buffer shall set into aligned size.
    //
    AlignedBurstCounter = BurstCounter;
    if ((BurstCounter % sizeof (UINT32)) != 0) {
      AlignedBurstCounter += sizeof (UINT32) - (BurstCounter % sizeof (UINT32));
    }

    //
    // Set the flash address.
    //
    Status = SetFlashSwCtlWriteCmd (PciIoProtocolPtr, (Address + CurrentOffset));
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    //
    // Wait for the flash is free.
    //
    Status = PollFlashFree (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    //
    // Burst the content into the flash.
    //
    Status = AcquireNvmAccessSemaphore (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    Status = SetFlashSwDataInBurst (PciIoProtocolPtr, (VOID *)FlashContentAddress, AlignedBurstCounter);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set the flash data into FLSWDATA - %r\n", Status));
      goto Release;
    }

    Status = ReleaseNvmAccessSemaphore (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    //
    // Move to next offset based on bursted size.
    //
    CurrentOffset       += BurstCounter;
    FlashContentAddress += BurstCounter;
  }

  //
  // Finish the writes operation.
  //
  goto Exit;

Release:
  if (EFI_ERROR (ReleaseNvmAccessSemaphore (PciIoProtocolPtr))) {
    Status = EFI_DEVICE_ERROR;
  }

Exit:
  FreePool (FlashContentPtr);

  return Status;
}

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
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsAuthDone;
  BOOLEAN     IsFlashUpdateDone;

  IsAuthDone        = FALSE;
  IsFlashUpdateDone = FALSE;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CheckFlashFwImageAuthDone (PciIoProtocolPtr, &IsAuthDone);
  if ((EFI_ERROR (Status)) || (!IsAuthDone)) {
    DEBUG ((DEBUG_ERROR, "Previous FW update failed.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Previous FW update succeeded.\n"));

  Status = CheckEepromUpdateDone (PciIoProtocolPtr, &IsFlashUpdateDone);
  if ((EFI_ERROR (Status)) || (!IsFlashUpdateDone)) {
    DEBUG ((DEBUG_ERROR, "Previous EEPROM dump failed.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Previous EEPROM dump succeeded.\n"));

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AuthFlashFwImage (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  UINT16      Offset;
  UINT16      Data;
  UINT32      SectorAddress;

  Offset        = 0;
  Data          = 0;
  SectorAddress = 0;

  DEBUG ((DEBUG_INFO, "Commit the EEPROM into flash via SW shadow RAM dump.\n"));

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get valid shadow RAM address
  //
  Status = GetValidShadowRamSectorAddress (PciIoProtocolPtr, &SectorAddress);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Valid shadow RAM sector address = 0x%08X\n", SectorAddress));

  //
  // Erase the sector for the valid shadow RAM.
  //
  Status = FlashSectorErase (PciIoProtocolPtr, SectorAddress, SHADOW_RAM_SECTOR_SIZE);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Write the EEPROM data into flash by WORD.
  //
  for (Offset = 0; Offset < (SHADOW_RAM_SECTOR_SIZE / sizeof (UINT16)); Offset++) {
    Status = ReadNvmEepromUint16 (PciIoProtocolPtr, Offset, &Data);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to read the EEPROM @ [0x%08X]\n", Offset));
      return EFI_DEVICE_ERROR;
    }

    Status = FlashWrite (
               PciIoProtocolPtr,
               (SectorAddress + (Offset * sizeof (UINT16))),
               (VOID *)&Data,
               sizeof (Data)
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to commit the EEPROM data into flash @ [0x%08X]\n", Offset));
      return EFI_DEVICE_ERROR;
    }

    gBS->Stall (FOXVILLE_COMMIT_EEPROM_TIMEOUT);
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsFwValid;
  BOOLEAN     IsBlocked;

  IsFwValid = FALSE;
  IsBlocked = TRUE;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetFwValidFromFlash (PciIoProtocolPtr, &IsFwValid);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = CheckFlashSecurityBlockProtectedSwAccess (PciIoProtocolPtr, &IsBlocked);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (IsFwValid) {
    Status = CommitEepromToFlashEec (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  } else if (!IsBlocked) {
    Status = CommitEepromToFlashSwDump (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Failed to commit EEPROM to flash due to under protected.\n"));
    return EFI_ACCESS_DENIED;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;

  Status = PhyReset (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Succeed to reload the GPHY FW.\n"));

  return EFI_SUCCESS;
}
