/**@file
  ACPI DSDT table for ISH

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
Scope (\_SB.PC00) {
  //
  // Integrated Sensor Hub (PCI Mode)
  //
  Device (ISHD) {
    Name (_ADR, 0x00120000)
  } // Device (ISHD)
}