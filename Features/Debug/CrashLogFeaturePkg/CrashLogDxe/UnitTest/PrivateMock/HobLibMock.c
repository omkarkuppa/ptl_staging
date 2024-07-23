/**@file
    HobLib Mock for the CrashLog Dxe driver.

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

@par Specification
**/

#include <Uefi.h>
#include <Base.h>
#include <Library/BaseMemoryLib.h>

#define EFI_HOB_TYPE_GUID_EXTENSION       0x0004
#define EFI_HOB_TYPE_END_OF_HOB_LIST      0xFFFF

///
/// Describes the format and size of the data inside the HOB.
/// All HOBs must contain this generic HOB header.
///
typedef struct {
  ///
  /// Identifies the HOB data structure type.
  ///
  UINT16    HobType;
  ///
  /// The length in bytes of the HOB.
  ///
  UINT16    HobLength;
  ///
  /// This field must always be set to zero.
  ///
  UINT32    Reserved;
} EFI_HOB_GENERIC_HEADER;

///
/// Allows writers of executable content in the HOB producer phase to
/// maintain and manage HOBs with specific GUID.
///
typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_GUID_EXTENSION.
  ///
  EFI_HOB_GENERIC_HEADER    Header;
  ///
  /// A GUID that defines the contents of this HOB.
  ///
  EFI_GUID                  Name;
  //
  // Guid specific data goes here
  //
} EFI_HOB_GUID_TYPE;

///
/// Union of all the possible HOB Types.
///
typedef union {
  EFI_HOB_GENERIC_HEADER                 *Header;
  EFI_HOB_GUID_TYPE                      *Guid;
  UINT8                                  *Raw;
} EFI_PEI_HOB_POINTERS;


VOID  *gHobList = NULL;


/**
  Returns the next instance of a HOB type from the starting HOB.

  This function searches the first instance of a HOB type from the starting HOB pointer.
  If there does not exist such HOB type from the starting HOB pointer, it will return NULL.
  In contrast with macro GET_NEXT_HOB(), this function does not skip the starting HOB pointer
  unconditionally: it returns HobStart back if HobStart itself meets the requirement;
  caller is required to use GET_NEXT_HOB() if it wishes to skip current HobStart.

  If HobStart is NULL, then ASSERT().

  @param  Type          The HOB type to return.
  @param  HobStart      The starting HOB pointer to search from.

  @return The next instance of a HOB type from the starting HOB.

**/
VOID *
EFIAPI
GetNextHob (
  IN UINT16      Type,
  IN CONST VOID  *HobStart
  )
{
  EFI_PEI_HOB_POINTERS  Hob;

  Hob.Raw = (UINT8 *)HobStart;
  //
  // Parse the HOB list until end of list or matching type is found.
  //
  while (Hob.Header->HobType != EFI_HOB_TYPE_END_OF_HOB_LIST) {
    if (Hob.Header->HobType == Type) {
      return Hob.Raw;
    }

    Hob.Raw = Hob.Raw + Hob.Header->HobLength;
  }

  return NULL;
}

/**
  Returns the next instance of the matched GUID HOB from the starting HOB.

  This function searches the first instance of a HOB from the starting HOB pointer.
  Such HOB should satisfy two conditions:
  its HOB type is EFI_HOB_TYPE_GUID_EXTENSION and its GUID Name equals to the input Guid.
  If there does not exist such HOB from the starting HOB pointer, it will return NULL.
  Caller is required to apply GET_GUID_HOB_DATA () and GET_GUID_HOB_DATA_SIZE ()
  to extract the data section and its size information, respectively.
  In contrast with macro GET_NEXT_HOB(), this function does not skip the starting HOB pointer
  unconditionally: it returns HobStart back if HobStart itself meets the requirement;
  caller is required to use GET_NEXT_HOB() if it wishes to skip current HobStart.

  If Guid is NULL, then ASSERT().
  If HobStart is NULL, then ASSERT().

  @param  Guid          The GUID to match with in the HOB list.
  @param  HobStart      A pointer to a Guid.

  @return The next instance of the matched GUID HOB from the starting HOB.

**/
VOID *
EFIAPI
GetNextGuidHob (
  IN CONST EFI_GUID  *Guid,
  IN CONST VOID      *HobStart
  )
{
  EFI_PEI_HOB_POINTERS  GuidHob;

  GuidHob.Raw = (UINT8 *)HobStart;
  while ((GuidHob.Raw = GetNextHob (EFI_HOB_TYPE_GUID_EXTENSION, GuidHob.Raw)) != NULL) {
    if (CompareGuid (Guid, &GuidHob.Guid->Name)) {
      break;
    }

    GuidHob.Raw = GuidHob.Raw + GuidHob.Header->HobLength;
  }

  return GuidHob.Raw;
}

/**
  Returns the first instance of the matched GUID HOB among the whole HOB list.

  This function searches the first instance of a HOB among the whole HOB list.
  Such HOB should satisfy two conditions:
  its HOB type is EFI_HOB_TYPE_GUID_EXTENSION and its GUID Name equals to the input Guid.
  If there does not exist such HOB from the starting HOB pointer, it will return NULL.
  Caller is required to apply GET_GUID_HOB_DATA () and GET_GUID_HOB_DATA_SIZE ()
  to extract the data section and its size information, respectively.

  If the pointer to the HOB list is NULL, then ASSERT().
  If Guid is NULL, then ASSERT().

  @param  Guid          The GUID to match with in the HOB list.

  @return The first instance of the matched GUID HOB among the whole HOB list.

**/
VOID *
EFIAPI
GetFirstGuidHob (
  IN CONST EFI_GUID  *Guid
  )
{
  return GetNextGuidHob (Guid, gHobList);
}
