/** @file
  Extract image from FV

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
#include <Library/MemoryAllocationLib.h>
#include <Library/FvImageExtractionLib.h>

#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/FirmwareVolume2.h>

//
// Use to indicate a dummy file
//
#define SYSTEM_FMP_DUMMY_FILE_SIGNATURE  SIGNATURE_32('$', 'D', 'U', 'M')

/**
  Check if a block of buffer is erased.

  @param[in] ErasePolarity  Erase polarity attribute of the firmware volume
  @param[in] InBuffer       The buffer to be checked
  @param[in] BufferSize     Size of the buffer in bytes

  @retval    TRUE           The block of buffer is erased
  @retval    FALSE          The block of buffer is not erased
**/
BOOLEAN
IsBufferErased (
  IN UINT8    ErasePolarity,
  IN VOID     *InBuffer,
  IN UINTN    BufferSize
  )
{
  UINTN   Count;
  UINT8   EraseByte;
  UINT8   *Buffer;

  if(ErasePolarity == 1) {
    EraseByte = 0xFF;
  } else {
    EraseByte = 0;
  }

  Buffer = InBuffer;
  for (Count = 0; Count < BufferSize; Count++) {
    if (Buffer[Count] != EraseByte) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Get FFS buffer pointer by FileName GUID and FileType.

  @param[in]   FdStart          The System Firmware FD image
  @param[in]   FdSize           The size of System Firmware FD image
  @param[in]   FileName         The FileName GUID of FFS to be found
  @param[in]   Type             The FileType of FFS to be found
  @param[out]  OutFfsBuffer     The FFS buffer found, including FFS_FILE_HEADER
  @param[out]  OutFfsBufferSize The size of FFS buffer found, including FFS_FILE_HEADER

  @retval TRUE  The FFS buffer is found.
  @retval FALSE The FFS buffer is not found.

**/
BOOLEAN
GetFfsByName (
  IN VOID                  *FdStart,
  IN UINTN                 FdSize,
  IN EFI_GUID              *FileName,
  IN EFI_FV_FILETYPE       Type,
  OUT VOID                 **OutFfsBuffer,
  OUT UINTN                *OutFfsBufferSize
  )
{
  UINTN                                     FvSize;
  EFI_FIRMWARE_VOLUME_HEADER                *FvHeader;
  EFI_FIRMWARE_VOLUME_EXT_HEADER            *FvExtHeader;
  EFI_FFS_FILE_HEADER                       *FfsHeader;
  UINT32                                    FfsSize;
  UINTN                                     TestLength;
  BOOLEAN                                   FvFound;

  DEBUG ((DEBUG_INFO, "GetFfsByName - FV: 0x%08x - 0x%08x\n", (UINTN)FdStart, (UINTN)FdSize));

  FvFound = FALSE;
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FdStart;
  while ((UINTN)FvHeader < (UINTN)FdStart + FdSize - 1) {
    FvSize = (UINTN)FdStart + FdSize - (UINTN)FvHeader;

    if (FvHeader->Signature != EFI_FVH_SIGNATURE) {
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)((UINTN)FvHeader + SIZE_4KB);
      continue;
    }
    DEBUG ((DEBUG_INFO, "checking FV....0x%08x - 0x%x\n", FvHeader, FvHeader->FvLength));
    FvFound = TRUE;
    if (FvHeader->FvLength > FvSize) {
      DEBUG ((DEBUG_INFO, "GetFfsByName - FvSize: 0x%08x, MaxSize - 0x%08x\n", (UINTN)FvHeader->FvLength, (UINTN)FvSize));
      return FALSE;
    }
    FvSize = (UINTN)FvHeader->FvLength;

    //
    // Find FFS
    //
    if (FvHeader->ExtHeaderOffset != 0) {
      FvExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *)((UINT8 *)FvHeader + FvHeader->ExtHeaderOffset);
      FfsHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *)FvExtHeader + FvExtHeader->ExtHeaderSize);
    } else {
      FfsHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *)FvHeader + FvHeader->HeaderLength);
    }
    FfsHeader = (EFI_FFS_FILE_HEADER *)((UINTN)FvHeader + ALIGN_VALUE((UINTN)FfsHeader - (UINTN)FvHeader, 8));

    while ((UINTN)FfsHeader < (UINTN)FvHeader + FvSize - 1) {
      DEBUG ((DEBUG_INFO, "GetFfsByName - FFS: 0x%08x\n", FfsHeader));
      TestLength = (UINTN)((UINTN)FvHeader + FvSize - (UINTN)FfsHeader);
      if (TestLength > sizeof(EFI_FFS_FILE_HEADER)) {
        TestLength = sizeof(EFI_FFS_FILE_HEADER);
      }
      if (IsBufferErased (1, FfsHeader, TestLength)) {
        break;
      }

      if (IS_FFS_FILE2(FfsHeader)) {
        FfsSize = FFS_FILE2_SIZE (FfsHeader);
      } else {
        FfsSize = FFS_FILE_SIZE (FfsHeader);
      }

      if (CompareGuid (FileName, &FfsHeader->Name) &&
          ((Type == EFI_FV_FILETYPE_ALL) || (FfsHeader->Type == Type))) {
        //
        // Check section
        //
        *OutFfsBuffer = FfsHeader;
        *OutFfsBufferSize = FfsSize;
        return TRUE;
      } else {
        //
        // Any other type is not allowed
        //
        DEBUG ((DEBUG_INFO, "GetFfsByName - other FFS type 0x%x, name %g\n", FfsHeader->Type, &FfsHeader->Name));
      }

      //
      // Next File
      //
      FfsHeader = (EFI_FFS_FILE_HEADER *)((UINTN)FfsHeader + ALIGN_VALUE(FfsSize, 8));
    }

    //
    // Next FV
    //
    FvHeader = (VOID *)(UINTN)((UINTN)FvHeader + FvHeader->FvLength);
    DEBUG ((DEBUG_INFO, "Next FV....0x%08x - 0x%x\n", FvHeader, FvHeader->FvLength));
  }

  if (!FvFound) {
    DEBUG ((DEBUG_ERROR, "GetFfsByName - NO FV Found\n"));
  }
  return FALSE;
}

