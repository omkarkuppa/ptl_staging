/** @file
  ACPI RTD3 SSDT table for Thunderbolt

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

**/

/// @details
/// Code in this file uses following variables:
/// SCLK: ICC Clock number - optional
/// WAKG: WAKE GPIO pad - optional
/// Below objects should be defined according to the format described in PinDriverLib.asl
/// RSTG: reset pin definition - mandatory
/// PWRG: power GPIO pad - optional
/// WAKP: Flag to indicate that power gating must not be performed if WAKE is enabled - optional
/// @defgroup pcie_scope PCIe Root Port Scope

External(\MMTB, MethodObj)
External(\DSCE, IntObj)

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
//
// External decalarations for optional objects.
// Defined by board specific code.
//
External(WAKG) //WAKE GPIO pad
External(PWRG)
External(SCLK)
External(WAKP)
External(TUID)
External(PSON)

External(NCB7)
External(LASX)
External(D3HT)
External(L23E)
External(L23R)
External(DPGE)
External(HPEX)
External(PMEX)
External(HPSX)
External(PMSX)
External(PDCX)
External(PDSX)
External(PSPX)

External(SLOT)
External(\TBD3, IntObj)

Name (TBPE, 1) // Reflects RTD3_PWR_EN value
Name (TOFF, 0) // Indicate whether system is entering Sx for POFF method

Name (WKEN, 0) // WAKE enable on PCIe device.
Name (TSTR, "                                ")    // dTBT identification string, reserved 32 characters
Name (PWRR, 0) // PCIe core power removal

If (LNot (CondRefOf (TUID))) {
  ADBG ("[dTBT] TUID is not present")
}

If (CondRefOf (PRTP)) {
  ADBG (Concatenate ("[dTBT] Current DTBT PCIe RP Type (PCH=2|CPU=4|ITBT=8) = ", ToHexString (PRTP)))
} Else {
  ADBG ("[dTBT] PRTP is not present for identifying RP type")
}

If (LAnd (CondRefOf (TUID), CondRefOf (SLOT))) {
  If (CondRefOf (PRTP)) {
    If (LEqual (PRTP, PCIE_RP_TYPE_PCH)) {
      Fprintf (TSTR, "dTBT %o (PCH-RP%o)", ToDecimalString (TUID), ToDecimalString (SLOT))
    } ElseIf (LEqual (PRTP, PCIE_RP_TYPE_CPU)) {
      Fprintf (TSTR, "dTBT %o (CPU-RP%o)", ToDecimalString (TUID), ToDecimalString (SLOT))
    } Else {
      Fprintf (TSTR, "dTBT %o (Type %o - RP%o)", ToDecimalString (TUID), ToDecimalString (PRTP), ToDecimalString (SLOT))
    }
    ADBG (Concatenate ("[dTBT] ", TSTR))
  }
}

//
// The deepest D-state supported by this device in the S0 system sleeping state where the device can wake itself,
// "4" represents D3cold.
//
Method (_S0W, 0, Serialized) {
  If (CondRefOf (\TBD3)) {
    If (LEqual (\TBD3, 0)) {
      ADBG (Concatenate ("[dTBT] Root port _S0W return 3 - dTBT ", ToDecimalString (TUID)))
      Return (0x3)
    } Else {
      ADBG (Concatenate ("[dTBT] Root port _S0W return 4 - dTBT ", ToDecimalString (TUID)))
      Return (0x4)
    }
  }
  Return (0x4)
}

