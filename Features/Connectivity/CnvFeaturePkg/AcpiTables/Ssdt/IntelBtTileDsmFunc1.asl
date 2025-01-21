/** @file
  Intel ACPI Sample Code for Intel BT Tile DSM FUNC1 Implementation

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
// Function 1 : Set Tile Activation
//
// Currently no action to BIOS in function 1. OEM can define if anything is needed when setting Tile activation.
//
If (LEqual (Arg2, One)) {
  ADBG (Concatenate ("Tile Activation ", ToDecimalString (Arg3)))
}