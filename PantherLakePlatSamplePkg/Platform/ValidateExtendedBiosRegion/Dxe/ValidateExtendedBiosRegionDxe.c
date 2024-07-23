/** @file
  Extended BIOS Region validation support DXE driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Uefi.h>
#include <Core/Dxe/DxeMain.h>
#include <Library/HobLib.h>


/**
  Find GUIDs from the HOB for the firmware volume matching the base address
  which is decoded by EFI_HANDLE

  @param[in]   Handle       EFI Handle on the firmware volume
  @param[out]  FileName     FileName GUID for the firmware volume
  @param[out]  FvName       FvName GUID for the firmware volume

  @retval     EFI_SUCCESS   The GUID is found
  @retval     EFI_NOT_FOUND The GUID is not found
**/
EFI_STATUS
FindFvGuidFromHob (
  IN  EFI_HANDLE Handle,
  OUT EFI_GUID   *FileName,
  OUT EFI_GUID   *FvName
  )
{
  EFI_STATUS                         Status;
  EFI_PEI_HOB_POINTERS               Hob;
  EFI_PHYSICAL_ADDRESS               FvBase;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *Fvb;

  FvBase = 0;
  Status = gBS->HandleProtocol (Handle, &gEfiFirmwareVolumeBlockProtocolGuid, (VOID **) &Fvb);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Fvb->GetPhysicalAddress (Fvb, &FvBase);
  } else {
    return EFI_NOT_FOUND;
  }

  Hob.Raw = GetHobList ();
  while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_FV3, Hob.Raw)) != NULL) {
    if (FvBase == Hob.FirmwareVolume3->BaseAddress) {
      CopyGuid (FileName, &Hob.FirmwareVolume3->FileName);
      CopyGuid (FvName,   &Hob.FirmwareVolume3->FvName);
      if (Hob.FirmwareVolume3->ExtractedFv) {
        DEBUG ((DEBUG_INFO, " This FV was extracted and saved in HOB \n"));
      }
      DEBUG ((DEBUG_INFO, " FV HOB Base %x \n", FvBase));
      return EFI_SUCCESS;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  return EFI_NOT_FOUND;
}

/**
  Find GUIDs for the firmware volume which has this driver

  @param[in]   Handle       EFI Handle on the firmware volume
  @param[out]  FileName     FileName GUID for the firmware volume
  @param[out]  FvName       FvName GUID for the firmware volume

  @retval     EFI_UNSUPPORTED   This function is not supported
**/
EFI_STATUS
FindFvGuid (
  IN  EFI_HANDLE  Handle,
  OUT EFI_GUID    *FileName,
  OUT EFI_GUID    *FvName
  )
{
  ///
  /// Intel RVP BIOS extracts FVs once, being saved in HOB.
  /// FindFvGuidFromHob () is expected to return the GUIDs
  /// for a current firmware volume.
  ///
  return EFI_UNSUPPORTED;
}


/**
  Entrypoint to validate Extended BIOS Region

  @param[in]  ImageHandle  Image Handle.
  @param[in]  SystemTable  EFI System Table.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
ValidateExtendedBiosRegionDxeEntrypoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                 Status;
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
  EFI_HANDLE                 Handle;
  EFI_GUID                   FileGuid;
  EFI_GUID                   FvName;

  DEBUG ((DEBUG_INFO, " =======================================================\n"));
  DEBUG ((DEBUG_INFO, " This driver resided in the FV for Extended BIOS Region \n"));
  DEBUG ((DEBUG_INFO, " Phase                :  DXE \n"));

  Handle = NULL;
  do {
    Status = gBS->HandleProtocol (ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **) &LoadedImage);
    ASSERT_EFI_ERROR (Status);
    if ((LOADED_IMAGE_PRIVATE_DATA_FROM_THIS (LoadedImage))->Type == EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE) {
      Handle = LoadedImage->DeviceHandle;
      break;
    }
    ImageHandle = LoadedImage->ParentHandle;
  } while (ImageHandle != NULL);

  Status = FindFvGuidFromHob (Handle, &FileGuid, &FvName);
  if (EFI_ERROR (Status)) {
    Status = FindFvGuid (Handle, &FileGuid, &FvName);
  }

  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, " Actual   FV GUID     :  %g   \n",   &FvName));
    DEBUG ((DEBUG_INFO, " Expected FV GUID     :  %g   \n",   &gFvExtendedAdvancedFvNameGuid));

    DEBUG ((DEBUG_INFO, " Actual   File GUID   :  %g   \n",   &FileGuid));
    DEBUG ((DEBUG_INFO, " Expected File GUID   :  %g   \n",   &gFvExtendedAdvancedFileGuid));
  }
  DEBUG ((DEBUG_INFO, " =======================================================\n"));

  return EFI_SUCCESS;
}