//
// TBT Debug Method - Scan hierarchy
//
Method (TSCH, 0, Serialized) {

  Store (\MMTB (SLOT, PRTP), Local7)
  // TODO Make one generic copy
  OperationRegion (TBDM, SystemMemory, Local7, 0x550) // TBT HR PCICFG MMIO
  Field (TBDM, DWordAcc, NoLock, Preserve) {
    DIVI, 32,
    CMDR, 32,
    Offset (0x84),
    TBPS, 2, // PowerState of TBT
    Offset (0x548),
    TB2P, 32,
    P2TB, 32
  }

  ADBG ("=============================================================")
  ADBG (Concatenate ("[dTBT] Scan TBT topology at dTBT ", ToDecimalString (TUID)))
  ADBG (Concatenate ("[dTBT]   RP VDID = ", ToHexString(VDID)))

  If(LNotEqual (VDID,0xFFFFFFFF)) {
    ADBG (Concatenate ("[dTBT]   RP D3HT Power State = ", ToHexString (D3HT)))
    ADBG (Concatenate ("[dTBT]   RP PDCX Presence Detect Changed = ", ToHexString (PDCX)))
    ADBG (Concatenate ("[dTBT]   RP PDSX Presence Detect State = ", ToHexString (PDSX)))
    ADBG (Concatenate ("[dTBT]   RP PSPX PME Status = ", ToHexString(PSPX)))
  }
  ADBG (Concatenate ("[dTBT] TBT HR ECAM address = ", ToHexString (Local7)))
  ADBG (Concatenate ("[dTBT] TBT HR VDID = ",ToHexString(DIVI)))

  If (LNotEqual (DIVI,0xFFFFFFFF)) {
    ADBG (Concatenate ("[dTBT]   TBT HR Power State = ", ToHexString (TBPS)))
    ADBG (Concatenate ("[dTBT]   TBT HR TBT2PCIE(0x548) = ", ToHexString (TB2P)))
    ADBG (Concatenate ("[dTBT]   TBT HR PCIE2TBT(0x54C) = ", ToHexString (P2TB)))
  }

  Add(Local7, 0x00108000, Local7)                   // Advance 1 Bus 1 Dev No. Bus N+1 Device 1
  ADBG (Concatenate ("[dTBT] TBT DS1 ECAM address = ", ToHexString (Local7)))

  OperationRegion (TDS1, SystemMemory, Local7, 0x100) // TBT DS1 PCICFG MMIO
  Field (TDS1,DWordAcc, NoLock, Preserve) {
    Offset (0),
    P1ID, 32,
    Offset(0xD0),
    , 29,
    LAI1, 1,                                        // PCIe Link Active Indicator of DS port 1
    Offset(0xD8),
    , 19,
    PDC1, 1,                                        // PCIe Presence Detect Changed Indicator of DS port 1
    , 2,
    PDS1, 1,                                        // PCIe Presence Detect State Indicator of DS port 1
  }

  ADBG (Concatenate ("[dTBT] TBT DS1 VDID = ",ToHexString (P1ID)))
  If (LNotEqual (P1ID,0xFFFFFFFF)) {
    ADBG (Concatenate ("[dTBT] TBT DS1 Device Presence Change = ", ToHexString (PDC1)))
    ADBG (Concatenate ("[dTBT] TBT DS1 Device Presence = ", ToHexString (PDS1)))
  }

  Add (Local7, 0x00010000, Local7)                   // Advacne 2 Dev No. Bus N+1 Device 3
  ADBG (Concatenate ("[dTBT] TBT DS3 ECAM address = ", ToHexString (Local7)))

  OperationRegion(TDS3, SystemMemory, Local7, 0x100)// TBT DS3 PCICFG MMIO
  Field(TDS3,DWordAcc, NoLock, Preserve) {
    Offset (0),
    P3ID, 32,
    Offset(0xD0),
    , 29,
    LAI3, 1,                                        // PCIe Link Active Indicator of DS port 3
    Offset(0xD8),
    , 19,
    PDC3, 1,                                        // PCIe Presence Detect Changed Indicator of DS port 3
    , 2,
    PDS3, 1,                                        // PCIe Presence Detect State Indicator of DS port 3
  }

  ADBG (Concatenate ("[dTBT] TBT DS3 VDID = ",ToHexString(P3ID)))
  If (LNotEqual (P3ID,0xFFFFFFFF)) {
    ADBG (Concatenate ("[dTBT] TBT DS3 Device Presence Change = ", ToHexString (PDC3)))
    ADBG (Concatenate ("[dTBT] TBT DS3 Device Presence = ", ToHexString (PDS3)))
  }

  TVAL ()
  ADBG ("=============================================================")
}

