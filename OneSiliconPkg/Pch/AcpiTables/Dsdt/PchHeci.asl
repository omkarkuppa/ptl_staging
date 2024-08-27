/**@file
  ACPI DSDT table for MEI1

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
External (HIDW, MethodObj)
External (HIWC, MethodObj)

Scope (\_SB.PC00) {
  //
  // Management Engine Interface 1 - Device 22, Function 0
  //
  Device (HECI) {
    Name (_ADR, 0x00160000)

    Method (_DSM, 0x4, NotSerialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
      ADBG ("HECI _DSM")
      if (PCIC (Arg0)) { return (PCID (Arg0,Arg1,Arg2,Arg3)) }

      //-------------------------------------------
      //  HID Wake up Event solution
      //-------------------------------------------
      If (CondRefOf (HIWC)) {
        If (HIWC (Arg0)) {
          If (CondRefOf (HIDW)) {
            Return (HIDW (Arg0, Arg1, Arg2, Arg3))
          }
        }
      }

      Return (Buffer () {0})
    } // End _DSM
  } // Device (HECI)

  //
  // Management Engine Interface 3 - Device 22, Function 4
  //
  Device (HEC3) {
    Name (_ADR, 0x00160004)

    Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
      If (PCIC (Arg0)) { Return (PCID (Arg0,Arg1,Arg2,Arg3)) }

      Return (Buffer () {0})
    } // End _DSM
  } // End Device HEC3

  //
  // Management Engine OSSE 0 - Device 19, Function 0 (#HECI1)
  //
  Device (OSE0) {
    Name (_ADR, 0x00130000)

  } // End Device OSE0

  //
  // Management Engine OSSE 1 - Device 19, Function 1 (#HECI2)
  //
  Device (OSE1) {
    Name (_ADR, 0x00130001)

  } // End Device OSE1

  //
  // Essential Security Engine 0 - Device 24, Function 0 (#HEC1)
  //
  Device (ESE0) {
    Name (_ADR, 0x00180000)
  }
}

