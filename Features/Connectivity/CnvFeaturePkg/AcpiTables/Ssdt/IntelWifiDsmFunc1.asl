/** @file
  Intel ACPI Sample Code for Intel WiFi DSM FUNC1 Implementation

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

External (CVPR, IntObj)

//
// Function 1: Platform Information
// Type: return type DWORD
// Bit 0 - Is vPRO platform
//
If (LEqual (Arg2, 1)) {
  ADBG (Concatenate ("vPro: ", CVPR))
  If (CondRefOf (CVPR)) {
    ShiftLeft (CVPR, 0, Local0)
  }
  Return (Local0)
}