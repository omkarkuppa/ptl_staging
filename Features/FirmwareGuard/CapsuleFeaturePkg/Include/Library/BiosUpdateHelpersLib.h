/** @file
  Library instance provide common functions that can be shared among different
  FmpDeviceLib instances especially for the instance which involves BIOS and/or
  multiple components update.

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

#ifndef __BIOS_UPDATE_HELPERS_LIB_H__
#define __BIOS_UPDATE_HELPERS_LIB_H__

#include <Protocol/FirmwareManagement.h>

/**
  Extract compact FvAdvanced image raw data from FMP payload image.

  The input image contains multiple FFS files. This function
  extract BIOS image first and then find out the compact FvAdvanced
  image within BIOS image.

  @param[in]   Image                 Pointer to FMP payload image (FMP image header is stripped off)
  @param[in]   ImageSize             The size of input image in bytes.
  @param[out]  FvAdvancedImage       The image pointer to compacted FvAdvanced raw data.
  @param[out]  FvAdvancedImageSize   The size of compacted FvAdvanced image.

  @retval TRUE  The compacted FvAdvanced is extracted.
  @retval FALSE The compacted FvAdvanced is not extracted.

**/
BOOLEAN
ExtractBiosObbFromImage (
  IN      VOID                          *Image,
  IN      UINTN                         ImageSize,
  IN      VOID                          **BiosObbImage,
  IN      UINTN                         *BiosObbImageSize
  );

/**
  Extract NonFitPayload image from FMP payload image.

  The input image contains multiple FFS files. This function
  extract BIOS image first and then find out NonFitPayload image within BIOS image.

  @param[in]   Image                 Pointer to FMP payload image (FMP image header is stripped off)
  @param[in]   ImageSize             The size of input image in bytes.
  @param[out]  NonFitPayload         The image pointer to NonFitPayload image.
  @param[out]  NonFitPayloadSize     The size of NonFitPayload.

  @retval TRUE  NonFitPayload image is extracted.
  @retval FALSE NonFitPayload image is not extracted.

**/
BOOLEAN
ExtractNonFitPayloadFromImage (
  IN      VOID                          *Image,
  IN      UINTN                         ImageSize,
  IN      VOID                          **NonFitPayload,
  IN      UINTN                         *NonFitPayloadSize
  );

/**
  Extract BIOS Extended image from FMP payload image.

  The input image contains multiple FFS files. This function
  extract BIOS image first and then find out Extended image within BIOS image.

  @param[in]   Image                 Pointer to FMP payload image (FMP image header is stripped off)
  @param[in]   ImageSize             The size of input image in bytes.
  @param[out]  BiosExtendedImage     The image pointer to Extended image.
  @param[out]  BiosExtendedImageSize The size of ExtendedBiosImage.

  @retval TRUE  BIOS Extended image is extracted.
  @retval FALSE BIOS Extended image is not extracted.

**/
BOOLEAN
ExtractExtendedBiosFromImage (
  IN      VOID                          *Image,
  IN      UINTN                         ImageSize,
  IN      VOID                          **BiosExtendedImage,
  IN      UINTN                         *BiosExtendedImageSize
  );
/**
  Check if the input Image contains expected files used for BIOS update.

  @param[in]   Image               Pointer to FMP payload image (FMP image header is stripped off)
  @param[in]   ImageSize           The size of Image in bytes

  @retval TRUE  The image contains expected files used for BIOS update.
  @retval FALSE The image does not contain expected files used for BIOS update.

**/
BOOLEAN
IsImageForBiosUpdate (
  IN      VOID                          *Image,
  IN      UINTN                         ImageSize
  );
/**
  Check if the input Image contains expected files used for Extended BIOS update.

  @param[in]   Image               Pointer to FMP payload image (FMP image header is stripped off)
  @param[in]   ImageSize           The size of Image in bytes

  @retval TRUE  The image contains expected files used for Extended BIOS update.
  @retval FALSE The image does not contain expected files used for Extended BIOS update.

**/
BOOLEAN
IsImageForExtendedBiosUpdate (
  IN      VOID                          *Image,
  IN      UINTN                         ImageSize
  );
/**
  Perform Extended BIOS update in stages

  The input Image contains multiple FFS files including config data file
  and Extended BIOS image file. This function extracts and parse config file first,
  then extracts BIOS and BGUP images described in config header.
  Finally update BIOS region by stage according to the address described in
  config data.
  This function povides the capability to support fault tolerant BIOS update
  by recording update progress and switching BootBlock region accordingly between
  update stages.

  @param[in] Image                   Pointer to FMP payload image (FMP image header is stripped off)
  @param[in] ImageSize               The size of Image in bytes
  @param[in] LastUpdatingProgress    Indicates the update progress if BIOS update got interrupted last time.
  @param[in] DisplayProgress         A function used to display the progress of the firmware update.
                                     This is an optional parameter that may be NULL.
  @param[in] StartPercentage         The start completion percentage value that may be used to report
                                     progress during the flash write operation.
  @param[in] EndPercentage           The end completion percentage value that may be used to report
                                     progress during the flash write operation.

  @retval EFI_SUCCESS                Extended BIOS update is successfully performed.
  @retval Other                      Extended BIOS update fails.

**/
EFI_STATUS
PerformExtendedBiosUpdateByConfigData (
  IN VOID                                           *Image,
  IN UINTN                                          ImageSize,
  IN UINT32                                         LastUpdatingProgress,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  DisplayProgress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );

/**
  Perform BIOS update in stages

  The input Image contains multiple FFS files including config data file
  and BIOS image file. This function extracts and parse config file first,
  then extracts BIOS and BGUP images described in config header.
  Finally update BIOS region by stage according to the address described in
  config data.
  This function povides the capability to support fault tolerant BIOS update
  by recording update progress and switching BootBlock region accordingly between
  update stages.

  @param[in] Image                   Pointer to FMP payload image (FMP image header is stripped off)
  @param[in] ImageSize               The size of Image in bytes
  @param[in] LastUpdatingProgress    Indicates the update progress if BIOS update got interrupted last time.
  @param[in] DisplayProgress         A function used to display the progress of the firmware update.
                                     This is an optional parameter that may be NULL.
  @param[in] StartPercentage         The start completion percentage value that may be used to report
                                     progress during the flash write operation.
  @param[in] EndPercentage           The end completion percentage value that may be used to report
                                     progress during the flash write operation.

  @retval EFI_SUCCESS                BIOS update is successfully performed.
  @retval Other                      BIOS update fails.

**/
EFI_STATUS
PerformBiosUpdateByConfigData (
  IN VOID                                           *Image,
  IN UINTN                                          ImageSize,
  IN UINT32                                         LastUpdatingProgress,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  DisplayProgress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );

#endif
