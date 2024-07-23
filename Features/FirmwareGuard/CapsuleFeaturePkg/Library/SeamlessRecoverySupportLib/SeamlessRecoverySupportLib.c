/** @file
  Library instance to support Seamless Recovery based system firmware update.
  The major functionalities are
  1. Save backup files to external storage used in recovery path.
  2. Get/Set/Clear update progress.

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
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/CpuPlatformLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <BiosGuard.h>

#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <Protocol/Spi.h>
#include <Library/VariablePolicyHelperLib.h>


#include <Guid/FileSystemInfo.h>
#include <Guid/FileInfo.h>
#include <Guid/FmpCapsule.h>

#include <IndustryStandard/Pci.h>
#include <Library/PostCodeLib.h>
#include <Library/VmdInfoLib.h>
#include <Library/TopSwapSupportLib.h>
#include <Library/ResiliencySupportLib.h>
#include <Library/HashSupportLib.h>

EFI_HANDLE                               mBackUpFileSystemHandle = NULL;
SYSTEM_FIRMWARE_UPDATE_PROGRESS          mPreviousUpdateProgress = {0};

CHAR16  *mRecoveryVariables[] = {
  SYSFW_UPDATE_PROGRESS_VARIABLE_NAME,
  SYSFW_UPDATE_CAPSULE_DIGEST_VARIABLE_NAME,
};

/**
  Returns the FMP Payload Header size in bytes.

  @param[in]  Header          FMP Payload Header to evaluate
  @param[in]  FmpPayloadSize  Size of FMP payload
  @param[out] Size            The size, in bytes, of the FMP Payload Header.

  @retval EFI_SUCCESS            The firmware version was returned.
  @retval EFI_INVALID_PARAMETER  Header is NULL.
  @retval EFI_INVALID_PARAMETER  Size is NULL.
  @retval EFI_INVALID_PARAMETER  Header is not a valid FMP Payload Header.

**/
EFI_STATUS
EFIAPI
GetFmpPayloadHeaderSize (
  IN  CONST VOID   *Header,
  IN  CONST UINTN  FmpPayloadSize,
  OUT UINT32       *Size
  );

/**
  Connect storage controllers to back up relevant files for Fault Tolerance/Seamless Recovery support.
  Currently only NVME and onboard SATA controller are supported.

**/
VOID
ConnectPlatformController (
  VOID
  )
{
  EFI_STATUS           Status;
  UINTN                Index;
  UINTN                HandleCount;
  EFI_HANDLE           *HandleBuffer;
  PCI_TYPE00           PciData;
  EFI_PCI_IO_PROTOCOL  *PciIo;
  UINTN                Segment;
  UINTN                Bus;
  UINTN                Device;
  UINTN                Function;

  HandleCount  = 0;
  HandleBuffer = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID *) &PciIo
                    );
    ASSERT_EFI_ERROR (Status);
    PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint8,
                          0,
                          sizeof (PciData),
                          &PciData
                          );
    if (Bus != 0) {
      //
      //  Locate all NVME controllers and connect them.
      //
      if ((PciData.Hdr.ClassCode[2] == PCI_CLASS_MASS_STORAGE) &&
          (PciData.Hdr.ClassCode[1] == PCI_CLASS_MASS_STORAGE_SOLID_STATE) &&
          (PciData.Hdr.ClassCode[0] == PCI_IF_MASS_STORAGE_SOLID_STATE_ENTERPRISE_NVMHCI)) {
        Status = gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Connect NVME Controller on PCI %d/%d/%d - %r\n", Bus, Device, Function, Status));
        }
      }
    } else {
      //
      //  Locate Sata controllers and connect them.
      //
      if ((PciData.Hdr.ClassCode[2] == PCI_CLASS_MASS_STORAGE) &&
          (PciData.Hdr.ClassCode[1] == PCI_CLASS_MASS_STORAGE_SATADPA)) {
        Status = gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Connect Sata Controller on PCI %d/%d/%d - %r\n", Bus, Device, Function, Status));
        }
      }
      if ((PciData.Hdr.ClassCode[2] == PCI_CLASS_MASS_STORAGE) &&
          (PciData.Hdr.ClassCode[1] == PCI_CLASS_MASS_STORAGE_RAID)) {

        //
        // if detected as RAID, need check this PCI device is vmd or not to decide connect or disconnect
        //
        if ((GetVmdBusNumber () == Bus) && (GetVmdDevNumber () == Device) && (GetVmdFuncNumber () == Function)) {
          Status = gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "Connect vmd Controller on PCI %d/%d/%d - %r\n", Bus, Device, Function, Status));
          }
        } else {
          //
          // In case that RAID controller has been connected before update function being invoked.
          //
          Status = gBS->DisconnectController (
                          HandleBuffer[Index],
                          NULL,
                          NULL
                          );
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "Disconnect RAID Controller on PCI %d/%d/%d - %r\n", Bus, Device, Function, Status));
          }
        }
      }
    }
  }
  if (HandleBuffer) {
    FreePool (HandleBuffer);
  }
  return;
}

