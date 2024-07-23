/** @file
  DPTF APIs that need EC supported.

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

#ifndef PARENT_OF_LPCB
#define PARENT_OF_LPCB PC00
#endif

External (\_SB.PARENT_OF_LPCB.LPCB.H_EC, DeviceObj)

External (\_SB.PLDT.FIDC, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CPDC, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.UPCR, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.UPPM, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPS, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPBS, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RCTP, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPM, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RFCS, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.UPF2, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RFTH, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RFTL, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RSTH, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.UPFH, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.UPFL, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RSFC, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RFPM, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.UPFP, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RCFS, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPV, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RRPT, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RADR, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RWPP, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.STSN, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RSSV, MethodObj)
External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.USTP, MethodObj)

// UVTH (Under Voltage Threshold.)
// UVTH is a command which BIOS sends to EC. [15:0]
//
// Arguments: (1)
//    Arg0 should be sent by Intel (R) Dynamic Tuning Technology driver in response to power delivery capability changes. The unit is 1mV.
// Return Value:
//    0x0           - Success
//    0xFFFFFFFFF   - EC is not available
//
Method (UVTH, 1, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    // power delivery capability changed
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.CPDC (Arg0)
    Return (0x0)
  } Else {
    Return (0xFFFFFFFF)
  }
}

// SCHG (Set Battery Charge Rate)
//
// Set Charge Rate in Amp
//
//  Arguments: (1)
//    Arg0 -  Charge rate in Amperes
//  Return Value:
//    0x0           - Success
//    0xFFFFFFFFF   - EC is not available
//
Method (SCHG, 1, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    // write amp value to EC and Set Charge Rate
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPCR (Arg0)
    Return (0x0)
  } Else {
    Return (0xFFFFFFFF)
  }
}

/**
  UPPM          Update the Programable Power Supply Mode.

  @param        1 byte data represent the PPS mode.
  @retval       0x0           - Success.
  @retval       0xFFFFFFFF    - EC is not available.
**/
Method (UPPM, 1, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPPM (Arg0)
    Return (0x0)
  } Else {
    Return (0xFFFFFFFF)
  }
}

// CMPP (Maximum peak current)
//
// The battery discharge current capability for at least 1ms, in 1mA unit
//
// Arguments: (0)
//   None
// Return Value:
//   battery discharge current value in mA
//
Method (CMPP, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPS (), 2))
  } Else {
    Return (0xFFFFFFFF)
  }
}

// VBNL (Battery No-Load Voltage)
//
// The battery instantaneous no-load voltage that can be obtained from battery fuel gauge. The unit is 1mV.
//
//  Arguments: (0)
//    None
//  Return Value:
//    battery no-load voltage value in mV
//    0xFFFFFFFFF   - EC is not available
//
Method (VBNL, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPS (), 1))
  } Else {
    Return (0xFFFFFFFF)
  }
}

// RBHF (Battery high frequency impedance Method)
//
// The RBHF The high frequency impedance value that can be obtained from battery fuel gauge. The unit is 1mOhm.
//
//  Arguments: (0)
//    None
//  Return Value:
//    high frequency impedance value in mOhm
//    0xFFFFFFFFF   - EC is not available
//
Method (RBHF, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPS (), 0))
  } Else {
    Return (0xFFFFFFFF)
  }
}

// PBSS (Power Battery Steady State Method)
//
// Returns max sustained power for battery.
//
//  Arguments: (0)
//    None
//  Return Value:
//    power in milliwatts
//    0xFFFFFFFFF   - EC is not available
//
Method (PBSS, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPBS ())
  } Else {
    Return (0xFFFFFFFF)
  }
}

// CTYP (Charger Type Method)
//
// The CTYP object provides charger type.
//
//  Arguments: (0)
//    None
//  Return Value:
//    0x01 = Traditional
//    0x02 = Hybrid
//    0x03 = NVDC
//    0xFFFFFFFFF   - EC is not available
//
Method (CTYP, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RCTP ())
  } Else {
    Return (0xFFFFFFFF)
  }
}

// BMAX (Battery A Maximum Method)
//
// Returns Battery A Maximum value
//
//  Arguments: (0)
//    None
//  Return Value:
//    Returns Battery A Maximum value
//    0xFFFFFFFFF   - EC is not available
//
Method (BMAX, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPM ())
  } Else {
    Return (0xFFFFFFFF)
  }
}

// FCHG (Check Fast Charging Enable Method)
//
// Checks if Fast Charging is enabled
//
//  Arguments: (0)
//    None
//  Return Value:
//    Returns 1 if Fast Charging is enabled
//    0xFFFFFFFFF   - EC is not available
//
Method (FCHG, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RFCS ())
  } Else {
    Return (0xFFFFFFFF)
  }
}

