/** @file
  Library instance provide common functions that can be shared among different
  FmpDeviceLib instances especially for the instance which involves BIOS and/or
  multiple components update.

  The major functionalitise are:
  1. Extract files/compacted FvAdvanced from a given image
  2. Parse config ini file
  3. Perform BIOS update by config data with/without BiosGuard BGUP image

  Refer to BiosUpdateHelpersLib.h for more infomation about the image structure.

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
#include <Library/BiosUpdateHelpersLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/ComponentUpdateLib.h>
#include <Library/ConfigProfileAccessLib.h>
#include <Library/FvImageExtractionLib.h>
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PcdLib.h>
#include <Library/CbntSvnCheckLib.h>
#include <Library/TopSwapSupportLib.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/ResiliencySupportLib.h>

BOOLEAN             mBiosGuardEnabled = FALSE;

/**
  Extract BIOS Obb image from FMP payload image.

  The input image contains multiple FFS files. This function
  extract BIOS image first and then find out Obb image within BIOS image.

  @param[in]   Image                 Pointer to FMP payload image (FMP image header is stripped off)
  @param[in]   ImageSize             The size of input image in bytes.
  @param[out]  BiosObbImage          The image pointer to BIOS Obb image.
  @param[out]  BiosObbImageSize      The size of BiosObbImage.

  @retval TRUE  BIOS Obb image is extracted.
  @retval FALSE BIOS Obb image is not extracted.

**/
BOOLEAN
ExtractBiosObbFromImage (
  IN      VOID                          *Image,
  IN      UINTN                         ImageSize,
  IN      VOID                          **BiosObbImage,
  IN      UINTN                         *BiosObbImageSize
  )
{
  VOID        *BiosImage;
  UINTN       BiosImageSize;

  BiosImage     = NULL;
  BiosImageSize = 0;

  //
  // Find out BIOS image within Image first.
  //
  if (ExtractImageByName ((VOID *) Image, ImageSize, &gCapsuleBiosImageFileGuid, &BiosImage, &BiosImageSize)) {
    //
    // BIOS image is found. Check boundary.
    //
    if (BiosImageSize >= (FixedPcdGet32 (PcdFlashObbOffset) + FixedPcdGet32 (PcdFlashObbSize))) {
      *BiosObbImage     = (VOID *)((UINTN) BiosImage + FixedPcdGet32 (PcdFlashObbOffset));
      *BiosObbImageSize = FixedPcdGet32 (PcdFlashObbSize);
      return TRUE;
    }
  }

  return FALSE;
}

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
  )
{
  VOID        *BiosImage;
  UINTN       BiosImageSize;

  BiosImage     = NULL;
  BiosImageSize = 0;

  //
  // Find out BIOS image within Image first.
  //
  if (ExtractImageByName ((VOID *) Image, ImageSize, &gCapsuleBiosImageFileGuid, &BiosImage, &BiosImageSize)) {
    //
    // BIOS image is found. Check boundary.
    //
    if (BiosImageSize >= (FixedPcdGet32 (PcdFlashNonFitPayloadOffset) + FixedPcdGet32 (PcdFlashNonFitPayloadSize))) {
      *NonFitPayload     = (VOID *)((UINTN) BiosImage + FixedPcdGet32 (PcdFlashNonFitPayloadOffset));
      *NonFitPayloadSize = FixedPcdGet32 (PcdFlashNonFitPayloadSize);
      return TRUE;
    }
  }

  return FALSE;
}

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
  )
{
  VOID        *BiosImage;
  UINTN       BiosImageSize;

  BiosImage     = NULL;
  BiosImageSize = 0;

  //
  // Find out Extended BIOS image within Image first.
  //
  if (ExtractImageByName ((VOID *) Image, ImageSize, &gCapsuleBiosImageFileGuid, &BiosImage, &BiosImageSize)) {
    //
    // Extended BIOS image is found. Check boundary.
    //
    if (BiosImageSize >= (FixedPcdGet32 (PcdFlashExtendSize)
                        + FixedPcdGet32 (PcdFlashObbOffset)
                        - FixedPcdGet32 (PcdFlashAllExtendedRegionSize)
                        + FixedPcdGet32 (PcdFlashObbSize))) {
      *BiosExtendedImage     = (VOID *)((UINTN) BiosImage);
      *BiosExtendedImageSize = FixedPcdGet32 (PcdFlashExtendSize);
      return TRUE;
    }
  }

  return FALSE;
}
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
  )
{
  EFI_STATUS                  Status;
  CONFIG_HEADER               ConfigHeader;
  UPDATE_CONFIG_DATA          *ConfigData;
  VOID                        *BiosImage;
  UINTN                       BiosImageSize;
  VOID                        *BgupImage;
  UINTN                       BgupImageSize;
  UINTN                       BiosSize;

  DEBUG ((DEBUG_INFO, "%a - start\n", __FUNCTION__));

  ConfigData = NULL;
  BiosSize = PcdGet32 (PcdBiosSize);

  //
  // Check if all needed binaries for BIOS update can be extracted from Image
  //
  Status = ExtractUpdateImageByConfigFile (
             Image,
             ImageSize,
             &gCapsuleBiosConfigFileGuid,
             &ConfigHeader,
             &ConfigData,
             &BiosImage,
             &BiosImageSize,
             mBiosGuardEnabled ? &BgupImage : NULL,
             mBiosGuardEnabled ? &BgupImageSize : NULL
             );

  if (ConfigData != NULL) {
    //
    // No need to use ConfigData in this function anymore.
    //
    FreePool (ConfigData);
  }

  if (EFI_ERROR (Status) || \
      (!CompareGuid (&(ConfigHeader.FileGuid), &gCapsuleBiosImageFileGuid))) {
    DEBUG ((DEBUG_ERROR, "Failed to extract config file or bios image\n"));
    return FALSE;
  }

  //
  // Compare the bios size to make sure update can continue
  //
  if (BiosImageSize != BiosSize) {
    DEBUG ((DEBUG_ERROR, "Bios size does not match, %d %d\n", BiosImageSize, BiosSize));
    return FALSE;
  }

#if FixedPcdGetBool (PcdCheckArbhSvnBeforeUpdate) == 1
  Status = CheckSvnFromBiosImage ((UINT8 *) BiosImage, BiosImageSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SVN check fail\n"));
    return FALSE;
  }
#endif

  DEBUG ((DEBUG_INFO, "%a - pass\n", __FUNCTION__));
  return TRUE;
}

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
  )
{
  EFI_STATUS                  Status;
  CONFIG_HEADER               ConfigHeader;
  UPDATE_CONFIG_DATA          *ConfigData;
  VOID                        *ExtendedBiosImage;
  UINTN                       ExtendedBiosImageSize;
  VOID                        *BgupImage;
  UINTN                       BgupImageSize;
  UINTN                       ExtendedBiosSize;

  ConfigData = NULL;
  ExtendedBiosSize = PcdGet32 (PcdFlashExtendSize) + 0x1000000;

  //
  // Check if all needed binaries for BIOS update can be extracted from Image
  //
  Status = ExtractUpdateImageByConfigFile (
             Image,
             ImageSize,
             &gCapsuleExtendedBiosConfigFileGuid,
             &ConfigHeader,
             &ConfigData,
             &ExtendedBiosImage,
             &ExtendedBiosImageSize,
             mBiosGuardEnabled ? &BgupImage : NULL,
             mBiosGuardEnabled ? &BgupImageSize : NULL
             );

  if (ConfigData != NULL) {
    //
    // No need to use ConfigData in this function anymore.
    //
    FreePool (ConfigData);
  }

  if (Status == EFI_SUCCESS) {
    if (CompareGuid (&(ConfigHeader.FileGuid), &gCapsuleBiosImageFileGuid)) {
      //
      // continue compare the bios size to make sure update can be continue
      //
      if (ExtendedBiosImageSize == ExtendedBiosSize) {
        DEBUG ((DEBUG_INFO, "IsImageForExtendedBiosUpdate: TRUE %d \n", ExtendedBiosSize));
        return TRUE;
      } else {
        DEBUG ((DEBUG_ERROR, "IsImageForExtendedBiosUpdate: bios size does not match, %d %d\n", ExtendedBiosImageSize, ExtendedBiosSize));
        return FALSE;
      }
    } else {
      DEBUG ((DEBUG_ERROR, "The file indicated in Congfig data is not for Extended BIOS update\n"));
    }
  }

#if FixedPcdGetBool (PcdCheckArbhSvnBeforeUpdate) == 1
  Status = CheckSvnFromBiosImage ((UINT8 *) ExtendedBiosImage, ExtendedBiosImageSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SVN check fail\n"));
    return FALSE;
  }
#endif

  DEBUG ((DEBUG_ERROR, "IsImageForExtendedBiosUpdate: FALSE\n"));
  return FALSE;
}

