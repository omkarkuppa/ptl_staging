/**@file
  Ufs Erase Driver.

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

#include "UfsEraseDxe.h"

//
// UFS definition
//
#define UFS_SANITIZE_TIME_CLEAR_128M_MIN  1  // Minumum 1 second to clear 128M UFS media
#define UFS_SANITIZE_TIME_CLEAR_128M_MAX  2  // Maximum 2 second to clear 128M UFS media
#define UFS_SANITIZE_TIME_UNMAP           3  // Aboud 3 second to unmap UFS media

/**
  Search protocol database and check to see if the protocol
  specified by ProtocolGuid is present on a ControllerHandle and opened by
  ChildHandle with an attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.
  If the ControllerHandle is found, then the protocol specified by ProtocolGuid
  will be opened on it.

  @param  ProtocolGuid   ProtocolGuid pointer.
  @param  ChildHandle    Child Handle to retrieve Parent information.

**/
VOID *
GetParentProtocol (
  IN  EFI_GUID    *ProtocolGuid,
  IN  EFI_HANDLE  ChildHandle
  )
{
  UINTN       Index;
  UINTN       HandleCount;
  VOID        *Interface;
  EFI_STATUS  Status;
  EFI_HANDLE  *HandleBuffer;

  //
  // Retrieve the list of all handles from the handle database
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  ProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  //
  // Iterate to find who is parent handle that is opened with ProtocolGuid by ChildHandle
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = EfiTestChildHandle (HandleBuffer[Index], ChildHandle, ProtocolGuid);
    if (!EFI_ERROR (Status)) {
      Status = gBS->HandleProtocol (HandleBuffer[Index], ProtocolGuid, (VOID **)&Interface);
      if (!EFI_ERROR (Status)) {
        gBS->FreePool (HandleBuffer);
        return Interface;
      }
    }
  }

  gBS->FreePool (HandleBuffer);
  return NULL;
}

/**
  Check if EFI_EXT_SCSI_PASS_THRU_PROTOCOL or EFI_SCSI_PASS_THRU_PROTOCOL exist on the parent handle.

  @param  ChildHandle  Child Handle to retrieve Parent information.

  @retval  TRUE    Should produce Block Io & Block Io2.
  @retval  FALSE   Should not produce Block Io & Block Io2.

**/
BOOLEAN
IsUfsDevice (
  IN  EFI_HANDLE ChildHandle
  )
{
  EFI_EXT_SCSI_PASS_THRU_PROTOCOL  *ExtScsiPassThru;

  //
  // Firstly, check if ExtScsiPassThru Protocol parent handle exists. If existence,
  // check its attribute, logic or physical.
  //
  ExtScsiPassThru = (EFI_EXT_SCSI_PASS_THRU_PROTOCOL *)GetParentProtocol (&gEfiExtScsiPassThruProtocolGuid, ChildHandle);
  if (ExtScsiPassThru != NULL) {
    return TRUE;
  }
  return FALSE;
}

/**
  Dump the data from given buffer

  @param[in] Buffer  The buffer which content will be exposed.
  @param[in] Size    Size of buffer
**/
VOID
DumpUfsBuffer(
  IN UINT8   *Buffer,
  IN UINT32  Size
  )
{
  UINT64     row = 0;
  UINT8      col = 0;
  UINT64     Offset = 0;

  if((Buffer == NULL) || (Size == 0)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "Dump UFS LU\n"));
  DEBUG ((DEBUG_INFO, "========================\n"));
  for (row = 0; row <= 0xf; row++) {
    DEBUG ((DEBUG_INFO, "%.8xh: ", Offset));
    for(col = 0; col <= 0xf; col++) {
      DEBUG ((DEBUG_INFO, "%.2x ", *(Buffer+Offset)));
      Offset++;
      if(Offset >= Size)
        break;
    }
    DEBUG ((DEBUG_INFO, "\n"));
    if(Offset >= Size)
      break;
  }
  DEBUG ((DEBUG_INFO, " ========================\n"));
}

