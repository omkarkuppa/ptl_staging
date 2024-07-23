/** @file
  ACPI RTD3 SSDT table for PCIe 5G WWAN

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
// PCIe slot power resource definition for 5G
//
If (LNotEqual (WRTO, 2)) { // Not exposing PXP Power resources even for D3/L1.2
  PowerResource (PXP5, 0, 0) {
    Method (_STA, 0, Serialized) {
      If (LEqual (VDID, 0xFFFFFFFF)) {
        ADBG (Concatenate ("_STA no RP ", ToHexString (SLOT)))
        Return (0)
      }
      Return (PSTA ())
    }

    Method (_ON, 0, Serialized) {
      //
      // Acquire _ON mutex and save acquire result so we can check for Mutex acquired
      //
      Store (Acquire (\WWMT, 1000), Local0)
      ADBG ("WWAN _ON WWMT Mutex:Acquired Try Done")
      If (LEqual (Local0, Zero)) // check for _ON Mutex acquired
      {
        ADBG ("WWAN _ON WWMT Mutex:Acquired Done")
        If (LEqual (VDID, 0xFFFFFFFF)) {
          ADBG (Concatenate ("_ON no RP ", ToHexString (SLOT)))
          Release (\WWMT)
          Return ()
        }

        If (LEqual (OFEN, 1)) {
          ADBG ("WWAN is already ON.")
          Release (\WWMT)
          Return ()
        }

        \_SB.SHPO (WAKG, 1) // Change HostOwnership to GPIO Driver mode for disabling wake

        //
        // Turn on slot power
        //
        PON5 ()

        //
        // Trigger L2/L3 ready exit flow in rootport - transition link to Detect
        //
        L23D ()

        If (CondRefOf (WOFF)) {
          If (LNotEqual (WOFF, Zero)) {
            Store (0, WOFF)

            // This time includes PCIe Detect Timer and PCIe Link Establishment Timer for modem. Time delay should
            // kick in L3 Path only
            Sleep (TR2P)
            ADBG (Concatenate ("SLP TR2P ", ToDecimalString (TR2P)))
          }
        }

        Store (1, OFEN)

        //
        // Release WWAN _ON mutex
        //
        Release (\WWMT)
        ADBG ("WWAN _ON WWMT Mutex:Release Done")
      }
    }

    Method (_OFF, 0, Serialized) {
      //
      // Acquire _OFF mutex and save acquire result so we can check for Mutex acquired
      //
      Store (Acquire (\WWMT, 1000), Local0)
      ADBG ("WWAN _OFF WWMT Mutex:Acquired Try Done")
      If (LEqual (Local0, Zero)) // check for _OFF Mutex acquired
      {
        ADBG ("WWAN _OFF WWMT Mutex:Acquired Done")
        If (LEqual (VDID, 0xFFFFFFFF)) {
          ADBG (Concatenate ("_OFF no RP", ToHexString (SLOT)))
          Release (\WWMT)
          Return ()
        }

        If (LEqual (OFEN, 0)) {
          ADBG ("WWAN is already OFF.")
          Release (\WWMT)
          Return ()
        }

        //
        // Trigger L2/L3 ready entry flow in rootport
        //
        DL23 ()

        //
        // Turn off slot power
        //
        POF5 ()

        Store (0, WKEN)
        Store (0, OFEN)

        //
        // Release WWAN _OFF mutex
        //
        Release (\WWMT)
        ADBG ("WWAN _OFF WWMT Mutex:Release Done")
      }
    }
  }
}

// Turn on power to PCIe Slot for 5G
// Since this method is also used by the remapped devices to turn on power to the slot
// this method should not make any access to the PCie config space.
Method (PON5, 0, Serialized) {
  ADBG (Concatenate ("PON RP ", ToHexString (SLOT)))

  // Turn ON Power for PCIe Slot
  If (CondRefOf (WOFF)) {
    If (LNotEqual (WOFF, 0)) {
      // Delay by TFDI ms if required using WOFF
      Divide (Subtract (Timer (), WOFF), 10000, , Local0)      // Store Elapsed time in ms, ignore remainder
      ADBG (Concatenate ("SLP TFDI ", ToDecimalString (TFDI)))
      If (LLess (Local0, TFDI)) {                              // If Elapsed time is less than TFDI ms
        Sleep (Subtract (TFDI, Local0))                        // Sleep for the remaining time
      }

      If (CondRefOf (PWRG)) {
        ADBG ("WWAN PWR ON")
        \PIN.ON (PWRG)
      }
      // Delay of TN2B ms between de-assertion of FCPO# and de-assertion of BBRST#
      Sleep (TN2B)
      ADBG (Concatenate ("SLP TN2B ", ToDecimalString (TN2B)))

      If (CondRefOf (BRST)) {
        // Drive BB Reset Pin high
        \PIN.OFF (BRST)
      }
      // Delay of TB2R ms between de-assertion of BBRST# and de-assertion of PERST#
      Sleep (TB2R)
      ADBG (Concatenate ("SLP TB2R ", ToDecimalString (TB2R)))
    }
  }

  If (CondRefOf (SCLK)) {
    SPCO (SCLK, 1)
  }

  // De-assert Reset Pin
  \PIN.OFF (RSTG)
}

// Turn off power to PCIe Slot 5G
// Since this method is also used by the remapped devices to turn off power to the slot
// this method should not make any access to the PCIe config space.
Method (POF5, 0, Serialized) {
  ADBG (Concatenate ("POFF RP ", ToHexString (SLOT)))

  // Assert Reset Pin
  // Reset pin is mandatory for correct PCIe RTD3 flow
  \PIN.ON (RSTG)

  //
  // On RTD3 entry, BIOS will instruct the PMC to disable source clocks.
  // This is done through sending a PMC IPC command.
  //
  If (CondRefOf (SCLK)) {
    SPCO (SCLK, 0)
    Sleep (16)
  }

  // Power OFF for Slot
  If (LEqual (WKEN, 0)) {
    // Delay of TR2B ms between assertion of PERST# and assertion of BBRST#
    Sleep (TR2B)
    If (CondRefOf (BRST)) {
      \PIN.ON (BRST) // Drive BB RESET Pin low
    }
    // Delay of TB2F ms between assertion of BBRST# and assertion of FCPO#
    Sleep (TB2F)
    ADBG (Concatenate ("TB2F ", ToDecimalString (TB2F)))
    ADBG ("WWAN PWR OFF")
    If (CondRefOf (PWRG)) {
      \PIN.OFF (PWRG)
    }
    // Store current timestamp in WOFF
    If (CondRefOf (WOFF)) {
      Store (Timer (), WOFF)
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
}

Scope (PXSX) {
  //
  // Method _PS0: Provides required delay for WWAN device to transition to D0 from another WWAN device state
  //
  Method (_PS0, 0, Serialized) {
    ADBG ("WWAN _PS0")
    //Wait for Link Active with retry every 16 ms with a timeout value of 176 ms.
    Store (0, Local0)
    While (LEqual (WWAN_PCIE_ROOT_PORT.LASX, 0)) {
      If (Lgreater (Local0, 20)) // Poll for ~300ms
      {
        ADBG ("WWAN _PS0 LASX TO")
        Break
      }
      Sleep (16)
      Increment (Local0)
    }
    ADBG (Concatenate ("_PS0 LASX Delay Time ", ToDecimalString (Local0)))
  }

  //
  // Method _PS3: Dummy _PS3 () to comply with ACPI Spec
  //
  Method (_PS3, 0, Serialized) {
    ADBG ("WWAN _PS3")
  }
}