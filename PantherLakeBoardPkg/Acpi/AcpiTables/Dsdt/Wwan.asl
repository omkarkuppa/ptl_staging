/** @file
  Intel ACPI Sample Code for WWAN

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

// WWST (WWAN Device Presence Status)
//
// Check if a WWAN Device is present on the RootPort.
//
// Arguments: (0)
//   None
// Return Value:
//    0 - if a device is not present.
//    1 - if a device is present.
//
Method (WWST,0,Serialized)
{
  // check Vendor-Device ID for supported devices
  If (CondRefOf (VDID)) {
    Switch (ToInteger (VDID)) {
      Case (0x73608086) {Return (1)} // XMM7360
      Case (0x75608086) {Return (1)} // XMM7560
      Case (0x4D7514C3) {Return (1)} // M80
      Case (0x080014C3) {Return (1)} // MaxSpring
      Default {Return (0)}           // no supported device
    }
  }Else {
    Return (0)
  }
}

//
// First Half of Reset Flow: WWAN Reset or OFF flow for Reset Recovery
// Arg0 - Indicates Reset Method : Warm Reset - 0  ,  Power Cycle Reset - 1
//
Method (FHRF, 1) {
  If (LNotEqual (\_SB.GGOV (\PRST), \WPRP)) {
    // 1. Trigger L2/L3 ready entry flow in root port
    DL23()
    ADBG ("WWAN DL23() Done")
    // 2. Assert PERST GPIO.
    \_SB.SGOV (\PRST, \WPRP)
    ADBG ("WWAN PERST:0 Done")
  } Else {
    ADBG ("Assume WWAN DL23() is already done")
  }

  // 3. Sleep for TR2B ms.
  Sleep (TR2B)
  // 4. Assert RESET GPIO.
  \_SB.SGOV (\WBRS, \PBRS)
  ADBG ("WWAN RST:0 Done")

  If (LEqual (Arg0, 0)) {                                      // For Warm Reset Flow
    // 5. Sleep for TBTG ms
    Sleep (TBTG)
    ADBG (Concatenate ("SLP TBTG ", ToDecimalString (TBTG)))
  } Else {
    If (LEqual (Arg0, 1)) {                                    // For Power Cycle Reset Flow
      // 5. Disable SRC Clock.
      SPCO (\WCLK, 0)
      ADBG ("WWAN SCLK:0 Done")
      // 6. Sleep for TB2F ms.
      Sleep (TB2F)
      ADBG (Concatenate ("SLP TB2F ", ToDecimalString (TB2F)))
      // 7. Assert FULL_CARD_POWER_OFF# GPIO.
      \_SB.SGOV (\WFCP, And (Not (\PFCP), 1))
      ADBG ("WWAN PWR:0 Done")
      // 8. Sleep for TFDI ms.
      Sleep (TFDI)
      ADBG (Concatenate ("SLP TFDI ", ToDecimalString (TFDI)))
    } Else {
      ADBG ("Unsupported Arg0 is detected.")
    }
  }
}

//
// 2nd Half of Reset Flow: WWAN ON flow for Reset Recovery
//
Method (SHRF) {
  // 2. Enable SRC Clock.
  SPCO (\WCLK, 1)
  ADBG ("WWAN SCLK:1 Done")
  // 3. De-assert FULL_CARD_POWER_OFF# GPIO.
  \_SB.SGOV (\WFCP, \PFCP)
  ADBG ("WWAN PWR:1 Done")
  // 4. Sleep for TN2B ms.
  Sleep (TN2B)
  ADBG (Concatenate ("SLP TN2B ", ToDecimalString (TN2B)))
  // 5. De-assert RESET GPIO.
  \_SB.SGOV (\WBRS, And (Not (\PBRS), 1))
  ADBG ("WWAN RST:1 Done")
  // 6. Sleep for TB2R ms.
  Sleep (TB2R)
  ADBG (Concatenate ("SLP TB2R ", ToDecimalString (TB2R)))
  // 7. De-assert PERST GPIO.
  \_SB.SGOV (\PRST, And (Not (\WPRP), 1))
  ADBG ("WWAN PERST:0 Done")
  // 8. Trigger L2/L3 ready exit flow in root port - transition link to Detect.
  L23D ()
  ADBG ("WWAN L23D() Done")
}

//
// Detect OEM SVID
// This is workaroud for OEM only and it's disabled by default.
//
Method (DOSV) {
  Store (0, Local0)
  While (LAnd (LNotEqual (PCIE_ROOT_PORT.PXSX.SVID, WSID), LLess (Local0, WSTO))) {
    Increment (Local0)
    Sleep (1)
  }
  ADBG (Concatenate ("DET Time ", ToDecimalString (Local0)))
}

Method (_RST, 0, Serialized) {
  ADBG ("_RST WWAN")

  If (LEqual (WWEN, 2)) {
    //
    // Reset flow for 5G WWAN
    //
    // 1. Acquire FLDR Mutex and save result to check for acquire Mutex
    Store (Acquire (\WWMT, 1000), Local0)
    ADBG ("WWAN FLDR Mutex: Acquired Try Done")
    // 2. Check for FLDR Mutex acquired
    If (LEqual (Local0, Zero))
    {
      ADBG ("Start WWAN FLDR")
      // 3. Perform 1st Half of FLDR Flow
      FHRF (0);
      // 4. Perform 2nd Half of FLDR Flow
      SHRF ();
      ADBG ("End WWAN FLDR")
      // 5. Release FLDR mutex
      Release (\WWMT)
      ADBG ("WWAN FLDR Mutex:Release Done")
      DOSV ();                                              // Workaround for OEM only
    }
  } Else {
    //
    // Reset flow for 4G WWAN
    //
    \_SB.SGOV (\WBRS, \PBRS)                // Drive BB RESET Pin low
    Sleep (200)                             // Safe 200ms
    Notify (PCIE_ROOT_PORT.PXSX, 1)
    \_SB.SGOV (\PRST, And (Not (\WPRP), 1)) // De-assert PERST
    \_SB.SGOV (\WBRS, And (Not (\PBRS), 1)) // Drive BB RESET Pin high
    Sleep (200)
    Store (0, Local0)
    While (LEqual (PCIE_ROOT_PORT.LASX, 0)) {
      If (Lgreater (Local0, 20)) {          // Wait for 300ms
        ADBG ("FLDR TO")
        Break
      }
      Sleep (16)
      Increment (Local0)
    }
    Sleep (10)
    ADBG (Concatenate ("RVP Delay Time ", ToDecimalString (Local0)))
    Notify (PCIE_ROOT_PORT.PXSX, 1)
  }
}

PowerResource (MRST, 5, 0)
{
  // Define the PowerResource for the WWAN device
  // PowerResource expects to have _STA, _ON and _OFF Method per ACPI Spec. Not having one of them will cause BSOD

  //
  // Method _STA: Dummy _STA () to comply with ACPI Spec
  //
  Method (_STA, 0, Serialized) {
    ADBG ("WWAN PR _STA")
    return (1)
  }

  //
  // Method _ON: Dummy _ON () to comply with ACPI Spec
  //
  Method (_ON, 0, Serialized) {
    ADBG ("WWAN PR _ON")
  }

  //
  // Method _OFF: Dummy _OFF () to comply with ACPI Spec
  //
  Method (_OFF, 0, Serialized) {
    ADBG ("WWAN PR _OFF")
  }

  //
  // Method _RST (): executes a WWAN device power-cycle reset using FULL_CARD_POWER_OFF# and RESET# signals
  //
  Method (_RST, 0, Serialized) {
    // 1. Acquire _PRR Mutex and save result to check for acquire Mutex
    Store (Acquire (\WWMT, 1000), Local0)
    ADBG ("WWAN PLDR Mutex: Acquired Try Done")
    // 2. Check for _PRR Mutex acquired
    If (LEqual (Local0, Zero))
    {
      ADBG ("Start WWAN PLDR")
      // 3. Perform 1st Half of PLDR Flow
      FHRF (1);
      // 4. Perform 2nd Half of PLDR Flow
      SHRF ();
      ADBG ("End WWAN PR _RST")
      // 5. Release _PRR mutex
      Release (\WWMT)
      ADBG ("WWAN PLDR Mutex: Release Done")
      DOSV (); // Workaround for OEM only
    }
  }
} // End MRST

Method (_PRR) {
  Return (Package () {MRST})
}

