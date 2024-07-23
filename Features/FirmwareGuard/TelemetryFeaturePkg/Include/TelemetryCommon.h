/** @file
  Telemetry Lib Common Header.

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
#ifndef _FIRMWARE_GUARD_TELEMETRY_COMMON_H_
#define _FIRMWARE_GUARD_TELEMETRY_COMMON_H_

#include <Protocol/AdapterInformation.h>
#include <Protocol/TelemetryAdapterInformation.h>

#define MOTHERBOARD_HEALTH_DATA_NAME      L"MotherBoardHealth"
#define VARNAME_LASTUPDATETIME            L"LastUpdateTime"
#define TELEMETRY_FW_VERSION_ME           L"TelemetryFwVersionME"
#define TELEMETRY_FW_VERSION_ESE          L"TelemetryFwVersionESE"

///
/// Variable defined for PHAT implementation.
/// This Variable is produced by PHAT DXE module.
///
typedef struct {
  UINT64                 MeanBootTime;               // Average time in nanoseconds elapsed just prior to loading the OS boot loader.
  UINT16                 NumberOfBootsSinceUpdate;   // Counter since last firmware update.
} MOTHER_BOARD_HEALTH_VARIABLE;

///
/// ACPI Firmware Version Structure
///
typedef struct {
  UINT16                       MajorVersion;
  UINT16                       MinorVersion;
  UINT16                       Revision;
  UINT16                       BuildNumber;
} INTEL_ACPI_FIRMWARE_VERSION;

//
// For firmware version shift
//
#define VERSION_MAJOR_VERSION_SHIFT 48
#define VERSION_MINOR_VERSION_SHIFT 32
#define VERSION_REVISION_SHIFT      16
#define VERSION_BUILD_NUM_SHIFT     0

//
// For RC version,
// Always set SKU bit fields to Zero
#define RC_BITFIELD_ZERO_SHIFT 24

///
/// This value depicts the format and contents
/// of the platform telemetry record.
///
typedef enum {
  FirmwareVersionDataRecord = 0,
  FirmwareHealthDataRecord  = 1
} FIRMWARE_TELEMETRY_RECORD_TYPE;

///
/// This value depicts the format and contents
/// of the platform telemetry record.
///
typedef enum {
  TelemetryUnhealthy = 0,
  TelemetryHealthy,
  TelemetryUnknown,
  TelemetryAdvisory,
} TELEMETRY_HEALTH_DRIVER_STATUS;

///
/// structure Revision (as defined in Telemetry Gen 2.0 spec.)
///
#define TELEMETRY_HOB_STRUCTURE_REVISION   0x02
#define TELEMETRY_ACPI_HEADER_REVISION     0x01
#define TELEMETRY_ACPI_STRUCTURE_REVISION  0x01
#define TELEMETRY_FVI_DATA_RECORD_REVISION 0x01

//
// For firmware version binary
//
#define EFI_ACPI_OEM_REVISION     0x00000005
#define EFI_ACPI_CREATOR_ID       SIGNATURE_32 ('M','S','F','T')
#define EFI_ACPI_CREATOR_REVISION 0x0100000D

#define DEFAULT_VARIABLE_VALUE          0x55AA55AA

#endif
