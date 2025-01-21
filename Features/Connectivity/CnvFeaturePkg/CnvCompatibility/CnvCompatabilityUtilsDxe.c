/** @file
  This is the Uefi driver that initializes the Intel PCH.

#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.

@par Specification Reference:
**/
#include "CnvCompatibilityDxe.h"

/**
  Read data from Firmware Block by FVB protocol Read.
  The data may cross the multi block ranges.

  @param  Fvb                   The FW_VOL_BLOCK_PROTOCOL instance from which to read data.
  @param  StartLba              Pointer to StartLba.
                                On input, the start logical block index from which to read.
                                On output,the end logical block index after reading.
  @param  Offset                Pointer to Offset
                                On input, offset into the block at which to begin reading.
                                On output, offset into the end block after reading.
  @param  DataSize              Size of data to be read.
  @param  Data                  Pointer to Buffer that the data will be read into.

  @retval EFI_SUCCESS           Successfully read data from firmware block.
  @retval others
**/
EFI_STATUS
CnvReadFvbData (
  IN     EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb,
  IN OUT EFI_LBA                             *StartLba,
  IN OUT UINTN                               *Offset,
  IN     UINTN                               DataSize,
  OUT    UINT8                               *Data
  )
{
  UINTN       BlockSize;
  UINTN       NumberOfBlocks;
  UINTN       BlockIndex;
  UINTN       ReadDataSize;
  EFI_STATUS  Status;

  //
  // Try read data in current block
  //
  BlockIndex   = 0;
  ReadDataSize = DataSize;
  Status       = Fvb->Read (Fvb, *StartLba, *Offset, &ReadDataSize, Data);
  if (Status == EFI_SUCCESS) {
    *Offset += DataSize;
    return EFI_SUCCESS;
  } else if (Status != EFI_BAD_BUFFER_SIZE) {
    //
    // other error will direct return
    //
    return Status;
  }

  //
  // Data crosses the blocks, read data from next block
  //
  DataSize -= ReadDataSize;
  Data     += ReadDataSize;
  *StartLba = *StartLba + 1;
  while (DataSize > 0) {
    Status = Fvb->GetBlockSize (Fvb, *StartLba, &BlockSize, &NumberOfBlocks);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // Read data from the crossing blocks
    //
    BlockIndex = 0;
    while (BlockIndex < NumberOfBlocks && DataSize >= BlockSize) {
      Status = Fvb->Read (Fvb, *StartLba + BlockIndex, 0, &BlockSize, Data);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      Data     += BlockSize;
      DataSize -= BlockSize;
      BlockIndex++;
    }

    //
    // Data doesn't exceed the current block range.
    //
    if (DataSize < BlockSize) {
      break;
    }

    //
    // Data must be got from the next block range.
    //
    *StartLba += NumberOfBlocks;
  }

  //
  // read the remaining data
  //
  if (DataSize > 0) {
    Status = Fvb->Read (Fvb, *StartLba + BlockIndex, 0, &DataSize, Data);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // Update Lba and Offset used by the following read.
  //
  *StartLba += BlockIndex;
  *Offset    = DataSize;

  return EFI_SUCCESS;
}

/**
  Given the supplied FW_VOL_BLOCK_PROTOCOL, allocate a buffer for output and
  copy the real length volume header into it.

  @param  Fvb                   The FW_VOL_BLOCK_PROTOCOL instance from which to
                                read the volume header
  @param  FwVolHeader           Pointer to pointer to allocated buffer in which
                                the volume header is returned.

  @retval EFI_OUT_OF_RESOURCES  No enough buffer could be allocated.
  @retval EFI_SUCCESS           Successfully read volume header to the allocated
                                buffer.
  @retval EFI_INVALID_PARAMETER The FV Header signature is not as expected or
                                the file system could not be understood.

**/
EFI_STATUS
CnvGetFwVolHeader (
  IN     EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb,
  OUT    EFI_FIRMWARE_VOLUME_HEADER          **FwVolHeader
  )
{
  EFI_STATUS                  Status;
  EFI_FIRMWARE_VOLUME_HEADER  TempFvh;
  UINTN                       FvhLength;
  EFI_LBA                     StartLba;
  UINTN                       Offset;
  UINT8                       *Buffer;

  //
  // Read the standard FV header
  //
  StartLba  = 0;
  Offset    = 0;
  FvhLength = sizeof (EFI_FIRMWARE_VOLUME_HEADER);
  Status    = CnvReadFvbData (Fvb, &StartLba, &Offset, FvhLength, (UINT8 *)&TempFvh);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Validate FV Header signature, if not as expected, continue.
  //
  if (TempFvh.Signature != EFI_FVH_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check to see that the file system is indeed formatted in a way we can
  // understand it...
  //
  if ((!CompareGuid (&TempFvh.FileSystemGuid, &gEfiFirmwareFileSystem2Guid)) &&
      (!CompareGuid (&TempFvh.FileSystemGuid, &gEfiFirmwareFileSystem3Guid)))
  {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Allocate a buffer for the caller
  //
  *FwVolHeader = AllocatePool (TempFvh.HeaderLength);
  if (*FwVolHeader == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Copy the standard header into the buffer
  //
  CopyMem (*FwVolHeader, &TempFvh, sizeof (EFI_FIRMWARE_VOLUME_HEADER));

  //
  // Read the rest of the header
  //
  FvhLength = TempFvh.HeaderLength - sizeof (EFI_FIRMWARE_VOLUME_HEADER);
  Buffer    = (UINT8 *)*FwVolHeader + sizeof (EFI_FIRMWARE_VOLUME_HEADER);
  Status    = CnvReadFvbData (Fvb, &StartLba, &Offset, FvhLength, Buffer);
  if (EFI_ERROR (Status)) {
    //
    // Read failed so free buffer
    //
    FreePool (*FwVolHeader);
  }

  return Status;
}

/**
  Verify checksum of the firmware volume header.

  @param  FvHeader       Points to the firmware volume header to be checked

  @retval TRUE           Checksum verification passed
  @retval FALSE          Checksum verification failed

**/
BOOLEAN
CnvVerifyFvHeaderChecksum (
  IN EFI_FIRMWARE_VOLUME_HEADER  *FvHeader
  )
{
  UINT16  Checksum;

  Checksum = CalculateSum16 ((UINT16 *)FvHeader, FvHeader->HeaderLength);

  if (Checksum == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}
