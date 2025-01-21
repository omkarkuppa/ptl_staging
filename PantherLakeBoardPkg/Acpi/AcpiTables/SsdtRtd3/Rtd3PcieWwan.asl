/** @file
  ACPI RTD3 SSDT table for PCIe WWAN
  Scope (\_SB.PCI0.RPXX)
    _S0W
    _S4W
    _DSW
    _PR0
      PXP
        _STA
        _ON
        _OFF
    _PR3
  Scope (\_SB.PCI0.RPXX.PXSX)
    _PS0 (for 5G only)
    _PS3 (for 5G only)
    _PRR
      MRST (in DSDT)
        _STA: Dummy
        _ON:  Dummy
        _OFF: Dummy
        _RST: FULL_CARD_POWER_OFF# and RESET# Reset
    _DSM (in DSDT)
      Function 0: Support Functions Query
      Function 1: Reserved
      Function 2: RESET#-only Reset
      Function 3: Power

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

/// @details
/// Code in this file uses following variables:
/// SCLK: ICC Clock number - optional
/// WAKG: WAKE GPIO pad - optional
/// Below objects should be defined according to the format described in PinDriverLib.asl
/// RSTG: reset pin definition - mandatory
/// PWRG: power GPIO pad - optional
/// WAKP: Flag to indicate that power gating must not be performed if WAKE is enabled - optional
/// @defgroup pcie_scope PCIe Root Port Scope **/

//
// AcpiPinDriverLib imports(from DSDT in platform)
//
External(\PIN.STA, MethodObj)
External(\PIN.ON, MethodObj)
External(\PIN.OFF, MethodObj)
//
// GpioLib imports(DSDT)
//
External(\_SB.SHPO, MethodObj)

External(\WWMT)

//
// External decalarations for optional objects.
// Defined by board specific code.
//
External(WAKG)
External(PWRG)
External(SCLK)
External(WAKP)
External(WWEN)
External(TN2B) // PcdBoardWwanTOn2ResDelayMs
External(TB2R) // PcdBoardWwanTOnRes2PerDelayMs
External(TR2P) // PcdBoardWwanTOnPer2PdsDelayMs
External(TR2B) // PcdBoardWwanPer2ResDelayMs
External(TB2F) // PcdBoardWwanRes2OffDelayMs
External(TFDI) // PcdBoardWwanTOffDisDelayMs

// WAKE enable on PCIe device.
Name(WKEN, 0)

// Last OFF Timestamp (WOFF): The time stamp of the last power resource _OFF method evaluation
Name(WOFF, 0)

Name(OFEN, 1)
Name(ONEN, 0)

//
// Method _S0W: Returns a value of 4 if D3cold is supported by the platform; returns 0 in all other cases
//
Method(_S0W, 0, Serialized) {
  //
  // WRTO - WWAN RTD3 Option
  //   3: D3/L2
  //   2: D3/L1.2
  //   1: D0/L1.2
  //   0: Disabled
  //
  If (LEqual (WRTO, 3)) {
    Return (4)
  } ElseIf (LEqual (WRTO, 2)) { // return 3 (D3hot) for D3/L1.2
    Return (3)
  } Else {
    Return (0)
  }
}

//
// Method _S4W: Returns a value of 4 if D3cold is supported by the platform; returns 0 in all other cases
//
Method(_S4W, 0, Serialized) {
  //
  // WRTO - WWAN RTD3 Option
  //   3: D3/L2
  //   2: D3/L1.2
  //   1: D0/L1.2
  //   0: Disabled
  //
  If (LEqual (WRTO, 3)) {
    Return (4)
  } ElseIf (LEqual (WRTO, 2)) { // return 3 (D3hot) for D3/L1.2
    Return (3)
  } Else {
    Return (0)
  }
}

