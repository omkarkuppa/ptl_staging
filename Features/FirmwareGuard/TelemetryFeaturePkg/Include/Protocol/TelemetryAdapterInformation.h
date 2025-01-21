/** @file
  EFI Adapter Information Protocol definition for Telemetry feature enabling.
  The EFI Adapter Information Protocol is used to dynamically and quickly discover
  or set device information for an adapter.

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

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
  This Protocol is introduced in UEFI Specification 2.4
**/

#ifndef __EFI_TELEMETRY_ADAPTER_INFORMATION_PROTOCOL_H__
#define __EFI_TELEMETRY_ADAPTER_INFORMATION_PROTOCOL_H__

#include <PlatformHealthAssesmentTable.h>

#define DEFAULT_FVI_TELEMETRY_VERSION       0X000000FFFFFFFFFF
#define FVI_INTEL_SIGNATURE                 SIGNATURE_32 ('I', 'N', 'T', 'C')

///
/// CSME Health Structure Revision
///
#define CSME_AIP_HEADER_REVISION  0x01
#define CSME_AIP_DATA_REVISION    0x02 //Due to the change ManufacturingMode is replaced by SpiProtectionMode since CSME Spec 15.0

#pragma pack(1)
typedef struct {
  UINT16           Length;      // length of Header + attached health state record
                                // 30 + device health record size
  UINT8            Revision;    // 0x01-Revision of this struct
  UINT8            Healthy;     // is the device healthy?
                                // 0 = healthy, 1 = not healthy
                                // 2 = unknown health (treated as healthy)
  EFI_GUID         Signature;   // The GUIDed record type that follows
  EFI_DEVICE_PATH  *DevicePath; // Device Path of the component
} EFI_AIP_HEALTH_HEADER;


typedef struct {
  EFI_AIP_HEALTH_HEADER  Header;
  UINT8                  HealthEntry[1];
} EFI_AIP_HEALTH_RECORD;

typedef struct {
  EFI_TIME               LastUpdateTime;
  EFI_GUID               EsrtId;
} EFI_LAST_UPDATE_DATA;

typedef struct {
  EFI_AIP_HEALTH_HEADER  Header;
  UINT8                  Revision;
  UINT64                 MeanBootTime;
  UINT64                 CurrentBootTime;
  UINT16                 NumberOfBootsSinceUpdate;
  EFI_LAST_UPDATE_DATA   LastUpdateTime[1];
} EFI_AIP_MOTHERBOARD_HEALTH_STATE;

typedef struct {
  UINT64                        Revision;    // 1 - Revision of the record
  UINT64                        RecordCount; // Number of Record entries (1-n)
  EFI_ACPI_6_5_PHAT_VERSION_ELEMENT         Record[0];   // Array of PHAT Version Elements.
                                             // First entry is the original producer of
                                             // the component,and if there's a subsequent
                                             // entry,that means a second agent modified
                                             // the original component in some way,
                                             // whichever the last entry is, that's the
                                             // currently running instance of the component.
                                             // This allows for IHV/IBV/OEM/others
                                             // to establish a chain of data records
                                             // associated with a given component.
} EFI_AIP_TELEMETRY_VERSION_RECORD;

typedef struct {
  UINT16                PlatformRecordType;  // Firmware Version Data Record.
  UINT16                RecordLength;        // 12+28*RecordCount - This value depicts.
                                             // The length of the version data record, in bytes.
  UINT8                 Revision;            // Revision of this Firmware Version Data Record, the value is fixed to 1.
  UINT8                 Reserved[3];
  UINT32                RecordCount;         // PHAT Version Element Count.
  EFI_ACPI_6_5_PHAT_VERSION_ELEMENT Records[];           // Array of PHAT Version Elements.
                                             // First entry is the original producer of
                                             // the component,and if there's a subsequent
                                             // entry,that means a second agent modified
                                             // the original component in some way,
                                             // whichever the last entry is, that's the
                                             // currently running instance of the component.
                                             // This allows for IHV/IBV/OEM/others
                                             // to establish a chain of data records
                                             // associated with a given component.
} EFI_COMPONENT_VERSION_DATA;

typedef struct {
  EFI_AIP_HEALTH_HEADER  Header;
  UINT8                  Revision;
  UINT8                  FwInitComplete;
  UINT8                  CurrentState;
  UINT8                  MfsFailure;
  UINT8                  MeOperationMode;
  UINT8                  ErrorCode;
  UINT8                  SpiProtectionMode;
  UINT8                  FpfSocConfigLock;
  UINT8                  FwUpdateInprogress;
} EFI_AIP_CSME_HEALTH_STATE;

typedef enum {
  Reserved,
  ErrorInfo,
  SMARTHealth,
  SlotInfo,
  ChangedNamespaceList,
  CommandsSupported,
  SelfTest,
  TelemetryHost,
  TelemetryController
} TELEMETRY_NVME_LOG_ID;

typedef struct {
  TELEMETRY_NVME_LOG_ID  LogIdentifier; // Type 2 log
  UINT64                 NameSpaceId;   // 0 or xFFFFFFFF is for
                                        // controller, any other
                                        // value is a namespace.
  UINT32                 DataLength;
  UINT8                  LogData[1];    // NVME Spec 1.3 Log Data
} NVME_LOG_DATA;

typedef struct {
  EFI_AIP_HEALTH_HEADER  Header;
  UINT8                  Revision;      // 0x01 - Revision of this structure
  UINT32                 LogCount;
  NVME_LOG_DATA          LogData[1];    // NVME Spec 1.3 Log Data
} EFI_AIP_NVME_HEALTH_STATE;

#pragma pack()

extern EFI_GUID gTelemetryVersionRecord2Guid;

extern EFI_GUID gAdapterInfoCsmeGuid;

extern EFI_GUID gAdapterInfoMrcGuid;

extern EFI_GUID gAdapterInfoNvmeHealthGuid;

extern EFI_GUID gAdapterInfoMotherBoardHealthGuid;

#endif
