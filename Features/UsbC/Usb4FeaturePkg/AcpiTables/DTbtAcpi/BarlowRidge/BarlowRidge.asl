/** @file
  BarlowRidge ACPI devices and methods

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

@par Specification
**/


#include "DTbtSwcmWrapper.h"

External (DTBT_US_PORT.DTID, IntObj)
External (DTBT_US_PORT, DeviceObj)
External (\DTRT, FieldUnitObj)

Scope(DTBT_US_PORT)
{
  //
  // PCIe downstream port 0 - connecting NHI device in downstream
  //
  Device (TBDH) {
    Name (_ADR, 0x00000000)
    //
    // Include different NHI implementation according to TBT ID value
    //
    If (CondRefOf (DTBT_US_PORT.DTID)) {
      if (LEqual (DTBT_US_PORT.DTID, 0)) {
        Include ("DTbt0Nhi.asl")
      } ElseIf (LEqual (DTBT_US_PORT.DTID, 1)) {
        Include ("DTbt1Nhi.asl")
      } Else {
        ADBG (Concatenate("Unsupported DTBT UID", DTBT_US_PORT.DTID))
      }
    }
  }

  //
  // PCIe downstream port 1 - PCIE port for PCIE tunneling of CIO port 0
  //
  Device (TBP1) {
    Name (_ADR, 0x00010000)
    Include ("DTbtPcie0.asl")
  }

  //
  // PCIe downstream port 2 - connecting XHCI controller in downstream
  //
  Device (TBDU) {
    Name (_ADR, 0x00020000)
    Include ("DTbtXhci.asl")
  }

  //
  // PCIe downstream port 3 - PCIE port for PCIE tunneling of CIO port 1
  //
  Device(TBP3) {
    Name (_ADR, 0x00030000)
    Include ("DTbtPcie1.asl")
  }

  Method (_RMV, 0, NotSerialized)
  {
    Return (Zero)
  }
}
