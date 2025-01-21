/** @file
  Intel ACPI Sample Code for connectivity modules

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

External (\_SB.BTRK, MethodObj)
External (\CNMT, MutexObj)
External (BRMT, IntObj)
External (CBTC, IntObj)
External (CBTI, IntObj)
External (RDLY, IntObj)
External (WIST, MethodObj)
External (WIFI_EP_PATH (01).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (02).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (03).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (04).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (05).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (06).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (07).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (08).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (09).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (10).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (11).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (12).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (13).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (14).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (15).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (16).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (17).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (18).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (19).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (20).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (21).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (22).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (23).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (24).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (25).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (26).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (27).BTIE, FieldUnitObj)
External (WIFI_EP_PATH (28).BTIE, FieldUnitObj)

Method (BTIV, 1, Serialized) {
  Store (FixedPcdGet8 (PcdMaxRootPortNumber), Local1)
  for (Local0 = 1, (Local0 == Local1), Local0++) {
    Switch (ToInteger (Local0)) {
      Case (1) {
        If (CondRefOf (WIFI_EP_PATH (01).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (01).BTIE)
        }
        Break
      }
      Case (2) {
        If (CondRefOf (WIFI_EP_PATH (02).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (02).BTIE)
        }
        Break
      }
      Case (3) {
        If (CondRefOf (WIFI_EP_PATH (03).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (03).BTIE)
        }
        Break
      }
      Case (4) {
        If (CondRefOf (WIFI_EP_PATH (04).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (04).BTIE)
        }
        Break
      }
      Case (5) {
        If (CondRefOf (WIFI_EP_PATH (05).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (05).BTIE)
        }
        Break
      }
      Case (6) {
        If (CondRefOf (WIFI_EP_PATH (06).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (06).BTIE)
        }
        Break
      }
      Case (7) {
        If (CondRefOf (WIFI_EP_PATH (07).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (07).BTIE)
        }
        Break
      }
      Case (8) {
        If (CondRefOf (WIFI_EP_PATH (08).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (08).BTIE)
        }
        Break
      }
      Case (9) {
        If (CondRefOf (WIFI_EP_PATH (09).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (09).BTIE)
        }
        Break
      }
      Case (10) {
        If (CondRefOf (WIFI_EP_PATH (10).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (10).BTIE)
        }
        Break
      }
      Case (11) {
        If (CondRefOf (WIFI_EP_PATH (11).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (11).BTIE)
        }
        Break
      }
      Case (12) {
        If (CondRefOf (WIFI_EP_PATH (12).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (12).BTIE)
        }
        Break
      }
      Case (13) {
        If (CondRefOf (WIFI_EP_PATH (13).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (13).BTIE)
        }
        Break
      }
      Case (14) {
        If (CondRefOf (WIFI_EP_PATH (14).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (14).BTIE)
        }
        Break
      }
      Case (15) {
        If (CondRefOf (WIFI_EP_PATH (15).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (15).BTIE)
        }
        Break
      }
      Case (16) {
        If (CondRefOf (WIFI_EP_PATH (16).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (16).BTIE)
        }
        Break
      }
      Case (17) {
        If (CondRefOf (WIFI_EP_PATH (17).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (17).BTIE)
        }
        Break
      }
      Case (18) {
        If (CondRefOf (WIFI_EP_PATH (18).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (18).BTIE)
        }
        Break
      }
      Case (19) {
        If (CondRefOf (WIFI_EP_PATH (19).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (19).BTIE)
        }
        Break
      }
      Case (20) {
        If (CondRefOf (WIFI_EP_PATH (20).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (20).BTIE)
        }
        Break
      }
      Case (21) {
        If (CondRefOf (WIFI_EP_PATH (21).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (21).BTIE)
        }
        Break
      }
      Case (22) {
        If (CondRefOf (WIFI_EP_PATH (22).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (22).BTIE)
        }
        Break
      }
      Case (23) {
        If (CondRefOf (WIFI_EP_PATH (23).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (23).BTIE)
        }
        Break
      }
      Case (24) {
        If (CondRefOf (WIFI_EP_PATH (24).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (24).BTIE)
        }
        Break
      }
      Case (25) {
        If (CondRefOf (WIFI_EP_PATH (25).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (25).BTIE)
        }
        Break
      }
      Case (26) {
        If (CondRefOf (WIFI_EP_PATH (26).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (26).BTIE)
        }
        Break
      }
      Case (27) {
        If (CondRefOf (WIFI_EP_PATH (27).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (27).BTIE)
        }
        Break
      }
      Case (28) {
        If (CondRefOf (WIFI_EP_PATH (28).BTIE)) {
          Store (Arg0, WIFI_EP_PATH (28).BTIE)
        }
        Break
      }
      Default {
        ADBG ("No matching RP found!!")
        Break
      }
    }
  }
}

//
// Device reset power resource for CNVi BT
//
PowerResource (BTRT, 5, 0)
{
  // Define the PowerResource for USB port
  // PowerResource expects to have _STA, _ON and _OFF Method per ACPI Spec. Not having one of them will cause BSOD

  // Method: Dummy _STA () to comply with ACPI Spec
  Method (_STA)
  {
    ADBG ("BT PR _STA")
    Return (1)
  }

  // Method: Dummy _ON () to comply with ACPI Spec
  Method (_ON, 0)
  {
    ADBG ("BT PR _ON")
  }

  // Method: Dummy _OFF () to comply with ACPI Spec
  Method (_OFF, 0)
  {
    ADBG ("BT PR _OFF")
  }

  // Method: _RST ()
  Method (_RST, 0, NotSerialized)
  {
    ADBG ("BT PR _RST")
    Store (Acquire (\CNMT, 1000), Local0)
    ADBG ("BT CNMT Mutex:Acquired Try Done")
    If (LEqual (Local0, Zero)) {
      \_SB.BTRK (0)
      If (CondRefOf (RDLY)) {
        Sleep (RDLY)
      }
      \_SB.BTRK (1)
      If (CondRefOf (RDLY)) {
        Sleep (RDLY)
      }
    }
    Release (\CNMT)
  }
} // End BTRT

//
// Platform-level device reset power resource for CNVd BT
//
PowerResource (DBTR, 5, 0)
{
  // Define the PowerResource for USB port
  // PowerResource expects to have _STA, _ON and _OFF Method per ACPI Spec. Not having one of them will cause BSOD

  // Method: Dummy _STA () to comply with ACPI Spec
  Method (_STA)
  {
    ADBG ("BT PR _STA")
    Return (1)
  }

  // Method: Dummy _ON () to comply with ACPI Spec
  Method (_ON, 0)
  {
    ADBG ("BT PR _ON")
  }

  // Method: Dummy _OFF () to comply with ACPI Spec
  Method (_OFF, 0)
  {
    ADBG ("BT PR _OFF")
  }

  // Method: _RST ()
  Method (_RST, 0, NotSerialized)
  {
    ADBG ("CNVd BT _RST")

    ADBG (Concatenate (" WVHO -  ", ToHexString (WVHO)))
    If (CondRefOf (\CNMT)) {
      Store (Acquire (\CNMT, 1000), Local0)
    }
    ADBG ("BT CNMT Mutex:Acquired Try Done")
    If (LEqual (Local0, Zero)) {
      If (LAnd (CondRefOf (BRMT), LEqual (BRMT, 0))) {
        ADBG ("BT RF Kill ON")
        \_SB.BTRK (0)
      } Else {
        If (LNotEqual (WVHO, 0)) {
          BTIV (0)
          ADBG ("Set BTIE Disable")
        }
      }
      If (CondRefOf (RDLY)) {
        Sleep (RDLY)
      }
      If (LAnd (CondRefOf (BRMT), LEqual (BRMT, 0))) {
        ADBG ("BT RF Kill OFF")
        \_SB.BTRK (1)
      } Else {
        If (LNotEqual (WVHO, 0)) {
          BTIV (1)
          ADBG ("Set BTIE Enable")
        }
      }
      If (CondRefOf (RDLY)) {
        Sleep (RDLY)
      }
    }
    If (CondRefOf (\CNMT)) {
      Release (\CNMT)
    }
  }
} // End DBTR

Method (_PRR) {
  //
  // Check if a discrete WiFi device is present on the root port
  //
  If (CondRefOf (WIST)) {
    If (WIST ()) {
      Return (Package () {DBTR})
    }
  }

  //
  // Default return CNVi BT power resource
  //
  Return (Package () {BTRT})
}