/**
  Perform clear operation on specific UFS device.

  @param[in] Private    Pointer to the device private data.
  @param[in] MediaId    The media ID that the write request is for.
  @param[in] Lba        The starting logical block address to be written.
                        The caller is responsible for writing to only
                        legitimate locations.
  @param[in,out] Token  A pointer to the token associated with the
                        transaction.
  @param[in] Size       Size data will be cleared.
  @param[in] Type       Type is used to define which value to be used to override data on the device.

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some error

**/
EFI_STATUS
UfsDataClear (
  IN     UFS_ERASE_PRIVATE_DATA  *Private,
  IN     UINT32                  MediaId,
  IN     EFI_LBA                 Lba,
  IN OUT UFS_ERASE_BLOCK_TOKEN   *Token,
  IN     UINTN                   Size,
  IN     UFS_CLEAR_TYPE          Type
  )
{
  EFI_STATUS             Status;
  EFI_BLOCK_IO_PROTOCOL  *BlkIo;
  EFI_BLOCK_IO_MEDIA     *Media;
  UINTN                  BlockSize;
  UINTN                  NumberOfBlocks;
  UINTN                  RemainingSize;
  UINTN                  BufferSize;
  VOID                   *Buffer;
  EFI_LBA                Block;

  DEBUG ((DEBUG_INFO, "%a: MediaId=%d, Lba=%8x, Size=%016lx. \n" , __FUNCTION__, MediaId, Lba, Size));

  Status = gBS->HandleProtocol (Private->Controller, &gEfiBlockIoProtocolGuid, (VOID **) &BlkIo);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: not support BlockIO: %r \n" , __FUNCTION__, Status));
    return Status;
  }

  Media = BlkIo->Media;

  DEBUG ((DEBUG_INFO, "%a: LastBlock=%8x, BlockSize=%8x\n" , __FUNCTION__, Media->LastBlock, Media->BlockSize));
  if (!(Media->MediaPresent) || (Media->LastBlock == 0)) {
    Status = EFI_NO_MEDIA;
    goto Done;
  }

  if (MediaId != Media->MediaId) {
    Status = EFI_MEDIA_CHANGED;
    goto Done;
  }

  if (Media->ReadOnly) {
    Status = EFI_WRITE_PROTECTED;
    goto Done;
  }

  if (Size == 0) {
    Size = (BlkIo->Media->LastBlock + 1) * (BlkIo->Media->BlockSize);
  }

  BlockSize = Media->BlockSize;
  if ((Size % BlockSize) != 0) {
    Status = EFI_INVALID_PARAMETER;
    goto Done;
  }

  NumberOfBlocks = Size / BlockSize;
  if ((Lba + NumberOfBlocks - 1) > Media->LastBlock) {
    Status = EFI_INVALID_PARAMETER;
    goto Done;
  }

  RemainingSize = Size;
  Status = EFI_SUCCESS;
  Block  = 0;
  while(RemainingSize > 0) {
    if(RemainingSize > SIZE_4GB) {
      Block = SIZE_4GB / BlkIo->Media->BlockSize;
      BufferSize = Block * BlkIo->Media->BlockSize;
    } else {
      BufferSize = RemainingSize;
      Block = RemainingSize / BlkIo->Media->BlockSize;
    }
    RemainingSize -= BufferSize;
    Buffer = AllocatePool (BufferSize);
    if (Buffer == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Done;
    }

    //
    // Determine how to override data on the device
    //
    if (Type == UfsClearZero) {
      SetMem(Buffer, BufferSize, 0x0);
    } else if (Type == UfsClearOne) {
      SetMem(Buffer, BufferSize, 0xFF);
    }
    Status = BlkIo->WriteBlocks (BlkIo, BlkIo->Media->MediaId, Lba, BufferSize, Buffer);
    Lba += Block;
    FreePool(Buffer);
    if (EFI_ERROR(Status)) {
      break;
    }
  }
  BlkIo->FlushBlocks(BlkIo);
Done:
  return Status;
}

