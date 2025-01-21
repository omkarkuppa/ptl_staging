/** @file
  Intel ACPI Sample Code for Connectivity Modules

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

External (\GMIO, MethodObj)
External (PCIE_ROOT_PORT.PBNU, IntObj)
External (VSEC, OpRegionObj)

If (LEqual (CondRefOf (VSEC), 0)) {
  If (LAnd (CondRefOf (\GMIO), CondRefOf (PCIE_ROOT_PORT.PBNU))) {
    If (LAnd (CondRefOf (PCIE_ROOT_PORT._ADR), CondRefOf (PCIE_ROOT_PORT.PXSX._ADR))) {
      OperationRegion (VSEC, SystemMemory, Add (GMIO (PCIE_ROOT_PORT.PBNU, PCIE_ROOT_PORT._ADR, PCIE_ROOT_PORT.PXSX._ADR), WVHO), 0x10)
      Field (VSEC, AnyAcc, NoLock, Preserve)
      {
        Offset (0xC), ,   // VSEC Control Reg
        1,
        BTIE, 1, ,        // BT Interface Enable bit
        1, ,
        1, ,
        1, ,
        1, ,
        1, ,
        1,
        PDRC, 1,          // PLDR Control bit
      }
    }
  }
}