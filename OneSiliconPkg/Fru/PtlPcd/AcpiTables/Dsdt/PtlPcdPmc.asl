/** @file
  Intel ACPI Reference Code for MTL SOC Power Management Control

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

#include <Register/TcoRegs.h>

Scope (\) {

  //
  // Define PCH ACPIBASE I/O as an ACPI operating region.  The base address
  // can be found in Device 31, Function 2, Offset 40h.
  //
  OperationRegion (PMIO, SystemIo, PMBS, 0x80)
  Field (PMIO, ByteAcc, NoLock, Preserve) {
          ,  8,
      PBSS,  1,       // Power Button Status
      Offset (R_ACPI_IO_GPE1_STS_31_0),       // General Purpose Event 1 Status [31:0]
      S210, 32,   // Tier 2 - GPE1_STS bits 0_31
      S211, 32,   // Tier 2 - GPE1_STS bits 32_63
      S212, 32,   // Tier 2 - GPE1_STS bits 64_95
      E210, 32,   // Tier 2 - GPE1_EN  bits 0_31
      E211, 32,   // Tier 2 - GPE1_EN  bits 32_63
      E212, 32,   // Tier 2 - GPE1_EN  bits 64_95
    Offset (R_ACPI_IO_GPE_CTRL),            // 0x40, General Purpose Event Control
          ,  17,
      GPEC,  1,       // Software GPE Control
    Offset (R_ACPI_IO_GPE0_STS_31_0),
      S100, 32,   // Tier 1 - GPE0_STS bits 0_31   // 0x60
      S101, 32,   // Tier 1 - GPE0_STS bits 32_63  // 0x64
      S102, 32,   // Tier 1 - GPE0_STS bits 64_95  // 0x68
    Offset (R_ACPI_IO_GPE0_STS_127_96),     // 0x6C, General Purpose Event 0 Status [127:96]
          , 19,
    Offset (R_ACPI_IO_GPE0_EN_127_96),      // 0x7C, General Purpose Event 0 Enable [127:96]
          ,  18,
      WAEN,  1,       // WADT EN
  }

  //
  // Is MODPHY power gating supported
  // Checks if MODPHY power gating is supported on given SKU
  //
  // @retval 0 MODPHY power gating is not supported
  // @retval 1 MODPHY power gating is supported
  //
  Method (IMPS) {
    Return (1)
  }

  Include ("Pmc.asl")
}