//
// TBT Debug Method - Dump All Variables
//
Method (TVAL, 0, Serialized) {
  ADBG (Concatenate ("[dTBT] TBPE = ", ToHexString (TBPE)))
  ADBG (Concatenate ("[dTBT] TOFF = ", ToHexString (TOFF)))
}

Method (PPS0, 0, Serialized) { // Platform specific PCIe root port _PS0 Hook Function.
  ADBG (Concatenate ("[dTBT] PCIE RP PPS0 - dTBT ", ToDecimalString (TUID)))
  TSCH ()
}

Method (PPS3, 0, Serialized) { // Platform specific PCIe root port _PS3 Hook Function.
  ADBG (Concatenate ("[dTBT] PCIE RP PPS3 - dTBT ", ToDecimalString (TUID)))
  TSCH ()
}

//
// Alternate DSD
//
Method (ADSD, 0) {
  ADBG (Concatenate ("[dTBT] Root port _DSD - dTBT ", ToHexString (TUID)))
  Return (
    Package () {
      ToUUID("6211E2C0-58A3-4AF3-90E1-927A4E0C55A4"),
      Package () {
        Package (2) {"HotPlugSupportInD3", 1},
      },
      ToUUID("EFCC06CC-73AC-4BC3-BFF0-76143807C389"),
      Package () {
        Package (2) {"ExternalFacingPort", 1}, // Property 1: This is a TBT/CIO port
        Package (2) {"UID", TUID}, // Property 2: UID of the TBT RP on platform, range is: 0, 1, ..., NumOfTBTRP - 1
      }
    }
  ) // End of Return ()
}

Method (_DSW, 3) {
  /// This method is used to enable/disable wake from PCIe (WKEN)
  ADBG (Concatenate ("[dTBT] Root port _DSW start - dTBT ", ToDecimalString (TUID)))
  ADBG (Concatenate ("[dTBT] Arg0 (Enable Wake) = ", ToHexString (Arg0)))
  ADBG (Concatenate ("[dTBT] Arg1 (Target Sx)   = ", ToHexString (Arg1)))
  ADBG (Concatenate ("[dTBT] Arg2 (Target Dx)   = ", ToHexString (Arg2)))

  If (LGreaterEqual (Arg1, 1)) {
    /// If entering Sx, need to disable WAKE# from generating runtime PME also set 2 to TOFF.
    ADBG ("[dTBT] RP prepares to enter Sx state: Disable wake, TOFF = 2")
    Store (0, WKEN)
    Store (2, TOFF)
  } Else {
    /// If Staying in S0
    If (LAnd (Arg0, Arg2))
    {
      ///- Check if Exiting D0 and arming for wake
      ADBG ("[dTBT] RP prepares to enter Dx state and enable wake capabilities: Enable wake, TOFF = 1")
      ///- Set PME
      Store (1, WKEN)
      Store (1, TOFF)
    } Else {
      ADBG ("[dTBT] RP stays in D0 or disable wake capabilities: Disable wake, TOFF = 0")
      ///- Disable runtime PME, either because staying in D0 or disabling wake
      Store (0, WKEN)
      Store (0, TOFF)
    }
  }

  ADBG (Concatenate ("[dTBT] Root port _DSW End - dTBT ", ToDecimalString (TUID)))
}

If (CondRefOf (\TBD3)) {
  If (LNotEqual (\TBD3, 0)) {
    // PowerResource of DTBT PCIE RP report when DTBT RTD3 D3 cold support
    // TBD3 (DTBT RTD3 Enable)  : 00 - Disable
    //                            01 - Enable
    //
    // PCIe slot power resource definition
    //
    PowerResource (PXP, 0, 0) {
      Method (_STA, 0) {
        If (LEqual (VDID, 0xFFFFFFFF)) {
          ADBG (Concatenate ("[dTBT] _STA no RP - dTBT ", ToDecimalString (TUID)))
          Return (0)
        }
        Return (PSTA ())
      }

      Method (_ON) {
        ADBG (Concatenate ("[dTBT] TBT _ON Start - dTBT ", ToDecimalString (TUID)))
        TSCH ()
        PON ()
        TSCH ()
        ADBG (Concatenate ("[dTBT] TBT _ON End - dTBT ", ToDecimalString (TUID)))
      }

      Method (_OFF) {
        ADBG (Concatenate ("[dTBT] TBT _OFF Start - dTBT ", ToDecimalString (TUID)))
        TSCH ()
        POFF ()
        TSCH ()
        ADBG (Concatenate ("[dTBT] TBT _OFF End - dTBT ", ToDecimalString (TUID)))
      }
    } // End of PowerResource (PXP, 0, 0)
  } // End of (LNotEqual (\TBD3, 0))
}

