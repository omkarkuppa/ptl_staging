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

#include <Uefi.h>
#include "BgupImageSupport.h"
#include "PlatformFlashAccessLibInternal.h"

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
  )
{
  EFI_STATUS                           Status;
  UINT8                                *CompareBuffer;
  PCH_SPI_PROTOCOL                     *SpiProtocol;

  ASSERT(WriteReq->DataSize == UPDATE_BLOCK_SIZE);

  CompareBuffer = NULL;

  CompareBuffer = AllocateZeroPool (WriteReq->DataSize);
  if (CompareBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  SpiProtocol = GetPchSpiProtocol();
  if (SpiProtocol == NULL) {
    Status = EFI_NOT_FOUND;
    goto Done;
  }

  //
  // Compare Buffer block with the destination
  //   Caution! As TopSwap bit may be toggled, We can't use MMIO read, instead expect SPI read to read use SPI address
  //
  Status = SpiProtocol->FlashRead (
                          SpiProtocol,
                          WriteReq->FlashRegionType,
                          (UINT32)WriteReq->FlashAddress,
                          (UINT32)WriteReq->DataSize,
                          CompareBuffer
                          );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SpiFlashUpdateBlock: FlashRead failed at Region %d, Address 0x%x\n", WriteReq->FlashRegionType, WriteReq->FlashAddress));
    goto Done;
  }

  if (CompareMem (CompareBuffer, WriteReq->Data, WriteReq->DataSize) == 0) {
    //
    // No need to update this block
    //
    Status = EFI_SUCCESS;
    goto Done;
  }

  //
  // Erase the block
  //
  Status = SpiProtocol->FlashErase (
                          SpiProtocol,
                          WriteReq->FlashRegionType,
                          (UINT32)WriteReq->FlashAddress,
                          (UINT32)WriteReq->DataSize
                          );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SpiFlashUpdateBlock: FlashErase failed at Region %d, Address 0x%x\n", WriteReq->FlashRegionType, WriteReq->FlashAddress));
    goto Done;
  }

  //
  // Wrtie the block
  //
  Status = SpiProtocol->FlashWrite (
                          SpiProtocol,
                          WriteReq->FlashRegionType,
                          (UINT32)WriteReq->FlashAddress,
                          (UINT32)WriteReq->DataSize,
                          WriteReq->Data
                          );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SpiFlashUpdateBlock: FlashErase failed at Region %d, Address 0x%x\n", WriteReq->FlashRegionType, WriteReq->FlashAddress));
    goto Done;
  }

  //
  // Read buffer back to verify udpate status
  //
  ZeroMem (CompareBuffer, WriteReq->DataSize);
  Status = SpiProtocol->FlashRead (
                          SpiProtocol,
                          WriteReq->FlashRegionType,
                          (UINT32)WriteReq->FlashAddress,
                          (UINT32)WriteReq->DataSize,
                          CompareBuffer
                          );

  if (CompareMem (CompareBuffer, WriteReq->Data, WriteReq->DataSize) != 0) {
    //
    // Block is not updated as expected.
    //
    Status = EFI_VOLUME_CORRUPTED;
    DEBUG ((DEBUG_ERROR, "SpiFlashUpdateBlock: Update failed at Region %d, Address 0x%x\n", WriteReq->FlashRegionType, WriteReq->FlashAddress));
  }

