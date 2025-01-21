/**@file
  Definition of Nvme erase block protocol

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

#ifndef __RST_ERASE_PROTOCOL_H__
#define __RST_ERASE_PROTOCOL_H__

#include <StorageSanitizeDefinitions.h>
#include <Library/EraseServiceLib.h>

#define RST_SATA_ERASE_PROTOCOL_GUID \
  { \
    0xfaf46ead, 0x315c, 0x4d6c, { 0xaf, 0xbf, 0xad, 0xd, 0xd3, 0x45, 0x34, 0x9d } \
  }

#define RST_NVME_ERASE_PROTOCOL_GUID \
  { \
    0x3b4584b6, 0x9ebc, 0x48ba, { 0xba, 0x40, 0x8, 0x55, 0x63, 0xa5, 0xd2, 0x48 } \
  }

typedef struct _RST_ERASE_PROTOCOL RST_ERASE_PROTOCOL;

#define RST_ERASE_PROTOCOL_REVISION ((2<<16) | (60))

//
// Data structure used to store
//
typedef struct {
  COMMAND_ID                Id;
  EFI_HANDLE                Handle;                 ///< Device handle
  EFI_HANDLE                ParentHandle;           ///< Parent Device handle
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINT32                    VidDid;
  CHAR8                     Manufacturer[MANUFACURER_MAX_LEN + 1];
  CHAR8                     ModelName[MN_MAX_LEN + 1];
  CHAR8                     SerialNumber[SN_MAX_LEN + 1];
  UINT8                     Method;
  EFI_STATUS                Result;
} RST_DEVICE;

/**
  Erase a specified number of device blocks.

  @param[in] This       Indicates a pointer to the calling context.
  @param[in] DevId      Namespace id of NVMe, Port id of SATA.
  @param[in] Lba        The starting logical block address to be erased.
                        The caller is responsible for erasing only legitimate locations.
  @param[in] Size       The size in bytes to be erased. This must be a multiple
                        of the physical block size of the
                                  device.
  @param[in] EraseType  Erase type to be used to erase data on device.

  @retval EFI_SUCCESS             The erase request was queued if Event is not
                                  NULL. The data was erased correctly to the
                                  device if the Event is NULL.to the device.
  @retval EFI_UNSUPPORTED         device not support this command.
  @retval EFI_INVALID_PARAMETER   The erase request contains LBAs that are not
                                  valid.

**/
typedef
EFI_STATUS
(EFIAPI *RST_ERASE) (
  IN RST_ERASE_PROTOCOL        *This,
  IN COMMAND_ID                DevId,
  IN EFI_LBA                   Lba,
  IN UINTN                     Size,
  IN UINT8                     EraseType
  );

/**
  Return the device list in VMD mode.

  @param[in] This         Indicates a pointer to the calling context.
  @param[out] NumOfDevice Amount of NVMe/SATA device in VMD mode.
  @param[out] DeviceList  A pointer to the list of device.

  @retval EFI_SUCCESS             Get device successfully.
  @retval EFI_NOT_FOUND           There is no device can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
typedef
EFI_STATUS
(EFIAPI *GET_RST_DEVICE_LIST) (
  IN  RST_ERASE_PROTOCOL  *This,
  OUT UINT32              *NumOfDevice,
  OUT RST_DEVICE          **DeviceList
  );

/**
  Return the result of erase operation on the device.

  @param[in] This          Indicates a pointer to the calling context.
  @param[in] DeviceId      Namespace id.
  @param[out] EraseResult  A pointer to the data of erase result.

  @retval EFI_SUCCESS             Get result of erase operation successfully.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
typedef
EFI_STATUS
(EFIAPI *GET_RST_ERASE_RESULT) (
  IN  RST_ERASE_PROTOCOL  *This,
  IN  COMMAND_ID          DeviceId,
  OUT VOID                **EraseResult
  );

/**
  Get the time.

  @param[in]  This         Indicates a pointer to the calling context.
  @param[in]  DeviceId     Namespace id of NVMe, Port id of SATA.
  @param[in]  EraseType    Erase type will be used to erase device block.

  @retval UINT64           Return time
**/
typedef
UINT64
(EFIAPI *GET_RST_ESTIMATE_TIME) (
  IN  RST_ERASE_PROTOCOL  *This,
  IN  COMMAND_ID          DeviceId,
  IN  UINT8               EraseType
  );

///
/// The Platform Erase Block Protocol provides the ability for a device to expose
/// erase functionality. This optional protocol is installed on the same handle
/// as the EFI_BLOCK_IO_PROTOCOL or EFI_BLOCK_IO2_PROTOCOL.
///
struct _RST_ERASE_PROTOCOL {
  //
  // The revision to which the EFI_ERASE_BLOCK_PROTOCOL adheres. All future
  // revisions must be backwards compatible. If a future version is not
  // backwards compatible, it is not the same GUID.
  //
  UINT64                  Revision;
  //
  // Returns the erase length granularity as a number of logical blocks. A
  // value of 1 means the erase granularity is one logical block.
  //
  UINT32                  EraseLengthGranularity;
  RST_ERASE               Erase;
  GET_RST_DEVICE_LIST     GetDeviceList;
  GET_RST_ERASE_RESULT    GetEraseResult;
  GET_RST_ESTIMATE_TIME   GetEstimateTime;
};

extern EFI_GUID gRstSataEraseProtocolGuid;
extern EFI_GUID gRstNvmeEraseProtocolGuid;

#endif
