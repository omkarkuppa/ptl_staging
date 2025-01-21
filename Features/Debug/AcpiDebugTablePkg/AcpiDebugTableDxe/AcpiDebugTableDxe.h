/** @file
  This is an definition of the ACPI debug table

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _ACPI_DEBUG_TABLE_DXE_H_
#define _ACPI_DEBUG_TABLE_DXE_H_

//
// Statements that include other header files
//
#include <Base.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Protocol/AcpiTable.h>

#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/DebugPortTable.h>
#include <IndustryStandard/DebugPort2Table.h>

//
// ACPI DBG2 - Debug Device Name Space String
//
#define ACPI_DBG2_DEFAULT_NAME_SPACE            "."
#define ACPI_DBG2_SERIALIO_UART0_NAME_SPACE     "\\_SB.UAH0"
#define ACPI_DBG2_SERIALIO_UART1_NAME_SPACE     "\\_SB.UAH1"
#define ACPI_DBG2_SERIALIO_UART2_NAME_SPACE     "\\_SB.UAH2"

#define SERIAL_IO_UART_MEM_SIZE                        0x2000
#define EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_INTEL_LPSS   0x0014

#endif