//
// _DSW (Device Sleep Wake)
//
// This control method can be used to enable or disable the device's ability to wake a
// sleeping system.
//
// Arguments: (3)
//   Arg0 - An Integer that contains the device wake capability control
//   Arg1 - An Integer that contains the target system state (0-4)
//   Arg2 - An Integer that contains the target device state
// Return Value:
//   None
//
Method(_DSW, 3) {
  // This method is used to enable/disable wake from PCIe (WKEN)
  ADBG("_DSW PCIe")
  ADBG(SLOT)
  If (LGreaterEqual (Arg1, 3)) {
    // If entering Sx, need to disable WAKE# from generating runtime PME
    Store(0, WKEN)
    ADBG("PCIE Sx DisWak")
  } Else {                   // If Staying in S0
    If(LAnd(Arg0, Arg2))     // Check if Exiting D0 and arming for wake
    {                        // Set PME
      Store(1, WKEN)
      ADBG("PCIE D3 EnWak")
    } Else {                 // Disable runtime PME, either because staying in D0 or disabling wake
      ADBG("PCIE D0 DisWak")
    }
  }
  /** @defgroup pcie_dsw PCIE _DSW **/
} // End _DSW

If (LEqual (WWEN, 2)) {
  Include ("Rtd3Pcie5GWwan.asl")
} Else {
  Include ("Rtd3Pcie4GWwan.asl")
}

// Returns the status of PCIe slot core power
Method (PSTA, 0) {
  //
  // Detect power pin status
  //
  If (CondRefOf (PWRG)) {
    If (LNot (\PIN.STA (PWRG))) {
      ADBG (Concatenate ("PSTA OFF ", ToHexString (SLOT)))
      Return (0)
    }
  }

  //
  // RESET# assertion is mandatory for PCIe RTD3
  // So if RESET# is asserted the whole slot is off
  //
  If (\PIN.STA (RSTG)) {
    ADBG (Concatenate ("PSTA OFF ", ToHexString (SLOT)))
    Return (0)
  } Else {
    ADBG (Concatenate ("PSTA ON ", ToHexString (SLOT)))
    Return (1)
  }
}

// Turn on power to PCIe Slot
// Since this method is also used by the remapped devices to turn on power to the slot
// this method should not make any access to the PCie config space.
Method (PON) {
  ADBG (Concatenate ("PON RP ", ToHexString (SLOT)))

  If (CondRefOf (WOFF)) {
    If (LNotEqual (WOFF, 0)) {
      // Turn ON Power for PCIe Slot
      If (CondRefOf (PWRG)) {
        ADBG ("WWAN PWR ON")
        \PIN.ON (PWRG)
        Sleep (PEP0)
        ADBG (Concatenate ("PEP0 ", ToHexString (PEP0)))
      }

      // Drive BB Reset Pin high
      // Delay of 20ms to make it ensure modem is come out from Reset modem
      \PIN.OFF (BRST)
      Sleep (20)
      } Else {
        // As per PCIe M.2 Spec , clock Should be applied in 20 microsecond
        Stall (20)
      }
    } Else {
    // As per PCIe M.2 Spec , clock Should be applied in 20 microsecond
    Stall (20)
  }

  If (CondRefOf (SCLK)) {
    SPCO (SCLK, 1)
  }

  // PERST De-assertion should be greater than 100 ms
  If (CondRefOf (WOFF)) {
    If (LNotEqual (WOFF, 0)) {
      Sleep (100)
    }
  }

  // De-assert Reset Pin
  \PIN.OFF (RSTG)
}

// Turn off power to PCIe Slot
// Since this method is also used by the remapped devices to turn off power to the slot
// this method should not make any access to the PCIe config space.
Method (POFF) {
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
}

If (LNotEqual (WRTO, 2)) {  // Not exposing _PR0 even for D3/L1.2
  Method (_PR0) {
    If (LEqual (WWEN, 2)) {
      Return (Package () {PXP5})
    } Else {
      Return (Package () {PXP})
    }
  }
}

If (LEqual (WRTO, 3)) {
  Method (_PR3) {
    If (LEqual (WWEN, 2)) {
      Return (Package () {PXP5})
    } Else {
      Return (Package () {PXP})
    }
  }
}
