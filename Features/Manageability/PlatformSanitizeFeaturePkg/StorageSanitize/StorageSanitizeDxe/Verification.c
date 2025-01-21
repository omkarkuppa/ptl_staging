/**@file
  Verification for Storage erase driver

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

#include "StorageErase.h"

/**
  Read the Lba, verify it or backup for later verification.

  @param[in]  BlkIo       Point to the BlockIo protocol.
  @param[in]  Lba         The starting Logical Block Address to read from
  @param[in]  TotalBlocks total blocks.
  @param[in]  Size        Block size of Buffer.
  @param[in]  Buffer      A pointer to the destination buffer for the data. The caller is
                          responsible for either having implicit or explicit ownership of the buffer.
  @param[in]  Verify      Verify the buffer or backup data for later verification.

  @retval EFI_SUCCESS           The data was read correctly from the device.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing the read.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId does not matched the current device.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.
  @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not valid,
                                or the buffer is not on proper alignment.
**/
BOOLEAN
ReadLba (
  IN EFI_BLOCK_IO_PROTOCOL *BlkIo,
  IN EFI_LBA               Lba,
  IN UINT32                TotalBlocks,
  IN UINT32                Size,
  IN OUT VOID              *Buffer,
  IN BOOLEAN               Verify
  )
{
  EFI_STATUS  Status;
  UINT16      RandomLba;
  UINT32      VerifyBlock;
  VOID        *TempBuffer;
  UINTN       Page;

  if ((BlkIo == NULL) || (Buffer == NULL) || (Size == 0)) {
    return FALSE;
  }

  if (TotalBlocks > 1) {
    //
    // Select the random location
    //
    GetRandomNumber16(&RandomLba);
    VerifyBlock = (UINT32)((TotalBlocks - 1) * RandomLba / 0xFFFF);
    if ((VerifyBlock + Size) > TotalBlocks) {
      Size = (UINT32)(TotalBlocks - VerifyBlock);
    }
    Lba = Lba + VerifyBlock;
  }

  if (Verify) {
    TempBuffer = Buffer;
  } else {
    Page = EFI_SIZE_TO_PAGES (Size * BlkIo->Media->BlockSize);
    //
    // Check if there is enough memory for read block
    // Double size of memory for read and memory map
    //
    TempBuffer = AllocatePages (Page * 2);
    if (TempBuffer == NULL) {
      return FALSE;
    } else {
      FreePages (TempBuffer, Page * 2);
      TempBuffer = AllocatePages (Page);
    }
  }
  if (TempBuffer == NULL) {
    return FALSE;
  }

  Status = BlkIo->ReadBlocks (
                  BlkIo,
                  BlkIo->Media->MediaId,
                  Lba,
                  Size * BlkIo->Media->BlockSize,
                  TempBuffer
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  if (Verify) {
    //
    // Verify if it becomes to zero
    //
    return IsZeroBuffer (Buffer, (Size * BlkIo->Media->BlockSize));
  } else {
    //
    // Backup data for late verify
    //
    ((VERIFY_DATA*)Buffer)->Lba  = Lba;
    ((VERIFY_DATA*)Buffer)->Size = Size;
    ((VERIFY_DATA*)Buffer)->Data = TempBuffer;
    return TRUE;
  }
}

/**
  Verify the device if the data is zero or backup the LBA for later verification.

  @param[in]  BlkIo       Point to the BlockIo protocol.
  @param[in]  Verify      Verify or backup for late verification.
  @param[OUT] VerifyInfo  Point to backup Lba for late verification.

  @retval TRUE            All verified data is zero.
  @retval FALSE           not zero.
**/
BOOLEAN
VerifyDevice (
  IN EFI_BLOCK_IO_PROTOCOL *BlkIo,
  IN BOOLEAN               Verify,
  OUT VERIFY_INFO          *VerifyInfo
  )
{
  UINT64      Size;
  VERIFY_DATA *Buffer;
  EFI_LBA     Lba;
  UINT32      NumberOfBlocks;
  UINTN       Blocks;
  UINTN       Index;
  UINT32      VerifySize;
  BOOLEAN     VerifyFlag;
  UINTN       Page;

  Lba        = 0;
  Blocks     = VERIFY_BLOCK_NUMBER;
  VerifyFlag = FALSE;
  Buffer     = NULL;

  if (BlkIo == NULL) {
    return VerifyFlag;
  }

  DEBUG ((DEBUG_INFO, "%a: Block: 0x%X, BlockSize: 0x%X\n", __FUNCTION__, BlkIo->Media->LastBlock + 1, BlkIo->Media->BlockSize));

  //
  // No need verify when there is no media
  //
  if ((BlkIo->Media->BlockSize == 0) || (BlkIo->Media->LastBlock == 0)) {
    return TRUE;
  }

  //
  // Split the space to 1024 piece, make sure each piece small than 4GB
  // if it larger than 4GB, set size of each piece to 4GB, extend the pieces
  //
  Size = (BlkIo->Media->LastBlock + 1) * BlkIo->Media->BlockSize;
  if (Size == 0) {
    return TRUE;
  }

  if (Size >= (MAX_BLOCK_SIZE * Blocks)) {
    NumberOfBlocks = (UINT32)(MAX_BLOCK_SIZE / BlkIo->Media->BlockSize);
    Blocks = (BlkIo->Media->LastBlock + 1) / NumberOfBlocks;
  } else {
    NumberOfBlocks = (UINT32)((BlkIo->Media->LastBlock + 1) / Blocks);
  }

  //
  // Each time verify the 5% of blocks. Make sure the size is 4KB align to reduce memory map
  //
  VerifySize = NumberOfBlocks / 20;
  Page = EFI_SIZE_TO_PAGES (VerifySize * BlkIo->Media->BlockSize);
  VerifySize = (UINT32)(EFI_PAGES_TO_SIZE (Page) / BlkIo->Media->BlockSize);

  if (Verify) {
    Buffer = AllocatePages (Page);
    if (Buffer == NULL) {
      return VerifyFlag;
    }
  } else {
    if (VerifyInfo == NULL) {
      return VerifyFlag;
    }
    //
    // Backup two locations of each blocks
    //
    VerifyInfo->Length = (Blocks * 2 + 4);
    Buffer = (VERIFY_DATA*) AllocateZeroPool (VerifyInfo->Length * sizeof (VERIFY_DATA));
    if (Buffer == NULL) {
      VerifyInfo->Length = 0;
      return VerifyFlag;
    }
    VerifyInfo->BackupLba = Buffer;
  }

  //
  // Verify or backup Lba 0, 1, 2
  //
  for (Index = 0; Index <= 2; Index ++) {
    VerifyFlag = ReadLba (BlkIo, Index, 0, 1, Buffer, Verify);
    if (!VerifyFlag) {
      return VerifyFlag;
    } else if (!Verify) {
      Buffer ++;
    }
  }

  //
  // Verify or backup last Lba
  //
  VerifyFlag = ReadLba (BlkIo, BlkIo->Media->LastBlock, 0, 1, Buffer, Verify);
  if (!VerifyFlag) {
    return VerifyFlag;
  } else if (!Verify) {
    Buffer ++;
  }

  //
  // Verify or backup two 5% data of each blocks
  //
  Lba = 3;
  for (Index = 0; Index < Blocks; Index ++) {
    if (Lba + NumberOfBlocks > BlkIo->Media->LastBlock) {
      NumberOfBlocks = (UINT32)(BlkIo->Media->LastBlock + 1 - Lba);
    }
    VerifyFlag = ReadLba (BlkIo, Lba, NumberOfBlocks, VerifySize, Buffer, Verify);
    if (VerifyFlag && !Verify) {
      Buffer ++;
    }

    if (VerifyFlag) {
      VerifyFlag = ReadLba (BlkIo, Lba, NumberOfBlocks, VerifySize, Buffer, Verify);
      if (VerifyFlag && !Verify) {
        Buffer ++;
      }
    }
    Lba += NumberOfBlocks;
    if (!VerifyFlag || (Buffer == NULL) || (Lba >= BlkIo->Media->LastBlock)) {
      break;
    }
  }
  DEBUG ((DEBUG_INFO, "%a: verifyLba %d, number:%d, Lba 0x%X, VerifySize:0x%X\n", __FUNCTION__, VerifyFlag, Index, Lba, VerifySize));

  if (Verify) {
    FreePages (Buffer, Page);
    Buffer = NULL;
  }
  return VerifyFlag;
}

/**
  Verify all erased devcie, check if all data is erased.

**/
VOID
VerifyEraseResult (
  PLATFORM_ERASE_DEVICE  *Dev
  )
{
  EFI_STATUS             Status;
  EFI_BLOCK_IO_PROTOCOL  *BlkIo;
  BOOLEAN                Result;

  BlkIo = NULL;
  if ((Dev->Handle != NULL) && (Dev->Result == ERASE_RESULT_SUCCESS)) {
    Status = gBS->HandleProtocol (Dev->Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
    if (!EFI_ERROR (Status)) {
      Result = VerifyDevice (BlkIo, TRUE, NULL);
      if (!Result) {
        Dev->Result |= ERASE_RESULT_VERIFICATION_FAILED;
      }
      DEBUG ((DEBUG_INFO, "%a: verify %a Result %d\n", __FUNCTION__, Dev->EraseInfo.ModelName, Dev->Result));
    }
  }
}

/**
  Check if it is the specific device, the data doesn't clear to zero.

  @param[in] Dev       Point to the platform erase list.

  @retval TRUE         it is the specific device.
  @retval FALSE        It is not the specific device.
**/
BOOLEAN
VerifySpecificDevice (
  IN PLATFORM_ERASE_DEVICE  *Dev
  )
{
  CHAR16 ModelName[MN_MAX_LEN + 1];

  if (Dev != NULL && (Dev->DeviceType == SATA)) {
    ZeroMem (ModelName, sizeof (ModelName));
    UnicodeSPrintAsciiFormat(ModelName, MN_MAX_LEN, "%a", Dev->EraseInfo.ModelName);
    if (StrStr (ModelName, L"INTEL") != NULL)
      return TRUE;
  } else if (Dev != NULL && (Dev->DeviceType == UFS)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Backup the Lba for late verification.

  @param[in] VerifyInfo  Point to the structure contains the backup Lba.
**/
VOID
BackupLba (
  IN OUT VERIFY_INFO *VerifyInfo
  )
{
  EFI_STATUS             Status;
  EFI_BLOCK_IO_PROTOCOL  *BlkIo;

  if ((VerifyInfo != NULL) && (VerifyInfo->Dev != NULL) && (VerifyInfo->Dev->Handle != NULL)) {
    Status = gBS->HandleProtocol (VerifyInfo->Dev->Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
    if (!EFI_ERROR (Status)) {
      VerifyDevice (BlkIo, FALSE, VerifyInfo);
    }
  }
}

/**
  verify the Backup the Lba if it is erased and reallocate.

  @param[in] VerifyInfo  Point to the structure contains the backup Lba.
**/
VOID
VerifyBackupLba (
  IN VERIFY_INFO *VerifyInfo
  )
{
  EFI_STATUS             Status;
  EFI_BLOCK_IO_PROTOCOL  *BlkIo;
  VERIFY_DATA            *backupBuffer;
  UINT64                 Number;
  EFI_LBA                Lba;
  VOID                   *TempBuffer;
  UINTN                  Size;
  BOOLEAN                Result;
  UINTN                  Page;

  if ((VerifyInfo == NULL) || (VerifyInfo->Length == 0) || (VerifyInfo->BackupLba == NULL)) {
    return;
  }

  Result     = TRUE;
  TempBuffer = NULL;

  if ((VerifyInfo->Dev != NULL) && (VerifyInfo->Dev->Handle != NULL)) {
    DEBUG ((DEBUG_INFO, "%a: Dev: %p\n", __FUNCTION__, VerifyInfo->Dev));
    Status = gBS->HandleProtocol (VerifyInfo->Dev->Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
    if (!EFI_ERROR (Status)) {
      Number       = 0;
      Status       = EFI_SUCCESS;
      backupBuffer = VerifyInfo->BackupLba;
      Size         = 0;
      while ((backupBuffer->Data != NULL) && (Number < VerifyInfo->Length)) {
        Size = (UINTN)backupBuffer->Size * BlkIo->Media->BlockSize;
        Lba  = backupBuffer->Lba;
        Page = EFI_SIZE_TO_PAGES (Size);
        //
        // Verify the device only the erase successful
        //
        if (VerifyInfo->Dev->Result != ERASE_RESULT_SUCCESS) {
          Result = FALSE;
        } else {
          TempBuffer = AllocatePages (Page);
          if (TempBuffer == NULL) {
            Result = FALSE;
          }
        }

        if (Result && (TempBuffer != NULL)) {
          Status = BlkIo->ReadBlocks (
                            BlkIo,
                            BlkIo->Media->MediaId,
                            Lba,
                            Size,
                            TempBuffer
                            );
          if (EFI_ERROR (Status)) {
            Result = FALSE;
          } else {
            if ((CompareMem (backupBuffer->Data, TempBuffer, Size) == 0)
               && (!IsZeroBuffer (TempBuffer, BlkIo->Media->BlockSize))) {
              VerifyInfo->Dev->Result |= ERASE_RESULT_VERIFICATION_FAILED;
              Result = FALSE;
            }
          }
        }
        FreePages (backupBuffer->Data, Page);
        backupBuffer->Data = NULL;
        backupBuffer++;
        Number++;
        if (TempBuffer) {
          FreePages (TempBuffer, Page);
          TempBuffer = NULL;
        }
      }
      DEBUG ((DEBUG_INFO, "%a: VerifyLba result %d, number: %d, Verify Buffer:%p\n",
        __FUNCTION__, VerifyInfo->Dev->Result, Number, VerifyInfo->BackupLba));
      //
      // Free the backup buffer
      //
      if (VerifyInfo->BackupLba) {
        FreePool (VerifyInfo->BackupLba);
        VerifyInfo->BackupLba = NULL;
      }
    }
  }
}
