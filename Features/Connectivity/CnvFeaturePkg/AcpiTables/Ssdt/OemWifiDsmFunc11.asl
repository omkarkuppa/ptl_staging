/** @file
  Intel ACPI Sample Code for OEM WiFi DSM FUNC11 Implementation

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

//
// Function 11: DLVR RFI Mitigation and DDR RFI Mitigation
//
// DLVR RFI Mitigation  - CDRM (BIT 0)
// DDR  RFI Mitigation  - CRFI (BIT 1)
//
// Return value |   CDRM    |   CRFI   |
// --------------------------------------
//      0       |  Enable   |  Enable  |
//      1       |  Disable  |  Enable  |
//      2       |  Enable   |  Disable |
//      3       |  Disable  |  Disable |
//
External (CRFI)

If (LEqual (Arg2, 11)) {
  If (CondRefOf (CDRM)) {
    If (CondRefOf (CRFI)) {
      If (LEqual (CDRM, 1)) {
        If (LEqual (CRFI, 1)) {
          Return (0)
        } Else {
          Return (2)
        }
      } Else {
        If (LEqual (CRFI, 1)) {
          Return (1)
        } Else {
          Return (3)
        }
      }
    }
  }
  Return (0)
}