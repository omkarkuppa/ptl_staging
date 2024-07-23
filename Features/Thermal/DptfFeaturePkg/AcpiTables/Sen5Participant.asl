/** @file
  Intel ACPI Reference Code for Intel(R) Dynamic Tuning Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

Scope(\_SB.IETM)
{

  Device(SEN5)
  {
    Name(_UID, "SEN5")
    Name(_STR, Unicode ("Thermistor Memory")) // Near memory
    Name(CTYP,0)  // Mode
    // PFLG
    //   0 - Default, participant is on the main board
    //   1 - Participant device is on the docking station
    //   2 - Participant device is on the detachable base
    //   3 - Participant device is an external device (such as a USB device, Intel(R) Dynamic Tuning Technology does not support this type of device today)
    Name(PFLG, 0)

    // Thermal Sensor Hysteresis, 2 degrees
    Name(GTSH, 20)
    // Sensor ID
    Name(SNID, 4)

    // Default values for trip points for the validation purpose.
    Name (SNAC,60)  // Active0 trip point in celsius
    Name (SNA1,50)  // Active1 trip point in celsius
    Name (SNA2,40)  // Active2 trip point in celsius
    Name (SNPV,65)  // Passive trip point in celsius
    Name (SNCC,80)  // Critical trip point in celsius
    Name (SNC3,70)  // Critical s3 trip point in celsius
    Name (SNHP,75)  // Hot trip point in celsius
    Name (SNSP,0)   // Thermal Sampling Period

    // Common methods between sensors. Do not move the include file above the Named methods\variables above.
    Include("SenXCommon.asl")

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
    Method(_STA)
    {
      If (LEqual(\_SB.S5DE,1)) {
        Return(0x0F)
      } Else {
        Return(0x00)
      }
    }

    // Number of Aux Trips available
    Name(PATC, 2)

    // PATx (Participant Programmable Auxiliary Trip) - Sets Aux Trip Point
    //
    // The PATx objects shall take a single integer parameter, in tenths of degree Kelvin, which
    // represents the temperature at which the device should notify the participant driver of
    // an auxiliary trip event. A PATx control method returns no value.
    //
    //  Arguments: (1)
    //    Arg0 - temperature in tenths of degree Kelvin
    //  Return Value:
    //    None
    //
    Method (PAT0,1,Serialized)
    {
      If (CondRefOf (\_SB.DPTF.STRP))
      {
        Divide (GTSH, 10,, Local0) // Hysteresis in degree value
        Store (\_SB.IETM.K10C (Arg0),Local1)
        \_SB.DPTF.STRP (SNID, Local1, Local0, 0) // Sensor ID, Trip Point in Celcius, 2degree Hysteresis, Low trip point
        Return
      }
      ADBG ("PAT0: STRP not available")
    }

    // PATx (Participant Programmable Auxiliary Trip) - Sets Aux Trip Point
    //
    // The PATx objects shall take a single integer parameter, in tenths of degree Kelvin, which
    // represents the temperature at which the device should notify the participant driver of
    // an auxiliary trip event. A PATx control method returns no value.
    //
    //  Arguments: (1)
    //    Arg0 - temperature in tenths of degree Kelvin
    //  Return Value:
    //    None
    //
    Method (PAT1,1,Serialized)
    {
      If (CondRefOf (\_SB.DPTF.STRP))
      {
        Divide (GTSH, 10,, Local0) // Hysteresis in degree value
        Store (\_SB.IETM.K10C (Arg0),Local1)
        \_SB.DPTF.STRP (SNID, Local1, Local0, 1) // Sensor ID, Trip Point in Celcius, 2degree Hysteresis, High trip point
        Return
      }
      ADBG ("PAT1: STRP not available")
    }
  } // End SEN5 Device
}// end Scope(\_SB.ITEM)
