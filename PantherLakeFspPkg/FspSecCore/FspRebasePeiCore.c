/** @file
  This file contains functions to provide PeiCore Rebase function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <Pi/PiFirmwareFile.h>
#include <IndustryStandard/PeImage.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiFirmwareVolume.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FspCommonLib.h>

#define ALIGNMENT 8

STATIC EFI_GUID   mPeiCoreFvGuid = {0x52C05B14, 0x0B98, 0x496c, {0xBC, 0x3B, 0x04, 0xB5, 0x02, 0x11, 0xD6, 0x80}};

/**
  This function returns the next larger size that meets the alignment
  requirement specified.

  @param[in]  Offset     Offset to parse.
  @param[in]  Alignment  The minimum required Alignment offset.

  @retval The offset aligned.

**/
UINT32
AlignPtr (
  IN UINT32 Offset,
  IN UINT8  Alignment
  ) {
  if ((Offset % Alignment) == 0) {
    return Offset;
  } else {
    Offset = (Offset / Alignment) * Alignment + Alignment;
    return Offset;
  }
}

/**
  Expands the 3 byte size commonly used in Firmware Volume data structures.

  @param[in]  Size  Address of the 3 byte array representing the size.

  @retval Fv size.

**/
UINT32
FvBufExpand3ByteSize (
  IN VOID *Size
  ) {
  return (((UINT8 *)Size)[2] << 16) + (((UINT8 *)Size)[1] << 8) + ((UINT8 *)Size)[0];
}

/**
  Get the FFS file size.

  @param[in]  Ffs  Pointer to FFS header.

  @retval FFS file size.

**/
UINT32
FvBufGetFfsFileSize (
  IN EFI_FFS_FILE_HEADER *Ffs
  ) {
  if (Ffs == NULL) {
    return 0;
  }
  if (Ffs->Attributes & FFS_ATTRIB_LARGE_FILE) {
    return (UINT32)((EFI_FFS_FILE_HEADER2 *)Ffs)->ExtendedSize;
  }

  return FvBufExpand3ByteSize (Ffs->Size);
}

