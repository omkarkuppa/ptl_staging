/** @file
  ACPI RTD3 I2C Touch Panel Power resource and PRxx, PSxx
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

External(TPIN)
External(TPBR)
External(TPCT)
External(TPTY)
//
// Touch panel Pins
//
External(TPDI)
External(TPDP)
External(TPLS)
External (VRRD,FieldUnitObj)
External(\_SB.GHPO, MethodObj)
External(\_SB.SHPO, MethodObj)

// GPIO methods
External (\_SB.SGRA, MethodObj)
External (\_SB.GGOV, MethodObj)
External (\_SB.SGOV, MethodObj)
External (\_SB.SGII, MethodObj)

If (LAnd( LAnd( LNotEqual (TPTY,0), LLess (TPTY,0xFF)), LEqual (TRT3,1))) {

  Name(ONTM, 0) ///ONTM: 0 - Not in Speculative ON ; Non-Zero - elapsed time in Nanosecs after Physical ON
  Alias(TPDL, TD_D)

// Timing of Power and Reset for I2C Touch Panel
//  _____________________________________________________________________________________________________
// | minimum required time |  Reset de-assert from Power On (ms)   |     Power Off from Reset assert (ms)|
// |_______________________|_______________________________________|_____________________________________|
// | current setting       |                2                      |                3                    |
// |_______________________|_______________________________________|_____________________________________|
// * no maximum time limitation.

        PowerResource(PXTC, 0, 0){
          Method(_STA){
            ADBG("I2C3 TPD _STA ON")
            Return(0x01)
          }

          Method(_ON){
            ADBG("I2C3 Tpd PWR ON")
            // enable int line
            If (LEqual (TPDM,1)) {
              \_SB.SGRA(TPDI, 1)
            }
          }

          Method(_OFF){
            ADBG("I2C3 Tpd PWR OFF")
            // disable int line
            If (LEqual (TPDM,1)) {
              \_SB.SGRA(TPDI, 0)
            }
          }
        } // End PXTC

  /// PS0X Method
  Method (PS0X,0,Serialized)
  {
    ADBG (Concatenate (Concatenate("TPD",TPLD_INDEX), " D0"))

    If (LEqual (ONTM,Zero))               /// Check if I2C controller is powered on
    {
      Return ()
    }

    ///
    /// Make sure "D0 delay" (TD_D) delay is elapsed before returning _PS0 \n
    /// Variables:
    ///- Local0: Elapse time since the _ON method \n
    ///- VRRD: VR Rampup Delay \n
    ///- TD_D: Time required for device to be ready after power on \n
    ///- Local1 = TD_D + VRRD: Need to incorporate VRRD since the _ON method no longer has VR Rampup Delay \n
    ///- So only need sleep for (Local1 - Local0), the amount of time remaining since the _ON method \n
    ///
    Divide (Subtract (Timer (), ONTM), 10000, , Local0) /// Store Elapsed time in ms, ignore remainder \n
    Add (TD_D, VRRD, Local1)              /// Incorporate VR Rampup Delay \n
    If (LLess (Local0, Local1))           /// Do not sleep if already past the delay requirement (TD_D + VRRD) for Touch Device \n
    {
      ///- Else, delay for power ramp
      Sleep (Subtract (Local1, Local0))   ///- Sleep (TD_D + VRRD - time elapsed)
    }

  ///@defgroup i2c_touchdev_ps0    I2C1 Touch Device _PS0 Method
  }

  Method (PS3X,0,Serialized)
  {
    ADBG (Concatenate (Concatenate("TPD",TPLD_INDEX), " D3"))
  ///@defgroup i2c_touchdev_ps3        I2C1 Touch Device _PS3 Method
  }

  Method (_PR0, Zero, NotSerialized, 0, PkgObj)    /// _PR0 Method
  {
     ADBG ("_STA PR0")

    If (LEqual (_STA (),0xF))             /// Check if Touch Device is enabled
    {
      Return (Package (){PXTC})                       ///- Return Device Specific Power Package (PTPL) if enabled
    } Else {
      Return (Package () {})              ///- Return NULL Power Package if disabled
    }

  ///@defgroup i2c_touchdev_pr0        I2C1 Touch Device _PR0 Method
  }

  Method (_PR3, Zero, NotSerialized, 0, PkgObj)    /// _PR3 Method
  {
     ADBG ("_STA PR3")
    If (LEqual (_STA (),0xF))             /// Check if Touch Device is enabled
    {
      Return (Package (){PXTC})                       ///- Return Device Specific Power Package (PTPL) if enabled
    } Else {
      Return (Package () {})              ///- Return NULL Power Package if disabled
    }

  ///@defgroup i2c_touchdev_pr3        I2C1 Touch Device _PR3 Method
  }

  Method(_PS0) { PS0X() }
  Method(_PS3) { PS3X() }
}