/** @file
  Library instance provide common functions that can be shared among different
  FmpDeviceLib instances especially for the instance which involves Fsp and/or
  multiple components update.

  The major functionalitise are:
  1. Extract files from a given image
  2. Parse config ini file
  3. Perform Fsp update by config data with/without BiosGuard BGUP image

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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/ResiliencySupportLib.h>
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/ConfigProfileAccessLib.h>
#include <Library/CbntSvnCheckLib.h>
#include <Library/TopSwapSupportLib.h>
#include <Library/BootGuardLib.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>

#include <Protocol/FirmwareManagement.h>
#include "FspBootManifest.h"

/**
  Parse FIT and get FBM base address.

  @retval FbmBaseAddress  FBM base address.

**/
UINTN
GetFbmBase (
  VOID
  )
{
  UINT64                         FitPointer;
  FIRMWARE_INTERFACE_TABLE_ENTRY *FitEntry;
  UINT32                         EntryNum;
  UINT32                         Index;
  UINTN                          FbmBaseAddress;

  FitPointer     = *(UINT64 *)(UINTN)FIT_POINTER_ADDRESS;
  FitEntry       = (FIRMWARE_INTERFACE_TABLE_ENTRY *)(UINTN)FitPointer;
  EntryNum       = (((UINT32)FitEntry[0].Size[2] << 16) |
                   ((UINT32)FitEntry[0].Size[1] << 8) |
                   (UINT32)FitEntry[0].Size[0]) & 0xFFFFFF;
  FbmBaseAddress = 0;

  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == FIT_TYPE_0D_FSP_BOOT_MANIFEST) {
      FbmBaseAddress = (UINTN)(FitEntry[Index].Address & MAX_UINT32);
      DEBUG ((DEBUG_INFO, "FBM BaseAddress 0x%x\n", FbmBaseAddress));
      break;
    }
  }

  return FbmBaseAddress;
}

/**
  Check if current BIOS supports FSP signed by checking FBM header.

  @retval TRUE   BIOS supports FSP signed.
  @retval FALSE  BIOS doesn't support FSP signed.

**/
BOOLEAN
IsBiosSupportFspSigned (
  VOID
  )
{
  FBM_HEADER *FbmHeader;

  FbmHeader = (FBM_HEADER *)(GetFbmBase ());

  if (FbmHeader == NULL) {
    return FALSE;
  }

  if (*(UINT64 *)(FbmHeader->StructureId) != FBM_STRUCTURE_ID) {
    return FALSE;
  }

  return TRUE;
}

