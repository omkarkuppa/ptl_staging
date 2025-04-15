/** @file
  This is the DXE driver that supports Cnv compatibilty.

#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.

@par Specification Reference:
**/
#include "CnvCompatibilityDxe.h"

STATIC CHAR16  *mFileName = L"FvCnvUnCompact.Fv";

/**
  This function will unload existing CNV driver which was loaded from flash.

  @param DriverGuid       The Existing DriverGuid Which should Unload

 **/
VOID
EFIAPI
UnloadCnvDriver (
  IN EFI_GUID  *DriverGuid
  )
{
  EFI_STATUS                 Status;
  UINTN                      Index;
  UINTN                      DriverImageHandleCount;
  EFI_HANDLE                 *DriverImageHandleBuffer;
  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage;
  EFI_GUID                   *NameGuid;
  EFI_DEVICE_PATH_PROTOCOL   *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL   *LastDeviceNode;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiLoadedImageProtocolGuid,
                  NULL,
                  &DriverImageHandleCount,
                  &DriverImageHandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  for (Index = 0; Index < DriverImageHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    DriverImageHandleBuffer[Index],
                    &gEfiLoadedImageProtocolGuid,
                    (VOID *)&LoadedImage
                    );
    if (LoadedImage->FilePath == NULL) {
      continue;
    }

    TempDevicePath = LoadedImage->FilePath;
    LastDeviceNode = TempDevicePath;
    while (!IsDevicePathEnd (TempDevicePath)) {
      LastDeviceNode = TempDevicePath;
      TempDevicePath = NextDevicePathNode (TempDevicePath);
    }

    NameGuid = EfiGetNameGuidFromFwVolDevicePathNode (
                 (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *)LastDeviceNode
                 );
    if ((NameGuid != NULL) && (CompareGuid (NameGuid, DriverGuid))) {
      DEBUG ((DEBUG_INFO, "UnloadImage %g \n", NameGuid));
      Status = gBS->UnloadImage (DriverImageHandleBuffer[Index]);
      DEBUG ((DEBUG_INFO, "UnloadImage  Status %r\n", Status));
      break;
    }
  }

  FreePool (DriverImageHandleBuffer);
}

/**
  Cnv Compatibility Entry Point

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver

**/
EFI_STATUS
EFIAPI
CnvCompatibleEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;
  EFI_GUID    *Guid;
  UINTN       Count;
  UINTN       Index;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Handle = NULL;

  //
  // Check whether CnvCompatibility Check enabled through PcdCnvDispatch
  //
  if ((PcdGetBool (PcdCnvDispatch))) {
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gCnvDispatchProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);
  } else {
    DEBUG ((DEBUG_INFO, "CnvCompatibility Check is Disabled\n"));
    Status = EFI_SUCCESS;
    return Status;
  }

  Status = IsSupportedCnvPresent ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Supported CNV device not found - %r\n", Status));
    return Status;
  }

  Status = IsCnvDriverPresent ((EFI_GUID *)PcdGetPtr (PcdCnvCompatibilityGuid));
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Supporting CNVi driver already available in the flash\n"));
    return Status;
  }

  if (PcdGetBool (PcdCnvUnloadRequirement)) {
    //
    // Unload existing Drivers and load from ESP
    //
    Guid  = PcdGetPtr (PcdUnloadDriverGuids);
    Count = PcdGetSize (PcdUnloadDriverGuids) / sizeof (EFI_GUID);

    for (Index = 0; Index < Count; Index++, Guid++) {
      UnloadCnvDriver (Guid);
    }
  }

  //
  // Load Fv from External Storage Device
  //
  LoadCnvDriverFromFv ();
  return EFI_SUCCESS;
}