/**
  Find out the file system of EFI System partition to save backup files into storeage.

  @param[out] FileSystemHandle    Pointer to the file system handle would be used to keep
                                  Seamless Recovery backup files.

  @retval EFI_SUCCESS             The file system is found.
  @retval Others                  Cannot find an available file system.

**/
EFI_STATUS
SearchBackupFileSystem (
  OUT   EFI_HANDLE       *FileSystemHandle
  )
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            *HandleArray;
  UINTN                                 HandleArrayCount;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  UINTN                                 Index;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *Fs;
  EFI_FILE                              *Root;
  EFI_FILE_SYSTEM_INFO                  *SysInfo;
  UINTN                                 SysInfoSize;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS       PreviousProgress;
  UINTN                                 MaxEspFreeSpace;

  DEBUG ((DEBUG_INFO, "SearchBackupFileSystem - entry\n"));

  *FileSystemHandle = NULL;
  HandleArray       = NULL;
  MaxEspFreeSpace   = 0;
  //
  // Search all EFI system partitions
  //
  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiPartTypeSystemPartGuid, NULL, &HandleArrayCount, &HandleArray);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot find ESP partition. Status = %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "ESP handle count is: %d\n", HandleArrayCount));

  for (Index = 0; (Index < HandleArrayCount) && (*FileSystemHandle == NULL); Index++) {
    Status = gBS->HandleProtocol (HandleArray[Index], &gEfiDevicePathProtocolGuid, (VOID **)&DevicePath);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot locate DevicePath protocol. Status = %r\n", Status));
      continue;
    }

    //
    // Get the SFS protocol from the handle
    //
    Status = gBS->HandleProtocol (HandleArray[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&Fs);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot locate SFS protocol. Status = %r\n", Status));
      continue;
    }

    //
    // Previous update is not finished, skip capsule backup
    //
    DEBUG ((DEBUG_INFO, "Check Update Process ...\n"));
    if (IsPreviousUpdateUnfinished (&PreviousProgress)) {
      DEBUG ((DEBUG_INFO, "Available ESP found! Previous update is not finished, skip checking free space.\n"));
      *FileSystemHandle = HandleArray[Index];
      return EFI_SUCCESS;
    }

    //
    // Open the root directory, get EFI_FILE_PROTOCOL
    //
    Status = Fs->OpenVolume (Fs, &Root);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot open volume. Status = %r\n", Status));
      continue;
    }

    SysInfo     = NULL;
    SysInfoSize = 0;
    Status = Root->GetInfo (Root, &gEfiFileSystemInfoGuid, &SysInfoSize, SysInfo);
    if (Status == EFI_BUFFER_TOO_SMALL) {
      SysInfo = AllocateZeroPool (SysInfoSize);
      if (SysInfo == NULL) {
        DEBUG ((DEBUG_ERROR, "System memory is out of resource to allocate file system info buffer.\n"));
        Root->Close (Root);
        break;
      }
      Status = Root->GetInfo (Root, &gEfiFileSystemInfoGuid, &SysInfoSize, SysInfo);
      if (Status == EFI_SUCCESS) {
        DEBUG ((DEBUG_INFO, "File system info:\n"));
        DEBUG ((DEBUG_INFO, "FreeSpace:0x%x bytes\n", SysInfo->FreeSpace));
        DEBUG ((DEBUG_INFO, "BlockSize:0x%x bytes\n", SysInfo->BlockSize));
        DEBUG ((DEBUG_INFO, "ReadOnly:%x\n", SysInfo->ReadOnly));
        if ((SysInfo->FreeSpace > MaxEspFreeSpace) && (!SysInfo->ReadOnly)) {
          DEBUG ((DEBUG_INFO, "Available ESP found\n"));
          MaxEspFreeSpace   = SysInfo->FreeSpace;
          *FileSystemHandle = HandleArray[Index];
        }
      }
      FreePool (SysInfo);
    }

    Root->Close (Root);
  }

  if (*FileSystemHandle == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    Status = EFI_SUCCESS;
  }

  FreePool (HandleArray);
  return Status;
}

/**
  Initialize mBackUpFileSystemHandle module variable

  @retval EFI_SUCCESS             Backup file system is found and assign to mBackUpFileSystemHandle
  @retval Others                  Cannot find an available file system to initialize mBackUpFileSystemHandle.

**/
EFI_STATUS
InitializeBackupFileSystem (
  VOID
  )
{
  EFI_STATUS             Status;

  if (mBackUpFileSystemHandle != NULL) {
    //
    // BackupFilesystem has been initialized.
    //
    return EFI_SUCCESS;
  }

  //
  // Connect storage and check free space.
  //
  ConnectPlatformController ();
  Status = SearchBackupFileSystem (&mBackUpFileSystemHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot find storage file system to support seamless recovery. Status = %r\n", Status));
  }

  return Status;
}

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
  )
{
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *FileSystem;
  EFI_FILE                              *Root;
  EFI_FILE                              *FileHandle;

  DEBUG ((DEBUG_INFO, "DeleteBackupFile - entry\n"));

  Status = gBS->HandleProtocol (FileSystemHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&FileSystem);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot locate SFS protocol. Status = %r\n", Status));
    return Status;
  }

  //
  // Open the root directory, get EFI_FILE_PROTOCOL
  //
  Status = FileSystem->OpenVolume (FileSystem, &Root);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot open volume. Status = %r\n", Status));
    return Status;
  }

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
  if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "File %s does not exist. No need to delete\n", FileName));
    return EFI_SUCCESS;
  } else if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot open file: %s. Status = %r\n", FileName, Status));
    return Status;
  }

  if (FileHandle == NULL) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Failed to open root dir on partition for writing. Stautus = %r\n", Status));
    return Status;
  }

  Status = FileHandle->Delete (FileHandle);
  DEBUG ((DEBUG_INFO, "Delete %s %r\n", FileName, Status));

  Root->Close (Root);

  return Status;
}

/**
  Delete backup Obb files from the external storage.

**/
VOID
DeleteBackupObbFiles (
  VOID
  )
{
  EFI_STATUS               Status;

  Status = InitializeBackupFileSystem ();

  if (Status == EFI_SUCCESS) {
    if (GetBiosResiliencyType () != SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      //
      // Not to delete the curent Obb for BIOS resiliency support.
      // Delete current Obb file only when we are about to update it.
      //
      DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME);
    }
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_NEW_OBB_BACKUP_FILE_NAME);
  }
}

