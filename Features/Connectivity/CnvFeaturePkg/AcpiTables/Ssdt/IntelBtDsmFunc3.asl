/** @file
  Intel ACPI Sample Code for Intel BT DSM FUNC3 Implementation

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

External (BRMT, IntObj)

//
// Function 3 : BT PLDR Configure
// Arg 3 DWORD,
// Struct PLDR_MODE
// {
//    UINT16 CmdType;
//    UINT16 CmdPayLoad;
// };
//
If (LEqual (Arg2, 3)) {
  CreateWordField (Arg3, 0, CMDT)
  CreateWordField (Arg3, 2, CMDP)
  //
  // CmdType 1 - BT Reset Method Type
  // CmdPayLoad 1 -
  //   0 - Legacy RF kill, via the W_DISBALE#2 [DEFAULT]
  //   1 - VSEC, via WiFi CfgSpace VSEC Bit 1
  //
  If (CondRefOf (BRMT)) {
    If (LEqual (CMDT, 1)) {
      Store (CMDP, BRMT)
    }
  }
}