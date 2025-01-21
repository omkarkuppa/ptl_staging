 /**@file
  This file defines the device objects for the TSE device

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
External (\PCHS, IntObj)

Scope (\_SB.PC00) {
  //
  // TSE Device
  //
  Device (TSE) {
     
    Name (_HID, "INTC10DF")                 // PTL 
    Name (_UID, 0)                          // 1st instance of the controller

    Method (_STA, 0x0, NotSerialized) {
      ADBG (Concatenate ("TSE Device Enabled = ", ToHexString (TSEE)))
      If (LEqual(TSEE, 1)) {
        Return(0x0F)
      }
      Return(0x00)
    } // End of Method (_STA)
  } // End of Device (TSE)
} // End of Scope for TSE