/**
  Delete backup NonFitPayload files from the external storage.

**/
VOID
DeleteBackupNonFitPayloadFiles (
  VOID
  )
{
  EFI_STATUS               Status;

  Status = InitializeBackupFileSystem ();

  if (Status == EFI_SUCCESS) {
    if (GetBiosResiliencyType () != SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      //
      // Not to delete the curent NonFitPayload for BIOS resiliency support.
      // Delete current NonFitPayload file only when we are about to update it.
      //
      DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_PAYLOAD_BACKUP_FILE_NAME);
    }
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_NEW_PAYLOAD_BACKUP_FILE_NAME);
  }
}

/**
  Delete backup Capsule files from the external storage plus the associated NV variable.

**/
VOID
DeleteBackupCapsules (
  VOID
  )
{
  EFI_STATUS               Status;

  Status = InitializeBackupFileSystem ();

  if (Status == EFI_SUCCESS) {
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CAPSULE_BACKUP_FILE_NAME);
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_WINDOWS_UX_CAPSULE_FILE_NAME);
  }

  gRT->SetVariable (
         SYSFW_UPDATE_CAPSULE_DIGEST_VARIABLE_NAME,
         &gSysFwUpdateProgressGuid,
         0,
         0,
         NULL
         );

}

/**
  Delete all backup files from the external storage plus the associated NV variable.

**/
VOID
DeleteBackupFiles (
  VOID
  )
{
  DeleteBackupObbFiles ();
  DeleteBackupCapsules ();
}

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
  )
{
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *FileSystem;
  EFI_FILE                              *Root;
  EFI_FILE                              *FileHandle;
  UINTN                                 WriteSize;

  DEBUG ((DEBUG_INFO, "WriteBackupFile (%s) - entry\n", FileName));

  Status = gBS->HandleProtocol (FileSystemHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&FileSystem);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot locate SFS protocol. Status = %r\n", Status));
    return Status;
  }

  //
  // Open the root directory, get EFI_FILE_PROTOCOL
  //
  Status = FileSystem->OpenVolume (FileSystem, &Root);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot open volume. Status = %r\n", Status));
    return Status;
  }

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Cannot open file: %s. Status = %r\n", FileName, Status));
    return Status;
  }

  if (FileHandle == NULL) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Failed to open root dir on partition for writing. Stautus = %r\n", Status));
    return Status;
  }

  do {
    WriteSize = (FileSize > SIZE_4KB) ? SIZE_4KB : FileSize;
    Status = FileHandle->Write (FileHandle, &WriteSize, FileBuffer);
    if (EFI_ERROR (Status)) {
      break;
    }
    FileSize = FileSize - WriteSize;
    FileBuffer = FileBuffer + WriteSize;
  } while (FileSize > 0);

  DEBUG ((DEBUG_INFO, "Write %s %r\n", FileName, Status));
  FileHandle->Close (FileHandle);
  Root->Close (Root);

  return Status;
}

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
  )
{
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *FileSystem;
  EFI_FILE                              *Root;
  EFI_FILE                              *FileHandle;
  UINTN                                 FileInfoSize;
  EFI_FILE_INFO                         *FileInfo;
  EFI_GUID                              FileInfoGuid = EFI_FILE_INFO_ID;

  DEBUG ((DEBUG_INFO, "ReadBackupFile (%s) - entry\n", FileName));

  Status = gBS->HandleProtocol (FileSystemHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&FileSystem);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot locate SFS protocol. Status = %r\n", Status));
    return Status;
  }

  Root = NULL;
  //
  // Open the root directory, get EFI_FILE_PROTOCOL
  //
  Status = FileSystem->OpenVolume (FileSystem, &Root);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot open volume. Status = %r\n", Status));
    return Status;
  }

  if (Root == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ, 0);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot open file: %s. Status = %r\n", FileName, Status));
    return Status;
  }

  if (FileHandle == NULL) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Failed to open root dir on partition for reading. Stautus = %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "FileInfoRead\n"));

  FileInfoSize = 0;
  FileInfo = NULL;

  Status = FileHandle->GetInfo (
                         FileHandle,
                         &FileInfoGuid,
                         &FileInfoSize,
                         NULL
                         );

  if (EFI_ERROR (Status)) {
    if (Status != EFI_BUFFER_TOO_SMALL) {
      DEBUG ((DEBUG_ERROR, "FileRead fail, GetInfo error, Status: %r\n", Status));
      return Status;
    }
  }

  if (Buffer == NULL || BufferSize == NULL) {
    DEBUG ((DEBUG_INFO, "FileInfoRead only, FileInfo Status: %r\n", Status));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "FileRead\n"));
  FileInfo = AllocatePool (FileInfoSize);
  if (FileInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "FileRead fail, AllocatePool(FileInfoSize: %x) error\n", FileInfoSize));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = FileHandle->GetInfo (
                         FileHandle,
                         &FileInfoGuid,
                         &FileInfoSize,
                         FileInfo
                         );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FileRead fail, GetInfo error, Status: %r\n", Status));
    goto done;
  }

  *BufferSize = (UINT32) FileInfo->FileSize;
  if (*BufferSize != 0) {
    *Buffer = AllocateZeroPool (*BufferSize);
    if (*Buffer == NULL) {
      DEBUG ((DEBUG_ERROR, "FileRead fail, AllocatePool(FileSize: %x) error\n", *BufferSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto done;
    }

    Status = FileHandle->Read (
                           FileHandle,
                           BufferSize,
                           *Buffer
                           );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "FileRead fail, Read error, Status: %r\n", Status));
      goto done;
    }

  } else {
    DEBUG ((DEBUG_INFO, "File size is 0, set return Buffer to NULL.\n"));
    *Buffer = NULL;
  }

  Status = EFI_SUCCESS;

