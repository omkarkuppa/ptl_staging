/** @file
  Intel One Click Recovery File Support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <OneClickRecovery.h>
#include "OcrBootOptionSupport.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

// File Path to Windows recovery BCD
STATIC CHAR16  *mWinReBcdFileName = L"\\EFI\\Microsoft\\Recovery\\BCD";

// File Path to Windows Boot Manager
STATIC CHAR16  *mWinBootMgFileName = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";

/**
  Checks to see if file exist in file system

  @param[in]    RootFs            Root File System
  @param[in]    FileName          File Name
  @param[out]   FileExist         File Exist

  @retval  EFI_SUCCESS            Successfully determine if file exist or not
  @retval  EFI_OUT_OF_RESOURCES   Ran out of resources
**/
EFI_STATUS
DoesFileExist (
  IN  EFI_FILE                        *RootFs,
  IN  CHAR16                          *FileName,
  OUT BOOLEAN                         *FileExist
  )
{
  EFI_STATUS                     Status;
  UINTN                          ScratchBufferSize;
  EFI_FILE                       *FileHandle;
  EFI_FILE_INFO                  *FileInfo;

  FileInfo       = NULL;
  FileHandle     = NULL;
  *FileExist     = FALSE;

  Status = RootFs->Open (
                     RootFs,
                     &FileHandle,
                     FileName,
                     EFI_FILE_MODE_READ,
                     0
                     );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //Get size of file info
  ScratchBufferSize = 0;
  Status = FileHandle->GetInfo (
                         FileHandle,
                         &gEfiFileInfoGuid,
                         &ScratchBufferSize,
                         NULL
                         );
  if (EFI_ERROR (Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }

  FileInfo = AllocateZeroPool (ScratchBufferSize);
  if (FileInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = FileHandle->GetInfo (
                         FileHandle,
                         &gEfiFileInfoGuid,
                         &ScratchBufferSize,
                         FileInfo
                         );
  if (EFI_ERROR (Status)) {
    FreePool (FileInfo);
    return Status;
  }

  if (FileInfo->FileSize > 0) {
    *FileExist = TRUE;
  }

  Status = FileHandle->Close (FileHandle);
  FreePool (FileInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Can not close the token file !\n"));
  }

  return Status;
}

/**
  Finds All WinRe Boot Options and adds them as boot options

  @param[in, out] UefiBootOptions      Boot Options
  @param[in, out] NumUefiBootOptions   Number of UEFI boot options

  @retval EFI_SUCCESS                  Added winre Boot Option if exist
  @retval EFI_BUFFER_TOO_SMALL         Not enough room to add boot option
  @retval EFI_INVALID_PARAMETER        parameters are null pointers
**/
EFI_STATUS
AddWinReBootOptions (
  IN OUT UEFI_BOOT_OPTION     *UefiBootOptions,
  IN OUT UINT16               *NumUefiBootOptions
  )
{
  EFI_STATUS                      Status;
  UINTN                           Index;
  UINTN                           HandleCount;
  EFI_HANDLE                      *HandleBuffer;
  EFI_DEVICE_PATH_PROTOCOL        *HandleFilePath;
  EFI_FILE                        *RootFs;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
  EFI_BLOCK_IO_PROTOCOL           *BlkIo;
  BOOLEAN                         FileExist;
  CHAR16                          *DevicePath;
  CHAR16                          WinReFullPath[MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH];
  CHAR16                          WinReDescription[MAX_UEFI_BOOT_OPTION_DESC_LENGTH];

  HandleFilePath = NULL;

  if ((UefiBootOptions == NULL) || (NumUefiBootOptions == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CreateOcrBootOptionDescription (L"WinRe", WinReDescription, MAX_UEFI_BOOT_OPTION_DESC_LENGTH);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "OCR - Find WinRe Boot Options\n"));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot Locate SimpleFileSystemProtocol\n"));
    return Status;
  }

  //
  // Find and open the file in removable media disk.
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiBlockIoProtocolGuid,
                    (VOID **) &BlkIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (BlkIo->Media->MediaPresent) {
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiSimpleFileSystemProtocolGuid,
                      (VOID **) &SimpleFileSystem
                      );
      if (EFI_ERROR (Status)) {
        continue;
      }

      Status = SimpleFileSystem->OpenVolume (
                                   SimpleFileSystem,
                                   &RootFs
                                   );
      if (EFI_ERROR (Status)) {
        continue;
      }

      // Look for WinRe BCD File and Windows boot manager
      Status = DoesFileExist (RootFs, mWinReBcdFileName, &FileExist);
      if (EFI_ERROR (Status) || !FileExist) {
        continue;
      }

      Status = DoesFileExist (RootFs, mWinBootMgFileName, &FileExist);
      if (EFI_ERROR (Status) || !FileExist) {
        continue;
      }

      Status =  gBS->HandleProtocol(
                       HandleBuffer[Index],
                       &gEfiDevicePathProtocolGuid,
                       (VOID **) &HandleFilePath
                       );
      if (EFI_ERROR (Status) || (HandleFilePath == NULL)) {
        continue;
      }

      DevicePath = ConvertDevicePathToText (HandleFilePath, FALSE, TRUE);

      if (DevicePath != NULL) {
        if (*NumUefiBootOptions < MAX_UEFI_BOOT_OPTIONS) {
          // Add WinRe Boot Option to Boot Option List
          UefiBootOptions[*NumUefiBootOptions].UefiBootOptionType = WinRe;

          UnicodeStrToAsciiStrS (
            WinReDescription,
            (CHAR8 *) UefiBootOptions[*NumUefiBootOptions].Description,
            MAX_UEFI_BOOT_OPTION_DESC_LENGTH
            );

          UefiBootOptions[*NumUefiBootOptions].DescriptionLength = (UINT16)StrnLenS (WinReDescription, MAX_UEFI_BOOT_OPTION_DESC_LENGTH);

          UnicodeSPrint (
            WinReFullPath,
            MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH*sizeof(CHAR16),
            L"%s/%s",
            DevicePath,
            mWinBootMgFileName
            );

          UnicodeStrToAsciiStrS (
            WinReFullPath,
            (CHAR8 *) UefiBootOptions[*NumUefiBootOptions].EfiDevicePath,
            MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH
            );

          UefiBootOptions[*NumUefiBootOptions].DevicePathLength = (UINT16)StrnLenS (WinReFullPath, MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH);
          (*NumUefiBootOptions)++;
        } else {
          DEBUG ((DEBUG_ERROR, "No free space to put WinRe Boot Option\n"));
          Status = EFI_BUFFER_TOO_SMALL;
          FreePool (HandleBuffer);
          return Status;
        }

      }
    }
  }
  FreePool (HandleBuffer);
  return EFI_SUCCESS;
}
