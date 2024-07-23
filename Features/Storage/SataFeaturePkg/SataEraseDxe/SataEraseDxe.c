/**@file
  Entrypoint of Sata Erase Driver.

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

#include "SataEraseDxe.h"

CHAR8 mDefaultPassStr[ATA_PASSWORD_MAX_LENGTH] = "PLATFORMERASE";

//
// SATA definition
//
#define ATA_TIMEOUT                      EFI_TIMER_PERIOD_SECONDS (5)  // Time out value for ATA pass through protocol
#define SATA_SANITIZE_TIME_CLEAR_4G_MIN  15                            // Minimum 15 second to clear 4G SATA media
#define SATA_SANITIZE_TIME_CLEAR_4G_MAX  60                            // Minimum 60 second to clear 4G SATA media

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
  EFI_STATUS  Status;
  UINTN       Index;
  UINTN       HandleCount;
  VOID        *Interface;
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
  Check if EFI_ATA_PASS_THRU_PROTOCOL exist on the parent handle.

  @param  ChildHandle  Child Handle to retrieve Parent information.

  @retval  TRUE    Should produce Block Io & Block Io2.
  @retval  FALSE   Should not produce Block Io & Block Io2.

**/
BOOLEAN
IsSataDevice (
  IN  EFI_HANDLE  ChildHandle
  )
{
  EFI_ATA_PASS_THRU_PROTOCOL  *PassThru;

  PassThru = (EFI_ATA_PASS_THRU_PROTOCOL *) GetParentProtocol (&gEfiAtaPassThruProtocolGuid, ChildHandle);
  if (PassThru != NULL) {
      return TRUE;
  }

  return FALSE;
}