done:
  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }

  if (FileHandle != NULL) {
    FileHandle->Close (FileHandle);
  }

  if (Root != NULL) {
    Root->Close (Root);
  }

  DEBUG ((DEBUG_INFO, "FileRead done, Status: %r, BufferSize: %x\n", Status, *BufferSize));
  return Status;
}

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
  )
{
  EFI_STATUS                            Status;

  DEBUG ((DEBUG_INFO, "SaveObbToStorage - entry\n"));

  Status = InitializeBackupFileSystem ();

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // 1. Delete previous backup files before creating new one
  //
  DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME);

  //
  // 2. Save Obb from current flash to external storage
  //
  Status = WriteBackupFile (
             mBackUpFileSystemHandle,
             SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME,
             (UINT8 *) (UINTN) FixedPcdGet32 (PcdFlashObbBase),
             (UINTN) FixedPcdGet32 (PcdFlashObbSize)
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to write Obb.bin(%r)\n", Status));
    return Status;
  }

  //
  // 3. Save Obb from new image to external storage if required.
  //
  if ((NewObbImage != NULL) && (NewObbImageSize != 0)) {
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_NEW_OBB_BACKUP_FILE_NAME);

    Status = WriteBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_NEW_OBB_BACKUP_FILE_NAME,
               (UINT8 *) (UINTN) NewObbImage,
               NewObbImageSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to write ObbN.bin (%r)\n", Status));
      DeleteBackupObbFiles ();
    }
  }

  return Status;
}

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
  )
{
  EFI_STATUS                            Status;

  DEBUG ((DEBUG_INFO, "SaveNonFitPayloadToStorage - entry\n"));

  Status = InitializeBackupFileSystem ();

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // 1. Delete previous backup files before creating new one
  //
  DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_PAYLOAD_BACKUP_FILE_NAME);

  //
  // 2. Save NonFitPayload from current flash to external storage
  //
  Status = WriteBackupFile (
             mBackUpFileSystemHandle,
             SYSFW_UPDATE_CURRENT_PAYLOAD_BACKUP_FILE_NAME,
             (UINT8 *) (UINTN) FixedPcdGet32 (PcdFlashNonFitPayloadBase),
             (UINTN) FixedPcdGet32 (PcdFlashNonFitPayloadSize)
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to write NonFitPayload.bin(%r)\n", Status));
    return Status;
  }

  //
  // 3. Save NonFitPayload from new image to external storage if required.
  //
  if ((NewNonFitPayloadImage != NULL) && (NewNonFitPayloadImageSize != 0)) {
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_NEW_PAYLOAD_BACKUP_FILE_NAME);

    Status = WriteBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_NEW_PAYLOAD_BACKUP_FILE_NAME,
               (UINT8 *) (UINTN) NewNonFitPayloadImage,
               NewNonFitPayloadImageSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to write NonFitPayloadN.bin (%r)\n", Status));
      DeleteBackupNonFitPayloadFiles ();
    }
  }

  return Status;
}

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
  )
{
  EFI_STATUS   Status;
  UINT8        *ExtendedBiosSourceBuffer;
  UINT32       ExtendedBiosSourceSize;

  ExtendedBiosSourceBuffer = NULL;

  DEBUG ((DEBUG_INFO, "SaveExtendedBiosToStorage - entry\n"));
  Status = InitializeBackupFileSystem ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // 1. Delete previous backup files before creating new one
  //
  DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_EXTENDED_BIOS_BACKUP_FILE_NAME);

  //
  // 2. Save Extended bios from current flash to external storage
  //
  ExtendedBiosSourceSize = FixedPcdGet32 (PcdFlashExtendSize);
  ExtendedBiosSourceBuffer = AllocateCopyPool (
                      ExtendedBiosSourceSize,
                      (VOID *) (UINTN) (FixedPcdGet32 (PcdFlashExtendedRegionBase) + FixedPcdGet32 (PcdFlashExtendOffset))
                      );

  Status = WriteBackupFile (
             mBackUpFileSystemHandle,
             SYSFW_UPDATE_CURRENT_EXTENDED_BIOS_BACKUP_FILE_NAME,
             ExtendedBiosSourceBuffer,
             (UINTN) ExtendedBiosSourceSize
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to write ExtendedBios.bin(%r)\n", Status));
    goto Exit;
  }

  //
  // 3. Save Extended bios from new image to external storage if required.
  //
  if ((NewExtendedImage != NULL) && (NewExtendedImageSize != 0)) {
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_NEW_EXTENDED_BIOS_BACKUP_FILE_NAME);

    Status = WriteBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_NEW_EXTENDED_BIOS_BACKUP_FILE_NAME,
               (UINT8 *) (UINTN) NewExtendedImage,
               NewExtendedImageSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to write ExtendedBiosN.bin (%r)\n", Status));
      if (GetBiosResiliencyType () != SUPPORT_BIOS_RESILIENCY_RECOVERY) {
        DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_EXTENDED_BIOS_BACKUP_FILE_NAME);
      }
      DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_NEW_EXTENDED_BIOS_BACKUP_FILE_NAME);
    }
  }
Exit:
  if (ExtendedBiosSourceBuffer != NULL) {
    FreePool (ExtendedBiosSourceBuffer);
  }
  return Status;
}


/**
  Calculate the size of dependency op-codes associated with the image.

  The param "Image" of FmpDeviceSetImage consists of dependency op-codes
  and Fmp Payload Image.

  Assumption: The dependency passes validation in Fmp->CheckImage().

  @param[in]   Image       Pointer to Image.
  @param[in]   ImageSize   Size of Image.

  @retval  Size of dependency op-codes.

**/
UINTN
CalculateFmpDependencySizeInNewImage (
  IN  UINT8                             *Image,
  IN  CONST UINTN                       ImageSize
  )
{
  UINT8  *Depex;

  Depex = ((EFI_FIRMWARE_IMAGE_DEP *)Image)->Dependencies;
  while (Depex < Image + ImageSize) {
    switch (*Depex)
    {
    case EFI_FMP_DEP_PUSH_GUID:
      Depex += sizeof (EFI_GUID) + 1;
      break;
    case EFI_FMP_DEP_PUSH_VERSION:
      Depex += sizeof (UINT32) + 1;
      break;
    case EFI_FMP_DEP_VERSION_STR:
      Depex += AsciiStrnLenS ((CHAR8 *) Depex, Image - (Depex + ImageSize)) + 1;
      break;
    case EFI_FMP_DEP_AND:
    case EFI_FMP_DEP_OR:
    case EFI_FMP_DEP_NOT:
    case EFI_FMP_DEP_TRUE:
    case EFI_FMP_DEP_FALSE:
    case EFI_FMP_DEP_EQ:
    case EFI_FMP_DEP_GT:
    case EFI_FMP_DEP_GTE:
    case EFI_FMP_DEP_LT:
    case EFI_FMP_DEP_LTE:
      Depex += 1;
      break;
    case EFI_FMP_DEP_END:
      Depex += 1;
      return Depex - Image;
    default:
      return 0;
    }
  }

  return 0;
}

