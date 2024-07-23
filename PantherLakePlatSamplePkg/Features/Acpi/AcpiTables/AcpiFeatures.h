/** @file
  This is an implementation of the ACPI features driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#ifndef _ACPI_FEATURES_H_
#define _ACPI_FEATURES_H_

//
// Statements that include other header files
//
#include <Base.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/CpuPlatformLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/PlatformNvsArea.h>

#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/WatchdogActionTable.h>
#include <IndustryStandard/DebugPortTable.h>
#include <IndustryStandard/DebugPort2Table.h>
#include <Lpit/Lpit.h>
#include <Library/PmcLib.h>
#include <Register/PmcRegs.h>
#include <Register/TcoRegs.h>
#include <Register/GenerationMsr.h>

//
// ACPI DBG2 - Debug Device Name Space String
//
#define ACPI_DBG2_DEFAULT_NAME_SPACE            "."
#define ACPI_DBG2_SERIALIO_UART0_NAME_SPACE     "\\_SB.UAH0"
#define ACPI_DBG2_SERIALIO_UART1_NAME_SPACE     "\\_SB.UAH1"
#define ACPI_DBG2_SERIALIO_UART2_NAME_SPACE     "\\_SB.UAH2"

//
// Function definitions
//
EFI_STATUS
LocateSupportProtocol (
  IN     EFI_GUID                      *Protocol,
  IN     EFI_GUID                      gEfiAcpiMultiTableStorageGuid,
  OUT    VOID                          **Instance,
  IN     BOOLEAN                       Type
  );

/**
  Entry point of the ACPI Features driver.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The driver installed without error.
  @retval    EFI_ABORTED  The driver encountered an error and could not complete
                          installation of the ACPI tables.
**/
EFI_STATUS
EFIAPI
InstallAcpiPlatformFeatures (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );
#endif