// FNSL (Fan Set Speed Level Method)
//
// Set Fan Speed Level 0-100 by Fan Index
//
//  Arguments: (2)
//    Arg0 - Fan Index (0-2) for fans 1-3
//    Arg1 - New Speed Level 0-100
//    Arg2 - Previous Speed Level. Limitation: EC doesn't remember the last Fan level in multi-fan setup
//  Return Value:
//    0x0           - Success
//    0xFFFFFFFFF   - EC is not available
//
Method (FNSL, 3, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Store (\_SB.PLDT.FIDC (Arg0), Local0) // Convert Fan ID to Fan Port
    // EC implementation-specific PWM port control
    // Command to turn the fan on
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPF2 (Local0, Arg1, Arg2)
    Return (0x0)
  } Else {
    Return (0xFFFFFFFF)
  }
}

Name(FTHL, Package()
{
  0xFFFFFFFF, // Fan RPM high threshold:Integer DWORD
  0xFFFFFFFF  // Fan RPM low threshold:Integer DWORD
})

// GFHE: Return the current fan speed threshold in RPM or percentage if fan does not report RPM information (EC Method)
//
// Arguments: (0)
// None
// Return Value:
// Return the current fan speed High threshold in RPM
//
Method (GFHE, 0, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RFTH ())
  } Else {
    Return (0xFFFFFFFFF)
  }
}

// GFLE: Return the current fan speed threshold in RPM or percentage if fan does not report RPM information (EC Method)
//
// Arguments: (0)
// None
// Return Value:
// Return the current fan speed Low threshold in RPM
//
Method (GFLE, 0, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RFTL ())
  } Else {
    Return (0xFFFFFFFFF)
  }
}

// SFHE: Set the fan speed High threshold in RPM.
//
// Arguments: (1)
// Arg0 - Integer: conveys to the platform the fan speed High threshold to be set.
// Return Value:
// None
//
Method (SFHE, 1, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPFH (Arg0)
  }
}

// SFLE: Set the fan speed Low threshold in RPM.
//
// Arguments: (1)
// Arg0 - Integer: conveys to the platform the fan speed Low threshold to be set.
// Return Value:
// None
//
Method (SFLE, 1, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPFL (Arg0)
  }
}

// GSHE: Get Fan Speed Threshold Hysterisis
//
// Arguments: (0)
// None
// Return Value:
// Return the current fan Threshold Hysterisis value
//
Method (GSHE, 0, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RSTH ())
  } Else {
    Return (0xFFFFFFFFF)
  }
}

// GFCE (Get Supported Fan Capability) (EC Method)
//
// Arguments: (0)
// None
// Return Value:
// Return the current supported fan capability
// Bit0 - Fan speed control supported, 0 - DTT cann't change fan speed directly, 1 - DTT can change the fan speed
// Bit1 - Fan operating mode control supported, 0 - DTT cann't change fan operating mode, 1 - DTT can change the fan mode
// Bit2 - Fan RPM report supported, 0 - Fan dosn't report RPM information(pin4 is not available), 1 - Fan can report RPM information to DTT
// Bit3 - Fan maintainance mode supported, 0 - Fan doesn't support maintainance mode, 1 - Fan supports maintainance mode
//
Method (GFCE, 0, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RSFC ())
  } Else {
    Return (0)
  }
}

// GFME (Get Fan Policy Mode) (EC Method)
//
// Arguments: (0)
// None
// Return Value:
// Return the current fan policy mode
// 0 - Default - EC controls the fan operating mode
// 1 - Performance - EC should allow high fan speed for best performance
// 2 - Balanced - EC should allow median fan speed for better performance
// 3 - Quiet - EC should allow low fan speed for quietness
// 4-14 - Reserved
// 15 - Maintainance - EC should perform any maintainance operation including dust cleaning by reverting the fan direction. OEM should decide the fan speed value in the maintainance mode
//
Method (GFME, 0, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RFPM ())
  } Else {
    Return (0)
  }
}

// SFME (Set Fan Policy Mode)
//
// Set the desired fan policy based on the current system usage scenarios
//
// Arguments: (1)
// Arg0 - Integer: conveys to the platform the fan policy mode to be set.
// Return Value:
// None
//
Method (SFME, 1, Serialized)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    // Update fan operating mode.
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPFP (Arg0)
  }
}


// GFNS (Get Fan Speed Method)
//
// Read Current Fan Speed in RPM by Fan Index
//
//  Arguments: (1)
//    Arg0 - Fan Index (0-2) for fans 1-3
//  Return Value:
//    Fan speed in RPM. report 0 RPM for invalid cases
//    0xFFFFFFFFF   - EC is not available
//
Method (GFNS, 1, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Switch (ToInteger (Arg0))
    {
      Case (0){
        Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RCFS ())
      }
      Case (1){
        Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RCFS ())
      }
      Case (2){
        Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RCFS ())
      }
      Default {
        Return (0) // Return 0 as speed if no registered Fan Present
      }
    }
  } Else {
    Return (0xFFFFFFFFF)
  }
}

// B1FC (Battery 1 Full Charge Method)
//
// Returns Battery 1 Full Charge Capacity
//
//  Arguments: (0)
//    None
//  Return Value:
//    Battery 1 Full Charge Capacity
//    0xFFFFFFFFF   - EC is not available
//
Method (B1FC, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPV (), 2))
  } Else {
    Return (0xFFFFFFFFF)
  }
}

