/** @file
  Erase service library header file

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

#ifndef _ERASE_SERVICE_LIB_H_
#define _ERASE_SERVICE_LIB_H_

#include <StorageSanitizeDefinitions.h>
#include <Library/UefiLib.h>

//
// The real port ID or namespace ID to be used as parameter when sending command thru
// NVMe passthru or ATA passthru protocol
//
typedef union {
  //
  // [15:0] - port ID  [31:16] - PortMultiplier
  //
  struct {
    UINT16  Port;
    UINT16  PortMultiplier;
  } Sata;
  //
  // Name space ID
  //
  UINT32  NameSpaceId;
  UINT32  Value;
} COMMAND_ID;

//
// Device type
//
typedef enum {
  NVME,
  UFS,
  SATA,
  NOT_DEFINED
} DEVICE_TYPE;

//
// Erase result list
//
typedef struct {
  UINT32      Id;
  CHAR8       Manufacturer[MANUFACURER_MAX_LEN + 1];
  CHAR8       ModelName[MN_MAX_LEN + 1];
  CHAR8       SerialNumber[SN_MAX_LEN + 1];
  UINT8       Method;
} ERASE_INFO;

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
  );

/**
  Enumerate the NVME / SATA device from RST, return the device list.

  @param[out]  DeviceList      A pointer to RST_DEVICE.
  @param[out]  DeviceNumber    A pointer to the number of entries in DeviceList.

  @retval EFI_SUCCESS             Get RST devices list.
  @retval EFI_NOT_FOUND           There is no device in RST.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.
**/
EFI_STATUS
EFIAPI
HandleRstDevice (
  IN  DEVICE_TYPE       DevType,
  OUT UINT32            *DeviceNumber,
  OUT VOID              **DeviceList
  );

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
  );

/**
  Call service of erase block protocol to perform erase operation

  @param[in] DevHandle  Device handle.
  @param[in] DevType    Device type.
  @param[in] EraseType  Erase type to be used to erase data on the device.
  @param[in] MediaId    The media ID that the erase request is for.
  @param[in] Size       The size in bytes to be erased. This must be a multiple
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
  );

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
  );

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
  );

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
  );

/**
  Return the estimate time.

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
  );

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
  );
#endif  // _ERASE_SERVICE_LIB_H_
