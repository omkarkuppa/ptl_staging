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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <TxtInfoHob.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/ConfigProfileAccessLib.h>
#include <Library/CbntSvnCheckLib.h>
#include <Library/TopSwapSupportLib.h>
#include <Library/ResiliencySupportLib.h>
#include <Protocol/FirmwareManagement.h>

#include "AcmImageParser.h"

/**
  Check if the input Image contains expected files used for BIOS update.

  @param[in]   Image               Pointer to FMP payload image (FMP image header is stripped off)
  @param[in]   ImageSize           The size of Image in bytes

  @retval TRUE  The image contains expected files used for BIOS update.
  @retval FALSE The image does not contain expected files used for BIOS update.

**/
BOOLEAN
IsImageForBtGAcmUpdate (
  IN      VOID                          *Image,
  IN      UINTN                         ImageSize
  )
{
  EFI_STATUS                  Status;
  CONFIG_HEADER               ConfigHeader;
  UPDATE_CONFIG_DATA          *ConfigData;
  VOID                        *BtGAcmImage;
  UINTN                       BtGAcmImageSize;
  VOID                        *BgupImage;
  UINTN                       BgupImageSize;
  BOOLEAN                     BiosGuardEnabled;

  DEBUG ((DEBUG_INFO, "%a - start\n", __FUNCTION__));

  ConfigData = NULL;
  BiosGuardEnabled = IsBiosGuardEnabled ();
  //
  // Check if all needed binaries for BIOS update can be extracted from Image
  //
  Status = ExtractUpdateImageByConfigFile (
             Image,
             ImageSize,
             &gCapsuleBtGAcmConfigFileGuid,
             &ConfigHeader,
             &ConfigData,
             &BtGAcmImage,
             &BtGAcmImageSize,
             BiosGuardEnabled ? &BgupImage : NULL,
             BiosGuardEnabled ? &BgupImageSize : NULL
             );

  if (ConfigData != NULL) {
    //
    // No need to use ConfigData in this function anymore.
    //
    FreePool (ConfigData);
  }

  if (EFI_ERROR (Status) || \
      (!CompareGuid (&(ConfigHeader.FileGuid), &gCapsuleBtGAcmImageFileGuid))) {
    DEBUG ((DEBUG_ERROR, "Failed to extract config file or acm image\n"));
    return FALSE;
  }

  //
  // Check if input ACM image buffer is used on current platform CPU.
  //
  if (!IsSAcmImageForCurrentCpu (BtGAcmImage, BtGAcmImageSize)) {
    DEBUG ((DEBUG_ERROR, "ACM image is not for current CPU.\n"));
    return FALSE;
  }

#if FixedPcdGetBool (PcdCheckArbhSvnBeforeUpdate) == 1
  Status = CheckSvnFromSAcmImage ((UINT8 *) BtGAcmImage, BtGAcmImageSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SVN check fail\n"));
    return FALSE;
  }
#endif

  DEBUG ((DEBUG_INFO, "%a - pass\n", __FUNCTION__));
  return TRUE;
}

/**
  Get the Startup ACM base address.

**/
VOID *
GetBiosAcmBaseAddress (
  VOID
  )
{
  TXT_INFO_HOB  *TxtInfoHob;

  //
  // ACM image offset would be recorded within HOB information.
  //
  TxtInfoHob = (TXT_INFO_HOB *)GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    return NULL;
  }

  return (VOID *)TxtInfoHob->Data.BiosAcmBase;
}