//
// Returns the status of PCIe slot core power
//
Method (PSTA, 0) {
  //
  // RESET# assertion is mandatory for PCIe RTD3
  // So if RESET# is asserted the whole slot is off
  //
  If(\PIN.STA (RSTG)) {
    ADBG (Concatenate ("[dTBT] PCIe slot core power status (PSTA): OFF - dTBT ", ToDecimalString (TUID)))
    Return (0)
  } Else {
    ADBG (Concatenate ("[dTBT] PCIe slot core power status (PSTA): ON - dTBT ", ToDecimalString (TUID)))
    Return (1)
  }
}

//
// Get Permission for Power Removal.
// Check whether or not to Disable Power Package GPIO During Device Power OFF(D3 Cold Transition).
// Input: VOID
//
// @return 1 if it is Safe to Remove/Disable Power. 0 Not allow for Power Removal.
//
Method (GPPR, 0) {
  ADBG (Concatenate ("[dTBT] Check permission for power removal - dTBT ", ToDecimalString (TUID)))

  // If WAKP has not been defined we can safely disable power.
  // If WAKP is defined this slot does not supply device with auxilary power and we have to keep primary power
  // to allow for WAKE. If WAKP is not equal to 0 and WKEN has been enabled do not disable the power.
  If (CondRefOf (WAKP)) {
    If (LAnd (LNotEqual (WAKP, 0), LEqual (WKEN, 0))) {
      ADBG ("[dTBT] WAKP is non-zero and WKEN is enabled, core power removal not allowed.")
      Return (0)
    }
  } Else {
    ADBG ("[dTBT] WAKP is not present")
  }

  //
  // If PWRR is 0, remove pcie slot power
  // If PWRR is 1, block pcie slot power
  //
  If (CondRefOf (PWRR)) {
    If (LNotEqual (PWRR, 0)) {
      ADBG_PRINT_PCIE_RP_INFO_AFTER ("PCIe slot power removal blocked For ")
      Return (0)
    }
  } Else {
    ADBG ("[dTBT] PWRR is not present")
  }

  //
  // Now Safe To Remove/Disable Power.
  //
  ADBG ("[dTBT] Safe to remove power")
  Return (1)
}