/**
  Search for a FMP capsule corredspnding to current FMP driver.

  @param[in] PayloadImage       Pointer to FMP payload image (FMP image header is stripped off).
  @param[in] PayloadImageSize   The size of PayloadImage.

  @retval Pointer to the capsule hob found.
          NULL means no such capsules there.

**/
EFI_HOB_UEFI_CAPSULE *
SearchThisFmpCapsule (
  IN VOID                         *PayloadImage,
  IN UINTN                        PayloadImageSize
  )
{
  EFI_PEI_HOB_POINTERS                          HobPointer;
  EFI_CAPSULE_HEADER                            *CapsuleHeader;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER        *FmpCapsuleHeader;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER  *ImageHeader;
  UINTN                                         Index;
  UINT64                                        *ItemOffsetList;
  EFI_FIRMWARE_IMAGE_AUTHENTICATION             *FmpImageAuth;
  UINT8                                         *FmpDependency;
  UINTN                                         FmpDependencySize;
  VOID                                          *FmpPayloadHeader;
  UINT32                                        FmpPayloadHeaderSize;
  VOID                                          *PayloadImageInCapsule;
  UINTN                                         PayloadImageSizeInCapsule;
  EFI_STATUS                                    Status;

  //
  // Search all capsule images from hob
  //
  HobPointer.Raw = GetHobList ();
  DEBUG ((DEBUG_INFO, "SearchThisFmpCapsule Start.\n"));

  while ((HobPointer.Raw = GetNextHob (EFI_HOB_TYPE_UEFI_CAPSULE, HobPointer.Raw)) != NULL) {
    CapsuleHeader = (EFI_CAPSULE_HEADER *) (UINTN) HobPointer.Capsule->BaseAddress;
    DEBUG ((DEBUG_INFO, "Check Capsule ...\n"));

    //
    // Must be a nested FMP capsule or FMP capsule
    //
    if (!CompareGuid (&gEfiFmpCapsuleGuid, &CapsuleHeader->CapsuleGuid)) {
      //
      // Might be a nested FMP, check further
      //
      CapsuleHeader = (EFI_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize);
    }

    if (!CompareGuid (&gEfiFmpCapsuleGuid, &CapsuleHeader->CapsuleGuid)) {
      HobPointer.Raw = GET_NEXT_HOB (HobPointer);
      continue;
    }

    FmpCapsuleHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize);
    ItemOffsetList = (UINT64 *)(FmpCapsuleHeader + 1);
    for (Index = FmpCapsuleHeader->EmbeddedDriverCount; Index < (UINT32)FmpCapsuleHeader->EmbeddedDriverCount + FmpCapsuleHeader->PayloadItemCount; Index++) {
      ImageHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER *)((UINT8 *)FmpCapsuleHeader + ItemOffsetList[Index]);
      //
      // Strip off all headers to get Fmp payload image in the capsule file,
      // then compare it with the 'PayloadImage' in FmpDeviceSetImage().
      FmpImageAuth = (EFI_FIRMWARE_IMAGE_AUTHENTICATION *) (ImageHeader + 1);
      if (ImageHeader->Version < EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER_INIT_VERSION) {
        if (ImageHeader->Version == 2) {
          FmpImageAuth = (EFI_FIRMWARE_IMAGE_AUTHENTICATION *)((UINT8*) ImageHeader + OFFSET_OF (EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER, ImageCapsuleSupport));
        } else if (ImageHeader->Version == 1) {
          FmpImageAuth = (EFI_FIRMWARE_IMAGE_AUTHENTICATION *)((UINT8*) ImageHeader + OFFSET_OF (EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER, UpdateHardwareInstance));
        } else {
          ASSERT (FALSE);
        }
      }

      if (((UINTN)FmpImageAuth + sizeof (FmpImageAuth->MonotonicCount) + (UINTN) FmpImageAuth->AuthInfo.Hdr.dwLength) < (UINTN)FmpImageAuth || \
          ((UINTN)FmpImageAuth + sizeof (FmpImageAuth->MonotonicCount) + (UINTN) FmpImageAuth->AuthInfo.Hdr.dwLength) >= (UINTN)FmpImageAuth + ImageHeader->UpdateImageSize) {
        //
        // Pointer overflow. Invalid image.
        //

        UINTN    TempData = 0;
        TempData = FmpImageAuth->AuthInfo.Hdr.dwLength;

        DEBUG ((DEBUG_INFO, "AuthInfo.Hdr.dwLength: %d\n", FmpImageAuth->AuthInfo.Hdr.dwLength));
        DEBUG ((DEBUG_INFO, "TempData: %d\n", TempData));
        DEBUG ((DEBUG_INFO, "ImageHeader->UpdateImageSize: %d\n", ImageHeader->UpdateImageSize));
        continue;
      }

      //
      //
      // Strip off Fmp Image Authenticaion to get Fmp Dependency
      //
      FmpDependency = (UINT8 *) FmpImageAuth + sizeof (FmpImageAuth->MonotonicCount) + FmpImageAuth->AuthInfo.Hdr.dwLength;

      //
      // Strip off Fmp Dependency to get Fmp Payload Header.
      //
      FmpDependencySize = CalculateFmpDependencySizeInNewImage (FmpDependency, (UINT8 *) FmpImageAuth + ImageHeader->UpdateImageSize - FmpDependency);
      FmpPayloadHeader = FmpDependency + FmpDependencySize;

      //
      // Strip off Fmp Payload Header to get Fmp Payload Image
      //
      Status = GetFmpPayloadHeaderSize (FmpPayloadHeader, (UINT8 *) FmpImageAuth + ImageHeader->UpdateImageSize - (UINT8 *)FmpPayloadHeader, &FmpPayloadHeaderSize);
      if (EFI_ERROR (Status)) {
        //
        // Invalid Fmp payload image.
        //
        DEBUG ((DEBUG_INFO, "Invalid Fmp payload image.\n"));
        continue;
      }
      PayloadImageInCapsule = (UINT8 *) FmpPayloadHeader + FmpPayloadHeaderSize;
      PayloadImageSizeInCapsule = ImageHeader->UpdateImageSize - sizeof (FmpImageAuth->MonotonicCount) - FmpImageAuth->AuthInfo.Hdr.dwLength - FmpDependencySize - FmpPayloadHeaderSize;

      //
      // Check size first.
      //
      if (PayloadImageSizeInCapsule == PayloadImageSize) {
        //
        // Compare two payloads to check if they are indentical.
        //
        if (CompareMem (PayloadImageInCapsule, PayloadImage, PayloadImageSize) == 0) {
          DEBUG ((DEBUG_INFO, "Target FMP capsule is detected(%g).\n", &ImageHeader->UpdateImageTypeId));
          return HobPointer.Capsule;
        }
      }
    }

    HobPointer.Raw = GET_NEXT_HOB (HobPointer);
  }

  return NULL;
}