/**
 This function is to check whether supported CNV device is present

 @retval EFI_SUCCESS             The function completed successfully
 @retval EFI_NOT_FOUND           Supported CNV device is not found

**/
EFI_STATUS
IsSupportedCnvPresent (
  VOID
  )
{
  UINTN                SizeData;
  UINTN                Index;
  UINTN                CnvSubIdIndex;
  EFI_HANDLE           *PciHandles;
  UINTN                PciHandlesSize;
  EFI_PCI_IO_PROTOCOL  *PciIo;
  PCI_TYPE00           PciConfig;
  EFI_STATUS           Status;
  UINT16               *CnvSubSystemId;
  UINTN                Count;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  PciHandles     = NULL;
  PciHandlesSize = 0;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciHandlesSize,
                  &PciHandles
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate protocol handle\n"));
    return Status;
  }

  for (Index = 0; Index < PciHandlesSize; Index++) {
    Status = gBS->HandleProtocol (
                    PciHandles[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    if ((!EFI_ERROR (Status)) && (PciIo != NULL)) {
      Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint32,
                            0,
                            sizeof (PciConfig) / sizeof (UINT32),
                            &PciConfig
                            );
      if ((!EFI_ERROR (Status)) && (PciConfig.Hdr.VendorId == INTEL_VENDOR_ID)) {
        if (IS_CLASS2 (&PciConfig, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER)) {
          CnvSubSystemId  = PcdGetPtr (PcdCnvSubSystemId);
          Count = PcdGetSize (PcdCnvSubSystemId)/sizeof (UINT16);
          for (CnvSubIdIndex = 0; CnvSubIdIndex < Count; CnvSubIdIndex++, CnvSubSystemId++) {
            if (PciConfig.Device.SubsystemID == *(CnvSubSystemId)) {
              PcdSet8S (PcdCnvPresent, 1);
              break;
            } else {
              PcdSet8S (PcdCnvPresent, 2);  // looking for external HardDisk
            }
          }
        }
      }
    }
  }

  SizeData = sizeof (EFI_GUID);

  switch (PcdGet8 (PcdCnvPresent)) {
    case 1:
      PcdSetPtrS (PcdCnvCompatibilityGuid, &SizeData, &gCnvCompatibilityGuid);
      break;
    case 2:
      PcdSetPtrS (PcdCnvCompatibilityGuid, &SizeData, &gCnvExternalGuid);
      break;
    default:
      DEBUG ((DEBUG_INFO, "CNVi 1/2/3 is not present \n"));
      Status = EFI_NOT_FOUND;
      return Status;
  }

  DEBUG ((DEBUG_INFO, "PcdCnvCompatibilityGuid is  %g\n", *(EFI_GUID *)PcdGetPtr (PcdCnvCompatibilityGuid)));

  //
  // Free any allocated buffers
  //
  FreePool (PciHandles);
  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return Status;
}

/**
  This function to check weather Cnv Driver present

  @param KeyGuid                   Guid to check is CnvDriverPresent

  @retval EFI_SUCCESS              Cnv Driver is Present
  @retval EFI_NOT_FOUND            Cnv Driver is not present

**/
EFI_STATUS
IsCnvDriverPresent (
  EFI_GUID  *KeyGuid
  )
{
  EFI_STATUS                          Status;
  UINTN                               HandleCount;
  EFI_HANDLE                          *HandleBuffer;
  UINTN                               Index;
  UINTN                               Handle;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Instance;
  EFI_FIRMWARE_VOLUME_HEADER          *FwVolHeader;
  EFI_HANDLE                          FvHandle;
  UINT32                              ExtHeaderOffset;
  EFI_FV_BLOCK_MAP_ENTRY              *BlockMap;
  UINTN                               LbaOffset;
  EFI_LBA                             LbaIndex;
  EFI_GUID                            FvNameGuid;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolumeBlockProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, " LocateHandleBuffer Status %r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "Volume Block handle Count is  %d\n", HandleCount));
  for (Handle = 0; Handle < HandleCount; Handle++) {
    FvHandle = HandleBuffer[Handle];
    Status   = gBS->HandleProtocol (
                      FvHandle,
                      &gEfiFirmwareVolumeBlockProtocolGuid,
                      (VOID **)&Instance
                      );

    Status = CnvGetFwVolHeader (Instance, &FwVolHeader);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "CnvGetFwVolHeader Status  %r\n", Status));
    }

    if (CnvVerifyFvHeaderChecksum (FwVolHeader) && (FwVolHeader->ExtHeaderOffset != 0)) {
      ExtHeaderOffset = (UINT32)FwVolHeader->ExtHeaderOffset;
      BlockMap        = FwVolHeader->BlockMap;
      LbaIndex        = 0;
      LbaOffset       = 0;

      //
      // Find LbaIndex and LbaOffset for FV extension header based on BlockMap.
      //
      while ((BlockMap->NumBlocks != 0) || (BlockMap->Length != 0)) {
        for (Index = 0; Index < BlockMap->NumBlocks && ExtHeaderOffset >= BlockMap->Length; Index++) {
          ExtHeaderOffset -= BlockMap->Length;
          LbaIndex++;
        }

        //
        // Check whether FvExtHeader is crossing the multi block range.
        //
        if (Index < BlockMap->NumBlocks) {
          LbaOffset = ExtHeaderOffset;
          break;
        }

        BlockMap++;
      }

      //
      // Read FvNameGuid from FV extension header.
      //
      Status = CnvReadFvbData (Instance, &LbaIndex, &LbaOffset, sizeof (FvNameGuid), (UINT8 *)&FvNameGuid);
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "in ReadFvbData Guid is %g\n", *(EFI_GUID *)(&FvNameGuid)));
        if (CompareGuid ((EFI_GUID *)&FvNameGuid, KeyGuid)) {
          DEBUG ((DEBUG_INFO, "In FindFvByGuid Compare guid is true and FvnameGuid is %g\n", FvNameGuid));
          DEBUG ((DEBUG_INFO, "In FindFvByGuid Compare guid is true and KeyGuid is  %g\n", KeyGuid));
          FreePool (HandleBuffer);
          return EFI_SUCCESS;
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  FreePool (HandleBuffer);
  return EFI_NOT_FOUND;
}

