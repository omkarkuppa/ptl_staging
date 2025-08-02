/** @file
  Intel ACPI Reference Code for Intel(R) Dynamic Tuning Technology

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
External (\_SB.DPTF.RBHF, MethodObj)
External (\_SB.DPTF.VBNL, MethodObj)
External (\_SB.DPTF.CMPP, MethodObj)

Scope (\_SB.IETM)
{

  Device (BAT1)  // Battery Participant
  {
    Name (_UID, "1")
    Method (_HID)
    {
      Return(\_SB.PLDT.GHID(_UID))  // Intel (R) Dynamic Tuning Technology platform power device
    }

    Name (_STR, Unicode ("Battery 1 Participant"))
    Name (PTYP, 0x0C)

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
      If (LEqual (\_SB.BATR,1)) {
        Return (0x0F)
      } Else {
        Return (0x00)
      }
    }

    If (LEqual (TTEF,0x01)) {  // Check for Time to Empty feature enable
      Name (PBDP, Package ()
      {
        Package ()
        {
          0x01,            // Revision
        },
        Package ()
        {
          0xFFFFFFFF,      // DWordConst: ProcessedDischargeRate - only valid for  0-0x7FFFFFF, 0xFFFFFFFF (default) if there is no processed discharge rate (available)
          0x00000000,      // DWordConst: Reserved (default = 0)
        }
      })
    } // End of TTEF

    // PMAX (Platform MAXimum)
    //
    // The PMAX object provides maximum power that can be supported by the battery in mW.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   maximum power in mW
    //
    Method (PMAX,0,Serialized)
    {
      If (CondRefOf (\_SB.DPTF.BMAX))
      {
        Store (\_SB.DPTF.BMAX (), Local0)
        If(LNotEqual (Local0, 0xFFFFFFFF)) // Check If EC opregion is available
        {
          // convert PMAX from signed to unsigned integer
          // PMAX Value = (NOT (OR (FFFF0000, ECValue))+1) * 10.
          If (Local0) { // don't convert a zero
            Not (Or (0xFFFF0000,Local0,Local0),Local0) // make it a 32 bit value before inverting the bits
            Multiply (Add (1,Local0,Local0),10,Local0) // add 1 and multiply by 10
          }
          Return (Local0)
        }
      }
      ADBG ("PMAX: EC not available")
      Return (0) // EC not available

    }

    // CTYP (Charger TYPe)
    //
    // The CTYP object provides charger type.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //  0x01 = Traditional
    //  0x02 = Hybrid
    //  0x03 = NVDC
    //
    Method (CTYP)
    {
      If (CondRefOf (\_SB.DPTF.CTYP))
      {
        Store (\_SB.DPTF.CTYP (), Local0)
        If (LNotEqual (Local0, 0xFFFFFFFF)) // Check If EC opregion is available
        {
          Return (Local0)
        }
      }
      ADBG ("CTYP: EC not available")
      Return (3)
    }

    // PBSS (Power Battery Steady State)
    //
    // Returns max sustained power for battery.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   power in milliwatts
    //
    Method (PBSS)
    {
      If (CondRefOf (\_SB.DPTF.PBSS))
      {
        Store (\_SB.DPTF.PBSS (), Local0)
        If (LNotEqual (Local0, 0xFFFFFFFF)) {
          Return (Local0)
        }
      }
      ADBG ("PBSS: EC not available")
      Return (100)
    }

    // DPSP (Intel (R) Dynamic Tuning Technology Battery Sampling Period)
    //
    // Sets the polling interval in 10ths of seconds. A value of 0 tells the driver to use event notification for PMAX and PBSS..
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing the polling rate in tenths of seconds.
    //   A value of 0 will specify using interrupts through the ACPI notifications.
    //
    //   The granularity of the sampling period is 0.1 seconds. For example, if the sampling period is 30.0
    //   seconds, then report 300; if the sampling period is 0.5 seconds, then will report 5.
    //
    Method (DPSP,0,Serialized)
    {
      Return (\_SB.PPPR)
    }

    // RBHF (Battery high frequency impedance)
    //
    // The RBHF The high frequency impedance value that can be obtained from battery fuel gauge. The unit is 1mOhm.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   high frequency impedance value in mOhm
    //

    Method (RBHF)
    {
      If (CondRefOf (\_SB.DPTF.RBHF))
      {
        Store (\_SB.DPTF.RBHF (), Local0)
        If (LNotEqual (Local0, 0xFFFFFFFF)) {
          Return (Local0)
        }
      }
      ADBG ("RBHF: EC not available")
      Return (0xFFFFFFFF)   //battery fuel gauge not supported.
    }

    // VBNL (Battery No-Load Voltage)
    //
    // The battery instantaneous no-load voltage that can be obtained from battery fuel gauge. The unit is 1mV.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   battery no-load voltage value in mV
    //

    Method (VBNL)
    {
      If (CondRefOf (\_SB.DPTF.VBNL))
      {
        Store (\_SB.DPTF.VBNL (), Local0)
        If (LNotEqual (Local0, 0xFFFFFFFF)) {
          Return (Local0)
        }
      }
      ADBG ("VBNL: EC not available")
      Return (0xFFFFFFFF)   //battery fuel gauge not supported.
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

    Method (CMPP)
    {
      If (CondRefOf (\_SB.DPTF.CMPP))
      {
        Store (\_SB.DPTF.CMPP (), Local0)
        If (LNotEqual (Local0, 0xFFFFFFFF)) {
          Return (Local0)
        }
      }
      ADBG ("CMPP: EC not available")
      Return (0xFFFFFFFF)   //battery fuel gauge not supported.
    }

    If (LEqual (TTEF,0x01)) {  // Check for Time to Empty feature enable
      // PCAP (IPF BIOS Battery Participant CAPabilities)
      //
      // BIOS use this to inform IPF that it supports TTE enhancments
      //
      // Arguments: (0)
      //   None
      // Return Value:
      //   An Integer containing participant capabilities
      //    Bit 3:0   - Version
      //    Bit 4     - This battery participant supports TTE enhancment methods (SPBD, GPBD, and GRBD)
      //    Bit 5     - This battery participant supports DTT Power Boss
      //    Bit 31:6  - Reserved
      //
      Method (PCAP)
      {
        Return (\_SB.DPTF.PCCE ())
      }

      // GRBD (Get Raw Battery Data)
      //
      // Arguments: (0)
      //   None
      // Return Value:
      //   A package containing Raw Data
      //    Revision // uint32 GRBD Revision Number
      //    Package() {
      //       BatteryState
      //       BatteryPresentRate
      //       BatteryRemainingCapacity
      //       BatteryPresentVoltage
      //    }
      //
      Method (GRBD)
      {
        Return (\_SB.DPTF.GRBC ())
      }

      // SPBD (Set Processed Battery Data)
      //
      // Arguments: (2)
      //   Arg0 - An integer containing Revision number
      //   Arg1 - A package containing the Processed Battery Data (Discharge Rate only in V1)
      // Return Value:
      //   None
      //
      Method (SPBD,2,Serialized)
      {
        //  Arg1 is a Buffer object
        Store (DeRefOf (Index (Arg1, 0)), Local0) // Byte 0
        Store (DeRefOf (Index (Arg1, 1)), Local1) // Byte 1
        Store (DeRefOf (Index (Arg1, 2)), Local2) // Byte 2
        Store (DeRefOf (Index (Arg1, 3)), Local3) // Byte 3

        //  Combine the 4 bytes into a single 32-bit integer
        ShiftLeft (Local3, 24, Local4)
        ShiftLeft (Local2, 16, Local5)
        Or (Local4, Local5, Local4)
        ShiftLeft (Local1, 8, Local5)
        Or (Local4, Local5, Local4)
        Or (Local4, Local0, Local4)

        //  Store processed data
        Store (Local4, Index (DeRefOf (Index (PBDP, 1)), 0))
      }

      // GPBD (Get Processed Battery Data)
      //
      // Arguments: (0)
      //   None
      // Return Value:
      //   A buffer containing Revision number and package with Processed Battery data
      //
      Method (GPBD)
      {
        Return ( PBDP )
      }

      // GPDE (Get Processed Battery Data For _BST method)
      //
      // Arguments: (0)
      //   None
      // Return Value:
      //   An integer value with Processed Battery data
      //
      Method (GPDE)
      {
        Store (DeRefOf (Index (DeRefOf (Index (PBDP, 1)), 0)), Local1)
        Return ( Local1 )
      }
    } // End of TTEF

  } // End Battery Participant
}// end Scope (\_SB)
