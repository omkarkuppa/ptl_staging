/**@file
  Nvme Storage Erase Driver.

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

#ifndef _NVME_STORAGE_ERASE_H_
#define _NVME_STORAGE_ERASE_H_

#include <PiDxe.h>
#include <IndustryStandard/Nvme.h>
#include <IndustryStandard/Atapi.h>

#include <Protocol/RstEraseProtocol.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/AtaPassThru.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/BlockIo.h>

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/StorageServiceLib.h>
#include <Library/AhciCommandLib.h>
#include <Library/VmdInfoLib.h>

//
// RST_SATA is a linked list to store child device information.
//
#define RST_SATA_SIGNATURE     SIGNATURE_32 ('r', 's', 'r', 'p')
#define RST_SATA_FROM_LINK(a)  CR(a, RST_SATA, Link, RST_SATA_SIGNATURE)

typedef struct {
  UINTN                     Signature;
  LIST_ENTRY                Link;
  UINT16                    Port;               // real port which used by AtaPassthru protocol
  UINT16                    PortMultiplier;
  ATA_IDENTIFY_DATA         *IdentifyData;
  EFI_HANDLE                Handle;             // device handle exist only when it support opal or not in RAID volume
  EFI_HANDLE                ParentHandle;       // handle of device's parent, used for device in raid.
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINT8                     Method;
  EFI_STATUS                Result;
} RST_SATA;

//
// RST_NVME is a linked list to store child device information.
//
#define RST_NVME_SIGNATURE     SIGNATURE_32 ('r', 'n', 'r', 'p')
#define RST_NVME_FROM_LINK(a)  CR(a, RST_NVME, Link, RST_NVME_SIGNATURE)

typedef struct {
  UINTN                       Signature;
  LIST_ENTRY                  Link;
  UINT32                      NameSpaceId;       // real namespace ID which used by NvmePassthru protocol
  NVME_ADMIN_CONTROLLER_DATA  *ControllerData;
  EFI_HANDLE                  Handle;            // device handle exist only when it support OPAL or not in RAID volume (not support OPAL)
  EFI_HANDLE                  ParentHandle;      // handle of device's parent, used for device in raid.
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath;
  UINT8                       Method;
  EFI_STATUS                  Result;
} RST_NVME;

#define RST_ERASE_DRIVER_NVME_SIGNATURE SIGNATURE_32 ('r', 'e', 'n', 'r')
#define PRIVATE_FROM_RST_NVME_ERASE(a)  CR (a, RST_ERASE_NVME_PRIVATE, EraseProtocol, RST_ERASE_DRIVER_NVME_SIGNATURE)

typedef struct {
  UINT32                                Signature;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmePassthru;
  LIST_ENTRY                            DeviceList;
  RST_ERASE_PROTOCOL                    EraseProtocol;
} RST_ERASE_NVME_PRIVATE;

#define RST_ERASE_DRIVER_SATA_SIGNATURE SIGNATURE_32 ('r', 'e', 's', 'r')
#define PRIVATE_FROM_RST_SATA_ERASE(a)  CR (a, RST_ERASE_SATA_PRIVATE, EraseProtocol, RST_ERASE_DRIVER_SATA_SIGNATURE)

typedef struct {
  UINT32                                Signature;
  EFI_ATA_PASS_THRU_PROTOCOL            *AtaPassthru;
  LIST_ENTRY                            DeviceList;
  RST_ERASE_PROTOCOL                    EraseProtocol;
} RST_ERASE_SATA_PRIVATE;

/**
  Check if the given device under VMD controlled

  @param[in] DevicePath  Device path.

  @retval TRUE   Device is under VMD controlled.
  @retval FALSE  Device is not under VMD controlled.

**/
BOOLEAN
IsVmdControlled (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  );

/**
  Update Nvme device list with namespace id, device path, Handle and so on.

  @param[in] Private    Pointer to the device private data.

  @retval EFI_SUCCESS   Successful

**/
EFI_STATUS
UpdateNvmeDiskInfo (
  IN RST_ERASE_NVME_PRIVATE  *Private
  );

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
  );

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
  );

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
  IN  RST_ERASE_PROTOCOL        *This,
  OUT UINT32                    *NumOfDevice,
  OUT RST_DEVICE                **DeviceList
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
EFI_STATUS
EFIAPI
GetNvmeEraseResult (
  IN  RST_ERASE_PROTOCOL   *This,
  IN  COMMAND_ID           DeviceId,
  OUT VOID                 **EraseResult
  );

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
  );

/**
  Update Sata device list with port id, device path, Handle and so on.

  @param[in] Private    Pointer to the device private data.

  @retval EFI_SUCCESS   Successful

**/
EFI_STATUS
UpdateSataDiskInfo (
  IN RST_ERASE_SATA_PRIVATE  *Private
  );

/**
  Enumerate Sata device

  @param[in] Private   Pointer to the device private data.
  @param[in] Handle    Handle of the devcie' parent.

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some error

**/
EFI_STATUS
EnumerateSataDevices (
  IN RST_ERASE_SATA_PRIVATE  *Private,
  IN EFI_HANDLE              Handle
  );

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
  );

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
  );

/**
  Return the result of erase operation on the device.

  @param[in] This          Indicates a pointer to the calling context.
  @param[in] DeviceId      Port id of SATA.
  @param[out] EraseResult  A pointer to the data of erase result.

  @retval EFI_SUCCESS             Get result of erase operation successfully.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetSataEraseResult (
  IN  RST_ERASE_PROTOCOL  *This,
  IN  COMMAND_ID          DeviceId,
  OUT VOID                **EraseResult
  );

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
  );
#endif //_NVME_STORAGE_ERASE_H_