/**
  This Function used to load Cnv Driver Loaded from  EFI System partition

  @param FvBaseAddress          The Start Adress where Fv is pointed
  @param FvSize                 The Size of Firmware Volume

 **/
VOID
EFIAPI
LoadCnvDriver (
  IN UINT8  *FvBaseAddress,
  IN UINTN  FvSize
  )
{
  EFI_STATUS  Status;
  UINT8       *FvEnd;
  EFI_HANDLE  FvHandle;
  UINTN       Size;

  FvEnd = (FvBaseAddress + ((EFI_FIRMWARE_VOLUME_HEADER *)FvBaseAddress)->FvLength);
  while (FvBaseAddress < FvEnd) {
    if (((EFI_FIRMWARE_VOLUME_HEADER *)FvBaseAddress)->Signature != EFI_FVH_SIGNATURE) {
      FvBaseAddress += 0x10;
      continue;
    }

    Size   = ((EFI_FIRMWARE_VOLUME_HEADER *)FvBaseAddress)->FvLength;
    Status = gDS->ProcessFirmwareVolume (
                    (VOID *)(EFI_FIRMWARE_VOLUME_HEADER *)FvBaseAddress,
                    Size,
                    &FvHandle
                    );
    DEBUG ((DEBUG_INFO, "ProcessFirmwareVolume Status is %r\n", Status));
    DEBUG ((DEBUG_INFO, "ProcessFirmwareVolume Fv handle value is 0x%x \n", FvHandle));
    break;
  }
}