Done:
  if (CompareBuffer !=  NULL) {
    FreePool (CompareBuffer);
  }

  return Status;
}


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
  )
{
  EFI_STATUS                           Status;
  UINTN                                Index;
  UINTN                                CountOfBlocks;
  UINTN                                AlignedHead;
  UINTN                                AlignedTail;
  UINT8                                *AlignedBuffer;
  FLASH_ACCESS_REQUEST                 AlignedWriteReq;
  FLASH_ACCESS_REQUEST                 TempReq;

  DEBUG ((DEBUG_INFO, "SpiFlashUpdate - Region  %d\n", (UINTN)WriteReq->FlashRegionType));
  DEBUG ((DEBUG_INFO, "SpiFlashUpdate - Address 0x%x\n", WriteReq->FlashAddress));
  DEBUG ((DEBUG_INFO, "SpiFlashUpdate - Length  0x%x(%d)\n", WriteReq->DataSize, WriteReq->DataSize));

  if (WriteReq->Data == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status        = EFI_SUCCESS;
  AlignedBuffer = NULL;

  //****************************************************************************
  // First Step:
  //    SPI flash is updated in unit of block.  To support Byte stream update, we need to handle
  //    unaligned header or tail before update.
  //
  //****************************************************************************
  AlignedHead   = WriteReq->FlashAddress & ~(UPDATE_BLOCK_SIZE - 1);
  AlignedTail   = (WriteReq->FlashAddress + WriteReq->DataSize + (UPDATE_BLOCK_SIZE - 1)) & ~(UPDATE_BLOCK_SIZE - 1);

  //
  //    Copy original write request to a new aligned request
  //
  CopyMem(&AlignedWriteReq, WriteReq, sizeof(FLASH_ACCESS_REQUEST));


  //
  // Update new aligned request with aligned buffer.  Make sure update request is block aligned both at the beginning and end
  //
  if (WriteReq->DataSize != AlignedTail - AlignedHead) {

    AlignedBuffer = AllocatePool(AlignedTail - AlignedHead);
    if (AlignedBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    AlignedWriteReq.FlashAddress   = AlignedHead;
    AlignedWriteReq.Data           = AlignedBuffer;
    AlignedWriteReq.DataSize       = AlignedTail - AlignedHead;

    ZeroMem(&TempReq, sizeof(FLASH_ACCESS_REQUEST));

    //
    // 1. Copy content from Header alignment area
    //
    if (AlignedHead != WriteReq->FlashAddress) {

      //
      // Init a new tempReq for Header alignment read
      //
      TempReq.FlashRegionType = WriteReq->FlashRegionType;
      TempReq.FlashAddress    = AlignedHead;
      TempReq.Data            = AlignedBuffer;
      TempReq.DataSize        = WriteReq->FlashAddress - AlignedHead;

      //
      // Caution! As TopSwap bit may be toggled, We can't use MMIO read, instead expect SPI read to read use SPI address
      //
      Status = FlashRead(&TempReq);
      if (EFI_ERROR(Status)) {
        ASSERT(!EFI_ERROR(Status));
        goto EXIT;
      }
    }

    //
    // 2. Copy content from Tail alignment area
    //
    if (AlignedTail != WriteReq->FlashAddress + WriteReq->DataSize) {

      //
      // Init a new tempReq for Tail alignment read
      //
      TempReq.FlashRegionType = WriteReq->FlashRegionType;
      TempReq.FlashAddress    = WriteReq->FlashAddress + WriteReq->DataSize;
      TempReq.Data            = AlignedBuffer + WriteReq->FlashAddress + WriteReq->DataSize - AlignedHead;
      TempReq.DataSize        = AlignedTail - WriteReq->FlashAddress - WriteReq->DataSize;

      //
      // Caution! As TopSwap bit may be toggled, We can't use MMIO read, instead expect SPI read to read use SPI address
      //
      Status = FlashRead(&TempReq);
      if (EFI_ERROR(Status)) {
        ASSERT(!EFI_ERROR(Status));
        goto EXIT;
      }
    }

    //
    // 3. Copy orginal data buffer
    //
    CopyMem(AlignedBuffer + WriteReq->FlashAddress - AlignedHead, WriteReq->Data, WriteReq->DataSize);

  }

  //************************************************************************
  // Second Step:
  //    Loop to block update flash. Each time update one flash block
  //
  //***********************************************************************
  CountOfBlocks = (UINTN) (AlignedWriteReq.DataSize / UPDATE_BLOCK_SIZE);
  AlignedWriteReq.DataSize = UPDATE_BLOCK_SIZE;

  for (Index = 0; Index < CountOfBlocks; Index++) {
    if (Progress != NULL) {
      Progress (StartPercentage + ((Index * (EndPercentage - StartPercentage)) / CountOfBlocks));
    }

    Status = InternalSpiFlashUpdateBlock (&AlignedWriteReq);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "SpiFlashUpdate failed (%r) at Region %d - 0x%x\n", Status, AlignedWriteReq.FlashRegionType, AlignedWriteReq.FlashAddress));
      break;
    }

    AlignedWriteReq.FlashAddress += UPDATE_BLOCK_SIZE;
    AlignedWriteReq.Data         += UPDATE_BLOCK_SIZE;
  }

