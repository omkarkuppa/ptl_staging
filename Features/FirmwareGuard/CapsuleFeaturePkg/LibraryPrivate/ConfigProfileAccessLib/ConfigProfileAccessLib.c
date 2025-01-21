/** @file
  Parse config file and extract image accordingly

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
#include <Library/UefiLib.h>
#include <Library/ConfigProfileAccessLib.h>
#include <Library/FvImageExtractionLib.h>

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
  );

/**
  Extract config file from input Image and parse the config file.

  @param[in]   Image             The input image.
  @param[in]   ImageSize         The size of input image in bytes.
  @param[in]   ConfigFileGuid    The filename of Config file within Image.
  @param[out]  ConfigHeader      Pointer to parsed Config header.
  @param[out]  ConfigDataArray   Pointer to parsed Config data.

  @retval EFI_SUCCESS      The config file is successfully extracted and parsed.
  @retval EFI_NOT_FOUND    The compacted FvAdvanced is not extracted.

**/
EFI_STATUS
ExtractAndParseConfigImage (
  IN      VOID                          *Image,
  IN      UINTN                         ImageSize,
  IN      EFI_GUID                      *ConfigFileGuid,
  IN OUT  CONFIG_HEADER                 *ConfigHeader,
  IN OUT  UPDATE_CONFIG_DATA            **ConfigDataArray
  )
{
  EFI_STATUS                    Status;
  VOID                          *ConfigImage;
  UINTN                         ConfigImageSize;

  DEBUG ((DEBUG_INFO, "ExtractAndParseConfigImage: entry\n"));

  Status          = EFI_NOT_FOUND;
  ConfigImage     = NULL;
  ConfigImageSize = 0;

  ZeroMem (ConfigHeader, sizeof (CONFIG_HEADER));
  *ConfigDataArray = NULL;

  if (ExtractImageByName (Image, ImageSize, ConfigFileGuid, &ConfigImage, &ConfigImageSize)) {
    Status = ParseUpdateDataFile (
               ConfigImage,
               ConfigImageSize,
               ConfigHeader,
               ConfigDataArray
               );
  } else {
    DEBUG ((DEBUG_ERROR, "Config image file is not found"));
  }

  if (EFI_ERROR (Status) && (*ConfigDataArray != NULL)) {
    FreePool (*ConfigDataArray);
    *ConfigDataArray = NULL;
  }

  DEBUG ((DEBUG_INFO, "ExtractAndParseConfigImage: exit (%r)\n", Status));

  return Status;
}

/**
  Extract update image by config file.

  The input FvImage contains multiple FFS files including config data file
  and update image file. This function extracts and parse config file first,
  then extracts images of FileGuid and HelperFileGuid described in config header.

  @param[in]   FvImage           The input firmware volume image consisted of multiple FFSs.
  @param[in]   FvImageSize       The size of FvImage in bytes.
  @param[in]   ConfigFileGuid    The filename of Config file within FvImage.
  @param[out]  ConfigHeader      Pointer to parsed Config header.
  @param[out]  ConfigDataArray   Pointer to parsed Config data.
  @param[out]  UpdateImage       Pointer to image described by ConfigHeader->FileGuid.
  @param[out]  UpdateImageSize   The size of UpdateImage in bytes.
  @param[out]  HelperImage       Pointer to image described by ConfigHeader->HelperFileGuid. May be NULL.
  @param[out]  HelperImageSize   The size of HelperImage in bytes. May be NULL.

  @retval EFI_SUCCESS      UpdateImage is successfully extracted.
  @retval EFI_NOT_FOUND    UpdateImage is not extracted.

**/
EFI_STATUS
ExtractUpdateImageByConfigFile (
  IN      VOID                          *FvImage,
  IN      UINTN                         FvImageSize,
  IN      EFI_GUID                      *ConfigFileGuid,
  IN OUT  CONFIG_HEADER                 *ConfigHeader,
  IN OUT  UPDATE_CONFIG_DATA            **ConfigDataArray,
  IN OUT  VOID                          **UpdateImage,
  IN OUT  UINTN                         *UpdateImageSize,
  IN OUT  VOID                          **HelperImage,   OPTIONAL
  IN OUT  UINTN                         *HelperImageSize OPTIONAL
  )
{
  EFI_STATUS              Status;
  UINTN                   Index;
  UINTN                   ExpecteSize;

  DEBUG ((DEBUG_INFO, "ExtractUpdateImageByConfigFile: entry\n"));

  if ((ConfigHeader == NULL) || (ConfigDataArray == NULL) || (UpdateImage == NULL) || (UpdateImageSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Extract and parse config file
  //
  Status = ExtractAndParseConfigImage (
             FvImage,
             FvImageSize,
             ConfigFileGuid,
             ConfigHeader,
             ConfigDataArray
             );

  if (EFI_ERROR (Status) || ConfigDataArray == NULL || *ConfigDataArray == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to parse config image\n"));
    return Status;
  }

  //
  // Extract Update image
  //
  if (!ExtractImageByName (FvImage, FvImageSize, &ConfigHeader->FileGuid, UpdateImage, UpdateImageSize)) {
    DEBUG ((DEBUG_ERROR, "Failed to find Update image (%g)\n", &ConfigHeader->FileGuid));
    Status = EFI_NOT_FOUND;
    goto cleanup;
  }

  //
  // Boundary check
  //
  ExpecteSize = 0;
  for (Index = 0; Index < ConfigHeader->NumOfUpdates; Index++) {
    ExpecteSize = MAX (
                    (*ConfigDataArray)[Index].ImageOffset + (*ConfigDataArray)[Index].Length,
                    ExpecteSize
                    );
  }

  if (*UpdateImageSize > ExpecteSize) {
    //
    // ImageOffset described in Config data is less than Update image length --> Fv override
    //
    DEBUG ((DEBUG_ERROR, "Update image size (0x%x) is greater than expected (0x%x)\n", *UpdateImageSize, ExpecteSize));
    Status = EFI_UNSUPPORTED;
    goto cleanup;
  }

  //
  // Extract Helper image
  //
  if ((HelperImage != NULL) && (HelperImageSize != NULL)) {
    if (ExtractImageByName (FvImage, FvImageSize, &ConfigHeader->HelperFileGuid, HelperImage, HelperImageSize)) {
      //
      // Boundary check
      //
      ExpecteSize = 0;
      for (Index = 0; Index < ConfigHeader->NumOfUpdates; Index++) {
        ExpecteSize = MAX (
                        (*ConfigDataArray)[Index].HelperImageOffset + (*ConfigDataArray)[Index].HelperImageLength,
                        ExpecteSize
                        );
      }
      if (*HelperImageSize < ExpecteSize) {
        //
        // HelperImageOffset described in Config data exceeds Helper image length
        //
        DEBUG ((DEBUG_ERROR, "Helper image size (0x%x) is smaller than expected (0x%x)\n", *HelperImageSize, ExpecteSize));
        Status = EFI_UNSUPPORTED;
        goto cleanup;
      }
    } else {
      //
      // Expect a Helper image but fail to extract
      //
      DEBUG ((DEBUG_ERROR, "Helper image is expected but fail to extract\n"));
      Status = EFI_NOT_FOUND;
      goto cleanup;
    }
  }

cleanup:
  if (EFI_ERROR (Status)) {
    FreePool (*ConfigDataArray);
    *ConfigDataArray = NULL;
    ZeroMem (ConfigHeader, sizeof (CONFIG_HEADER));
  }

  DEBUG ((DEBUG_INFO, "ExtractUpdateImageByConfigFile: exit (%r)\n", Status));
  return Status;
}