/**
  Find and Load Cnv driver from fs0: to RAM and then call LoadCnvDriver()

  @retval EFI_SUCCESS           Successfully Loaded Fv from External Harddisk

  @retval EFI_NOT_FOUND         Unable to Load from Fv

**/
EFI_STATUS
EFIAPI
LoadCnvDriverFromFv (
  VOID
  )
{
  UINTN                            Index = 0;
  EFI_STATUS                       Status;
  EFI_BLOCK_IO_PROTOCOL            *BlkIo;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *SimpleFileSystem;
  EFI_FILE                         *RootFs;
  UINTN                            ScratchBufferSize;
  EFI_FILE                         *FileHandle;
  EFI_FILE_INFO                    *FileInfo;
  UINTN                            NumSimpleFsHandles;
  EFI_HANDLE                       *SimpleFsHandle;
  EFI_DEVICE_PATH_PROTOCOL         *FsDevicePath;
  EFI_DEVICE_PATH_PROTOCOL         *TempDevicePath;
  EFI_FIRMWARE_VOLUME_HEADER       *Fvh;
  UINT8                            *FvAddress;
  UINTN                            AlignAddress;
  UINTN                            BufferSize;
  EFI_PHYSICAL_ADDRESS             BaseAddress;
  EFI_GUID                         *FvGuid;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &NumSimpleFsHandles,
                  &SimpleFsHandle
                  );
  DEBUG ((DEBUG_INFO, "Locate SFS Protocol. - Status=%r NumSimpleFsHandles = %d\n", Status, NumSimpleFsHandles));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Cannot Locate SFS protocol - Status=%r\n", Status));
    return Status;
  }

  for (Index = 0; Index < NumSimpleFsHandles; Index++) {
    Status = gBS->HandleProtocol (
                    SimpleFsHandle[Index],
                    &gEfiBlockIoProtocolGuid,
                    (VOID **)&BlkIo
                    );
    DEBUG ((DEBUG_INFO, "Handle BlockIo protocol Status=%r, BlkIo->Media->MediaPresent  - 0x%x \n", Status, BlkIo->Media->MediaPresent));
    ASSERT_EFI_ERROR (Status);

    //
    // Get the device path for fileSystem.
    //
    FsDevicePath   = DevicePathFromHandle (SimpleFsHandle[Index]);
    TempDevicePath = FsDevicePath;
    while (!IsDevicePathEnd (TempDevicePath)) {
      if ((DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH) &&
          (DevicePathSubType (TempDevicePath) == MSG_USB_DP))
      {
        break;
      }

      TempDevicePath = NextDevicePathNode (TempDevicePath);
    }

    if ((BlkIo->Media->MediaPresent) && (DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH) &&
        (DevicePathSubType (TempDevicePath) == MSG_USB_DP))
    {
      Status = gBS->HandleProtocol (
                      SimpleFsHandle[Index],
                      &gEfiSimpleFileSystemProtocolGuid,
                      (VOID **)&SimpleFileSystem
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Cannot handle SFS protocol. - Status=%r\n", Status));
        FreePool (SimpleFsHandle);
        return Status;
      }

      Status = SimpleFileSystem->OpenVolume (
                                   SimpleFileSystem,
                                   &RootFs
                                   );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Cannot open volume Status - %r \n", Status));
        FreePool (SimpleFsHandle);
        return Status;
      }

      Status = RootFs->Open (
                         RootFs,
                         &FileHandle,
                         mFileName,
                         EFI_FILE_MODE_READ,
                         0
                         );
      if (Status == EFI_NOT_FOUND) {
        DEBUG ((DEBUG_INFO, "File %s does not exist.\n", mFileName));
        FreePool (SimpleFsHandle);
        return Status;
      } else if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Cannot open file: %s. Status = %r\n", mFileName, Status));
        FreePool (SimpleFsHandle);
        return Status;
      }

      // Get size of file info
      ScratchBufferSize = 0;
      FileInfo          = NULL;
      Status            = FileHandle->GetInfo (
                                        FileHandle,
                                        &gEfiFileInfoGuid,
                                        &ScratchBufferSize,
                                        FileInfo
                                        );
      if (EFI_ERROR (Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        DEBUG ((DEBUG_INFO, "Get Info Failure. Status=%r\n", Status));
        FreePool (SimpleFsHandle);
        return Status;
      }

      FileInfo = AllocateZeroPool (ScratchBufferSize);
      if (FileInfo == NULL) {
        DEBUG ((DEBUG_INFO, "FileInfo value is NULL \n"));
        FreePool (SimpleFsHandle);
        return Status;
      }

      Status = FileHandle->GetInfo (
                             FileHandle,
                             &gEfiFileInfoGuid,
                             &ScratchBufferSize,
                             FileInfo
                             );
      BufferSize = FileInfo->FileSize + 0x10;

      FvAddress = AllocatePool (BufferSize);
      if (FvAddress ==  NULL) {
        FreePool (SimpleFsHandle);
        FreePool (FileInfo);
        return Status;
      }

      AlignAddress = (UINTN)FvAddress;
      AlignAddress = 0x10 - (AlignAddress & 0x0000000F);
      FvAddress    = FvAddress + AlignAddress;

      Status = FileHandle->Read (
                             FileHandle,
                             &BufferSize,
                             FvAddress
                             );
      BaseAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)FvAddress;
      Fvh         = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)(EFI_PHYSICAL_ADDRESS)FvAddress;
      FvGuid      = (GUID *)(UINTN)(BaseAddress + Fvh->ExtHeaderOffset);

      if (CompareGuid (FvGuid, (EFI_GUID *)PcdGetPtr (PcdCnvCompatibilityGuid))) {
        LoadCnvDriver (FvAddress, BufferSize);
        FreePool (SimpleFsHandle);
        FreePool (FileInfo);
        return EFI_SUCCESS;
      } else {
        FreePool (SimpleFsHandle);
        FreePool (FileInfo);
        return EFI_NOT_FOUND;
      }
    }
  }

  FreePool (SimpleFsHandle);
  return EFI_SUCCESS;
}
