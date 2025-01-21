/** @file
  Internal implementation for Platform Flash Access library.
  Internal function for PlatformFlashAccessLib.
  PlatformFlashAccessLib.c will call these functions for flash update.

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>

#include <BiosGuard.h>
#include <Register/EspiRegs.h>

//
// Used in SPI update function
//
#define UPDATE_BLOCK_SIZE                      SIZE_4KB

/**
  Update a block (with UPDATE_BLOCK_SIZE) on SPI flash.

  This function updates one block to FlashRegionType region with give Address in below steps:
  1. Read one block from the destination and compare it with Buffer.
     If the content is identical, no further action is needed. Return EFI_SUCCESS directly.
  2. Erase one block from the destination.
  3. Write one block to the destination with Buffer.
  4. Read the block back from destination and compare it to ensure the write operation is successful.

  @param[in]  WriteReq                 Request information for update flash.

  @retval     EFI_SUCCESS.             Operation is successful.
  @retval     EFI_OUT_OF_RESOURCES     Failed to allocate needed memory buffer.
  @retval     EFI_VOLUME_CORRUPTED     The block is not updated as expected.
  @retval     Others                   If there is any device errors.

**/
EFI_STATUS
InternalSpiFlashUpdateBlock (
  IN FLASH_ACCESS_REQUEST  *WriteReq
  );

/**
  Update a buffer to SPI flash.
  The size of the buffer must be aligned to UPDATE_BLOCK_SIZE.

  @param[in]  WriteReq                 Request information for write SPI flash.
  @param[in]  Progress                 A function used report the progress of the
                                       firmware update.  This is an optional parameter
                                       that may be NULL.
  @param[in]  StartPercentage          The start completion percentage value that may
                                       be used to report progress during the flash
                                       write operation.
  @param[in]  EndPercentage            The end completion percentage value that may
                                       be used to report progress during the flash
                                       write operation.

  @retval     EFI_SUCCESS              Operation is successful.
  @retval     EFI_INVALID_PARAMETER    Buffer is NULL or FlashAddress/Length is not well aligned.
  @retval     EFI_OUT_OF_RESOURCES     Failed to allocate needed memory buffer.
  @retval     EFI_VOLUME_CORRUPTED     SPI flash is not updated as expected.
  @retval     Others                   If there is any device errors.

**/
EFI_STATUS
InternalSpiFlashUpdate (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );

/**
  Update BIOS via BiosGuard service.

  @param[in]  WriteReq                 Request information for write SPI flash.
  @param[in]  Progress                 A function used report the progress of the
                                       firmware update.  This is an optional parameter
                                       that may be NULL.
  @param[in]  StartPercentage          The start completion percentage value that may
                                       be used to report progress during the flash
                                       write operation.
  @param[in]  EndPercentage            The end completion percentage value that may
                                       be used to report progress during the flash
                                       write operation.

  @retval EFI_SUCCESS                 The System Firmware image is updated successfully.
  @retval EFI_SECURITY_VIOLATION      The update operation fails due to SVN check error.
  @retval EFI_DEVICE_ERROR            The update operation fails.
  @retval EFI_INVALID_PARAMETER       BgupImage is NULL or BgupImageSize is unexpected.
  @retval Others                      The update operation fails.

**/
EFI_STATUS
InternalBiosGuardUpdate (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );
