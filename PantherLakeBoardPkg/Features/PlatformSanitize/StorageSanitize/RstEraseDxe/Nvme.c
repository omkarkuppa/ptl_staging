/**@file
  RST Storage Erase Driver for NVME.

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

#include "RstEraseDxe.h"

//
// NVME definition
//
#define NVME_CONTROLLER_ID           0
#define NVME_GENERIC_TIMEOUT         EFI_TIMER_PERIOD_SECONDS (5)
#define NVME_FORMAT_NVM_CMD_TIMEOUT  EFI_TIMER_PERIOD_SECONDS (3600)
#define CRYPTO_ERASE_SUPPORTED       BIT2

#define NVME_ALL_NAMESPACES             0xFFFFFFFF

#define NVME_SANITIZE_TIME_FORMAT        10   // About 10 second to format NVMe media
#define NVME_SANITIZE_TIME_CLEAR_4G_MIN  20   // Minumum 20 second to clear 4G NVMe media
#define NVME_SANITIZE_TIME_CLEAR_4G_MAX  180  // Maximum 180 second to clear 4G NVMe media

typedef enum {
  NoEraseMode,
  UserDataEraseMode,
  CryptoEraseMode
} SECURE_ERASE_MODE;

/**
  Get Nvme device

  @param[in] Private     Pointer to the device private data.
  @param[in] NameSpaceId Namespace id.

  @retval RST_NVME   Pointer to the Nvme device
  @retval NULL       No Nvme devie

**/
RST_NVME*
GetNvmeDevice (
  IN RST_ERASE_NVME_PRIVATE  *Private,
  IN UINT32                  NameSpaceId
  )
{
  LIST_ENTRY              *Node;
  RST_NVME                *Dev;

  if (Private == NULL) {
    return NULL;
  }
  //
  // Locate the device structure
  //
  for (Node = GetFirstNode (&Private->DeviceList);
       !IsNull (&Private->DeviceList, Node);
       Node = GetNextNode (&Private->DeviceList, Node)) {
    Dev = RST_NVME_FROM_LINK (Node);
    if (Dev->NameSpaceId == NameSpaceId) {
      return Dev;
    }
  }
  return NULL;
}

/**
  Update Nvme device list with namespace id, device path, Handle and so on.

  @param[in] Private    Pointer to the device private data.

  @retval EFI_SUCCESS   Successful

**/
EFI_STATUS
UpdateNvmeDiskInfo (
  IN RST_ERASE_NVME_PRIVATE  *Private
  )
{
  EFI_STATUS                Status;
  UINTN                     NumberOfDiskInfoHandles;
  EFI_HANDLE                *DiskInfoHandleList;
  UINTN                     Index;
  EFI_DEVICE_PATH_PROTOCOL  *TmpDevicePath;
  UINT32                    RstPortBitMap, NameSpaceId;
  RST_NVME                  *Dev;

  DiskInfoHandleList = NULL;
  DEBUG ((DEBUG_INFO, "%a - enter \n", __FUNCTION__));

  //
  // Locate all the handle installed diskinfo and is ATA device under VMD controlled (thru device path)
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDiskInfoProtocolGuid,
                  NULL,
                  &NumberOfDiskInfoHandles,
                  &DiskInfoHandleList
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  for (Index = 0; Index < NumberOfDiskInfoHandles; Index++) {
    Status = gBS->HandleProtocol (
                    DiskInfoHandleList[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **)&TmpDevicePath
                    );
    if (!EFI_ERROR(Status) &&
        (TmpDevicePath != NULL) &&
        IsVmdControlled (TmpDevicePath)) {
      //
      // Get real name space ID
      //
      Status= GetNamespaceId (TmpDevicePath, &RstPortBitMap);
      if (EFI_ERROR (Status)) {
        continue;
      } else {
        //
        // check if it is RAID, if RAID no block IO for each device, skip
        //
        if ((RstPortBitMap & (RstPortBitMap - 1)) != 0) {
          continue;
        }
      }
      for (NameSpaceId = 0; NameSpaceId < 31; NameSpaceId ++) {
        if (RstPortBitMap & ((UINT32)BIT0 << NameSpaceId)) {
          NameSpaceId = NameSpaceId + 1;
          Dev = GetNvmeDevice (Private, NameSpaceId);
          if (Dev != NULL) {
            Dev->DevicePath = TmpDevicePath;
            Dev->Handle = DiskInfoHandleList[Index];
            DEBUG((DEBUG_INFO, "%a - Update Device path (%s) on (NameSpaceId:%d)\n",
                   __FUNCTION__, ConvertDevicePathToText (Dev->DevicePath, TRUE, TRUE), Dev->NameSpaceId));
          }
          break;
        }
      }
    }
  }
  FreePool (DiskInfoHandleList);
  return EFI_SUCCESS;
}