/**
  Search for Windows UX Capsule

  @retval Pointer to the capsule hob found.
          NULL means no such capsules there.

**/
EFI_HOB_UEFI_CAPSULE *
SearchWindowsUxCapsule (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS                          HobPointer;
  EFI_CAPSULE_HEADER                            *CapsuleHeader;

  //
  // Search all capsule images from hob
  //
  HobPointer.Raw = GetHobList ();
  while ((HobPointer.Raw = GetNextHob (EFI_HOB_TYPE_UEFI_CAPSULE, HobPointer.Raw)) != NULL) {
    CapsuleHeader = (EFI_CAPSULE_HEADER *) (UINTN) HobPointer.Capsule->BaseAddress;

    if (CompareGuid (&CapsuleHeader->CapsuleGuid, &gWindowsUxCapsuleGuid)) {
      DEBUG ((DEBUG_INFO, "Windows UX capsule is detected\n"));
      return HobPointer.Capsule;
    }

    HobPointer.Raw = GET_NEXT_HOB (HobPointer);
  }

  return NULL;
}

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
  )
{
  EFI_STATUS            Status;
  EFI_HOB_UEFI_CAPSULE  *CurrentFmpCapsule;
  EFI_HOB_UEFI_CAPSULE  *WindowsUxCapsule;
  UINT8                 *HashShaDigest;

  DEBUG ((DEBUG_INFO, "SaveCurrentCapsuleToStorage - entry\n"));

  if ((PayloadImage == NULL) || (PayloadImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = InitializeBackupFileSystem ();

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // 1. Delete previous backup files before creating new one
  //
  DeleteBackupCapsules ();

  //
  // 2. Save current Capsule (including Capsule/FMP header) to external storage
  //
  CurrentFmpCapsule = SearchThisFmpCapsule (PayloadImage, PayloadImageSize);
  if (CurrentFmpCapsule == NULL) {
    DEBUG ((DEBUG_ERROR, "Cannot find Capsule image(%g)\n", &gEfiCallerIdGuid));
    return EFI_NOT_FOUND;
  }

  Status = WriteBackupFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CAPSULE_BACKUP_FILE_NAME, (UINT8 *) (UINTN) CurrentFmpCapsule->BaseAddress, CurrentFmpCapsule->Length);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // 2-1. Also keep the message digest of the saved capsule image in NV variable for integrity check.
  //
  HashShaDigest = AllocateZeroPool (CAPSULE_HASH_DIGEST_SIZE);
  if (HashShaDigest == NULL) {
    goto ClearBackup;
  }

  DEBUG ((DEBUG_INFO, "Create Capsule digest\n"));
  Status = CreateHashShaDigest (
             (UINT8 *)((UINTN)CurrentFmpCapsule->BaseAddress),
             CurrentFmpCapsule->Length,
             &HashShaDigest,
             CAPSULE_HASH_DIGEST_SIZE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to create hash digest for capsule.\n"));
    goto ClearBackup;
  }

  Status = gRT->SetVariable (
                  SYSFW_UPDATE_CAPSULE_DIGEST_VARIABLE_NAME,
                  &gSysFwUpdateProgressGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  CAPSULE_HASH_DIGEST_SIZE,
                  (VOID *)HashShaDigest
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to keep message digest of the saved Capsule image.\n"));
    goto ClearBackup;
  }

  //
  // 3. Save Windows UX Capsule to external storage if there is one.
  //
  WindowsUxCapsule = SearchWindowsUxCapsule ();
  if (WindowsUxCapsule != NULL) {
    DEBUG ((DEBUG_INFO, "Save Windows UX Capsule to external storage.\n"));
    Status = WriteBackupFile (
                mBackUpFileSystemHandle,
                SYSFW_UPDATE_WINDOWS_UX_CAPSULE_FILE_NAME,
                (UINT8 *) (UINTN) WindowsUxCapsule->BaseAddress,
                 WindowsUxCapsule->Length
                );
  }

ClearBackup:
  if (HashShaDigest != NULL) {
    FreePool (HashShaDigest);
  }

  if (EFI_ERROR (Status)) {
    DeleteBackupCapsules ();
  }

  return Status;
}

/**
  Search for ME recovery capsule on storage and return TRUE if present.

  @retval TRUE               ME recovery image is present.
  @retval FALSE              ME recovery image is not present.

**/
BOOLEAN
IsMeRecoveryCapsuleExistOnStorage (
  VOID
  )
{
  EFI_STATUS               Status;

  Status = InitializeBackupFileSystem ();
  if (!EFI_ERROR (Status)) {
    Status = ReadBackupFile (mBackUpFileSystemHandle, SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME, NULL, NULL);
  }
  return (Status == EFI_SUCCESS);
}

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
  )
{
  EFI_STATUS                            Status;
  EFI_HOB_UEFI_CAPSULE                  *CurrentFmpCapsule;

  DEBUG ((DEBUG_INFO, "SaveMeRecoveryCapsule - entry\n"));

  if ((PayloadImage == NULL) || (PayloadImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = InitializeBackupFileSystem ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Find the capsule which has PayloadImage content.
  //
  CurrentFmpCapsule = SearchThisFmpCapsule (PayloadImage, PayloadImageSize);
  if (CurrentFmpCapsule == NULL) {
    DEBUG ((DEBUG_ERROR, "Cannot find ME Capsule image(%g)\n", &gEfiCallerIdGuid));
    return EFI_NOT_FOUND;
  }

  //
  // Replace the any existing old recovery capsule with new capsule.
  //
  Status = DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME_NEW);
  if (!EFI_ERROR (Status) || (Status == EFI_NOT_FOUND)) {
    Status = WriteBackupFile (mBackUpFileSystemHandle, SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME_NEW, (UINT8 *) (UINTN) CurrentFmpCapsule->BaseAddress, CurrentFmpCapsule->Length);
  }

  return Status;
}

/**
  Replace MeRecovCap with MeRecovCapNew upon successful capsule update.


  @retval  EFI_SUCCESS    Successfully old recovery capsule with new recovery capsule.
  @retval  Others         Failed to sync old/new recovery capsules.

**/
EFI_STATUS
SyncMeRecoveryCapsules (
  VOID
  )
{
  EFI_STATUS                     Status;
  UINT8                          *Buffer;
  UINTN                          Length;

  Status           = EFI_SUCCESS;
  Buffer           = NULL;
  Length           = 0;

  DEBUG ((DEBUG_INFO, "SyncMeRecoveryCapsules - entry\n"));

  Status = InitializeBackupFileSystem ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = ReadBackupFile (mBackUpFileSystemHandle, SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME_NEW, (VOID **) &Buffer, &Length);
  if (!EFI_ERROR (Status)) {
    //
    // A new ME Recovery capsule exists on storage. Replace old recovery capsule with new capsule.
    //
    Status = DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME);
    if (!EFI_ERROR (Status) || (Status == EFI_NOT_FOUND)) {
      Status = WriteBackupFile (mBackUpFileSystemHandle, SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME, (UINT8 *) (UINTN) Buffer, Length);
      if (!EFI_ERROR (Status)) {
        DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME_NEW);
      }
    }
    FreePool (Buffer);
  }

  return Status;
}

