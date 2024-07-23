/** @file
  Intel ACPI Sample Code for GMIO method Implementation

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

@par Specification
**/

External (\_SB.PC00.PC2M, MethodObj)

//
// PCIe endpoint PCI config space to MMIO address
//
// Arg0 - Bus number of a root port.
// Arg1 - _ADR of the target root port controller.
// Arg2 - _ADR of the target endpoint device.
//
Method (GMIO, 3, Serialized) { // Get Memory Mapped I/O address
  // Bus number will be grabbed from the root port. (under the scope where this method exists)
  // Root port PCI config space definition
  If (CondRefOf (\_SB.PC00.PC2M)) {
    OperationRegion (PXCS, SystemMemory, \_SB.PC00.PC2M (Arg1, Arg0), 0x20) // PCI eXpress Configuration Space
    Field (PXCS, AnyAcc, NoLock, Preserve) {
      Offset (0x18), // Bus Number register
      PBUS, 8,       // Primary Bus Number
      SBUS, 8,       // Subordinate Bus Number
    }
  }
  If (CondRefOf (\_SB.PC00.GPCB)) {
    Store (\_SB.PC00.GPCB (), Local0) // MMIO Base address
  }
  Add (Local0, ShiftRight (And (Arg2, 0x001F0000), 1), Local0) // Device no
  Add (Local0, ShiftLeft (And (Arg2, 0x00000007), 12), Local0) // Function no
  Add (Local0, ShiftLeft (SBUS, 20), Local0)                   // Bus no

  Return (Local0)
}