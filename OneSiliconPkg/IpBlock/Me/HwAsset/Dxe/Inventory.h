/** @file
  Include file for PCI and Media device inventory.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2005 Intel Corporation.

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
#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#include <Library/DxeMeLib.h>

#define MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH  20
#define MEDIA_DEVICE_VERSION_NO_MAX_LENGTH 8
#define MEDIA_DEVICE_MODEL_NO_MAX_LENGTH   40

#define HWA_MEDIA_IN_ATA                 0x0000
#define HWA_MEDIA_IN_ATAPI               0x0001
#define HWA_MEDIA_IN_SATA                0x0002
#define HWA_MEDIA_IN_PCIE                0x0003

#define HWA_MEDIA_DT_HDD                 0x0000
#define HWA_MEDIA_DT_CDROM               0x0001
#define HWA_MEDIA_DT_DVD                 0x0002
#define HWA_MEDIA_DT_FDD                 0x0003
#define HWA_MEDIA_DT_MO                  0x0004

#define HWA_FRU_TYPE_PCI                  1

#pragma pack(1)

typedef struct {
  UINT8  SmbiosType;
  UINT8  Length;
  UINT16 SmbiosHandle;
  UINT32 FruType;
  UINT16 VendorId;
  UINT16 DeviceId;
  UINT16 RevisionId;
  UINT16 SubsystemVendorId;
  UINT16 SubsystemDeviceId;
  UINT32 ClassCode;
  UINT16 BusDevFunc;
} HWA_PCI_FRU_DATA;

typedef struct {
  UINT16            TableByteCount;
  UINT16            StructureCount;
  HWA_PCI_FRU_DATA  Data[0];
} HWA_FRU_TABLE;

typedef struct {
  UINT16 StructSize;
  UINT8  Interface;
  UINT8  DevType;
  UINT8  Rsvd[40];
  UINT8  SerialNo[MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH];
  UINT8  VersionNo[MEDIA_DEVICE_VERSION_NO_MAX_LENGTH];
  UINT8  ModelNo[MEDIA_DEVICE_MODEL_NO_MAX_LENGTH];
  UINT64 MaxMediaSize;
  UINT16 SupportedCmdSets[3];
} HWA_FRU_MEDIA_DEVICES;

typedef struct {
  UINT8                  SmbiosType;
  UINT8                  Length;
  UINT16                 SmbiosHandle;
  HWA_FRU_MEDIA_DEVICES  MediaData;
} HWA_MEDIA_ENTRY;

typedef struct {
  UINT16          TableByteCount;
  UINT16          StructureCount;
  HWA_MEDIA_ENTRY Data[0];
} HWA_MEDIA_TABLE;

#pragma pack()

///
/// NVME definition
///
#define NVME_GENERIC_TIMEOUT            EFI_TIMER_PERIOD_SECONDS (5)
#define NVME_CONTROLLER_ID              0

#define NVME_ALL_NAMESPACES             0xFFFFFFFF

/**
  Constructs each of the lower level asset tables

  @param[in]  AssetTablesData           Buffer of all Asset tables to send to FW
  @param[in]  IsMediaTablePushRequested Determines if CSME has requested Media Table in this boot

  @return     TableSize                 Total Size of the HWA Table
**/
UINT16
BuildHwaTable (
  IN  HWA_TABLE           *AssetTableData,
  IN  BOOLEAN             IsMediaTablePushRequested
  );

#endif // _INVENTORY_H_
