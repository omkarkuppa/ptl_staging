/**@file
  Nvme Erase Driver.

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

#include "NvmeEraseDxe.h"

//
// NVME definition
//
#define NVME_CONTROLLER_ID           0
#define NVME_GENERIC_TIMEOUT         EFI_TIMER_PERIOD_SECONDS (5)
#define NVME_FORMAT_NVM_CMD_TIMEOUT  EFI_TIMER_PERIOD_SECONDS (3600)
#define CRYPTO_ERASE_SUPPORTED       BIT2
#define NVME_SANITIZE_TIME_FORMAT        10   // About 10 second to format NVMe media
#define NVME_SANITIZE_TIME_CLEAR_4G_MIN  20   // Minumum 20 second to clear 4G NVMe media
#define NVME_SANITIZE_TIME_CLEAR_4G_MAX  180  // Maximum 180 second to clear 4G NVMe media
typedef enum {
  NoEraseMode,
  UserDataEraseMode,
  CryptoEraseMode
} SECURE_ERASE_MODE;

/**
  Search protocol database and check to see if the protocol
  specified by ProtocolGuid is present on a ControllerHandle and opened by
  ChildHandle with an attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.
  If the ControllerHandle is found, then the protocol specified by ProtocolGuid
  will be opened on it.

  @param[in] ProtocolGuid   ProtocolGuid pointer.
  @param[in] ChildHandle    Child Handle to retrieve Parent information.

  @retval Interface  Pointer of protocol interface

**/
VOID *
EFIAPI
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
  Check if EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL exist on the parent handle.

  @param[in] ChildHandle  Child Handle to retrieve Parent information.

  @retval TRUE   EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL exist on the parent handle.
  @retval FALSE  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL doesn't exist on the parent handle.

**/
BOOLEAN
IsNvmeDevice (
  IN  EFI_HANDLE      ChildHandle
  )
{
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *PassThru;

  PassThru = (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *) GetParentProtocol (&gEfiNvmExpressPassThruProtocolGuid, ChildHandle);
  if (PassThru != NULL) {
      return TRUE;
  }

  return FALSE;
}

/**
  Perform clear operation on specific NVMe device.

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
NvmeDataClear (
  IN     NVME_ERASE_BLOCK_PRIVATE    *Private,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     Lba,
  IN OUT NVME_ERASE_BLOCK_TOKEN      *Token,
  IN     UINTN                       Size,
  IN     NVME_CLEAR_TYPE             Type
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

  Status = gBS->HandleProtocol (Private->Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: not support BlockIO: %r \n", __FUNCTION__, Status));
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
    if (Type == NvmeClearZero) {
      SetMem(Buffer, BufferSize, 0x0);
    } else if (Type == NvmeClearOne) {
      SetMem(Buffer, BufferSize, 0xFF);
    }
    Status = BlkIo->WriteBlocks (BlkIo, BlkIo->Media->MediaId, Lba, BufferSize, Buffer);
    Lba += Block;
    FreePool (Buffer);
    if (EFI_ERROR(Status)) {
      break;
    }
  }
  BlkIo->FlushBlocks (BlkIo);
Done:
  return Status;
}

/**
  Get identify controller data.

  @param[in] PassThruProtocol  The pointer to the instance of NVME_PASS_THRU_PROTOCOL.
  @param[in] NamespaceId       Namespace Id for an NVM Express namespace present on the NVM Express controller
  @param[out] Buffer           The buffer used to store the identify controller data.

  @retval EFI_SUCCESS            Successfully performed operation.
  @retval EFI_NOT_READY          The NVM Express Command Packet could not be sent because the controller is not ready.
  @retval EFI_DEVICE_ERROR       A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER  NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid.
  @retval EFI_UNSUPPORTED        The command described by the NVM Express Command Packet is not supported by the NVM Express
  @retval EFI_TIMEOUT            A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeIdentifyController (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *PassThruProtocol,
  IN  UINT32                              NameSpaceId,
  OUT VOID                                *Buffer
  )
{
  EFI_STATUS                                Status;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;

  DEBUG ((DEBUG_INFO, "Sending NVME identify command (namespaceID: %d).\n", NameSpaceId));

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  Command.Cdw0.Opcode          = NVME_ADMIN_IDENTIFY_CMD;
  Command.Nsid                 = NameSpaceId;
  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = Buffer;
  CommandPacket.TransferLength = sizeof (NVME_ADMIN_CONTROLLER_DATA);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  //
  // Set bit 0 (Cns bit) to 1 to identify a controller
  //
  Command.Cdw10                = 1;
  Command.Flags                = CDW10_VALID;

  Status = PassThruProtocol->PassThru (PassThruProtocol, NameSpaceId, &CommandPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Identify Controller Command Status=%r\n", __FUNCTION__, Status));
  }

  return Status;
}

/**
  Perform Nvme Format NVM command.

  @param[in] PassThruProtocol     The pointer to the instance of NVME_PASS_THRU_PROTOCOL.
  @param[in] NamespaceId          Namespace Id for an NVM Express namespace present on the NVM Express controller
  @param[in] SecureEraseSettings  Type of the Secure Erase operation

  @retval EFI_SUCCESS            Successfully performed secure erase operation.
  @retval EFI_NOT_READY          The NVM Express Command Packet could not be sent because the controller is not ready.
  @retval EFI_DEVICE_ERROR       A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER  NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid.
  @retval EFI_UNSUPPORTED        The command described by the NVM Express Command Packet is not supported by the NVM Express
  @retval EFI_TIMEOUT            A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeFormatNvmCommand (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *PassThruProtocol,
  IN UINT32                                NameSpaceId,
  IN UINT32                                SecureEraseSettings
  )
{
  EFI_STATUS                                Status;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  NVME_ADMIN_FORMAT_NVM                     FormatNvm;

  DEBUG ((DEBUG_INFO, "Sending NvmeFormatNvmCommand (NameSpaceId: %d).\n", NameSpaceId));

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));
  ZeroMem (&FormatNvm, sizeof (NVME_ADMIN_FORMAT_NVM));

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;

  Command.Nsid                 = NameSpaceId;
  Command.Cdw0.Opcode          = NVME_ADMIN_FORMAT_NVM_CMD;
  CommandPacket.CommandTimeout = NVME_FORMAT_NVM_CMD_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  FormatNvm.Ses                = SecureEraseSettings;
  CopyMem (&CommandPacket.NvmeCmd->Cdw10, &FormatNvm, sizeof (NVME_ADMIN_FORMAT_NVM));
  CommandPacket.NvmeCmd->Flags = CDW10_VALID;

  Status = PassThruProtocol->PassThru (PassThruProtocol, NameSpaceId, &CommandPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NvmeStorageErase: FormatNvmCommand Status=%r\n", Status));
  }

  return Status;
}

/**
  Perform override erase operation on specific NVMe device.

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
NvmeDataOverwrite (
  IN     NVME_ERASE_BLOCK_PRIVATE    *Private,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     LBA,
  IN OUT NVME_ERASE_BLOCK_TOKEN      *Token,
  IN     UINTN                       Size
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Perform block erase operation on specific NVMe device.

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
NvmeDataBlockErase (
  IN     NVME_ERASE_BLOCK_PRIVATE    *Private,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     Lba,
  IN OUT NVME_ERASE_BLOCK_TOKEN      *Token,
  IN     UINTN                       Size
  )
{
  EFI_STATUS                  Status;
  NVME_ADMIN_CONTROLLER_DATA  ControllerData;

  DEBUG((DEBUG_INFO, "%a: Start. \n", __FUNCTION__));

  ZeroMem (&ControllerData, sizeof (NVME_ADMIN_CONTROLLER_DATA));
  Status = NvmeIdentifyController (Private->Passthru, NVME_CONTROLLER_ID, (VOID *)&ControllerData);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a: Get Nvme Identify Controller Status %r \n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a: Oacs:0x%2x, Fna:0x%x\n", __FUNCTION__, ControllerData.Oacs, ControllerData.Fna));
  if ((ControllerData.Oacs & FORMAT_NVM_SUPPORTED)) {
    return NvmeFormatNvmCommand (Private->Passthru, Private->NameSpaceId, UserDataEraseMode);
  }
  return EFI_UNSUPPORTED;
}

/**
  Perform crypto erase operation on specific NVMe device.

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
NvmeDataCryptoErase (
  IN     NVME_ERASE_BLOCK_PRIVATE    *Private,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     LBA,
  IN OUT NVME_ERASE_BLOCK_TOKEN      *Token,
  IN     UINTN                       Size
  )
{
  EFI_STATUS                  Status;
  NVME_ADMIN_CONTROLLER_DATA  ControllerData;

  DEBUG((DEBUG_INFO, "%a: Start. \n", __FUNCTION__));

  ZeroMem (&ControllerData, sizeof (NVME_ADMIN_CONTROLLER_DATA));
  Status = NvmeIdentifyController (Private->Passthru, NVME_CONTROLLER_ID, (VOID *)&ControllerData);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a: Get Nvme Identify Controller Status %r \n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a: Oacs:0x%2x, Fna:0x%x\n", __FUNCTION__, ControllerData.Oacs, ControllerData.Fna));
  if ((ControllerData.Oacs & FORMAT_NVM_SUPPORTED) && (ControllerData.Fna & CRYPTO_ERASE_SUPPORTED)) {
    NvmeFormatNvmCommand (Private->Passthru, Private->NameSpaceId, UserDataEraseMode);
    return NvmeFormatNvmCommand (Private->Passthru , Private->NameSpaceId, CryptoEraseMode);
  }
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
  IN NVME_ERASE_BLOCK_PRIVATE   *Private,
  IN NVME_ERASE_TYPE            EraseType,
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
NvmeEraseBlocks (
  IN     NVME_ERASE_BLOCK_PROTOCOL     *This,
  IN     UINT32                         MediaId,
  IN     EFI_LBA                        Lba,
  IN OUT NVME_ERASE_BLOCK_TOKEN         *Token,
  IN     UINTN                          Size,
  IN     NVME_ERASE_TYPE                EraseType,
  IN     NVME_ERASE_BLOCK_PROGRESS      Progress OPTIONAL
  )
{
  EFI_STATUS                Status;
  NVME_ERASE_BLOCK_PRIVATE  *Private;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = PRIVATE_FROM_ERASEBLK (This);
  DEBUG((DEBUG_INFO, "%a: EraseType:%d, DevicePath: %s \n",
    __FUNCTION__, EraseType, ConvertDevicePathToText (Private->DevicePath, TRUE, TRUE)));

  switch (EraseType) {
    case NvmeClear:
      Status = NvmeDataClear (Private, MediaId, Lba, Token, Size, NvmeClearZero);
      break;
    case NvmeOverwrite:
      Status = NvmeDataOverwrite (Private, MediaId, Lba, Token, Size);
      break;
    case NvmeBlockErase:
      Status = NvmeDataBlockErase (Private, MediaId, Lba, Token, Size);
      break;
    case NvmeCryptoErase:
      Status = NvmeDataCryptoErase (Private, MediaId, Lba, Token, Size);
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
  Return the result of erase operation on the device.

  @param[in]  This          Indicates a pointer to the calling context.
  @param[out] EraseInfo    A pointer to the data of erasing device information.

  @retval EFI_SUCCESS             Get result of erase operation successfully.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetNvmeEraseInfo (
  IN  NVME_ERASE_BLOCK_PROTOCOL      *This,
  OUT VOID                           **EraseInfo
  )
{
  NVME_ERASE_BLOCK_PRIVATE    *Private;
  NVME_ERASE_INFO             *FinalInfo;

  if ((This == NULL) || (EraseInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Private = PRIVATE_FROM_ERASEBLK (This);

  DEBUG((DEBUG_INFO, "%a: DevicePath: %s \n",
    __FUNCTION__, ConvertDevicePathToText (Private->DevicePath, TRUE, TRUE)));

  if (Private->Info != NULL) {
    FinalInfo = AllocateZeroPool (sizeof (NVME_ERASE_INFO));
    if (FinalInfo == NULL) {
      DEBUG((DEBUG_ERROR, "%a: not be able to allocate mem for device information \n",  __FUNCTION__));
      return EFI_NOT_FOUND;
    }
    CopyMem (FinalInfo, Private->Info, sizeof (NVME_ERASE_INFO));
    *EraseInfo = FinalInfo;
    DEBUG((DEBUG_INFO, "%a: find the information \n",  __FUNCTION__));
    return EFI_SUCCESS;
  }

  DEBUG((DEBUG_INFO, "%a: no information can  be found. \n",  __FUNCTION__));
  return EFI_NOT_FOUND;
}

/**
  Set the estimate time.

  @param[in] Private          Pointer to the NVME_ERASE_BLOCK_PRIVATE.
  @param[in] ControllerData   NVMe Controller data.

**/
VOID
SetNvmeEstimateTime (
  IN NVME_ERASE_BLOCK_PRIVATE    *Private,
  IN NVME_ADMIN_CONTROLLER_DATA  *ControllerData
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

  for (Index = 0; Index < NVME_SANITIZE_COUNT; Index ++) {
    switch (Index + 1) {
      case NvmeClear:
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
          Time += BlockNumber * NVME_SANITIZE_TIME_CLEAR_4G_MAX;
          Private->EstimateTime[Index] = LShiftU64 (Time, 32);
          Private->EstimateTime[Index] += BlockNumber * NVME_SANITIZE_TIME_CLEAR_4G_MIN;
        }
        break;
      }
      case NvmeBlockErase:
      {
        if ((ControllerData != NULL) && (ControllerData->Oacs & FORMAT_NVM_SUPPORTED)) {
          Private->EstimateTime[Index] = NVME_SANITIZE_TIME_FORMAT;
        }
        break;
      }
      case NvmeCryptoErase:
      {
        if ((ControllerData != NULL) && (ControllerData->Oacs & FORMAT_NVM_SUPPORTED) && (ControllerData->Fna & CRYPTO_ERASE_SUPPORTED)) {
          Private->EstimateTime[Index] = NVME_SANITIZE_TIME_FORMAT;
        }
        break;
      }
      case NvmeOverwrite:
      default:
        break;
    }
  }
}

/**
  Get the time.

  @param[in]  This         Indicates a pointer to the calling context.
  @param[in]  EraseType    Erase type will be used to erase device block.

  @retval EFI_SUCCESS      Successful get time.
  @retval Other            Fail to get time.
**/
UINT64
EFIAPI
GetNvmeEstimateTime (
  IN  NVME_ERASE_BLOCK_PROTOCOL      *This,
  IN  NVME_ERASE_TYPE                EraseType
  )
{
  NVME_ERASE_BLOCK_PRIVATE    *Private;

  if ((This == NULL)) {
    return 0;
  }

  Private = PRIVATE_FROM_ERASEBLK (This);
  switch (EraseType) {
    case NvmeClear:
    case NvmeOverwrite:
    case NvmeBlockErase:
    case NvmeCryptoErase:
      if (EraseType <= NVME_SANITIZE_COUNT) {
        return Private->EstimateTime[EraseType - 1];
      }
    default:
      return 0;
  }
}

/**
  Tests to see if this driver supports a given controller.

  This function checks to see if the controller contains an instance of the
  EFI_BLOCK_IO_PROTOCOL and returns EFI_SUCCESS if it does.

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
NvmeEraseBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*     RemainingDevicePath
  )
{
  EFI_STATUS             Status;
  EFI_BLOCK_IO_PROTOCOL  *BlockIo;

  //
  // Test if EFI_BLOCK_IO_PROTOCOL on controller Handle.
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiBlockIoProtocolGuid,
                  ( VOID ** )&BlockIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Open NVME_ERASE_BLOCK_PROTOCOL to check if protocol already installed on the handle
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gNvmeEraseBlockProtocolGuid,
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

  //DEBUG((DEBUG_INFO, "%a - is logicalpartition : %x \n", __FUNCTION__, BlockIo->Media->LogicalPartition));

  if (!(BlockIo->Media->LogicalPartition) && IsNvmeDevice (Controller)) {
    DEBUG((DEBUG_INFO, "%a - Found the instance for NVMe device\n", __FUNCTION__));
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Start function to install protocols on a supported device.

  The start function is designed to be called after the driver has confirmed the "controller",
  which is a child Handle, contains the EFI_BLOCK_IO_PROTOCOL.

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

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some error

**/
EFI_STATUS
EFIAPI
NvmeEraseBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*     RemainingDevicePath
  )
{
  EFI_STATUS                  Status;
  NVME_ERASE_BLOCK_PRIVATE    *Private;
  EFI_DEVICE_PATH_PROTOCOL    *TempDevicePath;
  EFI_HANDLE                  Handle;
  NVME_ADMIN_CONTROLLER_DATA  ControllerData;

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
  // Install NVME_ERASE_BLOCK_PROTOCOL
  //
  Private = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));
  if (Private == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Private->Signature                         = NVME_ERASE_BLOCK_SIGNATURE;
  Private->EraseBlock.Revision               = NVME_ERASE_BLOCK_PROTOCOL_REVISION;
  Private->EraseBlock.EraseLengthGranularity = 1;
  Private->EraseBlock.EraseBlocks            = NvmeEraseBlocks;
  Private->EraseBlock.GetEraseInfo           = GetNvmeEraseInfo;
  Private->EraseBlock.GetEstimateTime        = GetNvmeEstimateTime;
  Private->Handle                            = Controller;
  Private->DevicePath                        = TempDevicePath;
  Private->Info                              = NULL;

  //
  // Get instance of EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL which is the closest to device path
  //
  Handle = Controller;
  TempDevicePath = Private->DevicePath;
  Status = gBS->LocateDevicePath (
                  &gEfiNvmExpressPassThruProtocolGuid,
                  &TempDevicePath,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed to locate EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL in device path. \n"));
    goto Exit;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  (VOID**)&Private->Passthru
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed to get EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL. \n"));
    goto Exit;
  }

  Status = Private->Passthru->GetNamespace (Private->Passthru, TempDevicePath, &Private->NameSpaceId);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed to get namespace ID : %r. \n", Status));
    goto Exit;
  }

  Status = gBS->InstallProtocolInterface (
                  &Controller,
                  &gNvmeEraseBlockProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &Private->EraseBlock
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install the Erase Block Protocol! Status = %r\n", Status));
    goto Exit;
  }

  //
  // Get device info from device FW
  //
  ZeroMem (&ControllerData, sizeof (NVME_ADMIN_CONTROLLER_DATA));
  Status = NvmeIdentifyController (Private->Passthru, NVME_CONTROLLER_ID, (VOID *)&ControllerData);
  if (!EFI_ERROR (Status)) {
    SetNvmeEstimateTime (Private, &ControllerData);
  } else {
    SetNvmeEstimateTime (Private, NULL);
  }
  Private->Info = AllocateZeroPool (sizeof (NVME_ERASE_INFO));
  if ((Private->Info != NULL) && !EFI_ERROR (Status)) {
    Private->Info->VidDid= (ControllerData.Vid << 16);
    CopyMem(Private->Info->ModelName, ControllerData.Mn, sizeof(ControllerData.Mn));
    CopyMem(Private->Info->SerialNumber, ControllerData.Sn, sizeof(ControllerData.Sn));
  }

  DEBUG((DEBUG_INFO, "%a - Install erase block protocol successfully, namespace ID: %d\n", __FUNCTION__, Private->NameSpaceId));
  return EFI_SUCCESS;

Exit:

  if (Private != NULL) {
    FreePool (Private);
  }

  return Status;
}

/**
  Stop this driver on Controller.

  @param  This               Protocol instance pointer.
  @param  Controller         Handle of device to stop driver on
  @param  NumberOfChildren   Number of Handles in ChildHandleBuffer. If number of
                             children is zero stop the entire bus driver.
  @param  ChildHandleBuffer  List of Child Handles to Stop.

  @retval EFI_SUCCESS  This driver is removed Controller.
  @retval other        This driver could not be removed from this device.

**/
EFI_STATUS
EFIAPI
NvmeEraseBindingStop (
  EFI_DRIVER_BINDING_PROTOCOL*  This,
  EFI_HANDLE                    Controller,
  UINTN                         NumberOfChildren,
  EFI_HANDLE*                   ChildHandleBuffer
  )
{
  EFI_STATUS  Status;
  NVME_ERASE_BLOCK_PROTOCOL   *EraseBlock;
  NVME_ERASE_BLOCK_PRIVATE  *Private;
  Private = NULL;
  Status = gBS->OpenProtocol (
                  Controller,
                  &gNvmeEraseBlockProtocolGuid,
                  (VOID **)&EraseBlock,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (!EFI_ERROR (Status)) {
    Private = PRIVATE_FROM_ERASEBLK (EraseBlock);
    gBS->UninstallMultipleProtocolInterfaces (
           Controller,
           &gNvmeEraseBlockProtocolGuid,
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
EFI_DRIVER_BINDING_PROTOCOL gNvmeEraseDriverBinding = {
  NvmeEraseBindingSupported,
  NvmeEraseBindingStart,
  NvmeEraseBindingStop,
  0x10,
  NULL,
  NULL
};

/**
  Main entry for this driver.

  @param ImageHandle  Image Handle this driver.
  @param SystemTable  Pointer to SystemTable.

  @retval EFI_SUCCESS  This function always complete successfully.
**/
EFI_STATUS
EFIAPI
NvmeEraseEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE*  SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG((DEBUG_INFO, "%a - enter\n", __FUNCTION__));

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gNvmeEraseDriverBinding,
             ImageHandle,
             &gNvmeEraseComponentName,
             &gNvmeEraseComponentName2
             );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "%a - Install protocols to Handle failed\n", __FUNCTION__));
  }

  return Status;
}
