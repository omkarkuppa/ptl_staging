/** @file
  Intel ACPI Sample Code for Intel BT DSM FUNC5 Implementation

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

External (\_SB.RP_PATH1.BTPC.RSTT, IntObj)
External (CBTI, IntObj)

//
// Function 5: BT PRR for IOSF - Dynamic PLDR
// Arg 3: DWORD
// Struct PLDR_MODE {
//   UINT16 CmdType;
//   UINT16 CmdPayload
// };
//

If (LEqual (Arg2, 5)) {
  CreateWordField (Arg3, 0, BCMT)  // CmdType
  CreateWordField (Arg3, 2, BCMP)  // CmdPayload
  ADBG ("BT PRR for IOSF - Dynamic PLDR")
  ADBG (Concatenate ("CmdType = ", ToHexString (BCMT)))
  ADBG (Concatenate ("CmdPayload = ", ToHexString (BCMP)))

  //
  // Set BT _PRR Mode for IOSF
  //
  //
  // CmdType = 1, set (by ibtpci driver) _PRR Mode
  // Bit 0 : Reset Type
  //     0 : BT Core Reset (Default)
  //     1 : Product Reset (PLDR Abort flow)
  //
  If (LEqual (BCMT, 1)) {
    If (LAnd (CondRefOf (CBTI), LEqual (CBTI, 2))) {
      // BT interface is PCI (IOSF)
      If (CondRefOf (\_SB.RP_PATH1.BTPC.RSTT)) {
        ADBG (Concatenate ("Set BT _PRR Mode for IOSF = ", ToHexString (BCMP)))
        Store (BCMP, \_SB.RP_PATH1.BTPC.RSTT)
      }
    } Else {
      ADBG ("BT interface is not PCI (IOSF)")
    }
  }
  Return (0)
}