/**
  Perform BtGAcm update in stages

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
PerformBtGAcmUpdateByConfigData (
  IN VOID                                           *Image,
  IN UINTN                                          ImageSize,
  IN UINT32                                         LastUpdatingProgress,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  DisplayProgress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS            Status;
  CONFIG_HEADER         ConfigHeader;
  UPDATE_CONFIG_DATA    *ConfigData;
  VOID                  *BiosImage;
  UINTN                 BiosImageSize;
  VOID                  *BgupImage;
  UINTN                 BgupImageSize;
  UINTN                 DisplayLength;
  UINTN                 Index;
  UINTN                 TotalUpdateSize;
  UINTN                 BytesWritten;
  EFI_PHYSICAL_ADDRESS  FlashAddress;
  VOID                  *UpdateBuffer;
  UINTN                 UpdateLength;
  VOID                  *BgupBuffer;
  UINTN                 BgupBufferLength;
  FLASH_ACCESS_REQUEST  AccessReq;
  BOOLEAN               BiosGuardEnabled;
  EFI_PHYSICAL_ADDRESS  BiosAreaBaseAddress;
  VOID                  *BiosAcmBaseAddress;

  DEBUG ((DEBUG_INFO, "PerformBtGAcmUpdateByConfigData: entry\n"));

  ConfigData       = NULL;
  BiosImage        = NULL;
  BiosImageSize    = 0;
  BgupImage        = NULL;
  BgupImageSize    = 0;

  BytesWritten     = 0;
  DisplayLength    = EndPercentage - StartPercentage;

  BiosGuardEnabled    = IsBiosGuardEnabled ();
  BiosAreaBaseAddress = (EFI_PHYSICAL_ADDRESS)((UINTN)FixedPcdGet32 (PcdBiosAreaBaseAddress));
  BiosAcmBaseAddress  = GetBiosAcmBaseAddress ();

  //
  // Get the current platform S-ACM base address.
  //
  if (BiosAcmBaseAddress == NULL) {
    DEBUG ((DEBUG_WARN, "Failed to get BIOS S-ACM Base Address\n"));
    return EFI_NOT_FOUND;
  }
  DEBUG ((DEBUG_INFO, "Get the BIOS ACM target address: 0x%X\n", BiosAcmBaseAddress));

  //
  // Extract BIOS image and BGUP image (if BiosGuard is enabled) by parsing Config file.
  //
  Status = ExtractUpdateImageByConfigFile (
             Image,
             ImageSize,
             &gCapsuleBtGAcmConfigFileGuid,
             &ConfigHeader,
             &ConfigData,
             &BiosImage,
             &BiosImageSize,
             BiosGuardEnabled ? &BgupImage : NULL,
             BiosGuardEnabled ? &BgupImageSize : NULL
             );

  if (EFI_ERROR (Status) || ConfigData == NULL) {
    DEBUG ((DEBUG_ERROR, "PerformBtGAcmUpdateByConfigData: Failed to ExtractUpdateImageByConfigFile (%r)\n", Status));
    return Status;
  }

  //
  // Calculate the total size to be written
  //
  TotalUpdateSize = 0;
  for (Index = 0; Index < ConfigHeader.NumOfUpdates; Index++) {
    TotalUpdateSize += ConfigData[Index].Length;
  }

  DEBUG ((DEBUG_INFO, "Update Boot Guard ACM region - Start\n"));

  Status = EFI_NOT_STARTED;

  for (Index = 0; Index < ConfigHeader.NumOfUpdates; Index++) {
    DEBUG ((DEBUG_INFO,
      "PerformBtGAcmUpdateByConfigData (stage %d): BaseAddress - 0x%lx ImageOffset - 0x%x Length - 0x%x\n",
      Index,
      ConfigData[Index].BaseAddress,
      ConfigData[Index].ImageOffset,
      ConfigData[Index].Length
      ));

    if ((BiosAreaBaseAddress + ConfigData[Index].BaseAddress) != ((EFI_PHYSICAL_ADDRESS)BiosAcmBaseAddress)) {
      DEBUG ((DEBUG_INFO, "Not target base address, skip this stage.\n"));
      continue;
    }

    if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      TopSwapControl (TRUE);
    }

    FlashAddress     = ConfigData[Index].BaseAddress;
    UpdateBuffer     = (VOID *)((UINTN) BiosImage + ConfigData[Index].ImageOffset);
    UpdateLength     = ConfigData[Index].Length;
    BgupBuffer       = (VOID *)((UINTN) BgupImage + ConfigData[Index].HelperImageOffset);
    BgupBufferLength = ConfigData[Index].HelperImageLength;

    ZeroMem (&AccessReq, sizeof (AccessReq));
    AccessReq.FlashRegionType = FlashRegionBios;
    AccessReq.FlashAddress    = FlashAddress;
    AccessReq.Data            = UpdateBuffer;
    AccessReq.DataSize        = UpdateLength;
    AccessReq.BgupImage       = BgupBuffer;
    AccessReq.BgupImageSize   = BgupBufferLength;

    Status = FlashUpdate (
               &AccessReq,
               DisplayProgress,
               StartPercentage + ((BytesWritten * DisplayLength) / TotalUpdateSize),                  // StartPercentage
               StartPercentage + (((BytesWritten + UpdateLength) * DisplayLength) / TotalUpdateSize)  // EndPercentage
               );

    if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      TopSwapControl (FALSE);
    }

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UpdateBiosFirmware failed in stage %d (%r)\n", Index, Status));
      break;
    }

    BytesWritten += UpdateLength;
  }

  DEBUG ((DEBUG_INFO, "Update Boot Guard ACM region - Done\n"));

  if (ConfigData != NULL) {
    FreePool (ConfigData);
  }

  DEBUG ((DEBUG_INFO, "PerformBtGAcmUpdateByConfigData: exit (%r)\n", Status));
  return Status;
}