Method (PON, 0, Serialized) /// Turn on core power to PCIe Slot
{
  ADBG (Concatenate ("[dTBT] Turn on TBT core power (PON) Start - dTBT ", ToDecimalString (TUID)))
  ADBG (Concatenate ("[dTBT] Timestamp ", ToHexString (Timer ())))

  Store (PSTA (), Local0) // Check core power status, we may check PSTA () instead of TBPE in PON
  ADBG (Concatenate ("[dTBT] TBPE = ", ToHexString (TBPE)))

  If (LNotEqual (Local0, TBPE)) {
    ADBG ("[dTBT] TBPE flag doesn't align with TBT core power status!!")
  }

  // Check RTD3 power enable, if already ON, no need to execute sx_exit
  If (LEqual (TBPE, 1)) {
    ADBG ("[dTBT] RTD3 power already ON, abort TBT PON")
    ADBG (Concatenate ("[dTBT] Turn on TBT core power (PON) End - dTBT", ToDecimalString (TUID)))
    Return ()
  }

  //
  // dTBT special requirement on RTD3 exit:
  // 1. Guarantee that PCIe clock is running - MR clock request is asserted and BIOS can unmask the request and plus custom delay.
  // 2. Toggle RTD3 pin - This will indicate to MR that it can reset the PHY. Reset of the PHY requires a running clock.
  // 3. After 10ms de-assert the PERST (same as done today)
  //
  ADBG ("[dTBT] Special requirement on RTD3 exit")

  //
  // On RTD3 Exit, BIOS will instruct the PMC to Enable source clocks.
  // This is done through sending a PMC IPC command.
  //
  If (CondRefOf (\DTRC)) {
    If (\DTRC) {
      /// de-assert CLK_REQ MSK
      If (LAnd (CondRefOf (SPCO), (CondRefOf (SCLK)))) {
        ADBG ("[dTBT] De-assert CLK_REQ")
        SPCO (SCLK, 1)
      } Else {
        ADBG ("[dTBT] SPCO or SCLK is not present for de-asserting CLK_REQ")
      }
      If (CondRefOf (\DTRD)) {
        If (LNotEqual (\DTRD, 0)) {
          ADBG (Concatenate ("[dTBT] Sleep for dTBT RTD3 CLKREQ Delay(ms): ", ToDecimalString (\DTRD)))
          Sleep (\DTRD)
        }
      } Else {
        ADBG ("[dTBT] DTRD is not present for dTBT RTD3 CLKREQ delay")
      }
    } // If (\DTRC)
  } Else {
    ADBG ("[dTBT] DTRC is not present for de-asserting CLK_REQ")
  }

  /// Turn ON Power for PCIe Slot
  If (CondRefOf (PWRG)) {
    ADBG ("[dTBT] Turn on power for PCIe slot")
    \PIN.ON (PWRG)
    If (CondRefOf (PEP0)) {
      ADBG (Concatenate ("[dTBT] Sleep for device D0 transition delay(ms): ", ToDecimalString (PEP0)))
      Sleep (PEP0)
    } Else {
      ADBG ("[dTBT] PEP0 is not present for device D0 transition delay")
    }
  } Else {
    ADBG ("[dTBT] PWRG is not present for turning on PCIe slot power")
  }

  /// De-Assert Reset Pin
  ADBG ("[dTBT] De-Assert RST")
  \PIN.OFF (RSTG)

  Store (1, TBPE)
  ADBG ("[dTBT]  L2/L3 exit Start")
  L23D ()
  ADBG ("[dTBT]  L2/L3 exit End")

  PSTA () // Check core power status
  ADBG (Concatenate ("[dTBT] Timestamp ", ToHexString(Timer())))
  ADBG (Concatenate ("[dTBT] Turn on TBT core power (PON) End - dTBT ", ToDecimalString (TUID)))
} // End of ON

