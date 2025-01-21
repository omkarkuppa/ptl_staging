/** @file
  Support Seamless Recovery based system firmware update.

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

#ifndef __SEAMLESS_RECOVERY_SUPPORT_LIB_H__
#define __SEAMLESS_RECOVERY_SUPPORT_LIB_H__

#include <Guid/SysFwUpdateProgress.h>

/**
  Delete all backup files from the external storage plus the associated NV variable.

**/
VOID
DeleteBackupFiles (
  VOID
  );

/**
  Save Extended bios image from both current/new Extended BIOS to external storages.
  If NewExtendedImage is NULL or NewExtendedImageSize is 0, means to bacup current Extended image only.

  @param[in] NewExtendedImage       Pointers to Extended bios image in new BIOS.
  @param[in] NewExtendedImageSize   The size of NewExtendedImageImage.

  @retval  EFI_SUCCESS         Successfully backed up necessary files on external storage.
  @retval  Others              Failed to back up necessary files.

**/
EFI_STATUS
SaveExtendedBiosToStorage (
  IN VOID                         *NewExtendedImage,
  IN UINTN                        NewExtendedImageSize
  );

/**
  Save Obb image from both current/new BIOS to external storages.
  If NewObbImage is NULL or NewObbImageSize is 0, means to bacup current Obb image only.

  @param[in] NewObbImage       Pointers to Obb image in new BIOS.
  @param[in] NewObbImageSize   The size of NewObbImageImage.

  @retval  EFI_SUCCESS         Successfully backed up necessary files on external storage.
  @retval  Others              Failed to back up necessary files.

**/
EFI_STATUS
SaveObbToStorage (
  IN VOID                         *NewObbImage,
  IN UINTN                        NewObbImageSize
  );

/**
  Save NonFitPayload image from both current/new BIOS to external storages.
  If NewNonFitPayloadImage is NULL or NewNonFitPayloadImageSize is 0, means to backup current NonFitPayload image only.

  @param[in] NewNonFitPayloadImage       Pointers to NonFitPayload image in new BIOS.
  @param[in] NewNonFitPayloadImageSize   The size of NewNonFitPayloadImageImage.

  @retval  EFI_SUCCESS         Successfully backed up necessary files on external storage.
  @retval  Others              Failed to back up necessary files.

**/
EFI_STATUS
SaveNonFitPayloadToStorage (
  IN VOID                         *NewNonFitPayloadImage,
  IN UINTN                        NewNonFitPayloadImageSize
  );

/**
  Save current FMP Capsule and UX Capsule to external storages.

  @param[in] PayloadImage       Pointer to FMP payload image (FMP image header is stripped off).
  @param[in] PayloadImageSize   The size of PayloadImage.

  @retval  EFI_SUCCESS    Successfully backed up necessary files on external storage.
  @retval  Others         Failed to back up necessary files.

**/
EFI_STATUS
SaveCurrentCapsuleToStorage (
  IN VOID                         *PayloadImage,
  IN UINTN                        PayloadImageSize
  );

/**
  Search for ME recovery capsule on storage and return TRUE if present.

  @retval TRUE               ME recovery image is present.
  @retval FALSE              ME recovery image is not present.

**/
BOOLEAN
IsMeRecoveryCapsuleExistOnStorage (
  VOID
  );

/**
  Save current ME/Monolithic FMP Capsule as ME recovery capsule.

  @param[in] PayloadImage       Pointer to ME/Monolithic FMP payload image (FMP image header is stripped off).
  @param[in] PayloadImageSize   The size of PayloadImage.

  @retval  EFI_SUCCESS    Successfully backed up necessary files on external storage.
  @retval  Others         Failed to back up necessary files.

**/
EFI_STATUS
SaveMeRecoveryCapsule (
  IN VOID                         *PayloadImage,
  IN UINTN                        PayloadImageSize
  );

/**
  Replace MeRecovCap with MeRecovCapNew upon successful capsule update.


  @retval  EFI_SUCCESS    Successfully old recovery capsule with new recovery capsule.
  @retval  Others         Failed to sync old/new recovery capsules.

**/
EFI_STATUS
SyncMeRecoveryCapsules (
  VOID
  );