EXIT:

  if (Progress != NULL) {
    Progress (EndPercentage);
  }

  if (AlignedBuffer != NULL) {
    FreePool(AlignedBuffer);
  }

  return Status;
}

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
  )
{
  EFI_STATUS            Status;
  BIOSGUARD_HOB         *BiosGuardHobPtr;
  EFI_PHYSICAL_ADDRESS  BiosGuardMemAddress;
  UINT32                BiosGuardMemSize;
  EFI_PHYSICAL_ADDRESS  BgupCertificate;
  UINT8                 BiosGuardSwSmi;
  UINT8                 *BiosGuardMemAddressPtr;
  UINT8                 *BgupCertificatePtr;
  UINT64                BiosGuardStatus;
  BGUP_HEADER           *BgupHeaderPtr;
  UINTN                 BgupBufferSize;
  BGUPC_HEADER          *BgupCHeaderPtr;
  UINTN                 BgupCBufferSize;

  ASSERT (WriteReq->Data != NULL);
  ASSERT (WriteReq->BgupImage != NULL);
  ASSERT (WriteReq->BgupImageSize != 0);

  if ((WriteReq->BgupImage == NULL) || (WriteReq->BgupImageSize == 0)) {
    DEBUG ((DEBUG_ERROR, "%a: BIOS Bgup file error! Abort BIOS Firmware Update\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  BiosGuardHobPtr         = NULL;
  BiosGuardMemAddressPtr  = NULL;
  BgupCertificatePtr      = NULL;
  BiosGuardMemAddress     = 0;
  BiosGuardMemSize        = 0;
  BiosGuardSwSmi          = 0;
  Status                  = EFI_NOT_STARTED;
  BgupHeaderPtr           = NULL;
  BgupBufferSize          = 0;
  BgupCHeaderPtr          = NULL;
  BgupCBufferSize         = 0;

  //
  // Decompose the BGUP image within capsule.
  //
  Status = DecomposeBgupImage (
    WriteReq->BgupImage,
    WriteReq->BgupImageSize,
    &BgupHeaderPtr,
    &BgupBufferSize,
    &BgupCHeaderPtr,
    &BgupCBufferSize
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to decompose BGUP image. (%r)\n", Status));
    return Status;
  }

  //
  // Locate BIOS Guard NVS
  //
  BiosGuardHobPtr = GetFirstGuidHob (&gBiosGuardHobGuid);
  if (BiosGuardHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "BIOS Guard HOB not available\n"));
    return EFI_DEVICE_ERROR;
  }

  BiosGuardMemAddress    = (EFI_PHYSICAL_ADDRESS) BiosGuardHobPtr->BiosGuardMemAddress;
  BiosGuardMemSize       = (UINT32) LShiftU64 (BiosGuardHobPtr->BiosGuardMemSize, 20);
  BiosGuardSwSmi         = BiosGuardHobPtr->BiosGuardSwSmi;

  BgupCertificate        = (EFI_PHYSICAL_ADDRESS) (BiosGuardMemAddress + BiosGuardMemSize - BGUPC_MEMORY_OFFSET);
  BiosGuardMemAddressPtr = (UINT8 *) BiosGuardMemAddress;
  BgupCertificatePtr     = (UINT8 *) BgupCertificate;

  DEBUG ((DEBUG_INFO, "%a: BIOS Guard SPI Flow\n", __FUNCTION__));

  DEBUG ((DEBUG_INFO, "%a: BgupImageFileSize = 0x%08X\n", __FUNCTION__, WriteReq->BgupImageSize));
  DEBUG ((DEBUG_INFO, "%a: \tBgupHeaderPtr   = 0x%08p\n", __FUNCTION__, BgupHeaderPtr));
  DEBUG ((DEBUG_INFO, "%a: \tBgupBufferSize  = 0x%08X\n", __FUNCTION__, BgupBufferSize));
  DEBUG ((DEBUG_INFO, "%a: \tBgupCHeaderPtr  = 0x%08p\n", __FUNCTION__, BgupCHeaderPtr));
  DEBUG ((DEBUG_INFO, "%a: \tBgupCBufferSize = 0x%08X\n", __FUNCTION__, BgupCBufferSize));
  DEBUG ((DEBUG_INFO, "%a: BiosBinSize       = 0x%08X\n", __FUNCTION__, WriteReq->DataSize));

  CopyMem (BiosGuardMemAddressPtr, BgupHeaderPtr, BgupBufferSize);
  CopyMem ((BiosGuardMemAddressPtr + BgupBufferSize), WriteReq->Data, WriteReq->DataSize);
  CopyMem (BgupCertificatePtr, BgupCHeaderPtr, BgupCBufferSize);

  if (Progress != NULL) {
    Progress (StartPercentage);
  }

  DEBUG ((DEBUG_INFO, "Trigger Bios Guard update via SwSmi (0x%x)\n", BiosGuardSwSmi));
  IoWrite8 (R_PCH_IO_APM_CNT, BiosGuardSwSmi);

  CopyMem (&BiosGuardStatus, BgupCertificatePtr, sizeof (UINT64));

  DEBUG ((DEBUG_INFO, "%a: BIOS GUARD Bios Update Execution Result = 0x%016llX\n", __FUNCTION__, BiosGuardStatus));
  if (Progress != NULL) {
    Progress (EndPercentage);
  }

  if ((BiosGuardStatus & 0xFFFF) != 0) {
    switch (BiosGuardStatus) {
      case BIOSGUARD_ERR_BAD_SVN:
        Status = EFI_SECURITY_VIOLATION;
        break;
      default:
        Status = EFI_DEVICE_ERROR;
        break;
    }
  } else {
    Status = EFI_SUCCESS;
  }

  return Status;
}