Method (POFF, 0, Serialized) { /// Turn off core power to PCIe Slot
  ADBG (Concatenate ("[dTBT] Turn off TBT core power (POFF) Start - dTBT ", ToDecimalString (TUID)))
  ADBG (Concatenate ("[dTBT] Timestamp ", ToHexString (Timer ())))

  Store (PSTA (), Local0)  // Check core power status, we may check PSTA () != 0 instead of TOFF == 1 for power off action
  ADBG (Concatenate ("[dTBT] TBPE = ", ToHexString (TBPE)))
  ADBG (Concatenate ("[dTBT] TOFF = ", ToHexString (TOFF)))

  If (LEqual (TOFF, 0)) {
    ADBG ("[dTBT] _DSW indicates staying at D0 or disabling wake at S0, skip POFF action")
    ADBG (Concatenate ("[dTBT] Turn off TBT core power (POFF) End - dTBT ", ToDecimalString (TUID)))
    Return ()
  }

  If (LEqual (TOFF, 2)) {
    ADBG ("[dTBT] System is entering Sx, skip POFF action")
    ADBG (Concatenate ("[dTBT] Turn off TBT core power (POFF) End - dTBT ", ToDecimalString (TUID)))
    Return ()
  }

  //
  // Do power off action only if TOFF == 1 and power status is on
  //
  If (LEqual (Local0, 0)) {  // Instead of setting TOFF to 0, we may do power off only if PSTA () != 0
    ADBG ("[dTBT] Power already off, skip POFF action")
    ADBG (Concatenate ("[dTBT] Turn off TBT core power (POFF) End - dTBT ", ToDecimalString (TUID)))
    Return ()
  }

  ADBG ("[dTBT]  L2/L3 entry Start")
  DL23 ()
  ADBG ("[dTBT]  L2/L3 entry End")

  // Assert Reset Pin
  // Reset pin is mandatory for correct PCIe RTD3 flow
  ADBG ("[dTBT] Assert reset pin")
  \PIN.ON (RSTG)

  //
  // On RTD3 entry, BIOS will instruct the PMC to disable source clocks.
  // This is done through sending a PMC IPC command.
  //
  If (CondRefOf (\DTRC)) {
    If (\DTRC) {
      /// assert CLK_REQ MSK
      If (LAnd (CondRefOf (SPCO), (CondRefOf (SCLK)))) {
        ADBG ("[dTBT] Call SPCO to assert CLK_REQ")
        SPCO (SCLK, 0)
      } Else {
        ADBG ("[dTBT] SPCO or SCLK is not present for asserting CLK_REQ")
      }
      If (CondRefOf (\DTRD)) {
        If (LNotEqual (\DTRD, 0)) {
          ADBG (Concatenate ("[dTBT] Sleep for DTBT RTD3 CLKREQ Delay(ms): ", ToDecimalString (\DTRD)))
          Sleep (\DTRD)
        }
      }
    }
  } Else {
    ADBG ("[dTBT] DTRC is not present for asserting CLK_REQ")
  }

  ADBG ("[dTBT] Sleep 10 ms")
  Sleep (10)

  /// Power OFF for TBT
  If (CondRefOf (PWRG)) {
    //
    // Check power removal permission before power removal
    //
    If (LEqual (GPPR (), 1)) {
      ADBG ("[dTBT] RTD3 CIO power off")
      \PIN.OFF (PWRG)
    } Else {
      ADBG ("[dTBT] WAKE enable and keep CIO power on")
    }
  } Else {
    ADBG ("[dTBT] PWRG is not present for power removal!")
  }

  Store (0, TBPE)

  ADBG (Concatenate ("[dTBT] Sleep for Power Off Delay (ms):", ToHexString (\DTRO)))
  Sleep (\DTRO)

  PSTA ()  // Check core power status
  ADBG (Concatenate ("[dTBT] Timestamp ", ToHexString (Timer ())))
  ADBG (Concatenate ("[dTBT] Turn off TBT core power (POFF) End - dTBT", ToDecimalString (TUID)))
} // End of Method_OFF

Method (NFRP, 0, Serialized) { /// Notify root port
  ADBG (Concatenate ("[dTBT] NFRP entry : Force OS to rescan TBT hierarchy - dTBT ", ToDecimalString (TUID)))

  If (LEqual (VDID, 0xFFFFFFFF))
  {
    ADBG ("[dTBT] Root port gone, exit without notifying OS")
    Return ()
  }

  If (LEqual (\DTFS, 0x01)) {// if Thunderbolt(TM) support is enabled

    ADBG (Concatenate ("[dTBT] Notify OS of bus check and device wake event for RP SLOT ", ToHexString (SLOT)))
    Notify (^, 0)
    Notify (^, 2)
  }

  ADBG (Concatenate ("[dTBT] NFRP end - dTBT ", ToDecimalString (TUID)))
}

// Pcie slot core power turn on/off method.
// Arg0 - DSM Func(A) Arg3 value to determine if
//   power needs to be Blocked/Removed from pcie slot.
//   0x80000000 - Block core power removal.
//   0x0        - Allow core power removal.
Method (PCPR, 1, Serialized) {
  If(LEqual (Arg0, 0x80000000)) {
    Store (1, PWRR) //Block slot Power Removal
    Return (2)
  } ElseIf (LEqual (Arg0, 0x0)) {
    Store (0, PWRR) // Allow slot power Removal
    Return (1)
  } Else {
    Return (0)
  }
}

If (CondRefOf (TBD3)) {
  If (LEqual (TBD3, 1)) {
    Method (_PR0) {
      Return (Package () {DTBT_BR_PCIE_ROOT_PORT.PXP})
    }
    Method (_PR3) {
      Return (Package () {DTBT_BR_PCIE_ROOT_PORT.PXP})
    }
  }
}
