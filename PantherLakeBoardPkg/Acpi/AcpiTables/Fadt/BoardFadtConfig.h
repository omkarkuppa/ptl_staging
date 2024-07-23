/** @file
  This file describes the contents of the ACPI Fixed ACPI Description Table
  (FADT).  All changes to the FADT contents should be done in this file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _FADT_H_
#define _FADT_H_

//
// Statements that include other files
//

#include <IndustryStandard/Acpi.h>

#include "Register/PmcRegs.h"
#include "Register/TcoRegs.h"

#define EFI_ACPI_S4_BIOS_REQ          0xF2
#define EFI_ACPI_DUTY_OFFSET          0x01
#define EFI_ACPI_DUTY_WIDTH           0x03

//
// IA-PC Boot Architecture Flags, see latest definition in ACPI specification and related header file
// We should not use 8042 flag for legacy free platform, or XP reset fail. No impact to mobile.
//
#define EFI_ACPI_IAPC_BOOT_ARCH (EFI_ACPI_6_5_LEGACY_DEVICES)

//
// Fixed Feature Flags, see latest definition in ACPI specification and related header file
//
#define EFI_ACPI_FIXED_FEATURE_FLAGS (\
  EFI_ACPI_6_5_USE_PLATFORM_CLOCK | \
  EFI_ACPI_6_5_WBINVD | \
  EFI_ACPI_6_5_DCK_CAP | \
  EFI_ACPI_6_5_FIX_RTC | \
  EFI_ACPI_6_5_RTC_S4 | \
  EFI_ACPI_6_5_SLP_BUTTON | \
  EFI_ACPI_6_5_PROC_C1 | \
  EFI_ACPI_6_5_RESET_REG_SUP  \
  )

//
// PM1A Event Register Block Generic Address Information
//
#define EFI_ACPI_PM1A_EVT_BLK_ADDRESS           FixedPcdGet16 (PcdAcpiBaseAddress)

//
// PM1B Event Register Block Generic Address Information
//
#define EFI_ACPI_PM1B_EVT_BLK_ADDRESS           0x0
//
// PM1A Control Register Block Generic Address Information
//
#define EFI_ACPI_PM1A_CNT_BLK_ADDRESS           (EFI_ACPI_PM1A_EVT_BLK_ADDRESS + 0x04)

//
// PM1B Control Register Block Generic Address Information
//
#define EFI_ACPI_PM1B_CNT_BLK_ADDRESS           0x0

//
// PM2 Control Register Block Generic Address Information
//
#define EFI_ACPI_PM2_CNT_BLK_ADDRESS            (EFI_ACPI_PM1A_EVT_BLK_ADDRESS + 0x50)

//
// Power Management Timer Control Register Block Generic Address
// Information
//
#define EFI_ACPI_PM_TMR_BLK_ADDRESS             (EFI_ACPI_PM1A_EVT_BLK_ADDRESS + 0x08)

//
// General Purpose Event 0 Register Block Generic Address
// Information
//
#define EFI_ACPI_GPE0_BLK_BIT_WIDTH             0x100
#define EFI_ACPI_GPE0_BLK_ADDRESS               (EFI_ACPI_PM1A_EVT_BLK_ADDRESS + R_ACPI_IO_GPE0_STS_31_0)

//
// General Purpose Event 1 Register Block Generic Address
// Information
//
#define EFI_ACPI_GPE1_BASE                      (EFI_ACPI_GPE0_BLK_BIT_WIDTH / 2)
#define EFI_ACPI_GPE1_BLK_BIT_WIDTH             0xC0
#define EFI_ACPI_GPE1_BLK_ADDRESS               (EFI_ACPI_PM1A_EVT_BLK_ADDRESS + R_ACPI_IO_GPE1_STS_31_0)

//
// Number of bytes decoded by GPE0 block
//
#define EFI_ACPI_GPE0_BLK_LEN (EFI_ACPI_GPE0_BLK_BIT_WIDTH / 8)

//
// Number of bytes decoded by GPE1 block
//
#define EFI_ACPI_GPE1_BLK_LEN (EFI_ACPI_GPE1_BLK_BIT_WIDTH / 8)

#endif