/**
  Check platform capability to support Fault tolerance based system firmware update.

  @retval TRUE  Current platform is capable of supporting Fault tolerance based system firmware update.
  @retval FALSE Current platform is incapable of supporting Fault tolerance based system firmware update.

**/
BOOLEAN
IsBiosFaultTolerantUpdateSupported (
  VOID
  );

/**
  Check if system firmware update got interrupted last time.

  @param[in,out] PreviousUpdateProgress      Pointers to the progress where updating process got
                                             interrupted last time.

  @retval TRUE   Previous update process got interrupted.
  @retval FALSE  There is no indication that update was in progress.

**/
BOOLEAN
IsPreviousUpdateUnfinished (
  IN OUT SYSTEM_FIRMWARE_UPDATE_PROGRESS       *PreviousUpdateProgress
  );

/**
  Get the current update progress from variable

  @param[out] CurrentUpdateProgress       Pointers to the current progress from NV storage

  @retval EFI_SUCCESS             Update progress is get from to NV storage successfully or it is not set.
  @retval EFI_INVALID_PARAMETER   CurrentUpdateProgress is NULL.

**/
EFI_STATUS
GetCurrentUpdateProgress (
  IN OUT SYSTEM_FIRMWARE_UPDATE_PROGRESS       *CurrentUpdateProgress
  );

/**
  Record the current update progress

  @param[in] UpdatingComponent      The FW component being updated now.
  @param[in] UpdatingProgress       The updating stage associated to UpdatingComponent

  @retval EFI_SUCCESS   Update progress is recorded to NV storage successfully.
  @retval Others        Update progress is not recorded.

**/
EFI_STATUS
SetUpdateProgress (
  IN SYSTEM_FIRMWARE_COMPONENT  UpdatingComponent,
  IN UINT16                     UpdatingProgress
  );

/**
  Clear update progress in NV storage. This indicates that no FW update process is happening.

**/
VOID
ClearUpdateProgress (
  VOID
  );

/**
  Delete a file from an assigned file system.

  @param[in] FileSystemHandle    Handle of the file system that file would be deleted from.
  @param[in] FileName            Pointer to file name.

  @retval EFI_SUCCESS            File does not exist or deleted the file successfully.
  @retval Others                 Failed to delete the file.

**/
EFI_STATUS
DeleteFile (
  IN   EFI_HANDLE       FileSystemHandle,
  IN   CHAR16           *FileName
  );

/**
  Write a file to an assigned file system.

  @param[in] FileSystemHandle    Handle of the file system that file would be write into.
  @param[in] FileName            Pointer to file name.
  @param[in] FileBuffer          The buffer to be written into file system.
  @param[in] FileSize            The size of FileBuffer.

  @retval EFI_SUCCESS            Wrote the file successfully.
  @retval Others                 Failed to write the file.

**/
EFI_STATUS
WriteBackupFile (
  IN   EFI_HANDLE       FileSystemHandle,
  IN   CHAR16           *FileName,
  IN   UINT8            *FileBuffer,
  IN   UINTN            FileSize
  );

/**
  Read a file from an assigned file system.

  @param[in] FileSystemHandle    Handle of the file system that file would be read.
  @param[in] FileName            Pointer to file name.
  @param[out] Buffer             Address of the buffer to which file is read.
  @param[out] BufferSize         The size of Buffer.

  @retval EFI_SUCCESS            Read the file successfully.
  @retval Others                 Failed to read the file.

**/
EFI_STATUS
ReadBackupFile (
  IN  EFI_HANDLE *FileSystemHandle,
  IN  CHAR16     *FileName,
  OUT VOID       **Buffer,
  OUT UINTN      *BufferSize
  );

/**
  Initialize mBackUpFileSystemHandle module variable

  @retval EFI_SUCCESS             Backup file system is found and assign to mBackUpFileSystemHandle
  @retval Others                  Cannot find an available file system to initialize mBackUpFileSystemHandle.

**/
EFI_STATUS
InitializeBackupFileSystem (
  VOID
  );

#endif
