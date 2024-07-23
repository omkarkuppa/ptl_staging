/**@file
  Pei Sub Region Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiSubRegionLib.h>

EFI_GUID* mSubRegionGuids[] = {
  &gFmpCapsuleTsnMacAddrFileGuid
};

/**
  Return if input ImageGuid belongs to a Sub Region FMP

  @param[in] ImageGuid A pointer to GUID

  @retval TRUE  ImageGuid belongs to a Sub Region FMP
  @retval FALSE ImageGuid does not belong to a Sub Region FMP

**/
BOOLEAN
IsBiosSubRegionFmpGuid (
  IN  EFI_GUID    *ImageGuid
  )
{
  UINT8 Index = 0;

  for (Index = 0; Index < sizeof (mSubRegionGuids) / sizeof (mSubRegionGuids[0]); Index++) {
    if (CompareGuid (ImageGuid, mSubRegionGuids[Index])) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  This function returns the section size in a file.

  @param[in]  SectionType           EFI Firmware File Section type.
  @param[in]  Data                  A pointer to the section data.

  @retval Size                      Section data size.

**/
UINTN
GetSectionSize (
  IN  EFI_SECTION_TYPE              SectionType,
  IN  VOID                          *Data
  )
{
  UINT32                     SectionSize;
  UINTN                      SectionHeaderSize;
  EFI_COMMON_SECTION_HEADER  *SectionData;

  SectionHeaderSize = sizeof (EFI_COMMON_SECTION_HEADER);
  SectionData = (EFI_COMMON_SECTION_HEADER *)((UINT8 *)Data - SectionHeaderSize);
  if (SectionData->Type != SectionType) {
    SectionHeaderSize = sizeof (EFI_COMMON_SECTION_HEADER2);
    SectionData = (EFI_COMMON_SECTION_HEADER *)((UINT8 *)Data - SectionHeaderSize);
  }
  ASSERT (SectionData->Type == SectionType);
  if (IS_SECTION2(SectionData)) {
    SectionSize = SECTION2_SIZE(SectionData);
  } else {
    SectionSize = SECTION_SIZE(SectionData);
  }
  return SectionSize - SectionHeaderSize;
}

/**
  Searches all the available firmware volumes and returns the first matching FFS section.

  This function searches all the firmware volumes for FFS files with an FFS filename
  specified by NameGuid. The order in which the firmware volumes are searched is not
  deterministic. For each FFS file found, a search is made for FFS sections of type
  SectionType. If the FFS file contains at least SectionInstance instances of the FFS
  section specified by SectionType, then the SectionInstance instance is returned in
  Buffer. Buffer is allocated using AllocatePool(), and the size of the allocated buffer
  is returned in Size.

  If SectionType is EFI_SECTION_TE, and the search with an FFS file fails,
  the search will be retried with a section type of EFI_SECTION_PE32.

  If NameGuid is NULL, then ASSERT().
  If Buffer is NULL, then ASSERT().
  If Size is NULL, then ASSERT().


  @param  NameGuid             A pointer to to the FFS filename GUID to search for
                               within any of the firmware volumes in the platform.
  @param  SectionType          Indicates the FFS section type to search for within
                               the FFS file specified by NameGuid.
  @param  Buffer               On output, a pointer to a buffer
                               containing the FFS file section that was found.
  @param  Size                 On output, a pointer to the size, in bytes, of Buffer.

  @retval  EFI_SUCCESS          The specified FFS section was returned.
  @retval  EFI_NOT_FOUND        The specified FFS section could not be found.
  @retval  EFI_OUT_OF_RESOURCES There are not enough resources available to retrieve
                                the matching FFS section.
  @retval  EFI_DEVICE_ERROR     The FFS section could not be retrieves due to a
                                device error.
  @retval  EFI_ACCESS_DENIED    The FFS section could not be retrieves because the
                                firmware volume that contains the matching FFS
                                section does not allow reads.
**/
EFI_STATUS
EFIAPI
ReadSectionInformationFromFv  (
  IN  CONST EFI_GUID                *NameGuid,
  IN  EFI_SECTION_TYPE              SectionType,
  OUT VOID                          **Buffer,
  OUT UINTN                         *Size
  )
{
  EFI_STATUS                 Status;
  UINTN                      FvIndex;
  EFI_PEI_FV_HANDLE          VolumeHandle;
  EFI_PEI_FILE_HANDLE        FileHandle;
  EFI_COMMON_SECTION_HEADER  *SectionData;

  //
  // Search all FV
  //
  VolumeHandle = NULL;
  for (FvIndex = 0; ; FvIndex++) {
    Status = PeiServicesFfsFindNextVolume (FvIndex, &VolumeHandle);
    if (EFI_ERROR (Status)) {
      break;
    }
    //
    // Search PEIM FFS
    //
    FileHandle = NULL;
    Status = PeiServicesFfsFindFileByName (NameGuid, VolumeHandle, &FileHandle);
    if (EFI_ERROR (Status)) {
      continue;
    }
    //
    // Search Section
    //
    SectionData = NULL;
    Status = PeiServicesFfsFindSectionData (SectionType, FileHandle, Buffer);
    if (EFI_ERROR (Status)) {
      continue;
    }
    //
    // Get section size
    //
    *Size = GetSectionSize (SectionType, *Buffer);
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}