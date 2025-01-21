/** @file
  Intel ACPI Reference Code for BT Audio OffLoad

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

External (CRFP, IntObj)
External (CBTA, IntObj)
External (CBOI, IntObj)
External (HDBO, IntObj)

//
// Variables and definitions required by this file
// CBTA - variable holding CNVi BT Audio Offload
//

//
// Update BT Audio Offload setting
//
Method (AOLX) {
  // BT Audio Offload data structure
  Name (AODS, Package () {
    0,      // Revision
    0x12,   // DomainType, 0x12:Bluetooth Core
    0       // Audio Offload selection, 0:Disabled, 1:Enabled, 3:Enabled I2S, 5:Enabled SoundWire
  })

  If (LAnd (CondRefOf (CBTA), LEqual (CBTA, 1))) {
    If (LAnd (CondRefOf (CRFP), LEqual (CRFP, 1))) {
      If (LAnd (CondRefOf (CBOI), LEqual (CBOI, 1))) {
        Store (5, Index (AODS, 2))
      } Else {
        Store (3, Index (AODS, 2))
      }
    } Else {
      If (LAnd (CondRefOf (HDBO), And (HDBO, 1))) {
        Store (3, Index (AODS, 2))
      }
    }
  }
  Return (AODS)
}

//
// AOLD (Audio Offload)
//   Arguments:
//     None
//   Return Value:
//     Revision
//     DomainType
//     Audio Offload selection, default value is Disabled
//

Method (AOLD) {
  Return (AOLX ())
}