/**
  Get the Fv file size.

  @param[in]   Fv                Address of the Fv in memory.
  @param[out]  Size              Size of the Fv in memory.

  @retval EFI_SUCCESS            Get proper Fv size.
  @retval EFI_INVALID_PARAMETER  The Fv size is unhealthy.
  @retval EFI_VOLUME_CORRUPTED   The volume is corrupted.

**/
EFI_STATUS
FvBufGetSize (
  IN VOID   *Fv,
  OUT UINTN *Size
  ) {
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader;
  EFI_FV_BLOCK_MAP_ENTRY     *BlockMap;

  if (Fv == NULL) {
    DEBUG((DEBUG_ERROR, "Fv is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (Size == NULL) {
    DEBUG((DEBUG_ERROR, "Fv size is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  FvHeader   = (EFI_FIRMWARE_VOLUME_HEADER *)Fv;
  BlockMap   = FvHeader->BlockMap;
  *Size      = 0;

  while (BlockMap->Length != 0 || BlockMap->NumBlocks != 0) {
    *Size = *Size + (BlockMap->Length * BlockMap->NumBlocks);
    if (*Size >= BASE_1GB) {
      // If size is greater than 1GB, then assume it is corrupted
      return EFI_INVALID_PARAMETER;
    }
    BlockMap++;
  }

  if (*Size == 0) {
    // If size is 0, then assume the volume is corrupted
    return EFI_VOLUME_CORRUPTED;
  }

  return EFI_SUCCESS;
}

/**
  Get the delta between the FspM flash fv address and FspM fv cache address.
  When FspM fv is real loaded, more fv is added between 4G and FspM fv.
  So the cache address always is smaller than flash fv address.

  @param[in]  FvHandle        Fv address.

  @retval The Fv address delta.

**/
UINTN
GetRebaseDelta (
  IN UINTN FvHandle
  ) {
  UINTN                     Delta;
  UINTN                     OldImageBase;
  UINTN                     Offset;
  FSP_INFO_HEADER           *FspInfoHeader;

  Offset = AlignPtr ((UINTN)((EFI_FIRMWARE_VOLUME_HEADER *)FvHandle)->ExtHeaderOffset +
            sizeof (EFI_FIRMWARE_VOLUME_EXT_HEADER),ALIGNMENT) + sizeof (EFI_FFS_FILE_HEADER) +
            sizeof (EFI_COMMON_SECTION_HEADER);
  FspInfoHeader = (FSP_INFO_HEADER *)(FvHandle + Offset);
  OldImageBase  = FspInfoHeader->ImageBase;

  Delta = OldImageBase - FvHandle;

  return Delta;
}

/**
  Parse Pei Core Te secion in FFS file.
  Te Section layout as follows:
  // |-------------------|----> TeSectionAddress
  // |Te Section Header  |
  // |                   |----> EFI_TE_IMAGE_HEADER
  // |-------------------|
  // |                   |
  // |                   |
  // |                   |
  // |-------------------|----> relocation data block header address
  // |RelocationDataHeader|     = TeSectionAddress + TeHeaderSize - TeHeaderStrippedSize + TeHeader->DataDirectory[0].VirtualAddress
  // |-------------------|----> Relocation Data Block address
  // |Data1|Data2|..     |----> Relocation Data |15--- 11 -------- 0|
  // |-------------------|                      | Type |  Offset    |
  // |RelocationDataBlock|                      +-------------------+
  // |       .           |
  // |       .           |
  // |-------------------|----> Data 1 relocation address
  // |       .           |      = TeSectionAddress + BlockHeaderVirtualAddress + TeHeaderSize - TeHeaderStrippedSize +Data1Offset
  // |-------------------|----> Data 2 relocation address
  // |       .           |      = TeSectionAddress + BlockHeaderVirtualAddress + TeHeaderSize - TeHeaderStrippedSize +Data2Offset
  // |       .           |
  // |-------------------|----> TeSectionEnd
  This function complete address rebase at all data relocation address

  @param[in]     FvHandle    Pei Core Te Section address.
  @param[in]     Delta       Delta between the flash fv address and fv cache address.

  @retval EFI_SUCCESS                  The Te Section is successfully rebased.
  @retval EFI_INVALID_PARAMETER        The operation is aborted.

**/
EFI_STATUS
RebasePeiCoreTESection (
  IN UINTN FvHandle,
  IN UINTN Delta
  ) {
  UINT32                     BaseRelocOffset;
  EFI_TE_IMAGE_HEADER        *TeHeader = (EFI_TE_IMAGE_HEADER *)(FvHandle);
  EFI_IMAGE_BASE_RELOCATION  *BlockHeader;
  UINT32                     RelocateBlockLength;
  UINT32                     RelocateNum;
  VOID                       *RelocateData;
  UINT32                     RelocateOffset;
  UINT32                     RelocateRealOffset;
  UINT32                     *PtrForRebase;
  UINT32                     Index;

  if (TeHeader == NULL) {
    DEBUG((DEBUG_ERROR, "TeHeader is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Calculate the offset of relocation offset data block
  //
  BaseRelocOffset = sizeof (EFI_TE_IMAGE_HEADER) - TeHeader->StrippedSize + TeHeader->DataDirectory[0].VirtualAddress;

  //
  // Read relocation offset data block header
  //
  BlockHeader = (VOID *)(FvHandle + BaseRelocOffset);
  BaseRelocOffset += sizeof (EFI_IMAGE_BASE_RELOCATION);

  //
  // Read relocation offset data block length and nums
  //
  RelocateBlockLength = BlockHeader->SizeOfBlock - sizeof (EFI_IMAGE_BASE_RELOCATION);
  RelocateNum = (UINT32)(RelocateBlockLength / sizeof (UINT16));

  //
  // Read relocation offset data block
  //
  RelocateData = (VOID *)(FvHandle + BaseRelocOffset);

  for (Index = 0; Index < (UINT32)RelocateNum; Index++) {
    //
    // The low 12 bytes of each relocation data in relocation offset data block is relocation offset
    //
    RelocateOffset = *((UINT16 *)((UINTN)RelocateData + Index * sizeof (UINT16))) & 0x0FFF;

    //
    // Do section rebase at relocation address.
    //
    RelocateRealOffset = BlockHeader->VirtualAddress + RelocateOffset;
    RelocateRealOffset += sizeof (EFI_TE_IMAGE_HEADER) - TeHeader->StrippedSize;
    PtrForRebase = (VOID *)(FvHandle + RelocateRealOffset);

    *PtrForRebase = (UINT32)(*(UINT32 *)PtrForRebase - Delta);
  }
  //
  // Rebase Te section base in Te section header
  //
  TeHeader->ImageBase = TeHeader->ImageBase - Delta;

  return EFI_SUCCESS;
}

/**
  Found Pei Core Te section address in Fv file.

  @param[in]    FvHandle     Pointer to Fv.
  @param[out]   TeOffset     Pointer to Section offset.

  @retval EFI_SUCCESS             Successful get Te Section offset.
  @retval EFI_INVALID_PARAMETER   Fail to get Te Section offset

**/
EFI_STATUS
GetPeiCoreTeSectionFromFv (
  IN  UINTN  FvHandle,
  OUT UINT32 *TeOffset
  )
{
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FvHandle;
  UINTN                      FvSize;
  EFI_FFS_FILE_HEADER        *FfsHdr;
  UINT32                     SecOffset;
  EFI_COMMON_SECTION_HEADER  *SecHdr;
  UINT32                     SecSize;
  UINT32                     FfsSize;
  EFI_STATUS                 Status;

  FvSize            = 0;
  SecOffset         = 0;

  Status = FvBufGetSize ((VOID *)FvHandle, &FvSize);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Fail to Get Fv size  %r\n",Status));
    return Status;
  }

  if (FvHeader->Signature != EFI_FVH_SIGNATURE) {
    DEBUG((DEBUG_ERROR, "Fail to Get Fv Signature %r\n",Status));
    return EFI_INVALID_PARAMETER;
  }

  if (TeOffset == NULL) {
    DEBUG((DEBUG_ERROR, "TeOffset is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  *TeOffset = FvHeader->ExtHeaderOffset + sizeof (EFI_FIRMWARE_VOLUME_EXT_HEADER);

  *TeOffset = AlignPtr (*TeOffset, ALIGNMENT);

  while (*TeOffset < (FvSize - sizeof (EFI_FIRMWARE_VOLUME_HEADER))) {
    if (*TeOffset > FvSize) {
      break;
    }

    FfsHdr = (EFI_FFS_FILE_HEADER *)(FvHandle + *TeOffset);
    FfsSize = FvBufGetFfsFileSize (FfsHdr);

    if (CompareGuid (&mPeiCoreFvGuid, &FfsHdr->Name)) {
      *TeOffset += sizeof (EFI_FFS_FILE_HEADER);
      while (SecOffset < FfsSize - sizeof (EFI_FFS_FILE_HEADER)) {
        SecHdr = (EFI_COMMON_SECTION_HEADER *)(FvHandle + *TeOffset);
        //
        // Figure out TE type section
        //
        if (SecHdr->Type == EFI_SECTION_TE) {
          return EFI_SUCCESS;
        } else {
          SecSize = FvBufExpand3ByteSize (SecHdr->Size);
          SecOffset += SecSize;
          *TeOffset += SecSize;
        }
      }
    } else {
      *TeOffset += FfsSize;
      *TeOffset = AlignPtr (*TeOffset, ALIGNMENT);
    }
  }

  return EFI_INVALID_PARAMETER;
}

/**
  Rebase Pei Core Te section in the target Fv.

  @param[in]    FvHandle        Pointer to Fv.

  @retval  EFI_SUCCESS                    Relocation success.
  @retval  EFI_INVALID_PARAMETER          Relocation fail.
  @retval  EFI_NOT_FOUND                  Pei Core is not found from target FV.

**/
EFI_STATUS
EFIAPI
RebasePeiCoreFfs (
  IN UINTN FvHandle
  )
{
  EFI_STATUS          Status;
  UINT32              Offset;
  UINTN               Delta;
  UINTN               TeSectionAddress;

  //
  // Locate Pei Core Te section address
  //
  Offset = 0;
  Status = GetPeiCoreTeSectionFromFv (FvHandle, &Offset);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Fail to find PeiMain FFS %r\n", Status));
    return EFI_NOT_FOUND;
  }

  //
  // Rebase Pei Core
  //
  Delta = GetRebaseDelta (FvHandle);
  DEBUG((DEBUG_INFO, "Delta: 0x%x\n", Delta));

  TeSectionAddress = FvHandle + Offset + sizeof (EFI_COMMON_SECTION_HEADER);
  DEBUG((DEBUG_INFO, "TeSectionAddress: 0x%x\n", TeSectionAddress));

  return RebasePeiCoreTESection (TeSectionAddress, Delta);
}