/**
  Check platform capability to support Fault tolerance based system firmware update.

  @retval TRUE  Current platform is capable of supporting Fault tolerance based system firmware update.
  @retval FALSE Current platform is incapable of supporting Fault tolerance based system firmware update.

**/
BOOLEAN
IsBiosFaultTolerantUpdateSupported (
  VOID
  )
{
//  EFI_STATUS                Status;
//  PCH_SPI_PROTOCOL          *SpiProtocol = NULL;
//  UINT8                     PchStrap;

  DEBUG ((DEBUG_INFO, "Check PlatformCheckFtSupported.\n"));

/*  if ((FixedPcdGet32 (PcdFlashFvReservedSize) < FixedPcdGet32 (PcdFlashFvRecoverySize)) ||
      (FixedPcdGet32 (PcdFlashFvRecoverySize) != SIZE_128KB)) {
    DEBUG ((DEBUG_ERROR, "PcdFlashFvReservedSize or PcdFlashFvRecoverySize does not meet fault tolerant update requirement.\n"));
    return FALSE;
  }
*/
//
// @TODO: Determine if we need an API from SI or not.
//
//  Status = gBS->LocateProtocol (&gPchSpiProtocolGuid, NULL, (VOID **) &SpiProtocol);
//  ASSERT_EFI_ERROR (Status);
//  if (EFI_ERROR (Status)) {
//    DEBUG ((DEBUG_ERROR, "Failed to locate PCH SPI protocol\n"));
//    return FALSE;
//  }
//  Status = SpiProtocol->ReadPchSoftStrap (SpiProtocol, R_PCH_SPI_STRP_DSCR_63, sizeof (PchStrap), &PchStrap);
//  if (EFI_ERROR (Status)) {
//    DEBUG ((DEBUG_ERROR, "Failed to read PCH soft strap\n"));
//    return FALSE;
//  }
//
//  DEBUG ((DEBUG_INFO, "PCH Rescriptor Record 63: 0x%x\n", PchStrap));
//  if (((PchStrap & B_PCH_SPI_STRP_DSCR_63_TSBS) >> N_PCH_SPI_STRP_DSCR_63_TSBS) != V_PCH_SPI_STRP_DSCR_63_TSBS_128K) {
//    DEBUG ((DEBUG_ERROR, "Top Swap block size does not meet platform requirement\n"));
//    return FALSE;
//  }

  return TRUE;
}

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
  )
{
  if (PreviousUpdateProgress != NULL) {
    PreviousUpdateProgress->Component = mPreviousUpdateProgress.Component;
    PreviousUpdateProgress->Progress  = mPreviousUpdateProgress.Progress;
  }

  if (GetBiosResiliencyType () == NOT_SUPPORT_RECOVERY) {
    //
    // Add check for handle res bios update to bkc bios case.
    //
    if (mPreviousUpdateProgress.Component == UpdatingResiliency) {
      ClearUpdateProgress ();
      return FALSE;
    }
  }

  return (mPreviousUpdateProgress.Component != NotStarted) ? TRUE : FALSE;
}

