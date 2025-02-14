/** @file
  Platform recovery library implementation.

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

#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Ppi/RecoveryModule.h>
#include <Ppi/DeviceRecoveryModule.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Guid/SysFwUpdateProgress.h>
#include <Library/HashSupportLib.h>
#include <Library/PayloadResiliencySupportLib.h>

EFI_STATUS
EFIAPI
PlatformRecoveryModule (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN EFI_PEI_RECOVERY_MODULE_PPI *This
  );

/**
  A dummy function to serve LoadRecoveryCapsule PPI and indicate that a recovery capsule has been found.

  @param  PeiServices   General-purpose services that are available to every PEIM.
  @param  This          Indicates the EFI_PEI_RECOVERY_MODULE_PPI instance.

  @retval EFI_SUCCESS        The capsule was loaded correctly.

**/
EFI_STATUS
EFIAPI
DummyPlatformRecoveryModule (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN EFI_PEI_RECOVERY_MODULE_PPI *This
  );

static EFI_PEI_RECOVERY_MODULE_PPI mRecoveryPpi = {
  PlatformRecoveryModule
};

static EFI_PEI_RECOVERY_MODULE_PPI mDummyRecoveryPpi = {
  DummyPlatformRecoveryModule
};

static EFI_PEI_PPI_DESCRIPTOR mRecoveryPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiRecoveryModulePpiGuid,
  &mRecoveryPpi
};

static EFI_PEI_PPI_DESCRIPTOR mDummyRecoveryPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiRecoveryModulePpiGuid,
  &mDummyRecoveryPpi
};

/**
  Get recovery image from external storage.

  @param[in]  PeiServices  General purpose services available to every PEIM.
  @param[out] Buffer       Point to the recovery image buffer.
  @param[out] BufferSize   Size of the recovery image buffer.

  @retval     Status       EFI_SUCCESS if the recovery image is successfully found.

**/
EFI_STATUS
GetRecoveryImage (
  IN EFI_PEI_SERVICES               **PeiServices,
  OUT VOID                          **Buffer,
  OUT UINTN                         *BufferSize
  )
{
  EFI_STATUS                             Status;
  EFI_PEI_DEVICE_RECOVERY_MODULE_PPI     *DeviceRecoveryModule;
  UINTN                                  NumberOfImageProviders;
  BOOLEAN                                ProviderAvailable;
  UINTN                                  NumberRecoveryCapsules;
  UINTN                                  RecoveryCapsuleSize;
  EFI_GUID                               DeviceId;
  EFI_PHYSICAL_ADDRESS                   Address;
  UINTN                                  CapsuleInstance;

  Status = EFI_SUCCESS;

  ProviderAvailable = TRUE;
  NumberRecoveryCapsules = 0;
  NumberOfImageProviders = 0;
  DeviceRecoveryModule = NULL;

  DEBUG ((DEBUG_INFO | DEBUG_LOAD, "GetRecoveryImage Entry\n"));

  //
  // Search the platform for some recovery capsule if the DXE IPL
  // discovered a recovery condition and has requested a load.
  //
  while (ProviderAvailable) {
    Status = PeiServicesLocatePpi (
               &gEfiPeiDeviceRecoveryModulePpiGuid,
               NumberOfImageProviders,
               NULL,
               (VOID **) &DeviceRecoveryModule
               );
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO | DEBUG_LOAD, "Device Recovery PPI located\n"));
      NumberOfImageProviders++;
      Status = DeviceRecoveryModule->GetNumberRecoveryCapsules (
                                       PeiServices,
                                       DeviceRecoveryModule,
                                       &NumberRecoveryCapsules
                                       );

      DEBUG ((DEBUG_INFO | DEBUG_LOAD, "Number Of Recovery Capsules: %d\n", NumberRecoveryCapsules));

      if (NumberRecoveryCapsules != 0) {
        break;
      }
    } else {
      ProviderAvailable = FALSE;
    }
  }

  //
  // If there is an image provider, get the capsule ID
  //
  if (!ProviderAvailable) {
    return Status;
  }

  for (CapsuleInstance = 1; CapsuleInstance <= NumberRecoveryCapsules; CapsuleInstance++) {

    RecoveryCapsuleSize = 0;
    DEBUG ((DEBUG_INFO, "before GetRecoveryCapsuleInfo \n"));

    Status = DeviceRecoveryModule->GetRecoveryCapsuleInfo (
                                     PeiServices,
                                     DeviceRecoveryModule,
                                     CapsuleInstance,
                                     &RecoveryCapsuleSize,
                                     &DeviceId
                                     );

    if (EFI_ERROR (Status)) {
      continue;
    }

    DEBUG ((DEBUG_INFO | DEBUG_LOAD, "Recovery Capsule Size: %d\n", RecoveryCapsuleSize));

    *Buffer = NULL;
    Status = PeiServicesAllocatePages (
               EfiBootServicesCode,
               EFI_SIZE_TO_PAGES (RecoveryCapsuleSize),
               &Address
               );

    DEBUG ((DEBUG_INFO | DEBUG_LOAD, "AllocatePage Returns: %r\n", Status));

    if (EFI_ERROR (Status)) {
      continue;
    }

    *Buffer = (UINT8*)(UINTN) Address;
    Status = DeviceRecoveryModule->LoadRecoveryCapsule (
                                     PeiServices,
                                     DeviceRecoveryModule,
                                     CapsuleInstance,
                                     *Buffer
                                     );

    DEBUG ((DEBUG_INFO | DEBUG_LOAD, "LoadRecoveryCapsule Returns: %r\n", Status));

    if (EFI_ERROR (Status)) {
      ZeroMem ((VOID*)(UINTN) Address, EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (RecoveryCapsuleSize)));
      continue;
    }

    *BufferSize = RecoveryCapsuleSize;
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