/**
  Perform override operation on specific UFS device.

  @param[in] Private    Pointer to the device private data.
  @param[in] MediaId    The media ID that the write request is for.
  @param[in] Lba        The starting logical block address to be written.
                        The caller is responsible for writing to only
                        legitimate locations.
  @param[in,out] Token  A pointer to the token associated with the
                        transaction.
  @param[in] Size       Size data will be cleared.

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some error

**/
EFI_STATUS
UfsDataOverwrite (
  IN     UFS_ERASE_PRIVATE_DATA  *Private,
  IN     UINT32                  MediaId,
  IN     EFI_LBA                 LBA,
  IN OUT UFS_ERASE_BLOCK_TOKEN   *Token,
  IN     UINTN                   Size
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Perform unmap operation on specific UFS device.

  @param[in] Private    Pointer to the device private data.
  @param[in] MediaId    The media ID that the write request is for.
  @param[in] Lba        The starting logical block address to be written.
                        The caller is responsible for writing to only
                        legitimate locations.
  @param[in,out] Token  A pointer to the token associated with the
                        transaction.
  @param[in] Size       Size data will be cleared.

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some error

**/
EFI_STATUS
UfsDataUnmap (
  IN     UFS_ERASE_PRIVATE_DATA  *Private,
  IN     UINT32                  MediaId,
  IN     EFI_LBA                 Lba,
  IN OUT UFS_ERASE_BLOCK_TOKEN   *Token,
  IN     UINTN                   Size
  )
{
  EFI_STATUS                Status;
  EFI_ERASE_BLOCK_PROTOCOL  *EraseBlockProtocol;
  EFI_BLOCK_IO_PROTOCOL     *BlkIo;
  EFI_BLOCK_IO_MEDIA        *Media;
  UINTN                     BlockSize;
  UINTN                     NumberOfBlocks;

  DEBUG ((DEBUG_INFO, "%a: MediaId=%d, Lba=%8x, Size=%016lx. \n" , __FUNCTION__, MediaId, Lba, Size));

  Status = gBS->HandleProtocol (Private->Controller, &gEfiBlockIoProtocolGuid, (VOID **) &BlkIo);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: not support BlockIO: %r \n" , __FUNCTION__, Status));
    return Status;
  }

  Status = gBS->HandleProtocol (Private->Controller, &gEfiEraseBlockProtocolGuid, (VOID **) &EraseBlockProtocol);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: not support EraseBlock: %r \n" , __FUNCTION__, Status));
    return Status;
  }

  Media = BlkIo->Media;
  if (!(Media->MediaPresent)) {
    Status = EFI_NO_MEDIA;
    goto Done;
  }

  if (MediaId != Media->MediaId) {
    Status = EFI_MEDIA_CHANGED;
    goto Done;
  }

  if (Media->ReadOnly) {
    Status = EFI_WRITE_PROTECTED;
    goto Done;
  }

  if (Size == 0) {
    Size = (BlkIo->Media->LastBlock + 1) * (BlkIo->Media->BlockSize);
  }

  BlockSize = Media->BlockSize;
  if ((Size % BlockSize) != 0) {
    Status = EFI_INVALID_PARAMETER;
    goto Done;
  }

  NumberOfBlocks = Size / BlockSize;
  if ((Lba + NumberOfBlocks - 1) > Media->LastBlock) {
    Status = EFI_INVALID_PARAMETER;
    goto Done;
  }

  Status = EraseBlockProtocol->EraseBlocks (EraseBlockProtocol, BlkIo->Media->MediaId, Lba, NULL, Size);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: EraseBlocks Status: %r \n" , __FUNCTION__, Status));
    goto Done;
  }

Done:
  return Status;
}

/**
  Store the result after performing erase operation.

  @param[in] Private      Pointer to the device private data.
  @param[in] EraseType    Erase type to be used to erase data on device.
  @param[in] EraseStatus  Result of erase operation.

**/
VOID
UpdateResult (
  IN UFS_ERASE_PRIVATE_DATA   *Private,
  IN UFS_ERASE_TYPE           EraseType,
  IN EFI_STATUS               EraseStatus
  )
{
  if (Private->Info == NULL) {
    return;
  }

  Private->Info->Method = EraseType;
  Private->Info->Result = EraseStatus;

  DEBUG((DEBUG_INFO, "Update result list (Method:%d, Result:%r)\n",
    Private->Info->Method, Private->Info->Result));

}

