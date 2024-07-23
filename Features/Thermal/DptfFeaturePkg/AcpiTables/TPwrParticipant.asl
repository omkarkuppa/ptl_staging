/** @file
  Intel ACPI Reference Code for Intel(R) Dynamic Tuning Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

Scope (\_SB.IETM)
{

  Device (TPWR)  // Power participant
  {
    Name (_UID, "TPWR")
    Method (_HID)
    {
      Return (\_SB.PLDT.GHID (_UID))  // Intel(R) Dynamic Tuning Technology platform power device
    }

    Name (_STR, Unicode ("Platform Power"))
    Name (PTYP, 0x11)
    // PFLG
    //   0 - Default, participant is on the main board
    //   1 - Participant device is on the docking station
    //   2 - Participant device is on the detachable base
    //   3 - Participant device is an external device (such as a USB device, Intel (R) Dynamic Tuning Technology does not support this type of device today)
    Name (PFLG, 0)

    // _STA (Status)
    //
    // This object returns the current status of a device.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing a device status bitmap:
    //    Bit 0 - Set if the device is present.
    //    Bit 1 - Set if the device is enabled and decoding its resources.
    //    Bit 2 - Set if the device should be shown in the UI.
    //    Bit 3 - Set if the device is functioning properly (cleared if device failed its diagnostics).
    //    Bit 4 - Set if the battery is present.
    //    Bits 5-31 - Reserved (must be cleared).
    //
    Method (_STA)
    {
      If (LEqual (\_SB.PWRE,1)) {
        Return (0x0F)
      } Else {
        Return (0x00)
      }
    }

    // PSOC (Platform State of Charge)
    //
    // This object evaluates to the remaining battery state of charge in %.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   remaining battery charge in %
    //
    Method (PSOC)
    {
      If (CondRefOf (\_SB.DPTF.B1FC)) // prevent divide by zero
      {
        If (LEqual (\_SB.DPTF.B1FC (), 0xFFFFFFFF)) {
          ADBG ("EC is not available")
          Return (0)
        } ElseIf (LEqual (\_SB.DPTF.B1FC (), 0)) {
          ADBG ("Battery malfunction B1FC")
          Return (0) // Full charge cannot be 0, indicate malfunction in battery
        }
      }
      If (And (CondRefOf (\_SB.DPTF.B1FC), LEqual (\_SB.DPTF.B1FC (), 0))) // prevent divide by zero
      {
        ADBG ("Battery malfunction B1FC")
        Return (0) // Full charge cannot be 0, indicate malfunction in battery
      }
      If (Lnot(CondRefOf (\_SB.DPTF.B1RC)))
      {
        ADBG ("B1RC not available")
        Return (0)
      }
      If (LGreater(\_SB.DPTF.B1RC (), \_SB.DPTF.B1FC ())) // remaining charge > full charge
      {
        ADBG ("Battery malfunction RC greater than FC")
        Return (0) // Remaining Charge cannot be greater than Full Charge, this illegal value indicates the battery is malfunctioning
      }
      If (LEqual (\_SB.DPTF.B1RC (), \_SB.DPTF.B1FC ())) // remaining charge == full charge
      {
        ADBG ("Battery Fully Charged")
        Return (100) // indicate battery is fully charged
      }
      If (LLess (\_SB.DPTF.B1RC (), \_SB.DPTF.B1FC ())) // (RC*100)/FC=percent
      {
        Multiply (\_SB.DPTF.B1RC (),100,Local0) // RC*100
        Divide (Local0,\_SB.DPTF.B1FC (),Local2,Local1) // divide by FC
        Divide (Local2,100,,Local2) // account for the remainder in percent
        Divide (\_SB.DPTF.B1FC(),200,,Local3) // what number is 0.5%?
        If (LGreaterEqual (Local2,Local3)){ // round up if the remainder is equal or above .5
          Add (Local1,1,Local1)
        }
        ADBG (Concatenate("Battery Current Charge: ", ToDecimalString (Local1)))
        Return (Local1) // current charge in percent
      } Else {
        ADBG ("PSOC illegal condition")
        Return (0) // avoid compiler warning
      }
    }

    // PSRC (Power SouRCe)
    //
    // The PSRC object provides power source type.
    //
    // Arguments: (0)
    //   None
    // Return Value:(enumeration which Intel (R) Dynamic Tuning Technology driver expects)
    //   Bit[3:0] = Power Resource Type
    //   0x00 = DC
    //   0x01 = AC
    //   0x02 = USB
    //   Bit[7:4] = Power Delivery State Change Sequence Number
    //
    // Notes: (Bitmap from EC)
    // PwrSrcType Bit[0] = 0 <DC>, Bit[0] = 1 <AC>, Bit[1] = 1 <USB-PD>, Bit[2] = 1 <Wireless Charging>
    // Bit[7:4]  = Power Delivery State Change Sequence Number

    Method (PSRC, 0, Serialized)
    {
      ADBG ("PSRC")
      If (And (CondRefOf (\_SB.DPTF.PWRT), LEqual (\_SB.DPTF.PWRT (), 0xFFFFFFFF)))
      {
        ADBG ("EC is not available")
        Return (0) // EC not available, default to battery
      } Else {
        Store (\_SB.DPTF.PWRT (), Local0)
        And (Local0, 0xF0, Local1) // Keep Bit[7:4], clear Bit[3:0] in Local1
      }

      Switch (ToInteger (And (ToInteger (Local0),0x07))) // Switch Power Source Type Bits [3:0]
      {
        Case (0x0) { // Power source is DC
          ADBG ("DC")
          Or (Local1, 0x00, Local1)
        }
        Case (0x1) { // Power source is AC
          ADBG ( "AC")
          Or (Local1, 0x01, Local1)
        }
        Case (0x2) { // Power source is USB-PD
          ADBG ("PD")
          Or (Local1, 0x02, Local1)
        }
        Case (0x4) { // Power source is PPS Adapter
          ADBG ("PD and PPS Adapter")
          Or (Local1, 0x02, Local1)
        }
        Default {
          ADBG ("Default DC")
          Or (Local1, 0x00, Local1) //default to battery
        }
      } // end of switch case check
      Return (Local1)
    }

    // ARTG (Adapter RaTinG)
    //
    // The ARTG object provides AC adapter rating in mW.
    // ARTG should return 0 if PSRC is DC (0).
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   AC adapter rating in mW
    //
    Method (ARTG)
    {
      ADBG ("ARTG")
      If (LEqual ( (And (PSRC (),0x07)),1)) // Check Power Source Type Bits [3:0]
      {
        If (CondRefOf (\_SB.DPTF.ARTG))
        {
          Store (\_SB.DPTF.ARTG (), Local0)
          If (LNotEqual (Local0, 0xFFFFFFFF)) // Check If EC opregion is available
          {
            ADBG (Concatenate ("ARTG EC: ", ToHexString (Local0)))
            Return (Local0)
          }
        }
        ADBG ("ARTG EC not available")
        Return (90000) // 90 watts
      } Else {
        ADBG ("ARTG Power Source is  AC")
        Return (0)
      }
    }

    // PROP (Platform Rest Of worst case Power)
    //
    // This object provides maximum worst case platform rest of power.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   power in milliwatts
    //
    Method (PROP)
    {
      If (CondRefOf (\_SB.DPTF.PROP))
      {
        Store (\_SB.DPTF.PROP (), Local0)
        If (LNotEqual (Local0, 0xFFFFFFFF)) // Check If EC opregion is available
        {
          Multiply (Local0, 1000, Local0) // EC sending value in Watts, converting it to mW
          ADBG (Concatenate("PROP EC: ", ToHexString (Local0)))
          Return (Local0)
        }
      }
      ADBG ("PROP EC not available")
      Return (25000) // 25 watts
    }

    // PBOK
    //
    // PBOK is a command which BIOS sends to EC.
    // It will have a parameter which is the sequence number.
    //
    // Arguments: (1)
    //   Arg0 should be sent by Intel (R) Dynamic Tuning Technology driver which will be corresponding to Power delivery state change sequence number.
    //   Bit[3:0] = Power Delivery State Change Sequence number
    // Return Value:
    //   None
    //
    Method (PBOK,1,Serialized)
    {
      If (CondRefOf (\_SB.DPTF.PBOK))
      {
        Store (And (Arg0, 0x000F), Local0)
        \_SB.DPTF.PBOK (Local0)
        Return
      }
      ADBG ("PBOK not available")
    }

  } // End TPWR Device
}// end Scope (\_SB.IETM)
