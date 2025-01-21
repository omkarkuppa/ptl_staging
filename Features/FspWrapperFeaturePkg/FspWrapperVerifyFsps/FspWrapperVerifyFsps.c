/** @file
  FSP Wrapper extract and verify FSP-S entry PEIM implementation.

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
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>
#include <Ppi/GuidedSectionExtraction.h>
#include <Library/HobLib.h>
#include <Library/PeiPlatformFspsVerificationLib.h>

#define FSP_HEADER_IMAGEBASE_OFFSET  0x1C

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
  Verify the Guided Section GUID by checking if there is the Guided Section GUID HOB recorded the GUID itself.

  @param GuidedSectionGuid          The Guided Section GUID.
  @param GuidedSectionExtraction    A pointer to the pointer to the supported Guided Section Extraction Ppi
                                    for the Guided Section.

  @return TRUE      The GuidedSectionGuid could be identified, and the pointer to
                    the Guided Section Extraction Ppi will be returned to *GuidedSectionExtraction.
  @return FALSE     The GuidedSectionGuid could not be identified, or
                    the Guided Section Extraction Ppi has not been installed yet.

**/
BOOLEAN
VerifyGuidedSectionGuid (
  IN  EFI_GUID                                  *GuidedSectionGuid,
  OUT EFI_PEI_GUIDED_SECTION_EXTRACTION_PPI     **GuidedSectionExtraction
  )
{
  EFI_PEI_HOB_POINTERS  Hob;
  EFI_GUID              *GuidRecorded;
  VOID                  *Interface;
  EFI_STATUS            Status;

  //
  // Check if there is the Guided Section GUID HOB recorded the GUID itself.
  //
  Hob.Raw = GetFirstGuidHob (GuidedSectionGuid);
  if (Hob.Raw != NULL) {
    GuidRecorded = (EFI_GUID *) GET_GUID_HOB_DATA (Hob);
    if (CompareGuid (GuidRecorded, GuidedSectionGuid)) {
      //
      // Found the recorded GuidedSectionGuid.
      //
      Status = PeiServicesLocatePpi (GuidedSectionGuid, 0, NULL, (VOID **) &Interface);
      if (!EFI_ERROR (Status) && Interface != NULL) {
        //
        // Found the supported Guided Section Extraction Ppi for the Guided Section.
        //
        *GuidedSectionExtraction = (EFI_PEI_GUIDED_SECTION_EXTRACTION_PPI *) Interface;
        return TRUE;
      }
      return FALSE;
    }
  }

  return FALSE;
}


/**
  The Entry point of the FSP Wrapper Extract Guided Section

  @param[in]  FileHandle   Pointer to image file handle
  @param[in]  PeiServices  Pointer to PEI Services Table

  @return     EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
ExtractAndVerifyFspsEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_COMMON_SECTION_HEADER               *Section = NULL;
  EFI_PEI_GUIDED_SECTION_EXTRACTION_PPI   *GuidSectionPpi;
  EFI_STATUS                              Status;
  VOID                                    *OutputBuffer = NULL;
  UINTN                                   OutputBufferSize;
  UINT32                                  AuthenticationStatus;
  EFI_GUID                                *SectionDefinitionGuid;
  UINTN                                   FspImageBase;
  UINT32                                  BaseAddress;

  DEBUG ((DEBUG_INFO, "FspWrapperVerifyFspsEntry - Decompress and verify FSP-S\n"));

  Status = EFI_SUCCESS;

  Section = (EFI_COMMON_SECTION_HEADER *) FindFvSection (PcdGet32 (PcdFspsBaseAddress));
  if (Section->Type != EFI_SECTION_GUID_DEFINED) {
    DEBUG ((DEBUG_INFO, "Exit FspWrapperVerifyFspsEntry - Not Guided Section 0x%x\n", Section->Type));
    return EFI_INVALID_PARAMETER;
  }
  if (IS_SECTION2 (Section)) {
    SectionDefinitionGuid   = &((EFI_GUID_DEFINED_SECTION2 *)Section)->SectionDefinitionGuid;
  } else {
    SectionDefinitionGuid   = &((EFI_GUID_DEFINED_SECTION *)Section)->SectionDefinitionGuid;
  }

  if (VerifyGuidedSectionGuid (SectionDefinitionGuid, &GuidSectionPpi)) {
    Status = GuidSectionPpi->ExtractSection (
                               GuidSectionPpi,
                               Section,
                               &OutputBuffer,
                               &OutputBufferSize,
                               &AuthenticationStatus
                               );
  }
  if (Status == EFI_SUCCESS) {
    BaseAddress = ((((UINTN) OutputBuffer) + 0x10)& ~0xF);
    PcdSet32S (PcdFspsBaseAddress, BaseAddress);
    DEBUG ((DEBUG_INFO, "FSP-S Base Address set to 0x%x\n", BaseAddress));
    //
    // Verify, measure and create hssh event logs for FSP-S
    //
    PlatformVerifyFsps (BaseAddress);

    //
    // Update FSP-S base address in the FSP header.
    //
    Section = (EFI_COMMON_SECTION_HEADER *) FindFvSection (BaseAddress);
    FspImageBase = (UINT32) ((UINTN) Section + sizeof (EFI_RAW_SECTION) + FSP_HEADER_IMAGEBASE_OFFSET);
    *(UINT32 *)(UINTN)(FspImageBase) = BaseAddress;
  }

  return Status;
}
