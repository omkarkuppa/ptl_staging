/** @file
  Intel ACPI Sample Code for Intel BT DSM FUNC6 Implementation

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
External (\_SB.RP_PATH1.BTPC.PRRS, IntObj)
External (CBTI, IntObj)

//
// Function 6: Get (by ibtpci Driver) _PRR Mode (for IOSF) Status
//
If (LEqual (Arg2, 6)) {
  //
  // Get _PRR Mode and Get Last_PRR status
  //
  // Reset method type
  //   0: FLR (Core Reset)
  //   1: Product Reset
  //
  // Reset mode statusW
  //   0: Non-PLDR executed [Default]
  //   1: Core FLR completed successfully
  //   2: Product PLDR completed successfully
  //   3: Previous Product PLDR not completed
  //   4: Product PLDR timeout
  //
  If (CondRefOf (CBTI)) {
    If (LEqual (CBTI, 2)) {
      // BT interface is PCI (IOSF)
      If (LAnd (CondRefOf (\_SB.RP_PATH1.BTPC.RSTT), CondRefOf (\_SB.RP_PATH1.BTPC.PRRS))) {
        ADBG (Concatenate ("Get _PRR Mode = ", ToHexString (\_SB.RP_PATH1.BTPC.RSTT)))
        ADBG (Concatenate ("Get Last_PRR status = ", ToHexString (\_SB.RP_PATH1.BTPC.PRRS)))
        // return _PRR Mode and Last_PRR status
        Return (Package () {\_SB.RP_PATH1.BTPC.RSTT, \_SB.RP_PATH1.BTPC.PRRS})
      }
    } Else {
      ADBG ("BT interface is not PCI (IOSF)")
    }
  }
  Return (0)
}