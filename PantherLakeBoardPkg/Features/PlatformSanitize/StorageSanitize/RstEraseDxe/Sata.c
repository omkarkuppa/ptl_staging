/**@file
  RST Storage Erase Driver for SATA.

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

CHAR8  mDefaultPassStr[ATA_PASSWORD_MAX_LENGTH] = "PLATFORMERASE";

#define SATA_SANITIZE_TIME_CLEAR_4G_MIN  15        // Minimum 15 second to clear 4G SATA media
#define SATA_SANITIZE_TIME_CLEAR_4G_MAX  60        // Minimum 60 second to clear 4G SATA media

/**
  Enumerate Sata device

  @param[in] Private   Pointer to the device private data.
  @param[in] Handle    Handle of the device's parent.

  @retval EFI_SUCCESS  Operation is successful
  @retval Others       Operation failed due to an error

**/
EFI_STATUS
EnumerateSataDevices (
  IN RST_ERASE_SATA_PRIVATE  *Private,
  IN EFI_HANDLE              Handle
  )
{
  EFI_STATUS                  Status;
  UINT16                      Port;
  UINT16                      PortMultiplier;
  RST_SATA                    *Dev;

  InitializeListHead (&Private->DeviceList);

  Port = 0xFFFF;
  while (TRUE) {
    Status = Private->AtaPassthru->GetNextPort (Private->AtaPassthru, &Port);
    if (EFI_ERROR (Status)) {
      if (Status == EFI_NOT_FOUND) {
        break;
      } else {
        return Status;
      }
    }
    DEBUG ((DEBUG_INFO, "GetNextPort: Port = 0x%x\n", Port));
    PortMultiplier = 0xFFFF;
    while (TRUE) {
      Status = Private->AtaPassthru->GetNextDevice (Private->AtaPassthru, Port, &PortMultiplier);
      if (EFI_ERROR (Status)) {
        if (Status == EFI_NOT_FOUND) {
          break;
        } else {
          return Status;
        }
      }
      DEBUG ((DEBUG_INFO, "GetNextDevice: PortMultiplier = 0x%x\n", PortMultiplier));
      //
      // Get ATA identify data
      //
      Dev = AllocateZeroPool (sizeof (RST_SATA));
      if (Dev != NULL) {
        Dev->Signature = RST_SATA_SIGNATURE;
        Dev->Port = Port;
        Dev->PortMultiplier = PortMultiplier;
        Dev->Result = EFI_UNSUPPORTED;
        Dev->ParentHandle = Handle;
        InsertTailList (&Private->DeviceList, &Dev->Link);
        DEBUG ((DEBUG_INFO, "%a - Add RST Sata device to the list, Port = 0x%x\n", __FUNCTION__, Port));
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Get Sata device

  @param[in] Private Pointer to the device private data.
  @param[in] Port    Port id.

  @retval RST_NVME   Pointer to the Nvme device
  @retval NULL       No Nvme device

**/
RST_SATA*
GetSataDevice (
  IN RST_ERASE_SATA_PRIVATE  *Private,
  IN UINT16                  Port
  )
{
  LIST_ENTRY              *Node;
  RST_SATA                *Dev;

  //
  // Locate the device structure
  //
  for (Node = GetFirstNode (&Private->DeviceList);
       !IsNull (&Private->DeviceList, Node);
       Node = GetNextNode (&Private->DeviceList, Node)) {
    Dev = RST_SATA_FROM_LINK (Node);
    if (Dev->Port == Port) {
      return Dev;
    }
  }
  return NULL;
}

/**
  Update Sata device list with port id, device path, Handle and so on.

  @param[in] Private    Pointer to the device private data.

  @retval EFI_SUCCESS   Successful

**/
EFI_STATUS
UpdateSataDiskInfo (
  IN RST_ERASE_SATA_PRIVATE  *Private
  )
{
  EFI_STATUS                Status;
  UINTN                     NumberOfDiskInfoHandles;
  EFI_HANDLE                *DiskInfoHandleList;
  UINTN                     Index;
  EFI_DEVICE_PATH_PROTOCOL  *TmpDevicePath;
  EFI_DISK_INFO_PROTOCOL    *DiskInfo;
  UINT32                    Port, PortMultiplier;
  RST_SATA                  *Dev;

  DiskInfoHandleList = NULL;
  Port           = 0;
  PortMultiplier = 0;
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
      Status = GetPortNum (TmpDevicePath, (UINT16 *)&Port, (UINT16 *)&PortMultiplier);
      if (EFI_ERROR (Status)) {
        continue;
      } else {
        //
        // check if it is RAID, if RAID no block IO for each device, skip
        //
        if ((Port & (Port - 1)) != 0) {
          continue;
        }
      }
      Status = gBS->HandleProtocol (
                      DiskInfoHandleList[Index],
                      &gEfiDiskInfoProtocolGuid,
                      (VOID **)&DiskInfo
                      );
      if (EFI_ERROR (Status)) {
        continue;
      }
      //
      // Get real port/ port multiplier
      //
      Status = DiskInfo->WhichIde (DiskInfo, (UINT32 *)&Port, (UINT32 *)&PortMultiplier);
      if (EFI_ERROR(Status)) {
        continue;
      }
      Dev = GetSataDevice (Private, (UINT16)Port);
      if (Dev != NULL) {
        Dev->DevicePath = TmpDevicePath;
        Dev->Handle = DiskInfoHandleList[Index];
        DEBUG((DEBUG_INFO, "%a - Update device path (%s) on (Port:%d)\n",
                __FUNCTION__, ConvertDevicePathToText (Dev->DevicePath, TRUE, TRUE),  Dev->Port));
      }
    }
  }
  FreePool (DiskInfoHandleList);
  return EFI_SUCCESS;
}

/**
  Swap string.

  @param[in,out] AtaString  The pointer to the string to be swapped.
  @param[in]     Size       Size of the string.

**/
VOID
SataSwapAtaString (
  IN OUT CHAR8  *AtaString,
  IN UINTN      StrLen
  )
{
  UINTN   Index;
  CHAR8   TempChar;

  if (AtaString == NULL) {
    return;
  }

  //
  // Swap the byte order.
  //
  for (Index = 0; Index < StrLen; Index += 2) {
    TempChar = AtaString[Index + 1];
    AtaString[Index + 1] = AtaString[Index];
    AtaString[Index]     = TempChar;
  }
}

/**
  Perform clear operation on specific SATA device.

  @param[in] Private    Pointer to the device private data.
  @param[in] Device    Pointer to the device.

  @retval EFI_SUCCESS  Operation is successful
  @retval Others       Operation failed due to an error

**/
EFI_STATUS
SataDataClear (
  IN RST_ERASE_SATA_PRIVATE  *Private,
  IN RST_SATA                *Device
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
  Perform block erase operation on specific SATA device.

  @param[in] Private   Pointer to the device private data.
  @param[in] Device    Pointer to the device.

  @retval EFI_SUCCESS  Operation is successful
  @retval Others       Operation failed due to an error

**/
EFI_STATUS
SataDataBlockErase (
  IN RST_ERASE_SATA_PRIVATE  *Private,
  IN RST_SATA                *Device
  )
{
  EFI_STATUS         Status;
  UINT64             Timeout;

  DEBUG((DEBUG_INFO, "%a: Start. \n", __FUNCTION__));

  if (Device->IdentifyData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "multi_sector_setting 0x%04x\n", Device->IdentifyData->multi_sector_setting));
  DEBUG ((DEBUG_INFO, "security_status 0x%04x\n", Device->IdentifyData->security_status));

  if ((Device->IdentifyData->security_status & ATA_SECURITY_STATUS_FROZEN_BIT) != 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Security erase command
  //
  if ((Device->IdentifyData->security_status & ATA_SECURITY_STATUS_ENABLED_BIT) == 0 &&
    (Device->IdentifyData->security_status & ATA_SECURITY_STATUS_FEATURE_SET_BIT) != 0) {
    Status = SetAtaPassword (
               Private->AtaPassthru,
               Device->Port,
               Device->PortMultiplier,
               ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD,
               ATA_SECURITY_ERASE_LEVEL_MAXIMUM,
               0,
               mDefaultPassStr
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Timeout = (UINT64)(Device->IdentifyData->time_for_security_erase_unit) * 2 * 60;
    Timeout = EFI_TIMER_PERIOD_SECONDS (Timeout);
    Status = SendAtaSecureErase (
               Private->AtaPassthru,
               Device->Port,
               Device->PortMultiplier,
               ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD,
               ATA_SECURITY_ERASE_MODE_NORMAL,
               Timeout,
               mDefaultPassStr
               );
    Status = DisableAtaPassword (
               Private->AtaPassthru,
               Device->Port,
               Device->PortMultiplier,
               ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD,
               mDefaultPassStr
               );
    return Status;
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
SataErase (
  IN RST_ERASE_PROTOCOL        *This,
  IN COMMAND_ID                DevId,
  IN EFI_LBA                   Lba,
  IN UINTN                     Size,
  IN UINT8                     EraseType
  )
{
  EFI_STATUS              Status;
  RST_SATA                *Dev;
  RST_ERASE_SATA_PRIVATE  *Private;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Private = PRIVATE_FROM_RST_SATA_ERASE (This);

  DEBUG ((DEBUG_INFO, "%a - enter (DeviceId = 0x%x, erase type = %d)\n", __FUNCTION__, DevId.Value, EraseType));

  //
  // Locate the device structure
  //
  Dev = GetSataDevice (Private, DevId.Sata.Port);
  if (Dev == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // Update Identify data
  //
  if (Dev->IdentifyData == NULL) {
    Dev->IdentifyData = AllocateZeroPool (sizeof (ATA_IDENTIFY_DATA));
  }
  if (Dev->IdentifyData != NULL) {
    ZeroMem(Dev->IdentifyData, sizeof (ATA_IDENTIFY_DATA));
    Status = GetAtaIdentifyData (Private->AtaPassthru, Dev->Port, Dev->PortMultiplier, Dev->IdentifyData);
    if (EFI_ERROR (Status)) {
      FreePool (Dev->IdentifyData);
      Dev->IdentifyData = NULL;
    }
  }

  //
  // Perfore erase action
  //
  switch (EraseType) {
    case SANITIZE_BLOCK_ERASE:
      Status = SataDataBlockErase (Private, Dev);
      break;
    case SANITIZE_CLEAR:
      Status = SataDataClear (Private, Dev);
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
  DEBUG((DEBUG_INFO, "Update result list (Port:%d, Method:%d, Result:%r)\n", Dev->Port, Dev->Method, Dev->Result));

  return Status;
}

/**
  Return the device list of SATA in VMD mode.

  @param[in] This         Indicates a pointer to the calling context.
  @param[out] NumOfDevice Amount of SATA device in VMD mode.
  @param[out] DeviceList  A pointer to the list of device.

  @retval EFI_SUCCESS             Get device successfully.
  @retval EFI_NOT_FOUND           There is no device can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetSataDeviceList (
  IN  RST_ERASE_PROTOCOL        *This,
  OUT UINT32                    *NumOfDevice,
  OUT RST_DEVICE                **DeviceList
  )
{
  EFI_STATUS              Status;
  LIST_ENTRY              *Node;
  RST_SATA                *Dev;
  RST_ERASE_SATA_PRIVATE  *Private;
  UINT32                  Count;
  UINT32                  Index;
  RST_DEVICE              *DevTmp;
  UINTN                   DevicePathSize;

  if (This == NULL || NumOfDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a - enter\n", __FUNCTION__));
  Private = PRIVATE_FROM_RST_SATA_ERASE (This);
  //
  // Get all the device
  //
  for (Node = GetFirstNode (&Private->DeviceList), Count = 0;
       !IsNull (&Private->DeviceList, Node);
       Node = GetNextNode (&Private->DeviceList, Node)) {
    Dev = RST_SATA_FROM_LINK (Node);
    //
    // Get identify data from devices
    //
    if (Dev->IdentifyData == NULL) {
      Dev->IdentifyData = AllocateZeroPool (sizeof (ATA_IDENTIFY_DATA));
      if (Dev->IdentifyData != NULL) {
        Status = GetAtaIdentifyData (Private->AtaPassthru, Dev->Port, Dev->PortMultiplier, Dev->IdentifyData);
        if (EFI_ERROR (Status)) {
          FreePool (Dev->IdentifyData);
          Dev->IdentifyData = NULL;
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
  UpdateSataDiskInfo (Private);

  //
  // Prepare the data buffer to return
  //
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
    Dev = RST_SATA_FROM_LINK (Node);
    DevTmp[Index].Id.Sata.Port = Dev->Port;
    DevTmp[Index].Id.Sata.PortMultiplier = Dev->PortMultiplier;
    DevTmp[Index].ParentHandle = Dev->ParentHandle;
    if (Dev->IdentifyData != NULL) {
      SataSwapAtaString (Dev->IdentifyData->ModelName, AsciiStrLen (Dev->IdentifyData->ModelName) + 1);
      SataSwapAtaString (Dev->IdentifyData->SerialNo, AsciiStrLen (Dev->IdentifyData->SerialNo) + 1);
      AsciiStrCpyS (DevTmp[Index].ModelName, AsciiStrLen (Dev->IdentifyData->ModelName) + 1, Dev->IdentifyData->ModelName);
      AsciiStrCpyS (DevTmp[Index].SerialNumber, AsciiStrLen (Dev->IdentifyData->SerialNo) + 1, Dev->IdentifyData->SerialNo);
    }
    if (Dev->DevicePath != NULL) {
      DevicePathSize = GetDevicePathSize (Dev->DevicePath);
      DevTmp[Index].DevicePath = AllocateZeroPool (DevicePathSize);
      ASSERT (DevTmp[Index].DevicePath != NULL);
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
  @param[in] DeviceId      Port id.
  @param[out] EraseResult  A pointer to the data of erase result.

  @retval EFI_SUCCESS             Get result of erase operation successfully.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetSataEraseResult (
  IN  RST_ERASE_PROTOCOL   *This,
  IN  COMMAND_ID           DeviceId,
  OUT VOID                 **EraseResult
  )
{
  RST_ERASE_SATA_PRIVATE  *Private;
  RST_SATA                *Dev;
  RST_DEVICE              *FinalResult;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = PRIVATE_FROM_RST_SATA_ERASE (This);
  FinalResult = NULL;
  DEBUG ((DEBUG_INFO, "%a - enter (DeviceId = 0x%x)\n", __FUNCTION__, DeviceId.Value));

  Dev = GetSataDevice (Private, DeviceId.Sata.Port);
  if (Dev != NULL) {
    FinalResult = AllocateZeroPool (sizeof (RST_DEVICE));
    if (FinalResult == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    FinalResult->Id.Sata.Port = Dev->Port;
    FinalResult->Id.Sata.PortMultiplier = Dev->PortMultiplier;
    if (Dev->IdentifyData != NULL) {
      AsciiStrCpyS (FinalResult->ModelName, AsciiStrLen (Dev->IdentifyData->ModelName) + 1, Dev->IdentifyData->ModelName);
      AsciiStrCpyS (FinalResult->SerialNumber, AsciiStrLen (Dev->IdentifyData->SerialNo) + 1, Dev->IdentifyData->SerialNo);
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
  @param[in]  DeviceId     Sata port id.
  @param[in]  EraseType    Erase type will be used to erase device block.

  @retval UINT64           Return time
**/
UINT64
EFIAPI
GetSataEstimateTime (
  IN  RST_ERASE_PROTOCOL   *This,
  IN  COMMAND_ID           DeviceId,
  IN  UINT8                EraseType
  )
{
  EFI_STATUS              Status;
  RST_SATA                *Dev;
  RST_ERASE_SATA_PRIVATE  *Private;
  EFI_BLOCK_IO_PROTOCOL   *BlkIo;
  UINT32                  BlockNumber;
  UINT64                  Time;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = PRIVATE_FROM_RST_SATA_ERASE (This);
  Dev     = NULL;
  Time    = 0;
  DEBUG ((DEBUG_INFO, "%a - enter (DeviceId = 0x%x)\n", __FUNCTION__, DeviceId.Value));

  Dev = GetSataDevice (Private, DeviceId.Sata.Port);
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
        Time = BlockNumber * SATA_SANITIZE_TIME_CLEAR_4G_MAX;
        Time = Time << 32;
        Time += BlockNumber * SATA_SANITIZE_TIME_CLEAR_4G_MIN;
      }
      return Time;
    }
    case SANITIZE_BLOCK_ERASE:
      if ((Dev->IdentifyData!= NULL)) {
        return (UINT64)(Dev->IdentifyData->time_for_security_erase_unit) * 2 * 60;;
      }
      break;
    default:
      break;
  }
  return 0;
}
