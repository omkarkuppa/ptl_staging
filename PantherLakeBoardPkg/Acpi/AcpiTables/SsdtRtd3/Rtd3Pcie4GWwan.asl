/** @file
  ACPI RTD3 SSDT table for PCIe 4G WWAN

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

//
// PCIe slot power resource definition
//
PowerResource (PXP, 0, 0) {
  Method (_STA, 0, Serialized) {
    If (LEqual (VDID, 0xFFFFFFFF)) {
      ADBG (Concatenate ("_STA no RP ", ToHexString(SLOT)))
      Return(0)
    }
    Return (PSTA ())
  }

  Method (_ON, 0, Serialized) {
    If (LEqual (VDID, 0xFFFFFFFF)) {
      ADBG (Concatenate ("_ON no RP ", ToHexString (SLOT)))
      Return ()
    }

    If (LEqual (ONEN, 0)) {
      Return ()
    }

    \_SB.SHPO (WAKG, 1)

    If (CondRefOf (WOFF)) {
      If (LNotEqual (WOFF, Zero)) {
        Divide (Subtract (Timer (), WOFF), 10000, , Local0) // Store Elapsed time in ms, ignore remainder
        If (LLess (Local0, 500)) {                          // If Elapsed time is less than 500ms
          Sleep (Subtract (500, Local0))                    // Sleep for the remaining time
        }
      }
    }

    //
    // Turn on slot power
    //
    PON ()

    //
    // Trigger L2/L3 ready exit flow in rootport - transition link to Detect
    //
    L23D ()

    // This time includes PCIe Detect Timer and PCIe Link Establishment Timer for modem. Time delay should
    // kick in L3 Path only
    If (CondRefOf (WOFF)) {
      If (LNotEqual (WOFF, Zero)) {
        Store (0, WOFF)
        Sleep (500)
      }
    }

    Store (1, OFEN)
    Store (0, ONEN)
  }

  Method (_OFF, 0, Serialized) {
    If (LEqual (VDID, 0xFFFFFFFF)) {
      ADBG (Concatenate ("_OFF no RP", ToHexString (SLOT)))
      Return ()
    }

    If (LEqual (OFEN, 0)) {
      Return ()
    }

    //
    // Trigger L2/L3 ready entry flow in rootport
    //
    DL23 ()

    //
    // Turn off slot power
    //
    POFF ()

    If (LEqual (WKEN, 0)) {
      // Drive BB RESET Pin low
      \PIN.ON (BRST)
      Sleep (2) // 2ms
      ADBG ("WWAN PWR OFF")
      \PIN.OFF (PWRG)          // Set power pin to low
      If (CondRefOf (WOFF)) {
        Store (Timer (), WOFF) // Start OFF timer here
      }
    }

    // Enable WAKE
    If (CondRefOf (WAKG)) {
      If (LAnd (LNotEqual (WAKG, 0), WKEN)) {
        ADBG ("Enable WAKE")
        \_SB.SHPO (WAKG, 0)
      } Else {
        ADBG ("Disable WAKE")
        \_SB.SHPO (WAKG, 1)
      }
    }

    Store (0, OFEN)
    Store (1, ONEN)
  }
}