/** @file
  Parse the INI configuration file and pass the information to the update driver
  so that the driver can perform update accordingly.

  Refer to BiosUpdateHelpersLib.h for more infomation about the structure of
  configuration file.

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
#include <Library/IniParsingLib.h>
#include <Library/PrintLib.h>
#include <Library/ConfigProfileAccessLib.h>

#define MAX_LINE_LENGTH           512

/**
  Parse Config data file to get the updated data array.

  @param[in]      DataBuffer      Config raw file buffer.
  @param[in]      BufferSize      Size of raw buffer.
  @param[in, out] ConfigHeader    Pointer to the config header.
  @param[in, out] UpdateArray     Pointer to the config of update data.

  @retval EFI_NOT_FOUND         No config data is found.
  @retval EFI_OUT_OF_RESOURCES  No enough memory is allocated.
  @retval EFI_SUCCESS           Parse the config file successfully.

**/
EFI_STATUS
ParseUpdateDataFile (
  IN      UINT8                         *DataBuffer,
  IN      UINTN                         BufferSize,
  IN OUT  CONFIG_HEADER                 *ConfigHeader,
  IN OUT  UPDATE_CONFIG_DATA            **UpdateArray
  )
{
  EFI_STATUS                            Status;
  CHAR8                                 *SectionName;
  CHAR8                                 Entry[MAX_LINE_LENGTH];
  UINTN                                 Num;
  UINT64                                Num64;
  UINTN                                 Index;
  EFI_GUID                              FileGuid;
  VOID                                  *Context;
  BOOLEAN                               SupportHelperImage;

  //
  // First process the data buffer and get all sections and entries
  //
  Context = OpenIniFile (DataBuffer, BufferSize);
  if (Context == NULL) {
    DEBUG ((DEBUG_ERROR, "ParseUpdateDataFile: Failed to open ini file"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Now get NumOfUpdate
  //
  Status = GetDecimalUintnFromDataFile (
             Context,
             "Head",
             "NumOfUpdate",
             &Num
             );
  if (EFI_ERROR (Status) || (Num == 0)) {
    DEBUG ((DEBUG_ERROR, "ParseUpdateDataFile: NumOfUpdate not found\n"));
    CloseIniFile (Context);
    return EFI_NOT_FOUND;
  }

  ConfigHeader->NumOfUpdates = Num;
  *UpdateArray = AllocateZeroPool ((sizeof (UPDATE_CONFIG_DATA) * Num));
  if (*UpdateArray == NULL) {
    CloseIniFile (Context);
    DEBUG ((DEBUG_ERROR, "ParseUpdateDataFile: Run out of memory resouce\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // FileGuid
  //
  Status = GetGuidFromDataFile (
             Context,
             "Head",
             "ImageFileGuid",
             &FileGuid
             );
  if (EFI_ERROR (Status)) {
    CloseIniFile (Context);
    DEBUG ((DEBUG_ERROR, "ParseUpdateDataFile: ImageFileGuid not found\n"));
    return EFI_NOT_FOUND;
  }

  CopyGuid (&ConfigHeader->FileGuid, &FileGuid);

  //
  // HelperFileGuid
  //
  SupportHelperImage = TRUE;
  Status = GetGuidFromDataFile (
             Context,
             "Head",
             "HelperFileGuid",
             &FileGuid
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ParseUpdateDataFile: HelperFileGuid is not found\n"));
    SetMem (&FileGuid, sizeof (EFI_GUID), 0);
    SupportHelperImage = FALSE;
  }

  CopyGuid (&ConfigHeader->HelperFileGuid, &FileGuid);

  for (Index = 0 ; Index < ConfigHeader->NumOfUpdates ; Index++) {
    //
    // Get the section name of each update
    //
    AsciiStrCpyS (Entry, MAX_LINE_LENGTH, "Update");
    AsciiValueToStringS (Entry + AsciiStrLen(Entry), MAX_LINE_LENGTH - AsciiStrLen(Entry), 0, Index, 0);
    Status = GetStringFromDataFile (
               Context,
               "Head",
               Entry,
               &SectionName
               );
    if (EFI_ERROR (Status) || (SectionName == NULL)) {
      DEBUG ((DEBUG_ERROR, "ParseUpdateDataFile: [%d] %a not found\n", Index, Entry));
      CloseIniFile (Context);
      return EFI_NOT_FOUND;
    }

    //
    // The section name of this update has been found.
    // Now looks for all the config data of this update
    //
    (*UpdateArray)[Index].Index = Index;

    //
    // BaseAddress
    //
    Status = GetHexUint64FromDataFile (
               Context,
               SectionName,
               "BaseAddress",
               &Num64
               );
    if (EFI_ERROR (Status)) {
      CloseIniFile (Context);
      DEBUG ((DEBUG_ERROR, "ParseUpdateDataFile: [%d] BaseAddress not found\n", Index));
      return EFI_NOT_FOUND;
    }
    (*UpdateArray)[Index].BaseAddress = (EFI_PHYSICAL_ADDRESS) Num64;

    //
    // Length
    //
    Status = GetHexUintnFromDataFile (
               Context,
               SectionName,
               "Length",
               &Num
               );
    if (EFI_ERROR (Status)) {
      CloseIniFile (Context);
      DEBUG ((DEBUG_ERROR, "ParseUpdateDataFile: [%d] Length not found\n", Index));
      return EFI_NOT_FOUND;
    }
    (*UpdateArray)[Index].Length = (UINTN) Num;

    //
    // ImageOffset
    //
    Status = GetHexUintnFromDataFile (
               Context,
               SectionName,
               "ImageOffset",
               &Num
               );
    if (EFI_ERROR (Status)) {
      CloseIniFile (Context);
      DEBUG ((DEBUG_ERROR, "ParseUpdateDataFile: [%d] ImageOffset not found\n", Index));
      return EFI_NOT_FOUND;
    }
    (*UpdateArray)[Index].ImageOffset = (UINTN) Num;

    //
    // HelperImageOffset
    //
    Status = GetHexUintnFromDataFile (
               Context,
               SectionName,
               "HelperOffset",
               &Num
               );
    if (EFI_ERROR (Status) || !SupportHelperImage) {
      Num = 0;
    }

    (*UpdateArray)[Index].HelperImageOffset = (UINTN) Num;

    //
    // HelperImageLength
    //
    Status = GetHexUintnFromDataFile (
               Context,
               SectionName,
               "HelperLength",
               &Num
               );
    if (EFI_ERROR (Status) || !SupportHelperImage) {
      Num = 0;
    }

    (*UpdateArray)[Index].HelperImageLength = (UINTN) Num;

  }

  //
  // Now all configuration data got. Free those temporary buffers
  //
  CloseIniFile (Context);

  return EFI_SUCCESS;
}
