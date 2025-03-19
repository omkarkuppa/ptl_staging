/**@file
  I2C Rtd3 Inclusion Table
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

@par Specification
**/


Method (PS0X,0,Serialized)
{
  ADBG (Concatenate ("I2c PS0 :",ToHexString(CNTL)))
}
/// PS3X Method, called by PS3 method in PchSerialIo.asl
Method (PS3X,0,Serialized)
{
  ADBG (Concatenate ("I2c PS3 :",ToHexString(CNTL)))
}

/// \ref i2c0_pr_pxtc
///@defgroup i2c0_scope  I2C1 Scope
If (EPDO & 0x1) {
  PowerResource (PXTC, 0, 0) {
    Method (_STA) {
      Return (PSTA(0))
    }
    Method (_ON) {
      ADBG (Concatenate ("I2c Touch Pad On Method :",ToHexString(CNTL)))
      PON (0)
    }
    Method (_OFF) {
      ADBG (Concatenate ("I2c Touch Pad Off Method :",ToHexString(CNTL)))
      POFF (0)
    }
  } // End PXTC
}
/// \ref i2c0_pr_ptpl
///@defgroup i2c0_scope
If (EPDO & 0x2) {
  PowerResource (PTPL, 0, 0) {
    Method (_STA) {
      Return (PSTA (1))
    }
    Method (_ON){
      ADBG (Concatenate ("I2c Touch Panel On Method :",ToHexString(CNTL)))
      PON (1)
    }
    Method (_OFF){
      ADBG (Concatenate ("I2c Touch Panel Off Method :",ToHexString(CNTL)))
      POFF (1)
    }
  }
}

/// Variable:
Name (ONTM, 0) ///ONTM: 0 - Not in Speculative ON ; Non-Zeroelapsed time in Nanosecs after Physical ON
Method (PSTA, 1, Serialized) {
  If (Arg0 == 0) {
    ADBG ("TPD _STA ON")
    Return (0x01)
  }
  If (Arg0 == 1) {
    If (LEqual (DeRefOf (Index (TPWR,0)), 0)) {
      ADBG ("TPL _STA is always ON")
      Return (0x01)
    }

    If (LEqual (\_SB.GGOV (DeRefOf (Index (TPWR,0))), 1)) {
      ADBG ("TPL _STA ON")
      Return (0x01)
    } Else {
      ADBG ("TPL _STA OFF")
      Return (0x00)
    }
  }
  Return (0x00)
}

// Timing of Power and Reset for I2C Touch Panel
//  _____________________________________________________________________________________________________
// | minimum required time |  Reset de-assert from Power On (ms)   |     Power Off from Reset assert (ms)|
// |_______________________|_______________________________________|_____________________________________|
// | Wacom                 |                0                      |                0                    | 
// |_______________________|_______________________________________|_____________________________________|  
// | Elan                  |                1                      |                2                    | 
// |_______________________|_______________________________________|_____________________________________|
// | current setting       |                2                      |                3                    |
// |_______________________|_______________________________________|_____________________________________|
// * no maximum time limitation.

Method (PON, 1, Serialized) /// _ON Method \n Turn on
{
  If (Arg0 == 0) {
    ADBG ("I2C Tpd PWR ON")
    // enable int line
    If (CondRefOf (TPDM)) {
      If (LEqual (TPDM,1)) {
        \_SB.SGRA (DeRefOf (Index (PIRT,0)), 1)
      }
    }
  }
  ElseIf (Arg0 == 1) {
    ADBG ("I2C Touch PWR ON")

    If (LNotEqual (DeRefOf (Index (TPWR,0)), 0)) {
      // drive pwr high
      \_SB.SGOV (DeRefOf (Index (TPWR,0)), DeRefOf (Index (TPWR,1)))
      Sleep (2)                  // time for Reset de-assert from Power
    }

      // De-Assert GPIO RST
    \_SB.SGOV (DeRefOf (Index(TRST,0)), DeRefOf (Index (TRST,1)))
      // update ONTM
      Store (Timer(), ONTM)
      // enable int line
      If (CondRefOf (TPLM)) {
        If (LEqual (TPLM,1)) {
          \_SB.SGRA (DeRefOf (Index (TIRT,0)), 1)
        }
      }
  }
}

Method (POFF, 1, Serialized)  /// _OFF method \n Turn off
{
  If (Arg0 == 0) {
    ADBG ("I2C Tpd PWR OFF")
    // disable int line
    If (CondRefOf (TPDM)) {
      If (LEqual (TPDM,1)) {
        \_SB.SGRA (DeRefOf (Index (PIRT,0)), 0)
      }
    }
  }
  ElseIf (Arg0 == 1) {
    ADBG ("I2C Touch PWR OFF")
    // disable int line
    If (CondRefOf (TPLM)) {
      If (LEqual (TPLM,1)) {
        \_SB.SGRA (DeRefOf (Index (TIRT,0)), 0)
      }
    }
    // Assert GPIO RST
    Xor (DeRefOf (Index (TRST,1)), 1, Local0)
    \_SB.SGOV (DeRefOf (Index (TRST,0)), Local0)

    If (LNotEqual (DeRefOf (Index (TPWR,0)), 0)) {
      Sleep(3)                   // time for Power Off from Reset assert
      // drive pwr low
      Xor (DeRefOf (Index (TPWR,1)), 1, Local0)
      \_SB.SGOV (DeRefOf (Index (TPWR,0)), Local0)
    }

    // update ONTM
    Store (Zero , ONTM)  ///- Clear ONTM
  }
}

If (EPDO & 0x1) {
  If (CondRefOf (TPD0)) {
    Scope (TPD0) {
      Name (TD_N, "TPD0")                                  // TD_N -   Touch Device name string
      Name (TD_P, Package () {I2C_ACPI_DEV.PXTC})          // TD_P - Touch   Device Power Packag
      Alias (IC0D, TD_D)                                   // TD_D -   Touch Device power on delay
      Alias (I2C_ACPI_DEV.ONTM, TD_C)                      // TD_C - Touch   Device I2C controller power on timestam
      Include ("Rtd3I2cTouchDev.asl")
      Method (_PS0) { PS0X() }
      Method (_PS3) { PS3X() }
    } // End Of Scope(TPD0)
  }
}

If (EPDO & 0x2) {
  If (CondRefOf (TPL1)) {
    Scope (TPL1) {
      Name (TD_N, "TPL1")                                      // TD_N -   Touch Device name
      Name (TD_P, Package() {I2C_ACPI_DEV.PTPL})               // TD_P - Touch   Device Power Package
      Alias (IC1D, TD_D)                                       // TD_D -   Touch Device power on delay
      Alias (I2C_ACPI_DEV.ONTM, TD_C)                          // TD_C - Touch   Device I2C controller power ontimestamp
      Include ("Rtd3I2cTouchDev.asl")
      Method (_PS0) { PS0X() }
      Method (_PS3) { PS3X() }
    } // End Of Scope(TPL1)
  }
}