/**
  Cheack if ME Recovery is required.

  @retval  TRUE   ME Recovery is required.
  @retval  FALSE  ME Recovery is not required.
**/
BOOLEAN
IsMeRecoveryRequired (
  VOID
  )
{
  EFI_HOB_GUID_TYPE                      *GuidHob;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS        *UpdateProgress;

  GuidHob = GetFirstGuidHob (&gSysFwUpdateProgressGuid);
  if (GuidHob != NULL) {
    UpdateProgress = (SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob);
    ASSERT (UpdateProgress->Component < UpdatingTypeMax);
    return (UpdateProgress->Component == UpdatingMeResiliency);
  }
  return FALSE;
}

/**
  Get capsule image from external storage and build a Capsule HOB to pass to DXE.

  @param[in]  PeiServices  General purpose services available to every PEIM.

  @retval     Status       EFI_SUCCESS if the capsule image is successfully found.

**/
EFI_STATUS
LoadCapsuleImage (
  IN EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                       Status;
  UINTN                            RecoveryCapsuleSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *PeiReadOnlyVarPpi;
  UINTN                            VarSize;
  UINT8                            *HashDigestVar;
  VOID                             *Buffer;
  CHAR16                           *String;
  UINTN                            StringSize;

  Status            = EFI_SUCCESS;
  PeiReadOnlyVarPpi = NULL;

  //
  // Load ME Recovery capsule if ME Recovery is required
  //
  if (IsMeRecoveryRequired ()) {
    String = SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME;
    StringSize = StrnSizeS (String, 200);
    PcdSetPtrS (PcdRecoveryFileName, &StringSize, String);
    Status = GetRecoveryImage (PeiServices, &Buffer, &RecoveryCapsuleSize);
    DEBUG ((DEBUG_INFO, "Loading ME Recovery Capsule image file %s (%r)\n", (CHAR16 *)PcdGetPtr (PcdRecoveryFileName), Status));
    if (!EFI_ERROR (Status)) {
      BuildCvHob ((UINTN) Buffer, RecoveryCapsuleSize);
      DEBUG ((DEBUG_INFO | DEBUG_LOAD, " ME Recovery Capsule image address(0x%08x), Size:(0x%08x)\n", Buffer, RecoveryCapsuleSize));
    }
    return Status;
  }
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &PeiReadOnlyVarPpi
             );

  ASSERT_EFI_ERROR (Status);

  //
  // Load Capsule image digest from NV variable
  //
  VarSize       = CAPSULE_HASH_DIGEST_SIZE;
  HashDigestVar = AllocateZeroPool (CAPSULE_HASH_DIGEST_SIZE);
  if (HashDigestVar == NULL) {
    ASSERT_EFI_ERROR (Status);
    return EFI_OUT_OF_RESOURCES;
  }
  Status = PeiReadOnlyVarPpi->GetVariable (
                                PeiReadOnlyVarPpi,
                                SYSFW_UPDATE_CAPSULE_DIGEST_VARIABLE_NAME,
                                &gSysFwUpdateProgressGuid,
                                NULL,
                                &VarSize,
                                HashDigestVar
                                );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Find Capsule digest from variable:\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "Cannot find Capsule digest variable.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Load Capsule image
  //
  String = SYSFW_UPDATE_CAPSULE_BACKUP_FILE_NAME;
  StringSize = StrnSizeS (String, 200);
  PcdSetPtrS (PcdRecoveryFileName, &StringSize, String);
  Status = GetRecoveryImage (PeiServices, &Buffer, &RecoveryCapsuleSize);
  DEBUG ((DEBUG_INFO, "Loading Capsule image file %s (%r)\n", (CHAR16 *)PcdGetPtr (PcdRecoveryFileName), Status));

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Examine the capsule hash value
  //
  Status = ExamineHashShaDigest (
             (UINT8 *)Buffer,
             RecoveryCapsuleSize,
             HashDigestVar,
             CAPSULE_HASH_DIGEST_SIZE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Capsule image loaded from media is corrupted.\n"));
    return EFI_CRC_ERROR;
  }

  BuildCvHob ((UINTN) Buffer, RecoveryCapsuleSize);
  DEBUG ((DEBUG_INFO | DEBUG_LOAD, "Capsule module address(0x%08x), Size:(0x%08x)\n", Buffer, RecoveryCapsuleSize));

  //
  // Load UX Capsule image if found
  //
  String = SYSFW_UPDATE_WINDOWS_UX_CAPSULE_FILE_NAME;
  StringSize = StrnSizeS (String, 200);
  PcdSetPtrS (PcdRecoveryFileName, &StringSize, String);
  Status = GetRecoveryImage (PeiServices, &Buffer, &RecoveryCapsuleSize);
  DEBUG ((DEBUG_INFO, "Loading Windows UX Capsule image file %s (%r)\n", (CHAR16 *)PcdGetPtr (PcdRecoveryFileName), Status));
  if (!EFI_ERROR (Status)) {
    BuildCvHob ((UINTN) Buffer, RecoveryCapsuleSize);
    DEBUG ((DEBUG_INFO | DEBUG_LOAD, "Windows UX Capsule module address(0x%08x), Size:(0x%08x)\n", Buffer, RecoveryCapsuleSize));
  }

  return EFI_SUCCESS;
}