/**
  Get device port number from device path

  @param[in] DevicePath       Device path of device
  @param[out] Port            Port number
  @param[out] PortMultiplier  Port multiplier number

  @retval EFI_STATUS          Successful get the Port
  @retval Others              Fail to get the Port

**/
EFI_STATUS
GetPortNum (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINT16                   *Port,
  OUT UINT16                   *PortMultiplier
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  SATA_DEVICE_PATH          *Node;

  if (DevicePath == NULL || Port == NULL || PortMultiplier == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DevicePathNode = DevicePath;
  while (!IsDevicePathEndType (DevicePathNode)) {
    if (DevicePathType (DevicePathNode) == MESSAGING_DEVICE_PATH &&
        DevicePathSubType (DevicePathNode) == MSG_SATA_DP) {
      Node = (SATA_DEVICE_PATH *)DevicePathNode;
      *Port = Node->HBAPortNumber;
      *PortMultiplier = Node->PortMultiplierPortNumber;
      return EFI_SUCCESS;
    }
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }
  return EFI_NOT_FOUND;
}

/**
  Returns estimated secure erase time

  @param[in]       ControllerData      The buffer used to store the identify controller data.

  @retval          Secure erase time in 100 ns units
**/
UINT64
SataGetEraseTimeout (
  IN ATA_IDENTIFY_DATA  *ControllerData
  )
{
  UINT64  Time;
  UINT32  temp;

  DEBUG ((DEBUG_INFO, "%a: time_for_security_erase_unit:%04x, time_for_enhanced_security_erase_unit : %04x\n",
    __FUNCTION__, ControllerData->time_for_security_erase_unit, ControllerData->time_for_enhanced_security_erase_unit));

  // Use temporary variable to prevent numeric overflow due to improper casting
  temp = (UINT32)(ControllerData->time_for_security_erase_unit) * 2 * 60;
  Time = EFI_TIMER_PERIOD_SECONDS (temp);
  return Time;
}

/**
  Perform clear operation on specific SATA device.

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
  @retval others       Operation is failed due to some erro

**/
EFI_STATUS
SataDataClear (
  IN     SATA_ERASE_BLOCK_PRIVATE    *Private,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     Lba,
  IN OUT SATA_ERASE_BLOCK_TOKEN      *Token,
  IN     UINTN                       Size,
  IN     SATA_CLEAR_TYPE             Type
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

  Status = gBS->HandleProtocol (Private->Handle, &gEfiBlockIoProtocolGuid, (VOID **) &BlkIo);
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
    if (Type == SataClearZero) {
      SetMem(Buffer, BufferSize, 0x0);
    } else if (Type == SataClearOne) {
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
  Perform override erase operation on specific SATA device.

  @param[in] Private    Pointer to the device private data.
  @param[in] MediaId    The media ID that the write request is for.
  @param[in] Lba        The starting logical block address to be written.
                        The caller is responsible for writing to only
                        legitimate locations.
  @param[in,out] Token  A pointer to the token associated with the
                        transaction.
  @param[in] Size       Size data will be cleared.

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some erro

**/
EFI_STATUS
SataDataOverwrite (
  IN     SATA_ERASE_BLOCK_PRIVATE  *Private,
  IN     UINT32                    MediaId,
  IN     EFI_LBA                   LBA,
  IN OUT SATA_ERASE_BLOCK_TOKEN    *Token,
  IN     UINTN                     Size
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Perform block erase operation on specific SATA device.

  @param[in] Private    Pointer to the device private data.
  @param[in] MediaId    The media ID that the write request is for.
  @param[in] Lba        The starting logical block address to be written.
                        The caller is responsible for writing to only
                        legitimate locations.
  @param[in,out] Token  A pointer to the token associated with the
                        transaction.
  @param[in] Size       Size data will be cleared.

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some erro

**/
EFI_STATUS
SataDataBlockErase (
  IN     SATA_ERASE_BLOCK_PRIVATE    *Private,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     Lba,
  IN OUT SATA_ERASE_BLOCK_TOKEN      *Token,
  IN     UINTN                       Size
  )
{
  EFI_STATUS         Status;
  ATA_IDENTIFY_DATA  IdentifyData;
  UINT64             Timeout;

  DEBUG((DEBUG_INFO, "%a: Start. \n", __FUNCTION__));

  ZeroMem (&IdentifyData, sizeof (ATA_IDENTIFY_DATA));
  Status = GetAtaIdentifyData (Private->Passthru, Private->Port, Private->PortMultiplier, &IdentifyData);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a: Get ATA Identify Data Status %r \n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "IdentifyData.multi_sector_setting 0x%04x\n", IdentifyData.multi_sector_setting));
  DEBUG ((DEBUG_INFO, "IdentifyData.security_status 0x%04x\n", IdentifyData.security_status));
  if ((IdentifyData.security_status & ATA_SECURITY_STATUS_FROZEN_BIT) != 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Security erase command
  //
  if ((IdentifyData.security_status & ATA_SECURITY_STATUS_ENABLED_BIT) == 0 &&
    (IdentifyData.security_status & ATA_SECURITY_STATUS_FEATURE_SET_BIT) != 0) {
    Status = SetAtaPassword (
               Private->Passthru,
               Private->Port,
               Private->PortMultiplier,
               ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD,
               ATA_SECURITY_ERASE_LEVEL_MAXIMUM,
               0,
               mDefaultPassStr
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Timeout = SataGetEraseTimeout (&IdentifyData);
    Status = SendAtaSecureErase (
               Private->Passthru,
               Private->Port,
               Private->PortMultiplier,
               ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD,
               ATA_SECURITY_ERASE_MODE_NORMAL,
               Timeout,
               mDefaultPassStr
               );
    Status = DisableAtaPassword (
               Private->Passthru,
               Private->Port,
               Private->PortMultiplier,
               ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD,
               mDefaultPassStr
               );

  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Perform crypto erase operation on specific SATA device.

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
SataDataCryptoErase (
  IN     SATA_ERASE_BLOCK_PRIVATE    *Private,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     LBA,
  IN OUT SATA_ERASE_BLOCK_TOKEN      *Token,
  IN     UINTN                       Size
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Store the result after performing erase operation.

  @param[in] Private      Pointer to the device private data.
  @param[in] EraseType    Erase type to be used to erase data on device.
  @param[in] EraseStatus  Result of erase operation.

**/
VOID
UpdateResult (
  IN SATA_ERASE_BLOCK_PRIVATE   *Private,
  IN SATA_ERASE_TYPE            EraseType,
  IN EFI_STATUS                 EraseStatus
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

  @param[in]       This           Indicates a pointer to the calling context.
  @param[in]       MediaId        The media ID that the erase request is for.
  @param[in]       Lba            The starting logical block address to be
                                  erased. The caller is responsible for erasing
                                  only legitimate locations.
  @param[in, out]  Token          A pointer to the token associated with the
                                  transaction.
  @param[in]       Size           The size in bytes to be erased. This must be
                                  a multiple of the physical block size of the
                                  device.
  @param[in]       EraseType      Erase type to be used to erase data on device.
  @param[in]       Progress       Callback function to inform caller the progress of erase process.

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
SataEraseBlocks (
  IN     SATA_ERASE_BLOCK_PROTOCOL    *This,
  IN     UINT32                       MediaId,
  IN     EFI_LBA                      Lba,
  IN OUT SATA_ERASE_BLOCK_TOKEN       *Token,
  IN     UINTN                        Size,
  IN     SATA_ERASE_TYPE              EraseType,
  IN     SATA_ERASE_BLOCK_PROGRESS    Progress OPTIONAL
  )
{
  EFI_STATUS                Status;
  SATA_ERASE_BLOCK_PRIVATE  *Private;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "%a: EraseType:%d . \n", __FUNCTION__, EraseType));
  Private = PRIVATE_FROM_ERASEBLK (This);

  DEBUG ((DEBUG_INFO, "%a: DevicePath: %s\n",
      __FUNCTION__, ConvertDevicePathToText (Private->DevicePath, TRUE, TRUE)));
  switch (EraseType) {
    case SataClear:
      Status = SataDataClear (Private, MediaId, Lba, Token, Size, SataClearZero);
      break;
    case SataOverwrite:
      Status = SataDataOverwrite (Private, MediaId, Lba, Token, Size);
      break;
    case SataBlockErase:
      Status = SataDataBlockErase (Private, MediaId, Lba, Token, Size);
      break;
    case SataCryptoErase:
      Status = SataDataCryptoErase (Private, MediaId, Lba, Token, Size);
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
  Swap each byte in the string.

  @param[in,out] AtaString  The string to be swapped.
  @param[in] Size           Size of the string.

**/
VOID
SataSwapAtaString (
  IN OUT CHAR8  *AtaString,
  IN UINTN      Size
  )
{
  UINTN   Index;
  CHAR8   TempChar;

  if ((AtaString == NULL) || (Size < 2)) {
    return;
  }

  //
  // Swap the byte order.
  //
  for (Index = 0; Index < Size; Index += 2) {
    TempChar = AtaString[Index + 1];
    AtaString[Index + 1] = AtaString[Index];
    AtaString[Index]     = TempChar;
  }
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
GetSataEraseInfo (
  IN  SATA_ERASE_BLOCK_PROTOCOL  *This,
  OUT VOID                       **EraseInfo
  )
{
  SATA_ERASE_BLOCK_PRIVATE     *Private;
  SATA_ERASE_INFO              *FinalInfo;

  if ((This == NULL) || (EraseInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "%a: Start. \n", __FUNCTION__));
  Private = PRIVATE_FROM_ERASEBLK(This);

  if (Private->Info != NULL) {
    FinalInfo = AllocateZeroPool (sizeof(SATA_ERASE_INFO));
    if (FinalInfo == NULL) {
      return EFI_NOT_FOUND;
    }
    CopyMem (FinalInfo, Private->Info, sizeof (SATA_ERASE_INFO));
    *EraseInfo = FinalInfo;
    return EFI_SUCCESS;
  }
  DEBUG((DEBUG_INFO, "%a: no information can  be found. \n",  __FUNCTION__));
  return EFI_NOT_FOUND;
}

/**
  Set the estimate time.

  @param[in] Private          Pointer to the SATA_ERASE_BLOCK_PRIVATE.
  @param[in] IdentifyData     SATA identify data.

**/
VOID
SetSataEstimateTime (
  IN SATA_ERASE_BLOCK_PRIVATE    *Private,
  IN ATA_IDENTIFY_DATA           *IdentifyData
  )
{
  UINT8                  Index;
  EFI_STATUS             Status;
  EFI_BLOCK_IO_PROTOCOL  *BlkIo;
  UINT64                 Time;
  UINT32                 BlockNumber;

  BlkIo = NULL;
  Time  = 0;
  if (Private == NULL) {
    return;
  }

  for (Index = 0; Index < SATA_SANITIZE_COUNT; Index ++) {
    switch (Index + 1) {
      case SataClear:
      {
        Status = gBS->HandleProtocol (Private->Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
        if (!EFI_ERROR (Status) && (BlkIo != NULL) && (BlkIo->Media != NULL)){
          BlockNumber = (UINT32)((BlkIo->Media->LastBlock + 1) * BlkIo->Media->BlockSize / SIZE_4GB);
          if ((BlockNumber == 0) && (BlkIo->Media->BlockSize != 0)) {
            //
            // Set to 1 when media size is smaller than 4GB
            //
            BlockNumber = 1;
          }
          Time += BlockNumber * SATA_SANITIZE_TIME_CLEAR_4G_MAX;
          Private->EstimateTime[Index] = LShiftU64 (Time, 32);
          Private->EstimateTime[Index] += BlockNumber * SATA_SANITIZE_TIME_CLEAR_4G_MIN;
        }
        break;
      }
      case SataBlockErase:
      {
        if (IdentifyData != NULL) {
          Private->EstimateTime[Index] = (UINT64)(IdentifyData->time_for_security_erase_unit) * 2 * 60;
        }
        break;
      }
      case SataOverwrite:
      case SataCryptoErase:
      default:
        break;
    }
  }
}

/**
  Get the time.

  @param[in]  This         Indicates a pointer to the calling context.
  @param[in]  EraseType    Erase type will be used to erase device block.

  @retval UINT64           Return time
**/
UINT64
EFIAPI
GetSataEstimateTime (
  IN  SATA_ERASE_BLOCK_PROTOCOL      *This,
  IN  SATA_ERASE_TYPE                EraseType
  )
{
  SATA_ERASE_BLOCK_PRIVATE    *Private;

  if ((This == NULL)) {
    return 0;
  }

  Private = PRIVATE_FROM_ERASEBLK (This);
  switch (EraseType) {
    case SataClear:
    case SataOverwrite:
    case SataBlockErase:
    case SataCryptoErase:
      if (EraseType <= SATA_SANITIZE_COUNT) {
        return Private->EstimateTime[EraseType - 1];
      }
    default:
      return 0;
  }
}

/**
  Tests to see if this driver supports a given controller.

  This function checks to see if the controller contains an instance of the
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL and the EFI_BLOCK_IO_PROTOCOL
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
SataEraseBlockBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL* This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*    RemainingDevicePath
  )
{
  EFI_STATUS                      Status;
  EFI_BLOCK_IO_PROTOCOL           *BlockIo;

  //
  // Test if EFI_BLOCK_IO_PROTOCOL on controller Handle.
  //
  Status = gBS->OpenProtocol(
                  Controller,
                  &gEfiBlockIoProtocolGuid,
                  ( VOID ** ) &BlockIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Open SATA_ERASE_BLOCK_PROTOCOL to perform erase supported checks
  //
  Status = gBS->OpenProtocol(
                  Controller,
                  &gSataEraseBlockProtocolGuid,
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

  if (!(BlockIo->Media->LogicalPartition) && IsSataDevice (Controller)) {
    DEBUG((DEBUG_INFO, "%a - Found the instance for SATA device\n", __FUNCTION__));
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Start function to install protocols on a supported device.

  The start function is designed to be called after the Opal UEFI Driver has confirmed the
  "controller", which is a child Handle, contains the EF_STORAGE_SECURITY_COMMAND protocols.
  This function will complete the other necessary checks, such as verifying the device supports
  the correct version of Opal.  Upon verification, it will add the device to the
  Opal HII list in order to expose Opal management options.

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
SataEraseBlockBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL*   This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*      RemainingDevicePath
  )
{
  EFI_STATUS                  Status;
  SATA_ERASE_BLOCK_PRIVATE    *Private;
  EFI_HANDLE                  Handle;
  EFI_DEVICE_PATH_PROTOCOL    *TempDevicePath;
  UINT16                      Port;
  UINT16                      PortMultiplier;
  ATA_IDENTIFY_DATA           IdentifyData;
  EFI_BLOCK_IO_PROTOCOL       *BlockIo;

  Private = NULL;
  //
  // Get EFI_DEVICE_PATH_PROTOCOL on the controller
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&TempDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Device path protocol is not installed on device handle. \n"));
    goto Exit;
  }

  //
  // Get port number, abort if it is unable to get valid port number
  //
  Status = GetPortNum (TempDevicePath, &Port, &PortMultiplier);
  if (EFI_ERROR(Status)) {
    goto Exit;
  }

  //
  // Install SATA_ERASE_BLOCK_PROTOCOL
  //
  Private = AllocateZeroPool (sizeof (SATA_ERASE_BLOCK_PRIVATE));
  if (Private == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Private->Signature                         = SATA_ERASE_BLOCK_SIGNATURE;
  Private->EraseBlock.Revision               = SATA_ERASE_BLOCK_PROTOCOL_REVISION;
  Private->EraseBlock.EraseLengthGranularity = 1;
  Private->EraseBlock.EraseBlocks            = SataEraseBlocks;
  Private->EraseBlock.GetEraseInfo           = GetSataEraseInfo;
  Private->EraseBlock.GetEstimateTime        = GetSataEstimateTime;
  Private->DevicePath                        = TempDevicePath;
  Private->Port                              = Port;
  Private->PortMultiplier                    = PortMultiplier;
  Private->Handle                            = Controller;
  Private->Info                              = NULL;

  //
  // Get instance of EFI_ATA_PASS_THRU_PROTOCOL which is the closest to device path
  //
  Handle = Controller;
  TempDevicePath = Private->DevicePath;
  Status = gBS->LocateDevicePath (
                  &gEfiAtaPassThruProtocolGuid,
                  &TempDevicePath,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed to locate EFI_ATA_PASS_THRU_PROTOCOL in device path. \n"));
    goto Exit;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiAtaPassThruProtocolGuid,
                  (VOID**)&Private->Passthru
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed to get EFI_ATA_PASS_THRU_PROTOCOL. \n"));
    goto Exit;
  }

  Status = gBS->InstallProtocolInterface (
                  &Controller,
                  &gSataEraseBlockProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &Private->EraseBlock
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install the Erase Block Protocol! Status = %r\n", Status));
    goto Exit;
  }

  ZeroMem (&IdentifyData, sizeof (ATA_IDENTIFY_DATA));
  Status = GetAtaIdentifyData (Private->Passthru, Private->Port, Private->PortMultiplier, &IdentifyData);
  if (!EFI_ERROR (Status)) {
    SetSataEstimateTime (Private, &IdentifyData);
  } else {
    SetSataEstimateTime (Private, NULL);
  }

  Private->Info = AllocateZeroPool (sizeof (SATA_ERASE_INFO));
  if ((Private->Info != NULL) && !EFI_ERROR (Status)) {
    SataSwapAtaString (IdentifyData.ModelName, sizeof(IdentifyData.ModelName));
    SataSwapAtaString (IdentifyData.SerialNo, sizeof(IdentifyData.SerialNo));
    CopyMem(Private->Info->ModelName, IdentifyData.ModelName, sizeof(IdentifyData.ModelName));
    CopyMem(Private->Info->SerialNumber, IdentifyData.SerialNo, sizeof(IdentifyData.SerialNo));

    if (((IdentifyData.command_set_supported_82 & BIT1) != 0)
        && ((IdentifyData.security_status & BIT1) != 0)
        && ((IdentifyData.security_status & BIT2) != 0)) {
      DEBUG((DEBUG_INFO, "%a - Device is locked, read/write command is deny. Prevent using BlockIO to read/write\n", __FUNCTION__));
      gBS->OpenProtocol(
            Controller,
            &gEfiBlockIoProtocolGuid,
            ( VOID ** ) &BlockIo,
            This->DriverBindingHandle,
            Controller,
            EFI_OPEN_PROTOCOL_BY_DRIVER
            );
    }
  }

  DEBUG((DEBUG_INFO, "%a - Install Platform erase protocol successfully, port: %d, PortMultiplier: %d\n",
     __FUNCTION__, Private->Port, Private->PortMultiplier));
  return EFI_SUCCESS;

Exit:

  if (Private != NULL) {
    FreePool (Private);
  }

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
SataEraseBlockBindingStop (
  EFI_DRIVER_BINDING_PROTOCOL*    This,
  EFI_HANDLE                      Controller,
  UINTN                           NumberOfChildren,
  EFI_HANDLE*                     ChildHandleBuffer
  )
{
  EFI_STATUS  Status;
  SATA_ERASE_BLOCK_PROTOCOL  *EraseBlock;
  SATA_ERASE_BLOCK_PRIVATE   *Private;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gSataEraseBlockProtocolGuid,
                  (VOID **) &EraseBlock,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (!EFI_ERROR (Status)) {
    Private = PRIVATE_FROM_ERASEBLK (EraseBlock);
    gBS->UninstallMultipleProtocolInterfaces (
           Controller,
           &gSataEraseBlockProtocolGuid,
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
EFI_DRIVER_BINDING_PROTOCOL gSataEraseBlockDriverBinding = {
  SataEraseBlockBindingSupported,
  SataEraseBlockBindingStart,
  SataEraseBlockBindingStop,
  0x10,
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
SataEraseEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE*  SystemTable
  )
{
  EFI_STATUS                     Status;

  DEBUG((DEBUG_INFO, "%a - enter\n", __FUNCTION__));

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gSataEraseBlockDriverBinding,
             ImageHandle,
             &gSataEraseBlockComponentName,
             &gSataEraseBlockComponentName2
             );

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "%a - Install protocols to Handle failed\n", __FUNCTION__));
  }

  return Status;
}
