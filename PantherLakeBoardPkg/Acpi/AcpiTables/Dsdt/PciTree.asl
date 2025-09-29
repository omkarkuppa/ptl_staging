/** @file
  ACPI DSDT table

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

External (\PCHP, IntObj)
Include ("IocRootBridges.asl")
Scope (\_SB) {
  Name (SAR0, Package () {})
  Name (PAR0, Package () {
    // PCIe Host Bus (Bus 0)
    // D31
    Package () {0x001FFFFF, 0, 0, 16},
    Package () {0x001FFFFF, 1, 0, 17},
    Package () {0x001FFFFF, 2, 0, 18},
    Package () {0x001FFFFF, 3, 0, 19},
    // D30
    Package () {0x001EFFFF, 0, 0, 16},
    Package () {0x001EFFFF, 1, 0, 17},
    Package () {0x001EFFFF, 2, 0, 18},
    Package () {0x001EFFFF, 3, 0, 19},
    // D28
    Package () {0x001CFFFF, 0, 0, 16},
    Package () {0x001CFFFF, 1, 0, 17},
    Package () {0x001CFFFF, 2, 0, 18},
    Package () {0x001CFFFF, 3, 0, 19},
    // D25
    Package () {0x0019FFFF, 0, 0, 20},
    Package () {0x0019FFFF, 1, 0, 21},
    Package () {0x0019FFFF, 2, 0, 24},
    // D24 (ESE)
    // D23
    Package () {0x0017FFFF, 0, 0, 25},
    // D22
    Package () {0x0016FFFF, 0, 0, 16},
    Package () {0x0016FFFF, 1, 0, 17},
    Package () {0x0016FFFF, 2, 0, 18},
    Package () {0x0016FFFF, 3, 0, 19},
    // D21
    Package () {0x0015FFFF, 0, 0, 27},
    Package () {0x0015FFFF, 1, 0, 28},
    Package () {0x0015FFFF, 2, 0, 33},
    Package () {0x0015FFFF, 3, 0, 30},
    // D20
    Package () {0x0014FFFF, 0, 0, 16},
    Package () {0x0014FFFF, 1, 0, 17},
    Package () {0x0014FFFF, 2, 0, 18},
    Package () {0x0014FFFF, 3, 0, 19},
    // D18
    Package () {0x0012FFFF, 0, 0, 26},
    // D17
    Package () {0x0011FFFF, 0, 0, 31},
    Package () {0x0011FFFF, 1, 0, 32},
    // D16
    Package () {0x0010FFFF, 0, 0, 23},
    Package () {0x0010FFFF, 1, 0, 22},
    // D13
    Package () {0x000DFFFF, 0, 0, 16},
    // D7
    Package () {0x0007FFFF, 0, 0, 16},
    Package () {0x0007FFFF, 1, 0, 17},
    Package () {0x0007FFFF, 2, 0, 18},
    Package () {0x0007FFFF, 3, 0, 19},
    // D6
    Package () {0x0006FFFF, 0, 0, 16},
    Package () {0x0006FFFF, 1, 0, 17},
  })
  Name (HAR0, Package () {
    // PCIe Host Bus (Bus 0)
    // D31
    Package () {0x001FFFFF, 0, 0, 16},
    Package () {0x001FFFFF, 1, 0, 17},
    Package () {0x001FFFFF, 2, 0, 18},
    Package () {0x001FFFFF, 3, 0, 19},
    // D30
    Package () {0x001EFFFF, 0, 0, 16},
    Package () {0x001EFFFF, 1, 0, 17},
    Package () {0x001EFFFF, 2, 0, 18},
    Package () {0x001EFFFF, 3, 0, 19},
    // D28
    Package () {0x001CFFFF, 0, 0, 16},
    Package () {0x001CFFFF, 1, 0, 17},
    Package () {0x001CFFFF, 2, 0, 18},
    Package () {0x001CFFFF, 3, 0, 19},
    // D25
    Package () {0x0019FFFF, 0, 0, 20},
    Package () {0x0019FFFF, 1, 0, 21},
    Package () {0x0019FFFF, 2, 0, 24},
    // D24 (ESE)
    // D23
    // D22
    Package () {0x0016FFFF, 0, 0, 16},
    Package () {0x0016FFFF, 1, 0, 17},
    Package () {0x0016FFFF, 2, 0, 18},
    Package () {0x0016FFFF, 3, 0, 19},
    // D21
    Package () {0x0015FFFF, 0, 0, 27},
    Package () {0x0015FFFF, 1, 0, 28},
    Package () {0x0015FFFF, 2, 0, 33},
    Package () {0x0015FFFF, 3, 0, 30},
    // D20
    Package () {0x0014FFFF, 0, 0, 16},
    Package () {0x0014FFFF, 1, 0, 17},
    Package () {0x0014FFFF, 2, 0, 18},
    Package () {0x0014FFFF, 3, 0, 19},
    // D18
    Package () {0x0012FFFF, 0, 0, 26},
    // D17
    Package () {0x0011FFFF, 0, 0, 31},
    Package () {0x0011FFFF, 1, 0, 32},
    // D16
    Package () {0x0010FFFF, 0, 0, 23},
    Package () {0x0010FFFF, 1, 0, 22},
    // D13
    Package () {0x000DFFFF, 0, 0, 16},
    // D7
    Package () {0x0007FFFF, 0, 0, 16},
    Package () {0x0007FFFF, 1, 0, 17},
    Package () {0x0007FFFF, 2, 0, 18},
    Package () {0x0007FFFF, 3, 0, 19},
    // D6
    Package () {0x0006FFFF, 0, 0, 16},
    Package () {0x0006FFFF, 1, 0, 17},
    Package () {0x0006FFFF, 2, 0, 18},
    Package () {0x0006FFFF, 3, 0, 19},
  })

//---------------------------------------------------------------------------
// Begin PCI tree object scope
//---------------------------------------------------------------------------
  Scope (PC00) {
    Method (_PRT,0) {
      If (LEqual (PCHS, PCHP)) {
        Return (PAR0) // APIC mode buffer for PTL PCD P
      } Else {
        Return (HAR0) // APIC mode buffer for PTL PCD H
      }
    } // end _PRT
  } // end PC00 Bridge "Host Bridge"
} // end _SB scope