/**
  Extract FV image of FvGuid within input image FdStart.

  @param[in]   FdStart          The input image.
  @param[in]   FdSize           The size of input image in bytes.
  @param[in]   TargetFvGuid     FvNameGuid of the FV to be searched.
  @param[out]  TargetFvStart    The image pointer to the target FV.
  @param[out]  TargetFvSize     The size of the target FV.

  @retval TRUE  The FV image is found.
  @retval FALSE The FV image is not found.

**/
BOOLEAN
FindFvInImage (
  IN VOID                  *FdStart,
  IN UINTN                 FdSize,
  IN EFI_GUID              *TargetFvGuid,
  OUT VOID                 **TargetFvStart,
  OUT UINTN                *TargetFvSize
  )
{
  UINTN                                     FvSize;
  EFI_FIRMWARE_VOLUME_HEADER                *FvHeader;
  EFI_FIRMWARE_VOLUME_EXT_HEADER            *FvExtHeader;
  BOOLEAN                                   FvFound;

  DEBUG ((DEBUG_INFO, "FindFvInObbImage: 0x%08x - 0x%08x\n", (UINTN)FdStart, (UINTN)FdSize));

  FvFound = FALSE;
  *TargetFvStart = NULL;
  *TargetFvSize = 0;

  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FdStart;
  while ((UINTN)FvHeader < (UINTN)FdStart + FdSize - 1) {
    FvSize = (UINTN)FdStart + FdSize - (UINTN)FvHeader;

    if (FvHeader->Signature != EFI_FVH_SIGNATURE) {
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)((UINTN)FvHeader + SIZE_4KB);
      continue;
    }
    DEBUG ((DEBUG_INFO, "Checking FV....0x%08x - 0x%x\n", FvHeader, FvHeader->FvLength));
    FvFound = TRUE;
    if (FvHeader->FvLength > FvSize) {
      DEBUG ((DEBUG_ERROR, "Invalid FV - FvSize: 0x%08x, MaxSize - 0x%08x\n", (UINTN)FvHeader->FvLength, (UINTN)FvSize));
      break;
    }
    FvSize = (UINTN)FvHeader->FvLength;

    //
    // Must be a FV with ExtHeader to have FvNameGuid
    //
    if (FvHeader->ExtHeaderOffset != 0) {
      FvExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *) ((UINT8 *) FvHeader + FvHeader->ExtHeaderOffset);
      if (CompareGuid (&FvExtHeader->FvName, TargetFvGuid)) {
        *TargetFvStart = (VOID *) FvHeader;
        *TargetFvSize = FvSize;
        DEBUG ((DEBUG_INFO, "Found FV which has FvNameGuid (%g)....exit\n", TargetFvGuid));
        return TRUE;
      }
    }
    //
    // Next FV
    //
    FvHeader = (VOID *)(UINTN)((UINTN)FvHeader + FvHeader->FvLength);
  }

  if (!FvFound) {
    DEBUG ((DEBUG_ERROR, "NO FV Found in image\n"));
  }

  DEBUG ((DEBUG_ERROR, "FindFvInObbImage - Target FV Not Found\n"));
  return FALSE;
}

