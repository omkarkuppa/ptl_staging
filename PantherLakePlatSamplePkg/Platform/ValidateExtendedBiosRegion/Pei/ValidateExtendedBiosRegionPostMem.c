/** @file
  Extended BIOS Region validation support PEIM

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
#include <Library/PeiServicesLib.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Core/Pei/PeiMain.h>

/**
  Find GUIDs from the HOB for the firmware volume matching the base address
  which is decoded by EFI_PEI_HANDLE

  @param[in]   FvHandle     EFI Handle on the firmware volume
  @param[out]  FileName     FileName GUID for the firmware volume
  @param[out]  FvName       FvName GUID for the firmware volume

  @retval     EFI_SUCCESS   The GUID is found
  @retval     EFI_NOT_FOUND The GUID is not found
  @retval     Others        Returned values from PeiServicesFfsGetVolumeInfo
**/
EFI_STATUS
FindFvGuidFromHob (
  IN  EFI_PEI_FV_HANDLE FvHandle,
  OUT EFI_GUID          *FileName,
  OUT EFI_GUID          *FvName
  )
{
  EFI_STATUS             Status;
  EFI_PEI_HOB_POINTERS   Hob;
  EFI_PHYSICAL_ADDRESS   FvBase;
  EFI_FV_INFO            VolumeInfo;

  FvBase = 0;
  Status = PeiServicesFfsGetVolumeInfo (FvHandle, &VolumeInfo);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  FvBase = (EFI_PHYSICAL_ADDRESS) VolumeInfo.FvStart;

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

  @param[in]   FvHandle     EFI Handle on the firmware volume
  @param[out]  FileName     FileName GUID for the firmware volume
  @param[out]  FvName       FvName GUID for the firmware volume

  @retval     EFI_UNSUPPORTED   This function is not supported
**/
EFI_STATUS
FindFvGuid (
  IN  EFI_PEI_FV_HANDLE  FvHandle,
  OUT EFI_GUID           *FileName,
  OUT EFI_GUID           *FvName
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

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
ValidateExtendedBiosRegionPostMemEntrypoint (
  IN       EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  EFI_STATUS           Status;
  PEI_CORE_INSTANCE    *PrivateData;
  PEI_CORE_FV_HANDLE   *CoreFvHandle;
  EFI_GUID             FileGuid;
  EFI_GUID             FvName;

  DEBUG ((DEBUG_INFO, " =======================================================\n"));
  DEBUG ((DEBUG_INFO, " This PEIM resided in the FV for Extended BIOS Region \n"));
  DEBUG ((DEBUG_INFO, " Phase                :  PostMemory \n"));

  PrivateData = PEI_CORE_INSTANCE_FROM_PS_THIS (PeiServices);
  CoreFvHandle = &(PrivateData->Fv [PrivateData->CurrentPeimFvCount]);

  Status = FindFvGuidFromHob (CoreFvHandle->FvHandle, &FileGuid, &FvName);
  if (EFI_ERROR (Status)) {
    Status = FindFvGuid (CoreFvHandle->FvHandle, &FileGuid, &FvName);
  }

  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, " Actual   FV GUID     :  %g   \n",   &FvName));
    DEBUG ((DEBUG_INFO, " Expected FV GUID     :  %g   \n",   &gFvExtendedPostMemoryFvNameGuid));

    DEBUG ((DEBUG_INFO, " Actual   File GUID   :  %g   \n",   &FileGuid));
    DEBUG ((DEBUG_INFO, " Expected File GUID   :  %g   \n",   &gFvExtendedPostMemoryFileGuid));
  }
  DEBUG ((DEBUG_INFO, " =======================================================\n"));

  return EFI_SUCCESS;
}