/**
  Erase a specified number of device blocks.

  @param[in] This       Indicates a pointer to the calling context.
  @param[in] MediaId    The media ID that the erase request is for.
  @param[in] Lba        The starting logical block address to be erased.
                        The caller is responsible for erasing only legitimate locations.
  @param[in,out] Token  A pointer to the token associated with the transaction.
  @param[in] Size       The size in bytes to be erased. This must be a multiple
                        of the physical block size of the
                                  device.
  @param[in] EraseType  Erase type to be used to erase data on device.
  @param[in] Progress   Callback function to inform caller the progress of erase process.

  @retval EFI_SUCCESS             The erase request was queued if Event is not
                                  NULL. The data was erased correctly to the
                                  device if the Event is NULL.to the device.
  @retval EFI_WRITE_PROTECTED     The device cannot be erased due to write
                                  protection.
  @retval EFI_DEVICE_ERROR        The device reported an error while attempting
                                  to perform the erase operation.
  @retval EFI_INVALID_PARAMETER   The erase request contains LBAs that are not
                                  valid.
  @retval EFI_NO_MEDIA            There is no media in the device.
  @retval EFI_MEDIA_CHANGED       The MediaId is not for the current media.

**/
EFI_STATUS
EFIAPI
UfsBlockEraseEx (
  IN     UFS_ERASE_BLOCK_PROTOCOL    *This,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     Lba,
  IN OUT UFS_ERASE_BLOCK_TOKEN       *Token,
  IN     UINTN                       Size,
  IN     UFS_ERASE_TYPE              EraseType,
  IN     UFS_ERASE_BLOCK_PROGRESS    Progress OPTIONAL
  )
{
  EFI_STATUS              Status;
  UFS_ERASE_PRIVATE_DATA  *Private;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "%a: EraseType:%d . \n", __FUNCTION__, EraseType));
  Private = PRIVATE_FROM_ERASEBLOCK(This);

  switch (EraseType) {
    case UfsClear:
      Status = UfsDataClear (Private, MediaId, Lba, Token, Size, UfsClearZero);
      break;
    case UfsOverwrite:
      Status = UfsDataOverwrite (Private, MediaId, Lba, Token, Size);
      break;
    case UfsBlockErase:
      Status = UfsDataUnmap (Private, MediaId, Lba, Token, Size);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  //
  // Update result
  //
  UpdateResult (Private, EraseType, Status);
  return Status;
}