/**
  Extract and install required FVs from ObbImageBuffer.
  The integrity of ObbImageBuffer must be examined before invoking this function.

  @param[in]   ObbImageBuffer   The input image.
  @param[in]   ObbImageSize     The size of input image in bytes.

  @retval Status   If required FV are successfully installed.
**/
EFI_STATUS
ReportPostMemFvInObbBuffer (
  IN VOID             *ObbImageBuffer,
  IN UINTN            ObbImageSize
  )
{
  EFI_GUID                *FvGuid;
  UINTN                   FvCount;
  UINTN                   Index;
  VOID                    *FvImage;
  UINTN                   FvImageSize;

  if ((ObbImageBuffer == NULL) || (ObbImageSize == 0)) {
    DEBUG ((DEBUG_ERROR, "ObbImageBuffer is required\n"));
    return EFI_INVALID_PARAMETER;
  }

  FvGuid  = PcdGetPtr (PcdObbRecoveryRequiredFvGuid);
  FvCount = PcdGetSize (PcdObbRecoveryRequiredFvGuid) / sizeof (EFI_GUID);

  for (Index = 0; Index < FvCount; Index++, FvGuid++) {
    if (FindFvInImage (ObbImageBuffer, ObbImageSize, FvGuid, &FvImage, &FvImageSize)) {
      DEBUG ((DEBUG_INFO, "Install FV %g - 0x%x, 0x%x\n", FvGuid, FvImage, FvImageSize));
      PeiServicesInstallFvInfo2Ppi (
        &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) FvImage)->FileSystemGuid),
        FvImage,
        (UINT32) FvImageSize,
        NULL,
        NULL,
        0
        );
    } else {
      DEBUG ((DEBUG_ERROR, "Fv %g is not found in recovery image\n", FvGuid));
      return EFI_NOT_FOUND;
    }
  }

  return EFI_SUCCESS;
}