/**
  Get the current update progress from variable

  @param[out] CurrentUpdateProgress       Pointers to the current progress from NV storage

  @retval EFI_SUCCESS             Update progress is get from to NV storage successfully or it is not set.
  @retval EFI_INVALID_PARAMETER   CurrentUpdateProgress is NULL.

**/
EFI_STATUS
GetCurrentUpdateProgress (
  IN OUT SYSTEM_FIRMWARE_UPDATE_PROGRESS     *CurrentUpdateProgress
  )
{
  EFI_STATUS                            Status;
  UINTN                                 VariableSize;

  if (CurrentUpdateProgress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if system is continuing from an interrupted update.
  //
  ZeroMem (CurrentUpdateProgress, sizeof (SYSTEM_FIRMWARE_UPDATE_PROGRESS));
  VariableSize = sizeof (SYSTEM_FIRMWARE_UPDATE_PROGRESS);

  Status = gRT->GetVariable (
                  SYSFW_UPDATE_PROGRESS_VARIABLE_NAME,
                  &gSysFwUpdateProgressGuid,
                  NULL,
                  &VariableSize,
                  CurrentUpdateProgress
                  );
  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Current UpdateProgress component = 0x%x.\n", CurrentUpdateProgress->Component));
  DEBUG ((DEBUG_INFO, "Current UpdateProgress progress  = 0x%x.\n", CurrentUpdateProgress->Progress));

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                            Status;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS       UpdateProgress;
  UINT16                                CapsuleStatusIndicater;
  UINTN                                 VariableSize;

  VariableSize = sizeof (SYSTEM_FIRMWARE_UPDATE_PROGRESS);
  ZeroMem (&UpdateProgress, VariableSize);

  Status = gRT->GetVariable (
                  SYSFW_UPDATE_PROGRESS_VARIABLE_NAME,
                  &gSysFwUpdateProgressGuid,
                  NULL,
                  &VariableSize,
                  &UpdateProgress
                  );

  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  UpdateProgress.Component = UpdatingComponent;
  UpdateProgress.Progress  = UpdatingProgress;

  CapsuleStatusIndicater = ((UINT16)0xF000) + (((UINT16)UpdatingComponent) << 8)
                         + (((UINT16)UpdatingProgress) << 4) + (UINT16)(IsTopSwapEnabled());
  //
  // show the postcode to let validation aware the update status.
  //
  PostCode((UINT32)CapsuleStatusIndicater);

  // add debug information of UpdatingComponent, UpdatingProgress
  DEBUG ((DEBUG_INFO, "Set UpdateProgress component = 0x%x.\n", UpdateProgress.Component));
  DEBUG ((DEBUG_INFO, "Set UpdateProgress progress  = 0x%x.\n", UpdateProgress.Progress));

  Status = gRT->SetVariable (
                  SYSFW_UPDATE_PROGRESS_VARIABLE_NAME,
                  &gSysFwUpdateProgressGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  VariableSize,
                  &UpdateProgress
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Clear update progress in NV storage. This indicates that no FW update process is happening.

**/
VOID
ClearUpdateProgress (
  VOID
  )
{
  EFI_STATUS   Status;
  //
  // F07F to show the status of end of capsule update
  //
  PostCode(0xF07F);
  //
  // Delete progress variable
  //
  Status = gRT->SetVariable (
                  SYSFW_UPDATE_PROGRESS_VARIABLE_NAME,
                  &gSysFwUpdateProgressGuid,
                  0,
                  0,
                  NULL
                  );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_FOUND)) {
    ASSERT_EFI_ERROR (Status);
  }

}

/**
  The constructor function of SeamlessRecoverySupportLib.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor successfully .
**/
EFI_STATUS
EFIAPI
SeamlessRecoverySupportLibConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS     Status;
  UINTN          VariableSize;
  UINTN          Index;
  EDKII_VARIABLE_POLICY_PROTOCOL   *VariablePolicy = NULL;

  //
  // Check if system is continuing from an interrupted update.
  //
  ZeroMem (&mPreviousUpdateProgress, sizeof (SYSTEM_FIRMWARE_UPDATE_PROGRESS));
  VariableSize = sizeof (SYSTEM_FIRMWARE_UPDATE_PROGRESS);

  Status = gRT->GetVariable (
                  SYSFW_UPDATE_PROGRESS_VARIABLE_NAME,
                  &gSysFwUpdateProgressGuid,
                  NULL,
                  &VariableSize,
                  &mPreviousUpdateProgress
                  );

  if ((Status != EFI_NOT_FOUND) && (Status != EFI_SUCCESS)) {
    DEBUG ((DEBUG_INFO, "Get UpdateProgress variable = %r.\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "Last boot UpdateProgress component = 0x%x.\n", mPreviousUpdateProgress.Component));
  DEBUG ((DEBUG_INFO, "Last boot UpdateProgress progress  = 0x%x.\n", mPreviousUpdateProgress.Progress));

  //
  // Lock variables related to update progress and backup file.
  //
  Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID**)&VariablePolicy);
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < ARRAY_SIZE (mRecoveryVariables); Index++) {
      Status = RegisterBasicVariablePolicy (VariablePolicy,
                 &gSysFwUpdateProgressGuid,
                 mRecoveryVariables[Index],
                 VARIABLE_POLICY_NO_MIN_SIZE,
                 VARIABLE_POLICY_NO_MAX_SIZE,
                 VARIABLE_POLICY_NO_MUST_ATTR,
                 VARIABLE_POLICY_NO_CANT_ATTR,
                 VARIABLE_POLICY_TYPE_LOCK_NOW
                 );
    }
  }

  return EFI_SUCCESS;
}
