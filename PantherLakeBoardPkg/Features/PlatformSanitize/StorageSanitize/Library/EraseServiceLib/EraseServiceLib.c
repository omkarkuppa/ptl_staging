/**@file
  Erase service library file

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Protocol/NvmeEraseBlock.h>
#include <Protocol/SataEraseBlock.h>
#include <Protocol/UfsEraseBlock.h>
#include <Protocol/DevicePath.h>
#include <Protocol/RstEraseProtocol.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/StorageServiceLib.h>
#include <Library/EraseServiceLib.h>

/**
  Trim the space at end of string

  @param[in] String    String input.
**/
VOID
TrimSpace (
  IN CHAR8  *String
  )
{
  UINTN Length;
  UINTN Index;
  if (String == NULL) {
    return;
  }
  Length = AsciiStrLen (String);
  if (Length < 2) {
    return;
  }
  Index = 0;
  while (Index < (Length - 1)) {
    if ((String[Index] == L' ') && (String[Index + 1] == L' ')) {
      ZeroMem (&String[Index], Length - Index);
      break;
    }
    Index += 2;
  }
}

/**
  Get the Rst Nvme / Sata Erase protocol

  @param[in] DevType    Device type: Nvme or Sata.
  @param[in] Protocol   Pointer to the protocol.

  @retval EFI_SUCCESS   Successful get the protocol
  @retval Others        Failure to get the protocl
**/
EFI_STATUS
GetRstProtocol (
  IN  DEVICE_TYPE        DevType,
  OUT VOID               **Protocol
  )
{
  EFI_STATUS             Status;
  RST_ERASE_PROTOCOL     *RstEraseProtocol;

  RstEraseProtocol = NULL;
  Status = EFI_SUCCESS;

  if (Protocol == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DevType == NVME) {
    Status = gBS->LocateProtocol (
                    &gRstNvmeEraseProtocolGuid,
                    NULL,
                    (VOID **)&RstEraseProtocol
                    );
  } else if (DevType == SATA) {
    Status = gBS->LocateProtocol (
                    &gRstSataEraseProtocolGuid,
                    NULL,
                    (VOID **)&RstEraseProtocol
                    );
  }
  if (EFI_ERROR (Status) || (RstEraseProtocol == NULL)) {
    return Status;
  }
  *Protocol = (VOID*) RstEraseProtocol;
  return EFI_SUCCESS;
}

/**
  Check if the device is handled by erase protocol

  @param[in] DevHandle   Device handle.
  @param[in] DevicePath  Device path.

  @retval TRUE   Indicate the device is handled by erase protocol.
  @retval FALSE  Indicate the device is not handled by erase protocol.

**/
BOOLEAN
EFIAPI
IsEraseSupported (
  IN EFI_HANDLE                DevHandle,
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_STATUS  Status;
  VOID        *EraseBlocks;

  if (EFI_SUCCESS == GetNamespaceId (DevicePath, NULL)) {
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gNvmeEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      return TRUE;
    }
  } else if (EFI_SUCCESS == GetPortNum (DevicePath, NULL, NULL)) {
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gSataEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      return TRUE;
    }
  } else if (EFI_SUCCESS == GetUfsDevice (DevicePath)) {
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gUfsEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      return TRUE;
    }
  }

  return FALSE;
}