/**
  Extract and install required FVs from NonFitPayloadBuffer.
  The integrity of NonFitPayloadBuffer must be examined before invoking this function.

  @param[in]   NonFitPayloadBuffer   The input image.
  @param[in]   NonFitPayloadSize     The size of input image in bytes.

  @retval Status   If required FV are successfully installed.
**/
EFI_STATUS
ReportPostMemFvInNonFitPayloadBuffer (
  IN VOID             *NonFitPayloadBuffer,
  IN UINTN            NonFitPayloadSize
  )
{
  EFI_GUID                *FvGuid;
  UINTN                   FvCount;
  UINTN                   Index;
  VOID                    *FvImage;
  UINTN                   FvImageSize;

  if ((NonFitPayloadBuffer == NULL) || (NonFitPayloadSize == 0)) {
    DEBUG ((DEBUG_ERROR, "NonFitPayloadBuffer is required\n"));
    return EFI_INVALID_PARAMETER;
  }

  FvGuid  = PcdGetPtr (PcdNonFitPayloadRecoveryRequiredFvGuid);
  FvCount = PcdGetSize (PcdNonFitPayloadRecoveryRequiredFvGuid) / sizeof (EFI_GUID);

  for (Index = 0; Index < FvCount; Index++, FvGuid++) {
    if (FindFvInImage (NonFitPayloadBuffer, NonFitPayloadSize, FvGuid, &FvImage, &FvImageSize)) {
      DEBUG ((DEBUG_INFO, "Install FV %g - 0x%x, 0x%x\n", FvGuid, FvImage, FvImageSize));
      PeiServicesInstallFvInfo2Ppi (
        &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) FvImage)->FileSystemGuid),
        FvImage,
        (UINT32) FvImageSize,
        NULL,
        NULL,
        0
        );
    } else {
      DEBUG ((DEBUG_ERROR, "Fv %g is not found in recovery image\n", FvGuid));
      return EFI_NOT_FOUND;
    }
  }

  return EFI_SUCCESS;
}