/**
  Convert a Null-terminated Unicode string to a Null-terminated
  ASCII string.

  @param  Source        The pointer to a Null-terminated Unicode string.
  @param  Destination   The pointer to a Null-terminated ASCII string.
  @param  DestMax       The maximum number of Destination Ascii
                        char, including terminating null char.

  @retval EFI_SUCCESS           String is converted.
  @retval EFI_BUFFER_TOO_SMALL  If DestMax is NOT greater than StrLen(Source).
  @retval EFI_INVALID_PARAMETER If Destination is NULL.
                                If Source is NULL.
**/
EFI_STATUS
UfsUnicodeStrToAsciiStr(
  IN      CONST CHAR16              *Source,
  OUT     CHAR8                     *Destination,
  IN      UINTN                     DestMax
)
{
  UINTN            SourceLen;
  CHAR8   TempChar;

  if ((Source == NULL) || (Destination == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  SourceLen = StrnLenS (Source, DestMax);
  if (SourceLen > DestMax) {
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // convert string
  //
  while ((*Source != '\0') && (SourceLen > 0)) {
    //
    // If any Unicode characters for UFS, non-zero value in the low 8 bits.
    // upper 8 bits is the characters.
    //
    TempChar = (CHAR8)*Source;
    if (TempChar != 0x0) {
      break;
    }
    *(Destination++) = (CHAR8)(*Source >> 8);
    Source++;
    SourceLen--;
  }
  *Destination = '\0';

  return EFI_SUCCESS;
}

/**
  Get the String descriptor from UFS.

  @param  UfsDevConfig        The pointer to EFI_UFS_DEVICE_CONFIG_PROTOCOL.
  @param  Index               The index of the descriptor.
  @param  Descriptor          The String output from the String descriptor.
  @param  Size                The size of the output buffer.

  @retval EFI_SUCCESS           return the Descriptor string.
  @retval EFI_INVALID_PARAMETER If UfsDevConfig is NULL.
                                If Size is NULL.
                                If Descriptor is NULL
**/
EFI_STATUS
UfsGetDescriptorString (
  IN     EFI_UFS_DEVICE_CONFIG_PROTOCOL *UfsDevConfig,
  IN     UINT8                          Index,
  OUT    CHAR8                          *Descriptor,
  IN OUT UINT8                          *Size
  )
{
  EFI_STATUS                      Status;
  UINT32                          DescriptorSize;
  UFS_STRING_DESC                 StringDescriptor;
  UINT8                           DescriptorLength;

  if ((UfsDevConfig == NULL) || (Size == NULL) || (Descriptor == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  DescriptorSize = sizeof (UFS_STRING_DESC);
  Status = UfsDevConfig->RwUfsDescriptor (UfsDevConfig, TRUE, UfsStringDesc, Index, 0, (VOID*) &StringDescriptor, (UINT32*) &DescriptorSize);
  if (!EFI_ERROR (Status)) {
    DescriptorLength = *Size;
    if (StringDescriptor.Length < 2) {
      DescriptorLength = 0;
    } else if ((StringDescriptor.Length - 2) < (DescriptorLength*2)) {
      DescriptorLength = (StringDescriptor.Length - 2)/2;
    }

    Status = UfsUnicodeStrToAsciiStr (StringDescriptor.Unicode, Descriptor, (UINTN)DescriptorLength);
    if (!EFI_ERROR(Status)) {
      *Size = DescriptorLength;
    } else {
      SetMem (Descriptor, DescriptorLength, 0x0);
      *Size = 0;
    }
  }

  return Status;
}

/**
  Get the UFS Manufacturer, Serial and Product from descriptor.

  @param  Private        The pointer to UFS_ERASE_PRIVATE_DATA.

  @retval EFI_SUCCESS           return the Descriptor string.
  @retval EFI_INVALID_PARAMETER If UfsDevConfig is NULL.

**/
EFI_STATUS
UfsInitDeviceInfo (
  IN OUT UFS_ERASE_PRIVATE_DATA  *Private
  )
{
  EFI_STATUS                      Status;
  EFI_UFS_DEVICE_CONFIG_PROTOCOL  *UfsDevConfig;
  UINT32                          DescriptorSize;
  UFS_DEV_DESC                    DeviceDescriptor;
  UINT8                           DescriptorIndex;
  UINT8                           Size;

  if ((Private == NULL) || (Private->UfsDevConfig == NULL) || (Private->Info == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  UfsDevConfig = Private->UfsDevConfig;
  DescriptorSize = sizeof (UFS_DEV_DESC);
  Status = UfsDevConfig->RwUfsDescriptor (UfsDevConfig, TRUE, UfsDeviceDesc, 0, 0, (VOID*) &DeviceDescriptor, (UINT32*) &DescriptorSize);
  if (!EFI_ERROR (Status)) {
    DescriptorIndex = DeviceDescriptor.ManufacturerName;
    Size = sizeof (Private->Info->Manufacturer) - 1;
    Status = UfsGetDescriptorString (UfsDevConfig, DescriptorIndex, Private->Info->Manufacturer, (UINT8*)&Size);

    DescriptorIndex = DeviceDescriptor.SerialName;
    Size = sizeof (Private->Info->SerialNumber) - 1;
    Status = UfsGetDescriptorString (UfsDevConfig, DescriptorIndex, Private->Info->SerialNumber, (UINT8*)&Size);

    DescriptorIndex = DeviceDescriptor.ProductName;
    Size = sizeof (Private->Info->ModelName) - 1;
    Status = UfsGetDescriptorString (UfsDevConfig, DescriptorIndex, Private->Info->ModelName, (UINT8*)&Size);
  }

  return Status;
}

/**
  Return the result of erase operation on the device.

  @param[in]  This          Indicates a pointer to the calling context.
  @param[out] EraseInfo     A pointer to the data of erasing device information.

  @retval EFI_SUCCESS             Get result of erase operation successfully.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetUfsEraseInfo (
  IN  UFS_ERASE_BLOCK_PROTOCOL  *This,
  OUT VOID                      **EraseInfo
  )
{
  UFS_ERASE_PRIVATE_DATA      *Private;
  UFS_ERASE_INFO              *FinalInfo;

  if ((This == NULL) || (EraseInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  Private = PRIVATE_FROM_ERASEBLOCK(This);

  if ((Private == NULL) || (Private->UfsDevConfig == NULL)) {
    return EFI_NOT_FOUND;
  }

  if (Private->Info != NULL) {
    FinalInfo = AllocateZeroPool (sizeof (UFS_ERASE_INFO));
    if (FinalInfo == NULL) {
      return EFI_NOT_FOUND;
    }
    CopyMem (FinalInfo, Private->Info, sizeof (UFS_ERASE_INFO));
    *EraseInfo = FinalInfo;
    return EFI_SUCCESS;
  }
  DEBUG((DEBUG_INFO, "%a: no information can be found. \n",  __FUNCTION__));
  return EFI_NOT_FOUND;
}

/**
  Set the estimate time.

  @param[in] Private    Pointer to the UFS_ERASE_PRIVATE_DATA.

**/
VOID
SetUfsEstimateTime (
  IN UFS_ERASE_PRIVATE_DATA  *Private
  )
{
  UINT8                     Index;
  EFI_STATUS                Status;
  EFI_BLOCK_IO_PROTOCOL     *BlkIo;
  EFI_ERASE_BLOCK_PROTOCOL  *EraseBlockProtocol;
  UINT64                    Time;
  UINT32                    BlockNumber;

  BlkIo = NULL;
  Time  = 0;
  if (Private == NULL) {
    return;
  }

  for (Index = 0; Index < UFS_SANITIZE_COUNT; Index ++) {
    switch (Index + 1) {
      case UfsClear:
      {
        Status = gBS->HandleProtocol (Private->Controller, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
        if (!EFI_ERROR (Status) && (BlkIo != NULL) && (BlkIo->Media != NULL)){
          BlockNumber = (UINT32)((BlkIo->Media->LastBlock + 1) * BlkIo->Media->BlockSize / SIZE_128MB);
          if ((BlockNumber == 0) && (BlkIo->Media->BlockSize != 0)) {
            BlockNumber = 1;
          }
          Time += BlockNumber * UFS_SANITIZE_TIME_CLEAR_128M_MAX;
          Private->EstimateTime[Index] = LShiftU64 (Time, 32);
          Private->EstimateTime[Index] += BlockNumber * UFS_SANITIZE_TIME_CLEAR_128M_MIN;
        }
        break;
      }
      case UfsBlockErase:
      {
        Status = gBS->HandleProtocol (Private->Controller, &gEfiEraseBlockProtocolGuid, (VOID **) &EraseBlockProtocol);
        if (!EFI_ERROR (Status)){
          Private->EstimateTime[Index] = UFS_SANITIZE_TIME_UNMAP;
        }
        break;
      }
      case UfsOverwrite:
      case UfsCryptoErase:
      default:
        break;
    }
  }
}

/**
  Get the time.

  @param[in]  This         Indicates a pointer to the calling context.
  @param[in]  EraseType    Erase type will be used to erase device block.

  @retval UINT64           time.
**/
UINT64
EFIAPI
GetUfsEstimateTime (
  IN  UFS_ERASE_BLOCK_PROTOCOL      *This,
  IN  UFS_ERASE_TYPE                EraseType
  )
{
  UFS_ERASE_PRIVATE_DATA    *Private;

  if ((This == NULL)) {
    return 0;
  }

  Private = PRIVATE_FROM_ERASEBLOCK(This);
  switch (EraseType) {
    case UfsClear:
    case UfsOverwrite:
    case UfsBlockErase:
    case UfsCryptoErase:
      if (EraseType <= UFS_SANITIZE_COUNT) {
        return Private->EstimateTime[EraseType-1];
      }
    default:
      return 0;
  }
}

/**
  Tests to see if this driver supports a given controller.

  This function checks to see if the controller contains an instance of the
  EFI_BLOCK_IO_PROTOCOL and it is UFS controller
  and returns EFI_SUCCESS if it does.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The Handle of the controller to test. This Handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  This parameter is ignored.

  @retval EFI_SUCCESS               The device contains required protocols
  @retval EFI_ALREADY_STARTED       The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by the driver
                                    specified by This.
  @retval EFI_ACCESS_DENIED         The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by a different
                                    driver or an application that requires exclusive access.
                                    Currently not implemented.
  @retval EFI_UNSUPPORTED           The device does not contain requires protocols

**/
EFI_STATUS
EFIAPI
UfsEraseBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL* This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*    RemainingDevicePath
  )
{
  EFI_STATUS             Status;
  EFI_BLOCK_IO_PROTOCOL  *BlockIo;

  //
  // Test EFI_BLOCK_IO_PROTOCOL on controller Handle.
  //
  Status = gBS->OpenProtocol(
                  Controller,
                  &gEfiBlockIoProtocolGuid,
                  ( VOID ** ) &BlockIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
     return Status;
  }

  //
  // Open UFS_ERASE_BLOCK_PROTOCOL to perform erase supported checks
  //
  Status = gBS->OpenProtocol(
                  Controller,
                  &gUfsEraseBlockProtocolGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  //
  // protocol is already installed
  //
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  if (IsUfsDevice (Controller)) {
    DEBUG((DEBUG_INFO, "%a - Block erase protocol installed on UFS device handle.\n", __FUNCTION__));
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_UNSUPPORTED;
  }
  return Status;
}

/**
  Start function to install protocols on a supported device.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The Handle of the controller to start. This Handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath   A pointer to the remaining portion of a device path.  This
                                    parameter is ignored by device drivers, and is optional for bus
                                    drivers. For a bus driver, if this parameter is NULL, then handles
                                    for all the children of Controller are created by this driver.
                                    If this parameter is not NULL and the first Device Path Node is
                                    not the End of Device Path Node, then only the Handle for the
                                    child device specified by the first Device Path Node of
                                    RemainingDevicePath is created by this driver.
                                    If the first Device Path Node of RemainingDevicePath is
                                    the End of Device Path Node, no child Handle is created by this
                                    driver.

  @retval EFI_SUCCESS               Opal management was enabled.
  @retval EFI_DEVICE_ERROR          The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES      The request could not be completed due to a lack of resources.
  @retval Others                    The driver failed to start the device.

**/
EFI_STATUS
EFIAPI
UfsEraseBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL* This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*    RemainingDevicePath
  )
{
  EFI_STATUS                      Status;
  UFS_ERASE_PRIVATE_DATA          *Private;
  EFI_DEVICE_PATH_PROTOCOL        *TempDevicePath;
  EFI_HANDLE                      Handle;
  CHAR16                          ModelName[100];

  DEBUG((DEBUG_INFO, "%a: - EfiDriverBindingStart\n", __FUNCTION__));
  Private = NULL;

  //
  // Open UFS_ERASE_BLOCK_PROTOCOL to perform erase supported checks
  //
  Status = gBS->OpenProtocol(
                  Controller,
                  &gUfsEraseBlockProtocolGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  //
  // protocol is already installed
  //
  if (!EFI_ERROR (Status)) {
    Status = EFI_ALREADY_STARTED;
    goto Exit;
  }

  Private = AllocateZeroPool (sizeof (UFS_ERASE_PRIVATE_DATA));
  if (Private == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Private->Signature                         = UFS_ERASE_SIGNATURE;
  Private->Controller                        = Controller;
  Private->EraseBlock.Revision               = 1;
  Private->EraseBlock.EraseLengthGranularity = 1;
  Private->EraseBlock.EraseBlocks            = UfsBlockEraseEx;
  Private->EraseBlock.GetEraseInfo           = GetUfsEraseInfo;
  Private->EraseBlock.GetEstimateTime        = GetUfsEstimateTime;
  Private->Info                              = NULL;

  Status = gBS->InstallProtocolInterface (
                  &Controller,
                  &gUfsEraseBlockProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &Private->EraseBlock
                  );
  Status = gBS->HandleProtocol (Controller, &gEfiDevicePathProtocolGuid, (VOID **) &TempDevicePath);
  if (!EFI_ERROR (Status)) {
    Private->DevicePath = TempDevicePath;
    Status = gBS->LocateDevicePath (
                    &gEfiExtScsiPassThruProtocolGuid,
                    &TempDevicePath,
                    &Handle
                    );
    if (!EFI_ERROR (Status)) {
      gBS->HandleProtocol (
             Handle,
             &gEfiExtScsiPassThruProtocolGuid,
             (VOID**)&Private->ScsiInterfaceExt
             );
    }

    TempDevicePath = Private->DevicePath;
    Status = gBS->LocateDevicePath (
                    &gEfiUfsDeviceConfigProtocolGuid,
                    &TempDevicePath,
                    &Handle
                    );
    if (!EFI_ERROR (Status)) {
      gBS->HandleProtocol (
             Handle,
             &gEfiUfsDeviceConfigProtocolGuid,
             (VOID **) &Private->UfsDevConfig
             );
    }

    SetUfsEstimateTime (Private);
    if (Private->UfsDevConfig != NULL) {
      //
      // Set device information
      //
      Private->Info = AllocateZeroPool (sizeof (UFS_ERASE_INFO));
      if (Private->Info != NULL) {
        UfsInitDeviceInfo (Private);
        //
        // Build controller name for Component Name (2) protocol.
        //
        SetMem(ModelName, sizeof (ModelName), 0x0);
        if ((Private->Info->Manufacturer[0] != 0x0)
            || (Private->Info->ModelName[0] != 0x0)
            || (Private->Info->SerialNumber[0] != 0x0)) {
          UnicodeSPrintAsciiFormat (ModelName, sizeof (ModelName), "%a %a %a",
            Private->Info->Manufacturer, Private->Info->ModelName, Private->Info->SerialNumber);
          AddUnicodeString2 (
            "eng",
            gUfsEraseComponentName.SupportedLanguages,
            &(Private->ControllerNameTable),
            ModelName,
            TRUE
            );

          AddUnicodeString2 (
            "en",
            gUfsEraseComponentName2.SupportedLanguages,
            &(Private->ControllerNameTable),
            ModelName,
            FALSE
            );
        }
      }
    }
  }
  return EFI_SUCCESS;

Exit:

  return Status;
}

/**
  Stop this driver on Controller.

  @param  This              Protocol instance pointer.
  @param  Controller        Handle of device to stop driver on
  @param  NumberOfChildren  Number of Handles in ChildHandleBuffer. If number of
                            children is zero stop the entire bus driver.
  @param  ChildHandleBuffer List of Child Handles to Stop.

  @retval EFI_SUCCESS       This driver is removed Controller.
  @retval other             This driver could not be removed from this device.

**/
EFI_STATUS
EFIAPI
UfsStorageEraseBindingStop (
  EFI_DRIVER_BINDING_PROTOCOL*    This,
  EFI_HANDLE                      Controller,
  UINTN                           NumberOfChildren,
  EFI_HANDLE*                     ChildHandleBuffer
  )
{
  EFI_STATUS                Status;
  UFS_ERASE_BLOCK_PROTOCOL  *EraseBlock;
  UFS_ERASE_PRIVATE_DATA    *Private;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gUfsEraseBlockProtocolGuid,
                  (VOID **) &EraseBlock,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Private = PRIVATE_FROM_ERASEBLOCK (EraseBlock);
    gBS->UninstallMultipleProtocolInterfaces (
           Controller,
           &gUfsEraseBlockProtocolGuid,
           EraseBlock,
           NULL
           );
    if ((Private != NULL) && (Private->Info != NULL)) {
      FreePool(Private->Info);
    }
    FreePool (Private);
  }

  return EFI_SUCCESS;
}

//
// Globals
//
EFI_DRIVER_BINDING_PROTOCOL gUfsEraseDriverBinding = {
  UfsEraseBindingSupported,
  UfsEraseBindingStart,
  UfsStorageEraseBindingStop,
  0x1,
  NULL,
  NULL
};

/**
  Main entry for this driver.

  @param ImageHandle     Image Handle this driver.
  @param SystemTable     Pointer to SystemTable.

  @retval EFI_SUCCESS    This function always complete successfully.
**/
EFI_STATUS
EFIAPI
UfsEraseEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE*  SystemTable
  )
{
  EFI_STATUS                     Status;

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gUfsEraseDriverBinding,
             ImageHandle,
             &gUfsEraseComponentName,
             &gUfsEraseComponentName2
             );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "%a - Install protocols to Handle failed\n", __FUNCTION__));
    return Status ;
  }

  return Status;
}