/**
  Enumerate the NVME / SATA device from RST, return the device list.

  @param[out]  DeviceList      A pointer to a buffer of RST_DEVICE list.
  @param[out]  DeviceNumber    A pointer to the number of entries in DeviceList.

  @retval EFI_SUCCESS             Get RST devices list.
  @retval EFI_NOT_FOUND           There is no device in RST.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.
**/
EFI_STATUS
EFIAPI
HandleRstDevice (
  IN  DEVICE_TYPE       DevType,
  OUT UINT32           *DeviceNumber,
  OUT VOID             **DeviceList
  )
{
  EFI_STATUS             Status;
  RST_ERASE_PROTOCOL     *RstEraseProtocol;
  UINT32                 NumOfDevice;
  RST_DEVICE             *RstDeviceList;

  DEBUG ((DEBUG_INFO, "%a - enter\n", __FUNCTION__));

  if ((DeviceNumber == NULL) || (DeviceList == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *DeviceNumber    = 0;
  *DeviceList      = NULL;
  Status           = EFI_NOT_FOUND;
  RstEraseProtocol = NULL;

  Status = GetRstProtocol (DevType, (VOID**) &RstEraseProtocol);
  if (EFI_ERROR (Status) || (RstEraseProtocol == NULL)) {
    return Status;
  }
  Status = RstEraseProtocol->GetDeviceList (RstEraseProtocol, &NumOfDevice, &RstDeviceList);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  *DeviceNumber = NumOfDevice;
  *DeviceList   = RstDeviceList;
  return EFI_SUCCESS;
}

/**
  Get the device information.

  @param[in]  RstDevice       A pointer to a buffer of RST_DEVICE list.
  @param[in]  Index           Location in the device list.
  @param[out] Handle          Handle of the Rst device.
  @param[out] ParentHandle    Handle of the Rst parent device.
  @param[out] CommandId       Namespace id or port id.

  @retval EFI_SUCCESS             Get RST device information.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.
**/
EFI_STATUS
EFIAPI
GetRstDevice (
  IN  VOID              *RstDevice,
  IN  UINT32            Index,
  OUT EFI_HANDLE        *Handle,
  OUT EFI_HANDLE        *ParentHandle,
  OUT UINT32            *CommandId
  )
{
  RST_DEVICE             *RstDeviceList;

  DEBUG ((DEBUG_INFO, "%a - enter\n", __FUNCTION__));

  RstDeviceList = RstDevice;
  if ((RstDevice == NULL) || (&RstDeviceList[Index] == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Handle != NULL) && (RstDeviceList[Index].Handle != NULL)) {
    *Handle = RstDeviceList[Index].Handle;
  }
  if (ParentHandle != NULL) {
    *ParentHandle = RstDeviceList[Index].ParentHandle;
  }
  if (CommandId != NULL) {
    *CommandId = RstDeviceList[Index].Id.Value;
  }
  return EFI_SUCCESS;
}

/**
  Call service of erase block protocol to perform erase operation

  @param[in] DevHandle   Device handle.
  @param[in] DevType     Device type.
  @param[in] EraseType   Erase type to be used to erase data on the device.
  @param[in] MediaId     The media ID that the erase request is for.
  @param[in] Size        The size in bytes to be erased. This must be a multiple
                         of the physical block size of the device.

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
PerformEraseService (
  IN EFI_HANDLE                DevHandle,
  IN DEVICE_TYPE               DevType,
  IN UINT8                     EraseType,
  IN UINT32                    MediaId,
  IN UINTN                     Size
  )
{
  EFI_STATUS  Status;
  VOID        *EraseBlocks;

  Status = EFI_UNSUPPORTED;

  if (DevType == NVME) {
    //
    // Perform erase operation for NVMe device
    //
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gNvmeEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      Status = ((NVME_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->EraseBlocks (
                                                             EraseBlocks,
                                                             MediaId,
                                                             0,
                                                             NULL,
                                                             Size,
                                                             EraseType,
                                                             NULL);
    }
  } else if (DevType == SATA) {
    //
    // Perform erase operation for SATA device
    //
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gSataEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      Status = ((SATA_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->EraseBlocks (
                                                             EraseBlocks,
                                                             MediaId,
                                                             0,
                                                             NULL,
                                                             Size,
                                                             EraseType,
                                                             NULL);
    }
  } else if (DevType == UFS) {
    //
    // Perform erase operation for UFS device
    //
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gUfsEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      Status = ((UFS_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->EraseBlocks (
                                                            EraseBlocks,
                                                            MediaId,
                                                            0,
                                                            NULL,
                                                            Size,
                                                            EraseType,
                                                            NULL);
    }
  }

  return Status;
}

/**
  Call service of RST_ERASE_PROTOCOL to perform erase operation

  @param[in] DeviceId    Namespace id or port id.
  @param[in] DevType     Device type.
  @param[in] EraseType   Erase type to be used to erase data on the device.
  @param[in] Size        The size in bytes to be erased. This must be a multiple
                         of the physical block size of the device.

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
PerformRstEraseService (
  IN COMMAND_ID                DeviceId,
  IN DEVICE_TYPE               DevType,
  IN UINT8                     EraseType,
  IN UINTN                     Size
  )
{
  EFI_STATUS    Status;
  VOID          *EraseProtocol;

  Status = EFI_UNSUPPORTED;

  Status = GetRstProtocol (DevType, (VOID**) &EraseProtocol);
  if (!EFI_ERROR (Status)) {
    Status = ((RST_ERASE_PROTOCOL *)EraseProtocol)->Erase (
                                                      EraseProtocol,
                                                      DeviceId,
                                                      0,
                                                      Size,
                                                      EraseType);
  }
  return Status;
}
/**
  Call service of erase block protocol to get erase device information

  @param[in] DevHandle     Device handle.
  @param[in] DevType       Device type.
  @param[out] EraseInfo    A pointer to the erase information.

  @retval EFI_SUCCESS             Get information of erase devices.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetEraseInfo (
  IN EFI_HANDLE                DevHandle,
  IN DEVICE_TYPE               DevType,
  OUT ERASE_INFO               *EraseInfo
  )
{
  EFI_STATUS    Status;
  VOID          *EraseBlocks;
  VOID          *DevInfo;
  ERASE_INFO    TmpInfo;

  if (EraseInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
  DevInfo =  NULL;
  ZeroMem (&TmpInfo, sizeof (ERASE_INFO));

  if (DevType == NVME) {
    //
    // Get result list from NVMe device
    //
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gNvmeEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      Status = ((NVME_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->GetEraseInfo (EraseBlocks, &DevInfo);
      if (!EFI_ERROR (Status)) {
        TmpInfo.Method = ((NVME_ERASE_INFO *)DevInfo)->Method;
        CopyMem(TmpInfo.ModelName, ((NVME_ERASE_INFO *)DevInfo)->ModelName, MN_MAX_LEN);
        CopyMem(TmpInfo.SerialNumber, ((NVME_ERASE_INFO *)DevInfo)->SerialNumber, SN_MAX_LEN);
      }
    }
  } else if (DevType == SATA) {
    //
    // Get result list from SATA device
    //
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gSataEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      Status = ((SATA_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->GetEraseInfo (EraseBlocks, &DevInfo);
      if (!EFI_ERROR (Status)) {
        TmpInfo.Method = ((SATA_ERASE_INFO *)DevInfo)->Method;
        CopyMem(TmpInfo.ModelName, ((SATA_ERASE_INFO *)DevInfo)->ModelName, MN_MAX_LEN);
        CopyMem(TmpInfo.SerialNumber, ((SATA_ERASE_INFO *)DevInfo)->SerialNumber, SN_MAX_LEN);
      }
    }
  } else if (DevType == UFS) {
    //
    // Get result list from UFS device
    //
    Status = gBS->HandleProtocol (
                    DevHandle,
                    &gUfsEraseBlockProtocolGuid,
                    (VOID**)&EraseBlocks
                    );
    if (!EFI_ERROR (Status)) {
      Status = ((UFS_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->GetEraseInfo (EraseBlocks, &DevInfo);
      if (!EFI_ERROR (Status)) {
        TmpInfo.Method = ((UFS_ERASE_INFO *)DevInfo)->Method;
        CopyMem(TmpInfo.Manufacturer, ((UFS_ERASE_INFO *)DevInfo)->Manufacturer, MANUFACURER_MAX_LEN);
        CopyMem(TmpInfo.ModelName, ((UFS_ERASE_INFO *)DevInfo)->ModelName, MN_MAX_LEN);
        CopyMem(TmpInfo.SerialNumber, ((UFS_ERASE_INFO *)DevInfo)->SerialNumber, SN_MAX_LEN);
      }
    }
  }

  if (!EFI_ERROR (Status)) {
    TrimSpace (TmpInfo.Manufacturer);
    TrimSpace (TmpInfo.ModelName);
    TrimSpace (TmpInfo.SerialNumber);
    CopyMem (EraseInfo, &TmpInfo, sizeof (ERASE_INFO));
  }
  if (DevInfo != NULL) {
    FreePool (DevInfo);
  }

  return Status;
}

/**
  Call service of RST_ERASE_PROTOCOL to get erase device information

  @param[in] DeviceId      Namespace id or port id.
  @param[in] DevType       Device type.
  @param[out] EraseInfo    A pointer to the erase information.

  @retval EFI_SUCCESS             Get information of erase devices.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetRstEraseInfo (
  IN COMMAND_ID                DeviceId,
  IN DEVICE_TYPE               DevType,
  OUT ERASE_INFO               *EraseInfo
  )
{
  EFI_STATUS    Status;
  VOID          *EraseProtocol;
  VOID          *DevInfo;
  ERASE_INFO    TmpInfo;

  if (EraseInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
  DevInfo =  NULL;
  ZeroMem (&TmpInfo, sizeof (ERASE_INFO));

  Status = GetRstProtocol (DevType, (VOID**) &EraseProtocol);
  if (!EFI_ERROR (Status)) {
    Status = ((RST_ERASE_PROTOCOL *)EraseProtocol)->GetEraseResult (EraseProtocol, DeviceId, &DevInfo);
    if (!EFI_ERROR (Status)) {
      TmpInfo.Method = ((RST_DEVICE *)DevInfo)->Method;
      CopyMem(TmpInfo.ModelName, ((RST_DEVICE *)DevInfo)->ModelName, MN_MAX_LEN);
      CopyMem(TmpInfo.SerialNumber, ((RST_DEVICE *)DevInfo)->SerialNumber, SN_MAX_LEN);
    }
  }
  if (!EFI_ERROR (Status)) {
    TrimSpace (TmpInfo.Manufacturer);
    TrimSpace (TmpInfo.ModelName);
    TrimSpace (TmpInfo.SerialNumber);
    CopyMem (EraseInfo, &TmpInfo, sizeof (ERASE_INFO));
  }
  if (DevInfo != NULL) {
    FreePool (DevInfo);
  }

  return Status;
}

/**
  Get the time.

  @param[in] DevHandle     Device handle.
  @param[in] DevType       Device type.
  @param[in] EraseType     Erase type will be used to erase device block.

  @retval UINT64           Estimate time.

**/
UINT64
EFIAPI
GetEraseEstimateTime (
  IN EFI_HANDLE                DevHandle,
  IN DEVICE_TYPE               DevType,
  IN UINT8                     EraseType
  )
{
  EFI_STATUS    Status;
  VOID          *EraseBlocks;
  UINT64        Time;

  Time = 0;
  if (DevType == NVME) {
    //
    // Get estimate time from NVMe device
    //
    Status = gBS->HandleProtocol (DevHandle, &gNvmeEraseBlockProtocolGuid, (VOID**)&EraseBlocks);
    if (!EFI_ERROR (Status)) {
      Time = ((NVME_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->GetEstimateTime (EraseBlocks, EraseType);
    }
  } else if (DevType == SATA) {
    //
    // Get estimate time from SATA device
    //
    Status = gBS->HandleProtocol (DevHandle, &gSataEraseBlockProtocolGuid, (VOID**)&EraseBlocks);
    if (!EFI_ERROR (Status)) {
      Time = ((SATA_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->GetEstimateTime (EraseBlocks, EraseType);
    }
  } else if (DevType == UFS) {
    //
    // Get estimate time from UFS device
    //
    Status = gBS->HandleProtocol (DevHandle, &gUfsEraseBlockProtocolGuid, (VOID**)&EraseBlocks);
    if (!EFI_ERROR (Status)) {
      Time = ((UFS_ERASE_BLOCK_PROTOCOL *)EraseBlocks)->GetEstimateTime (EraseBlocks, EraseType);
    }
  }

  return Time;
}

/**
  Get the time.

  @param[in] DeviceId      Namespace id or port id.
  @param[in] DevType       Device type.
  @param[in] EraseType     Erase type will be used to erase device block.

  @retval UINT64           Estimate time.

**/
UINT64
EFIAPI
GetRstEraseEstimateTime (
  IN COMMAND_ID                DeviceId,
  IN DEVICE_TYPE               DevType,
  IN UINT8                     EraseType
  )
{
  EFI_STATUS    Status;
  VOID          *EraseProtocol;
  UINT64        Time;

  Time = 0;
  Status = GetRstProtocol (DevType, (VOID**) &EraseProtocol);
  if (!EFI_ERROR (Status)) {
    Time = ((RST_ERASE_PROTOCOL *)EraseProtocol)->GetEstimateTime (EraseProtocol, DeviceId, EraseType);
  }

  return Time;
}
