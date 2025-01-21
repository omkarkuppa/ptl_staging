/**@file
  Storage service library file

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

#include <PiDxe.h>

#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathToText.h>

#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/StorageServiceLib.h>

/**
  Check if it is UFS device by device path

  @param[in] DevicePath  Device path.

  @retval EFI_SUCCESS    Indicate the device is UFS device
  @retval EFI_NOT_FOUND  Indicate the device is not UFS device

**/
EFI_STATUS
EFIAPI
GetUfsDevice (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;

  DevicePathNode = DevicePath;
  while (!IsDevicePathEndType (DevicePathNode)) {
    if (DevicePathType (DevicePathNode) == MESSAGING_DEVICE_PATH &&
        DevicePathSubType (DevicePathNode) == MSG_UFS_DP) {
      return EFI_SUCCESS;
    }
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }
  return EFI_NOT_FOUND;
}

/**
  Get SATA port number from device path

  @param[in] DevicePath       Device path of device
  @param[out] Port            Port number
  @param[out] PortMultiplier  Port multiplier number

  @retval EFI_STATUS          Successful get the Port
  @retval Others              Fail to get the Port

**/
EFI_STATUS
EFIAPI
GetPortNum (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINT16                   *Port OPTIONAL,
  OUT UINT16                   *PortMultiplier OPTIONAL
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  SATA_DEVICE_PATH          *Node;

  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DevicePathNode = DevicePath;
  while (!IsDevicePathEndType (DevicePathNode)) {
    if (DevicePathType (DevicePathNode) == MESSAGING_DEVICE_PATH &&
        DevicePathSubType (DevicePathNode) == MSG_SATA_DP) {
      Node = (SATA_DEVICE_PATH *)DevicePathNode;
      if (Port != NULL) {
        *Port = Node->HBAPortNumber;
      }
      if (PortMultiplier != NULL) {
        *PortMultiplier = Node->PortMultiplierPortNumber;
      }
      return EFI_SUCCESS;
    }
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }
  return EFI_NOT_FOUND;
}

/**
  Get NVMe device namespace ID from device path

  @param[in] DevicePath  Device path of device
  @param[out] Nsid       Name space ID

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some error

**/
EFI_STATUS
EFIAPI
GetNamespaceId (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINT32                   *Nsid OPTIONAL
  )
{
  EFI_DEVICE_PATH_PROTOCOL    *TempDevicePath;
  NVME_NAMESPACE_DEVICE_PATH  *Node;

  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)DevicePath;
  while (!IsDevicePathEndType (TempDevicePath)) {
    if (DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH &&
          DevicePathSubType (TempDevicePath) == MSG_NVME_NAMESPACE_DP) {
      Node  = (NVME_NAMESPACE_DEVICE_PATH *)TempDevicePath;
      if (Nsid != NULL) {
        *Nsid = Node->NamespaceId;
      }
      return EFI_SUCCESS;
    }
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }
  return EFI_UNSUPPORTED;
}