/**
  Perform Extended BIOS update in stages

  The input Image contains multiple FFS files including config data file
  and Extended BIOS image file. This function extracts and parse config file first,
  then extracts Extended BIOS and BGUP images described in config header.
  Finally update Extended BIOS region by stage according to the address described in
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
PerformExtendedBiosUpdateByConfigData (
  IN VOID                                           *Image,
  IN UINTN                                          ImageSize,
  IN UINT32                                         LastUpdatingProgress,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  DisplayProgress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS                            Status;
  CONFIG_HEADER                         ConfigHeader;
  UPDATE_CONFIG_DATA                    *ConfigData;
  VOID                                  *ExtendedBiosImage;
  UINTN                                 ExtendedBiosImageSize;
  VOID                                  *BgupImage;
  UINTN                                 BgupImageSize;
  UINTN                                 DisplayLength;
  UINTN                                 Index;
  UINTN                                 TotalUpdateSize;
  UINTN                                 BytesWritten;
  EFI_PHYSICAL_ADDRESS                  FlashAddress;
  VOID                                  *UpdateBuffer;
  UINTN                                 UpdateLength;
  VOID                                  *BgupBuffer;
  UINTN                                 BgupBufferLength;
  FLASH_ACCESS_REQUEST                  FlashReq;

  DEBUG ((DEBUG_INFO, "PerformExtendedBiosUpdateByConfigData: entry\n"));

  ConfigData              = NULL;
  ExtendedBiosImage       = NULL;
  ExtendedBiosImageSize   = 0;
  BgupImage               = NULL;
  BgupImageSize           = 0;
  BytesWritten            = 0;
  DisplayLength           = EndPercentage - StartPercentage;

  //
  // Extract Extended BIOS image and BGUP image (if BiosGuard is enabled) by parsing Config file.
  //
  Status = ExtractUpdateImageByConfigFile (
             Image,
             ImageSize,
             &gCapsuleExtendedBiosConfigFileGuid,
             &ConfigHeader,
             &ConfigData,
             &ExtendedBiosImage,
             &ExtendedBiosImageSize,
             mBiosGuardEnabled ? &BgupImage : NULL,
             mBiosGuardEnabled ? &BgupImageSize : NULL
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PerformBiosUpdateByConfigData: Failed to ExtractUpdateImageByConfigFile (%r)\n", Status));
    return Status;
  }

  //
  // Calculate the toltal size to be written
  //
  TotalUpdateSize = 0;
  for (Index = 0; Index < ConfigHeader.NumOfUpdates; Index++) {
    TotalUpdateSize += ConfigData[Index].Length;
    if (Index < LastUpdatingProgress) {
      BytesWritten += ConfigData[Index].Length;
    }
  }

  for (Index = LastUpdatingProgress; Index < ConfigHeader.NumOfUpdates; Index++) {
    DEBUG ((DEBUG_INFO,
      "PerformBiosUpdateByConfigData (stage %d): BaseAddress - 0x%lx ImageOffset - 0x%x Length - 0x%x\n",
      Index,
      ConfigData[Index].BaseAddress,
      ConfigData[Index].ImageOffset,
      ConfigData[Index].Length
      ));

    FlashAddress     = ConfigData[Index].BaseAddress;
    UpdateBuffer     = (VOID *)((UINTN) ExtendedBiosImage + ConfigData[Index].ImageOffset);
    UpdateLength     = ConfigData[Index].Length;
    BgupBuffer       = (VOID *)((UINTN) BgupImage + ConfigData[Index].HelperImageOffset);
    BgupBufferLength = ConfigData[Index].HelperImageLength;

    if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      if (Index == BiosIbbR) {
        TopSwapControl (TRUE);
      }
    } else {
      if (Index == BiosIbb) {
        TopSwapControl (TRUE);
      }
    }

    SetUpdateProgress (UpdatingBios, (UINT16) Index);
    ZeroMem (&FlashReq, sizeof (FlashReq));
    FlashReq.Data = UpdateBuffer;
    FlashReq.DataSize = UpdateLength;
    FlashReq.FlashAddress = FlashAddress;
    FlashReq.FlashRegionType = FlashRegionBios;
    FlashReq.BgupImage = BgupBuffer;
    FlashReq.BgupImageSize = BgupBufferLength;
    Status = UpdateBiosFirmware (
               &FlashReq,
               DisplayProgress,
               StartPercentage + ((BytesWritten * DisplayLength) / TotalUpdateSize),
               StartPercentage + (((BytesWritten + UpdateLength) * DisplayLength) / TotalUpdateSize)
               );

    if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      if (Index == BiosIbbR) {
        TopSwapControl (FALSE);
      }
    } else {
      if (Index == BiosIbb) {
        TopSwapControl (FALSE);
      }
    }

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UpdateExtendedBiosFirmware failed in stage %d (%r)\n", Index, Status));
      break;
    }

    BytesWritten += UpdateLength;
  }

  if (ConfigData != NULL) {
    FreePool (ConfigData);
  }

  DEBUG ((DEBUG_INFO, "PerformExtendedBiosUpdateByConfigData: exit (%r)\n", Status));
  return Status;
}

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
  )
{
  EFI_STATUS                            Status;
  CONFIG_HEADER                         ConfigHeader;
  UPDATE_CONFIG_DATA                    *ConfigData;
  VOID                                  *BiosImage;
  UINTN                                 BiosImageSize;
  VOID                                  *BgupImage;
  UINTN                                 BgupImageSize;
  UINTN                                 DisplayLength;
  UINTN                                 Index;
  UINTN                                 TotalUpdateSize;
  UINTN                                 BytesWritten;
  EFI_PHYSICAL_ADDRESS                  FlashAddress;
  VOID                                  *UpdateBuffer;
  UINTN                                 UpdateLength;
  VOID                                  *BgupBuffer;
  UINTN                                 BgupBufferLength;
  FLASH_ACCESS_REQUEST                  FlashReq;

  DEBUG ((DEBUG_INFO, "PerformBiosUpdateByConfigData: entry\n"));

  ConfigData      = NULL;
  BiosImage       = NULL;
  BiosImageSize   = 0;
  BgupImage       = NULL;
  BgupImageSize   = 0;
  BytesWritten    = 0;
  DisplayLength   = EndPercentage - StartPercentage;

  //
  // Extract BIOS image and BGUP image (if BiosGuard is enabled) by parsing Config file.
  //
  Status = ExtractUpdateImageByConfigFile (
             Image,
             ImageSize,
             &gCapsuleBiosConfigFileGuid,
             &ConfigHeader,
             &ConfigData,
             &BiosImage,
             &BiosImageSize,
             mBiosGuardEnabled ? &BgupImage : NULL,
             mBiosGuardEnabled ? &BgupImageSize : NULL
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PerformBiosUpdateByConfigData: Failed to ExtractUpdateImageByConfigFile (%r)\n", Status));
    return Status;
  }

  //
  // Calculate the toltal size to be written
  //
  TotalUpdateSize = 0;
  for (Index = 0; Index < ConfigHeader.NumOfUpdates; Index++) {
    TotalUpdateSize += ConfigData[Index].Length;
    if (Index < LastUpdatingProgress) {
      BytesWritten += ConfigData[Index].Length;
    }
  }

  for (Index = LastUpdatingProgress; Index < ConfigHeader.NumOfUpdates; Index++) {
    DEBUG ((DEBUG_INFO,
      "PerformBiosUpdateByConfigData (stage %d): BaseAddress - 0x%lx ImageOffset - 0x%x Length - 0x%x\n",
      Index,
      ConfigData[Index].BaseAddress,
      ConfigData[Index].ImageOffset,
      ConfigData[Index].Length
      ));

    FlashAddress     = ConfigData[Index].BaseAddress;
    UpdateBuffer     = (VOID *)((UINTN) BiosImage + ConfigData[Index].ImageOffset);
    UpdateLength     = ConfigData[Index].Length;
    BgupBuffer       = (VOID *)((UINTN) BgupImage + ConfigData[Index].HelperImageOffset);
    BgupBufferLength = ConfigData[Index].HelperImageLength;

    if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      if (Index == BiosIbbR) {
        TopSwapControl (TRUE);
      }
    } else {
      if (Index == BiosIbb) {
        TopSwapControl (TRUE);
      }
    }

    SetUpdateProgress (UpdatingBios, (UINT16) Index);
    ZeroMem (&FlashReq, sizeof (FlashReq));
    FlashReq.Data = UpdateBuffer;
    FlashReq.DataSize = UpdateLength;
    FlashReq.FlashAddress = FlashAddress;
    FlashReq.FlashRegionType = FlashRegionBios;
    FlashReq.BgupImage = BgupBuffer;
    FlashReq.BgupImageSize = BgupBufferLength;
    Status = UpdateBiosFirmware (
               &FlashReq,
               DisplayProgress,
               StartPercentage + ((BytesWritten * DisplayLength) / TotalUpdateSize),
               StartPercentage + (((BytesWritten + UpdateLength) * DisplayLength) / TotalUpdateSize)
               );

    if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      if (Index == BiosIbbR) {
        TopSwapControl (FALSE);
      }
    } else {
      if (Index == BiosIbb) {
        TopSwapControl (FALSE);
      }
    }

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UpdateBiosFirmware failed in stage %d (%r)\n", Index, Status));
      break;
    }

    BytesWritten += UpdateLength;
  }

  if (ConfigData != NULL) {
    FreePool (ConfigData);
  }

  DEBUG ((DEBUG_INFO, "PerformBiosUpdateByConfigData: exit (%r)\n", Status));
  return Status;
}

/**
  The constructor function of BiosUpdateHelperLib.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor successfully .

**/
EFI_STATUS
EFIAPI
BiosUpdateHelpersLibConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  mBiosGuardEnabled = IsBiosGuardEnabled ();

  return EFI_SUCCESS;
}
