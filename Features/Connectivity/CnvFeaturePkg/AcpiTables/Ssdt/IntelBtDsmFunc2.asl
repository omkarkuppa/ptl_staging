/** @file
  Intel ACPI Sample Code for Intel BT DSM FUNC2 Implementation

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

External (RDLY, IntObj)

//
// Function 2 : Get BT reset timing
//
If (LEqual (Arg2, 2)) {
  If (CondRefOf (RDLY)) {
    ADBG (Concatenate ("Get RDLY ", ToDecimalString (RDLY)))
    Return (RDLY)
  }
}