/** @file
  Dxe Telemetry Acpi Lib implementation.

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
#ifndef _DXE_TELEMETRY_ACPI_LIB_H_
#define _DXE_TELEMETRY_ACPI_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Guid/Acpi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <TelemetryHob.h>
#include <Library/HobLib.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Guid/FirmwarePerformance.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <TelemetryCommon.h>
#include <Protocol/DevicePath.h>
#include <Library/DevicePathLib.h>
#include <Protocol/FirmwareManagement.h>
#include <Include/Guid/CapsuleReport.h>
#include <Library/PhatAcpiLib.h>

#define CAPSULELAST                           L"CapsuleLast"
#define CAPSULE_VAR_NAME                      L"Capsule"


extern GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID  *mDriverHealthSupportList[];

#endif