/**
  Get identify controller data.

  @param[in] NvmeDevice   The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in] NamespaceId  NamespaceId for an NVM Express namespace present on the NVM Express controller
  @param[out] Buffer      The buffer used to store the identify controller data.

  @retval EFI_SUCCESS       Successfully got the identify controller data.
  @retval EFI_DEVICE_ERROR  Failed to get the identify controller data.

**/
EFI_STATUS
NvmeIdentifyController (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmeDevice,
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
  Command.Nsid                 = 1;
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

  Status = NvmeDevice->PassThru (NvmeDevice, NameSpaceId, &CommandPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Identify Controller Command Status=%r\n", __FUNCTION__, Status));
  }

  return Status;
}

/**
  Perform Nvme Format NVM command.

  @param[in] NvmeDevice           The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in] NamespaceId          NamespaceId for an NVM Express namespace present on the NVM Express controller
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
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
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

  Command.Nsid                 = 1;
  Command.Cdw0.Opcode          = NVME_ADMIN_FORMAT_NVM_CMD;
  CommandPacket.CommandTimeout = NVME_FORMAT_NVM_CMD_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  FormatNvm.Ses                = SecureEraseSettings;
  CopyMem (&CommandPacket.NvmeCmd->Cdw10, &FormatNvm, sizeof (NVME_ADMIN_FORMAT_NVM));
  CommandPacket.NvmeCmd->Flags = CDW10_VALID;

  Status = NvmeDevice->PassThru (NvmeDevice, NameSpaceId, &CommandPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "NvmeStorageErase: FormatNvmCommand Status=%r\n", Status));
  }

  return Status;
}

/**
  Enumerate Nvme device

  @param[in] Private   Pointer to the device private data.
  @param[in] Handle    Handle of the device's parent.

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some error

**/
EFI_STATUS
EnumerateNvmeDevices (
  IN RST_ERASE_NVME_PRIVATE  *Private,
  IN EFI_HANDLE              Handle
  )
{
  EFI_STATUS                  Status;
  UINT32                      NamespaceId;
  RST_NVME                    *Dev;

  InitializeListHead (&Private->DeviceList);

  Dev = NULL;
  NamespaceId = NVME_ALL_NAMESPACES;
  while (TRUE) {
    Status = Private->NvmePassthru->GetNextNamespace (Private->NvmePassthru, &NamespaceId);
    if (EFI_ERROR (Status)) {
      if (Status == EFI_NOT_FOUND) {
        break;
      } else {
        return Status;
      }
    }
    DEBUG ((DEBUG_INFO, "GetNextNamespace: NamespaceId = 0x%x\n", NamespaceId));

    //
    // Get NVME identify data
    //
    Dev = AllocateZeroPool (sizeof (RST_NVME));
    if (Dev != NULL) {
      Dev->Signature = RST_NVME_SIGNATURE;
      Dev->NameSpaceId = NamespaceId;
      Dev->Result = EFI_UNSUPPORTED;
      Dev->ParentHandle = Handle;
      InsertTailList (&Private->DeviceList, &Dev->Link);
      DEBUG ((DEBUG_INFO, "%a - Add RST NVMe device to the list, NamespaceId = 0x%x\n", __FUNCTION__, NamespaceId));
    }
  }

  return EFI_SUCCESS;
}

