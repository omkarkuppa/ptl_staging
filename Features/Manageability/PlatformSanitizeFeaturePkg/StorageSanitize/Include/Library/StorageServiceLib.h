/** @file
  Storage service library header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _STORAGE_SERVICE_LIB_H_
#define _STORAGE_SERVICE_LIB_H_

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
  );

/**
  Get device port number from device path

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
  );

/**
  Get device namespace ID from device path

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
  );

/**
  Disable ATA security on device by port id.

  @param[in] Handle              Device Handle
  @param[in] ParentHandle        Parent Device Handle
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[in] HostPassword        Password received from host
  @param[in] DeviceName          Device name

  @retval EFI_SUCCESS  ATA security was disabled successfully
  @retval Others       An error occurred due to some error
**/
EFI_STATUS
EFIAPI
DisableAtaSecurityWithPortId (
  IN EFI_HANDLE  Handle,
  IN EFI_HANDLE  ParentHandle,
  IN UINT16      PortId,
  IN UINT16      PortMultiplier,
  IN CHAR8       *HostPassword,
  IN CHAR16      *DeviceName
  );

/**
  Get ATA frozen state on device by port id.

  @param[in] Handle              Device Handle
  @param[in] ParentHandle        Parent Device Handle
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[out] FrozenState       Device is frozen or not

  @retval EFI_SUCCESS  ATA security was disabled successfully
  @retval Others       An error occurred due to some error
**/
EFI_STATUS
EFIAPI
GetAtaFrozenStateWithPortId (
  IN EFI_HANDLE  Handle,
  IN EFI_HANDLE  ParentHandle,
  IN UINT16      Port,
  IN UINT16      PortMultiplier,
  OUT UINT8      *FrozenState
  );

/**
  Get ATA security state on device by port id.

  @param[in] Handle              Device Handle
  @param[in] ParentHandle        Parent Device Handle
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.

  @retval TRUE/FALSE   TRUE: ATA device was enabled security feature
**/
BOOLEAN
EFIAPI
GetAtaSecurityStateWithPortId (
  IN EFI_HANDLE  Handle,
  IN EFI_HANDLE  ParentHandle,
  IN UINT16      Port,
  IN UINT16      PortMultiplier
  );
#endif  // _STORAGE_SERVICE_LIB_H_