/**
  Extract the image raw data within input FvImage by FileName GUID.

  @param[in]   FvImage                 Pointer to a buffer containing FVs
  @param[in]   FvImageSize             The size of FvImage buffer in bytes.
  @param[in]   FileName                The FileName GUID of FFS to be found
  @param[out]  RawDataImage            The image pointer to requested raw data.
  @param[out]  RawDataImageSize        The size of RawDataImage.

  @retval TRUE  The image is extracted.
  @retval FALSE The image is not extracted.
**/
BOOLEAN
ExtractImageByName (
  IN VOID                  *FvImage,
  IN UINTN                 FvImageSize,
  IN EFI_GUID              *FileName,
  OUT VOID                 **RawDataImage,
  OUT UINTN                *RawDataImageSize
  )
{
  BOOLEAN     Result;
  UINT32      FileHeaderSize;

  if ((FvImage == NULL) || (RawDataImage == NULL) || (RawDataImageSize == NULL)) {
    DEBUG ((DEBUG_ERROR, "ExtractImageByName: Invalid arguments\n"));
    ASSERT (FALSE);
    return FALSE;
  }

  *RawDataImage = NULL;
  *RawDataImageSize = 0;

  Result = GetFfsByName (FvImage, FvImageSize, FileName, EFI_FV_FILETYPE_RAW, RawDataImage, RawDataImageSize);
  if (!Result) {
    DEBUG ((DEBUG_INFO, "Extract image %g failed\n", FileName));
    return FALSE;
  }

  if (IS_FFS_FILE2 (*RawDataImage)) {
    FileHeaderSize = sizeof (EFI_FFS_FILE_HEADER2);
  } else {
    FileHeaderSize = sizeof (EFI_FFS_FILE_HEADER);
  }

  *RawDataImage = (UINT8 *)*RawDataImage + FileHeaderSize;
  *RawDataImageSize = *RawDataImageSize - FileHeaderSize;

  if (*((UINT32 *)(*RawDataImage)) == SYSTEM_FMP_DUMMY_FILE_SIGNATURE) {
    DEBUG ((DEBUG_INFO, "Dummy image (%g)\n", FileName));
    Result = FALSE;
    *RawDataImage = NULL;
    *RawDataImageSize = 0;
  }

  return Result;
}