/**
  Perform clear operation on specific NVMe device.

  @param[in] Private   Pointer to the device private data.
  @param[in] Device    Pointer to the device.

  @retval EFI_SUCCESS  Operation is successful
  @retval Others       Operation failed due to an error

**/
EFI_STATUS
NvmeDataClear (
  IN RST_ERASE_NVME_PRIVATE  *Private,
  IN RST_NVME                *Device
  )
{
  EFI_STATUS             Status;
  EFI_BLOCK_IO_PROTOCOL  *BlkIo;
  EFI_BLOCK_IO_MEDIA     *Media;
  UINTN                  RemainingSize;
  UINTN                  BufferSize;
  VOID                   *Buffer;
  UINTN                  Size;
  EFI_LBA                Block;
  EFI_LBA                Lba;

  DEBUG((DEBUG_INFO, "%a: Start. \n", __FUNCTION__));

  Status = gBS->HandleProtocol (Device->Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
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

  if (Media->ReadOnly) {
    Status = EFI_WRITE_PROTECTED;
    goto Done;
  }

  Size = (BlkIo->Media->LastBlock + 1) * (BlkIo->Media->BlockSize);

  RemainingSize = Size;
  Status = EFI_SUCCESS;
  Block  = 0;
  Lba    = 0;
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

    ZeroMem (Buffer, BufferSize);
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
  Perform block erase operation on specific NVMe device.

  @param[in] Private   Pointer to the device private data.
  @param[in] Device    Pointer to the device.

  @retval EFI_SUCCESS  Operation is successful
  @retval Others       Operation failed due to an error

**/
EFI_STATUS
NvmeDataBlockErase (
  IN RST_ERASE_NVME_PRIVATE  *Private,
  IN RST_NVME                *Device
  )
{
  DEBUG((DEBUG_INFO, "%a: Start. \n", __FUNCTION__));

  if (Device->ControllerData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a: Oacs:0x%2x, Fna:0x%x\n", __FUNCTION__, Device->ControllerData->Oacs, Device->ControllerData->Fna));
  if ((Device->ControllerData->Oacs & FORMAT_NVM_SUPPORTED)) {
    return NvmeFormatNvmCommand (Private->NvmePassthru, Device->NameSpaceId, UserDataEraseMode);;
  }
  return EFI_UNSUPPORTED;
}

/**
  Perform crypto erase operation on specific NVMe device.

  @param[in] Private   Pointer to the device private data.
  @param[in] Device    Pointer to the device.

  @retval EFI_SUCCESS  Operation is successful
  @retval Others       Operation failed due to an error

**/
EFI_STATUS
NvmeDataCryptoErase (
  IN RST_ERASE_NVME_PRIVATE  *Private,
  IN RST_NVME                *Device
  )
{
  DEBUG((DEBUG_INFO, "%a: Start. \n", __FUNCTION__));

  if (Device->ControllerData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a: Oacs:0x%2x, Fna:0x%x\n", __FUNCTION__, Device->ControllerData->Oacs, Device->ControllerData->Fna));
  if ((Device->ControllerData->Oacs & FORMAT_NVM_SUPPORTED) && (Device->ControllerData->Fna & CRYPTO_ERASE_SUPPORTED)) {
    return NvmeFormatNvmCommand (Private->NvmePassthru , Device->NameSpaceId, CryptoEraseMode);
  }
  return EFI_UNSUPPORTED;
}

/**
  Erase a specified number of device blocks.

  @param[in] This       Indicates a pointer to the calling context.
  @param[in] DevId      Namespace id of NVMe, Port id of SATA.
  @param[in] Lba        The starting logical block address to be erased.
                        The caller is responsible for erasing only legitimate locations.
  @param[in] Size       The size in bytes to be erased. This must be a multiple
                        of the physical block size of the device.
  @param[in] EraseType  Erase type to be used to erase data on device.

  @retval EFI_SUCCESS             The erase request was queued if Event is not
                                  NULL. The data was erased correctly to the
                                  device if the Event is NULL.to the device.
  @retval EFI_UNSUPPORTED         device not support this command.
  @retval EFI_INVALID_PARAMETER   The erase request contains LBAs that are not
                                  valid.

**/
EFI_STATUS
EFIAPI
NvmeErase (
  IN RST_ERASE_PROTOCOL        *This,
  IN COMMAND_ID                DevId,
  IN EFI_LBA                   Lba,
  IN UINTN                     Size,
  IN UINT8                     EraseType
  )
{
  EFI_STATUS              Status;
  RST_NVME                *Dev;
  RST_ERASE_NVME_PRIVATE  *Private;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Private = PRIVATE_FROM_RST_NVME_ERASE (This);
  DEBUG ((DEBUG_INFO, "%a - enter (DeviceId = 0x%x, erase type = %d)\n", __FUNCTION__, DevId.Value, EraseType));

  Dev = GetNvmeDevice (Private, DevId.NameSpaceId);
  if (Dev == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // Perfore erase action
  //
  switch (EraseType) {
    case SANITIZE_BLOCK_ERASE:
      Status = NvmeDataBlockErase (Private, Dev);
      break;
    case SANITIZE_CRYPTO_ERASE:
      Status = NvmeDataCryptoErase (Private, Dev);
      break;
    case SANITIZE_CLEAR:
      Status = NvmeDataClear (Private, Dev);
      break;
    default:
      Status = EFI_UNSUPPORTED;
    break;
  }

  //
  // Store the erase result
  //
  Dev->Method = EraseType;
  Dev->Result = Status;
  DEBUG((DEBUG_INFO, "Update result list (Id:%d, Method:%d, Result:%r)\n", Dev->NameSpaceId, Dev->Method, Dev->Result));

  return Status;
}

/**
  Return the device list of nvme in VMD mode.

  @param[in] This         Indicates a pointer to the calling context.
  @param[out] NumOfDevice Amount of NVMe device in VMD mode.
  @param[out] DeviceList  A pointer to the list of device.

  @retval EFI_SUCCESS             Get device successfully.
  @retval EFI_NOT_FOUND           There is no device can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetNvmeDeviceList (
  IN     RST_ERASE_PROTOCOL        *This,
  OUT    UINT32                    *NumOfDevice,
  OUT    RST_DEVICE                **DeviceList
  )
{
  EFI_STATUS              Status;
  LIST_ENTRY              *Node;
  RST_NVME                *Dev;
  RST_ERASE_NVME_PRIVATE  *Private;
  UINT32                   Count;
  UINT32                  Index;
  RST_DEVICE              *DevTmp;
  UINTN                   DevicePathSize;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a - enter\n", __FUNCTION__));
  Private = PRIVATE_FROM_RST_NVME_ERASE (This);
  //
  // Get all the device
  //
  for (Node = GetFirstNode (&Private->DeviceList), Count = 0;
       !IsNull (&Private->DeviceList, Node);
       Node = GetNextNode (&Private->DeviceList, Node)) {
    Dev = RST_NVME_FROM_LINK (Node);
    //
    // Get identify data from devices
    //
    if (Dev->ControllerData == NULL) {
      Dev->ControllerData = AllocateZeroPool (sizeof (NVME_ADMIN_CONTROLLER_DATA));
      if (Dev->ControllerData != NULL) {
        Status = NvmeIdentifyController (Private->NvmePassthru, Dev->NameSpaceId, Dev->ControllerData);
        if (EFI_ERROR (Status)) {
          FreePool (Dev->ControllerData);
          Dev->ControllerData = NULL;
        }
      }
    }
    Count ++;
  }

  if (Count == 0) {
    *NumOfDevice = 0;
    return EFI_NOT_FOUND;
  }
  DEBUG ((DEBUG_INFO, "%a - count = %d\n", __FUNCTION__, Count));

  //
  // Find out all the device which has device handle installed
  //
  UpdateNvmeDiskInfo (Private);

  DevTmp = AllocateZeroPool (Count * sizeof (RST_DEVICE));
  if (DevTmp == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Get all the device
  //
  for (Node = GetFirstNode (&Private->DeviceList), Index = 0;
       !IsNull (&Private->DeviceList, Node);
       Node = GetNextNode (&Private->DeviceList, Node), Index ++) {
    Dev = RST_NVME_FROM_LINK (Node);
    DevTmp[Index].Id.Value = Dev->NameSpaceId;
    DevTmp[Index].ParentHandle = Dev->ParentHandle;
    if (Dev->ControllerData != NULL) {
      DevTmp[Index].VidDid= (Dev->ControllerData->Vid << 16);
      CopyMem(DevTmp[Index].ModelName, Dev->ControllerData->Mn, sizeof(Dev->ControllerData->Mn));
      CopyMem(DevTmp[Index].SerialNumber, Dev->ControllerData->Sn, sizeof(Dev->ControllerData->Sn));
    }
    if (Dev->DevicePath != NULL) {
      DevicePathSize = GetDevicePathSize (Dev->DevicePath);
      DevTmp[Index].DevicePath = AllocateZeroPool (DevicePathSize);
      if (DevTmp[Index].DevicePath == NULL) {
        continue;
      }
      CopyMem (DevTmp[Index].DevicePath, Dev->DevicePath, DevicePathSize);
      DevTmp[Index].Handle = Dev->Handle;
    }
  }
  *NumOfDevice = Count;
  *DeviceList = DevTmp;
  return EFI_SUCCESS;
}

/**
  Return the result of erase operation on the device.

  @param[in] This          Indicates a pointer to the calling context.
  @param[in] DeviceId      Namespace id.
  @param[out] EraseResult  A pointer to the data of erase result.

  @retval EFI_SUCCESS             Get result of erase operation successfully.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetNvmeEraseResult (
  IN  RST_ERASE_PROTOCOL   *This,
  IN  COMMAND_ID           DeviceId,
  OUT VOID                 **EraseResult
  )
{
  RST_ERASE_NVME_PRIVATE  *Private;
  RST_NVME                *Dev;
  RST_DEVICE              *FinalResult;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = PRIVATE_FROM_RST_NVME_ERASE (This);
  FinalResult = NULL;

  DEBUG ((DEBUG_INFO, "%a - enter (DeviceId = 0x%x)\n", __FUNCTION__, DeviceId.Value));
  Dev = GetNvmeDevice (Private, DeviceId.NameSpaceId);
  if (Dev != NULL) {
    FinalResult = AllocateZeroPool (sizeof (RST_DEVICE));
    if (FinalResult == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    FinalResult->Id.Value = Dev->NameSpaceId;
    if (Dev->ControllerData != NULL) {
      FinalResult->VidDid= (Dev->ControllerData->Vid << 16);
      CopyMem(FinalResult->ModelName, Dev->ControllerData->Mn, sizeof(Dev->ControllerData->Mn));
      CopyMem(FinalResult->SerialNumber, Dev->ControllerData->Sn, sizeof(Dev->ControllerData->Sn));
    }
    FinalResult->Method = Dev->Method;
    FinalResult->Result = Dev->Result;
  }

  if (FinalResult == NULL) {
    return EFI_NOT_FOUND;
  }

  *EraseResult = FinalResult;

  DEBUG ((DEBUG_INFO, "%a - exit\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Get the time.

  @param[in]  This         Indicates a pointer to the calling context.
  @param[in]  DeviceId     Namespace id.
  @param[in]  EraseType    Erase type will be used to erase device block.

  @retval UINT64           Return time
**/
UINT64
EFIAPI
GetNvmeEstimateTime (
  IN  RST_ERASE_PROTOCOL   *This,
  IN  COMMAND_ID           DeviceId,
  IN  UINT8                EraseType
  )
{
  EFI_STATUS              Status;
  RST_NVME                *Dev;
  RST_ERASE_NVME_PRIVATE  *Private;
  EFI_BLOCK_IO_PROTOCOL   *BlkIo;
  UINT32                  BlockNumber;
  UINT64                  Time;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = PRIVATE_FROM_RST_NVME_ERASE (This);
  Dev     = NULL;
  Time    = 0;
  DEBUG ((DEBUG_INFO, "%a - enter (DeviceId = 0x%x)\n", __FUNCTION__, DeviceId.Value));

  Dev = GetNvmeDevice (Private, DeviceId.NameSpaceId);
  if (Dev == NULL) {
    return 0;
  }

  switch (EraseType) {
    case SANITIZE_CLEAR:
    {
      Status = gBS->HandleProtocol (Dev->Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
      if (!EFI_ERROR (Status) && (BlkIo != NULL) && (BlkIo->Media != NULL)){
        BlockNumber = (UINT32)((BlkIo->Media->LastBlock + 1) * BlkIo->Media->BlockSize / SIZE_4GB);
        if ((BlockNumber == 0) && (BlkIo->Media->BlockSize != 0)) {
          //
          // Set to 1 when media size is smaller than 4GB
          //
          BlockNumber = 1;
        }
        Time = BlockNumber * NVME_SANITIZE_TIME_CLEAR_4G_MAX;
        Time = Time << 32;
        Time += BlockNumber * NVME_SANITIZE_TIME_CLEAR_4G_MIN;
      }
      return Time;
    }
    case SANITIZE_CRYPTO_ERASE:
      if ((Dev->ControllerData != NULL)
        && (Dev->ControllerData->Oacs & FORMAT_NVM_SUPPORTED)
        && (Dev->ControllerData->Fna & CRYPTO_ERASE_SUPPORTED)) {
        return NVME_SANITIZE_TIME_FORMAT;
      }
      break;
    case SANITIZE_BLOCK_ERASE:
      if ((Dev->ControllerData != NULL) && (Dev->ControllerData->Oacs & FORMAT_NVM_SUPPORTED)) {
        return NVME_SANITIZE_TIME_FORMAT;
      }
      break;
    default:
      break;
  }
  return 0;
}
