/** @file
  FSPM Extract Guided Section Peim Implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <PiPei.h>
#include <Library/SecExtractGuidedSectionLib.h>
#include <Library/FspFbmSupportLib.h>
#include <Guid/FspHeaderFile.h>
#include <Library/DebugLib.h>

/**
  Find FSP header pointer.

  @param[in] FlashFvFspBase Flash address of FSP FV.

  @return FSP header pointer.
**/
EFI_COMMON_SECTION_HEADER *
EFIAPI
FindFvSection (
  IN EFI_PHYSICAL_ADDRESS  FlashFvFspBase
  )
{
  UINT8  *CheckPointer;

  // FV section will be found at FV_HEADER Length + FFS_HEADER_LENGTH

  CheckPointer = (UINT8 *)(UINTN) FlashFvFspBase;
  if (((EFI_FIRMWARE_VOLUME_HEADER *)CheckPointer)->ExtHeaderOffset != 0) {
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_HEADER *)CheckPointer)->ExtHeaderOffset;
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_EXT_HEADER *)CheckPointer)->ExtHeaderSize;
    CheckPointer = (UINT8 *)ALIGN_POINTER (CheckPointer, 8);
  } else {
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_HEADER *)CheckPointer)->HeaderLength;
  }

  CheckPointer = CheckPointer + sizeof (EFI_FFS_FILE_HEADER);
  return (EFI_COMMON_SECTION_HEADER *)CheckPointer;
}

/**
  Rebase FSP-M image base address in FSP-M headr

**/
VOID
RebaseFspmImageBase (
  VOID
)
{
  FSP_INFO_HEADER            *FspHeader;
  UINT32                     FspmBaseAddress;
  EFI_COMMON_SECTION_HEADER  *Section = NULL;

  FspmBaseAddress = PcdGet32 (PcdSecondaryDataStackBase) + SIZE_4KB;
  Section = (EFI_COMMON_SECTION_HEADER *) FindFvSection (FspmBaseAddress);
  FspHeader = (FSP_INFO_HEADER *) ((UINTN) Section + sizeof (EFI_RAW_SECTION));
  FspHeader->ImageBase = FspmBaseAddress;
  return;
}

/**
  The Entry point of the FSP Wrapper Extract Guided Section

  @param[in]  FspmBaseAddress   Base address of compressed FSP-M

  @retval  RETURN_SUCCESS Decompression completed successfully
  @retval  RETURN_INVALID_PARAMETER
                          Not able to find FSPM FV.

**/
EFI_STATUS
EFIAPI
ExtractFspm (
  IN UINT32                  FspmBaseAddress
  )
{
  EFI_COMMON_SECTION_HEADER  *Section = NULL;
  EFI_STATUS                 Status;
  VOID                       *OutputBuffer = NULL;
  UINT32                     RequiredBufferSize;
  UINT8                      *ScratchBuffer = NULL;
  UINT32                     AuthenticationStatus;
  UINT32                     ScratchBufferSize;
  UINT32                     OutputBufferSize;
  UINT16                     SectionAttribute;

  Section = (EFI_COMMON_SECTION_HEADER *) FindFvSection (FspmBaseAddress);

  if (Section->Type != EFI_SECTION_GUID_DEFINED) {
    DEBUG ((DEBUG_INFO, "Exit ExtractFspm - Not Guided Section 0x%x\n", Section->Type));
    return EFI_INVALID_PARAMETER;
  }

  // 0x10 gets added as decompression header. Need to align with 4K boundary.
  OutputBuffer = (UINTN *) (UINTN)(PcdGet32 (PcdSecondaryDataStackBase) + SIZE_4KB - 0x10);

  Status = GuidedSectionGetInfo (
                                 Section,
                                 &OutputBufferSize,
                                 &ScratchBufferSize,
                                 &SectionAttribute
                                 );

  RequiredBufferSize = OutputBufferSize + ScratchBufferSize + SIZE_4KB;
  if (RequiredBufferSize > (PcdGet32 (PcdSecondaryDataStackSize))) {
    DEBUG ((DEBUG_INFO, "FSP-M binary size is larger than the allocated buffer!\n"));
    return EFI_BUFFER_TOO_SMALL;
  }

  ScratchBuffer = ((UINT8 *) OutputBuffer) + (SIZE_4KB * EFI_SIZE_TO_PAGES (OutputBufferSize));
  Status = GuidedSectionExtraction (
                                    Section,
                                    &OutputBuffer,
                                    ScratchBuffer,
                                    &AuthenticationStatus
                                    );

  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "FSP-M was successfully decompressed at 0x%x\n",
            PcdGet32 (PcdSecondaryDataStackBase) + SIZE_4KB
            ));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "FSP-M decompression fail\n"));
  return Status;
  }
