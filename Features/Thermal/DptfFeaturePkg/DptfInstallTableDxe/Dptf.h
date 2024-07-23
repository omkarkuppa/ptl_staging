/** @file
  Header file for the Intel(R) Dynamic Tuning Technology driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _DPTF_H_
#define _DPTF_H_

#include <PiDxe.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Acpi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/FirmwareVolume2.h>

#ifndef DPTF_BASE_ADDRESS
#define DPTF_BASE_ADDRESS   0xFED98000
#endif
#ifndef DPTF_LENGTH
#define DPTF_LENGTH         0x8000
#endif

///
/// AML parsing definitions
///
#define AML_METHOD_OP       0x14

EFI_STATUS
EFIAPI
InitializeDptf (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  );

EFI_STATUS
EFIAPI
LoadAcpiTables(
  VOID
  );

#define ACPI_OBJECT_DISABLE 1

#endif
