/** @file
  Intel platform storage erase notify Protocol.

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

@par Specification
**/

#ifndef _STORAGE_SANITIZE_NOTIFY_PROTOCOL_H_
#define _STORAGE_SANITIZE_NOTIFY_PROTOCOL_H_

#include <StorageSanitizeDefinitions.h>

typedef struct _STORAGE_ERASE_NOTIFY_PROTOCOL  STORAGE_ERASE_NOTIFY_PROTOCOL;

#define STORAGE_ERASE_NOTIFY_PROTOCOL_REVISION  1

#define ERASE_RESULT_DISABLE_SECURITY_FAILED    BIT0
#define ERASE_RESULT_ERASE_OPERATION_FAILED     BIT1
#define ERASE_RESULT_VERIFICATION_FAILED        BIT2
#define ERASE_RESULT_SUCCESS                    BIT7

//
// Data structure of device information
//
typedef struct {
  UINT32   NameSize;  // Size in byte
  UINT32   SnSize;
  CHAR16   Name[MN_MAX_LEN + 1];
  CHAR8    Sn[SN_MAX_LEN + 1];
  CHAR8    Password[PASSWORD_MAX_LENGTH];
  UINTN    Result;
} ERASE_DEVICE_INFO;

typedef struct {
  UINT32            TotalSize;
  UINT8             Selected;  //Selected to erase
  //ERASE_DEVICE_INFO *DeviceList;
} ERASE_DEVICE_LIST;

//
// Data structure of erase report
//
typedef struct {
  UINT32   DataSize;  // Size in byte
  VOID     *Data;
} ERASE_DOCUMENTATION;

//
// Erase operation configuration
//
typedef union {
  struct {
    UINT32  Verification : 1;
    UINT32  ReportToUsb  : 1;
    UINT32  Rsvd         : 30;
  } b;
  UINT32  Value;
} ERASE_CONFIGURATION;

/**
  Callback function for storage erase.

  @param[in] EraseStatus  Status return for erase
  @param[in] ReportNum    Conunt of the reports
  @param[in] Report       Pointer to an array of report
  @param[in] DeviceList   Pointer to device list which has erased
                          It includes name, serial number and result of erased device

  @retval EFI_SUCCESS     Storage erase is successful.
  @retval Others          failed to erase all devices.
**/
typedef
VOID
(EFIAPI *STORAGE_ERASE_COMPLETE) (
  IN EFI_STATUS            EraseStatus,
  IN UINT8                 ReportNum,
  IN ERASE_DOCUMENTATION   *Report,
  IN ERASE_DEVICE_LIST     *DeviceList
  );

/**
  Function for storage erase.

  @param[in] Configuration     Configuration of erase operation
  @param[in] DeviceList        The Device list which is not selected for Erasing
  @param[in] CompleteFunction  Callback function when erase completed.

  @retval EFI_SUCCESS     Storage erase is successful.
  @retval Others          failed to erase all devices.
**/
typedef
EFI_STATUS
(EFIAPI *STORAGE_ERASE) (
  IN  ERASE_CONFIGURATION    Configuration,
  IN  ERASE_DEVICE_LIST      *DeviceList,
  IN  STORAGE_ERASE_COMPLETE CompleteFunction
  );

/**
  Function to Get Device List.

  @param[out] DeviceList        The Device list which is not selected for Erasing

  @retval EFI_SUCCESS           Storage erase is successful.
  @retval Others                failed to erase all devices.
**/
typedef
EFI_STATUS
(EFIAPI *GET_DEVICE) (
  OUT VOID      **DeviceList
  );

///
/// The Storage erase notify Protocol provides interface to notify
/// platform storage erase driver to start storage erase operation.
///
typedef struct _STORAGE_ERASE_NOTIFY_PROTOCOL {
  UINT8           Revision;
  STORAGE_ERASE   StorageErase;
  GET_DEVICE      GetDevice;
} STORAGE_ERASE_NOTIFY_PROTOCOL;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID  gStorageSanitizeNotifyProtocolGuid;

#endif