/**
  A dummy function to serve LoadRecoveryCapsule PPI and indicate that a recovery capsule has been found.

  @param  PeiServices   General-purpose services that are available to every PEIM.
  @param  This          Indicates the EFI_PEI_RECOVERY_MODULE_PPI instance.

  @retval EFI_SUCCESS        The capsule was loaded correctly.

**/
EFI_STATUS
EFIAPI
DummyPlatformRecoveryModule (
  IN EFI_PEI_SERVICES               **PeiServices,
  EFI_PEI_RECOVERY_MODULE_PPI       *This
  )
{
  DEBUG ((DEBUG_INFO | DEBUG_LOAD, "PlatformRecoveryModule image has been found. Return directly.\n"));
  return EFI_SUCCESS;
}
/**
  Loads a DXE capsule from some media into memory and updates the HOB table with the DXE firmware volume information.
  If system firmware update is in progress, also loads Capsule image from media and create Capsule HOB to continue
  the update process.
  The integrity of Capsule image is checked by message digest.

  @param  PeiServices   General-purpose services that are available to every PEIM.
  @param  This          Indicates the EFI_PEI_RECOVERY_MODULE_PPI instance.

  @retval EFI_SUCCESS        The capsule was loaded correctly.
  @retval Outhers            A recovery DXE capsule cannot be found.

**/
EFI_STATUS
EFIAPI
PlatformRecoveryModule (
  IN EFI_PEI_SERVICES               **PeiServices,
  EFI_PEI_RECOVERY_MODULE_PPI       *This
  )
{
  EFI_STATUS                       Status;
  VOID                             *RecoveryBuffer;
  UINTN                            RecoveryBufferSize;
  CHAR16                           *String;
  UINTN                            StringSize;
  EFI_HOB_GUID_TYPE                *GuidHob;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS  *UpdateProgress;
  EFI_PEI_FILE_HANDLE              DigestFileHandle;
  VOID                             *DigestFileBuffer;
  BOOLEAN                          InstallObbFvsFromRecoveryImage;

  DEBUG ((DEBUG_INFO | DEBUG_LOAD, "PlatformRecoveryModule Entry\n"));

  GuidHob            = NULL;
  UpdateProgress     = NULL;
  RecoveryBuffer     = NULL;
  RecoveryBufferSize = 0;
  String             = NULL;
  StringSize         = 0;
  DigestFileHandle   = NULL;
  DigestFileBuffer   = NULL;
  Status             = EFI_SUCCESS;

  InstallObbFvsFromRecoveryImage = FALSE;

  GuidHob = GetFirstGuidHob (&gBiosInfoRecoveryGuid);
  if (GuidHob != NULL) {
    //
    // gBiosInfoRecoveryGuid GuidHob exists, Obb would be loaded from external storage and examined.
    //
    InstallObbFvsFromRecoveryImage = TRUE;
  }

  GuidHob = GetFirstGuidHob (&gSysFwUpdateProgressGuid);
  if (GuidHob != NULL) {
    UpdateProgress = (SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob);
  }

  //
  // Load Obb file from external storage and install FVs.
  //
  if (InstallObbFvsFromRecoveryImage) {
    DEBUG ((DEBUG_INFO, "InstallObbFvsFromRecoveryImage\n"));
    //
    // Load Obb digest file on ROM (PreMem FV) first for further check.
    //
    Status = PeiServicesFfsFindFileByName (&gObbSha384HashFileGuid, (EFI_PEI_FV_HANDLE)(UINTN) FixedPcdGet32 (PcdFlashFvPreMemoryBase), &DigestFileHandle);
    if (!EFI_ERROR (Status)) {
      //
      // Search RAW section.
      //
      Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, DigestFileHandle, &DigestFileBuffer);
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Find OBB digest on ROM\n"));
      }
    }

    if (EFI_ERROR (Status) || (DigestFileBuffer == NULL)) {
      DEBUG ((DEBUG_ERROR, "Failed to located Obb digest file on ROM\n"));
      return EFI_NOT_FOUND;
    }

    //
    // Decide Recovery filename to be loaded. Load Obb.bin by default
    //
    String = SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME;

    if ((UpdateProgress != NULL) && \
        ((UpdateProgress->Component == UpdatingBios) && (UpdateProgress->Progress >= BiosIbb))) {
      //
      // Ibb has been updated. Load corresponding new ObbN.bin.
      //
      String = SYSFW_UPDATE_NEW_OBB_BACKUP_FILE_NAME;
    }

    //
    // Search Obb recovery file from external storage media
    //
    StringSize = StrnSizeS (String, 200);
    PcdSetPtrS (PcdRecoveryFileName, &StringSize, String);
    Status = GetRecoveryImage (PeiServices, &RecoveryBuffer, &RecoveryBufferSize);
    DEBUG ((DEBUG_INFO, "Loading Obb recovery file %s (%r)\n", (CHAR16 *)PcdGetPtr (PcdRecoveryFileName), Status));
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // Examine OBB file digest from media
    //
    Status = ExamineHashShaDigest (
             (UINT8 *)RecoveryBuffer,
             RecoveryBufferSize,
             (UINT8 *)DigestFileBuffer,
             BACKUP_FILE_HASH_DIGEST_SIZE
             );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "OBB file loaded from media is corrupted.\n"));
      return Status;
    }

    //
    // Report FVs in Obb image
    //
    Status = ReportPostMemFvInObbBuffer (RecoveryBuffer, RecoveryBufferSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to install required FVs from Obb recovery file.\n"));
      return Status;
    }

    DEBUG ((DEBUG_INFO, "Required FVs from Obb recovery file are installed.\n"));

    if (IsPayloadBackupEnabled ()) {
      //
      // Load NonFitPayloads digest file on ROM (PreMem FV) first for further check.
      //
      Status = PeiServicesFfsFindFileByName (&gPayloadSha384HashFileGuid, (EFI_PEI_FV_HANDLE)(UINTN) FixedPcdGet32 (PcdFlashFvPreMemoryBase), &DigestFileHandle);
      if (!EFI_ERROR (Status)) {
        //
        // Search RAW section.
        //
        Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, DigestFileHandle, &DigestFileBuffer);
        if (!EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO, "Find NonFitPayloads digest on ROM\n"));
        }
      }

      if (EFI_ERROR (Status) || (DigestFileBuffer == NULL)) {
        DEBUG ((DEBUG_ERROR, "Failed to located NonFitPayloads digest file on ROM\n"));
        return EFI_NOT_FOUND;
      }

      //
      // Decide Recovery filename to be loaded. Load NonFitPayload.bin by default
      //
      String = SYSFW_UPDATE_CURRENT_PAYLOAD_BACKUP_FILE_NAME;

      if ((UpdateProgress != NULL) && \
          ((UpdateProgress->Component == UpdatingBios) && (UpdateProgress->Progress >= BiosIbb))) {
        //
        // Ibb has been updated. Load corresponding new NonFitPayloadN.bin.
        //
        String = SYSFW_UPDATE_NEW_PAYLOAD_BACKUP_FILE_NAME;
      }

      //
      // Search NonFitPayload recovery file from external storage media
      //
      StringSize = StrnSizeS (String, 200);
      PcdSetPtrS (PcdRecoveryFileName, &StringSize, String);
      Status = GetRecoveryImage (PeiServices, &RecoveryBuffer, &RecoveryBufferSize);
      DEBUG ((DEBUG_INFO, "Loading NonFitPayload recovery file %s (%r)\n", (CHAR16 *)PcdGetPtr (PcdRecoveryFileName), Status));
      if (EFI_ERROR (Status)) {
        return Status;
      }

      //
      // Examine NonFitPayload file digest from media
      //
      Status = ExamineHashShaDigest (
              (UINT8 *)RecoveryBuffer,
              RecoveryBufferSize,
              (UINT8 *)DigestFileBuffer,
              BACKUP_FILE_HASH_DIGEST_SIZE
              );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "NonFitPayload file loaded from media is corrupted.\n"));
        return Status;
      }

      //
      // Report FVs in NonFitPayload image
      //
      Status = ReportPostMemFvInNonFitPayloadBuffer (RecoveryBuffer, RecoveryBufferSize);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to install required FVs from NonFitPayload recovery file.\n"));
        return Status;
      }

      DEBUG ((DEBUG_INFO, "Required FVs from NonFitPayload recovery file are installed.\n"));
    }

  } else {
    DEBUG ((DEBUG_INFO, "Obb Fvs should already be installed in ReportPostMemFv\n"));
  }

  if ((UpdateProgress != NULL) && (UpdateProgress->Component != UpdatingResiliency)) {
    DEBUG ((DEBUG_INFO, "Loading Capsule image to continue an interrupted update\n"));
    Status = LoadCapsuleImage (PeiServices);
  }

  if (Status == EFI_SUCCESS) {
    //
    // Since recovery file has been found. Reinstall RecoveryPpi with a dummy interface to avoid installing duplicated capsules/FvInfo.
    //
    Status = PeiServicesReInstallPpi (&mRecoveryPpiList, &mDummyRecoveryPpiList);
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Provide the functionality of the Recovery Module.

  @retval  Status  EFI_SUCCESS if the interface could be successfully
                   installed
**/
EFI_STATUS
EFIAPI
InitializeRecovery (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = PeiServicesInstallPpi (&mRecoveryPpiList);

  return Status;
}
