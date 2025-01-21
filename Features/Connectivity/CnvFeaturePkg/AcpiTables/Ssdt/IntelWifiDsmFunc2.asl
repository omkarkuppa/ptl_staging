/** @file
  Intel ACPI Sample Code for Intel WiFi DSM FUNC2 Implementation

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

External (\_SB.RP_PATH1.CNVW.PRRS, IntObj)
External (\_SB.RP_PATH1.CNVW.RSTT, IntObj)
External (RSTY, IntObj)
External (FLRC, IntObj)
External (BOFC, IntObj)
External (DPRS, IntObj)
External (WIST, MethodObj)

//
// Function 2: Dynamic PLDR
// Arg 3: DWORD
// Struct PLDR_MODE {
//   UINT16 CmdType;
//   UINT16 CmdPayload
// };
//

If (LEqual (Arg2, 2)) {
  CreateWordField (Arg3, 0, CMDT)
  CreateWordField (Arg3, 2, CMDP)
  ADBG ("Dynamic PLDR")
  ADBG (Concatenate ("CmdType = ", ToHexString (CMDT)))
  ADBG (Concatenate ("CmdPayload = ", ToHexString (CMDP)))

  //
  // Get _PRR Mode
  //
  If (LEqual (CMDT, 1)) {
    If (CondRefOf (\_SB.RP_PATH1.CNVW.RSTT)) {
      ADBG (Concatenate ("Get _PRR Mode = ", ToHexString (\_SB.RP_PATH1.CNVW.RSTT)))
      Return (\_SB.RP_PATH1.CNVW.RSTT)
    } Else {
      If (CondRefOf (WIST)) {
        If (WIST ()) {
          If (CondRefOf (RSTY)) {
            Return (RSTY)
          }
        }
      }
    }
    Return (0)
  }
  //
  // Set (by WiFi driver) _PRR Mode
  // Bit 0 : Reset Type
  //     0 : Core Reset (Default)
  //     1 : Product Reset (CNVi + CNVd GaP)
  //
  // Bit 1 : Product Reset FLR Control
  //     0 : Product Flow without FLR
  //     1 : Product flow with WiFi FLR + Timeout (10ms) [Default]
  //
  // Bit 2 : Product Reset BT ON/OFF Control
  //     0 : Product Flow without BT OFF-ON
  //     1 : Product flow with BT ON_OFF + BT Timeout (160ms) [Default]
  //
  If (LEqual (CMDT, 2)) {
    If (CondRefOf (\_SB.RP_PATH1.CNVW.RSTT)) {
      ADBG (Concatenate ("Set _PRR Mode = ", ToHexString (CMDP)))
      Store (CMDP, \_SB.RP_PATH1.CNVW.RSTT)
    } Else {
      If (CondRefOf (WIST)) {
        If (WIST ()) {
          ADBG (Concatenate ("Set _PRR Mode = ", ToHexString (CMDP)))

          And (CMDP, BIT0, Local0)
          If (CondRefOf (RSTY)) {
            Store (Local0, RSTY)
          }
          And (CMDP, BIT1, Local1)
          ShiftRight (Local1, 1, Local1)
          If (CondRefOf (FLRC)) {
            Store (Local1, FLRC)
          }
          And (CMDP, BIT2, Local2)
          ShiftRight (Local2, 2, Local2)
          If (CondRefOf (BOFC)) {
            Store (Local2, BOFC)
          }
        }
      }
    }
    Return (0)
  }
  //
  // Get Last_PRR status
  //
  If (LEqual (CMDT, 3)) {
    If (CondRefOf (\_SB.RP_PATH1.CNVW.PRRS)) {
      ADBG (Concatenate ("Get Last_PRR status PRRS = ", ToHexString (\_SB.RP_PATH1.CNVW.PRRS)))
      Return (\_SB.RP_PATH1.CNVW.PRRS)
    } Else {
      If (CondRefOf (WIST)) {
        If (WIST ()) {
          If (CondRefOf (DPRS)) {
            ADBG (Concatenate ("Get Last_PRR status DPRS = ", ToHexString (DPRS)))
            Return (DPRS)
          }
        }
      }
    }
    Return (0)
  }
  Return (0)
}