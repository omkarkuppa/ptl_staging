/** @file
  EFI Telemetry Hob Headers [SAS_v03f].
  This file may be moved in future versions

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

#ifndef __TELEMETRY_HOB_H__
#define __TELEMETRY_HOB_H__

#include <Pi/PiHob.h>

#define FVI_INTEL_SIGNATURE  SIGNATURE_32 ('I', 'N', 'T', 'C')

#pragma pack(1)

///
/// Firmware Version Information HOB Structure
///
typedef struct {
  EFI_HOB_GENERIC_HEADER Header;      // length of Header + health state
                                      // 60 + length of DevicePath
                                      // + device version record size
  UINT8                 Revision;     // 0x02 - Revision of this struct
  UINT8                 Reserved[3];  // To avoid unaligned pointer at
                                      // Signature field in X64 build
  EFI_GUID              Signature;    // The Version Record GUID
                                      // EFI_TELEMETRY_VERSION_RECORD2_GUID
  UINT16                RecordCount;  // The number of records that
                                      // follow.
  EFI_ACPI_6_5_PHAT_VERSION_ELEMENT Records[];    // Records[RecordCount]
} TELEMETRY_FIRMWARE_VERSION_INFO2_HOB;

#pragma pack()

extern EFI_GUID gTelemetryVersionHobGuid;

#endif