// B1RC (Battery 1 Remaining Charge Method)
//
// Returns Battery 1 Remaining Charge
//
//  Arguments: (0)
//    None
//  Return Value:
//    Battery 1 Remaining Charge
//    0xFFFFFFFFF   - EC is not available
//
Method (B1RC, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBPV (), 1))
  } Else {
    Return (0xFFFFFFFFF)
  }
}

// PWRT (Power Resource Type Method)
//
// This method is used to evaluated the current power resource type
//
//  Arguments: (0)
//    None
//  Return Value:
//    bitwise encoded information about power resource connected
//    0xFFFFFFFFF   - EC is not available
// Notes: (Bitmap from EC)
// PwrSrcType Bit[0] = 0 <DC>, Bit[0] = 1 <AC>, Bit[1] = 1 <USB-PD>, Bit[2] = 1 <Wireless Charging>
// Bit[7:4]  = Power Delivery State Change Sequence Number
//
Method (PWRT, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RRPT ())
  } Else {
    Return (0xFFFFFFFFF)
  }
}

// ARTG (Adapter Rating Method)
//
// This method returns the rating of the adapter connected in mW
//
//  Arguments: (0)
//    None
//  Return Value:
//    Worst case platform power in Watts
//    0xFFFFFFFFF   - EC is not available
//
Method (ARTG, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Multiply (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RADR (), 10, Local0) // Becasue of EC-space constraint EC can only send 2bytes=15000mW but system uses 150000mW, hence bios is multiplying this value by 10.
  } Else {
    Store (0xFFFFFFFF, Local0)
  }
  Return (Local0)
}

// PROP (Rest of Platform Power Method)
//
// Worst case platform power in Watts
//
//  Arguments: (0)
//    None
//  Return Value:
//    Worst case platform power in Watts
//    0xFFFFFFFFF   - EC is not available
//
Method (PROP, 0, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RWPP ())
  } Else {
    Return (0xFFFFFFFFF)
  }
}

// PBOK (Power Boss OK Method)
//
// Used by DTT to inform EC about power delivery state change
// EC will de-assert PROCHOT if the sequence number matches the current power change sequence number
// Method used in conjuction with PSRC to provide a mechanism for AC removal protection.
// to prevent system brown out
//
//  Arguments: (1)
//    Arg0 - Power Delivery State change sequence Number
//  Return Value:
//    0x0           - Success
//    0xFFFFFFFFF   - EC is not available
//
Method (PBOK, 1, Serialized)
{
  // Check If EC opregion is available
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    // Store sequence number
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.STSN (Arg0)
    Return (0x0)
  } Else {
    Return (0xFFFFFFFF)
  }
}

// SNRD (Sensor Read Method)
//
// Read a temperature value of EC Sensor by Sensor Index
//
//  Arguments: (1)
//    Arg0 - Sensor Index (0-4) for sensors 1-5
//  Return Value:
//    Sensor temperature in tenths of deg celsius. report 300C for invalid cases
//
Method (SNRD, 1, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RSSV (), Local0)
    Switch (ToInteger (Arg0))
    {
      Case (0){
        Return (DerefOf (Index (Local0, 0)))
      }
      Case (1){
        Return (DerefOf (Index (Local0, 1)))
      }
      Case (2){
        Return (DerefOf (Index (Local0, 2)))
      }
      Case (3){
        Return (DerefOf (Index (Local0, 3)))
      }
      Case (4){
        Return (DerefOf (Index (Local0, 4)))
      }
      Default {
        Return (3000)
      }
    }
  }
  Else {
    Return (3000)
  }
}
//
// Create a Mutex for PATx methods to prevent Sx resume race condition problems asscociated with EC commands.
//
Mutex (PATM, 0)

// STRP (Set Trip Point Method)
//
// Set trip point for a specific EC sensor by Sensor Index
//
//  Arguments: (4)
//    Arg0 - Sensor Index (0-4) for sensors 1-5
//    Arg1 - Trip point temperature in tenths of degree celsius
//    Arg2 - Sensor Hysteresis in deg celsius
//    Arg3 - Input to indicate whether it is Low trip point or High trip point
//           0 - Low trip point
//           1 - High trip point
//  Return Value:
//    0x0           - Success
//    0xFFFFFFFFF   - EC is not available or the Mutex acquire fail
//
Method (STRP, 4, Serialized, 0, IntObj)
{
  If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
    Store (Acquire (PATM, 100), Local0)  // Save Acquire result so we can check for Mutex acquired
    If (LEqual (Local0, Zero))  // Check for Mutex acquired
    {
      // Set Trip point.
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.USTP (Arg0, Arg1, Arg2, Arg3)
      Release (PATM)
      Return (0x0)
    } Else {
      Return (0xFFFFFFFFF)
    }
  } Else {
    Return (0xFFFFFFFFF)
  }
}