/**
  Check if this is VerifiedBoot

  @retval TRUE   This is VerifiedBoot
  @retval FALSE  This is NOT VerifiedBoot

**/
BOOLEAN
IsVerifiedBoot (
  VOID
  )
{
  if ((AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check if the input Image contains expected files used for Fsp update.

  @param[in]   Image        Pointer to FMP payload image (FMP image header is stripped off)
  @param[in]   ImageSize    The size of Image in bytes

  @retval TRUE              The image contains expected files used for Fsp update.
  @retval FALSE             The image does not contain expected files used for Fsp update.

**/
BOOLEAN
IsImageForFspUpdate (
  IN  VOID    *Image,
  IN  UINTN   ImageSize
  )
{
  EFI_STATUS            Status;
  CONFIG_HEADER         ConfigHeader;
  UPDATE_CONFIG_DATA    *ConfigData;
  VOID                  *FspImage;
  UINTN                 FspImageSize;
  VOID                  *BgupImage;
  UINTN                 BgupImageSize;
  BOOLEAN               BiosGuardEnabled;

  ConfigData = NULL;
  BiosGuardEnabled = IsBiosGuardEnabled ();

  //
  // Check if all needed binaries for Fsp update can be extracted from Image
  //
  Status = ExtractUpdateImageByConfigFile (
             Image,
             ImageSize,
             &gCapsuleFspConfigFileGuid,
             &ConfigHeader,
             &ConfigData,
             &FspImage,
             &FspImageSize,
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
      (!CompareGuid (&(ConfigHeader.FileGuid), &gCapsuleFspImageFileGuid))) {
    DEBUG ((DEBUG_ERROR, "Failed to extract config file or acm image\n"));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "%a - pass\n", __FUNCTION__));
  return TRUE;
}

/**
  Perform Fsp update in stages.

  The input Image contains multiple FFS files including config data file
  and Fsp image file. This function extracts and parse config file first,
  then extracts Fsp and BGUP images described in config header.
  Finally update Fsp region by stage according to the address described in
  config data.
  This function povides the capability to support fault tolerant Fsp update
  by recording update progress and switching BootBlock region accordingly between
  update stages.

  @param[in] Image                   Pointer to FMP payload image (FMP image header is stripped off)
  @param[in] ImageSize               The size of Image in bytes
  @param[in] LastUpdatingProgress    Indicates the update progress if Fsp update got interrupted last time.
  @param[in] DisplayProgress         A function used to display the progress of the firmware update.
                                     This is an optional parameter that may be NULL.
  @param[in] StartPercentage         The start completion percentage value that may be used to report
                                     progress during the flash write operation.
  @param[in] EndPercentage           The end completion percentage value that may be used to report
                                     progress during the flash write operation.

  @retval EFI_SUCCESS                Fsp update is successfully performed.
  @retval Other                      Fsp update fails.

**/
EFI_STATUS
PerformFspUpdateByConfigData (
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
  VOID                                  *FspImage;
  UINTN                                 FspImageSize;
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
  FLASH_ACCESS_REQUEST                  AccessReq;
  BOOLEAN                               BiosGuardEnabled;

  DEBUG ((DEBUG_INFO, "PerformFspUpdateByConfigData: entry\n"));

  ConfigData       = NULL;
  FspImage        = NULL;
  FspImageSize    = 0;
  BgupImage        = NULL;
  BgupImageSize    = 0;

  BytesWritten     = 0;
  DisplayLength    = EndPercentage - StartPercentage;

  BiosGuardEnabled = IsBiosGuardEnabled ();

  //
  // Extract Fsp image and BGUP image (if BiosGuard is enabled) by parsing Config file.
  //
  Status = ExtractUpdateImageByConfigFile (
             Image,
             ImageSize,
             &gCapsuleFspConfigFileGuid,
             &ConfigHeader,
             &ConfigData,
             &FspImage,
             &FspImageSize,
             BiosGuardEnabled ? &BgupImage : NULL,
             BiosGuardEnabled ? &BgupImageSize : NULL
             );

  if (EFI_ERROR (Status) || ConfigData == NULL) {
    DEBUG ((DEBUG_ERROR, "PerformFspUpdateByConfigData: Failed to ExtractUpdateImageByConfigFile (%r)\n", Status));
    return Status;
  }

  //
  // Calculate the total size to be written
  //
  TotalUpdateSize = 0;
  for (Index = 0; Index < ConfigHeader.NumOfUpdates; Index++) {
    TotalUpdateSize += ConfigData[Index].Length;
  }

  DEBUG ((DEBUG_INFO, "Update Fsp region - Start\n"));

  if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
    TopSwapControl (TRUE);
  }

  for (Index = 0; Index < ConfigHeader.NumOfUpdates; Index++) {
    DEBUG ((DEBUG_INFO,
      "PerformFspUpdateByConfigData (stage %d): BaseAddress - 0x%lx ImageOffset - 0x%x Length - 0x%x\n",
      Index,
      ConfigData[Index].BaseAddress,
      ConfigData[Index].ImageOffset,
      ConfigData[Index].Length
      ));

    FlashAddress     = ConfigData[Index].BaseAddress;
    UpdateBuffer     = (VOID *)((UINTN) FspImage + ConfigData[Index].ImageOffset);
    UpdateLength     = ConfigData[Index].Length;
    BgupBuffer       = (VOID *)((UINTN) BgupImage + ConfigData[Index].HelperImageOffset);
    BgupBufferLength = ConfigData[Index].HelperImageLength;

    ZeroMem (&AccessReq, sizeof (AccessReq));
    AccessReq.FlashRegionType = FlashRegionBios;
    AccessReq.FlashAddress = FlashAddress;
    AccessReq.Data = UpdateBuffer;
    AccessReq.DataSize = UpdateLength;
    AccessReq.BgupImage = BgupBuffer;
    AccessReq.BgupImageSize = BgupBufferLength;
    Status = FlashUpdate (
               &AccessReq,
               DisplayProgress,
               StartPercentage + ((BytesWritten * DisplayLength) / TotalUpdateSize),                  // StartPercentage
               StartPercentage + (((BytesWritten + UpdateLength) * DisplayLength) / TotalUpdateSize)  // EndPercentage
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UpdateFspFirmware failed in stage %d (%r)\n", Index, Status));
      break;
    }

    BytesWritten += UpdateLength;
  }

  if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
    TopSwapControl (FALSE);
  }

  if (ConfigData != NULL) {
    FreePool (ConfigData);
  }

  DEBUG ((DEBUG_INFO, "PerformFspUpdateByConfigData: exit (%r)\n", Status));
  return Status;
}
